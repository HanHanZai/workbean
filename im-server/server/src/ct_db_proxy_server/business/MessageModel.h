/*================================================================
 *   Copyright (C) 2014 All rights reserved.
 *
 *   文件名称：MessageModel.h
 *   创 建 者：Zhang Yuanhao
 *   邮    箱：bluefoxah@gmail.com
 *   创建日期：2014年12月15日
 *   描    述：
 *
 ================================================================*/

#ifndef MESSAGE_MODEL_H_
#define MESSAGE_MODEL_H_

#include <list>
#include <string>

#include "util.h"
#include "ImPduBase.h"
#include "AudioModel.h"
#include "IM.BaseDefine.pb.h"
#include "IM.Message.pb.h"

using namespace std;

class CMessageModel {
public:
	virtual ~CMessageModel();
	static CMessageModel* getInstance();

    /* A to B发送数据 */
    bool sendMessage(uint32_t nRelateId, uint32_t nFromId, uint32_t nToId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime,
                     uint32_t nMsgId, string& strMsgContent);
    /* A to B发送语音 */
    bool sendAudioMessage(uint32_t nRelateId, uint32_t nFromId, uint32_t nToId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime,
                          uint32_t nMsgId, const char* pMsgContent, uint32_t nMsgLen);
    /* 获取指定用户的消息 */
    void getMessage(uint32_t nUserId, uint32_t nPeerId, uint32_t nMsgId, uint32_t nMsgCnt,
                    list<IM::BaseDefine::MsgInfo>& lsMsg);
    /* 去除未读消息  */
    bool clearMessageCount(uint32_t nUserId, uint32_t nPeerId);
    /* 获取消息id */
    uint32_t getMsgId(uint32_t nRelateId);
    /* 获取未读取的消息数量 */
    void getUnreadMsgCount(uint32_t nUserId, uint32_t &nTotalCnt, list<IM::BaseDefine::UnreadInfo>& lsUnreadCount);
    /* 获取最后一条消息 */
    void getLastMsg(uint32_t nFromId, uint32_t nToId, uint32_t& nMsgId, string& strMsgData, IM::BaseDefine::MsgType & nMsgType, uint32_t nStatus = 0);
    /* 获取所有未读的 */
    void getUnReadCntAll(uint32_t nUserId, uint32_t &nTotalCnt);
    void getMsgByMsgId(uint32_t nUserId, uint32_t nPeerId, const list<uint32_t>& lsMsgId, list<IM::BaseDefine::MsgInfo>& lsMsg);
    bool resetMsgId(uint32_t nRelateId);
	bool setreadSystemMsg(uint32_t nFromId, uint32_t nToId, uint32_t nMsgId);
	void getNoReadSystemMessage(uint32_t nUserId, uint32_t nPeerId ,list<IM::Message::IMMsgData>& lsMsg);
private:
	CMessageModel();
    void incMsgCount(uint32_t nFromId, uint32_t nToId);
private:
	static CMessageModel*	m_pInstance;
};



#endif /* MESSAGE_MODEL_H_ */
