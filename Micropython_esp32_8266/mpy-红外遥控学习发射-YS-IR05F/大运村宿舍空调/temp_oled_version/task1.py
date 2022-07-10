import dht
from machine import *
from machine import UART
import time
import machine
from config import *
import machine,ssd1306

oled=ssd1306.SSD1306_I2C(128,64,machine.I2C(scl=machine.Pin(22),sda=machine.Pin(23)))
oled.fill(0)
oled.text("starting...",0,54) #[col, row]
oled.show()

led = Pin(2,Pin.OUT)
led.off()
u=UART(2,9600)
u.read()
c=config('data.ini')
ret = c.readAll()
c2=config('status.ini')
ret2 = c2.readAll()
mydht = dht.DHT11(Pin(5))
# mydht.measure()
# temp = mydht.temperature()
# print(temp)
high_temp,low_temp = 26, 24 #触发温度

def save_status(key, val):
    global ret2, c2
    ret2[key] = val
    c2.writeConfig(ret2)
# send
def send_signal(next_status = False):
    global u, led, ret2
    u.write(ret['open']) #open=close
    u.write(ret['open'])
    u.write(ret['open'])
    led.on()
    time.sleep(1)
    led.off()
    #
    save_status('flag', next_status)

def show_oled(temp_dict):
    global oled
    oled.fill(0)
    for idx, key in enumerate(temp_dict):
        oled.text("{}:{}".format(key, temp_dict[key]),0,idx*10) #[col, row]
    oled.show()

# exec_now - 当人工现状刚刚开机时
reset_pin = Pin(18, Pin.IN, Pin.PULL_UP)
if(reset_pin.value() == 0):
    save_status('exec_now', True)
    show_oled(ret2)
    time.sleep(1)

def task_main():
    global mydht,u,ret,flag,led,ret2,c2,high_temp,low_temp
    mydht.measure()
    temp = mydht.temperature()
    save_status('high_temp', high_temp)
    save_status('low_temp', low_temp)
    last_temp = ret2.get('last_temp', 0)
    exec_now = ret2.get('exec_now', False)
    ###
    if temp >= high_temp and (last_temp < temp or exec_now):
        # open
        send_signal(next_status = True)
    
    if temp <= low_temp and (last_temp > temp or exec_now):
        # close
        send_signal(next_status = False)
    
    save_status('last_temp', temp)
    save_status('exec_now', False)
    print(ret2)
    show_oled(ret2)
    time.sleep(2)    
    # return 0

## main
task_main()
machine.deepsleep(10000) # 休眠ms
# reset()
