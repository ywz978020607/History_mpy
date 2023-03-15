import network
import socket
import ure
import time
import machine 

NETWORK_PROFILES = 'wifi.dat'

wlan_ap = network.WLAN(network.AP_IF)
wlan_ap.active(False)
wlan_sta = network.WLAN(network.STA_IF)

server_socket = None

def get_connection():
    """return a working WLAN(STA_IF) instance or None"""

    # First check if there already is any connection:
    if wlan_sta.isconnected():
        return wlan_sta

    connected = False
    try:
        # ESP connecting to WiFi takes time, wait a bit and try again:
        time.sleep(3)
        if wlan_sta.isconnected():
            return wlan_sta

        # Read known network profiles from file
        profiles = read_profiles()

        # Search WiFis in range
        wlan_sta.active(True)
        networks = wlan_sta.scan()

        AUTHMODE = {0: "open", 1: "WEP", 2: "WPA-PSK", 3: "WPA2-PSK", 4: "WPA/WPA2-PSK"}
        # for ssid in profiles:
        #     password = profiles[ssid]
        #     connected = do_connect(ssid, password)
        #     if connected:
        #         break
        if not wlan_sta.isconnected():
            for ssid, bssid, channel, rssi, authmode, hidden in sorted(networks, key=lambda x: x[3], reverse=True):
                ssid = ssid.decode('utf-8')
                encrypted = authmode > 0
                print("ssid: %s chan: %d rssi: %d authmode: %s" % (ssid, channel, rssi, AUTHMODE.get(authmode, '?')))
                if encrypted:
                    if ssid in profiles:
                        password = profiles[ssid]
                        connected = do_connect(ssid, password)
                    else:
                        print("skipping unknown encrypted network")
                else:  # open
                    # connected = do_connect(ssid, None)
                    pass
                if connected:
                    break
    except OSError as e:
        print("exception", str(e))

    # start web server for connection manager:
    if not connected:
        wlan_sta.active(False)
        import wifimgr_noscan_utils
        while True:
            if not wifimgr_noscan_utils.check_wlan_connected():
                wifimgr_noscan_utils.startAP()
            else:
                machine.reset()
        # startAP()
        # machine.reset()
        """
        import xxxx
        while True:
            if not xxxx.check_wlan_connected():
                xxxx.startAP()
            else:
                xxxx.home()
        """
    return wlan_sta if connected else None

def do_connect(ssid, password):
    wlan_sta.active(True)
    if wlan_sta.isconnected():
        return True
    print('Connect to %s' % ssid)
    wlan_sta.connect(ssid, password)
    for retry in range(200):
        connected = wlan_sta.isconnected()
        if connected:
            break
        time.sleep(0.1)
        print('.', end='')
    if connected:
        print('\nConnected : ', wlan_sta.ifconfig())
    else:
        print('\nFailed. Not Connected to: ' + ssid)
    return connected

def read_profiles():
    with open(NETWORK_PROFILES) as f:
        lines = f.readlines()
    profiles = {}
    for line in lines:
        ssid, password = line.strip("\n").split(";")
        profiles[ssid] = password
    return profiles


def write_profiles(profiles):
    lines = []
    for ssid, password in profiles.items():
        lines.append("%s;%s\n" % (ssid, password))
    with open(NETWORK_PROFILES, "w") as f:
        f.write(''.join(lines))
