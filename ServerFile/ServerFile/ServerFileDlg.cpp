
// ServerFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServerFile.h"
#include "ServerFileDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CServerFileDlg 对话框



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


// CServerFileDlg 消息处理程序

BOOL CServerFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	//显示IP
	if (GetLocalHostInfo(strHostName, strIPAddress))
		return	TRUE;
	GetDlgItem(IDC_IP)->SetWindowText(strIPAddress);
	strPort.Format(L"%d", 6000);
	GetDlgItem(IDC_Port)->SetWindowText(strPort);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//自定义代码

UINT ServerWaitThread(LPVOID para)
{
	CServerFileDlg* sDlg = (CServerFileDlg*)para;
	int size = sizeof(SOCKADDR_IN);;
	sDlg->m_clientSocket = accept(sDlg->m_socket, (sockaddr*)&sDlg->addrClient, &size);
	sDlg->GetDlgItem(IDC_STATIC_STATE)->SetWindowText(L"连接已经建立...");
	while (1)
	{
		//等待连接
	}//whlie
	return 0;
}

int CServerFileDlg::GetLocalHostInfo(CString &strHostName, CString &strIPAddress)
{
	//初始化winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char  HostName[100];
	gethostname(HostName, sizeof(HostName));// 获得本机主机名.
	hostent* hn;
	hn = gethostbyname(HostName);//根据本机主机名得到本机ip

	strIPAddress = inet_ntoa(*(struct in_addr *)hn->h_addr_list[0]);//把ip换成字符串形式
	return 0;	
}

UINT FileSendThread(LPVOID para)
{
	CServerFileDlg* sDlg = (CServerFileDlg*)para;
	CFile file;
	if (!file.Open(sDlg->m_FilePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("文件打开失败"));
		return 1;
	}
	char buf[READSIZE] = { 0 };
	file.Seek(0, CFile::begin);
	int nLen = 0;//读取长度
	int nSize = 0;//发送长度
	DWORD dwCount = 0;
	for (;;)//开始传送文件
	{
		//一次读取READSIZE大小的文件内容
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
	// TODO:  在此添加控件通知处理程序代码
	//创建服务器套接字
	GetDlgItem(IDC_BUTTON_LISTEN)->EnableWindow(FALSE);

	if (InitSocket(m_socket) == FALSE)
	{
		AfxMessageBox(L"初始化套接字失败!");
		exit(0);
	}

	USES_CONVERSION;//调用函数，T2A和W2A均支持ATL和MFC中的字符转换
	char * pFileName = T2A(strIPAddress);
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.s_addr = inet_addr(pFileName);
	m_wPort = (DWORD)GetDlgItemInt(IDC_Port);
	svrAddr.sin_port = htons(m_wPort);
	if (bind(m_socket, (sockaddr *)&svrAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(L"绑定错误!");
		exit(0);
	}
	if (listen(m_socket, 5)< 0)
	{
		AfxMessageBox(L"监听错误!");
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
	// TODO:  在此添加控件通知处理程序代码
	char* bufname;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"所有文件 (*.*)|*.*||", this);
	if (dlg.DoModal() == IDOK)
	{

		m_FilePath = dlg.GetPathName();//获取文件路径	
		m_fileinfo.FileName = dlg.GetFileName();//获取文件名	
		CFile file(m_FilePath, CFile::modeRead);//打开文件	
		m_fileinfo.FileSize = file.GetLength();//获取文件大小	
		file.Close();//关闭文件

		//将文件数据更新到控件上
		m_FileSize.Format(L"%d", m_fileinfo.FileSize);
		GetDlgItem(IDC_FILE_SIZE)->SetWindowText(m_FileSize);
		GetDlgItem(IDC_FILE)->SetWindowText(m_FilePath);

		GetDlgItem(IDC_BUTTON_CHOOSE_FILE)->EnableWindow(FALSE);

		int nSize = send(m_clientSocket, (const char *)&m_fileinfo.FileSize, 4, 0);//发送文件大小

		int filenamelen = m_fileinfo.FileName.GetLength();
		nSize = send(m_clientSocket, (const char *)&filenamelen, 4, 0);//发送文件名长度

		USES_CONVERSION;
		bufname = T2A(m_fileinfo.FileName.GetBuffer(filenamelen));
		nSize = send(m_clientSocket, (const char *)bufname, filenamelen + 1, 0);//发送文件名

	}
}

void CServerFileDlg::OnBnClickedButtonSendFile()
{
	// TODO:  在此添加控件通知处理程序代码
	//服务器准备好发送文件
	m_FileSendThread = AfxBeginThread(FileSendThread, this);
	m_FileSendThread->m_bAutoDelete = TRUE;

	GetDlgItem(IDC_BUTTON_SEND_FILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CHOOSE_FILE)->EnableWindow(TRUE);
}

