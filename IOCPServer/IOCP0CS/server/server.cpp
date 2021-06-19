//https://www.cnblogs.com/xiaobingqianrui/p/9258665.html
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"kernel32.lib")

enum class IO_OPERATION
{
	IO_READ, IO_WRITE
};

struct IO_DATA
{//可以使用CONTAINING_RECORD，将OVERLAPPED转IO_DATA
	IO_OPERATION opCode;
	OVERLAPPED Overlapped;
	SOCKET client;
	WSABUF wsabuf;
	int nBytes;
};

HANDLE g_hIOCP = 0;
char g_buffer[1024] = { 0 };

DWORD WINAPI WorkerThread(LPVOID context)
{
	//cout << "WorkerThread() begin" << endl;
	while (true)
	{
		DWORD dwIoSize = 0;
		void* lpCompletionKey = NULL;
		LPOVERLAPPED lpOverlapped = NULL;
		BOOL bRet = GetQueuedCompletionStatus(g_hIOCP, &dwIoSize,
			(LPDWORD)&lpCompletionKey, (LPOVERLAPPED*)&lpOverlapped, INFINITE);
		cout << "GetQueuedCompletionStatus() bRet=" << bRet;
		cout << ", dwIoSize=" << dwIoSize << ", Key=" << lpCompletionKey << endl;
		IO_DATA* lpIOContext = CONTAINING_RECORD(lpOverlapped, IO_DATA, Overlapped);
		if (dwIoSize == 0)
		{
			cout << "Client disconnect" << endl;
			int nRet = closesocket(lpIOContext->client);
			cout << "closesocket() nRet=" << nRet << endl;
			delete lpIOContext;
			continue;
		}
		//cout << "client=" << hex << lpIOContext->client << endl;
		cout << "dwThreadId=" << dec << GetCurrentThreadId() << endl;
		cout << "opCode=" << hex << (int)lpIOContext->opCode << endl;
		if (lpIOContext->opCode == IO_OPERATION::IO_READ)
		{// a read operation complete
			cout << "Client IO_READ" << endl;
			ZeroMemory(&lpIOContext->Overlapped,
				sizeof(lpIOContext->Overlapped));
			lpIOContext->wsabuf.buf = g_buffer;
			lpIOContext->wsabuf.len = strlen(g_buffer) + 1;
			lpIOContext->opCode = IO_OPERATION::IO_WRITE;
			lpIOContext->nBytes = strlen(g_buffer) + 1;
			DWORD dwFlags = 0;
			DWORD nBytes = strlen(g_buffer) + 1;
			int nRet = WSASend(lpIOContext->client,
				&lpIOContext->wsabuf, 1, &nBytes,
				dwFlags, &(lpIOContext->Overlapped), NULL);
			if (nRet == SOCKET_ERROR)
			{
				int nErr = WSAGetLastError();
				if (ERROR_IO_PENDING != nErr)
				{
					cout << "WASSend Failed! nErr=" << nErr << endl;
					nRet = closesocket(lpIOContext->client);
					cout << "closesocket() nRet=" << nRet << endl;
					delete lpIOContext;
					continue;
				}
			}
			memset(g_buffer, NULL, sizeof(g_buffer));
		}
		else if (lpIOContext->opCode == IO_OPERATION::IO_WRITE)
		{//a write operation complete
			cout << "Client IO_WRITE" << endl;
			// Write operation completed, so post Read operation.
			DWORD dwFlags = 0;
			DWORD nBytes = sizeof(g_buffer);
			lpIOContext->opCode = IO_OPERATION::IO_READ;
			lpIOContext->wsabuf.buf = g_buffer;
			lpIOContext->wsabuf.len = nBytes;
			lpIOContext->nBytes = nBytes;
			ZeroMemory(&lpIOContext->Overlapped,
				sizeof(lpIOContext->Overlapped));
			int nRet = WSARecv(lpIOContext->client,
				&lpIOContext->wsabuf, 1, &nBytes,
				&dwFlags, &(lpIOContext->Overlapped), NULL);
			if (nRet == SOCKET_ERROR)
			{
				int nErr = WSAGetLastError();
				if (ERROR_IO_PENDING != nErr)
				{
					cout << "WASRecv Failed! nErr=" << nErr << endl;
					int nRet = closesocket(lpIOContext->client);
					cout << "closesocket() nRet=" << nRet << endl;
					delete lpIOContext;
					continue;
				}
			}
			cout << lpIOContext->wsabuf.buf << endl;
		}
	}
	cout << "WorkerThread() end" << endl;
	return 0;
}

int GetCpuCoreCount()
{
	SYSTEM_INFO sysInfo = { 0 };
	GetSystemInfo(&sysInfo);
	return sysInfo.dwNumberOfProcessors;
}

int main()
{
	int nThreadCount = GetCpuCoreCount() * 2;
	cout << "nThreadCount=" << nThreadCount << endl;

	WSADATA wsaData = { 0 };
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	cout << "WSAStartup() nRet=" << nRet << endl;
	if (nRet != NO_ERROR)
	{
		int nErr = WSAGetLastError();
		return 1;
	}
	SOCKET hSocket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	cout << "WSASocket() hSocket=" << hex << hSocket << endl;

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(6000);
	server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	nRet = bind(hSocket, (sockaddr*)&server, sizeof(server));
	cout << "bind() nRet=" << nRet << endl;
	nRet = listen(hSocket, nThreadCount);
	cout << "listen() nRet=" << nRet << endl;

	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		NULL, 0, nThreadCount); //dwCompletionKey, 0)
	cout << "CreateIOCP() hIOCP=" << hex << g_hIOCP << endl;
	for (int i = 0; i < nThreadCount; ++i)
	{
		HANDLE hThread = 0;
		DWORD dwThreadId = 0;
		hThread = CreateThread(NULL, 0, WorkerThread, 0, 0, &dwThreadId);
		cout << "CreateThread() hThread=" << hex << hThread;
		cout << ", dwThreadId=" << dec << dwThreadId << endl;
		if (hThread)
		{
			CloseHandle(hThread);
		}
	}

	while (hSocket)
	{
		SOCKET hClient = accept(hSocket, NULL, NULL);
		cout << "accept() hClient=" << hex << hClient << endl;
		HANDLE hIocpTemp = CreateIoCompletionPort((HANDLE)hClient, 
			g_hIOCP, hClient, 0); //dwNumberOfConcurrentThreads
		cout << "CreateIOCP() hIocpTemp=" << hex << hIocpTemp << endl;
		if (hIocpTemp == NULL)
		{
			int nErr = WSAGetLastError();
			cout << "Bind Socket2IOCP Failed! nErr=" << nErr << endl;
			nRet = closesocket(hClient);
			cout << "closesocket() nRet=" << nRet << endl;
			break; //不退出，会一直失败
		}
		else
		{ //post a recv request
			IO_DATA* data = new IO_DATA;
			cout << "data=" << data << endl;
			memset(data, 0, sizeof(IO_DATA));
			data->nBytes = 0;
			data->opCode = IO_OPERATION::IO_READ;
			memset(g_buffer, NULL, sizeof(g_buffer));
			data->wsabuf.buf = g_buffer;
			data->wsabuf.len = sizeof(g_buffer);
			data->client = hClient;
			DWORD nBytes = sizeof(g_buffer), dwFlags = 0;
			int nRet = WSARecv(hClient, &data->wsabuf, 1,
				&nBytes, &dwFlags, &(data->Overlapped), NULL);
			if (nRet == SOCKET_ERROR)
			{
				int nErr = WSAGetLastError();
				if (ERROR_IO_PENDING != nErr)
				{
					cout << "WASRecv Failed! nErr=" << nErr << endl;
					nRet = closesocket(hClient);
					cout << "closesocket() nRet=" << nRet << endl;
					delete data;
				}
			}
			////cout << data->wsabuf.buf << endl;
		}
	}
	nRet = closesocket(hSocket);
	cout << "closesocket() nRet=" << nRet << endl;
	nRet = WSACleanup();
	cout << "WSACleanup() nRet=" << nRet << endl;
	system("pause");
	return 0;
}