/*
 * DBPool.h
 *
 *  Created on: 2014年7月22日
 *      Author: ziteng
 *  Modify By ZhangYuanhao
 *  2015-01-12
 *  enable config the max connection of every instance
 */

#ifndef DBPOOL_H_
#define DBPOOL_H_

#include "../base/util.h"
#include "ThreadPool.h"
#include <mysql.h>

#define MAX_ESCAPE_STRING_LEN	10240

class CResultSet {
public:
	CResultSet(MYSQL_RES* res);
	virtual ~CResultSet();

	bool Next();
	int GetInt(const char* key); /* 获取对应key值对应的int value值 */
	char* GetString(const char* key); /* 获取对应key值对应的string value值 */
private:
	int _GetIndex(const char* key);

	MYSQL_RES* 			m_res;
	MYSQL_ROW			m_row;
	map<string, int>	m_key_map; /* key和对应的index */
};

/*
 * TODO:
 * 用MySQL的prepare statement接口来防止SQL注入
 * 暂时只用于插入IMMessage表，因为只有那里有SQL注入的风险，
 * 以后可以把全部接口用这个来实现替换
 */
/* 纯参数 */
class CPrepareStatement {
public:
	CPrepareStatement();
	virtual ~CPrepareStatement();

	bool Init(MYSQL* mysql, string& sql);

	void SetParam(uint32_t index, int& value);
	void SetParam(uint32_t index, uint32_t& value);
    void SetParam(uint32_t index, string& value);
    void SetParam(uint32_t index, const string& value);

	bool ExecuteUpdate();
	uint32_t GetInsertId();
private:
	MYSQL_STMT*	m_stmt; /* 句柄对象 */
	MYSQL_BIND*	m_param_bind; /* 参数绑定 */
	uint32_t	m_param_cnt; /* 参数数量 */
};

class CDBPool;

class CDBConn {
public:
	CDBConn(CDBPool* pDBPool);
	virtual ~CDBConn();
	int Init();

	CResultSet* ExecuteQuery(const char* sql_query); /* 查询 */
	bool ExecuteUpdate(const char* sql_query); /* 更新 */
	char* EscapeString(const char* content, uint32_t content_len); /* 返回有效的SQL语句对象 */

	uint32_t GetInsertId(); /* 获取自增id */

	const char* GetPoolName(); /* 获取数据库名称 */
	MYSQL* GetMysql() { return m_mysql; } /* 获取数据库对象 */
private:
	CDBPool* 	m_pDBPool;	// to get MySQL server information
	MYSQL* 		m_mysql;
	//MYSQL_RES*	m_res;
	char		m_escape_string[MAX_ESCAPE_STRING_LEN + 1];
};

/* 单个数据库池对象 */
class CDBPool {
public:
	CDBPool(const char* pool_name, const char* db_server_ip, uint16_t db_server_port,
			const char* username, const char* password, const char* db_name, int max_conn_cnt);
	virtual ~CDBPool();

	int Init();
	CDBConn* GetDBConn(); /* 获取对象的数据库连接对象 */
	void RelDBConn(CDBConn* pConn); 

	const char* GetPoolName() { return m_pool_name.c_str(); }
	const char* GetDBServerIP() { return m_db_server_ip.c_str(); }
	uint16_t GetDBServerPort() { return m_db_server_port; }
	const char* GetUsername() { return m_username.c_str(); }
	const char* GetPasswrod() { return m_password.c_str(); }
	const char* GetDBName() { return m_db_name.c_str(); }
private:
	string 		m_pool_name; /* 数据库名称 */
	string 		m_db_server_ip; /* 数据库ip */
	uint16_t	m_db_server_port; /* 数据库端口 */
	string 		m_username; /* 用户名称 */
	string 		m_password; /* 相关密码 */
	string 		m_db_name; /* 数据库名称newim */
	int			m_db_cur_conn_cnt; /* 当前连接的数据库数量 */
	int 		m_db_max_conn_cnt; /* 最大连接的数据库数量 */
	list<CDBConn*>	m_free_list; /* 数据库连接对象 */
	CThreadNotify	m_free_notify; /* 线程同步对象 */
};

// manage db pool (master for write and slave for read)
class CDBManager {
public:
	virtual ~CDBManager();

	static CDBManager* getInstance();

	int Init();

	CDBConn* GetDBConn(const char* dbpool_name);
	void RelDBConn(CDBConn* pConn);
private:
	CDBManager();

private:
	static CDBManager*		s_db_manager;
	map<string, CDBPool*>	m_dbpool_map;
};

#endif /* DBPOOL_H_ */
