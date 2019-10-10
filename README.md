# StServerFrame

#### 介绍
基于epoll实现的单线程网络服务端框架

#### 软件架构


- Core   - 存放net相关的文件 TcpServer TcpConnection Listener TcpSession等
- Event  - 存放网络事件相关文件 EventModule EventLoop Channel 等
- UserDef - 存放业务层文件 新建的业务层服务 EchoServer， 继承自 TcpSession的 EchoSession等

- 网络事件通知模块：
-     EventLoop
-       - EpollModule
-       - Channel
- 应用层：
-     TcpServer:
-       - TcpConnection ： tcp连接类
-         - TcpSession ：业务层Session需要集成的类
-       - Listener ：监听类
-       - EventLoop : 所属事件循环
- 


#### 安装教程

- 1 git clone git@gitee.com:storm_zy/StServerFrame.git
- 2 cd StServerFrame
- 3 sh makeall.sh
- 4 cd bin
- 5 sh runall.sh - 开启
- 6 sh closeall.sh - 关闭



#### 使用说明
bin目录下的 cfg.ini 里面有对端口的配置，将port字段改为想要本服务监听的端口，
然后在客户端用tcp连接该端口即可。

