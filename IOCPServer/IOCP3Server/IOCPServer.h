//IOCPServer.h : 应用程序的主头文件
#pragma once
// 有关此类的实现，请参阅 IOCPServer.cpp
class CMyServerApp : public CWinApp
{
public:
	CMyServerApp();

public:
	virtual BOOL InitInstance();
	// 实现
	DECLARE_MESSAGE_MAP()
};