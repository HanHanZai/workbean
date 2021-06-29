/*
 * imconn.h
 *
 *  Created on: 2013-6-5
 *      Author: ziteng
 */

#ifndef IMCONN_H_
#define IMCONN_H_

#include "netlib.h"
#include "util.h"
#include "ImPduBase.h"

/* 服务器心跳检测时间 */
#define SERVER_HEARTBEAT_INTERVAL	5000
/* 服务器超时间隔 */
#define SERVER_TIMEOUT				30000
/* 客户端心跳检测时间间隔 */
#define CLIENT_HEARTBEAT_INTERVAL	30000
/* 客户端超时 */
#define CLIENT_TIMEOUT				120000
/* 移动端超时 */
#define MOBILE_CLIENT_TIMEOUT       60000 * 5
/* 读入的内存大小 */
#define READ_BUF_SIZE	2048

/* 基础连接对象 */
class CImConn : public CRefObject
{
public:
	CImConn();
	virtual ~CImConn();

	/* 忙碌标记 */
	bool IsBusy() { return m_busy; }
	/* 发送PDU数据 */
	/* CImPdu是数据封装的内容 */
	int SendPdu(CImPdu* pPdu) { return Send(pPdu->GetBuffer(), pPdu->GetLength()); }
	/* 发送数据 */
	int Send(void* data, int len); /* 沾包问题的出现 */

	/* handle是对应的socket对象 */
	virtual void OnConnect(net_handle_t handle) { m_handle = handle; }
	/* 连接确认 */
	virtual void OnConfirm() {}
	/* 读取数据 */
	virtual void OnRead();
	/* 写入数据 */
	virtual void OnWrite();
	/* 关闭连接 */
	virtual void OnClose() {}
	/* 处理定时任务,这个是callback嘛 */
	virtual void OnTimer(uint64_t curr_tick) {}
	/* 数据写入完成后的操作 */
    virtual void OnWriteCompelete() {};

	/* 作为服务器接收到的消息,处理PDU事件 */
	virtual void HandlePdu(CImPdu* pPdu) {}

protected:
	/* 连接的套接字对象 */
	net_handle_t	m_handle;
	/* 是否忙碌 */
	bool			m_busy;

	/* 远端ip */
	string			m_peer_ip;
	/* 远端端口 */
	uint16_t		m_peer_port;
	CSimpleBuffer	m_in_buf; //输入缓存
	CSimpleBuffer	m_out_buf; //输出缓存

	bool			m_policy_conn;
    uint64_t        m_last_all_user_tick;

	/* 接收到的字节 */
	uint32_t		m_recv_bytes;
	/* 上一次发送时间 */
	uint64_t		m_last_send_tick;
	/* 上一次接收时间 */
	uint64_t		m_last_recv_tick;
};

typedef hash_map<net_handle_t, CImConn*> ConnMap_t;
typedef hash_map<uint32_t, CImConn*> UserMap_t;

/* 每个连接对应绑定的回调函数 */
void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam);
void ReadPolicyFile();

#endif /* IMCONN_H_ */
