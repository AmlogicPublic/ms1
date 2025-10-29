"""
UART驱动程序
支持串口通信功能
基于USB转串口接口实现

Note: This driver must be used through USBDriverTop, not standalone.
"""

from typing import List, Tuple
from .dll_process_manager import DLLProcessManager

# UART奇偶校验位常量
UART_Parity_No = 0      # 无校验
UART_Parity_Odd = 1     # 奇校验
UART_Parity_Even = 2    # 偶校验

# UART停止位常量
UART_StopBits_1 = 0     # 1位停止位
UART_StopBits_1_5 = 1   # 1.5位停止位
UART_StopBits_2 = 2     # 2位停止位

class UARTDriver:
    """UART驱动程序类 - 必须通过USBDriverTop使用"""
    
    def __init__(self, manager, dll_name: str):
        """
        初始化UART驱动
        :param manager: 共享的DLL进程管理器（由USBDriverTop提供）
        :param dll_name: 共享的DLL名称（由USBDriverTop提供）
        """
        self._manager = manager
        self._dll_name = dll_name

        self.usb_opened = {}

    # ==================== UART操作 ====================
    
    def config_uart_param(self, baud_rate: int, parity: int = UART_Parity_No, 
                         stop_bits: int = UART_StopBits_1, usb_index: int = 0) -> bool:
        """
        配置UART参数
        :param baud_rate: 波特率 (如9600, 115200等)
        :param parity: 奇偶校验位 (UART_Parity_No/Odd/Even)
        :param stop_bits: 停止位 (UART_StopBits_1/1_5/2)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert baud_rate > 0, "波特率必须大于0"
        assert parity in [UART_Parity_No, UART_Parity_Odd, UART_Parity_Even], "无效的奇偶校验位"
        assert stop_bits in [UART_StopBits_1, UART_StopBits_1_5, UART_StopBits_2], "无效的停止位"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        result = self._manager.call(self._dll_name, "ConfigUARTParam", [baud_rate, parity, stop_bits, usb_index])
        return result == 0
    
    def send_data(self, data: List[int], usb_index: int = 0) -> bool:
        """
        发送数据
        :param data: 要发送的数据列表
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert len(data) > 0, "发送数据不能为空"
        assert all(0 <= b <= 255 for b in data), "数据值必须在0-255范围内"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（发送数组）
        args_spec = [
            {"type": "array", "data": data},
            len(data), usb_index
        ]
        
        result = self._manager.call(self._dll_name, "UARTSendData", args_spec)
        return result >= 0
    
    def receive_data(self, max_len: int, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        接收数据
        :param max_len: 最大接收长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 接收到的数据列表)
        """
        assert max_len > 0, "最大接收长度必须大于0"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（接收数组）
        args_spec = [
            {"type": "empty_array", "size": max_len},
            max_len, usb_index
        ]
        return_array_indices = [0]  # 接收数组在参数中的索引
        
        result, array_data = self._manager.call_with_arrays(self._dll_name, "UARTRcvData",
                                                            args_spec, return_array_indices)
        
        if result >= 0:
            return True, array_data[0][:result]
        return False, []
    
    def send_string(self, text: str, encoding: str = 'utf-8', usb_index: int = 0) -> bool:
        """
        发送字符串数据
        :param text: 要发送的字符串
        :param encoding: 字符编码格式
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        try:
            data_bytes = text.encode(encoding)
            data_list = list(data_bytes)
            return self.send_data(data_list, usb_index)
        except UnicodeEncodeError as e:
            raise ValueError(f"字符编码错误: {e}")
    
    def receive_string(self, max_len: int, encoding: str = 'utf-8', usb_index: int = 0) -> Tuple[bool, str]:
        """
        接收字符串数据
        :param max_len: 最大接收长度
        :param encoding: 字符编码格式
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 接收到的字符串)
        """
        success, data = self.receive_data(max_len, usb_index)
        if success and data:
            try:
                text = bytes(data).decode(encoding)
                return True, text
            except UnicodeDecodeError as e:
                print(f"字符解码错误: {e}")
                return False, ""
        return False, ""

    def open_usb(self, usb_index: int = 0) -> bool:
        """
        Open USB device for UART operations
        :param usb_index: USB index (0-99)
        :return: True if successful
        """
        assert 0 <= usb_index <= 99, "Invalid USB index"
        if usb_index in self.usb_opened:
            return True
        result = self._manager.call(self._dll_name, "OpenUsb", [usb_index])
        if result == 0:
            self.usb_opened[usb_index] = True
            return True
        return False

    def close_usb(self, usb_index: int = 0) -> bool:
        """
        Close USB device
        :param usb_index: USB index (0-99)
        :return: True if successful
        """
        assert 0 <= usb_index <= 99, "Invalid USB index"
        if usb_index not in self.usb_opened:
            return True
        result = self._manager.call(self._dll_name, "CloseUsb", [usb_index])
        if result == 0:
            self.usb_opened.pop(usb_index, None)
            return True
        return False

    # ==================== 辅助方法 ====================
    
    def __enter__(self):
        """上下文管理器入口"""
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """上下文管理器退出, 自动关闭所有打开的USB"""
        for usb_index in list(self.usb_opened.keys()):
            self.close_usb(usb_index)
    
    def close_all_usb(self):
        """关闭所有打开的USB设备"""
        for usb_index in list(self.usb_opened.keys()):
            self.close_usb(usb_index)


def main():
    """示例用法"""
    print("UART驱动程序示例")
    
    # 初始化驱动（需要提供DLL路径）
    manager = DLLProcessManager()
    driver = UARTDriver(manager, "usb_peripheral_dll")
    
    # 如果有DLL文件，可以这样加载：
    # manager.load_dll("usb_peripheral_dll", "path/to/USB2UARTSPIIICDLL.dll")
    # manager.set_function_prototype("usb_peripheral_dll", "OpenUsb", ["c_uint"], "c_int")
    # ... other prototypes ...
    
    try:
        print("\n=== UART通信示例 ===")
        
        # 1. 打开USB设备
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 2. 配置UART参数：9600波特率,无校验,1位停止位
            if driver.config_uart_param(9600, UART_Parity_No, UART_StopBits_1, 0):
                print("✓ UART参数配置成功 (9600, 8, N, 1)")
                
                # 3. 发送数据
                test_data = [0x48, 0x65, 0x6C, 0x6C, 0x6F]  # "Hello"
                if driver.send_data(test_data, 0):
                    print(f"✓ 发送数据成功: {test_data}")
                else:
                    print("✗ 发送数据失败")
                    
                # 4. 发送字符串
                if driver.send_string("Hello World!\r\n", usb_index=0):
                    print("✓ 发送字符串成功")
                else:
                    print("✗ 发送字符串失败")
                    
                # 5. 接收数据
                success, rcv_data = driver.receive_data(100, 0)
                if success and rcv_data:
                    print(f"✓ 接收数据成功: {rcv_data}")
                else:
                    print("○ 没有接收到数据")
                    
                # 6. 接收字符串
                success, rcv_text = driver.receive_string(100, usb_index=0)
                if success and rcv_text:
                    print(f"✓ 接收字符串成功: '{rcv_text}'")
                else:
                    print("○ 没有接收到字符串")
                    
            else:
                print("✗ UART参数配置失败")
                
            # 关闭USB
            if driver.close_usb(0):
                print("✓ USB设备关闭成功")
        else:
            print("✗ USB设备打开失败")
            
        print("\n=== 高波特率测试 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 测试115200波特率,偶校验,2位停止位
            if driver.config_uart_param(115200, UART_Parity_Even, UART_StopBits_2, 0):
                print("✓ 高速UART参数配置成功 (115200, 8, E, 2)")
                
                # 发送大量数据测试
                large_data = list(range(256))  # 0-255
                if driver.send_data(large_data, 0):
                    print("✓ 大数据块发送成功")
                else:
                    print("✗ 大数据块发送失败")
                    
            driver.close_usb(0)
            
    except Exception as e:
        print(f"✗ 发生错误: {e}")
    finally:
        # 确保关闭所有USB连接
        driver.close_all_usb()


if __name__ == "__main__":
    main()
