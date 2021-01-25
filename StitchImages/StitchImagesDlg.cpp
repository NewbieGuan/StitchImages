
// StitchImagesDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "StitchImages.h"
#include "StitchImagesDlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStitchImagesDlg 对话框



CStitchImagesDlg::CStitchImagesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STITCHIMAGES_DIALOG, pParent)

	, m_savePath(_T(""))
	, m_scale_down(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStitchImagesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO_H, m_horizontal);
	DDX_Control(pDX, IDC_RADIO_V, m_vertical);
	DDX_Text(pDX, EDIT_SAVEPATH, m_savePath);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, EDIT_SCALE_DOWN, m_scale_down);
}

BEGIN_MESSAGE_MAP(CStitchImagesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BT_SELECT_PICS, &CStitchImagesDlg::OnBnClickedSelectPics)
	ON_BN_CLICKED(BT_SELECT_SAVEPATH, &CStitchImagesDlg::OnBnClickedSelectSavepath)
	ON_BN_CLICKED(BT_STITCH_PICS, &CStitchImagesDlg::OnBnClickedStitchPics)
END_MESSAGE_MAP()


// CStitchImagesDlg 消息处理程序

BOOL CStitchImagesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码
	CheckRadioButton(IDC_RADIO_H, IDC_RADIO_V, IDC_RADIO_H);   //设置“横向拼接”为默认选中
	
	m_progress.SetRange32(0, 1000);   //设置进度条范围

	CSpinButtonCtrl *pSpin;
	pSpin = (CSpinButtonCtrl*)GetDlgItem(SPIN_SCALE_DOWN);
	pSpin->SetRange32(1, 100);   //设置数值调节范围为1-100
	pSpin->SetBase(10);   //设置数值为10进制

	CEdit *pEdit;
	pEdit= (CEdit*)GetDlgItem(EDIT_SCALE_DOWN);
	pEdit->SetWindowText(_T("1"));   //设置缩小倍数默认为1

	PasswordCheck(); //密码处理

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStitchImagesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CStitchImagesDlg::OnPaint()
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
HCURSOR CStitchImagesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//密码处理
void CStitchImagesDlg::PasswordCheck() {
	if (dlgPassword.is_correct()) return;

	GetDlgItem(BT_SELECT_PICS)->EnableWindow(FALSE);//让选择图片文件按钮暂时不能点击
	GetDlgItem(BT_SELECT_SAVEPATH)->EnableWindow(FALSE);//让选择保存路径按钮暂时不能点击
	GetDlgItem(BT_STITCH_PICS)->EnableWindow(FALSE);//让处理图片按钮暂时不能点击

	dlgPassword.DoModal();
	if (dlgPassword.is_correct()) {
		GetDlgItem(BT_SELECT_PICS)->EnableWindow(TRUE);
		GetDlgItem(BT_SELECT_SAVEPATH)->EnableWindow(TRUE);
		GetDlgItem(BT_STITCH_PICS)->EnableWindow(TRUE);
	}
}



//按下“选择图片文件”按钮
void CStitchImagesDlg::OnBnClickedSelectPics()
{
	// TODO: 在此添加控件通知处理程序代码
	vecPicPaths.clear();

	CFileDialog dlg(TRUE, _T("*.bmp"), NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("Image Files(*.bmp;*.jpg;*.png;*.tif)|*.bmp;*.jpg;*.png;*.tif||"), NULL);
	dlg.m_ofn.lpstrTitle = _T("选择图片");
	// 为了实现多文件同时添加
	DWORD max_file = 40000;             // 定义own filename buffer的大小
	TCHAR * lsf = new TCHAR[max_file];
	dlg.m_ofn.nMaxFile = max_file;
	dlg.m_ofn.lpstrFile = lsf;
	dlg.m_ofn.lpstrFile[0] = NULL;      // 初始化对话框

	CString filename = _T("");
	if (dlg.DoModal() == IDOK)
	{
		POSITION fileNamesPosition = dlg.GetStartPosition();
		while (fileNamesPosition != NULL)
		{
			filename = dlg.GetNextPathName(fileNamesPosition);
			vecPicPaths.push_back(filename);
		}
	}
	delete lsf;

	size_t count = vecPicPaths.size();
	if (count == 0)
		MessageBox(_T("没有选择文件"), NULL, MB_OK | MB_ICONQUESTION);
	else
		sort(vecPicPaths.begin(), vecPicPaths.end(), 
			[](const CString& a, const CString& b)->bool 
	{
		int temp = StrCmpLogicalW(a, b);
		bool result;
		switch (temp) {
		case -1:result = true; break;
		default:result = false; break;
		}
		return result;
	});   //对路径名进行排序，使之有序
}


//按下“选择保存路径”按钮
void CStitchImagesDlg::OnBnClickedSelectSavepath()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_savePath = ShowDialog();
	UpdateData(FALSE);
}


//按下“拼接图片”按钮
void CStitchImagesDlg::OnBnClickedStitchPics()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	scale = _ttoi(m_scale_down);
	if (vecPicPaths.empty())  //如果图片路径为空
		AfxMessageBox(_T("请选择图片！"));
	else if (!PathFileExists(m_savePath))  //如果结果保存路径为空
		AfxMessageBox(_T("请选择处理结果保存路径！"));
	else if(scale < 1 || scale>100)
		AfxMessageBox(_T("缩小倍数为1-100！"));
	else
	{
		GetDlgItem(BT_STITCH_PICS)->EnableWindow(FALSE);//让拼接图片按钮暂时不能点击
		unsigned int threadID;
		_beginthreadex(NULL, 0, &CStitchImagesDlg::StitchPics, this, 0, &threadID);   //开辟线程
	}
}


//显示选择保存路径对话框
CString CStitchImagesDlg::ShowDialog()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择处理结果存储路径");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			strFolderPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}

	return strFolderPath;
}


//线程函数
UINT CStitchImagesDlg::StitchPics(LPVOID pParam)
{
	CStitchImagesDlg* dlg = static_cast<CStitchImagesDlg*>(pParam);
	if (dlg)
	{
		bool is_normal = true;//正常时为1，异常时为0
		int path_size = dlg->vecPicPaths.size();
		dlg->SetProgressBar(0, path_size);   //设置初始进度条
		Mat combine;
		if (dlg->m_horizontal.GetCheck())//如果选中"横向拼接"
		{
			for (int num = 0; num < path_size; ++num)
			{
				string image_path= CT2A(dlg->vecPicPaths[num].GetString());//CString转std::string
				Mat image = imread(image_path, IMREAD_UNCHANGED);
				if (num == 0)
				{
					combine = image;
					dlg->SetProgressBar(num + 1, path_size);   //设置进度条
				}
				else if (image.rows != combine.rows)//如果图片行数不相等
				{
					AfxMessageBox(_T("图片大小不一致！"));
					is_normal = false;
					break;
				}
				else
				{
					hconcat(combine, image, combine);//横向拼接
					dlg->SetProgressBar(num + 1, path_size);   //设置进度条
				}
			}
		}
		else//如果选中"纵向拼接"
		{
			for (int num = 0; num < path_size; ++num)
			{
				string image_path = CT2A(dlg->vecPicPaths[num].GetString());//CString转std::string
				Mat image = imread(image_path, IMREAD_UNCHANGED);
				if (num == 0)
				{
					combine = image;
					dlg->SetProgressBar(num + 1, path_size);   //设置进度条
				}
				else if (image.cols != combine.cols)//如果图片列数不相等
				{
					AfxMessageBox(_T("图片大小不一致！"));
					is_normal = false;
					break;
				}
				else
				{
					vconcat(combine, image, combine);//纵向拼接
					dlg->SetProgressBar(num + 1, path_size);   //设置进度条
				}
			}
		}

		if (is_normal)
		{
			CString FileName, FileSuffix;
			FileName = dlg->vecPicPaths[0].Right(dlg->vecPicPaths[0].GetLength() - dlg->vecPicPaths[0].ReverseFind('\\') - 1);//从路径中截取文件名 "picture_1.bmp"
			FileSuffix = dlg->vecPicPaths[0].Right(dlg->vecPicPaths[0].GetLength() - dlg->vecPicPaths[0].ReverseFind('.')); //获取文件的后缀 ".bmp"
			string save_path = CT2A(dlg->m_savePath.GetString()) + string("\\") + "combine" + CT2A(FileSuffix.GetString());

			int rows_new = combine.rows / dlg->scale;
			int cols_new = combine.cols / dlg->scale;
			Mat combine_new;
			resize(combine, combine_new, Size(cols_new, rows_new));
			imwrite(save_path, combine_new);
			AfxMessageBox(_T("拼接完成！"));
		}
		else
			dlg->SetProgressBar(0, path_size);   //设置初始进度条
		dlg->GetDlgItem(BT_STITCH_PICS)->EnableWindow(TRUE);//让处理图片回到可以点击状态
	}
	_endthreadex(0);
	return 0;
}


//设置进度条
void CStitchImagesDlg::SetProgressBar(int num, int num_all)
{
	int nMin, nMax;
	m_progress.GetRange(nMin, nMax);
	int nPos = nMax * num / num_all;
	m_progress.SetPos(nPos);
}