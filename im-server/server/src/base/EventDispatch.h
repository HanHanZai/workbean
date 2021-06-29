/*
 * A socket event dispatcher, features include: 
 * 1. portable: worked both on Windows, MAC OS X,  LINUX platform
 * 2. a singleton pattern: only one instance of this class can exist
 */
#ifndef __EVENT_DISPATCH_H__
#define __EVENT_DISPATCH_H__

#include "ostype.h"
#include "util.h"

#include "Lock.h"

enum {
	SOCKET_READ		= 0x1,
	SOCKET_WRITE	= 0x2,
	SOCKET_EXCEP	= 0x4,
	SOCKET_ALL		= 0x7
};

class CEventDispatch
{
public:
	virtual ~CEventDispatch();

	/* 添加一个事件 */
	void AddEvent(SOCKET fd, uint8_t socket_event);

	/* 移除对应事件 */
	void RemoveEvent(SOCKET fd, uint8_t socket_event);

	/* 添加定时器事件 */
	void AddTimer(callback_t callback, void* user_data, uint64_t interval);
	/* 移除定时器事件 */
	void RemoveTimer(callback_t callback, void* user_data);
    
	/* 添加循环事件 */
    void AddLoop(callback_t callback, void* user_data);

	/* 开始事件分发 */
	void StartDispatch(uint32_t wait_timeout = 100);
	/* 中止事件分发 */
    void StopDispatch();
    
	/* 标记当前运行状态 */
    bool isRunning() {return running;}

	static CEventDispatch* Instance();
protected:
	CEventDispatch();

private:
	void _CheckTimer();
    void _CheckLoop();

	typedef struct {
		callback_t	callback;
		void*		user_data;
		uint64_t	interval;
		uint64_t	next_tick;
	} TimerItem;

private:
#ifdef _WIN32
	/* 读取的select事件 */
	fd_set	m_read_set;
	/* 写入的select事件 */
	fd_set	m_write_set;
	/* 异常的select事件 */
	fd_set	m_excep_set;
#elif __APPLE__
	int 	m_kqfd;
#else
	int		m_epfd;
#endif
	/* 对象锁 */
	CLock			m_lock;
	/* 定时器事件列表 */
	list<TimerItem*>	m_timer_list;
	/* 循环事件列表 */
	list<TimerItem*>	m_loop_list;

	static CEventDispatch* m_pEventDispatch;
    
	/* 运行状态位 */
    bool running;
};

#endif
