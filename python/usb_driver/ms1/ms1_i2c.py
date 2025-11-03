"""
MS1 MCU I2C Driver
Encapsulates I2C operations for MS1 MCU, including AHB register read/write

Note: Must be used with a USBDriverTop instance.
"""

from usb.i2c_driver import IIC_Rate_1M, IIC_ADDRMOD_7BIT

# MS1 I2C Configuration
MS1_SLAVE_ADDR = 0x3D      # MS1 I2C slave address (0x7A >> 1)
MS1_I2C_RATE = IIC_Rate_1M  # I2C clock rate: 1MHz


class MS1I2CDriver:
    """MS1 MCU I2C Driver with AHB bus register access"""
    
    def __init__(self, usb_top, usb_index: int = 0):
        """
        Initialize MS1 I2C Driver
        :param usb_top: USBDriverTop instance (required)
        :param usb_index: USB device index (default: 0)
        """
        self._usb_top = usb_top
        self.driver = usb_top.i2c
        self.slave_addr = MS1_SLAVE_ADDR
        self.usb_index = usb_index
        self.i2c_rate = MS1_I2C_RATE
        self.is_configured = False
    
    def configure(self) -> bool:
        """
        Configure I2C parameters for MS1
        :return: True if successful
        """
        # Configure I2C master mode parameters
        if not self.driver.config_master_param(self.i2c_rate, 0, self.usb_index):
            return False
        
        self.is_configured = True
        return True
    
    def check_slave(self) -> bool:
        """
        Check if slave address exists on I2C bus
        :return: True if slave responds
        """
        assert self.is_configured, "Device not configured, call configure() first"
        return self.driver.check_slave_address(IIC_ADDRMOD_7BIT, self.slave_addr, self.usb_index)
    
    def set_slave_addr(self, addr: int):
        """
        Set I2C slave address
        :param addr: 7-bit slave address
        """
        assert 0 <= addr <= 0x7F, "7-bit address must be in 0-0x7F range"
        self.slave_addr = addr
    
    def write(self, start_addr: int, data):
        """
        Write data to register(s) starting from start_addr
        :param start_addr: Starting register address
        :param data: Single value (int) or list of values to write
        :return: True if successful
        """
        assert self.is_configured, "Device not configured, call configure() first"
        assert 0 <= start_addr <= 0xFFFFFFFF, "Address must be 32-bit"
        
        # Convert single value to list
        if isinstance(data, int):
            data_list = [data]
        else:
            data_list = list(data)
        
        # Write each register
        for i, value in enumerate(data_list):
            assert 0 <= value <= 0xFFFFFFFF, f"Data[{i}] must be 32-bit"
            addr = start_addr + (i * 4)  # Each register is 4 bytes
            
            # Convert address to bytes (little-endian)
            addr_bytes = [
                (addr >> 0) & 0xFF,
                (addr >> 8) & 0xFF,
                (addr >> 16) & 0xFF,
                (addr >> 24) & 0xFF
            ]
            
            # Convert data to bytes (little-endian)
            data_bytes = [
                (value >> 0) & 0xFF,
                (value >> 8) & 0xFF,
                (value >> 16) & 0xFF,
                (value >> 24) & 0xFF
            ]
            
            # Use register_send to include slave address
            if not self.driver.register_send(IIC_ADDRMOD_7BIT, self.slave_addr, 
                                            addr_bytes, data_bytes, self.usb_index):
                return False
        
        return True
    
    def read(self, start_addr: int, count: int = 1):
        """
        Read data from register(s) starting from start_addr
        :param start_addr: Starting register address
        :param count: Number of 32-bit registers to read (default: 1)
        :return: List of values, or None if failed
        """
        assert self.is_configured, "Device not configured, call configure() first"
        assert 0 <= start_addr <= 0xFFFFFFFF, "Address must be 32-bit"
        assert count > 0, "Count must be positive"
        
        results = []
        
        for i in range(count):
            addr = start_addr + (i * 4)  # Each register is 4 bytes
            
            # Convert address to bytes (little-endian)
            addr_bytes = [
                (addr >> 0) & 0xFF,
                (addr >> 8) & 0xFF,
                (addr >> 16) & 0xFF,
                (addr >> 24) & 0xFF
            ]
            
            # Use register_read to include slave address
            success, data_bytes = self.driver.register_read(IIC_ADDRMOD_7BIT, self.slave_addr,
                                                           addr_bytes, 4, self.usb_index)
            if not success or len(data_bytes) < 4:
                return None
            
            # Convert bytes to 32-bit value (little-endian)
            value = (data_bytes[0] | 
                    (data_bytes[1] << 8) | 
                    (data_bytes[2] << 16) | 
                    (data_bytes[3] << 24))
            
            results.append(value)
        
        return results

