/*================================================================
 *   Copyright (C) 2014 All rights reserved.
 *
 *   文件名称：GroupModel.h
 *   创 建 者：Zhang Yuanhao
 *   邮    箱：bluefoxah@gmail.com
 *   创建日期：2014年12月15日
 *   描    述：
 *
 ================================================================*/

#ifndef __IM_GROUP_MODEL__
#define __IM_GROUP_MODEL__

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <set>

#include "MessageModel.h"
#include "IM.BaseDefine.pb.h"

using namespace std;

const uint32_t MAX_UNREAD_COUNT = 100;


class CGroupModel {
public:
    virtual ~CGroupModel();
    static CGroupModel* getInstance();
    
public:
    /* 创建一个Group对象 */
    uint32_t createGroup(uint32_t nUserId, const string& strGroupName, const string& strGroupAvatar, uint32_t nGroupType, set<uint32_t>& setMember);
    /* 删除一个Group对象 */
    bool removeGroup(uint32_t nUserId, uint32_t nGroupId, list<uint32_t>& lsCurUserId);
    /* 获取指定用户的群组 */
    void getUserGroup(uint32_t nUserId, list<IM::BaseDefine::GroupVersionInfo>& lsGroup, uint32_t nGroupType);
    /* 获取用户群组id，有限制，上限100 */
    void getUserGroupIds(uint32_t nUserId, list<uint32_t>& lsGroupId, uint32_t nLimited = 100);
    /* 获取群信息 */
    void getGroupInfo(map<uint32_t,IM::BaseDefine::GroupVersionInfo>& mapGroupId, list<IM::BaseDefine::GroupInfo>& lsGroupInfo);
    bool setPush(uint32_t nUserId, uint32_t nGroupId, uint32_t nType, uint32_t nStatus);
    void getPush(uint32_t nGroupId, list<uint32_t>& lsUser, list<IM::BaseDefine::ShieldStatus>& lsPush);
    /* 修改团队成员 */
    bool modifyGroupMember(uint32_t nUserId, uint32_t nGroupId, IM::BaseDefine::GroupModifyType nType, set<uint32_t>& setUserId,
                           list<uint32_t>& lsCurUserId);
    /* 获取指定群组的所有用户 */
    void getGroupUser(uint32_t nGroupId, list<uint32_t>& lsUserId);
    /* 判断用户是否在群组里，先查缓存 */
    bool isInGroup(uint32_t nUserId, uint32_t nGroupId);
    /* 查不到再查数据库 */
    bool isInGroup2(uint32_t nUserId, uint32_t nGroupId);

    /* 更新群组中的聊天信息 */
    void updateGroupChat(uint32_t nGroupId);
    /* 群组信息是否有效，查缓存 */
    bool isValidateGroupId(uint32_t nGroupId);
    /* 群组信息是否有效，查数据库 */ 
    bool isValidateGroupId2(uint32_t nGroupId);
    /* 获取用户加入时间 */
    uint32_t getUserJoinTime(uint32_t nGroupId, uint32_t nUserId);

private:
    CGroupModel();
    
    /* 插入一个新的群组 */
    bool insertNewGroup(uint32_t reqUserId, const string& groupName, const string& groupAvatar, uint32_t groupType, uint32_t memCnt, uint32_t& groupId);
    /* 插入一个新成员 */
    bool insertNewMember(uint32_t nGroupId,set<uint32_t>& setUsers);
    /* 更新群头像 */
    string GenerateGroupAvatar(uint32_t groupId);
    /* 获取群版本号，这个要从数据库中查 */
    void getGroupVersion(list<uint32_t>&lsGroupId, list<IM::BaseDefine::GroupVersionInfo>& lsGroup, uint32_t nGroupType);
    /* 查询权限 */
    bool hasModifyPermission(uint32_t nUserId, uint32_t nGroupId, IM::BaseDefine::GroupModifyType nType);
    /* 添加成员 */
    bool addMember(uint32_t nGroupId, set<uint32_t>& setUser,list<uint32_t>& lsCurUserId);
    /* 移除成员 */
    bool removeMember(uint32_t nGroupId, set<uint32_t>& setUser,list<uint32_t>& lsCurUserId);
    /* 移除重复用户 */
    void removeRepeatUser(uint32_t nGroupId, set<uint32_t>& setUser);
    /* 移除Session对象 */
    void removeSession(uint32_t nGroupId, const set<uint32_t>& lsUser);
    /* 新增群组版本 */
    bool incGroupVersion(uint32_t nGroupId);
    /* 清理群组成员 */
    void clearGroupMember(uint32_t nGroupId);
    /* 填充群成员 */
    void fillGroupMember(list<IM::BaseDefine::GroupInfo>& lsGroups);
        
private:
    static CGroupModel*	m_pInstance;
};

#endif /* defined(__IM_GROUP_MODEL__) */
