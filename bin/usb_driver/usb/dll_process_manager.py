"""
通用DLL进程管理器
将DLL操作隔离到独立进程中执行,内置锁机制确保原子性
完全通用,不包含任何特定外设的驱动逻辑
"""

import ctypes
import multiprocessing as mp
import threading
import queue
from typing import Any, Dict, Optional, List, Tuple, Union


def dll_worker_process(request_queue, response_queue):
    """通用DLL工作进程"""
    dlls: Dict[str, ctypes.CDLL] = {}
    func_prototypes: Dict[str, Dict[str, Tuple]] = {}  # {dll_name: {func_name: (argtypes, restype)}}
    dll_lock = threading.Lock()  # 进程内锁,确保DLL调用原子性

    # 导入ctypes类型
    from ctypes import c_ubyte, c_uint, POINTER

    def _parse_prototype(proto_desc):
        """解析字符串形式的函数原型描述"""
        type_map = {
            'c_uint': c_uint,
            'c_ubyte': c_ubyte,
            'c_int': ctypes.c_int,
            'POINTER_c_ubyte': POINTER(c_ubyte),
        }

        argtypes_desc, restype_desc = proto_desc
        argtypes = [type_map[t] for t in argtypes_desc] if argtypes_desc else []
        restype = type_map[restype_desc] if restype_desc else None

        return argtypes, restype

    def _rebuild_args(args_spec):
        """在工作进程中重建ctypes参数"""
        rebuilt_args = []
        array_objects = []

        for arg_spec in args_spec:
            if isinstance(arg_spec, dict):
                if arg_spec["type"] == "array":
                    # 重建ctypes数组
                    data = arg_spec["data"]
                    array_type = c_ubyte * len(data)
                    array_obj = array_type(*data)
                    rebuilt_args.append(array_obj)
                    array_objects.append((array_obj, len(data)))
                elif arg_spec["type"] == "empty_array":
                    # 创建空数组用于接收数据
                    size = arg_spec["size"]
                    array_type = c_ubyte * size
                    array_obj = array_type()
                    rebuilt_args.append(array_obj)
                    array_objects.append((array_obj, size))
            else:
                # 简单参数
                rebuilt_args.append(arg_spec)
                array_objects.append(None)

        return rebuilt_args, array_objects

    while True:
        try:
            request = request_queue.get(timeout=1.0)
            if request is None:  # 停止信号
                break

            cmd, args = request

            if cmd == "load_dll":
                dll_name, dll_path = args
                try:
                    with dll_lock:
                        dll = ctypes.WinDLL(dll_path)
                        dlls[dll_name] = dll
                        func_prototypes[dll_name] = {}
                        response_queue.put(("success", None))
                except Exception as e:
                    response_queue.put(("error", str(e)))

            elif cmd == "set_func_prototype":
                dll_name, func_name, proto_desc = args
                try:
                    with dll_lock:
                        if dll_name in dlls:
                            argtypes, restype = _parse_prototype(proto_desc)
                            func = getattr(dlls[dll_name], func_name)
                            func.argtypes = argtypes
                            func.restype = restype
                            func_prototypes[dll_name][func_name] = (argtypes, restype)
                            response_queue.put(("success", None))
                        else:
                            response_queue.put(("error", f"DLL {dll_name} not loaded"))
                except Exception as e:
                    response_queue.put(("error", str(e)))

            elif cmd == "call_dll":
                dll_name, func_name, args_spec = args
                try:
                    with dll_lock:
                        dll = dlls[dll_name]
                        func = getattr(dll, func_name)

                        # 重建参数
                        rebuilt_args, _ = _rebuild_args(args_spec)

                        result = func(*rebuilt_args)
                        response_queue.put(("success", result))
                except Exception as e:
                    response_queue.put(("error", str(e)))

            elif cmd == "call_dll_with_arrays":
                dll_name, func_name, args_spec, return_array_indices = args
                try:
                    with dll_lock:
                        dll = dlls[dll_name]
                        func = getattr(dll, func_name)

                        # 重建参数
                        rebuilt_args, array_objects = _rebuild_args(args_spec)

                        # 调用函数
                        result = func(*rebuilt_args)

                        # 提取需要返回的数组数据
                        return_arrays = []
                        for idx in return_array_indices:
                            if idx < len(array_objects) and array_objects[idx] is not None:
                                array_obj, array_len = array_objects[idx]
                                return_arrays.append(list(array_obj[:array_len]))
                            else:
                                return_arrays.append([])

                        response_queue.put(("success", (result, return_arrays)))

                except Exception as e:
                    response_queue.put(("error", str(e)))

        except queue.Empty:
            continue
        except Exception as e:
            response_queue.put(("error", str(e)))


class DLLProcessManager:
    """通用DLL进程管理器,内置锁机制"""

    def __init__(self):
        self._request_queue = mp.Queue()
        self._response_queue = mp.Queue()
        self._process = mp.Process(
            target=dll_worker_process,
            args=(self._request_queue, self._response_queue),
            daemon=True
        )
        self._process.start()

    def load_dll(self, name: str, path: str):
        """
        加载DLL到工作进程
        :param name: DLL名称
        :param path: DLL路径
        """
        self._request_queue.put(("load_dll", (name, path)))
        status, result = self._response_queue.get(timeout=10.0)
        if status == "error":
            raise RuntimeError(f"加载DLL失败: {result}")

    def set_function_prototype(self, dll_name: str, func_name: str, argtypes_desc: List[str], restype_desc: str):
        """
        设置函数原型
        :param dll_name: DLL名称
        :param func_name: 函数名
        :param argtypes_desc: 参数类型描述列表 (如 ['c_uint', 'c_ubyte'])
        :param restype_desc: 返回类型描述 (如 'c_int')
        """
        proto_desc = (argtypes_desc, restype_desc)
        self._request_queue.put(("set_func_prototype", (dll_name, func_name, proto_desc)))
        status, result = self._response_queue.get(timeout=5.0)
        if status == "error":
            raise RuntimeError(f"设置函数原型失败: {result}")

    def call(self, dll_name: str, func_name: str, args_spec: List[Union[Any, Dict]]) -> Any:
        """
        调用DLL函数（简单调用）
        :param dll_name: DLL名称
        :param func_name: 函数名
        :param args_spec: 参数规范,支持简单值和字典描述的数组
        """
        self._request_queue.put(("call_dll", (dll_name, func_name, args_spec)))
        status, result = self._response_queue.get(timeout=30.0)
        if status == "error":
            raise RuntimeError(f"DLL调用失败: {result}")
        return result

    def call_with_arrays(self, dll_name: str, func_name: str, args_spec: List[Union[Any, Dict]],
                         return_array_indices: List[int]) -> Tuple[Any, List[List[int]]]:
        """
        调用DLL函数并返回数组数据
        :param dll_name: DLL名称
        :param func_name: 函数名
        :param args_spec: 参数规范,支持简单值和字典描述的数组
        :param return_array_indices: 需要返回的数组参数索引列表
        """
        self._request_queue.put(("call_dll_with_arrays", (dll_name, func_name, args_spec, return_array_indices)))
        status, result = self._response_queue.get(timeout=30.0)
        if status == "error":
            raise RuntimeError(f"DLL调用失败: {result}")
        return result

    def shutdown(self):
        """关闭进程管理器"""
        try:
            self._request_queue.put(None)
            self._process.join(timeout=5.0)
        except:
            pass
        finally:
            if self._process.is_alive():
                self._process.terminate()


# 全局单例
_manager: Optional[DLLProcessManager] = None


def get_manager() -> DLLProcessManager:
    """获取全局进程管理器"""
    global _manager
    if _manager is None:
        _manager = DLLProcessManager()
    return _manager


def shutdown_manager():
    """关闭全局进程管理器"""
    global _manager
    if _manager is not None:
        _manager.shutdown()
        _manager = None
