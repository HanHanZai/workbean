/*
 * DBServConn.h
 *
 *  Created on: 2013-7-8
 *      Author: ziteng@mogujie.com
 */

#ifndef DBSERVCONN_H_
#define DBSERVCONN_H_

#include "imconn.h"
#include "ServInfo.h"
#include "RouteServConn.h"

//相当于个客户端
class CDBServConn : public CImConn
{
public:
	CDBServConn();
	virtual ~CDBServConn();

	bool IsOpen() { return m_bOpen; }

	void Connect(const char* server_ip, uint16_t server_port, uint32_t serv_idx);
	virtual void Close();

	virtual void OnConfirm();
	virtual void OnClose();
	virtual void OnTimer(uint64_t curr_tick);

	/* 处理PDU数据 */
	virtual void HandlePdu(CImPdu* pPdu);
private:
	/* 处理验证回应 */
	void _HandleValidateResponse(CImPdu* pPdu);
	/* 获取过去的会话回应 */
    void _HandleRecentSessionResponse(CImPdu* pPdu);
	/* 所有用户信息的回应 */
    void _HandleAllUserResponse(CImPdu* pPdu);
	/* 获取消息列表回应 */
    void _HandleGetMsgListResponse(CImPdu* pPdu);
	/* 根据消息id获取消息回应 */
    void _HandleGetMsgByIdResponse(CImPdu* pPdu);
	/* 处理消息数据 */
    void _HandleMsgData(CImPdu* pPdu);
	/* 处理未读消息回应 */
	void _HandleUnreadMsgCountResponse(CImPdu* pPdu);
	/* 获取最近的消息id回应 */
    void _HandleGetLatestMsgIDRsp(CImPdu* pPdu);
	/* 处理DB写入消息回应 */
	void _HandleDBWriteResponse(CImPdu* pPdu);
	/* 处理用户信息回应 */
	void _HandleUsersInfoResponse(CImPdu* pPdu);
	/* 停止接收包内容 */
	void _HandleStopReceivePacket(CImPdu* pPdu);
	/* 移除消息的回应 */
	void _HandleRemoveSessionResponse(CImPdu* pPdu);
	/* 处理改变头像的回应 */
	void _HandleChangeAvatarResponse(CImPdu* pPdu);
	/* 修改登录信息的回应 */
    void _HandleChangeSignInfoResponse(CImPdu* pPdu);
	/* 处理设置设备token信息的回应 */
    void _HandleSetDeviceTokenResponse(CImPdu* pPdu);
	/* 处理获取token信息的回应 */
    void _HandleGetDeviceTokenResponse(CImPdu* pPdu);
	/* 处理部门的相关回应 */
    void _HandleDepartmentResponse(CImPdu* pPdu);
	/* 处理消息屏蔽回应 */
    void _HandlePushShieldResponse(CImPdu* pPdu);
	/* 查询消息屏蔽回应 */
    void _HandleQueryPushShieldResponse(CImPdu* pPdu);
private:
	bool 		m_bOpen;
	uint32_t	m_serv_idx;
};

/* 初始化数据库服务连接 */
void init_db_serv_conn(serv_info_t* server_list, uint32_t server_count, uint32_t concur_conn_cnt);
/* 初始化消息服务连接数量 */
void init_msg_serv_count(const char* msg_server_ip_addr1,const char* msg_server_ip_addr2, uint16_t msg_server_port, uint32_t max_conn_cnt);

/* 获取登录的数据库连接实例对象 */
CDBServConn* get_db_serv_conn_for_login();
/* 获取数据库连接实例对象 */
CDBServConn* get_db_serv_conn();

#endif /* DBSERVCONN_H_ */