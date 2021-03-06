
// ClientFileDlg.h : 头文件
//

#pragma once

//自定义文件结构体
struct Fileinfo{
	CString FileName;
	unsigned int FileSize;
}; 

// CClientFileDlg 对话框
class CClientFileDlg : public CDialogEx
{
// 构造
public:
	CClientFileDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENTFILE_DIALOG };

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
	void FileSend();
	CString m_FilePath;
	CString m_FileName;
	CString m_FileSize;
	Fileinfo m_fileinfo;

	SOCKET m_socket;
	SOCKADDR_IN  svrAddr;
	UINT m_Port;
	CString m_strIPAddress;

	CWinThread* m_ClientThread;
	CWinThread* m_ConectThread;
	int dwCount = 0;

	afx_msg void OnBnClickedButtonLink();
	afx_msg void OnBnClickedButtonInterrupt();
	afx_msg void OnBnClickedButtonContinue();
};

#define READSIZE 1024*20
