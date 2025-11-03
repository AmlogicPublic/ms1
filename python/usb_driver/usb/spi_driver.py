"""
SPI驱动程序
支持SPI主从模式通信
基于USB转SPI接口实现

Note: This driver must be used through USBDriverTop, not standalone.
"""

from typing import List, Tuple
from .dll_process_manager import DLLProcessManager

# SPI速率常量
SPI_Rate_281K = 0     # 281K
SPI_Rate_562K = 1     # 562K
SPI_Rate_1_125M = 2   # 1.125M
SPI_Rate_2_25M = 3    # 2.25M
SPI_Rate_4_5M = 4     # 4.5M
SPI_Rate_9M = 5       # 9M
SPI_Rate_18M = 6      # 18M
SPI_Rate_36M = 7      # 36M

# SPI帧格式常量
SPI_MSB = 0  # 高位在前
SPI_LSB = 1  # 低位在前

# SPI时钟模式常量
SPI_SubMode_0 = 0  # CPOL=0,CPHA=0
SPI_SubMode_1 = 1  # CPOL=0,CPHA=1
SPI_SubMode_2 = 2  # CPOL=1,CPHA=0
SPI_SubMode_3 = 3  # CPOL=1,CPHA=1

# 数据传输长度限制
MAX_DATA_LEN_BASIC = 1024    # 基础版和多电压版
MAX_DATA_LEN_FAST_SPI = 8192  # 快速版SPI
MAX_DATA_LEN_SLAVE_RCV = 65535  # 快速版SPI从模式接收

# CS延时最大值
MAX_CS_DELAY = 700000  # 微秒


class SPIDriver:
    """SPI驱动程序类 - 必须通过USBDriverTop使用"""

    def __init__(self, manager, dll_name: str):
        """
        初始化SPI驱动
        :param manager: 共享的DLL进程管理器（由USBDriverTop提供）
        :param dll_name: 共享的DLL名称（由USBDriverTop提供）
        """
        self._manager = manager
        self._dll_name = dll_name

        self.usb_opened = {}

    # ==================== SPI主模式操作 ====================

    def config_master_param(self, rate: int, first_bit: int, sub_mode: int, usb_index: int = 0) -> bool:
        """
        配置SPI主模式参数
        :param rate: SPI时钟频率 (SPI_Rate_281K~SPI_Rate_36M)
        :param first_bit: 帧格式 (SPI_MSB, SPI_LSB)
        :param sub_mode: 时钟模式 (SPI_SubMode_0~3)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        valid_rates = [SPI_Rate_281K, SPI_Rate_562K, SPI_Rate_1_125M, SPI_Rate_2_25M,
                       SPI_Rate_4_5M, SPI_Rate_9M, SPI_Rate_18M, SPI_Rate_36M]
        assert rate in valid_rates, "无效的SPI速率"
        assert first_bit in [SPI_MSB, SPI_LSB], "无效的帧格式"
        assert sub_mode in [SPI_SubMode_0, SPI_SubMode_1, SPI_SubMode_2, SPI_SubMode_3], "无效的时钟模式"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        result = self._manager.call(self._dll_name, "ConfigSPIParam", [rate, first_bit, sub_mode, usb_index])
        return result == 0

    def set_cs0(self, cs_level: int, usb_index: int = 0) -> bool:
        """
        设置SPI CS0管脚电平
        :param cs_level: CS0电平 (0=低电平, 1=高电平)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert cs_level in [0, 1], "CS电平必须为0或1"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        result = self._manager.call(self._dll_name, "SPISetCS0", [cs_level, usb_index])
        return result == 0

    def set_cs1(self, cs_level: int, usb_index: int = 0) -> bool:
        """
        设置SPI CS1管脚电平
        :param cs_level: CS1电平 (0=低电平, 1=高电平)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert cs_level in [0, 1], "CS电平必须为0或1"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        result = self._manager.call(self._dll_name, "SPISetCS1", [cs_level, usb_index])
        return result == 0

    def master_send_and_rcv_data(self, cs_select: int, start_cs: int, end_cs: int, cs_delay: int,
                                 duplex: int, dummy: int, send_buf: List[int], rcv_len: int,
                                 usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        SPI主模式发送和接收数据
        :param cs_select: CS管脚选择 (0=CS0, 1=CS1)
        :param start_cs: 传输前CS电平 (0=拉低, 1=拉高)
        :param end_cs: 传输后CS电平 (0=拉低, 1=拉高)
        :param cs_delay: CS延时(微秒,最大700000)
        :param duplex: 通讯方式 (0=半双工, 1=全双工)
        :param dummy: 接收时虚拟字节
        :param send_buf: 发送数据列表
        :param rcv_len: 接收数据长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 接收到的数据列表)
        """
        assert cs_select in [0, 1], "CS选择必须为0或1"
        assert start_cs in [0, 1], "起始CS电平必须为0或1"
        assert end_cs in [0, 1], "结束CS电平必须为0或1"
        assert 0 <= cs_delay <= MAX_CS_DELAY, f"CS延时必须在0-{MAX_CS_DELAY}微秒范围内"
        assert duplex in [0, 1], "通讯方式必须为0或1"
        assert 0 <= dummy <= 255, "虚拟字节必须在0-255范围内"
        assert len(send_buf) > 0, "发送缓存不能为空"
        assert rcv_len > 0, "接收长度必须大于0"
        assert len(send_buf) + rcv_len <= MAX_DATA_LEN_FAST_SPI, f"发送和接收数据总长度不能超过{MAX_DATA_LEN_FAST_SPI}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        # 准备参数规范（发送数组 + 接收数组）
        args_spec = [
            cs_select, start_cs, end_cs, cs_delay, duplex, dummy,
            {"type": "array", "data": send_buf},           # 发送数组
            {"type": "empty_array", "size": rcv_len},      # 接收数组
            len(send_buf), rcv_len, usb_index
        ]
        return_array_indices = [7]  # 接收数组在参数中的索引

        result, array_data = self._manager.call_with_arrays(self._dll_name, "SPISendAndRcvData",
                                                            args_spec, return_array_indices)

        if result >= 0:
            return True, array_data[0][:result]
        return False, []

    def master_send_data(self, start_cs: int, end_cs: int, send_buf: List[int], usb_index: int = 0) -> bool:
        """
        SPI主模式发送数据
        :param start_cs: 传输前CS电平 (0=拉低, 1=拉高)
        :param end_cs: 传输后CS电平 (0=拉低, 1=拉高)
        :param send_buf: 发送数据列表
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert start_cs in [0, 1], "起始CS电平必须为0或1"
        assert end_cs in [0, 1], "结束CS电平必须为0或1"
        assert len(send_buf) > 0, "发送缓存不能为空"
        assert len(send_buf) <= MAX_DATA_LEN_FAST_SPI, f"发送数据长度不能超过{MAX_DATA_LEN_FAST_SPI}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        # 准备参数规范（发送数组）
        args_spec = [
            start_cs, end_cs,
            {"type": "array", "data": send_buf},
            len(send_buf), usb_index
        ]

        result = self._manager.call(self._dll_name, "SPISendData", args_spec)
        return result == 0

    def master_rcv_data(self, start_cs: int, end_cs: int, rcv_len: int, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        SPI主模式读取数据
        :param start_cs: 传输前CS电平 (0=拉低, 1=拉高)
        :param end_cs: 传输后CS电平 (0=拉低, 1=拉高)
        :param rcv_len: 读取数据长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 接收到的数据列表)
        """
        assert start_cs in [0, 1], "起始CS电平必须为0或1"
        assert end_cs in [0, 1], "结束CS电平必须为0或1"
        assert rcv_len > 0, "接收长度必须大于0"
        assert rcv_len <= MAX_DATA_LEN_FAST_SPI, f"接收数据长度不能超过{MAX_DATA_LEN_FAST_SPI}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        # 准备参数规范（接收数组）
        args_spec = [
            start_cs, end_cs,
            {"type": "empty_array", "size": rcv_len},
            rcv_len, usb_index
        ]
        return_array_indices = [2]  # 接收数组在参数中的索引

        result, array_data = self._manager.call_with_arrays(self._dll_name, "SPIRcvData",
                                                            args_spec, return_array_indices)

        if result >= 0:
            return True, array_data[0][:result]
        return False, []

    # ==================== SPI从模式操作 ====================

    def config_slave_param(self, first_bit: int, sub_mode: int, usb_index: int = 0) -> bool:
        """
        设置SPI从模式参数
        :param first_bit: 帧格式 (SPI_MSB, SPI_LSB)
        :param sub_mode: 时钟模式 (SPI_SubMode_0~3)
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert first_bit in [SPI_MSB, SPI_LSB], "无效的帧格式"
        assert sub_mode in [SPI_SubMode_0, SPI_SubMode_1, SPI_SubMode_2, SPI_SubMode_3], "无效的时钟模式"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        result = self._manager.call(self._dll_name, "ConfigSPIParamSlave", [first_bit, sub_mode, usb_index])
        return result == 0

    def slave_preload_data(self, data_buf: List[int], usb_index: int = 0) -> bool:
        """
        SPI从模式预装数据
        :param data_buf: 预装数据列表
        :param usb_index: USB索引值 (0-99)
        :return: True=成功, False=失败
        """
        assert len(data_buf) > 0, "预装数据不能为空"
        assert len(data_buf) <= MAX_DATA_LEN_FAST_SPI, f"预装数据长度不能超过{MAX_DATA_LEN_FAST_SPI}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        # 准备参数规范（发送数组）
        args_spec = [
            {"type": "array", "data": data_buf},
            len(data_buf), usb_index
        ]

        result = self._manager.call(self._dll_name, "SPISlavePreloadData", args_spec)
        return result == 0

    def slave_rcv_data(self, max_len: int, usb_index: int = 0) -> Tuple[bool, List[int]]:
        """
        SPI从模式读取数据
        :param max_len: 最大读取长度
        :param usb_index: USB索引值 (0-99)
        :return: (成功标志, 接收到的数据列表)
        """
        assert max_len > 0, "最大读取长度必须大于0"
        assert max_len <= MAX_DATA_LEN_SLAVE_RCV, f"最大读取长度不能超过{MAX_DATA_LEN_SLAVE_RCV}字节"
        assert 0 <= usb_index <= 99, "USB索引必须在0-99范围内"

        # 准备参数规范（接收数组）
        args_spec = [
            {"type": "empty_array", "size": max_len},
            max_len, usb_index
        ]
        return_array_indices = [0]  # 接收数组在参数中的索引

        result, array_data = self._manager.call_with_arrays(self._dll_name, "SPISlaveRcvData",
                                                            args_spec, return_array_indices)

        if result >= 0:
            return True, array_data[0][:result]
        return False, []

    def open_usb(self, usb_index: int = 0) -> bool:
        """
        Open USB device for SPI operations
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
    print("SPI驱动程序示例")

    # 初始化驱动（需要提供DLL路径）
    manager = DLLProcessManager()
    driver = SPIDriver(manager, "usb_peripheral_dll")
    
    # 如果有DLL文件，可以这样加载：
    # manager.load_dll("usb_peripheral_dll", "path/to/USB2UARTSPIIICDLL.dll")
    # manager.set_function_prototype("usb_peripheral_dll", "OpenUsb", ["c_uint"], "c_int")
    # ... other prototypes ...

    try:
        print("\n=== SPI主模式示例 ===")

        # 1. 打开USB设备
        if driver.open_usb(0):
            print("✓ USB设备打开成功")

            # 2. 配置SPI主模式参数：18MHz,高位在前,模式0
            if driver.config_master_param(SPI_Rate_18M, SPI_MSB, SPI_SubMode_0, 0):
                print("✓ SPI主模式参数配置成功 (18MHz, MSB, Mode0)")

                # 3. 设置CS0为高电平
                if driver.set_cs0(1, 0):
                    print("✓ CS0设置为高电平")

                    # 4. SPI主模式发送和接收数据
                    send_data = [0x9F]  # 读取Flash ID命令
                    success, rcv_data = driver.master_send_and_rcv_data(
                        cs_select=0, start_cs=0, end_cs=1, cs_delay=50,
                        duplex=1, dummy=0xFF, send_buf=send_data, rcv_len=3, usb_index=0
                    )
                    if success:
                        print(f"✓ SPI主模式发送接收成功,接收数据: {[hex(x) for x in rcv_data]}")
                    else:
                        print("✗ SPI主模式发送接收失败")

                    # 5. 单独发送数据
                    cmd_data = [0x06]  # Write Enable命令
                    if driver.master_send_data(0, 1, cmd_data, 0):
                        print("✓ SPI主模式发送数据成功")
                    else:
                        print("✗ SPI主模式发送数据失败")

                    # 6. 单独接收数据
                    success, status_data = driver.master_rcv_data(0, 1, 1, 0)
                    if success:
                        print(f"✓ SPI主模式接收数据成功,接收数据: {[hex(x) for x in status_data]}")
                    else:
                        print("✗ SPI主模式接收数据失败")

            # 关闭USB
            if driver.close_usb(0):
                print("✓ USB设备关闭成功")
        else:
            print("✗ USB设备打开失败")

        print("\n=== SPI从模式示例 ===")

        if driver.open_usb(0):
            print("✓ USB设备打开成功")

            # 配置SPI从模式参数
            if driver.config_slave_param(SPI_MSB, SPI_SubMode_0, 0):
                print("✓ SPI从模式参数配置成功 (MSB, Mode0)")

                # 预装数据
                preload_data = [0xAA, 0x55, 0x01, 0x02, 0x03, 0x04]
                if driver.slave_preload_data(preload_data, 0):
                    print(f"✓ SPI从机预装数据成功: {[hex(x) for x in preload_data]}")

                    # 读取从机接收到的数据
                    success, rcv_data = driver.slave_rcv_data(10, 0)
                    if success and rcv_data:
                        print(f"✓ SPI从机读取数据成功,接收数据: {[hex(x) for x in rcv_data]}")
                    else:
                        print("○ SPI从机没有接收到数据")

            driver.close_usb(0)

        print("\n=== 高速SPI测试 ===")

        if driver.open_usb(0):
            print("✓ USB设备打开成功")

            # 测试36MHz,LSB模式,时钟模式3
            if driver.config_master_param(SPI_Rate_36M, SPI_LSB, SPI_SubMode_3, 0):
                print("✓ 高速SPI参数配置成功 (36MHz, LSB, Mode3)")

                # 发送大量数据测试
                large_data = list(range(100))  # 0-99
                if driver.master_send_data(0, 1, large_data, 0):
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
