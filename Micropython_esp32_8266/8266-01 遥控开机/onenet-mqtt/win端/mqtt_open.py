import requests
import time
import json
import sys

device_id = "963736788"

#cmd
def send(val):
    global device_id
    url = "http://api.heclouds.com/cmds?device_id=" + device_id
    headers={'api-key':'2kJV69exxxxxxxxx'}
    downdata = {"lock": float(val)}
    res = requests.post(url, headers=headers, data=json.dumps(downdata))
    cnt = 0
    while json.loads(res.text).get('errno', 0) != 0 and cnt < 3:
        print("error: {}, resent waiting...".format(json.loads(res.text)))
        time.sleep(3)
        res = requests.post(url, headers=headers, data=json.dumps(downdata))
        cnt += 1

#status
def check_status():
    global device_id
    url = "http://api.heclouds.com/devices/" + device_id
    headers={'api-key':'2kJV69exxxxxxxxx'}
    res = requests.get(url, headers=headers)
    name = res.json()['data']['auth_info']
    online_status = res.json()['data']['online']
    last_time = res.json()['data']['last_ct']
    print("{}: online: {}, last_time: {}".format(name, online_status, last_time))

if __name__ == "__main__":
    check_status()
    if len(sys.argv) > 1 and sys.argv[1] == "check":
        exit()

    val = float(sys.argv[1]) if len(sys.argv) > 1 else 2
    send(val = val)
    check_status()

    print("ok")


