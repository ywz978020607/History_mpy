from machine import SoftUART, Pin

uart1 = SoftUART(tx=Pin(2), rx=Pin(4))
uart1.init(tx=Pin(2), rx=Pin(4), baudrate=9600, timeout=0)

uart2 = SoftUART(tx=Pin(12), rx=Pin(13))
uart2.init(tx=Pin(12), rx=Pin(13), baudrate=9600, timeout=0)

# .read()  .write(b'\x01')

"""
import uos
from machine import *
uos.dupterm(None, 1)

u1 = SoftUART(tx=Pin(1), rx=Pin(3))
u1.init(tx=Pin(1), rx=Pin(3), baudrate=9600, timeout=0)
"""
