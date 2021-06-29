#ifndef __NETLIB_H__
#define __NETLIB_H__

#include "ostype.h"

#define NETLIB_OPT_SET_CALLBACK			1	
#define NETLIB_OPT_SET_CALLBACK_DATA	2
#define NETLIB_OPT_GET_REMOTE_IP		3
#define NETLIB_OPT_GET_REMOTE_PORT		4
#define NETLIB_OPT_GET_LOCAL_IP			5
#define NETLIB_OPT_GET_LOCAL_PORT		6
#define NETLIB_OPT_SET_SEND_BUF_SIZE	7
#define NETLIB_OPT_SET_RECV_BUF_SIZE	8

#define NETLIB_MAX_SOCKET_BUF_SIZE		(128 * 1024)

#ifdef __cplusplus
extern "C" {
#endif

/* 初始化网络 */
int netlib_init();

/* 销毁网络 */
int netlib_destroy();

/* 开始监听 */
int netlib_listen(const char* server_ip, uint16_t port,callback_t callback,void* callback_data);

/* 网络连接 */
net_handle_t netlib_connect(const char*	server_ip,uint16_t port,callback_t callback,void* callback_data);

/* 消息发送 */
int netlib_send(net_handle_t handle, void* buf, int len);

/* 消息接收 */
int netlib_recv(net_handle_t handle, void* buf, int len);

/* 关闭连接 */
int netlib_close(net_handle_t handle);

/* 选项设置 */
int netlib_option(net_handle_t handle, int opt, void* optval);

/* 注册定时器事件 */
int netlib_register_timer(callback_t callback, void* user_data, uint64_t interval);

/* 删除定时器事件 */
int netlib_delete_timer(callback_t callback, void* user_data);

/* 添加循环事件 */
int netlib_add_loop(callback_t callback, void* user_data);

/* 开启事件循环 */
void netlib_eventloop(uint32_t wait_timeout = 100);

/* 关闭事件 */
void netlib_stop_event();

/* 判断是否存活 */
bool netlib_is_running();

#ifdef __cplusplus
}
#endif

#endif
