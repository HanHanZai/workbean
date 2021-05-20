// MainDlg.h : 头文件
#pragma once
#include "Client.h"

#define MAX_LIST_ITEM_COUNT 1000
#define WM_ADD_LIST_ITEM (WM_USER + 100)  

// CMainDlg 对话框
class CMainDlg : public CDialog
{
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// 开始测试
	afx_msg void OnBnClickedOk();
	// 停止测试
	afx_msg void OnBnClickedStop();
	// 退出
	afx_msg void OnBnClickedCancel();
	// 对话框销毁
	afx_msg void OnDestroy();
	// 列表框内容的刷新（添加列表项）
	afx_msg LRESULT OnAddListItem(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	// 初始化界面信息
	void InitGUI();
	// 初始化List控件
	void InitListCtrl();
public:
	// 为主界面添加信息信息(在类CIocpModel中调用)
	static void AddInformation(const string& strInfo);

private:
	CClient m_Client; // 客户端连接对象
};
