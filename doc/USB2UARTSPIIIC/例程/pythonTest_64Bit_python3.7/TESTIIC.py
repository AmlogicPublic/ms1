# -*- coding: utf-8 -*-
import ctypes
import os
import time
#import numpy

#import pandas

AbsPath  = os.path.abspath(os.path.dirname(__file__))
Dir_USB2UARTSPIIICDLL = os.path.join(AbsPath, "USB2UARTSPIIICDLL.dll")
print(Dir_USB2UARTSPIIICDLL)
#导入动态链接库
#64位
I2cDriver_dll_Usb = ctypes.CDLL(Dir_USB2UARTSPIIICDLL)
#32位
#SpiDriver_dll_Usb = ctypes.windll.LoadLibrary(Dir_USB2UARTSPIIICDLL) 
#打开端口
Is_Open  = I2cDriver_dll_Usb.OpenUsb(ctypes.c_int(0))
print(Is_Open)

#配置参数
Is_True = I2cDriver_dll_Usb.ConfigIICParam(ctypes.c_uint(0),ctypes.c_uint(10000),ctypes.c_uint(0))
print(Is_True)

#发送数据
Create_SendBuffer = (ctypes.c_char * 9)()
Create_SendBuffer = b"\xA0\x00\x01\x02\x03\x04\x05\x06\x07"

Is_Convey = I2cDriver_dll_Usb.IICSendData(ctypes.c_bool(True),ctypes.c_bool(True),Create_SendBuffer,\
                                                ctypes.c_int(9),ctypes.c_uint(0)) 
print(Is_Convey)

time.sleep(1)

#接收数据
Create_SendBuffer1 = (ctypes.c_char * 9)()
Create_SendBuffer1 = b"\xA1\x00\x59\x59\x23\x07\x31\x12\x17"

Is_Convey = I2cDriver_dll_Usb.IICSendData(ctypes.c_bool(True),ctypes.c_bool(False),Create_SendBuffer1,\
                                                ctypes.c_int(1),ctypes.c_uint(0)) 

Create_RevBuffer = b"\x00\x00\x00\x00\x00\x00\x00\x00"
Is_Recieve = I2cDriver_dll_Usb.IICRcvData(ctypes.c_bool(True),Create_RevBuffer,ctypes.c_int(8),ctypes.c_int(0))
print(Is_Recieve)

for i in range(0,8):
    print('%#X'%Create_RevBuffer[i])


I2cDriver_dll_Usb.CloseUsb(ctypes.c_uint(0))

#os.system("pause") 



