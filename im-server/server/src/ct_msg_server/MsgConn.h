/*
 * MsgConn.h
 *
 *  Created on: 2013-7-5
 *      Author: ziteng@mogujie.com
 */

#ifndef MSGCONN_H_
#define MSGCONN_H_

#include "imconn.h"


#define KICK_FROM_ROUTE_SERVER 		1
#define MAX_ONLINE_FRIEND_CNT		100	//通知好友状态通知的最多个数

typedef struct {
	uint32_t msg_id;
	uint32_t from_id;
	uint64_t timestamp;
} msg_ack_t;

class CImUser;

class CMsgConn : public CImConn
{
public:
	CMsgConn();
	virtual ~CMsgConn();

    string GetLoginName() { return m_login_name; }
    uint32_t GetUserId() { return m_user_id; }
    void SetUserId(uint32_t user_id) { m_user_id = user_id; }
    uint32_t GetHandle() { return m_handle; }
    uint16_t GetPduVersion() { return m_pdu_version; }
    uint32_t GetClientType() { return m_client_type; }
    /* 区分移动端和PC端 */
    uint32_t GetClientTypeFlag();
    void SetOpen() { m_bOpen = true; }
    bool IsOpen() { return m_bOpen; }
    void SetKickOff() { m_bKickOff = true; }
    bool IsKickOff() { return m_bKickOff; }
    void SetOnlineStatus(uint32_t status) { m_online_status = status; }
    uint32_t GetOnlineStatus() { return m_online_status; }
    
    /* 同步数据库服务器，修改用户状态 */
    void SendUserStatusUpdate(uint32_t user_status);

    /* 关闭消息连接，kick_user为true的话，强制用户下线 */
	virtual void Close(bool kick_user = false);

    /* 连接 */
	virtual void OnConnect(net_handle_t handle);
    /* 关闭连接 */
	virtual void OnClose();

    /* 定时器操作 */
	virtual inline void OnTimer(uint64_t curr_tick);

    /* 处理消息服务器的回应信息 */
	virtual void HandlePdu(CImPdu* pPdu);

    //添加消息到发送列表中
	void AddToSendList(uint32_t msg_id, uint32_t from_id);
    //从消息发送列表中删除
	void DelFromSendList(uint32_t msg_id, uint32_t from_id);
    //更新用户状态
    void updateUserStatus(uint32_t user_status);

private:
    //同步心跳
    void _HandleHeartBeat(CImPdu* pPdu);
    //处理登录请求
	void _HandleLoginRequest(CImPdu* pPdu);
    //处理登出请求
    void _HandleLoginOutRequest(CImPdu* pPdu);
    //获取之前的会话session请求
    void _HandleClientRecentContactSessionRequest(CImPdu* pPdu);
    /* 发送消息数据 */
	void _HandleClientMsgData(CImPdu* pPdu);
    /* 消息数据确认 */
	void _HandleClientMsgDataAck(CImPdu* pPdu);
    /* 请求消息服务器的同步时间 */
	void _HandleClientTimeRequest(CImPdu* pPdu);
    /* 请求消息列表信息 */
    void _HandleClientGetMsgListRequest(CImPdu* pPdu);
    /* 根据消息id来获取消息 */
    void _HandleClientGetMsgByMsgIdRequest(CImPdu* pPdu);
    /* 请求未读的消息数量 */
	void _HandleClientUnreadMsgCntRequest(CImPdu* pPdu);
    /* 处理消息已读取确认信息 */
	void _HandleClientMsgReadAck(CImPdu* pPdu);
    /* 请求最近的消息id信息 */
    void _HandleClientGetLatestMsgIDReq(CImPdu* pPdu);
    /* peer to peer消息处理 */
	void _HandleClientP2PCmdMsg(CImPdu* pPdu);
    /* 用户信息请求 */
	void _HandleClientUserInfoRequest(CImPdu* pPdu);
    /* 用户状态请求 */
	void _HandleClientUsersStatusRequest(CImPdu* pPdu);
    /* 移除会话请求 */
	void _HandleClientRemoveSessionRequest(CImPdu* pPdu);
    /* 请求所有用户信息 */
	void _HandleClientAllUserRequest(CImPdu* pPdu);
    /* 请求调整图标 */
    void _HandleChangeAvatarRequest(CImPdu* pPdu);
    /* 改变登录信息 */
    void _HandleChangeSignInfoRequest(CImPdu* pPdu);

    /* 处理设备token信息 */
    void _HandleClientDeviceToken(CImPdu* pPdu);
    /* 请求剔出PC客户端 */
    void _HandleKickPCClient(CImPdu* pPdu);
    /* 部门信息请求 */
    void _HandleClientDepartmentRequest(CImPdu* pPdu);
    /* 发送朋友状态通知 未使用 */
    void _SendFriendStatusNotify(uint32_t status);
    /* 屏蔽请求 */
    void _HandlePushShieldRequest(CImPdu* pPdu);
    /* 请求查询屏蔽 */
    void _HandleQueryPushShieldRequest(CImPdu* pPdu);
private:
    string          m_login_name;  //登录名称，如果遇到中文名，需要转化为拼音
    uint32_t        m_user_id;
    bool			m_bOpen;	// only DB validate passed will be set to true;
    bool            m_bKickOff; //强制剔除
    uint64_t		m_login_time; //登录时间
    
    uint32_t		m_last_seq_no; //最后请求序列号
    
    uint16_t		m_pdu_version; //pdu消息版本
    
    string 			m_client_version;	// e.g MAC/2.2, or WIN/2.2
    
    list<msg_ack_t>	m_send_msg_list; //消息发送列表
    
    uint32_t		m_msg_cnt_per_sec; //每秒发送的消息数量
    
    uint32_t        m_client_type;        //客户端登录方式
    
    uint32_t        m_online_status;      //在线状态 1-online, 2-off-line, 3-leave
};

void init_msg_conn();

#endif /* MSGCONN_H_ */
