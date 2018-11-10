
// ClientFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientFile.h"
#include "ClientFileDlg.h"
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


// CClientFileDlg 对话框



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


// CClientFileDlg 消息处理程序

BOOL CClientFileDlg::OnInitDialog()
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
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(L"6000");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientFileDlg::OnPaint()
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
HCURSOR CClientFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//自定义函数
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

		int length = recv(pDlg->m_socket, (char*)(&pDlg->m_fileinfo.FileSize), sizeof(4), 0);//接收文件长度
		if (length <= 0){ break; }
		length = recv(pDlg->m_socket, (char*)(&len), sizeof(4), 0);//接收文件名长度	
		if (length <= 0){ break; }
		length = recv(pDlg->m_socket, (char*)bufname, len + 1, 0);//接收文件名
		if (length <= 0){ break; }
		pDlg->dwCount = 0;//遇到新文件文件大小清0
		pDlg->m_FilePath = L"D:\\TEST\\" + (CString)bufname;
		length = 0;
		CFile file(pDlg->m_FilePath, CFile::modeCreate | CFile::modeWrite);
		pDlg->m_FileName = file.GetFileName();	//获取保持文件名
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
		file.Close();//关闭文件
		CString m_FileSize;
		m_FileSize.Format(L"%d", pDlg->m_fileinfo.FileSize);
	}//whlie
	return 0;
}

UINT ConnectThread(LPVOID para)
{
	CClientFileDlg* pDlg = (CClientFileDlg*)para;
	//初始化winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	pDlg->GetDlgItem(IDC_BUTTON_LINK)->EnableWindow(FALSE);

	pDlg->m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	pDlg->svrAddr.sin_family = AF_INET;
	pDlg->GetDlgItem(IDC_IPADDRESS)->GetWindowText(pDlg->m_strIPAddress);	//获取IP
	USES_CONVERSION;//调用函数，T2A和W2A均支持ATL和MFC中的字符转换
	char * pFileName = T2A(pDlg->m_strIPAddress);
	pDlg->svrAddr.sin_addr.s_addr = inet_addr(pFileName);
	pDlg->m_Port = (DWORD)pDlg->GetDlgItemInt(IDC_EDIT_PORT);
	pDlg->svrAddr.sin_port = htons(pDlg->m_Port);
	memset(&(pDlg->svrAddr.sin_zero), 0, sizeof(pDlg->svrAddr.sin_zero));

	// 向服务器发出连接请求 
	if (connect(pDlg->m_socket, (SOCKADDR *)&pDlg->svrAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		AfxMessageBox(L"connet fail!");
		pDlg->GetDlgItem(IDC_BUTTON_LINK)->EnableWindow(TRUE);
		return 1;
	}
	pDlg->GetDlgItem(IDC_STATIC_STATE)->SetWindowText(L"连接已经建立...");

	//连接建立后开启发送线程
	pDlg->m_ClientThread = AfxBeginThread(ClientWaitThread, pDlg);
	pDlg->m_ClientThread->m_bAutoDelete = TRUE;
	return 0;
}

void CClientFileDlg::OnBnClickedButtonLink()
{
	// TODO:  在此添加控件通知处理程序代码
	
	//套接字创建
	m_ConectThread = AfxBeginThread(ConnectThread, this);
	m_ConectThread->m_bAutoDelete = TRUE;

}


void CClientFileDlg::OnBnClickedButtonInterrupt()
{
	// TODO:  在此添加控件通知处理程序代码
	m_ClientThread->SuspendThread();
}


void CClientFileDlg::OnBnClickedButtonContinue()
{
	// TODO:  在此添加控件通知处理程序代码
	m_ClientThread->ResumeThread();
}
