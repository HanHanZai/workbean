//
//  file_client_conn.h
//  im-server-mac-new
//
//  Created by wubenqi on 15/7/16.
//  Copyright (c) 2015年 benqi. All rights reserved.
//

#ifndef FILE_SERVER_FILE_CLIENT_CONN_H_
#define FILE_SERVER_FILE_CLIENT_CONN_H_

#include "base/imconn.h"
#include "ct_file_server/transfer_task.h"

// TODO
// 异常情况处理
// 断线、服务器重启等
class FileClientConn : public CImConn {
public:
    FileClientConn()
        : auth_(false),
          user_id_(0),
          transfer_task_(NULL) {
    }
    
    virtual ~FileClientConn() { }
    
    /* 关闭 */
    virtual void Close();
    
    // void Close2();
    /* 连接操作 */
    virtual void OnConnect(net_handle_t handle);
    /* 关闭操作 */
    virtual void OnClose();
    /* 定时器操作 */
    virtual void OnTimer(uint64_t curr_tick);
    
    /* 写操作 */
    virtual void OnWrite();

    /* 处理回调 */
    virtual void HandlePdu(CImPdu* pdu);
    
    /* 清理传输任务 */
    void ClearTransferTask() {
        user_id_ = 0;
        transfer_task_ = NULL;
    }
    
private:
    /* 处理心跳函数 */
    void _HandleHeartBeat(CImPdu* pdu);
    
    /* 处理客户端文件登录请求 */
    void _HandleClientFileLoginReq(CImPdu* pdu);

    /* 处理客户端文件状态 */
    void _HandleClientFileStates(CImPdu* pdu);

    /* 处理客户端文件上传请求 */
    void _HandleClientFilePullFileReq(CImPdu* pdu);

    /* 处理客户端文件上传回应 */
    void _HandleClientFilePullFileRsp(CImPdu *pdu);
    
    /* 状态通知 */
    int _StatesNotify(int state, const std::string& task_id, uint32_t user_id, CImConn* conn);
    
    // bool _IsAuth() const { return auth_; }
    
    /// yunfan add 2014.8.18
private:
    /* 预上传 */
    int _PreUpload(const char* task_id);
    //  int _DoUpload(const char* task_id);
    /// yunan add end
    
    bool		auth_;
    
    uint32_t	user_id_;
    // 当前设计每个连接对应一次任务，故可预先缓存
    BaseTransferTask* transfer_task_;
};

/* 初始化文件客户端连接 */
void InitializeFileClientConn();
/* 文件客户端连接回调 */
void FileClientConnCallback(void* callback_data, uint8_t msg, uint32_t handle, void* param);


#endif /* defined(FILE_SERVER_FILE_CLIENT_CONN_H_) */
