/*================================================================
*     Copyright (c) 2014年 lanhu. All rights reserved.
*   
*   文件名称：CacheManager.h
*   创 建 者：Zhang Yuanhao
*   邮    箱：bluefoxah@gmail.com
*   创建日期：2014年12月02日
*   描    述：
*
#pragma once
================================================================*/
#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include <list>
#include <map>

#include "ostype.h"
#include "Lock.h"
#include "Condition.h"
#include "ImPduBase.h"
#include "public_define.h"
#include "IM.BaseDefine.pb.h"

/* 同步中心 */
class CSyncCenter
{
public:
    static CSyncCenter* getInstance();
    
    /* 获取最后的更新的用户信息 */
    uint32_t getLastUpdate() {
               CAutoLock auto_lock(&last_update_lock_);
                return m_nLastUpdate;
           }
    /* 获取最后更新的群组信息 */
    uint32_t getLastUpdateGroup() {
                CAutoLock auto_lock(&last_update_lock_);
                return m_nLastUpdateGroup;
           }
    
    /* 获取部门信息 */
    string getDeptName(uint32_t nDeptId);
    /* 开始同步 */
    void startSync();
    /* 停止同步 */
    void stopSync();
    /* 初始化 */
    void init();
    /* 更新全部内容 */
    void updateTotalUpdate(uint32_t nUpdated);

private:
    /* 更新最后一次更新的团队信息 */
    void updateLastUpdateGroup(uint32_t nUpdated);
    
    CSyncCenter();
    ~CSyncCenter();
    /* 同步团队聊天信息 */
    static void* doSyncGroupChat(void* arg);
    
private:
    /* 获取部门信息 */
    void getDept(uint32_t nDeptId, DBDeptInfo_t** pDept);
    DBDeptMap_t* m_pDeptInfo;

    static CSyncCenter* m_pInstance;
    uint32_t    m_nLastUpdateGroup;
    uint32_t    m_nLastUpdate;

    CCondition* m_pCondGroupChat;
    CLock*      m_pLockGroupChat; /* 资源锁 */
    static bool m_bSyncGroupChatRuning; /* 全局静态变量,标识同步聊天 */
    bool m_bSyncGroupChatWaitting; /* 等待? */ 
#ifdef _WIN32
    DWORD		m_nGroupChatThreadId; /* 聊天线程 */
#else
    pthread_t	m_nGroupChatThreadId;
#endif
    CLock last_update_lock_; /* 最后一次更新锁 */


};


#endif /*defined(__CACHEMANAGER_H__) */
