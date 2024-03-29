# import dht
from machine import *
from machine import UART
import time
from config import *
import machine,ssd1306
import lm75a

class TempControl():
    # init
    def __init__(self) -> None:
        self.oled = None
        try:
            self.oled = ssd1306.SSD1306_I2C(128,64,machine.I2C(scl=machine.Pin(22),sda=machine.Pin(23)))
            self.oled.fill(0)
            self.oled.text("starting...",0,54) #[col, row]
            self.oled.show()
        except:
            print("no oled")
        self.led = Pin(2,Pin.OUT)
        self.led.off()
        self.u=UART(2,9600)
        self.u.read()
        self.c=config('data.ini')
        self.ret = self.c.readAll()
        self.c2=config('status.ini')
        self.ret2 = self.c2.readAll()
        # self.mydht = dht.DHT11(Pin(5))
        self.sensor = lm75a.LM75A(I2C(scl=Pin(25), sda=Pin(26)))
        self.high_temp, self.low_temp = 27.8, 25.2 #触发温度
        self.delta_temp = 0.1

        self.check_exec_now()
        # 使用重启代替定时器防止进程死掉
        self.cnt_count()
    # get avg temp
    def _get_temp_avg(self):
        temp_list = []
        for ii in range(10):
            temp_list.append(self.sensor.temp())
            time.sleep_ms(100)
        return sum(temp_list)/len(temp_list) if temp_list else 0

    # exec_now - 当手动接地时立即执行
    def check_exec_now(self):
        reset_pin = Pin(18, Pin.IN, Pin.PULL_UP)
        if(reset_pin.value() == 0):
            self.save_status('exec_now', True)
            self.show_oled(self.ret2)
            time.sleep(1)

    def cnt_check(self):
        if self.ret2.get('cnt', 0) > 0:
            return False
        else:
            self.cnt_set()
            return True

    def cnt_set(self, reset_cnt = 60):
        # task and set cnt
        self.save_status('cnt', reset_cnt)

    def cnt_count(self):
        if self.ret2.get('cnt', 0) > 0:
            self.save_status('cnt', self.ret2.get('cnt', 0) - 1)

    def save_status(self, key, val):
        self.ret2[key] = val
        self.c2.writeConfig(self.ret2)
    # send
    def send_signal(self, next_status = False, reset_cnt = 60):
        self.u.write(self.ret['open']) #open=close
        self.u.write(self.ret['open'])
        self.u.write(self.ret['open'])
        self.led.on()
        time.sleep(1)
        self.led.off()
        self.save_status('flag', next_status)
        # reset timer
        self.cnt_set(reset_cnt)

    def show_oled(self, temp_dict):
        if self.oled:
            self.oled.fill(0)
            for idx, key in enumerate(temp_dict):
                self.oled.text("{}:{}".format(key, temp_dict[key]),0,idx*10) #[col, row]
            self.oled.show()

    def task_main(self):
        # self.mydht.measure()
        # temp = self.mydht.temperature()
        # temp = self.sensor.temp()
        temp = self._get_temp_avg()
        self.save_status('high_temp', self.high_temp)
        self.save_status('low_temp', self.low_temp)
        last_temp = self.ret2.get('last_temp', 0)
        exec_now = self.ret2.get('exec_now', False)
        if abs(temp - last_temp) < self.delta_temp:
            temp = last_temp
        ###
        if temp >= self.high_temp and (last_temp < temp or exec_now or self.cnt_check()):
            # open
            self.send_signal(next_status = True, reset_cnt=60)
        if temp <= self.low_temp:
            if (last_temp > temp or exec_now or self.cnt_check()):
                # close
                self.send_signal(next_status = False, reset_cnt=60)
            elif self.ret2.get('cnt', 0) > 50: #注意Δ，保证正常空调降温时间够长
                self.cnt_set(self.ret2.get('cnt', 0) + 1)
        ###
        self.save_status('last_temp', temp)
        self.save_status('exec_now', False)
        print(self.ret2)
        self.show_oled(self.ret2)
        time.sleep(2)



## ---------- main -------------
try:
    tempControl = TempControl()
    try:
        tempControl.task_main()
    except Exception as e:
        tempControl.show_oled({"e": e})
except Exception as e:
    print(e)

machine.deepsleep(60000) # 休眠ms

