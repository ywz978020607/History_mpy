import requests
import time
import json
import sys

#upload
url="http://api.heclouds.com/devices/611890860/datapoints"
headers={'api-key':'gjU2173SbsvrSi4OpLyK8IXW3tc='}
data_name = ["lock"]

def upload(val):
    global data_name#,data_value
    temp_list = []
    for ii in range(len(data_name)):
        # temp_list.append({'id':data_name[ii],'datapoints':[{'value':'%.1f' % data_value[ii]}]} )
        temp_list.append({'id':data_name[ii],'datapoints':[{'value': val}]} )
    data = {'datastreams':temp_list}
    rp = requests.post(url, headers=headers, data=json.dumps(data))
    rp.close()

data_value = [0] 
data_time = [0] 
def down():
    global data_name,data_value
    r=requests.get(url,headers=headers)
    data_value[0] = r.json()['data']['datastreams'][0]['datapoints'][0]['value']
    data_value[0] = (int)(data_value[0])
    data_time[0] = r.json()['data']['datastreams'][0]['datapoints'][0]['at']
    r.close()  #esp32的urequest，记得关 close !!!
    print("value:{}, at:{}".format(data_value[0], data_time[0]))

if __name__ == "__main__":
    down()
    if len(sys.argv) > 1 and sys.argv[1] == "check":
        exit()
    
    val = sys.argv[1] if len(sys.argv) > 1 else 1
    upload(val = val)
    down()

    print("ok")
