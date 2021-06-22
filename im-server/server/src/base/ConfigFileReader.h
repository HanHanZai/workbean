/*
 * ConfigFileReader.h
 *
 *  Created on: 2013-7-2
 *      Author: ziteng@mogujie.com
 */

#ifndef CONFIGFILEREADER_H_
#define CONFIGFILEREADER_H_

#include "util.h"

/* 读取配置文件 */
class CConfigFileReader
{
public:
	CConfigFileReader(const char* filename);
	~CConfigFileReader();

    char* GetConfigName(const char* name); /* 获取对应的key值 */
    int SetConfigValue(const char* name, const char*  value); /* 设置key-value值 */
private:
    void _LoadFile(const char* filename); /* 加载配置文件 */
    int _WriteFIle(const char*filename = NULL); /* 写配置文件 */
    void _ParseLine(char* line); /* 格式化配置文件 */
    char* _TrimSpace(char* name); /* 去除无效的空格 */

    bool m_load_ok;
    map<string, string> m_config_map;
    string m_config_file;
};



#endif /* CONFIGFILEREADER_H_ */
