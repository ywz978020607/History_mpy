# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import uos, machine
#uos.dupterm(None, 1) # disable REPL on UART(0)
import gc
import wifimgr
# import   webrepl


wlan = wifimgr.get_connection()

# webrepl.start()

gc.collect()

from test1 import *
# from test_c3 import *
run()