#include "netlib.h"
#include "BaseSocket.h"
#include "EventDispatch.h"

/* 初始化socket库 */
int netlib_init()
{
	int ret = NETLIB_OK;
#ifdef _WIN32
	WSADATA wsaData;
	WORD wReqest = MAKEWORD(1, 1);
	if (WSAStartup(wReqest, &wsaData) != 0)
	{
		ret = NETLIB_ERROR;
	}
#endif

	return ret;
}

int netlib_destroy()
{
	int ret = NETLIB_OK;
#ifdef _WIN32
	if (WSACleanup() != 0)
	{
		ret = NETLIB_ERROR;
	}
#endif

	return ret;
}

int netlib_listen(const char* server_ip, uint16_t port,callback_t callback,void* callback_data)
{
	/* 不需要释放，因为listen方法已经把自己交给了全局静态变量来进行管理 */
	CBaseSocket* pSocket = new CBaseSocket();
	if (!pSocket)
		return NETLIB_ERROR;

	int ret =  pSocket->Listen(server_ip, port, callback, callback_data);
	if (ret == NETLIB_ERROR)
		delete pSocket;
	return ret;
}

net_handle_t netlib_connect(const char* server_ip, uint16_t	port,callback_t callback,void* callback_data)
{
	CBaseSocket* pSocket = new CBaseSocket();
	if (!pSocket)
		return NETLIB_INVALID_HANDLE;

	net_handle_t handle = pSocket->Connect(server_ip, port, callback, callback_data);
	if (handle == NETLIB_INVALID_HANDLE)
		delete pSocket;
	return handle;
}

int netlib_send(net_handle_t handle, void* buf, int len)
{
	/* FindBaseSocket内部添加了对应的引用 */
	CBaseSocket* pSocket = FindBaseSocket(handle);
	if (!pSocket){
		return NETLIB_ERROR;
	}

	int ret = pSocket->Send(buf, len);
	pSocket->ReleaseRef();
	return ret;
}

int netlib_recv(net_handle_t handle, void* buf, int len)
{
	CBaseSocket* pSocket = FindBaseSocket(handle);
	if (!pSocket)
		return NETLIB_ERROR;

	int ret = pSocket->Recv(buf, len);
	pSocket->ReleaseRef();
	return ret;
}

int netlib_close(net_handle_t handle)
{
	CBaseSocket* pSocket = FindBaseSocket(handle);
	if (!pSocket)
		return NETLIB_ERROR;

	/* 下面连续释放了2次引用,万一释放掉了,下面这个pSocket对象都不存在了,是不是会报错 */
	int ret = pSocket->Close();
	pSocket->ReleaseRef();
	return ret;
}

int netlib_option(net_handle_t handle, int opt, void* optval)
{
	CBaseSocket* pSocket = FindBaseSocket(handle);
	if (!pSocket)
		return NETLIB_ERROR;

	if ((opt >= NETLIB_OPT_GET_REMOTE_IP) && !optval)
		return NETLIB_ERROR;

	switch (opt)
	{
	case NETLIB_OPT_SET_CALLBACK: /* 设置回调 */
		pSocket->SetCallback((callback_t)optval);
		break;
	case NETLIB_OPT_SET_CALLBACK_DATA: /* 设置回调数据参数 */
		pSocket->SetCallbackData(optval);
		break;
	case NETLIB_OPT_GET_REMOTE_IP: /* 获取远端ip */
		*(string*)optval = pSocket->GetRemoteIP();
		break;
	case NETLIB_OPT_GET_REMOTE_PORT: /* 获取远端端口 */
		*(uint16_t*)optval = pSocket->GetRemotePort();
		break;
	case NETLIB_OPT_GET_LOCAL_IP: /* 获取本地ip */
		*(string*)optval = pSocket->GetLocalIP();
		break;
	case NETLIB_OPT_GET_LOCAL_PORT: /* 获取本地端口 */
		*(uint16_t*)optval = pSocket->GetLocalPort();
		break;
	case NETLIB_OPT_SET_SEND_BUF_SIZE: /* 设置发送的容器数据大小 */
		pSocket->SetSendBufSize(*(uint32_t*)optval);
		break;
	case NETLIB_OPT_SET_RECV_BUF_SIZE:/* 设置接收的容器大小 */
		pSocket->SetRecvBufSize(*(uint32_t*)optval);
		break;
	}

	pSocket->ReleaseRef();
	return NETLIB_OK;
}

int netlib_register_timer(callback_t callback, void* user_data, uint64_t interval)
{
	CEventDispatch::Instance()->AddTimer(callback, user_data, interval);
	return 0;
}

int netlib_delete_timer(callback_t callback, void* user_data)
{
	CEventDispatch::Instance()->RemoveTimer(callback, user_data);
	return 0;
}

int netlib_add_loop(callback_t callback, void* user_data)
{
	CEventDispatch::Instance()->AddLoop(callback, user_data);
	return 0;
}

void netlib_eventloop(uint32_t wait_timeout)
{
	CEventDispatch::Instance()->StartDispatch(wait_timeout);
}

void netlib_stop_event()
{
    CEventDispatch::Instance()->StopDispatch();
}

bool netlib_is_running()
{
    return CEventDispatch::Instance()->isRunning();
}
