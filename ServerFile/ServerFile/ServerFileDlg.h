
// ServerFileDlg.h : 头文件
//

#pragma once

//自定义文件结构体
struct Fileinfo{
	CString FileName;
	ULONGLONG FileSize;
};

// CServerFileDlg 对话框
class CServerFileDlg : public CDialogEx
{
// 构造
public:
	CServerFileDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVERFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonListen();
	afx_msg void OnBnClickedButtonSendFile();
	
	//控件对应的变量
	CString m_port;
	CString m_ip;


	CString strIPAddress, strHostName, strPort;

	//socket相关变量
	SOCKET m_socket;
	SOCKET m_clientSocket;
	SOCKADDR_IN svrAddr;
	SOCKADDR_IN addrClient;
	DWORD m_wPort;

	CWinThread* m_SeverThread;//线程句柄
	CWinThread* m_FileSendThread;

	//文件相关变量
	CString m_FilePath;
	CString m_FileName;
	Fileinfo m_fileinfo;
	int m_FileNumber;
	CString m_FileSize;

	int InitSocket(SOCKET &m_sock);
	int GetLocalHostInfo(CString &strHostName, CString &strIPAddress);
	
	afx_msg void OnBnClickedButtonChooseFile();
};

#define READSIZE 1024*20
