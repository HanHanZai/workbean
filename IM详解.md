### IM系统详解

- 代码结构

```c++
1.base:基础功能相关模块，包括协议，共享锁等相关内容
2.ct_db_proxy_server db相关协议层
3.ct_file_server 文件相关协议层
4.ct_http_msg_server http消息服务器
5.ct_msfs 高性能互连网络中的MSFS集成调度算法
6.ct_push_server 
7.ct_route_server
8.hiredis c库继承redis
9.libsecurity 加密库
10.log4cxx 日志库
11.protobuf pb协议库
12.slog 封装了log4cxx
    
ssh root@106.13.231.243 mars@112358#2021#6#1
```

### 功能接口类

```c++
1.CRefObject，引用类
2.Singleton，单例泛化类，只支持默认构造的对象
3.CThreadPool CWorkerThread 线程池 CThreadNotify 条件变量对象 CTask接口类对象，重载run方法即可
CThreadPool has cWorkerThread and CThreadNotify CWorkerThread has CTask 
4.CEventDispatch 时间分发类
5.CBaseSocket 基本的socket连接对象，通过CEventDispatch事件进行整体监听
netlib.h/netlib.cpp 提供非友非成员函数进行调用 ostype.h 一些宏定义和对应的依赖绑定
...
```

### 数据库表结构

- on_IMAdmin

| field   | type                  | NULL | Key  | default | Extra |
| :------ | --------------------- | ---- | ---- | ------- | :---- |
| id      | mediumint(6) unsigned | No   | PRI  | NULL    | auto  |
| uname   | varchar(40)           | No   |      | NULL    |       |
| pwd     | char(32)              | No   |      | NULL    |       |
| status  | tinyint(2) unsigned   | No   |      | 0       |       |
| created | int(11) unsigned      | No   |      | 0       |       |
| updated | int(11) unsigned      | No   |      | 0       |       |

- on_IMApp_version

| Field       | Type         | NULL | Key  | Default | Extra |
| ----------- | ------------ | ---- | ---- | ------- | ----- |
| id          | bigint(11)   | No   | PRI  | NULL    | auto  |
| file_name   | varchar(255) | YES  |      | NULL    |       |
| version     | varchar(255) | YES  |      | NULL    |       |
| update_type | tinyint(2)   | YES  |      | NULL    |       |
| remark      | varchar(255) | YES  |      | NULL    |       |
| add_time    | datetime     | No   |      | NULL    |       |

- on_IMAudio

| Field    | Type             | NULL | Key  | Default | Extra |
| -------- | ---------------- | ---- | ---- | ------- | ----- |
| id       | int(11)          | No   | PRI  | NULL    | auto  |
| fromId   | int(11) unsigned | No   | MUL  | NULL    |       |
| toId     | int(11)unsigned  | No   |      | NULL    |       |
| path     | varchar(255)     | YES  |      |         |       |
| size     | int(11)unsigned  | No   |      | 0       |       |
| duration | int(11)unsigned  | No   |      | 0       |       |
| created  | int(11)unsigned  | No   |      | NULL    |       |

- on_IMAudioReplay

| Filed    | Type         | Null | Key  | Default | Extra |
| -------- | ------------ | ---- | ---- | ------- | ----- |
| id       | bigint(6)    | No   | PRI  | NULL    | auto  |
| key_word | varchar(255) | No   |      | NULL    |       |
| reply    | varchar(255) | No   |      | NULL    |       |
| uid      | bigint(6)    | No   |      | 0       |       |
| status   | tinyint(2)   | No   |      | 1       |       |
| add_time | datetime     | No   |      | NULL    | auto  |

- on_IMDepart

| Filed      | Type            | Null | Key  | Default | Extra |
| ---------- | --------------- | ---- | ---- | ------- | ----- |
| id         | int(11)unsigned | No   | PRI  | NULL    | auto  |
| uid        | int(20)         | No   |      | 0       |       |
| departName | varchar(64)     | No   | MUL  |         |       |
| priority   | int(11)unsigned | No   | MUL  | 0       |       |
| parentId   | int(11)unsigned | No   |      | NULL    |       |
| status     | int(11)unsigned | No   |      | 0       |       |
| created    | int(11)unsigned | No   |      | NULL    |       |
| updated    | int(11)unsigned | No   |      | NULL    |       |

- on_IMDiscovery

| Field        | Type             | Null | Key  | Default | Extra |
| ------------ | ---------------- | ---- | ---- | ------- | ----- |
| id           | int(11) unsigned | No   | PRI  | NULL    | auto  |
| itemName     | varchar(64)      | No   | MUL  |         |       |
| itemUrl      | varchar(64)      | No   |      |         |       |
| itemPriority | int(11)unsigned  | No   |      | NULL    |       |
| status       | int(11)unsigned  | No   |      | 0       |       |
| created      | int(11)unsigned  | No   |      | NULL    |       |
| updated      | int(11)unsigned  | No   |      | NULL    |       |



```c++
on_IMAdmin
on_IMApp_version
on_IMAudio
on_IMAutoReplay
on_IMDepart
on_IMDiscovery
on_IMGroup
on_IMGroupMember
on_IMGroupMessage
on_IMMessage
on_IMMessage_collect
on_IMOnline
on_IMPush_notice
on_IMQRLogin
on_IMRecentSession
on_IMRobot
on_IMTransmitFile
on_IMUser
on_IMUserFriend
on_IMUserGeoData
on_IMUser_losure
on_IMuserFriends_expand
on_bank
on_bankcard
on_luckymoney
on_luckymoney_log
on_order_list
on_payLog
on_pay_transfer
on_user_account
on_user_bank
on_userpaylog
```



