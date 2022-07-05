import dht
from machine import *
from machine import UART
import time
import machine
from config import *



led = Pin(2,Pin.OUT)
led.on()
time.sleep(1)
led.off()
u=UART(2,9600)
c=config('data.ini')
ret = c.readAll()

c2=config('status.ini')
ret2 = c2.readAll()
u.read()
##################################
# 重置引脚
reset_pin = Pin(23, Pin.OUT)
if(reset_pin.value() == 0):
    ret2['cnt'] = 0
    c2.writeConfig(ret2)
    machine.deepsleep(10000)

if 'flag' not in ret2:
    ret2['flag'] = 0
print(ret2['flag'])
if 'cnt' not in ret2:
    ret2['cnt'] = 0

def task_main():
    global u,ret,led,ret2,c2
    # cnt++ & status
    if ret2['cnt'] > 60 and ret2['flag']==1:
        # 关闭
        u.write(ret['open'])
        ret2['flag'] = 0
    elif ret2['cnt'] > 150:
        # 重置
        ret2['cnt'] = 0
    else:
        ret2['cnt'] += 1
    
    # cnt=0时开启 & flag=1
    if ret2['cnt'] == 0:
        u.write(ret['open'])
        ret2['flag'] = 1
    c2.writeConfig(ret2)
    return 0

task_main()

machine.deepsleep(60000) # 休眠ms
# reset()
