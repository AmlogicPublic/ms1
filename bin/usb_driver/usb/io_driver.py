"""
IO驱动程序
支持数字IO控制功能
基于USB转数字IO接口实现

Note: This driver must be used through USBDriverTop, not standalone.
"""

from typing import List, Tuple, Dict
import time
from .dll_process_manager import DLLProcessManager

# IO方向常量
IO_DIR_INPUT = 0   # 输入
IO_DIR_OUTPUT = 1  # 输出

# IO电平常量
IO_LEVEL_LOW = 0   # 低电平
IO_LEVEL_HIGH = 1  # 高电平

class IODriver:
    """IO驱动程序类 - 必须通过USBDriverTop使用"""
    
    def __init__(self, manager, dll_name: str):
        """
        初始化IO驱动
        :param manager: 共享的DLL进程管理器（由USBDriverTop提供）
        :param dll_name: 共享的DLL名称（由USBDriverTop提供）
        """
        self._manager = manager
        self._dll_name = dll_name
        self.io_config = {}  # 记录IO配置
        self.usb_opened = {}

    # ==================== IO基础操作 ====================
    
    def io_set_and_read(self, io_num: int, io_dir: int, io_bit: int, usb_index: int = 0) -> Tuple[bool, int]:
        """
        IO设置和读取
        :param io_num: IO编号
        :param io_dir: IO方向 (IO_DIR_INPUT, IO_DIR_OUTPUT)
        :param io_bit: IO电平 (IO_LEVEL_LOW, IO_LEVEL_HIGH, 用于输出)
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 读取到的IO状态)
        """
        assert io_num >= 0, "IO编号必须大于等于0"
        assert io_dir in [IO_DIR_INPUT, IO_DIR_OUTPUT], "无效的IO方向"
        assert io_bit in [IO_LEVEL_LOW, IO_LEVEL_HIGH], "无效的IO电平"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        result = self._manager.call(self._dll_name, "IOSetAndRead", [io_num, io_dir, io_bit, usb_index])
        
        # 记录IO配置
        if usb_index not in self.io_config:
            self.io_config[usb_index] = {}
        self.io_config[usb_index][io_num] = {'dir': io_dir, 'bit': io_bit}
        
        if result >= 0:
            return True, result
        return False, 0
    
    def set_output(self, io_num: int, level: int, usb_index: int = 0) -> bool:
        """
        设置IO输出电平
        :param io_num: IO编号
        :param level: 输出电平 (IO_LEVEL_LOW, IO_LEVEL_HIGH)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        success, _ = self.io_set_and_read(io_num, IO_DIR_OUTPUT, level, usb_index)
        return success
    
    def read_input(self, io_num: int, usb_index: int = 0) -> Tuple[bool, int]:
        """
        读取IO输入状态
        :param io_num: IO编号
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 输入电平)
        """
        return self.io_set_and_read(io_num, IO_DIR_INPUT, IO_LEVEL_LOW, usb_index)
    
    def toggle_output(self, io_num: int, usb_index: int = 0) -> bool:
        """
        切换IO输出电平
        :param io_num: IO编号
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        # 先读取当前状态
        success, current_level = self.read_input(io_num, usb_index)
        if success:
            new_level = IO_LEVEL_HIGH if current_level == IO_LEVEL_LOW else IO_LEVEL_LOW
            return self.set_output(io_num, new_level, usb_index)
        return False

    # ==================== 批量IO操作 ====================
    
    def set_multiple_outputs(self, io_levels: Dict[int, int], usb_index: int = 0) -> bool:
        """
        设置多个IO输出
        :param io_levels: IO编号和电平的字典 {io_num: level}
        :param usb_index: USB索引值 (0-99)
        :return: True=全部成功, False=有失败
        """
        all_success = True
        for io_num, level in io_levels.items():
            if not self.set_output(io_num, level, usb_index):
                all_success = False
        return all_success
    
    def read_multiple_inputs(self, io_nums: List[int], usb_index: int = 0) -> Tuple[bool, Dict[int, int]]:
        """
        读取多个IO输入
        :param io_nums: IO编号列表
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, IO编号和电平的字典)
        """
        results = {}
        all_success = True
        
        for io_num in io_nums:
            success, level = self.read_input(io_num, usb_index)
            if success:
                results[io_num] = level
            else:
                all_success = False
                
        return all_success, results
    
    def get_io_config(self, usb_index: int = 0) -> Dict[int, Dict]:
        """
        获取IO配置信息
        :param usb_index: USB索引值 (0-99)
        :return: IO配置字典
        """
        return self.io_config.get(usb_index, {}).copy()

    # ==================== 高级IO操作 ====================
    
    def pulse_output(self, io_num: int, pulse_width: float, active_level: int = IO_LEVEL_HIGH, usb_index: int = 0) -> bool:
        """
        输出脉冲信号
        :param io_num: IO编号
        :param pulse_width: 脉冲宽度(秒)
        :param active_level: 有效电平
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        inactive_level = IO_LEVEL_LOW if active_level == IO_LEVEL_HIGH else IO_LEVEL_HIGH
        
        # 设置为无效电平
        if not self.set_output(io_num, inactive_level, usb_index):
            return False
        
        # 设置为有效电平
        if not self.set_output(io_num, active_level, usb_index):
            return False
        
        # 等待脉冲宽度
        time.sleep(pulse_width)
        
        # 设置回无效电平
        return self.set_output(io_num, inactive_level, usb_index)
    
    def blink_led(self, io_num: int, count: int, on_time: float = 0.5, off_time: float = 0.5, usb_index: int = 0) -> bool:
        """
        LED闪烁控制
        :param io_num: IO编号
        :param count: 闪烁次数
        :param on_time: 点亮时间(秒)
        :param off_time: 熄灭时间(秒)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        for i in range(count):
            # 点亮LED
            if not self.set_output(io_num, IO_LEVEL_HIGH, usb_index):
                return False
            time.sleep(on_time)
            
            # 熄灭LED
            if not self.set_output(io_num, IO_LEVEL_LOW, usb_index):
                return False
                
            if i < count - 1:  # 最后一次不需要等待off_time
                time.sleep(off_time)
        
        return True
    
    def wait_for_input(self, io_num: int, target_level: int, timeout: float = 10.0, usb_index: int = 0) -> bool:
        """
        等待IO输入变为指定电平
        :param io_num: IO编号
        :param target_level: 目标电平
        :param timeout: 超时时间(秒)
        :param usb_index: USB索引值 (0-99)
        :return: True=达到目标电平, False=超时
        """
        start_time = time.time()
        
        while time.time() - start_time < timeout:
            success, current_level = self.read_input(io_num, usb_index)
            if success and current_level == target_level:
                return True
            time.sleep(0.01)  # 10ms轮询间隔
            
        return False
    
    def monitor_inputs(self, io_nums: List[int], duration: float, interval: float = 0.1, usb_index: int = 0) -> Tuple[bool, List[Tuple[float, Dict[int, int]]]]:
        """
        监控IO输入变化
        :param io_nums: 要监控的IO编号列表
        :param duration: 监控持续时间(秒)
        :param interval: 采样间隔(秒)
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 时间戳和IO状态的列表)
        """
        data_points = []
        start_time = time.time()
        
        while time.time() - start_time < duration:
            current_time = time.time() - start_time
            success, io_states = self.read_multiple_inputs(io_nums, usb_index)
            
            if success:
                data_points.append((current_time, io_states.copy()))
            else:
                return False, []
                
            time.sleep(interval)
        
        return True, data_points
    
    def detect_input_change(self, io_num: int, timeout: float = 10.0, usb_index: int = 0) -> Tuple[bool, int, int]:
        """
        检测IO输入电平变化
        :param io_num: IO编号
        :param timeout: 超时时间(秒)
        :param usb_index: USB索引值 (0-99)
        :return: (检测到变化, 初始电平, 最终电平)
        """
        # 读取初始电平
        success, initial_level = self.read_input(io_num, usb_index)
        if not success:
            return False, 0, 0
            
        start_time = time.time()
        
        while time.time() - start_time < timeout:
            success, current_level = self.read_input(io_num, usb_index)
            if success and current_level != initial_level:
                return True, initial_level, current_level
            time.sleep(0.001)  # 1ms轮询间隔
            
        return False, initial_level, initial_level

    # ==================== 模式控制 ====================
    
    def pwm_software(self, io_num: int, frequency: float, duty_cycle: float, duration: float, usb_index: int = 0) -> bool:
        """
        软件PWM输出
        :param io_num: IO编号
        :param frequency: 频率(Hz)
        :param duty_cycle: 占空比(0.0-1.0)
        :param duration: 持续时间(秒)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        if not (0.0 <= duty_cycle <= 1.0):
            return False
            
        period = 1.0 / frequency
        on_time = period * duty_cycle
        off_time = period * (1.0 - duty_cycle)
        
        start_time = time.time()
        
        while time.time() - start_time < duration:
            # 高电平
            if not self.set_output(io_num, IO_LEVEL_HIGH, usb_index):
                return False
            time.sleep(on_time)
            
            # 低电平
            if not self.set_output(io_num, IO_LEVEL_LOW, usb_index):
                return False
            time.sleep(off_time)
        
        return True
    
    def button_with_debounce(self, io_num: int, debounce_time: float = 0.05, usb_index: int = 0) -> Tuple[bool, bool]:
        """
        带消抖的按键检测
        :param io_num: IO编号
        :param debounce_time: 消抖时间(秒)
        :param usb_index: USB索引值 (0-99)
        :return: (检测成功, 按键是否被按下)
        """
        # 读取初始状态
        success, initial_state = self.read_input(io_num, usb_index)
        if not success:
            return False, False
            
        # 等待消抖时间
        time.sleep(debounce_time)
        
        # 再次读取状态
        success, final_state = self.read_input(io_num, usb_index)
        if not success:
            return False, False
            
        # 如果状态稳定且为低电平（按键按下）,返回True
        pressed = (initial_state == final_state) and (final_state == IO_LEVEL_LOW)
        return True, pressed

    def open_usb(self, usb_index: int = 0) -> bool:
        """
        Open USB device for IO operations
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
        """上下文管理器退出,自动关闭所有打开的USB"""
        for usb_index in list(self.usb_opened.keys()):
            self.close_usb(usb_index)
    
    def close_all_usb(self):
        """关闭所有打开的USB设备"""
        for usb_index in list(self.usb_opened.keys()):
            self.close_usb(usb_index)


def main():
    """示例用法"""
    print("IO驱动程序示例")
    
    # 初始化驱动（需要提供DLL路径）
    manager = DLLProcessManager()
    driver = IODriver(manager, "usb_peripheral_dll")
    
    # 如果有DLL文件，可以这样加载：
    # manager.load_dll("usb_peripheral_dll", "path/to/USB2UARTSPIIICDLL.dll")
    # manager.set_function_prototype("usb_peripheral_dll", "OpenUsb", ["c_uint"], "c_int")
    # ... other prototypes ...

    try:
        print("\n=== IO基本操作示例 ===")
        
        # 1. 打开USB设备
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 2. 设置IO输出
            if driver.set_output(0, IO_LEVEL_HIGH, 0):
                print("✓ IO0设置为高电平")
            else:
                print("✗ IO0设置失败")
                
            if driver.set_output(1, IO_LEVEL_LOW, 0):
                print("✓ IO1设置为低电平")
            else:
                print("✗ IO1设置失败")
                
            # 3. 读取IO输入
            success, level = driver.read_input(2, 0)
            if success:
                level_str = "高电平" if level == IO_LEVEL_HIGH else "低电平"
                print(f"✓ IO2输入状态: {level_str}")
            else:
                print("✗ IO2读取失败")
                
            # 4. 切换IO输出
            if driver.toggle_output(0, 0):
                print("✓ IO0电平已切换")
            else:
                print("✗ IO0切换失败")
                
            # 5. 批量设置IO
            io_settings = {0: IO_LEVEL_LOW, 1: IO_LEVEL_HIGH, 3: IO_LEVEL_HIGH}
            if driver.set_multiple_outputs(io_settings, 0):
                print("✓ 批量IO设置成功")
            else:
                print("✗ 批量IO设置失败")
                
            # 6. 批量读取IO
            success, io_states = driver.read_multiple_inputs([2, 4, 5], 0)
            if success:
                print("✓ 批量IO读取成功:")
                for io_num, level in io_states.items():
                    level_str = "高" if level == IO_LEVEL_HIGH else "低"
                    print(f"   IO{io_num}: {level_str}电平")
            else:
                print("✗ 批量IO读取失败")
                
            # 关闭USB
            if driver.close_usb(0):
                print("✓ USB设备关闭成功")
        else:
            print("✗ USB设备打开失败")
            
        print("\n=== LED控制示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # LED闪烁
            print("开始LED闪烁测试 (IO0)...")
            if driver.blink_led(0, count=5, on_time=0.2, off_time=0.2, usb_index=0):
                print("✓ LED闪烁完成")
            else:
                print("✗ LED闪烁失败")
                
            # 脉冲输出
            print("输出脉冲信号 (IO1)...")
            if driver.pulse_output(1, pulse_width=0.5, active_level=IO_LEVEL_HIGH, usb_index=0):
                print("✓ 脉冲输出完成")
            else:
                print("✗ 脉冲输出失败")
                
            driver.close_usb(0)
            
        print("\n=== 软件PWM示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            print("开始软件PWM输出 (IO0, 10Hz, 30%占空比, 持续3秒)...")
            if driver.pwm_software(0, frequency=10.0, duty_cycle=0.3, duration=3.0, usb_index=0):
                print("✓ 软件PWM输出完成")
            else:
                print("✗ 软件PWM输出失败")
                
            driver.close_usb(0)
            
        print("\n=== 输入监控示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 等待输入变化
            print("等待IO2输入变为高电平 (10秒超时)...")
            if driver.wait_for_input(2, IO_LEVEL_HIGH, timeout=3.0, usb_index=0):
                print("✓ 检测到IO2变为高电平")
            else:
                print("○ IO2输入等待超时")
                
            # 检测输入变化
            print("监控IO2输入变化 (3秒超时)...")
            success, initial, final = driver.detect_input_change(2, timeout=3.0, usb_index=0)
            if success:
                initial_str = "高" if initial == IO_LEVEL_HIGH else "低"
                final_str = "高" if final == IO_LEVEL_HIGH else "低"
                print(f"✓ 检测到变化: {initial_str}电平 -> {final_str}电平")
            else:
                print("○ 未检测到输入变化")
                
            # 按键检测
            print("按键检测测试 (IO2)...")
            success, pressed = driver.button_with_debounce(2, debounce_time=0.05, usb_index=0)
            if success:
                if pressed:
                    print("✓ 检测到按键按下")
                else:
                    print("○ 按键未按下")
            else:
                print("✗ 按键检测失败")
                
            # IO监控
            print("监控多个IO输入 (持续2秒)...")
            success, monitor_data = driver.monitor_inputs([2, 3], duration=2.0, interval=0.1, usb_index=0)
            if success:
                print(f"✓ 监控完成,共采集{len(monitor_data)}个数据点")
                # 显示前3个数据点
                for i, (timestamp, states) in enumerate(monitor_data[:3]):
                    states_str = ', '.join([f"IO{io}={'H' if level else 'L'}" for io, level in states.items()])
                    print(f"   {timestamp:.1f}s: {states_str}")
            else:
                print("✗ IO监控失败")
                
            # 显示IO配置
            config = driver.get_io_config(0)
            if config:
                print("✓ 当前IO配置:")
                for io_num, cfg in config.items():
                    dir_str = "输出" if cfg['dir'] == IO_DIR_OUTPUT else "输入"
                    bit_str = "高" if cfg['bit'] == IO_LEVEL_HIGH else "低"
                    print(f"   IO{io_num}: {dir_str}, {bit_str}电平")
                    
            driver.close_usb(0)
            
    except Exception as e:
        print(f"✗ 发生错误: {e}")
    finally:
        # 确保关闭所有USB连接
        driver.close_all_usb()


if __name__ == "__main__":
    main()
