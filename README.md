# poor_client
一个小工具集

文件目录树：
.
├── CMakeLists.txt
├── CMakeLists.txt.user
├── data(添加的自定义文件，例如日志文件，静态文件等，在编译后需要放在可执行文件(exe，sh)的同级目录：bulid-debug或bulid-relest文件夹下)
│   └── terminal(终端相关文件，例如指令记录等)
│       └── home
│           └── root
├── feature(功能实现和处理文件)
│   ├── terminal(终端)
│   │   ├── bin(终端专属指令实现)
│   │   │   ├── sl.cpp(显示小火车)
│   │   │   └── sl.h(显示小火车)
│   │   ├── command.cpp(指令处理)
│   │   ├── command.h(指令处理)
│   │   ├── feature.cpp
│   │   ├── feature.h
│   │   └── terminal_bin
│   └── tools(工具集)
│       ├── convertor(转换器)
│       │   └── timestamp(时间戳转换)
│       │       ├── timestamp.cpp
│       │       └── timestamp.h
│       └── encipher(加密)
│           ├── base64.cpp(文件转base64)
│           ├── base64.h
│           ├── md5.cpp(md5加密)
│           └── md5.h
├── main.cpp(主函数)
├── mainwindow.cpp(主界面)
├── mainwindow.h
├── overloading(库文件重写)
│   └── terminal(终端相关)
│       ├── plain_text_edit.cpp(重写QPlainTextEdit库，用来显示终端界面)
│       └── plain_text_edit.h
├── README.md(项目描述文件)
├── server(服务器通信相关)
│   ├── client_network.cpp(服务器通信管理)
│   ├── client_network.h
│   ├── uploads(用户文件上传)
│   │   ├── useruploadsmanager.cpp(用户文件上传文件)
│   │   └── useruploadsmanager.h
│   └── user(用户活动相关)
│       ├── user_manager.cpp(用户登录，注册管理)
│       └── user_manager.h
└── ui(ui文件)
    ├── file(云盘功能)
    │   ├── fileuploads.cpp
    │   └── fileuploads.h
    ├── terminal(终端界面)
    │   ├── terminal.cpp
    │   └── terminal.h
    ├── tools(工具集界面)
    │   ├── convertor(转换器界面)
    │   │   ├── convertor.cpp(转换器界面)
    │   │   ├── convertor.h
    │   │   └── timestamp(时间戳界面)
    │   │       ├── timestamp_ui.cpp
    │   │       └── timestamp_ui.h
    │   ├── tools.cpp(工具集界面)
    │   └── tools.h
    └── user(用户登录界面)
        ├── user_login_account.cpp(用户账号登录)
        ├── user_login_account.h
        ├── user_login_phone_code.cpp(用户手机登录)
        ├── user_login_phone_code.h
        ├── user_login_phone.cpp(用户验证码输入界面)
        └── user_login_phone.h
        

# 项目开发日志
今天是2024年6月18号，我决定给项目做一个开发日志，记录开发进程，开发难度，开发技术，顺带学习一下md文件（所以文末有md语法）
## TODO灵感收集
用来统计灵感，实现的功能放在最下面，并标记已实现，乱序
### 实现终端显示小火车：sl
做一个有趣的指令，一个小火车移动，同时能学习终端的显示处理和qt的显示处理
参数：-F：每次刷新都让小火车向上一行
	-l：小一点的小火车

### 实现云盘功能
将数据放在云端，学习网络文件传输，文件切片，压缩等
数据放在云端
文件路径：使用映射路径，数据库保存两份文件路径，一份是映射路径(客户端显示，客户端存储)，一份是实际文件路径(服务器保存路径)
	数据库中的路径保存文件所在的文件夹，不保存文件名：path:/user/年/月/
	允许用户在客户端修改文件夹和移动位置，实际修改的路径是显示路径，不影响实际路径
	在客户端建立映射路径对应的文件夹，用来保存对应文件
	实际路径：
		用户名/年/月/文件
	显示路径：
		用户名/主文件夹/照片
文件移动：客户进行的文件移动，只修改虚拟路径
	文件移动时，在客户端原文件位置显示一个灰色虚影，点击提示移动后的路径
处理同名文件：服务器保存文件时使用 文件ID 进行区分，保证服务器不会出现同名文件
文件上传时保存数据：文件ID，文件原始名，文件格式，上传时间，虚拟文件路径，实际文件路径，文件状态(上传中，已上传，被删除，被屏蔽，损坏，丢失)
#### 图片
开发者同时是一名摄影爱好者，所以对图片有特殊要求
所以文件上传对图片有特殊处理：使用独立数据库，文件夹保存
客户端查看图片时使用懒加载
	懒加载(显示低分辨率照片用于预览，点击图片：加载高清图片，下载图片：下载原件，下载raw：如果有就下载对应raw文件)
图片的详细信息(拍摄设备，参数等)在上传时在客户端使用工具进行检查，然后传给服务器
图片上传时保存数据：图片ID，文件原始名，文件格式，上传时间，机身，镜头，焦段，光圈，快门，ISO，拍摄时间，拍摄地点，虚拟文件路径，实际文件路径，文件状态(上传中，已上传，被删除，被屏蔽，损坏，丢失)
#### 其他文件


## 开发进程
### 项目开端到2024-6-18
项目应该开始于2024年5月15日前后，项目立项是想做一个面试用的demo，也可以做一个代码范例库，方便以后用到类似功能做一个速查
目前暂定服务器端使用qt框架，考虑到qt框架在服务器端并不主流，后期会转用其他的c++框架
6-18：决定并编写开发日志
	为了理清思路，收集随时产生的灵感，督促开发等原因决定编写日志
	日志选择使用倒序，方便第一时间看到最新的进度
6-17：决定windows和linux环境的构建方式
	由于项目在windows和ubuntu(linux)下同时编写运行
	windows下使用cmake+minGW进行编译(主要是客户端部分)
	linux环境下使用cmake+gcc进行编译(主要是服务器部分)
6-14：决定了客户端的文件结构如下
	不使用 ui文件：因为重构类名和文件名时，不会重构对应的 ui文件，导致每次重构 ui文件都会异常
	将界面文件(ui)，功能文件(feature)，库文件重写(overloading)分成三个文件夹：实现功能和界面上的分离
		功能文件(feature)可能会比较杂，包括各种算法，功能等都会放在这里，作为示例代码库
6-13：决定使用仿终端界面作为主要发力点：本意是做功能性学习，所以重点会放在服务器上


markdown语法

基本符号：* - + >

你只需要记住这四个基本符号就能控制文档的排版和样式，因为基本上所有的markdown标记都是基于这四个符号及其组合。需要注意的是，基本符号后有一个用于分割标记符和内容的空格。
标题（从大到小取决于#号的数量）

# 一级标题

## 二级标题

### 三级标题

#### 四级标题

##### 五级标题 
###### 六级标题

# 正文与段落
正文
换行以后直接开始书写，不用加任何符号。

段落
一个段落以一个自然的 回车 作为换行分隔。


# 字体样式

倾斜
*倾斜*

加粗
**加粗**

倾斜并加粗 
***倾斜并加粗***

文字删除线 
~~文字删除线~~

# 插入
引用
这是一段引用 > 引用内容

分隔符
连续输入三个以上的  --- 添加分隔符，下面就会出现一条横线：

## 列表

### 无序列表
使用 * - + 中的任何一个符号加空格就可以创建无序列表，但如我这样的强迫症患者完美主义者可以进一步使用+和-来表现层次关系。
* 这是一个无序列表
* 这是一个无序列表

+ 这是一个父无序列表
  - 这是一个子无序列表
  
### 有序列表
使用 数字+点+空格+内容 创建有序列表。
1. 这是一个有序列表
2. 这是一个有序列表
3. 这是一个有序列表
  1. 这是一个嵌套有序列表
  2. 这是一个嵌套有序列表
  3. 这是一个嵌套有序列表

列表可以嵌套，使用时在嵌套列表前按 tab 或 空格 来控制缩进。


## 链接
用markdown写下你的第一个md文档 的文章链接是这样构成的
[用markdown写下你的第一个md文档](http://www.jianshu.com/p/de9c98bba332) 
也可以直接用尖括号包含网址的方式
<http://www.jianshu.com>来直接构造一个可点击的链接 http://www.jianshu.com


## 图片
像构造一个链接一样，只需要在前面加！
![图片名称](图片地址)

## 代码
markdown也常用来编写代码文档，因此文档中引入代码必不可少。

### 代码块
使用 ` 符号包裹代码构建一个代码块，` 符号在左上角esc键下方，请在半角状态输入。
一些markdown工具支持更简单的按tab缩进方式。
` console.log('这是一个代码块'); `

### 代码段
使用三个 `
```
代码段
```
这样的方式来构建一个多行的代码段。
```
print("这是一个")
print("由markdowns标记符构成")
print("多行代码段")
```

## 表格

使用
- |符号把内容分割为你认为合适的表格样式就好。
使用 :符号标识对齐。

表头1|表头2|表头3
:----|:-----:|-----:
左对齐|居中对齐|右对齐




