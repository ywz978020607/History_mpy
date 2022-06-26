import time
import json
import requests

#mqtt下发到设备
url="http://api.heclouds.com/cmds?device_id=721347240"
headers={'api-key':'2kJV69exxxxxxxxxxxxx'}

up="123"
up=str(up)
rp=requests.post(url,headers=headers,data=up)
rp.close()

