
// NewFringePictureDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "NewFringePicture.h"
#include "NewFringePictureDlg.h"
#include "afxdialogex.h"
#include <opencv.hpp>
#include <iostream>
#include <math.h>


using namespace std;
using namespace cv;

//#define dir 0    //dir:0 横向条纹；1：纵向条纹 ；

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
	CEdit m_graymax, m_graymin,m_resx, m_resy, m_step, m_cycle;
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_graymax);   
	DDX_Control(pDX, IDC_EDIT3, m_graymin);
	DDX_Control(pDX, IDC_EDIT4, m_resx);
	DDX_Control(pDX, IDC_EDIT5, m_resy);
	DDX_Control(pDX, IDC_EDIT6, m_step);
	DDX_Control(pDX, IDC_EDIT7, m_cycle);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewFringePictureDlg 对话框



CNewFringePictureDlg::CNewFringePictureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEWFRINGEPICTURE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewFringePictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNewFringePictureDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, &CNewFringePictureDlg::OnBnClickedTransverse)
	ON_BN_CLICKED(IDC_RADIO2, &CNewFringePictureDlg::OnBnClickedVertical)
	ON_BN_CLICKED(IDOK, &CNewFringePictureDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewFringePictureDlg 消息处理程序

BOOL CNewFringePictureDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNewFringePictureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNewFringePictureDlg::OnPaint()
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
HCURSOR CNewFringePictureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNewFringePictureDlg::OnBnClickedTransverse()
{
	// TODO: 在此添加控件通知处理程序代码
	dir = 0;
}


void CNewFringePictureDlg::OnBnClickedVertical()
{
	// TODO: 在此添加控件通知处理程序代码
	dir = 1;
}


void CNewFringePictureDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	ofstream file;
	file.open("debug.txt");

	String FilePathName = "..\\pictures\\InverseIndex\\";
	String Name;
	//String FilePathName;
	int Dir = dir; //生成条纹的方向
	m_graymax = GetDlgItemInt(IDC_EDIT2);
	m_graymin = GetDlgItemInt(IDC_EDIT3);
	m_resx = GetDlgItemInt(IDC_EDIT4);
	m_resy = GetDlgItemInt(IDC_EDIT5);
	m_step = GetDlgItemInt(IDC_EDIT6);
	m_cycle = GetDlgItemInt(IDC_EDIT7);

	int phase; //相移值  0/90/180/270
	float cycle; //周期长度（像素）
	float pointphase;  //两像素相位间距
	float grayRatio = 0;//相对于行向64周期条纹图最大灰度的比例
	grayRatio = 1.0f;   //低频率防止过饱和
	
	Mat p_image;
	p_image.create(Size(m_resx, m_resy), CV_8UC1);  //   创建resolution_Y*resolution_X矩阵
	int channel = 1;
	int bitSize = channel;   //创建图像模型

						
	float kk, bb;
	int gray = 0;
	cycle = m_resx / m_cycle;//一周期的像素个数
								//phase = step * 360.0 / sequencePhaseIncrease;  //sequencePhaseIncrease:第几步
	pointphase = CV_PI * 2.0 / cycle; //每个像素值的弧度大小
									  //colPointer = lpbm;
	float Bxy = 0.0f;
	float Cxy = 0.0f;
	if (Dir == 0)  //生成横向条纹图
	{
		for (phase = 0; phase < m_step; phase++) {
			int angle = (90 * (phase + 1)) % 360;
			for (int m = 0; m < m_resx; m++)//从左到右
			{
				kk = pointphase * m + phase * CV_PI / 2.0;//当前像素值的弧度大小
				bb = sin(kk + CV_PI / 2.0); 
				for (int n = 0; n < m_resy; n++)
				{
					Bxy = (float)(m_graymin + m_graymax) / 2.0; //0对应的灰度值
					Cxy = (float)(m_graymax - m_graymin) / 2.0*grayRatio;
					gray = (int)(Bxy + Cxy * bb + 0.5);  //转为灰度值(就近取整NINT)
					p_image.at<uchar>(n, m) = gray;
					
				}
			}
			Name += "Transverse";
			char buf1[10];
			_itoa(m_cycle, buf1, 10);
			Name += buf1;
			Name += "-";
			char buf2[10];
            _itoa(angle, buf2, 10);
			Name += buf2;
			Name += ".bmp";
			FilePathName += Name;
			file << FilePathName << endl;

			imwrite(FilePathName, p_image);
			imshow(Name, p_image);	
			Name = "";
			FilePathName = "..\\pictures\\InverseIndex\\";
			waitKey(0);
		}
	}
	else
	{
		for (phase = 0; phase < m_step; phase++) {
			int angle = (90 * (phase + 1)) % 360;
			for (int m = m_resy - 1; m >= 0; m--)//自下而上
			{
				kk = pointphase * m + phase * CV_PI / 2.0;
				bb = sin(kk + CV_PI / 2.0);
				for (int n = 0; n < m_resx; n++)
				{
					Bxy = (float)(m_graymin + m_graymax) / 2.0;
					Cxy = (float)(m_graymax - m_graymin) / 2.0*grayRatio;
					gray = (int)(Bxy + Cxy * bb + 0.5);
					p_image.at<uchar>(m, n) = gray;
				}
			}
			Name += "Vertical";
			char buf1[10];
			_itoa(m_cycle, buf1, 10);
			Name += buf1;
			Name += "-";
			char buf2[10];
			_itoa(angle, buf2, 10);
			Name += buf2;
			Name += ".bmp";
			FilePathName += Name;
			file << FilePathName << endl;
			imwrite(FilePathName, p_image);
			imshow(Name, p_image);
			Name = "";
			FilePathName = "..\\pictures\\InverseIndex\\";
			waitKey(0);
		}
	}
	file.close();
}
