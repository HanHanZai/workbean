#include <time.h>
#include "MyServer.h"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"User32.lib")

CMyServer* g_pServer = NULL;
HANDLE CMyServer::m_hMutex = CreateMutexA(NULL, FALSE, "m_hMutex");
vector<string> CMyServer::m_vtServerMsgs;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created.
	if (g_pServer)
	{
		return g_pServer->msgProc(msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

CMyServer::CMyServer(HINSTANCE hInstance)
{
	mhAppInst = hInstance;
	mhMainWnd = 0;
	mServerPort = DEFAULT_PORT;	//监听端口
	hEB_InputServerIP = NULL;
	hEB_InputServerPort = NULL;
	hLB_Output = NULL;
	hST_TextServerIP = NULL;
	hST_TextServerPort = NULL;
	lpszAppName = TEXT("GameServer");
	lpszTitle = TEXT("GameServer Window");
	hBtnStart = NULL;
	hBtnStop = NULL;
	hBtnExit = NULL;
	//初始化窗口
	this->InitMainWindow();
	LPVOID pfn = (LPVOID)AddServerMsgs;
	m_IOCP.SetLogFunc((LOG_FUNC)pfn);
	g_pServer = this;
}

CMyServer::~CMyServer()
{
}

HINSTANCE CMyServer::getAppInst()
{
	return mhAppInst;
}

HWND CMyServer::getMainWnd() {
	return mhMainWnd;
}

void CMyServer::Init()
{
	// 初始化Socket库
	if (m_IOCP.LoadSocketLib() == false)
	{
		MessageBoxA(NULL, "加载Winsock 2.2失败，服务器端无法运行！",
			"提示!", MB_OK);
		PostQuitMessage(0);
	}
	//初始化窗口子控件
	this->InitControls(mhMainWnd);
	//ip地址和监听端口
	SetWindowTextA(hEB_InputServerIP,
		m_IOCP.GetLocalIP().c_str());
	char port[10] = { 0 };
	_itoa_s(this->mServerPort, port, 10);
	SetWindowTextA(hEB_InputServerPort, port);
	EnableWindow(hEB_InputServerIP, false);
}

void CMyServer::ShutDown()
{
	m_IOCP.Stop();
}

int CMyServer::Run()
{
	MSG msg;
	msg.message = WM_NULL;
	static long last_time = (long)time(0);
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//处理消息显示工作
			this->SettleServerMsgs();
		}
	}
	return (int)msg.wParam;
}

void CMyServer::InitMainWindow()
{
	WNDCLASSEX	wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = MainWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = mhAppInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpszAppName;	// Registered Class Name

	if (RegisterClassEx(&wndclass) == 0)
	{
		MessageBoxA(0, "RegisterClass FAILED", 0, 0);
		exit(1);
	}
	// Create the window
	mhMainWnd = CreateWindow(lpszAppName,
		lpszTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX,
		200, 200, 800, 600, NULL, NULL, mhAppInst, NULL);
	if (!mhMainWnd)
	{
		MessageBoxA(0, "CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}
	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
}

void CMyServer::InitControls(HWND hWnd)
{
	// hEB_InputServerIP
	hST_TextServerIP = CreateWindowA("static", "Server IP",
		WS_CHILD | SS_CENTER | WS_VISIBLE, 5, 5, 120, 28,
		hWnd, (HMENU)IDC_hST_TextServerIP, mhAppInst, NULL);

	// hEB_InputServerIP
	hST_TextServerPort = CreateWindowA("static", "Port",
		WS_CHILD | SS_CENTER | WS_VISIBLE, 125, 5, 50, 28,
		hWnd, (HMENU)IDC_hST_TextServerPort, mhAppInst, NULL);

	// hEB_InputServerIP
	hEB_InputServerIP = CreateWindowExA(WS_EX_CLIENTEDGE,
		"EDIT", "192.168.0.2", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		5, 20, 120, 28,
		hWnd, (HMENU)IDC_hEB_InputServerIP, mhAppInst, NULL);

	// hEB_InputServerPort
	hEB_InputServerPort = CreateWindowExA(WS_EX_CLIENTEDGE,
		"EDIT", "6000", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		125, 20, 50, 28,
		hWnd, (HMENU)IDC_hEB_InputServerPort, mhAppInst, NULL);

	// hLB_Output
	hLB_Output = CreateWindowExA(WS_EX_CLIENTEDGE,
		"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY
		| WS_VSCROLL | WS_HSCROLL | WS_BORDER, 5, 50, 780, 480,
		hWnd, (HMENU)IDC_hLB_Output, mhAppInst, NULL);

	//开始监听
	hBtnStart = CreateWindowA("BUTTON", "开始监听",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 5, 530, 100, 28,
		hWnd, (HMENU)IDC_hBtn_Start, mhAppInst, NULL);

	//停止监听
	hBtnStop = CreateWindowA("BUTTON", "停止监听",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 530, 100, 28,
		hWnd, (HMENU)IDC_hBtn_Stop, mhAppInst, NULL);

	//退出按钮
	hBtnExit = CreateWindowA("BUTTON", "退出",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 650, 530, 100, 28,
		hWnd, (HMENU)IDC_hBtn_Exit, mhAppInst, NULL);
}

//添加服务器消息
void CMyServer::AddServerMsgs(const string& msg)
{
	WaitForSingleObject(m_hMutex, INFINITE);
	m_vtServerMsgs.push_back(msg);
	ReleaseMutex(m_hMutex);
}

//处理服务器消息
void CMyServer::SettleServerMsgs()
{
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_vtServerMsgs.size() > 0)
	{
		for (DWORD i = 0; i < m_vtServerMsgs.size(); i++)
		{
			this->ShowText(m_vtServerMsgs.at(i));
		}
	}
	else
	{
		Sleep(10);
	}
	m_vtServerMsgs.clear();
	ReleaseMutex(m_hMutex);
}

void CMyServer::ShowText(string& msg)
{
	int Line;
	// add string to the listbox
	SendMessageA(hLB_Output, LB_ADDSTRING, 0, (LPARAM)msg.c_str());
	// determine number of items in listbox
	Line = SendMessage(hLB_Output, LB_GETCOUNT, 0, 0);
	// flag last item as the selected item, to scroll listbox down
	SendMessage(hLB_Output, LB_SETCURSEL, Line - 1, 0);
	// unflag all items to eliminate negative highlite
	SendMessage(hLB_Output, LB_SETCURSEL, -1, 0);
}

LRESULT CMyServer::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_hBtn_Start:
			//ShowText(string("Connecting to Server"));
			//设置监听端口
			char aa[64];// = { 0 };
			GetWindowTextA(hEB_InputServerPort, aa, 64);
			this->mServerPort = atoi(aa); //atoi
			this->m_IOCP.Start(this->mServerPort);
			EnableWindow(hBtnStart, false);
			break;

		case IDC_hBtn_Stop:
			this->m_IOCP.Stop();
			EnableWindow(hBtnStart, true);
			break;

		case IDC_hBtn_Exit:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_CLOSE:
	{
		DestroyWindow(mhMainWnd);
	}
	return 0;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;
	}
	return DefWindowProc(mhMainWnd, msg, wParam, lParam);
}