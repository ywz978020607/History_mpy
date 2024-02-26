from machine import Pin
from machine import Timer
from time import sleep_ms
import bluetooth
import network
import time
import json

BLE_MSG = ""

class ESP32_BLE():
    def __init__(self, name):
        self.led = Pin(12, Pin.OUT)    #配置LED灯引脚为输出模式
        self.timer1 = Timer(0)         #配置定时器
        self.name = name
        self.ble = bluetooth.BLE()     #创建蓝牙对象
        self.ble.active(True)          #开启蓝牙
        self.ble.config(gap_name=name) #配置蓝牙信息
        self.disconnected()            #设置定时器中断
        self.ble.irq(self.ble_irq)     #蓝牙时间处理
        self.register()                #配置蓝牙的uuid
        self.ble.gatts_write(self.rx, bytes(100))#默认蓝牙只接收20字节，这里更改为接收100字节
        self.advertiser()              #蓝牙广播
        self.ok=0

    def disconnected(self):        
        self.timer1.init(period=100, mode=Timer.PERIODIC, callback=lambda t: self.led.value(not self.led.value()))

    def register(self):        
        service_uuid = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
        reader_uuid = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'
        sender_uuid = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'

        services = (
            (
                bluetooth.UUID(service_uuid), 
                (
                    (bluetooth.UUID(sender_uuid), bluetooth.FLAG_NOTIFY), 
                    (bluetooth.UUID(reader_uuid), bluetooth.FLAG_WRITE),
                 )
             ), 
        )

        ((self.tx, self.rx,), ) = self.ble.gatts_register_services(services)

    def advertiser(self):
        name = bytes(self.name, 'UTF-8')
        adv_data = b'\x02\x01\x02' + bytearray((len(name) + 1, 0x09)) + name
        self.ble.gap_advertise(100, adv_data)

    def ble_irq(self, event, data):
        global BLE_MSG
        if event == 1: #手机连接了此设备
            self.connected()
        elif event == 2: #手机断开此设备
            if self.ok==0:
                self.advertiser()
                self.disconnected()
        elif event == 3: #手机发送了数据 
            buffer = self.ble.gatts_read(self.rx)
            BLE_MSG = buffer.decode('UTF-8').strip()
            self.wifi_get_data()

    def connected(self):
        self.timer1.deinit()
        self.led.value(0)

    def wifi_get_data(self):
        if len(BLE_MSG)>0:
            ssid,password=BLE_MSG.split('_____pass')   #分离信息
            wifi_connect_ok=wifi_connect(ssid[4:],password) #对WiFi进行连接
            if wifi_connect_ok:
                config = dict(ssid=ssid[4:], password=password)  #将WiFi信息写成字典格式
                with open('wifi_config','w+') as f: #将字典转成json写入wifi_config文件中
                    f.write(json.dumps(config))
                self.ok=1
                self.ble.active(False)  #关闭蓝牙
            else:
                self.ble.active(False) #这里要先关闭蓝牙，不关闭的话虽然也可以用，但python会弹出报错
                self.__init__("ESP32") #重新初始化蓝牙

def wifi_connect(ssid,pwd):
    station = network.WLAN(network.STA_IF)  #配置WiFi的STA模式
    station.active(True)   #开启esp32的WiFi
    station.connect(ssid, pwd) #进行WiFi连接
    count=0
    while station.isconnected() == False: #检测是否连接成功
        count+=1
        time.sleep(1)
        if count>5:    #如果检测到大于5次没有连接成功（5秒后还没有联网）关闭WiFi并返回
            station.active(False)
            return 0
    print("Connection successful")
    print(station.ifconfig())
    led = Pin(12, Pin.OUT)  #连接成功后配置LED引脚为输出模式，让LED灯长亮
    led.value(1)
    return 1

if __name__ == "__main__":
    try:
        with open('wifi_config','r+') as f: #尝试打开wifi_config文件
            config = json.loads(f.read())   #获取wifi_config的信息
        wifi_connect_ok=wifi_connect(config['ssid'],config['password']) #进行WiFi连接
        if wifi_connect_ok:
            wifi_connect_flag=1
        else:
            ble = ESP32_BLE("ESP32")  #如果连接失败则进入蓝牙配网模式
    except OSError:
        ble = ESP32_BLE("ESP32")  #如果查找不到文件则进入蓝牙配网模式

    while True:
        # if wifi_connect_ok or ble.ok: #如果WiFi已连接，进行xxxxx
        if 1:
            pass
