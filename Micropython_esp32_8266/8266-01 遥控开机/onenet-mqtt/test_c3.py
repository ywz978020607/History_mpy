from machine import *
con = Pin(12, Pin.OUT, Pin.PULL_UP)
con.on()

from T1 import *
import time
#esp32端
###############################################
CLIENT_ID = "1048956392"
username='234533'
password='2kJV69exxxxxxxxx'
###############################################
myMQTT = T1(CLIENT_ID,username,password)
#wifi
myMQTT.wifi()
myMQTT.mqttInit()
###############################################
# Init sensor

###updict
mydict = {}
mydict['data'] = [0] 

def task_main():
    global mydict, con
    myMQTT.publish('$dp',mydict) #$dp - onenet自动存储 不能订阅
    # myMQTT.publish('c211120',mydict)
    myMQTT.c.check_msg() #检测接收并调用sub_cb # 如果掉线则报错
    recvdict = myMQTT.get_data
    myMQTT.get_data = {} #清空
    if recvdict!={}:
        print(recvdict)
        if "lock" in recvdict:
            con.off()
            time.sleep(float(recvdict["lock"]))
            con.on()

def run():
    resub_count = 0
    while 1:
        try:
            task_main()
            resub_count = 0
        except:
            # 重连
            resub_count += 1
            try:
                myMQTT.mqttInit()
            except:
                print("resub error")
        # 断网/重连次数过多直接重启
        # if not myMQTT.wifiobj.mywifi.isconnected() or resub_count > 3:
        if not myMQTT.wifiobj or not myMQTT.wifiobj.isconnected() or resub_count > 3:
            machine.reset()
        time.sleep(30)
