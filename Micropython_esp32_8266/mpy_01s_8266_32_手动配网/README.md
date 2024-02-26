# 手动配网终极方案-不要求固件，esp01(1Mflash)也可运行！

## 方案一：scan版
```
import wifimgr
wlan = wifimgr.get_connection()
```

## 方案二：手动输入版(no scan)
为了避免内存爆炸（测试发现），将判断直连和开启ap配网分开，调用方式为：
```
import wifimgr_noscan
wlan = wifimgr_noscan.get_connection()

import wifimgr2
wlan = wifimgr2.get_connection()
```



# Related:
注意：发现所有的参考代码里有个bug，do_connect判断连接后返回None，回去之后却判断True False，均加以改正。
- https://github.com/tayfunulu/WiFiManager
- https://zhuanlan.zhihu.com/p/369017239