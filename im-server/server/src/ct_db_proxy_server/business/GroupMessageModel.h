/*================================================================
 *   Copyright (C) 2014 All rights reserved.
 *
 *   文件名称：GroupMessageModel.h
 *   创 建 者：Zhang Yuanhao
 *   邮    箱：bluefoxah@gmail.com
 *   创建日期：2014年12月15日
 *   描    述：
 *
 ================================================================*/

#ifndef GROUP_MESSAGE_MODEL_H_
#define GROUP_MESSAGE_MODEL_H_

#include <list>
#include <string>

#include "util.h"
#include "ImPduBase.h"
#include "AudioModel.h"
#include "GroupModel.h"
#include "IM.BaseDefine.pb.h"

using namespace std;


class CGroupMessageModel {
public:
	virtual ~CGroupMessageModel();
	static CGroupMessageModel* getInstance();

    /* 更新数据库信息 */ 
    bool sendMessage(uint32_t nFromId, uint32_t nGroupId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime, uint32_t nMsgId, const string& strMsgContent);
    bool sendAudioMessage(uint32_t nFromId, uint32_t nGroupId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime, uint32_t nMsgId,const char* pMsgContent, uint32_t nMsgLen);
    /* 获取群信息 */
    void getMessage(uint32_t nUserId, uint32_t nGroupId, uint32_t nMsgId, uint32_t nMsgCnt,
                    list<IM::BaseDefine::MsgInfo>& lsMsg);
    /* 清理缓存信息 */
    bool clearMessageCount(uint32_t nUserId, uint32_t nGroupId);
    /* 获取群组自增id */
    uint32_t getMsgId(uint32_t nGroupId);
    /* 获取指定用户未读的消息数量 */
    void getUnreadMsgCount(uint32_t nUserId, uint32_t &nTotalCnt, list<IM::BaseDefine::UnreadInfo>& lsUnreadCount);
    /* 获取最后的消息内容 */
    void getLastMsg(uint32_t nGroupId, uint32_t& nMsgId, string& strMsgData, IM::BaseDefine::MsgType & nMsgType, uint32_t& nFromId);
    /* 获取用户所有未读的消息 */
    void getUnReadCntAll(uint32_t nUserId, uint32_t &nTotalCnt);
    /* 根据用户id，群组id，消息id来获取消息内容 */
    void getMsgByMsgId(uint32_t nUserId, uint32_t nGroupId, const list<uint32_t>& lsMsgId, list<IM::BaseDefine::MsgInfo>& lsMsg);
    /* 重置消息id，从0开始 */
    bool resetMsgId(uint32_t nGroupId);
private:
    CGroupMessageModel();
    /* 增加指定群组指定用户的消息数量 */
    bool incMessageCount(uint32_t nUserId, uint32_t nGroupId);

private:
	static CGroupMessageModel*	m_pInstance;
};



#endif /* MESSAGE_MODEL_H_ */
