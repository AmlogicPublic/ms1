"""
PWM驱动程序
支持脉宽调制输出功能
基于USB转PWM接口实现

Note: This driver must be used through USBDriverTop, not standalone.
"""

from typing import List, Tuple
import time
import math
from .dll_process_manager import DLLProcessManager

class PWMDriver:
    """PWM驱动程序类 - 必须通过USBDriverTop使用"""
    
    def __init__(self, manager, dll_name: str):
        """
        初始化PWM驱动
        :param manager: 共享的DLL进程管理器（由USBDriverTop提供）
        :param dll_name: 共享的DLL名称（由USBDriverTop提供）
        """
        self._manager = manager
        self._dll_name = dll_name
        self.pwm_active = {}  # 记录PWM活动状态
        self.usb_opened = {}

    # ==================== PWM基础操作 ====================
    
    def pwm_out(self, prescaler: int, period: int, pulse: int, pulse_num: int, usb_index: int = 0) -> bool:
        """
        PWM输出
        :param prescaler: 预分频器值
        :param period: 周期值
        :param pulse: 脉冲宽度值
        :param pulse_num: 脉冲数量 (0=连续输出)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert prescaler > 0, "预分频器值必须大于0"
        assert period > 0, "周期值必须大于0"
        assert 0 <= pulse <= period, "脉冲宽度值必须在0到周期值之间"
        assert pulse_num >= 0, "脉冲数量必须大于等于0"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        result = self._manager.call(self._dll_name, "PWMOut", [prescaler, period, pulse, pulse_num, usb_index])
        
        if result == 0:
            self.pwm_active[usb_index] = True
            return True
        return False
    
    def close_pwm(self, usb_index: int = 0) -> bool:
        """
        关闭PWM输出
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        result = self._manager.call(self._dll_name, "PWMClose", [usb_index])
        
        if result == 0:
            self.pwm_active[usb_index] = False
            return True
        return False
    
    def is_pwm_active(self, usb_index: int = 0) -> bool:
        """检查PWM是否活动"""
        return self.pwm_active.get(usb_index, False)

    # ==================== 高级PWM操作 ====================
    
    def set_frequency_duty(self, frequency: float, duty_cycle: float, clock_freq: int = 48000000, pulse_num: int = 0, usb_index: int = 0) -> bool:
        """
        根据频率和占空比设置PWM
        :param frequency: PWM频率 (Hz)
        :param duty_cycle: 占空比 (0.0-1.0)
        :param clock_freq: 时钟频率 (Hz, 默认48MHz)
        :param pulse_num: 脉冲数量 (0=连续输出)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert frequency > 0, "频率必须大于0"
        assert 0.0 <= duty_cycle <= 1.0, "占空比必须在0.0-1.0之间"
        assert clock_freq > 0, "时钟频率必须大于0"
        
        # 计算最佳的预分频器和周期值
        target_period_ticks = clock_freq / frequency
        
        # 寻找合适的预分频器值 (1-65536)
        best_prescaler = 1
        best_period = int(target_period_ticks)
        min_error = float('inf')
        
        for prescaler in range(1, 65537):
            period = int(target_period_ticks / prescaler)
            if period < 1 or period > 65535:
                continue
                
            actual_freq = clock_freq / (prescaler * period)
            error = abs(frequency - actual_freq)
            
            if error < min_error:
                min_error = error
                best_prescaler = prescaler
                best_period = period
        
        if best_period < 1:
            return False
            
        pulse = int(best_period * duty_cycle)
        
        return self.pwm_out(best_prescaler, best_period, pulse, pulse_num, usb_index)
    
    def set_servo_angle(self, angle: float, pulse_num: int = 0, usb_index: int = 0) -> bool:
        """
        设置舵机角度 (标准舵机: -90°到+90°)
        :param angle: 角度 (-90.0 到 +90.0)
        :param pulse_num: 脉冲数量 (0=连续输出)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert -90.0 <= angle <= 90.0, "角度必须在-90到+90度之间"
        
        # 标准舵机PWM参数：20ms周期,1-2ms脉宽
        # -90度: 1ms脉宽, 0度: 1.5ms脉宽, +90度: 2ms脉宽
        pulse_width_ms = 1.5 + (angle / 90.0) * 0.5  # 1.0-2.0ms
        
        # 50Hz (20ms周期)
        return self.set_frequency_duty_width(50.0, pulse_width_ms, pulse_num, usb_index)
    
    def set_frequency_duty_width(self, frequency: float, pulse_width_ms: float, pulse_num: int = 0, usb_index: int = 0) -> bool:
        """
        根据频率和脉冲宽度设置PWM
        :param frequency: PWM频率 (Hz)
        :param pulse_width_ms: 脉冲宽度 (毫秒)
        :param pulse_num: 脉冲数量 (0=连续输出)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert frequency > 0, "频率必须大于0"
        assert pulse_width_ms > 0, "脉冲宽度必须大于0"
        
        period_ms = 1000.0 / frequency
        assert pulse_width_ms <= period_ms, "脉冲宽度不能大于周期"
        
        duty_cycle = pulse_width_ms / period_ms
        return self.set_frequency_duty(frequency, duty_cycle, pulse_num=pulse_num, usb_index=usb_index)
    
    def generate_tone(self, frequency: float, duration: float, volume: float = 0.5, usb_index: int = 0) -> bool:
        """
        生成蜂鸣器音调
        :param frequency: 音调频率 (Hz)
        :param duration: 持续时间 (秒)
        :param volume: 音量 (0.0-1.0, 影响占空比)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert frequency > 0, "频率必须大于0"
        assert duration > 0, "持续时间必须大于0"
        assert 0.0 <= volume <= 1.0, "音量必须在0.0-1.0之间"
        
        # 计算脉冲数量
        pulse_count = int(frequency * duration)
        
        # 根据音量调整占空比
        duty_cycle = 0.5 * volume
        
        return self.set_frequency_duty(frequency, duty_cycle, pulse_num=pulse_count, usb_index=usb_index)
    
    def play_melody(self, notes: List[Tuple[float, float]], volume: float = 0.5, usb_index: int = 0) -> bool:
        """
        播放旋律
        :param notes: 音符列表 [(频率Hz, 持续时间s), ...]
        :param volume: 音量 (0.0-1.0)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        for frequency, duration in notes:
            if frequency > 0:  # 0频率表示休止符
                if not self.generate_tone(frequency, duration, volume, usb_index):
                    return False
            else:
                # 休止符：关闭PWM并等待
                self.close_pwm(usb_index)
                time.sleep(duration)
                
        # 播放完成后关闭PWM
        return self.close_pwm(usb_index)
    
    def led_breathing(self, frequency: float, cycles: int, usb_index: int = 0) -> bool:
        """
        LED呼吸灯效果
        :param frequency: 呼吸频率 (Hz)
        :param cycles: 呼吸周期数
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert frequency > 0, "频率必须大于0"
        assert cycles > 0, "周期数必须大于0"
        
        pwm_freq = 1000.0  # 1kHz PWM频率
        steps = 100  # 呼吸步骤数
        step_time = (1.0 / frequency) / (2 * steps)  # 每步时间
        
        for cycle in range(cycles):
            # 逐渐变亮
            for step in range(steps):
                duty = step / steps
                pulse_num = int(pwm_freq * step_time)
                if not self.set_frequency_duty(pwm_freq, duty, pulse_num=pulse_num, usb_index=usb_index):
                    return False
                time.sleep(step_time)
                
            # 逐渐变暗
            for step in range(steps, 0, -1):
                duty = step / steps
                pulse_num = int(pwm_freq * step_time)
                if not self.set_frequency_duty(pwm_freq, duty, pulse_num=pulse_num, usb_index=usb_index):
                    return False
                time.sleep(step_time)
        
        return self.close_pwm(usb_index)
    
    def motor_speed_control(self, speed_percent: float, direction: str = 'forward', usb_index: int = 0) -> bool:
        """
        电机速度控制
        :param speed_percent: 速度百分比 (0.0-100.0)
        :param direction: 方向 ('forward', 'reverse', 'brake')
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert 0.0 <= speed_percent <= 100.0, "速度百分比必须在0-100之间"
        assert direction in ['forward', 'reverse', 'brake'], "方向必须是'forward', 'reverse'或'brake'"
        
        if direction == 'brake':
            return self.close_pwm(usb_index)
            
        # 通常电机控制使用1kHz PWM
        frequency = 1000.0
        duty_cycle = speed_percent / 100.0
        
        # 对于反向,可能需要额外的逻辑来控制方向引脚
        # 这里只是示例,实际应用可能需要配合IO控制
        
        return self.set_frequency_duty(frequency, duty_cycle, pulse_num=0, usb_index=usb_index)

    # ==================== 便利方法 ====================
    
    def get_pwm_parameters(self, frequency: float, duty_cycle: float, clock_freq: int = 48000000) -> Tuple[int, int, int]:
        """
        计算PWM参数
        :param frequency: 目标频率 (Hz)
        :param duty_cycle: 占空比 (0.0-1.0)
        :param clock_freq: 时钟频率 (Hz)
        :return: (预分频器, 周期, 脉冲宽度)
        """
        target_period_ticks = clock_freq / frequency
        
        best_prescaler = 1
        best_period = int(target_period_ticks)
        min_error = float('inf')
        
        for prescaler in range(1, 65537):
            period = int(target_period_ticks / prescaler)
            if period < 1 or period > 65535:
                continue
                
            actual_freq = clock_freq / (prescaler * period)
            error = abs(frequency - actual_freq)
            
            if error < min_error:
                min_error = error
                best_prescaler = prescaler
                best_period = period
        
        pulse = int(best_period * duty_cycle)
        return best_prescaler, best_period, pulse
    
    def calculate_actual_frequency(self, prescaler: int, period: int, clock_freq: int = 48000000) -> float:
        """
        计算实际频率
        :param prescaler: 预分频器值
        :param period: 周期值
        :param clock_freq: 时钟频率 (Hz)
        :return: 实际频率 (Hz)
        """
        return clock_freq / (prescaler * period)
    
    def calculate_duty_cycle(self, period: int, pulse: int) -> float:
        """
        计算占空比
        :param period: 周期值
        :param pulse: 脉冲宽度值
        :return: 占空比 (0.0-1.0)
        """
        return pulse / period if period > 0 else 0.0

    def open_usb(self, usb_index: int = 0) -> bool:
        """
        Open USB device for PWM operations
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
        Close USB device and stop PWM if active
        :param usb_index: USB index (0-99)
        :return: True if successful
        """
        assert 0 <= usb_index <= 99, "Invalid USB index"
        if usb_index not in self.usb_opened:
            return True
        
        # Close PWM if active
        if self.is_pwm_active(usb_index):
            self.close_pwm(usb_index)
        
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
        """上下文管理器退出,自动关闭所有打开的USB和PWM"""
        for usb_index in list(self.usb_opened.keys()):
            if self.is_pwm_active(usb_index):
                self.close_pwm(usb_index)
            self.close_usb(usb_index)
    
    def close_all_usb(self):
        """关闭所有打开的USB设备和PWM"""
        for usb_index in list(self.usb_opened.keys()):
            if self.is_pwm_active(usb_index):
                self.close_pwm(usb_index)
            self.close_usb(usb_index)


def main():
    """示例用法"""
    print("PWM驱动程序示例")
    
    # 初始化驱动（需要提供DLL路径）
    manager = DLLProcessManager()
    driver = PWMDriver(manager, "usb_peripheral_dll")
    
    # 如果有DLL文件，可以这样加载：
    # manager.load_dll("usb_peripheral_dll", "path/to/USB2UARTSPIIICDLL.dll")
    # manager.set_function_prototype("usb_peripheral_dll", "OpenUsb", ["c_uint"], "c_int")
    # ... other prototypes ...
    
    # 定义常用音符频率
    NOTES = {
        'C4': 261.63, 'D4': 293.66, 'E4': 329.63, 'F4': 349.23,
        'G4': 392.00, 'A4': 440.00, 'B4': 493.88, 'C5': 523.25
    }
    
    try:
        print("\n=== PWM基本操作示例 ===")
        
        # 1. 打开USB设备
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 2. 基础PWM输出
            print("开始基础PWM输出...")
            if driver.pwm_out(prescaler=1000, period=1000, pulse=500, pulse_num=0, usb_index=0):
                print("✓ PWM输出开始 (50%占空比)")
                time.sleep(2)
                
                # 关闭PWM
                if driver.close_pwm(0):
                    print("✓ PWM输出已关闭")
                else:
                    print("✗ PWM关闭失败")
            else:
                print("✗ PWM输出失败")
                
            # 3. 根据频率和占空比设置PWM
            print("\n设置1kHz, 25%占空比的PWM...")
            if driver.set_frequency_duty(1000.0, 0.25, pulse_num=0, usb_index=0):
                print("✓ PWM参数设置成功")
                time.sleep(2)
                driver.close_pwm(0)
            else:
                print("✗ PWM参数设置失败")
                
            # 关闭USB
            if driver.close_usb(0):
                print("✓ USB设备关闭成功")
        else:
            print("✗ USB设备打开失败")
            
        print("\n=== 舵机控制示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 舵机角度控制
            angles = [-90, -45, 0, 45, 90, 0]
            for angle in angles:
                print(f"设置舵机角度: {angle}°")
                if driver.set_servo_angle(angle, pulse_num=50, usb_index=0):
                    print(f"✓ 舵机转到{angle}°")
                    time.sleep(1)
                else:
                    print(f"✗ 舵机角度{angle}°设置失败")
                    
            driver.close_pwm(0)
            driver.close_usb(0)
            
        print("\n=== 蜂鸣器音调示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 单音调测试
            print("播放440Hz音调 (1秒)...")
            if driver.generate_tone(440.0, 1.0, volume=0.5, usb_index=0):
                print("✓ 音调播放完成")
            else:
                print("✗ 音调播放失败")
                
            time.sleep(0.5)
                
            # 播放简单旋律 (C大调音阶)
            print("播放C大调音阶...")
            melody = [
                (NOTES['C4'], 0.5), (NOTES['D4'], 0.5), (NOTES['E4'], 0.5), (NOTES['F4'], 0.5),
                (NOTES['G4'], 0.5), (NOTES['A4'], 0.5), (NOTES['B4'], 0.5), (NOTES['C5'], 0.5)
            ]
            
            if driver.play_melody(melody, volume=0.3, usb_index=0):
                print("✓ 旋律播放完成")
            else:
                print("✗ 旋律播放失败")
                
            driver.close_usb(0)
            
        print("\n=== LED呼吸灯示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            print("开始LED呼吸灯效果 (0.5Hz, 3个周期)...")
            if driver.led_breathing(0.5, 3, usb_index=0):
                print("✓ 呼吸灯效果完成")
            else:
                print("✗ 呼吸灯效果失败")
                
            driver.close_usb(0)
            
        print("\n=== 电机控制示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 电机速度控制
            speeds = [25, 50, 75, 100, 50, 0]
            for speed in speeds:
                if speed > 0:
                    print(f"设置电机速度: {speed}%")
                    if driver.motor_speed_control(speed, 'forward', usb_index=0):
                        print(f"✓ 电机速度设置为{speed}%")
                    else:
                        print(f"✗ 电机速度{speed}%设置失败")
                else:
                    print("停止电机")
                    if driver.motor_speed_control(0, 'brake', usb_index=0):
                        print("✓ 电机已停止")
                    else:
                        print("✗ 电机停止失败")
                        
                time.sleep(1)
                
            driver.close_usb(0)
            
        print("\n=== PWM参数计算示例 ===")
        
        # 计算PWM参数
        freq = 1000.0
        duty = 0.3
        prescaler, period, pulse = driver.get_pwm_parameters(freq, duty)
        actual_freq = driver.calculate_actual_frequency(prescaler, period)
        actual_duty = driver.calculate_duty_cycle(period, pulse)
        
        print(f"目标频率: {freq}Hz, 目标占空比: {duty*100:.1f}%")
        print(f"计算参数: 预分频={prescaler}, 周期={period}, 脉宽={pulse}")
        print(f"实际频率: {actual_freq:.2f}Hz, 实际占空比: {actual_duty*100:.1f}%")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            print("使用计算的参数输出PWM...")
            if driver.pwm_out(prescaler, period, pulse, pulse_num=5000, usb_index=0):
                print("✓ 精确PWM输出完成")
            else:
                print("✗ 精确PWM输出失败")
                
            driver.close_usb(0)
            
    except Exception as e:
        print(f"✗ 发生错误: {e}")
    finally:
        # 确保关闭所有USB连接和PWM
        driver.close_all_usb()


if __name__ == "__main__":
    main()
