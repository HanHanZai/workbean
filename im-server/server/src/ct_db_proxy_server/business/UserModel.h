/*================================================================
*     Copyright (c) 2015年 lanhu. All rights reserved.
*   
*   文件名称：UserModel.h
*   创 建 者：Zhang Yuanhao
*   邮    箱：bluefoxah@gmail.com
*   创建日期：2015年01月05日
*   描    述：
*
#pragma once
================================================================*/
#ifndef __USERMODEL_H__
#define __USERMODEL_H__

#include "IM.BaseDefine.pb.h"
#include "ImPduBase.h"
#include "public_define.h"
class CUserModel
{
public:
    static CUserModel* getInstance();
    ~CUserModel();
    //获取好友资料更新的用户列表
    void getChangedId(uint32_t& nLastTime,uint32_t& nReqId,list<uint32_t> &lsIds);

    //获取用户信息
    void getUsers(list<uint32_t> lsIds, list<IM::BaseDefine::UserInfo>& lsUsers);

    //获取指定用户信息
    bool getUser(uint32_t nUserId, DBUserInfo_t& cUser);

    //更新用户信息
    bool updateUser(DBUserInfo_t& cUser);

    //新增用户
    bool insertUser(DBUserInfo_t& cUser);
//    void getUserByNick(const list<string>& lsNicks, list<IM::BaseDefine::UserInfo>& lsUsers);
    //清理用户记数
    void clearUserCounter(uint32_t nUserId, uint32_t nPeerId, IM::BaseDefine::SessionType nSessionType);
    //设置呼叫日志
    void setCallReport(uint32_t nUserId, uint32_t nPeerId, IM::BaseDefine::ClientType nClientType);

    //更新用户签到信息
    bool updateUserSignInfo(uint32_t user_id, const string& sign_info);
    //获取用户签到信息
    bool getUserSingInfo(uint32_t user_id, string* sign_info);

    //更新盾牌状态？
    bool updatePushShield(uint32_t user_id, uint32_t shield_status);
    //获取令牌
    bool getPushShield(uint32_t user_id, uint32_t* shield_status);
    //更新用户在想状态
    bool updateUserOnline(uint32_t nUserId,uint32_t status);

private:
    CUserModel();
private:
    static CUserModel* m_pInstance;
};

#endif /*defined(__USERMODEL_H__) */
