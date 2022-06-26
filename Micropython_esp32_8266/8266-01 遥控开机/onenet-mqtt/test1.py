from T1 import *
import dht
#esp32端
###############################################
CLIENT_ID = "963736788"
username='234533'
password='2kJV69eUrcMxxxxxxxx'
###############################################
myMQTT = T1(CLIENT_ID,username,password)
#wifi
myMQTT.wifi("ywzywz","12345678")
myMQTT.mqttInit()
###############################################
# Init sensor

###updict
mydict = {}
mydict['data'] = [0] 

def task_main():
    global mydict
    myMQTT.publish('$dp',mydict) #$dp - onenet自动存储 不能订阅
    # myMQTT.publish('c211120',mydict)
    myMQTT.c.check_msg() #检测接收并调用sub_cb # 如果掉线则报错
    recvdict = myMQTT.get_data
    myMQTT.get_data = {} #清空
    if recvdict!={}:
        print(recvdict)
    

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
        # 断网直接重启
        if not myMQTT.wifiobj.mywifi.isconnected():
            machine.reset()
        time.sleep(5)
