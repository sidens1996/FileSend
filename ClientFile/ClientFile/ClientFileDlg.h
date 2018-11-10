
// ClientFileDlg.h : ͷ�ļ�
//

#pragma once

//�Զ����ļ��ṹ��
struct Fileinfo{
	CString FileName;
	unsigned int FileSize;
}; 

// CClientFileDlg �Ի���
class CClientFileDlg : public CDialogEx
{
// ����
public:
	CClientFileDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENTFILE_DIALOG };

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
