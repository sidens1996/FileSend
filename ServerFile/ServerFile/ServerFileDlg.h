
// ServerFileDlg.h : ͷ�ļ�
//

#pragma once

//�Զ����ļ��ṹ��
struct Fileinfo{
	CString FileName;
	ULONGLONG FileSize;
};

// CServerFileDlg �Ի���
class CServerFileDlg : public CDialogEx
{
// ����
public:
	CServerFileDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVERFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonListen();
	afx_msg void OnBnClickedButtonSendFile();
	
	//�ؼ���Ӧ�ı���
	CString m_port;
	CString m_ip;


	CString strIPAddress, strHostName, strPort;

	//socket��ر���
	SOCKET m_socket;
	SOCKET m_clientSocket;
	SOCKADDR_IN svrAddr;
	SOCKADDR_IN addrClient;
	DWORD m_wPort;

	CWinThread* m_SeverThread;//�߳̾��
	CWinThread* m_FileSendThread;

	//�ļ���ر���
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
