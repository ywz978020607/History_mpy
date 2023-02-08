# micropython库

## 1. 增加软串口，esp32已通过，感谢大佬鼠源码。
增加主要只增加modserial.c即可，其他增加开关及配置：参考http://micropython.com.cn/en/latet/develop/library.html
有了软串口，8266及esp32可以在更多场景使用mpy~

- esp32 https://github.com/ywz978020607/micropython/tree/esp32soft 
- esp32s2/s3/c3等衍生系列代码相同
- esp8266 https://github.com/ywz978020607/micropython/tree/8266soft

固件见子目录