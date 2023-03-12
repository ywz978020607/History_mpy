from machine import *
import time

import struct
import json
def ByteToHex( bins ):
    return ''.join( [ "%02X" % x for x in bins ] ).strip()

#data0
ad1=ADC(Pin(33)) 
ad1.atten(ADC.ATTN_11DB)
ad1.width(ADC.WIDTH_12BIT)
ad1.read()

led = Pin(2,Pin.OUT)
led.off()
# time.sleep(10)
led.on()

u = UART(2,9600)
u.read()
# """
# >>> u = UART(2,9600)
# >>> u.read()
# >>> u.read()
# >>> u.read()
# b'\r\n+STATUS: 1,MQTT CLOSED\r\n\r\n+STATUS: 1,MQTT CONNECTED\r\n123123'
# >>> u.read()
# >>> u.read()
# """

def task_main():
    global ad1,u,led

    val = ad1.read()
    a = {"data0":val}
    # a = {"Temperature":22.5}
    b = json.dumps(a).replace(" ","")
    c =b.encode() #ByteToHex(c)
    lencode = len(c)
    data = struct.pack("B",3)  + struct.pack(">H",lencode) + c +b'\r\n'

    u.write(data)

    led.off()
    time.sleep(1)
    led.on()
    print("once")


def run():
    while 1:
        
        try:
            task_main()
        except:
            print("error")
        
        time.sleep(2)



