import struct
import json

def ByteToHex( bins ):
    return ''.join( [ "%02X" % x for x in bins ] ).strip()

a = {"Temperature":22.5}
b = json.dumps(a)
c =b.encode()
data = ByteToHex(c)

def StrToBytes

"""
mqtt.heclouds.com   183.230.40.39   端口6002

234533 #产品ID
131817 #用户ID --nouse
721347240 #设备ID

2kJV6xxxxxxxxxxxxxx

#必须先创建对应的数据流模板
0300147B2254656D7065726174757265223A32322E357D
#订阅不允许订阅$dp  换个别的随便订阅 /mqtt/topic/0

#单片机发送
up = b'\x03\x00\x14'+b'{"Temperature":22.5}\r\n' ---ok!
"""
