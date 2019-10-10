# StServerFrame

#### 介绍
基于epoll实现的单线程网络服务端框架

#### 软件架构

- Core   - 存放net相关的文件 TcpServer TcpConnection Listener TcpSession等
- Event  - 存放网络事件相关文件 EventModule EventLoop Channel 等
- UserDef - 存放业务层文件 新建的业务层服务 EchoServer， 继承自 TcpSession的 EchoSession等


```
 网络事件通知模块：
     EventLoop
       - EpollModule
       - Channel
 应用层：
     TcpServer:
       - TcpConnection ： tcp连接类
         - TcpSession ：业务层Session需要集成的类
       - Listener ：监听类
       - EventLoop : 所属事件循环
```




#### 安装教程

- 1 git clone git@gitee.com:storm_zy/StServerFrame.git
- 2 cd StServerFrame
- 3 sh makeall.sh
- 4 cd bin
- 5 sh runall.sh - 开启
- 6 sh closeall.sh - 关闭



#### 使用说明
1. bin/cfg.ini 是配置端口 里面的listen_port即是程序中监听的端口
2. bin目录放的是运行所需要的文件 包括编译后生成的可执行文件  EchoServer.out

------------------------------------------------------------------

```
欢迎关注 [ **懒人漫说** ] 公众号，分享Java、Android、C/C++ 技术，包括基础、自己遇到的问题解决过程。
当然如果关注并留言问题的话，我们力所能及的话会帮你解决并回复哟。我们和你一样，是正在成长的程序员，
我们也会分享自己的成长路上的感想，希望可以和你一起努力成长。
```
![输入图片说明](https://images.gitee.com/uploads/images/2019/1010/164417_77823785_5001667.png "qrcode_min.png")