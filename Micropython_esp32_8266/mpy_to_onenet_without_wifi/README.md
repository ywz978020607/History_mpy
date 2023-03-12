与onenet-mqtt通信，常用型号：
1. 4g系列-塔石E29V-模块配置，完全透传，直接通信
2. nbiot系列：
    - m5311：配置较麻烦，价格优势一般，售后情况很差，弃用
    - 塔石E33V：完全透传，且能够和4g的E29V接口兼容，价格也便宜，唯一缺点是稍稍有点大，不过也还好，代码简洁。
    - 移远BC系列：BC20[GPS+NBIOT]、BC25/26/28模块[NBIOT]以及各个型号相应的增加stm32f0单独mcu的透传板。AT模块体积很小，但需要AT配置，支持二次开发，也有透传板可以单独对额外mcu编程，不过供电均为3.3V供电，建议DIY小板，如8266+AMS1117降压加上MOS开关来控制模块


塔石系列为透传，无论4g还是nbiot，配置好后使用方法如下：
```
# 塔石系列
up_data={'temp':temp}
packed_data = json.dumps(up_data).replace(" ","").encode() #ByteToHex(c)
lencode = len(packed_data)
data = struct.pack("B",3)  + struct.pack(">H",lencode) + packed_data
u2.write(data)
#
recv = str(u2.read())
if "on" in recv:
    pass
```