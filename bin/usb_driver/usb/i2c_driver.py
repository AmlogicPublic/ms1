"""
I2C驱动程序
支持I2C主从模式通信
基于USB转I2C接口实现

Note: This driver must be used through USBDriverTop, not standalone.
"""

from typing import List, Tuple

# I2C速率常量
IIC_Rate_1K = 0       # 1K
IIC_Rate_5K = 1       # 5K
IIC_Rate_10K = 2      # 10K
IIC_Rate_20K = 3      # 20K
IIC_Rate_50K = 4      # 50K
IIC_Rate_80K = 5      # 80K
IIC_Rate_100K = 6     # 100K
IIC_Rate_200K = 7     # 200K
IIC_Rate_400K = 8     # 400K
IIC_Rate_600K = 9     # 600K
IIC_Rate_800K = 10    # 800K
IIC_Rate_1M = 11      # 1M

# I2C寻址模式
IIC_ADDRMOD_7BIT = 0   # 7位地址
IIC_ADDRMOD_10BIT = 1  # 10位地址

# 数据传输长度限制
MAX_DATA_LEN_BASIC = 1024    # 基础版和多电压版
MAX_DATA_LEN_FAST_I2C = 2048 # 快速版I2C

class I2CDriver:
    """I2C驱动程序类 - 必须通过USBDriverTop使用"""
    
    def __init__(self, manager, dll_name: str):
        """
        初始化I2C驱动
        :param manager: 共享的DLL进程管理器（由USBDriverTop提供）
        :param dll_name: 共享的DLL名称（由USBDriverTop提供）
        """
        self._manager = manager
        self._dll_name = dll_name

    # ==================== I2C主模式操作 ====================
    
    def config_master_param(self, rate: int, clk_stretch: int = 0, usb_index: int = 0) -> bool:
        """
        设置I2C主模式参数
        :param rate: I2C时钟频率 (IIC_Rate_1K~IIC_Rate_1M)
        :param clk_stretch: 时钟延展等待周期数 (0-0xFFFFFFFF, 0xFFFFFFFF=无限等待)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        valid_rates = [IIC_Rate_1K, IIC_Rate_5K, IIC_Rate_10K, IIC_Rate_20K, IIC_Rate_50K,
                      IIC_Rate_80K, IIC_Rate_100K, IIC_Rate_200K, IIC_Rate_400K,
                      IIC_Rate_600K, IIC_Rate_800K, IIC_Rate_1M]
        assert rate in valid_rates, "无效的I2C速率"
        assert 0 <= clk_stretch <= 0xFFFFFFFF, "时钟延展值超出范围"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        result = self._manager.call(self._dll_name, "ConfigIICParam", [rate, clk_stretch, usb_index])
        return result == 0
    
    def master_send_and_rcv_data(self, start_bit: int, stop_bit: int, send_buf: List[int],
                                rcv_len: int, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        I2C主模式发送和接收数据
        :param start_bit: START信号 (0=不加Start, 1=加Start)
        :param stop_bit: STOP信号 (0=不加Stop, 1=加Stop)
        :param send_buf: 发送数据列表
        :param rcv_len: 接收数据长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 接收到的数据列表)
        """
        assert start_bit in [0, 1], "START信号必须为0或1"
        assert stop_bit in [0, 1], "STOP信号必须为0或1"
        assert len(send_buf) > 0, "发送缓存不能为空"
        assert rcv_len > 0, "接收长度必须大于0"
        assert len(send_buf) + rcv_len <= MAX_DATA_LEN_FAST_I2C, f"发送和接收数据总长度不能超过{MAX_DATA_LEN_FAST_I2C}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（发送数组 + 接收数组）
        args_spec = [
            start_bit, stop_bit,
            {"type": "array", "data": send_buf},           # 发送数组
            {"type": "empty_array", "size": rcv_len},      # 接收数组
            len(send_buf), rcv_len, usb_index
        ]
        return_array_indices = [3]  # 接收数组在参数中的索引
        
        result, array_data = self._manager.call_with_arrays(self._dll_name, "IICSendAndRcvData",
                                                            args_spec, return_array_indices)
        
        if result >= 0:
            return True, array_data[0][:result]
        return False, []
    
    def master_send_data(self, start_bit: int, stop_bit: int, send_buf: List[int], usb_index: int = 0) -> bool:
        """
        I2C主模式发送数据
        :param start_bit: START信号 (0=不加Start, 1=加Start)
        :param stop_bit: STOP信号 (0=不加Stop, 1=加Stop)
        :param send_buf: 发送数据列表
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert start_bit in [0, 1], "START信号必须为0或1"
        assert stop_bit in [0, 1], "STOP信号必须为0或1"
        assert len(send_buf) > 0, "发送缓存不能为空"
        assert len(send_buf) <= MAX_DATA_LEN_FAST_I2C, f"发送数据长度不能超过{MAX_DATA_LEN_FAST_I2C}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（发送数组）
        args_spec = [
            start_bit, stop_bit,
            {"type": "array", "data": send_buf},
            len(send_buf), usb_index
        ]
        
        result = self._manager.call(self._dll_name, "IICSendData", args_spec)
        return result >= 0
    
    def master_rcv_data(self, stop_bit: int, rcv_len: int, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        I2C主模式读取数据
        :param stop_bit: STOP信号 (0=不加Stop, 1=加Stop)
        :param rcv_len: 读取数据长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 接收到的数据列表)
        """
        assert stop_bit in [0, 1], "STOP信号必须为0或1"
        assert rcv_len > 0, "接收长度必须大于0"
        assert rcv_len <= MAX_DATA_LEN_FAST_I2C, f"接收数据长度不能超过{MAX_DATA_LEN_FAST_I2C}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（接收数组）
        args_spec = [
            stop_bit,
            {"type": "empty_array", "size": rcv_len},
            rcv_len, usb_index
        ]
        return_array_indices = [1]  # 接收数组在参数中的索引
        
        result, array_data = self._manager.call_with_arrays(self._dll_name, "IICRcvData",
                                                            args_spec, return_array_indices)
        
        if result >= 0:
            return True, array_data[0][:result]
        return False, []
    
    def check_slave_address(self, addr_mode: int, addr: int, usb_index: int = 0) -> bool:
        """
        检查I2C从机地址
        :param addr_mode: 寻址模式 (IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT)
        :param addr: 从机地址
        :param usb_index: USB索引值 (0-99)
        :return: True=地址有效, False=地址无效
        """
        assert addr_mode in [IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT], "无效的寻址模式"
        if addr_mode == IIC_ADDRMOD_7BIT:
            assert 0 <= addr <= 0x7F, "7位地址必须在0-0x7F范围内"
        else:
            assert 0 <= addr <= 0x3FF, "10位地址必须在0-0x3FF范围内"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        result = self._manager.call(self._dll_name, "IICCheckSlaveAddr", [addr_mode, addr, usb_index])
        
        if result == 1:
            return True   # 检测到从机地址
        elif result == 0:
            return False  # 没有检测到从机地址
        elif result == -1:
            raise RuntimeError("USB设备未打开")
        else:
            raise RuntimeError(f"I2C协议错误,错误码: {result}")
    
    def register_send(self, addr_mode: int, addr: int, reg_buf: List[int], send_buf: List[int], usb_index: int = 0) -> bool:
        """
        I2C寄存器写入
        :param addr_mode: 寻址模式 (IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT)
        :param addr: 从机地址
        :param reg_buf: 寄存器地址数据
        :param send_buf: 要写入的数据
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert addr_mode in [IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT], "无效的寻址模式"
        if addr_mode == IIC_ADDRMOD_7BIT:
            assert 0 <= addr <= 0x7F, "7位地址必须在0-0x7F范围内"
        else:
            assert 0 <= addr <= 0x3FF, "10位地址必须在0-0x3FF范围内"
        assert len(reg_buf) > 0, "寄存器地址不能为空"
        assert len(send_buf) > 0, "发送数据不能为空"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（寄存器数组 + 发送数组）
        args_spec = [
            addr_mode, addr,
            {"type": "array", "data": reg_buf},
            {"type": "array", "data": send_buf},
            len(reg_buf), len(send_buf), usb_index
        ]
        
        result = self._manager.call(self._dll_name, "IICRegisterSend", args_spec)
        return result >= 0
    
    def register_read(self, addr_mode: int, addr: int, reg_buf: List[int], rcv_len: int, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        I2C寄存器读取
        :param addr_mode: 寻址模式 (IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT)
        :param addr: 从机地址
        :param reg_buf: 寄存器地址数据
        :param rcv_len: 要读取的数据长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 读取到的数据列表)
        """
        assert addr_mode in [IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT], "无效的寻址模式"
        if addr_mode == IIC_ADDRMOD_7BIT:
            assert 0 <= addr <= 0x7F, "7位地址必须在0-0x7F范围内"
        else:
            assert 0 <= addr <= 0x3FF, "10位地址必须在0-0x3FF范围内"
        assert len(reg_buf) > 0, "寄存器地址不能为空"
        assert rcv_len > 0, "接收长度必须大于0"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（寄存器数组 + 接收数组）
        args_spec = [
            addr_mode, addr,
            {"type": "array", "data": reg_buf},
            {"type": "empty_array", "size": rcv_len},
            len(reg_buf), rcv_len, usb_index
        ]
        return_array_indices = [3]  # 接收数组在参数中的索引
        
        result, array_data = self._manager.call_with_arrays(self._dll_name, "IICRegisterRead",
                                                            args_spec, return_array_indices)
        
        if result >= 0:
            return True, array_data[0][:result]
        return False, []
    
    def direct_send(self, addr_mode: int, addr: int, send_buf: List[int], usb_index: int = 0) -> bool:
        """
        I2C直接发送数据
        :param addr_mode: 寻址模式 (IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT)
        :param addr: 从机地址
        :param send_buf: 要发送的数据
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert addr_mode in [IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT], "无效的寻址模式"
        if addr_mode == IIC_ADDRMOD_7BIT:
            assert 0 <= addr <= 0x7F, "7位地址必须在0-0x7F范围内"
        else:
            assert 0 <= addr <= 0x3FF, "10位地址必须在0-0x3FF范围内"
        assert len(send_buf) > 0, "发送数据不能为空"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（发送数组）
        args_spec = [
            addr_mode, addr,
            {"type": "array", "data": send_buf},
            len(send_buf), usb_index
        ]
        
        result = self._manager.call(self._dll_name, "IICDirectSend", args_spec)
        return result >= 0
    
    def direct_read(self, addr_mode: int, addr: int, rcv_len: int, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        I2C直接读取数据
        :param addr_mode: 寻址模式 (IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT)
        :param addr: 从机地址
        :param rcv_len: 要读取的数据长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 读取到的数据列表)
        """
        assert addr_mode in [IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT], "无效的寻址模式"
        if addr_mode == IIC_ADDRMOD_7BIT:
            assert 0 <= addr <= 0x7F, "7位地址必须在0-0x7F范围内"
        else:
            assert 0 <= addr <= 0x3FF, "10位地址必须在0-0x3FF范围内"
        assert rcv_len > 0, "接收长度必须大于0"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（接收数组）
        args_spec = [
            addr_mode, addr,
            {"type": "empty_array", "size": rcv_len},
            rcv_len, usb_index
        ]
        return_array_indices = [2]  # 接收数组在参数中的索引
        
        result, array_data = self._manager.call_with_arrays(self._dll_name, "IICDirectRead",
                                                            args_spec, return_array_indices)
        
        if result >= 0:
            return True, array_data[0][:result]
        return False, []

    # ==================== I2C从模式操作 ====================
    
    def config_slave_param(self, addr_mode: int, addr: int, usb_index: int = 0) -> bool:
        """
        设置I2C从模式参数
        :param addr_mode: 寻址模式 (IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT)
        :param addr: 本机地址
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert addr_mode in [IIC_ADDRMOD_7BIT, IIC_ADDRMOD_10BIT], "无效的寻址模式"
        if addr_mode == IIC_ADDRMOD_7BIT:
            assert 0 <= addr <= 0x7F, "7位地址必须在0-0x7F范围内"
        else:
            assert 0 <= addr <= 0x3FF, "10位地址必须在0-0x3FF范围内"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        result = self._manager.call(self._dll_name, "ConfigIICParamSlave", [addr_mode, addr, usb_index])
        return result == 0
    
    def slave_preload_data(self, data_buf: List[int], usb_index: int = 0) -> bool:
        """
        I2C从模式预装数据
        :param data_buf: 预装数据列表
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert len(data_buf) > 0, "预装数据不能为空"
        assert len(data_buf) <= MAX_DATA_LEN_FAST_I2C, f"预装数据长度不能超过{MAX_DATA_LEN_FAST_I2C}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（预装数据数组）
        args_spec = [
            {"type": "array", "data": data_buf},
            len(data_buf), usb_index
        ]
        
        result = self._manager.call(self._dll_name, "IICSlavePreloadData", args_spec)
        return result == 0
    
    def slave_rcv_data(self, max_len: int, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        I2C从模式读取数据
        :param max_len: 最大读取长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 接收到的数据列表)
        """
        assert max_len > 0, "最大读取长度必须大于0"
        assert max_len <= MAX_DATA_LEN_FAST_I2C, f"最大读取长度不能超过{MAX_DATA_LEN_FAST_I2C}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"
        
        # 准备参数规范（接收数组）
        args_spec = [
            {"type": "empty_array", "size": max_len},
            max_len, usb_index
        ]
        return_array_indices = [0]  # 接收数组在参数中的索引
        
        result, array_data = self._manager.call_with_arrays(self._dll_name, "IICSlaveRcvData",
                                                            args_spec, return_array_indices)
        
        if result >= 0:
            return True, array_data[0][:result]
        return False, []

    # ==================== 便利方法 ====================
    
    def read_eeprom(self, chip_addr: int, mem_addr: int, data_len: int, addr_16bit: bool = False, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        读取EEPROM数据
        :param chip_addr: EEPROM芯片地址 (7位地址)
        :param mem_addr: 内存地址
        :param data_len: 读取数据长度
        :param addr_16bit: 是否使用16位内存地址
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 读取到的数据列表)
        """
        if addr_16bit:
            reg_buf = [(mem_addr >> 8) & 0xFF, mem_addr & 0xFF]
        else:
            reg_buf = [mem_addr & 0xFF]
            
        return self.register_read(IIC_ADDRMOD_7BIT, chip_addr, reg_buf, data_len, usb_index)
    
    def write_eeprom(self, chip_addr: int, mem_addr: int, data_buf: List[int], addr_16bit: bool = False, usb_index: int = 0) -> bool:
        """
        写入EEPROM数据
        :param chip_addr: EEPROM芯片地址 (7位地址)
        :param mem_addr: 内存地址
        :param data_buf: 要写入的数据
        :param addr_16bit: 是否使用16位内存地址
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        if addr_16bit:
            reg_buf = [(mem_addr >> 8) & 0xFF, mem_addr & 0xFF]
        else:
            reg_buf = [mem_addr & 0xFF]
            
        return self.register_send(IIC_ADDRMOD_7BIT, chip_addr, reg_buf, data_buf, usb_index)

