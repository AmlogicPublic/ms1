"""
USB Driver Top - Unified USB peripheral driver manager
Manages single DLL process and all peripheral drivers
All peripherals share the same DLL instance and can be used in parallel
"""

import os
from typing import Optional
from .dll_process_manager import DLLProcessManager
from .i2c_driver import I2CDriver
from .spi_driver import SPIDriver
from .uart_driver import UARTDriver
from .pwm_driver import PWMDriver
from .adc_driver import ADCDriver
from .io_driver import IODriver


class USBDriverTop:
    """Top-level USB driver manager that coordinates all peripheral drivers"""
    
    def __init__(self, dll_path: Optional[str] = None):
        """
        Initialize USB driver top with all peripherals
        :param dll_path: Path to USB2UARTSPIIICDLL.dll
        """
        # Create single DLL process manager instance
        self._manager = DLLProcessManager()
        self._dll_name = "usb_peripheral_dll"
        self._dll_loaded = False
        self._usb_opened = {}  # Track opened USB devices
        
        # Initialize peripheral drivers (not loaded yet)
        self.i2c: Optional[I2CDriver] = None
        self.spi: Optional[SPIDriver] = None
        self.uart: Optional[UARTDriver] = None
        self.pwm: Optional[PWMDriver] = None
        self.adc: Optional[ADCDriver] = None
        self.io: Optional[IODriver] = None
        
        # Auto-load DLL if path provided
        if dll_path:
            self.load_dll(dll_path)
        else:
            # Try to find DLL in default location
            default_dll = os.path.join(os.path.dirname(__file__), "USB2UARTSPIIICDLL.dll")
            if os.path.exists(default_dll):
                self.load_dll(default_dll)
    
    def load_dll(self, dll_path: str):
        """
        Load DLL once and initialize all peripheral drivers
        :param dll_path: Path to USB2UARTSPIIICDLL.dll
        """
        assert os.path.exists(dll_path), f"DLL file not found: {dll_path}"
        
        # Load DLL into process manager
        self._manager.load_dll(self._dll_name, dll_path)
        self._dll_loaded = True
        
        # Setup function prototypes for all peripherals
        self._setup_all_function_prototypes()
        
        # Initialize all peripheral drivers with shared manager and dll_name
        self.i2c = I2CDriver(manager=self._manager, dll_name=self._dll_name)
        self.spi = SPIDriver(manager=self._manager, dll_name=self._dll_name)
        self.uart = UARTDriver(manager=self._manager, dll_name=self._dll_name)
        self.pwm = PWMDriver(manager=self._manager, dll_name=self._dll_name)
        self.adc = ADCDriver(manager=self._manager, dll_name=self._dll_name)
        self.io = IODriver(manager=self._manager, dll_name=self._dll_name)
    
    def _setup_all_function_prototypes(self):
        """Setup function prototypes for all peripherals at once"""
        # USB basic operations (shared by all)
        self._manager.set_function_prototype(self._dll_name, "OpenUsb", ["c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "CloseUsb", ["c_uint"], "c_int")
        
        # I2C functions
        self._manager.set_function_prototype(self._dll_name, "ConfigIICParam", ["c_uint", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICSendAndRcvData", 
                                             ["c_ubyte", "c_ubyte", "POINTER_c_ubyte", "POINTER_c_ubyte", "c_uint", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICSendData", 
                                             ["c_ubyte", "c_ubyte", "POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICRcvData", 
                                             ["c_ubyte", "POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICCheckSlaveAddr", 
                                             ["c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICRegisterSend", 
                                             ["c_ubyte", "c_uint", "POINTER_c_ubyte", "POINTER_c_ubyte", "c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICRegisterRead", 
                                             ["c_ubyte", "c_uint", "POINTER_c_ubyte", "POINTER_c_ubyte", "c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICDirectSend", 
                                             ["c_ubyte", "c_uint", "POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICDirectRead", 
                                             ["c_ubyte", "c_uint", "POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "ConfigIICParamSlave", 
                                             ["c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICSlavePreloadData", 
                                             ["POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "IICSlaveRcvData", 
                                             ["POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        
        # SPI functions
        self._manager.set_function_prototype(self._dll_name, "ConfigSPIParam",
                                             ["c_uint", "c_uint", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "SPISetCS0",
                                             ["c_ubyte", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "SPISetCS1",
                                             ["c_ubyte", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "SPISendAndRcvData",
                                             ["c_ubyte", "c_ubyte", "c_ubyte", "c_uint", "c_ubyte", "c_ubyte",
                                              "POINTER_c_ubyte", "POINTER_c_ubyte", "c_uint", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "SPISendData",
                                             ["c_uint", "c_uint", "POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "SPIRcvData",
                                             ["c_uint", "c_uint", "POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "ConfigSPIParamSlave",
                                             ["c_uint", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "SPISlavePreloadData",
                                             ["POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "SPISlaveRcvData",
                                             ["POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        
        # UART functions
        self._manager.set_function_prototype(self._dll_name, "ConfigUARTParam", ["c_uint", "c_uint", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "UARTSendData", ["POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "UARTRcvData", ["POINTER_c_ubyte", "c_uint", "c_uint"], "c_int")
        
        # PWM functions
        self._manager.set_function_prototype(self._dll_name, "PWMOut", ["c_uint", "c_uint", "c_uint", "c_uint", "c_uint"], "c_int")
        self._manager.set_function_prototype(self._dll_name, "PWMClose", ["c_uint"], "c_int")
        
        # ADC functions
        self._manager.set_function_prototype(self._dll_name, "GetADCVal", ["c_ubyte", "c_uint"], "c_int")
        
        # IO functions
        self._manager.set_function_prototype(self._dll_name, "IOSetAndRead", ["c_uint", "c_uint", "c_uint", "c_uint"], "c_int")
    
    def open_usb(self, usb_index: int = 0) -> bool:
        """
        Open USB device
        :param usb_index: USB index (0-99)
        :return: True if successful
        """
        assert 0 <= usb_index <= 99, "USB index must be in range 0-99"
        assert self._dll_loaded, "DLL not loaded, call load_dll() first"
        
        if usb_index in self._usb_opened:
            return True  # Already opened
        
        result = self._manager.call(self._dll_name, "OpenUsb", [usb_index])
        if result == 0:
            self._usb_opened[usb_index] = True
            return True
        return False
    
    def close_usb(self, usb_index: int = 0) -> bool:
        """
        Close USB device
        :param usb_index: USB index (0-99)
        :return: True if successful
        """
        assert 0 <= usb_index <= 99, "USB index must be in range 0-99"
        assert self._dll_loaded, "DLL not loaded"
        
        if usb_index not in self._usb_opened:
            return True  # Already closed
        
        # Close PWM if active
        if self.pwm and hasattr(self.pwm, 'pwm_active') and usb_index in self.pwm.pwm_active:
            if self.pwm.pwm_active[usb_index]:
                self.pwm.close_pwm(usb_index)
        
        result = self._manager.call(self._dll_name, "CloseUsb", [usb_index])
        if result == 0:
            self._usb_opened.pop(usb_index, None)
            return True
        return False
    
    def is_usb_opened(self, usb_index: int = 0) -> bool:
        """Check if USB device is opened"""
        return usb_index in self._usb_opened
    
    def close_all_usb(self):
        """Close all opened USB devices"""
        for usb_index in list(self._usb_opened.keys()):
            self.close_usb(usb_index)
    
    def shutdown(self):
        """Shutdown driver manager and close all resources"""
        self.close_all_usb()
        self._manager.shutdown()
    
    def __enter__(self):
        """Context manager entry"""
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """Context manager exit, auto-close all USB devices"""
        self.close_all_usb()

