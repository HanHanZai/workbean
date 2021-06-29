//
//  session_manager.h
//  my_push_server
//
//  Created by luoning on 14-11-11.
//  Copyright (c) 2014年 luoning. All rights reserved.
//

#ifndef __my_push_server__session_manager__
#define __my_push_server__session_manager__

#include <stdio.h>
#include "socket/base_io_stream.h"
#include "thread/base_thread.hpp"
#ifdef _WIN32
#include <hash_map>
#include <list>
using namespace stdext;
#else
#include <ext/hash_map>
using namespace __gnu_cxx;
#endif

#include "push_session.h"
#include "apns_client.h"
#include "push_server.h"
#include "timer/Timer.hpp"

class CSessionManager
{
public:
    CSessionManager();
    virtual ~CSessionManager();
    
    static CSessionManager* GetInstance();
    static void TimerProc(int32_t nIndex, void* param);
    
    /* 开始检查推送会话 */
    void StartCheckPushSession();
    /* 停止推送会话 */
    void StopCheckPushSession();
    /* 检查推送会话是否超时 */
    void CheckPushSessionTimeOut();
    /* 检查推送服务是否删除 */
    void CheckPushSessionDelete();
    
    /* 通过SockId来推送会话 */
    void AddPushSessionBySockID(uint32_t nsockid, push_session_ptr pSession);
    /* 根据SockId移除推送会话 */
    void RemovePushSessionBySockID(uint32_t nsockid);
    /* 根据SockId获取推送会话 */
    push_session_ptr GetPushSessionBySockID(uint32_t nsockid);
    /* 清理推送会话 */
    void ClearPushSession();
    /* 停止所有的推送会话 */
    void StopAllPushSession();
    
    /* 设置推送服务器客户端 */
    void SetAPNSClient(apns_client_ptr pClient) { m_pAPNSClient = pClient; }
    /* 获取推送服务客户端 */
    apns_client_ptr GetAPNSClient() { return m_pAPNSClient; }
    /* 移除推送服务器客户端 */
    void RemoveAPNSClient();
    
    /* 设置推送服务器 */
    void SetPushServer(push_server_ptr pServer) { m_pPushServer = pServer; }
    /* 获取推送服务器 */
    push_server_ptr GetPushServer() { return m_pPushServer; }
    /* 移除推送服务器 */
    void RemovePushServer();
    
private:
    /* 清理所有的推送服务会话 */
    void _ClearPushSessionForMap();
    
private:
    /* 所有的消息推送map对象 */
    hash_map<uint32_t /* sockid */, push_session_ptr> m_MapPushSessionBySockID;
    CBaseMutex m_MapIOPushSessionBySockIDMutex; 
    
    apns_client_ptr m_pAPNSClient;
    push_server_ptr m_pPushServer;
    
    CTimer m_checktimer;
};

#endif /* defined(__my_push_server__session_manager__) */
