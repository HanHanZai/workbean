#include "EventDispatch.h"
#include "BaseSocket.h"

#define MIN_TIMER_DURATION	100	// 100 miliseconds

CEventDispatch* CEventDispatch::m_pEventDispatch = NULL;

CEventDispatch::CEventDispatch()
{
    running = false;
#ifdef _WIN32
	FD_ZERO(&m_read_set); /* 一系列可读绑定消息 */
	FD_ZERO(&m_write_set); /* 一系列可写绑定信息 */
	FD_ZERO(&m_excep_set); /* 一系列异常信息 */
#elif __APPLE__
	m_kqfd = kqueue();
	if (m_kqfd == -1)
	{
		log("kqueue failed");
	}
#else
	m_epfd = epoll_create(1024);
	if (m_epfd == -1)
	{
		log("epoll_create failed");
	}
#endif
}

CEventDispatch::~CEventDispatch()
{
#ifdef _WIN32

#elif __APPLE__
	close(m_kqfd);
#else
	close(m_epfd);
#endif
}

/* 增加或者更新一个回调，根据回调和用户数据当作标识 */
void CEventDispatch::AddTimer(callback_t callback, void* user_data, uint64_t interval)
{
	list<TimerItem*>::iterator it;
	/* 存在相关的事件，刷新对应的时间 */
	for (it = m_timer_list.begin(); it != m_timer_list.end(); it++)
	{
		TimerItem* pItem = *it;
		if (pItem->callback == callback && pItem->user_data == user_data)
		{
			pItem->interval = interval;
			pItem->next_tick = get_tick_count() + interval;
			return;
		}
	}

	/* 不存在的事件，重新添加一个事件 */
	TimerItem* pItem = new TimerItem;
	pItem->callback = callback;
	pItem->user_data = user_data;
	pItem->interval = interval;
	pItem->next_tick = get_tick_count() + interval;
	m_timer_list.push_back(pItem);
}

void CEventDispatch::RemoveTimer(callback_t callback, void* user_data)
{
	list<TimerItem*>::iterator it;
	for (it = m_timer_list.begin(); it != m_timer_list.end(); it++)
	{
		TimerItem* pItem = *it;
		if (pItem->callback == callback && pItem->user_data == user_data)
		{
			m_timer_list.erase(it);
			delete pItem;
			return;
		}
	}
}

/* 定时任务 */
void CEventDispatch::_CheckTimer()
{
	uint64_t curr_tick = get_tick_count();
	list<TimerItem*>::iterator it;

	for (it = m_timer_list.begin(); it != m_timer_list.end(); )
	{
		TimerItem* pItem = *it;
		it++;		// iterator maybe deleted in the callback, so we should increment it before callback
		if (curr_tick >= pItem->next_tick)
		{
			pItem->next_tick += pItem->interval;
			pItem->callback(pItem->user_data, NETLIB_MSG_TIMER, 0, NULL);
		}
	}
}

/* 新增新的循环任务 */
void CEventDispatch::AddLoop(callback_t callback, void* user_data)
{
    TimerItem* pItem = new TimerItem;
    pItem->callback = callback;
    pItem->user_data = user_data;
    m_loop_list.push_back(pItem);
}

/* 循环任务 */
void CEventDispatch::_CheckLoop()
{
    for (list<TimerItem*>::iterator it = m_loop_list.begin(); it != m_loop_list.end(); it++) {
        TimerItem* pItem = *it;
        pItem->callback(pItem->user_data, NETLIB_MSG_LOOP, 0, NULL);
    }
}

CEventDispatch* CEventDispatch::Instance()
{
	if (m_pEventDispatch == NULL)
	{
		m_pEventDispatch = new CEventDispatch();
	}

	return m_pEventDispatch;
}

#ifdef _WIN32

/* 新增事件 */
void CEventDispatch::AddEvent(SOCKET fd, uint8_t socket_event)
{
	CAutoLock func_lock(&m_lock);

	if ((socket_event & SOCKET_READ) != 0)
	{
		FD_SET(fd, &m_read_set);
	}
		
	if ((socket_event & SOCKET_WRITE) != 0)
	{
		FD_SET(fd, &m_write_set);
	}

	if ((socket_event & SOCKET_EXCEP) != 0)
	{
		FD_SET(fd, &m_excep_set);
	}
}

/* 移除事件 */
void CEventDispatch::RemoveEvent(SOCKET fd, uint8_t socket_event)
{
	CAutoLock func_lock(&m_lock);

	if ((socket_event & SOCKET_READ) != 0)
	{
		FD_CLR(fd, &m_read_set);
	}

	if ((socket_event & SOCKET_WRITE) != 0)
	{
		FD_CLR(fd, &m_write_set);
	}

	if ((socket_event & SOCKET_EXCEP) != 0)
	{
		FD_CLR(fd, &m_excep_set);
	}
}

/* 开始事件分发 */
void CEventDispatch::StartDispatch(uint32_t wait_timeout)
{
	fd_set read_set, write_set, excep_set;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = wait_timeout * 1000;	// 10 millisecond

    if(running)
        return;
    running = true;
    
    while (running)
	{
		/* 检查定时任务 */
		_CheckTimer(); 
		/* 检查循环任务 */
        _CheckLoop();

		if (!m_read_set.fd_count && !m_write_set.fd_count && !m_excep_set.fd_count)
		{
			Sleep(MIN_TIMER_DURATION);
			continue;
		}

		m_lock.lock();
		memcpy(&read_set, &m_read_set, sizeof(fd_set));
		memcpy(&write_set, &m_write_set, sizeof(fd_set));
		memcpy(&excep_set, &m_excep_set, sizeof(fd_set));
		m_lock.unlock();

		/* 等待所有事件 */
		/* 包含读，写，异常 */
		int nfds = select(0, &read_set, &write_set, &excep_set, &timeout);

		if (nfds == SOCKET_ERROR)
		{
			log("select failed, error code: %d", GetLastError());
			Sleep(MIN_TIMER_DURATION);
			continue;			// select again
		}

		if (nfds == 0) 
		{
			continue;
		}

		/* 接收到读事件 */
		for (u_int i = 0; i < read_set.fd_count; i++)
		{
			//log("select return read count=%d\n", read_set.fd_count);
			SOCKET fd = read_set.fd_array[i];
			CBaseSocket* pSocket = FindBaseSocket((net_handle_t)fd);
			if (pSocket)
			{
				pSocket->OnRead();
				pSocket->ReleaseRef();
			}
		}

		/* 接收到写事件 */
		for (u_int i = 0; i < write_set.fd_count; i++)
		{
			//log("select return write count=%d\n", write_set.fd_count);
			SOCKET fd = write_set.fd_array[i];
			CBaseSocket* pSocket = FindBaseSocket((net_handle_t)fd);
			if (pSocket)
			{
				pSocket->OnWrite();
				pSocket->ReleaseRef();
			}
		}

		/* 接收到异常事件 */
		for (u_int i = 0; i < excep_set.fd_count; i++)
		{
			//log("select return exception count=%d\n", excep_set.fd_count);
			SOCKET fd = excep_set.fd_array[i];
			CBaseSocket* pSocket = FindBaseSocket((net_handle_t)fd);
			if (pSocket)
			{
				pSocket->OnClose();
				pSocket->ReleaseRef();
			}
		}

	}
}

void CEventDispatch::StopDispatch()
{
    running = false;
}

#elif __APPLE__

void CEventDispatch::AddEvent(SOCKET fd, uint8_t socket_event)
{
	struct kevent ke;

	if ((socket_event & SOCKET_READ) != 0)
	{
		EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		kevent(m_kqfd, &ke, 1, NULL, 0, NULL);
	}

	if ((socket_event & SOCKET_WRITE) != 0)
	{
		EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		kevent(m_kqfd, &ke, 1, NULL, 0, NULL);
	}
}

void CEventDispatch::RemoveEvent(SOCKET fd, uint8_t socket_event)
{
	struct kevent ke;

	if ((socket_event & SOCKET_READ) != 0)
	{
		EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		kevent(m_kqfd, &ke, 1, NULL, 0, NULL);
	}

	if ((socket_event & SOCKET_WRITE) != 0)
	{
		EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
		kevent(m_kqfd, &ke, 1, NULL, 0, NULL);
	}
}

void CEventDispatch::StartDispatch(uint32_t wait_timeout)
{
	struct kevent events[1024];
	int nfds = 0;
	struct timespec timeout;
	timeout.tv_sec = 0;
	timeout.tv_nsec = wait_timeout * 1000000;

    if(running)
        return;
    running = true;
    
    while (running)
	{
		nfds = kevent(m_kqfd, NULL, 0, events, 1024, &timeout);

		for (int i = 0; i < nfds; i++)
		{
			int ev_fd = events[i].ident;
			CBaseSocket* pSocket = FindBaseSocket(ev_fd);
			if (!pSocket)
				continue;

			if (events[i].filter == EVFILT_READ)
			{
				//log("OnRead, socket=%d\n", ev_fd);
				pSocket->OnRead();
			}

			if (events[i].filter == EVFILT_WRITE)
			{
				//log("OnWrite, socket=%d\n", ev_fd);
				pSocket->OnWrite();
			}

			pSocket->ReleaseRef();
		}

		_CheckTimer();
        _CheckLoop();
	}
}

void CEventDispatch::StopDispatch()
{
    running = false;
}

#else

void CEventDispatch::AddEvent(SOCKET fd, uint8_t socket_event)
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLERR | EPOLLHUP;
	ev.data.fd = fd;
	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) != 0)
	{
		log("epoll_ctl() failed, errno=%d", errno);
	}
}

void CEventDispatch::RemoveEvent(SOCKET fd, uint8_t socket_event)
{
	if (epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, NULL) != 0)
	{
		log("epoll_ctl failed, errno=%d", errno);
	}
}

void CEventDispatch::StartDispatch(uint32_t wait_timeout)
{
	struct epoll_event events[1024];
	int nfds = 0;

    if(running)
        return;
    running = true;
    
	while (running)
	{
		nfds = epoll_wait(m_epfd, events, 1024, wait_timeout);
		for (int i = 0; i < nfds; i++)
		{
			int ev_fd = events[i].data.fd;
			CBaseSocket* pSocket = FindBaseSocket(ev_fd);
			if (!pSocket)
				continue;
            
            //Commit by zhfu @2015-02-28
            #ifdef EPOLLRDHUP
            if (events[i].events & EPOLLRDHUP)
            {
                //log("On Peer Close, socket=%d, ev_fd);
                pSocket->OnClose();
            }
            #endif
            // Commit End

			if (events[i].events & EPOLLIN)
			{
				//log("OnRead, socket=%d\n", ev_fd);
				pSocket->OnRead();
			}

			if (events[i].events & EPOLLOUT)
			{
				//log("OnWrite, socket=%d\n", ev_fd);
				pSocket->OnWrite();
			}

			if (events[i].events & (EPOLLPRI | EPOLLERR | EPOLLHUP))
			{
				//log("OnClose, socket=%d\n", ev_fd);
				pSocket->OnClose();
			}

			pSocket->ReleaseRef();
		}

		_CheckTimer();
        _CheckLoop();
	}
}

void CEventDispatch::StopDispatch()
{
    running = false;
}


#endif
