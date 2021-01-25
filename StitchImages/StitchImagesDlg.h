
// StitchImagesDlg.h: 头文件
//

#pragma once

#include<opencv.hpp>
#include<vector>
#include "CPassword.h"

using namespace std;
using namespace cv;

// CStitchImagesDlg 对话框
class CStitchImagesDlg : public CDialogEx
{
// 构造
public:
	CStitchImagesDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STITCHIMAGES_DIALOG };
#endif

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
	CButton m_horizontal;   //"横向拼接"单选按钮
	CButton m_vertical;   //"纵向拼接"单选按钮
	CProgressCtrl m_progress;   //进度条
	CString m_savePath;     //保存路径
	CString m_scale_down;   //缩小倍数
	int scale;   //缩小倍数
	vector<CString> vecPicPaths;    //所选图片路径容器
	CPassword dlgPassword;

	void PasswordCheck();   //校验密码文件是否正确
	afx_msg void OnBnClickedSelectPics();   //按下“选择图片文件”按钮
	afx_msg void OnBnClickedSelectSavepath();   //按下“选择保存路径”按钮
	afx_msg void OnBnClickedStitchPics();   //按下“拼接图片”按钮
	CString CStitchImagesDlg::ShowDialog();   //显示选择保存路径对话框
	void CStitchImagesDlg::SetProgressBar(int num, int num_all);   //设置进度条

	static UINT StitchPics(LPVOID pParam);
};
