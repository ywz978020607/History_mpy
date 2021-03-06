# 主仓库在History下，为了解耦降仓库体积，硬件端文件单独成仓库，支持请到History主仓库提Star～

# 单片机系列之mpy与arduino选择  
对于stm32来说，arduino不是很稳，很多库残缺，建议直接买mpy适配的型号，或换成avr等单片机用arduino开发  
而对于esp系列来说，分为两种
- esp8266
  8266用的比较多的有8266-01(s) 、 8266-12F模组小开发板(如D1mini等) 以及8266-nodemcu相对较大版本。后两者区别不大，8266-01能用的引脚非常少，且需要上下拉电阻(我之前有单独画过精简底板，但焊接麻烦难量产)。8266使用mpy能够快速开发，但存在一些缺点，8266本身性能差跑mpy不是非常快；都支持软串口。因此，如果对性能有要求(如不需要联网，仅作为逻辑开关使用)可以考虑使用arduino。但mpy也推荐，特别作为联网透传模块来用，使用webrepl能够快速调试透传，值得注意是后续需要对长时间运行后断网/断订阅重连进行fix，这个过程并不会因为使用arduino就会稳而可以忽略，对于稳定运行，二者都需要考虑异常处理。
- esp32
  esp32性能强大，目前有esp32、esp32-s2/c系列、esp32-s3等。micropython对esp32系列非常推荐，已支持软串口，但存在蓝牙功能不完善的缺点。考虑到无论是mpy还是arduino，对于蓝牙和wifi一般都不太容易兼得(天线&内存设计)，故在有蓝牙需求时，优先使用arduino，能够获得更加稳定完善的功能，其他物联网需求下强推micropython.


对于Micropython库各种用法及组件、arduino-esp32-ble库可参考github搜索History_mpy仓库。

# 软串口源码&编译：
- esp32: https://github.com/ywz978020607/micropython/tree/esp32soft
- esp8266: https://github.com/ywz978020607/micropython/tree/8266soft
