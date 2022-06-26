# arduino-作为esp32-ble时的补充

## mockmouse - test1文件夹
躺在床上用ipad不太方便，想到使用esp32+摇杆实现鼠标模拟+滚轮模拟，方便看电子书等操作



# arduino安装
esp32
```
https://blog.csdn.net/solar_Lan/article/details/123334948  
为了兼容s3 最好安装arduino-1.8.19
https://github.com/espressif/arduino-esp32  下载zip文件夹解压-速度效率最高

打开arduino下的hardware文件夹，在hardware文件夹中创建一个espressif文件夹
将git clone的文件夹或从zip压缩包中解压出的文件夹移动到espressif文件夹中，并重命名为esp32
打开esp32文件夹，打开tool文件夹，找到get.exe并双击运行，等待其运行结束。（最好在powershell执行，能看到报错信息，开一下全局梯子）
运行Arduino IDE，即可在工具>开发板>ESP32 Arduino选项栏中看到ESP32-S3板型
通过usb连接ESP32-S3开发板，在选择板型后，在工具>端口选项栏中选择对应的COM端口，另外也可在工具选项栏中设置开发板各项参数。

添加库：
库文件的github下载相应的zip文件，在arduino中选择从zip加载库，直接导入即可。
eg：
ble-鼠标模拟库 https://github.com/T-vK/ESP32-BLE-Mouse
mqtt-pubsubclient https://github.com/knolleary/pubsubclient.git  （也可以直接在库管理中搜索安装）
```
esp8266
```
首选项的附加管理中添加
http://arduino.esp8266.com/stable/package_esp8266com_index.json
之后重启打开开发板管理器-搜索8266，安装即可
```
