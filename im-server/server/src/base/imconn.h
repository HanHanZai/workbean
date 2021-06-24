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

class CImConn : public CRefObject
{
public:
	CImConn();
	virtual ~CImConn();

	bool IsBusy() { return m_busy; }
	int SendPdu(CImPdu* pPdu) { return Send(pPdu->GetBuffer(), pPdu->GetLength()); }
	int Send(void* data, int len); /* 沾包问题的出现 */

	virtual void OnConnect(net_handle_t handle) { m_handle = handle; }
	virtual void OnConfirm() {}
	virtual void OnRead();
	virtual void OnWrite();
	virtual void OnClose() {}
	virtual void OnTimer(uint64_t curr_tick) {}
    virtual void OnWriteCompelete() {};

	/* 处理PDU事件 */
	virtual void HandlePdu(CImPdu* pPdu) {}

protected:
	net_handle_t	m_handle;
	bool			m_busy;

	string			m_peer_ip;
	uint16_t		m_peer_port;
	CSimpleBuffer	m_in_buf; //输入缓存
	CSimpleBuffer	m_out_buf; //输出缓存

	bool			m_policy_conn;
	uint32_t		m_recv_bytes;
	uint64_t		m_last_send_tick;
	uint64_t		m_last_recv_tick;
    uint64_t        m_last_all_user_tick;
};

typedef hash_map<net_handle_t, CImConn*> ConnMap_t;
typedef hash_map<uint32_t, CImConn*> UserMap_t;

void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam);
void ReadPolicyFile();

#endif /* IMCONN_H_ */