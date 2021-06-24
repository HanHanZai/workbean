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

- on_IMGroup

| Field            | Type                | Null | Key  | Default | Extra |
| ---------------- | ------------------- | ---- | ---- | ------- | ----- |
| id               | int(11)             | No   | PRI  | NULL    | auto  |
| name             | varchar(255)        | No   | MUL  |         |       |
| avatar           | varchar(255)        | No   |      |         |       |
| creator          | int(11)unsigned     | No   | MUL  | 0       |       |
| type             | tinyint(3) unsigned | No   |      | 1       |       |
| userCnt          | int(11)unsigned     | No   |      | 0       |       |
| status           | tinyint(3)unsigned  | No   |      | 1       |       |
| version          | int(11)unsigned     | N0   |      | 1       |       |
| lastChated       | int(11)unsigned     | No   |      | 0       |       |
| updated          | int(11)unsigned     | No   |      | 0       |       |
| created          | int(11)unsigned     | No   |      | 0       |       |
| flag             | int(11)             | YES  |      | 0       |       |
| disable_send_msg | int(11)             | YES  |      | 0       |       |

- on_IMGroupMember

| Field        | Type               | Null | Key  | Default | Extra |
| ------------ | ------------------ | ---- | ---- | ------- | ----- |
| id           | int(11)            | No   | PRI  | NULL    | auto  |
| groupId      | int(11)unsigned    | No   | MUL  | NULL    |       |
| userId       | int(11)unsigned    | No   | MUL  | NULL    |       |
| status       | tinyint(4)unsigned | No   |      | 1       |       |
| remak        | varchar(50)        | YES  |      | NULL    |       |
| created      | int(11)unsigned    | No   |      | 0       |       |
| updated      | int(11)unsigned    | No   |      | 0       |       |
| disable_send | int(11)            | YES  |      | 0       |       |
| role         | int(11)            | YES  |      | 0       |       |
| flag         | int(11)            | YES  |      | 0       |       |

- on_IMMessage

| Field    | Type               | Null | Key  | Default | Extra |
| -------- | ------------------ | ---- | ---- | ------- | ----- |
| id       | int(11)            | No   | PRI  | NULL    | auto  |
| relateId | int(11)unsigned    | No   | MUL  | NULL    |       |
| fromId   | int(11)unsigned    | No   | MUL  | NULL    |       |
| toId     | int(11)unsigned    | No   |      | NULL    |       |
| msgId    | int(11)unsigned    | No   |      | NULL    |       |
| content  | varchar(4096)      | YES  |      |         |       |
| type     | tinyint(2)unsigned | No   |      | 1       |       |
| status   | tinyint(1)unsigned | No   |      | 0       |       |
| updated  | int(11)unsigned    | No   |      | NULL    |       |
| created  | int(11)unsigned    | No   |      | NULL    |       |
| flag     | int(11)            | YES  |      | 0       |       |

- on_IMMessage_collect

| Field     | Type         | Null | Key  | Default | Extra |
| --------- | ------------ | ---- | ---- | ------- | ----- |
| id        | bigint(11)   | No   | PRI  | NULL    | auto  |
| uid       | bigint(11)   | No   |      | NULL    |       |
| send_uid  | bigint(11)   | No   |      | NULL    |       |
| send_name | varchar(128) | No   |      | NULL    |       |
| send_time | varchar(128) | No   |      | NULL    |       |
| text      | varchar(512) | No   |      | NULL    |       |
| status    | tinyint(2)   | No   |      | 1       |       |
| add_time  | datetime     | No   |      | NULL    |       |

- on_IMOnline

| Field      | Type       | Null | Key  | Default | Extra |
| ---------- | ---------- | ---- | ---- | ------- | ----- |
| uid        | bigint(20) | No   | PRI  | NULL    |       |
| appid      | int(11)    | YES  |      | NULL    |       |
| status     | int(255)   | YES  |      | 2       |       |
| updatetime | bigint(11) | YES  |      | 0       |       |

- on_IMPush_notice

| Field    | Type         | Null | Key  | Default | Extra |
| -------- | ------------ | ---- | ---- | ------- | ----- |
| id       | bigint(11)   | No   | PRI  | NULL    | auto  |
| title    | varchar(125) | No   |      | NULL    |       |
| content  | varchar(255) | No   |      | NULL    |       |
| add_time | datetime     | No   |      | NULL    |       |

- on_IMQRLogin

| FIeld     | Type        | Null | Key  | Default | Extra |
| --------- | ----------- | ---- | ---- | ------- | ----- |
| id        | int(11)     | No   | PRI  | NULL    | auto  |
| code      | varchar(50) | YES  |      | NULL    |       |
| uid       | int(11)     | YES  |      | NULL    |       |
| logintime | datetime    | YES  |      | NULL    |       |

- on_IMRecentSession

| Field     | Type        | Null | Key  | Default | Extra |
| --------- | ----------- | ---- | ---- | ------- | ----- |
| id        | int(11)     | No   | PRI  | NULL    | auto  |
| code      | varchar(50) | YES  |      | NULL    |       |
| uid       | int(11)     | YES  |      | NULL    |       |
| logintime | datetime    | YES  |      | NULL    |       |

- on_IMRobot

| Field      | Type    | Null | Key  | Default | Extra |
| ---------- | ------- | ---- | ---- | ------- | ----- |
| id         | int(11) | No   | PRI  | NULL    | auto  |
| uid        | int(11) | YES  |      | NULL    |       |
| ctype      | int(11) | YES  |      | NULL    |       |
| groupId    | int(11) | YES  |      | NULL    |       |
| status     | int(11) | YES  |      | NULL    |       |
| role       | int(11) | YES  |      | NULL    |       |
| lv         | int(11) | YES  |      | NULL    |       |
| updatetime | int(11) | YES  |      | NULL    |       |

- on_IMTransmitFile

| Filed      | Type     | Null | Key  | Default | Extra |
| ---------- | -------- | ---- | ---- | ------- | ----- |
| id         | int(11)  | No   | PRI  | NULL    | auto  |
| uid        | int(11)  | YES  |      | NULL    |       |
| ctype      | int(11)  | YES  |      | NULL    |       |
| groupid    | int(11)  | YES  |      | NULL    |       |
| status     | int(11)  | YES  |      | NULL    |       |
| role       | int(11)  | YES  |      | NULL    |       |
| lv         | int(11)  | YES  |      | NULL    |       |
| updatetime | datetime | YES  |      | NULL    |       |

- on_IMUser

| Field              | Type               | Null | Key  | Default | Extra |
| ------------------ | ------------------ | ---- | ---- | ------- | ----- |
| id                 | bigint(20)unsigned | No   | PRI  | NULL    | auto  |
| appId              | int(11)            | YES  |      | 0       |       |
| outId              | int(11)            | YES  |      | 0       |       |
| username           | varchar(32)        | No   | MUL  |         |       |
| password           | varchar(32)        | No   |      |         |       |
| salt               | varchar(32)        | No   |      |         |       |
| api_token          | vartchar(32)       | YES  |      |         |       |
| nickname           | varchar(32)        | No   |      |         |       |
| realname           | varchar(50)        | YES  |      | NULL    |       |
| sex                | tinyint(1)unsigned | No   |      | 0       |       |
| avatar             | varchar(255)       | YES  |      |         |       |
| domain             | varchar(32)        | No   | MUL  |         |       |
| phone              | varchar(15)        | No   | MUL  |         |       |
| email              | varchar(64)        | No   |      |         |       |
| departId           | int(11)unsigned    | No   |      | NULL    |       |
| push_shield_status | tinyint(1)unsigned | No   |      | 0       |       |
| sign_info          | varcahr(128)       | No   |      |         |       |
| status             | int(11)unsigned    | YES  |      | 0       |       |
| updated            | int(11)unsigned    | No   |      | NULL    |       |
| created            | int(11)unsigned    | NO   |      | NULL    |       |
| data_sign          | varchar(128)       | YES  |      | NULL    |       |
| Longitude          | float(11,7)        | YES  |      | NULL    |       |
| Latitude           | float(11,7)        | YES  |      | NULL    |       |
| open_id            | varchar(100)       | YES  |      | NULL    |       |
| access_token       | varchar(200)       | YES  |      | NULL    |       |
| unionid            | varchar(50)        | YES  |      | NULL    |       |
| reg_from           | varchar(20)        | YES  |      | NULL    |       |
| code               | varchar(50)        | YES  |      | NULL    |       |
| topuid             | int(11)            | YES  |      | 0       |       |

- on_IMUserFriends

| Field     | Type         | Null | Key  | Default | Extra |
| --------- | ------------ | ---- | ---- | ------- | ----- |
| id        | bigint(20)   | No   | PRI  | NULL    | auto  |
| uid       | int(11)      | No   |      | NULL    |       |
| friuid    | int(11)      | No   |      | NULL    |       |
| friName   | varchar(50)  | YES  |      | NULL    |       |
| friAvatar | varchar(120) | YES  |      | NULL    |       |
| remark    | varchar(50)  | YES  |      | NULL    |       |
| groupId   | int(11)      | YES  |      | 1       |       |
| message   | varchar(100) | YES  |      | NULL    |       |
| status    | int(11)      | No   |      | NULL    |       |
| role      | int(11)      | YES  |      | NULL    |       |
| lv        | int(11)      | YES  |      | NULL    |       |
| updated   | int(11)      | YES  |      | NULL    |       |
| created   | int(11)      | YES  |      | NULL    |       |

- on_IMUserGeoData

| Field   | Type        | Null | Key  | Default | Extra |
| ------- | ----------- | ---- | ---- | ------- | ----- |
| id      | int(11)     | No   | PRI  | NULL    | auto  |
| uid     | int(11)     | YES  |      | NULL    |       |
| city    | varchar(50) | YES  |      | NULL    |       |
| lng     | float(11,7) | YES  |      | NULL    |       |
| lat     | float(11,7) | YES  |      | NULL    |       |
| uids    | text        | YES  |      | NULL    |       |
| data    | text        | YES  |      | NULL    |       |
| status  | int(11)     | YES  |      | 1       |       |
| updated | int(11)     | YES  |      | NULL    |       |

- on_IMUser_losure

| Field          | Type         | Null | Key  | Default | Extra |
| -------------- | ------------ | ---- | ---- | ------- | ----- |
| uid            | bigint(11)   | No   | PRI  | NULL    |       |
| closure_day    | int(11)      | No   |      | 0       |       |
| account_status | tinyint(2)   | No   |      | 1       |       |
| normal_data    | datetime     | YES  |      | NULL    |       |
| closure_data   | datatime     | YES  |      | NULL    |       |
| closure_reason | varchar(255) | YES  |      | NULL    |       |

- on_IMuserFriends_expand

| Field        | Type         | Null | Key  | Default | Extra |
| ------------ | ------------ | ---- | ---- | ------- | ----- |
| id           | bigint(11)   | No   | PRI  | NULL    | auto  |
| userId       | bigInt(11)   | No   |      | NULL    |       |
| frendId      | bigint(11)   | No   |      | NULL    |       |
| black_status | tinyint(2)   | No   |      | 1       |       |
| remark       | varchar(255) | YES  |      | NULL    |       |
| add_time     | datetime     | YES  |      | NULL    |       |

- on_bank

| Field | Type         | Null | Key  | Default | Extra |
| ----- | ------------ | ---- | ---- | ------- | ----- |
| id    | bigint(11)   | No   | PRI  | NULL    | auto  |
| name  | varchar(125) | No   |      |         |       |
| code  | varchar(125) | No   |      |         |       |
| css   | text         | YES  |      | NULL    |       |
| color | varchar(125) | YES  |      | NULL    |       |

- on_bankcard

| Field       | Type        | Null | Key  | Default | Extra |
| ----------- | ----------- | ---- | ---- | ------- | ----- |
| id          | int(11)     | No   | PRI  | NULL    | auto  |
| uid         | int(11)     | YES  |      | NULL    |       |
| type        | int(11)     | YES  |      | NULL    |       |
| bankname    | varchar(45) | YES  |      | NULL    |       |
| bankaddress | varchar(45) | YES  |      | NULL    |       |
| realname    | varchar(45) | YES  |      | NULL    |       |
| cardno      | varchar(45) | YES  |      | NULL    |       |
| role        | int(11)     | YES  |      | NULL    |       |
| status      | int(11)     | YES  |      | NULL    |       |
| lv          | int(11)     | YES  |      | NULL    |       |
| addtime     | datetime    | YES  |      | NULL    |       |

- on_luckymoney

| Field    | Type         | Null | Key  | Default | Extra |
| -------- | ------------ | ---- | ---- | ------- | ----- |
| id       | int(11)      | No   | PRI  | NULL    | auto  |
| senduid  | int(15)      | YES  |      | NULL    |       |
| type     | int(11)      | YES  |      | NULL    |       |
| type2    | int(11)      | YES  |      | NULL    |       |
| name     | varchar(45)  | YES  |      | NULL    |       |
| allmoney | decimal(8,2) | YES  |      | NULL    |       |
| usemoney | decimal(8,2) | YES  |      | NULL    |       |
| allnum   | int(11)      | YES  |      | 0       |       |
| usenum   | int(11)      | YES  |      | 0       |       |
| msg      | varchar(45)  | YES  |      | NULL    |       |
| status   | int(11)      | YES  |      | NULL    |       |
| lv       | int(11)      | YES  |      | NULL    |       |
| addtime  | datetime     | YES  |      | NULL    |       |
| groupId  | int(9)       | YES  |      | 0       |       |

- on_luckymoney_log

| Field   | Type         | Null | Key  | Default | Extra |
| ------- | ------------ | ---- | ---- | ------- | ----- |
| id      | int(11)      | No   | PRI  | NULL    | auto  |
| pid     | int(11)      | YES  |      | NULL    |       |
| uid     | int(15)      | YES  |      | NULL    |       |
| type    | int(11)      | YES  |      | NULL    |       |
| money   | decimal(8,2) | YES  |      | NULL    |       |
| lv      | int(11)      | YES  |      | NULL    |       |
| status  | int(11)      | YES  |      | NULL    |       |
| addtime | datetime     | YES  |      | NULL    |       |
| fuid    | int(11)      | YES  |      | 0       |       |
| flag    | int(11)      | YES  |      | 0       |       |

- on_order_list

| Field                | Type         | Null | Key  | Default | Extra |
| -------------------- | ------------ | ---- | ---- | ------- | ----- |
| id                   | bigint(11)   | No   | PRI  | NULL    | auto  |
| uid                  | bigint(11)   | No   |      | NULL    |       |
| username             | varchar(50)  | YES  |      | NULL    |       |
| ordid                | varchar(100) | No   |      | NULL    |       |
| ordtime              | datetime(6)  | YES  |      | NULL    |       |
| productid            | int(11)      | YES  |      | NULL    |       |
| ordtitle             | varchar(100) | YES  |      | NULL    |       |
| ordbuynum            | int(11)      | YES  |      | 0       |       |
| ordprice             | float(10,2)  | YES  |      | 0.00    |       |
| ordfee               | float(10,2)  | YES  |      | 0.00    |       |
| ordstatus            | int(11)      | YES  |      | 0       |       |
| payment_type         | varchar(50)  | YES  |      | NULL    |       |
| payment_trade_no     | varchar(100) | YES  |      | NULL    |       |
| payment_trade_status | varchar(50)  | YES  |      | NULL    |       |
| payment_notify_id    | varchar(1pp) | YES  |      | NULL    |       |
| payment_notify_time  | datetime(6)  | YES  |      | NULL    |       |
| payment_buyer_email  | varchar(100) | YES  |      | NULL    |       |
| payment_res          | varchar(500) | YES  |      | NULL    |       |
| ordcode              | varchar(100) | YES  |      | NULL    |       |
| isused               | int(11)      | YES  |      | 0       |       |
| usetime              | int(11)      | YES  |      | NULL    |       |
| checkuser            | int(11)      | YES  |      | NULL    |       |

- on_payLog

| Field    | Type         | Null | Key  | Default | Extra                       |
| -------- | ------------ | ---- | ---- | ------- | --------------------------- |
| id       | bigint(11)   | No   | PRI  | NULL    | auto                        |
| log      | varchar(255) | YES  |      | NULL    |                             |
| orderNo  | varchar(255) | No   |      | NULL    |                             |
| add_time | datetime     | No   |      | NULL    | on update CURRENT_TIMESTAMP |

- on_pay_transfer

| Field      | Type          | Null | Key  | Default | Extra |
| ---------- | ------------- | ---- | ---- | ------- | ----- |
| id         | bigint(11)    | No   | PRI  | NULL    | auto  |
| uid        | bigint(11)    | No   |      | NULL    |       |
| realName   | varchar(120)  | No   |      | NULL    |       |
| friendid   | bigint(11)    | No   |      | NULL    |       |
| friendName | varchar(120)  | YES  |      | NULL    |       |
| amount     | decimal(11,2) | YES  |      | NULL    |       |
| isNotify   | tinyint(255)  | YES  |      | 1       |       |
| add_time   | datetime      | YES  |      | NULL    |       |

- on_user_account

| Field           | Type         | Null | Key  | Default | Extra |
| --------------- | ------------ | ---- | ---- | ------- | ----- |
| uid             | int(11)      | No   | PRI  | NULL    |       |
| pay_password    | varchar(512) | YES  |      | NULL    |       |
| available_money | double(10,2) | YES  |      | NULL    |       |
| freeze_money    | double(10,2) | YES  |      | NULL    |       |
| realname        | varchar(20)  | YES  |      | NULL    |       |
| idcard          | varchar(20)  | YES  |      | NULL    |       |
| updatetiem      | datetime     | YES  |      | NULL    |       |

- on_user_bank

| Field         | Type         | Null | Key  | Default | Extra     |
| ------------- | ------------ | ---- | ---- | ------- | --------- |
| id            | bigint(11)   | No   | PRI  | NULL    | auto      |
| bank_id       | bigint(11)   | YES  |      | NULL    |           |
| bank_name     | varchar(125) | No   |      | NULL    |           |
| bank_code     | varchar(125) | No   |      | NULL    |           |
| card_type     | varchar(2)   | No   |      |         |           |
| card_no       | varchar(125) | No   |      | 1       |           |
| phone         | varchar(125) | YES  |      | NULL    |           |
| uid           | bigint(11)   | YES  |      | NULL    |           |
| bind_id       | varchar(125) | YES  |      | NULL    |           |
| bind_status   | varchar(125) | YES  |      | NULL    |           |
| bind_order_no | varchar(125) | YES  |      | NULL    |           |
| status        | tinyint(2)   | YES  |      | 1       |           |
| add_time      | datetime     | No   |      | NULL    |           |
| update_time   | datetime     | No   |      | NULL    | on update |

- on_userpaylog

| Filed    | Type         | Null | Key  | Default | Extra |
| -------- | ------------ | ---- | ---- | ------- | ----- |
| id       | int(11)      | No   | PRI  | NULL    | auto  |
| uid      | varchar(12)  | YES  |      | NULL    |       |
| allmoney | decimal(8,2) | YES  |      | NULL    |       |
| paytouid | varchar(12)  | YES  |      | NULL    |       |
| paymoney | decimal(8,2) | YES  |      | NULL    |       |
| paytype  | int(11)      | YES  |      | NULL    |       |
| paymsg   | varchar(50)  | YES  |      | NULL    |       |
| paytime  | datetime     | YES  |      | NULL    |       |
| orderno  | varchar(45)  | YES  |      | NULL    |       |
| lv       | int(11)      | YES  |      | 0       |       |
| status   | int(11)      | YES  |      | 0       |       |

### 消息传输所使用到的PB结构

| PB协议                                                       |
| ------------------------------------------------------------ |
| IM.BaseDefine  登录，用户列表，消息，群组，文件，切换服务器，其他结构 |
| IM.Buddy 用户结构                                            |
| IM.File 文件结构                                             |
| IM.Group 群组结构                                            |
| IM.Login 登录结构                                            |
| IM.Message 消息结构                                          |
| IM.Other 其他结构                                            |
| IM.Server 服务结构                                           |
| IM.SwitchService 切换服务器结构                              |

### 各个服务器的功能

```c++
1.ct_db_proxy_server:处理数据库相关的交互
2.ct_file_msg_server:处理文件传输的相关操作
3.ct_http_msg_server:处理群消息和成员相关消息
```









