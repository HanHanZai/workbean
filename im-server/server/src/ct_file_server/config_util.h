//
//  config_util.h
//  im-server-mac-new
//
//  Created by wubenqi on 15/7/16.
//  Copyright (c) 2015年 benqi. All rights reserved.
//

#ifndef FILE_SERVER_CONFIG_UTIL_H_
#define FILE_SERVER_CONFIG_UTIL_H_

#include <list>

#include "base/singleton.h"
#include "base/pb/protocol/IM.BaseDefine.pb.h"

/* 配置相关插件，包括服务器集合和定时器相关内容 */
class ConfigUtil : public Singleton<ConfigUtil> {
public:
    ~ConfigUtil() { }
    
    /* 添加地址 */
    void AddAddress(const char* ip, uint16_t port);
    const std::list<IM::BaseDefine::IpAddr>& GetAddressList() const {
        return addrs_;
    }
    
    /* 设置任务超时 */
    void SetTaskTimeout(uint32_t timeout) { task_timeout_ = timeout; }
    
    /* 获取任务时间 */
    uint32_t GetTaskTimeout() const { return task_timeout_; }
    
private:
    friend class Singleton<ConfigUtil>;
    
    ConfigUtil()
        : task_timeout_(3600) { }
    
    std::list<IM::BaseDefine::IpAddr> addrs_;
    uint32_t task_timeout_;
};

#endif /* defined(FILE_SERVER_CONFIG_UTIL_H_) */
