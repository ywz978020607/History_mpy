# micropython库

## 1. 增加软串口，esp32已通过，感谢大佬鼠源码。
增加主要只增加modserial.c即可，其他增加开关及配置：参考http://micropython.com.cn/en/latet/develop/library.html

[MR链接](https://github.com/micropython/micropython/pull/8827/files#diff-750438eae3e1bb61aa412395c72b063d0641bac6401199f0b543c9cfd7b76742)

有了软串口，8266及esp32可以在更多场景使用mpy~

https://github.com/ywz978020607/micropython/tree/esp32soft
https://github.com/ywz978020607/micropython/tree/8266soft
