
// ServerFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerFile.h"
#include "ServerFileDlg.h"
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


// CServerFileDlg �Ի���



CServerFileDlg::CServerFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerFileDlg::IDD, pParent)
	, m_port(_T(""))
	, m_ip(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Port, m_port);
	DDX_Text(pDX, IDC_IP, m_ip);

}

BEGIN_MESSAGE_MAP(CServerFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, &CServerFileDlg::OnBnClickedButtonListen)
	ON_BN_CLICKED(IDC_BUTTON_SEND_FILE, &CServerFileDlg::OnBnClickedButtonSendFile)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_FILE, &CServerFileDlg::OnBnClickedButtonChooseFile)
END_MESSAGE_MAP()


// CServerFileDlg ��Ϣ�������

BOOL CServerFileDlg::OnInitDialog()
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

	//��ʾIP
	if (GetLocalHostInfo(strHostName, strIPAddress))
		return	TRUE;
	GetDlgItem(IDC_IP)->SetWindowText(strIPAddress);
	strPort.Format(L"%d", 6000);
	GetDlgItem(IDC_Port)->SetWindowText(strPort);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CServerFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerFileDlg::OnPaint()
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
HCURSOR CServerFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�Զ������

UINT ServerWaitThread(LPVOID para)
{
	CServerFileDlg* sDlg = (CServerFileDlg*)para;
	int size = sizeof(SOCKADDR_IN);;
	sDlg->m_clientSocket = accept(sDlg->m_socket, (sockaddr*)&sDlg->addrClient, &size);
	sDlg->GetDlgItem(IDC_STATIC_STATE)->SetWindowText(L"�����Ѿ�����...");
	while (1)
	{
		//�ȴ�����
	}//whlie
	return 0;
}

int CServerFileDlg::GetLocalHostInfo(CString &strHostName, CString &strIPAddress)
{
	//��ʼ��winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char  HostName[100];
	gethostname(HostName, sizeof(HostName));// ��ñ���������.
	hostent* hn;
	hn = gethostbyname(HostName);//���ݱ����������õ�����ip

	strIPAddress = inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);//��ip�����ַ�����ʽ
	return 0;	
}

UINT FileSendThread(LPVOID para)
{
	CServerFileDlg* sDlg = (CServerFileDlg*)para;
	CFile file;
	if (!file.Open(sDlg->m_FilePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("�ļ���ʧ��"));
		return 1;
	}
	char buf[READSIZE] = { 0 };
	file.Seek(0, CFile::begin);
	int nLen = 0;//��ȡ����
	int nSize = 0;//���ͳ���
	DWORD dwCount = 0;
	for (;;)//��ʼ�����ļ�
	{
		//һ�ζ�ȡREADSIZE��С���ļ�����
		nLen = file.Read(buf, READSIZE);
		if (nLen == 0)
			break;
		nSize = send(sDlg->m_clientSocket, (const char *)buf, nLen, 0);
		dwCount += nSize;
	}
	file.Close();
	sDlg->GetDlgItem(IDC_BUTTON_SEND_FILE)->EnableWindow(TRUE);
	return 0;
}

void CServerFileDlg::OnBnClickedButtonListen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�����������׽���
	GetDlgItem(IDC_BUTTON_LISTEN)->EnableWindow(FALSE);

	if (InitSocket(m_socket) == FALSE)
	{
		AfxMessageBox(L"��ʼ���׽���ʧ��!");
		exit(0);
	}

	USES_CONVERSION;//���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
	char * pFileName = T2A(strIPAddress);
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.s_addr = inet_addr(pFileName);
	m_wPort = (DWORD)GetDlgItemInt(IDC_Port);
	svrAddr.sin_port = htons(m_wPort);
	if (bind(m_socket, (sockaddr *)&svrAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(L"�󶨴���!");
		exit(0);
	}
	if (listen(m_socket, 5)< 0)
	{
		AfxMessageBox(L"��������!");
		exit(0);
	}

	m_SeverThread = AfxBeginThread(ServerWaitThread, this);
	m_SeverThread->m_bAutoDelete = TRUE;

}

int CServerFileDlg::InitSocket(SOCKET &m_sock)
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		return FALSE;
	}

	if ((m_sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

void CServerFileDlg::OnBnClickedButtonChooseFile()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char* bufname;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"�����ļ� (*.*)|*.*||", this);
	if (dlg.DoModal() == IDOK)
	{

		m_FilePath = dlg.GetPathName();//��ȡ�ļ�·��	
		m_fileinfo.FileName = dlg.GetFileName();//��ȡ�ļ���	
		CFile file(m_FilePath, CFile::modeRead);//���ļ�	
		m_fileinfo.FileSize = file.GetLength();//��ȡ�ļ���С	
		file.Close();//�ر��ļ�

		//���ļ����ݸ��µ��ؼ���
		m_FileSize.Format(L"%d", m_fileinfo.FileSize);
		GetDlgItem(IDC_FILE_SIZE)->SetWindowText(m_FileSize);
		GetDlgItem(IDC_FILE)->SetWindowText(m_FilePath);

		GetDlgItem(IDC_BUTTON_CHOOSE_FILE)->EnableWindow(FALSE);

		int nSize = send(m_clientSocket, (const char *)&m_fileinfo.FileSize, 4, 0);//�����ļ���С

		int filenamelen = m_fileinfo.FileName.GetLength();
		nSize = send(m_clientSocket, (const char *)&filenamelen, 4, 0);//�����ļ�������

		USES_CONVERSION;
		bufname = T2A(m_fileinfo.FileName.GetBuffer(filenamelen));
		nSize = send(m_clientSocket, (const char *)bufname, filenamelen + 1, 0);//�����ļ���

	}
}

void CServerFileDlg::OnBnClickedButtonSendFile()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//������׼���÷����ļ�
	m_FileSendThread = AfxBeginThread(FileSendThread, this);
	m_FileSendThread->m_bAutoDelete = TRUE;

	GetDlgItem(IDC_BUTTON_SEND_FILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CHOOSE_FILE)->EnableWindow(TRUE);
}

