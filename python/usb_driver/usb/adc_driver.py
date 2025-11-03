"""
ADC驱动程序
支持模拟信号采集功能
基于USB转ADC接口实现

Note: This driver must be used through USBDriverTop, not standalone.
"""

from typing import List, Tuple
import time
import statistics
from .dll_process_manager import DLLProcessManager

class ADCDriver:
    """ADC驱动程序类 - 必须通过USBDriverTop使用"""
    
    def __init__(self, manager, dll_name: str):
        """
        初始化ADC驱动
        :param manager: 共享的DLL进程管理器（由USBDriverTop提供）
        :param dll_name: 共享的DLL名称（由USBDriverTop提供）
        """
        self._manager = manager
        self._dll_name = dll_name

        self.usb_opened = {}

    # ==================== ADC操作 ====================
    
    def get_adc_value(self, channel: int, usb_index: int = 0) -> Tuple[bool, int]:
        """
        获取ADC值
        :param channel: ADC通道号 (通常0-7)
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, ADC原始值)
        """
        assert 0 <= channel <= 255, "ADC通道号必须在0-255范围内"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        result = self._manager.call(self._dll_name, "GetADCVal", [channel, usb_index])
        
        if result >= 0:
            return True, result
        return False, 0
    
    def get_adc_voltage(self, channel: int, vref: float = 3.3, resolution: int = 12, usb_index: int = 0) -> Tuple[bool, float]:
        """
        获取ADC电压值
        :param channel: ADC通道号
        :param vref: 参考电压 (V)
        :param resolution: ADC分辨率 (位数)
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 电压值)
        """
        success, raw_value = self.get_adc_value(channel, usb_index)
        if success:
            max_value = (1 << resolution) - 1  # 2^resolution - 1
            voltage = (raw_value / max_value) * vref
            return True, voltage
        return False, 0.0
    
    def get_adc_percent(self, channel: int, usb_index: int = 0) -> Tuple[bool, float]:
        """
        获取ADC百分比值
        :param channel: ADC通道号
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 百分比值 0-100%)
        """
        success, raw_value = self.get_adc_value(channel, usb_index)
        if success:
            # 假设12位ADC,最大值4095
            percent = (raw_value / 4095.0) * 100.0
            return True, percent
        return False, 0.0
    
    def read_multiple_samples(self, channel: int, sample_count: int, interval: float = 0.001, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        读取多个ADC样本
        :param channel: ADC通道号
        :param sample_count: 采样次数
        :param interval: 采样间隔(秒)
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, ADC值列表)
        """
        assert sample_count > 0, "采样次数必须大于0"
        assert interval >= 0, "采样间隔必须大于等于0"
        
        samples = []
        for i in range(sample_count):
            success, value = self.get_adc_value(channel, usb_index)
            if success:
                samples.append(value)
            else:
                return False, []
                
            if i < sample_count - 1:  # 最后一次不需要延时
                time.sleep(interval)
        
        return True, samples
    
    def get_average_value(self, channel: int, sample_count: int = 10, interval: float = 0.001, usb_index: int = 0) -> Tuple[bool, float]:
        """
        获取平均ADC值
        :param channel: ADC通道号
        :param sample_count: 采样次数
        :param interval: 采样间隔(秒)
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 平均值)
        """
        success, samples = self.read_multiple_samples(channel, sample_count, interval, usb_index)
        if success and samples:
            return True, statistics.mean(samples)
        return False, 0.0
    
    def get_statistics(self, channel: int, sample_count: int = 100, interval: float = 0.001, usb_index: int = 0) -> Tuple[bool, dict]:
        """
        获取ADC统计信息
        :param channel: ADC通道号
        :param sample_count: 采样次数
        :param interval: 采样间隔(秒)
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 统计信息字典)
        """
        success, samples = self.read_multiple_samples(channel, sample_count, interval, usb_index)
        if success and samples:
            stats = {
                'count': len(samples),
                'min': min(samples),
                'max': max(samples),
                'mean': statistics.mean(samples),
                'median': statistics.median(samples),
                'stdev': statistics.stdev(samples) if len(samples) > 1 else 0,
                'range': max(samples) - min(samples)
            }
            return True, stats
        return False, {}
    
    def monitor_channel(self, channel: int, duration: float, interval: float = 0.1, usb_index: int = 0) -> Tuple[bool, List[Tuple[float, int]]]:
        """
        监控ADC通道
        :param channel: ADC通道号
        :param duration: 监控持续时间(秒)
        :param interval: 采样间隔(秒)
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 时间戳和ADC值的列表)
        """
        assert duration > 0, "监控持续时间必须大于0"
        assert interval > 0, "采样间隔必须大于0"
        
        data_points = []
        start_time = time.time()
        
        while time.time() - start_time < duration:
            current_time = time.time() - start_time
            success, value = self.get_adc_value(channel, usb_index)
            
            if success:
                data_points.append((current_time, value))
            else:
                return False, []
                
            time.sleep(interval)
        
        return True, data_points
    
    def calibrate_channel(self, channel: int, known_voltages: List[float], vref: float = 3.3, usb_index: int = 0) -> Tuple[bool, dict]:
        """
        校准ADC通道
        :param channel: ADC通道号
        :param known_voltages: 已知电压值列表
        :param vref: 参考电压
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 校准参数字典)
        """
        if len(known_voltages) < 2:
            return False, {}
            
        measured_values = []
        for voltage in known_voltages:
            print(f"请将{voltage}V电压连接到通道{channel},然后按Enter继续...")
            input()
            
            success, avg_value = self.get_average_value(channel, 50, 0.01, usb_index)
            if success:
                measured_values.append(avg_value)
            else:
                return False, {}
        
        # 计算线性校准参数 (y = ax + b)
        n = len(known_voltages)
        sum_x = sum(measured_values)
        sum_y = sum(known_voltages)
        sum_xy = sum(x * y for x, y in zip(measured_values, known_voltages))
        sum_x2 = sum(x * x for x in measured_values)
        
        # 最小二乘法计算斜率和截距
        a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x)
        b = (sum_y - a * sum_x) / n
        
        calibration_params = {
            'slope': a,
            'intercept': b,
            'measured_values': measured_values,
            'known_voltages': known_voltages
        }
        
        return True, calibration_params
    
    def apply_calibration(self, raw_value: int, calibration_params: dict) -> float:
        """
        应用校准参数
        :param raw_value: 原始ADC值
        :param calibration_params: 校准参数
        :return: 校准后的电压值
        """
        if 'slope' in calibration_params and 'intercept' in calibration_params:
            return calibration_params['slope'] * raw_value + calibration_params['intercept']
        return 0.0

    # ==================== 多通道操作 ====================
    
    def read_all_channels(self, channel_count: int = 8, usb_index: int = 0) -> Tuple[bool, dict]:
        """
        读取所有ADC通道
        :param channel_count: 通道数量
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 通道值字典)
        """
        channel_values = {}
        
        for ch in range(channel_count):
            success, value = self.get_adc_value(ch, usb_index)
            if success:
                channel_values[f'CH{ch}'] = value
            else:
                return False, {}
                
        return True, channel_values
    
    def compare_channels(self, channels: List[int], usb_index: int = 0) -> Tuple[bool, dict]:
        """
        比较多个ADC通道
        :param channels: 要比较的通道列表
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 比较结果字典)
        """
        if len(channels) < 2:
            return False, {}
            
        channel_values = {}
        for ch in channels:
            success, value = self.get_adc_value(ch, usb_index)
            if success:
                channel_values[ch] = value
            else:
                return False, {}
        
        comparison = {
            'values': channel_values,
            'min_channel': min(channel_values, key=lambda k: channel_values[k]),
            'max_channel': max(channel_values, key=lambda k: channel_values[k]),
            'min_value': min(channel_values.values()),
            'max_value': max(channel_values.values()),
            'range': max(channel_values.values()) - min(channel_values.values())
        }
        
        return True, comparison

    def open_usb(self, usb_index: int = 0) -> bool:
        """
        Open USB device for ADC operations
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
    print("ADC驱动程序示例")
    
    # 初始化驱动（需要提供DLL路径）
    manager = DLLProcessManager()
    driver = ADCDriver(manager, "usb_peripheral_dll")
    
    # 如果有DLL文件，可以这样加载：
    # manager.load_dll("usb_peripheral_dll", "path/to/USB2UARTSPIIICDLL.dll")
    # manager.set_function_prototype("usb_peripheral_dll", "OpenUsb", ["c_uint"], "c_int")
    # ... other prototypes ...

    try:
        print("\n=== ADC基本操作示例 ===")
        
        # 1. 打开USB设备
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 2. 读取单个ADC值
            success, adc_value = driver.get_adc_value(0, 0)
            if success:
                print(f"✓ ADC通道0原始值: {adc_value}")
                
                # 转换为电压值 (假设3.3V参考电压,12位ADC)
                success, voltage = driver.get_adc_voltage(0, vref=3.3, resolution=12, usb_index=0)
                if success:
                    print(f"✓ ADC通道0电压值: {voltage:.3f}V")
                    
                # 转换为百分比
                success, percent = driver.get_adc_percent(0, 0)
                if success:
                    print(f"✓ ADC通道0百分比: {percent:.1f}%")
            else:
                print("✗ ADC读取失败")
                
            # 3. 读取平均值
            success, avg_value = driver.get_average_value(0, sample_count=20, interval=0.01, usb_index=0)
            if success:
                print(f"✓ ADC通道0平均值 (20次采样): {avg_value:.1f}")
            else:
                print("✗ ADC平均值读取失败")
                
            # 4. 获取统计信息
            success, stats = driver.get_statistics(0, sample_count=50, interval=0.005, usb_index=0)
            if success:
                print("✓ ADC通道0统计信息:")
                print(f"   最小值: {stats['min']}")
                print(f"   最大值: {stats['max']}")
                print(f"   平均值: {stats['mean']:.2f}")
                print(f"   中位数: {stats['median']:.2f}")
                print(f"   标准差: {stats['stdev']:.2f}")
                print(f"   范围: {stats['range']}")
            else:
                print("✗ ADC统计信息获取失败")
                
            # 关闭USB
            if driver.close_usb(0):
                print("✓ USB设备关闭成功")
        else:
            print("✗ USB设备打开失败")
            
        print("\n=== 多通道ADC示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 读取所有通道
            success, all_channels = driver.read_all_channels(channel_count=4, usb_index=0)
            if success:
                print("✓ 所有ADC通道值:")
                for ch, value in all_channels.items():
                    voltage = (value / 4095.0) * 3.3  # 转换为电压
                    print(f"   {ch}: {value} ({voltage:.3f}V)")
            else:
                print("✗ 多通道ADC读取失败")
                
            # 比较多个通道
            success, comparison = driver.compare_channels([0, 1, 2], 0)
            if success:
                print("✓ 通道比较结果:")
                print(f"   最小值通道: CH{comparison['min_channel']} = {comparison['min_value']}")
                print(f"   最大值通道: CH{comparison['max_channel']} = {comparison['max_value']}")
                print(f"   值范围: {comparison['range']}")
            else:
                print("✗ 通道比较失败")
                
            driver.close_usb(0)
            
        print("\n=== ADC监控示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            print("开始监控ADC通道0,持续3秒...")
            success, monitor_data = driver.monitor_channel(0, duration=3.0, interval=0.1, usb_index=0)
            if success:
                print(f"✓ 监控完成,共采集{len(monitor_data)}个数据点")
                # 显示前5个和后5个数据点
                for i, (timestamp, value) in enumerate(monitor_data[:5]):
                    voltage = (value / 4095.0) * 3.3
                    print(f"   {timestamp:.1f}s: {value} ({voltage:.3f}V)")
                    
                if len(monitor_data) > 10:
                    print("   ...")
                    for i, (timestamp, value) in enumerate(monitor_data[-5:]):
                        voltage = (value / 4095.0) * 3.3
                        print(f"   {timestamp:.1f}s: {value} ({voltage:.3f}V)")
            else:
                print("✗ ADC监控失败")
                
            driver.close_usb(0)
            
        print("\n=== 高精度采样示例 ===")
        
        if driver.open_usb(0):
            print("✓ USB设备打开成功")
            
            # 高频采样
            print("执行高频采样测试...")
            success, samples = driver.read_multiple_samples(0, sample_count=1000, interval=0.001, usb_index=0)
            if success:
                print(f"✓ 高频采样完成,采集{len(samples)}个样本")
                min_val, max_val = min(samples), max(samples)
                avg_val = sum(samples) / len(samples)
                print(f"   最小值: {min_val}")
                print(f"   最大值: {max_val}")
                print(f"   平均值: {avg_val:.2f}")
                print(f"   噪声范围: {max_val - min_val}")
            else:
                print("✗ 高频采样失败")
                
            driver.close_usb(0)
            
    except Exception as e:
        print(f"✗ 发生错误: {e}")
    finally:
        # 确保关闭所有USB连接
        driver.close_all_usb()


if __name__ == "__main__":
    main()
