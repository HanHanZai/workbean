/*
 *  a wrap for non-block socket class for Windows, LINUX and MacOS X platform
 */

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "ostype.h"
#include "util.h"

enum
{
	SOCKET_STATE_IDLE, //初始化状态
	SOCKET_STATE_LISTENING, //开始监听
	SOCKET_STATE_CONNECTING, //正在连接
	SOCKET_STATE_CONNECTED, //连接完成
	SOCKET_STATE_CLOSING //连接关闭
};

class CBaseSocket : public CRefObject
{
public:
	CBaseSocket();
	
	virtual ~CBaseSocket();

	/* 获取socket对象 */
	SOCKET GetSocket() { return m_socket; }
	/* 设置socket对象 */
	void SetSocket(SOCKET fd) { m_socket = fd; }
	/* 设置当前用户状态 */
	void SetState(uint8_t state) { m_state = state; }

	/* 设置回调函数 */
	void SetCallback(callback_t callback) { m_callback = callback; }
	/* 设置回调数据信息 */
	void SetCallbackData(void* data) { m_callback_data = data; }

	/* 设置远端ip */
	void SetRemoteIP(char* ip) { m_remote_ip = ip; }
	/* 设置远端端口 */
	void SetRemotePort(uint16_t port) { m_remote_port = port; }

	/* 设置发送的数据大小 */
	void SetSendBufSize(uint32_t send_size);
	/* 设置接收的数据大小 */
	void SetRecvBufSize(uint32_t recv_size);

	const char*	GetRemoteIP() { return m_remote_ip.c_str(); }
	uint16_t	GetRemotePort() { return m_remote_port; }
	const char*	GetLocalIP() { return m_local_ip.c_str(); }
	uint16_t	GetLocalPort() { return m_local_port; }
public:
	/* 服务器监听,当服务器对象 */
	int Listen(const char* server_ip, uint16_t port,callback_t callback,void* callback_data);

	/* 客户端连接,当客户端对象 */
	net_handle_t Connect(const char*server_ip,uint16_t port,callback_t callback,void*callback_data);

	/* 发送数据信息 */
	int Send(void* buf, int len);

	/* 接收数据信息 */
	int Recv(void* buf, int len);

	/* 断开连接 */
	int Close();

public:	
	/* 读数据 */
	void OnRead();
	/* 写数据 */
	void OnWrite();
	/* 关闭连接 */
	void OnClose();

private:	
	/* 获取错误码 */
	int _GetErrorCode();
	/* 判断是否阻塞 */
	bool _IsBlock(int error_code);

	/* 设置非阻塞状态 */
	void _SetNonblock(SOCKET fd);
	/* 设置重新使用地址 */
	void _SetReuseAddr(SOCKET fd);
	/* 设置不延迟,有数据就发 */
	void _SetNoDelay(SOCKET fd);
	/* 设置对应地址 */
	void _SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr);

	/* 连接一个新的socket对象 */
	void _AcceptNewSocket();

private:
	//远端ip和端口
	string			m_remote_ip;
	uint16_t		m_remote_port;

	//本地ip和端口
	string			m_local_ip;
	uint16_t		m_local_port;

	//回调函数和数据
	callback_t		m_callback;
	void*			m_callback_data;

	//当前状态
	uint8_t			m_state;
	//对应socket对象
	SOCKET			m_socket;
};

CBaseSocket* FindBaseSocket(net_handle_t fd);

#endif
