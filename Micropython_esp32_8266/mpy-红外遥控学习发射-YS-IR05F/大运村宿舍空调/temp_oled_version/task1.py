import dht
from machine import *
from machine import UART
import time
import machine
from config import *
import machine,ssd1306


class TempControl():
    # init
    def __init__(self) -> None:
        self.oled=ssd1306.SSD1306_I2C(128,64,machine.I2C(scl=machine.Pin(22),sda=machine.Pin(23)))
        self.oled.fill(0)
        self.oled.text("starting...",0,54) #[col, row]
        self.oled.show()
        self.led = Pin(2,Pin.OUT)
        self.led.off()
        self.u=UART(2,9600)
        self.u.read()
        self.c=config('data.ini')
        self.ret = self.c.readAll()
        self.c2=config('status.ini')
        self.ret2 = self.c2.readAll()
        self.mydht = dht.DHT11(Pin(5))
        self.high_temp, self.low_temp = 26, 24 #触发温度

        self.check_exec_now()
    
    # exec_now - 当手动接地时立即执行
    def check_exec_now(self):
        reset_pin = Pin(18, Pin.IN, Pin.PULL_UP)
        if(reset_pin.value() == 0):
            self.save_status('exec_now', True)
            self.show_oled(self.ret2)
            time.sleep(1)
        
    def save_status(self, key, val):
        self.ret2[key] = val
        self.c2.writeConfig(self.ret2)
    # send
    def send_signal(self, next_status = False):
        self.u.write(self.ret['open']) #open=close
        self.u.write(self.ret['open'])
        self.u.write(self.ret['open'])
        self.led.on()
        time.sleep(1)
        self.led.off()
        self.save_status('flag', next_status)

    def show_oled(self, temp_dict):
        self.oled.fill(0)
        for idx, key in enumerate(temp_dict):
            self.oled.text("{}:{}".format(key, temp_dict[key]),0,idx*10) #[col, row]
        self.oled.show()

    def task_main(self):
        self.mydht.measure()
        temp = self.mydht.temperature()
        self.save_status('high_temp', self.high_temp)
        self.save_status('low_temp', self.low_temp)
        last_temp = self.ret2.get('last_temp', 0)
        exec_now = self.ret2.get('exec_now', False)
        ###
        if temp >= self.high_temp and (last_temp < temp or exec_now):
            # open
            self.send_signal(next_status = True)
        
        if temp <= self.low_temp and (last_temp > temp or exec_now):
            # close
            self.send_signal(next_status = False)
        
        self.save_status('last_temp', temp)
        self.save_status('exec_now', False)
        print(self.ret2)
        self.show_oled(self.ret2)
        time.sleep(2)

## main
try:
    tempControl = TempControl()
    try:
        tempControl.task_main()
    except Exception as e:
        tempControl.show_oled({"e": e})
except Exception as e:
    print(e)

machine.deepsleep(10000) # 休眠ms

