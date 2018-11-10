
// ClientFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientFile.h"
#include "ClientFileDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientFileDlg �Ի���



CClientFileDlg::CClientFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LINK, &CClientFileDlg::OnBnClickedButtonLink)
	ON_BN_CLICKED(IDC_BUTTON_INTERRUPT, &CClientFileDlg::OnBnClickedButtonInterrupt)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, &CClientFileDlg::OnBnClickedButtonContinue)
END_MESSAGE_MAP()


// CClientFileDlg ��Ϣ�������

BOOL CClientFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(L"6000");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�Զ��庯��
UINT ClientWaitThread(LPVOID para)
{
	CClientFileDlg* pDlg = (CClientFileDlg*)para;

	char bufname[1024];
	char buf[READSIZE];
	memset(buf, 0, sizeof(buf));
	while (1)
	{
		if (pDlg->m_socket == NULL)
			break;
		int len = 0;

		int length = recv(pDlg->m_socket, (char*)(&pDlg->m_fileinfo.FileSize), sizeof(4), 0);//�����ļ�����
		if (length <= 0){ break; }
		length = recv(pDlg->m_socket, (char*)(&len), sizeof(4), 0);//�����ļ�������	
		if (length <= 0){ break; }
		length = recv(pDlg->m_socket, (char*)bufname, len + 1, 0);//�����ļ���
		if (length <= 0){ break; }
		pDlg->dwCount = 0;//�������ļ��ļ���С��0
		pDlg->m_FilePath = L"D:\\TEST\\" + (CString)bufname;
		length = 0;
		CFile file(pDlg->m_FilePath, CFile::modeCreate | CFile::modeWrite);
		pDlg->m_FileName = file.GetFileName();	//��ȡ�����ļ���
		pDlg->GetDlgItem(IDC_EDIT_FILE_PATH)->SetWindowText(pDlg->m_FilePath);
		for (;;)
		{
			length = recv(pDlg->m_socket, (char*)buf, READSIZE, 0);
			if (length == 0)
				break;
			file.Write(buf, length);
			pDlg->dwCount += length;
			CString str_dwCount;
			str_dwCount.Format(L"%d", pDlg->dwCount);
			pDlg->GetDlgItem(IDC_EDIT_FILE_SIZE)->SetWindowText(str_dwCount);
			if (pDlg->dwCount == pDlg->m_fileinfo.FileSize)
				break;

		}//for(;;)
		file.Close();//�ر��ļ�
		CString m_FileSize;
		m_FileSize.Format(L"%d", pDlg->m_fileinfo.FileSize);
	}//whlie
	return 0;
}

UINT ConnectThread(LPVOID para)
{
	CClientFileDlg* pDlg = (CClientFileDlg*)para;
	//��ʼ��winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	pDlg->GetDlgItem(IDC_BUTTON_LINK)->EnableWindow(FALSE);

	pDlg->m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	pDlg->svrAddr.sin_family = AF_INET;
	pDlg->GetDlgItem(IDC_IPADDRESS)->GetWindowText(pDlg->m_strIPAddress);	//��ȡIP
	USES_CONVERSION;//���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
	char * pFileName = T2A(pDlg->m_strIPAddress);
	pDlg->svrAddr.sin_addr.s_addr = inet_addr(pFileName);
	pDlg->m_Port = (DWORD)pDlg->GetDlgItemInt(IDC_EDIT_PORT);
	pDlg->svrAddr.sin_port = htons(pDlg->m_Port);
	memset(&(pDlg->svrAddr.sin_zero), 0, sizeof(pDlg->svrAddr.sin_zero));

	// ������������������� 
	if (connect(pDlg->m_socket, (SOCKADDR *)&pDlg->svrAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		AfxMessageBox(L"connet fail!");
		pDlg->GetDlgItem(IDC_BUTTON_LINK)->EnableWindow(TRUE);
		return 1;
	}
	pDlg->GetDlgItem(IDC_STATIC_STATE)->SetWindowText(L"�����Ѿ�����...");

	//���ӽ������������߳�
	pDlg->m_ClientThread = AfxBeginThread(ClientWaitThread, pDlg);
	pDlg->m_ClientThread->m_bAutoDelete = TRUE;
	return 0;
}

void CClientFileDlg::OnBnClickedButtonLink()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	//�׽��ִ���
	m_ConectThread = AfxBeginThread(ConnectThread, this);
	m_ConectThread->m_bAutoDelete = TRUE;

}


void CClientFileDlg::OnBnClickedButtonInterrupt()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_ClientThread->SuspendThread();
}


void CClientFileDlg::OnBnClickedButtonContinue()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_ClientThread->ResumeThread();
}
