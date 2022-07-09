import dht
from machine import *
from machine import UART
import time
import machine
from config import *

led = Pin(2,Pin.OUT)
led.off()
# led.on()
# time.sleep(1)
# led.off()
u=UART(2,9600)
c=config('data.ini')
ret = c.readAll()

c2=config('status.ini')
ret2 = c2.readAll()
u.read()
#start or close temp set
high_temp = 26
low_temp = 24

# 重置引脚
reset_pin = Pin(23, Pin.IN, Pin.PULL_UP)
if(reset_pin.value() == 0):
    ret2['flag'] = '0'
    c2.writeConfig(ret2)
    led.on()
    time.sleep(1)
    led.off()
    machine.deepsleep(5000)

##################################
mydht = dht.DHT11(Pin(5))
mydht.measure()
temp = mydht.temperature()
print(temp)

flag = ret2['flag']
print(flag)

def task_main():
    global mydht,u,ret,flag,led,ret2,c2,high_temp,low_temp
    mydht.measure()
    temp = mydht.temperature()
    if temp>=high_temp:
        if flag=='0':
            #use
            u.write(ret['open'])
            flag = '1'
            ret2['flag'] = '1'
            c2.writeConfig(ret2)
            print(u.read())
    if temp<=low_temp:
        if flag=='1':
            #use
            u.write(ret['open'])
            flag = '0'
            ret2['flag'] = '0'
            c2.writeConfig(ret2)
            print(u.read())
    if flag=='0':
        led.off()
    elif flag=='1':
        led.on()
    time.sleep(2)    
    return 0

task_main()

machine.deepsleep(30000) # 休眠ms
# reset()
