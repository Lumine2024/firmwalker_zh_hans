# firmwalker 汉化版
一个简单的bash/C++脚本，用于搜索提取或挂载的固件文件系统。

它将在提取或挂载的固件文件系统中搜索可能引发固件不安全问题的东西，例如：

* etc/shadow和etc/passwd
* etc/ssl目录
* 搜索SSL相关文件，如.pem，.crt等。
* 搜索配置文件
* 查找脚本文件
* 搜索其他.bin文件
* 查找关键字，如admin， password， remote等。
* 搜索物联网设备上使用的常用web服务器
* 搜索常用的二进制文件，如ssh， tftp， dropbear等。
* 搜索url，电子邮件地址和IP地址

## 使用
* 使用方法：
  * shell版：`./firmwalker linksys/fmk/rootfs ../firmwalker.txt`
  * c++版：（c++标准至少要在c++17，否则`filesystem`库没法用）
    * 直接编译源文件：
      ```shell
      g++ -std=c++23 firmwalker.cpp -o firmwalker
      ./firmwalker linksys/fmk/rootfs ../firmwalker.txt
      ```
    * 使用CMake
      ```shell
      mkdir build
      cd build
      cmake ..
      make
      ./firmwalker linksys/fmk/rootfs ../firmwalker.txt
      ```
  * 跑可执行文件时，第二个参数为可选项，默认为`firmwalker.txt`
* 如果打不开，请执行`chmod +x firmwalker.sh`
* 除非你指定一个不同的文件名作为第二个参数，否则文件`firmwalker.txt`将被创建在与脚本文件相同的目录下
* 不要把`firmwalker.sh`文件放在要搜索的目录中，这会导致脚本搜索自己和它正在创建的文件

## 示例文件
- https://1drv.ms/f/s!AucQMYXJNefdvGZyeYt16H72VCLv
它包含以下内容：
* squashfs-root.zip - 包含随机提取的路由器固件文件。Firmwalker可以在这个文件系统上运行。
* rt-ac66u.txt - firmwalker输出文件
* xc.txt - Ubiquiti设备的firmwalker输出文件

### 相关链接
* https://craigsmith.net
* https://woktime.wordpress.com
* https://www.owasp.org/index.php/OWASP_Internet_of_Things_Project#tab=Firmware_Analysis
