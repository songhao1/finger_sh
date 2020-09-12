
// FingerDlg.h: 头文件
//

#pragma once

#include "CZKFPEngX.h"//引入指纹采集器SDK


#include<direct.h>//for _mkdir()
#define TEMP_DIR "temp\\"//临时文件夹，用于存放指纹入库前的所有中间处理结果
#define TEMP "temp"
#define DB_DIR "Database\\"//指纹库文件夹
#define DB "Database"//指纹库文件夹
#define DB_INDEX_TXT "Database\\index.txt"//指纹库索引文件
void InitDatabase();//创建并初始化指纹库
void InitFile(char *filename);//创建并初始化文件
void GetDatabaseInfo(char *info);//获取指纹库统计信息


// CFingerDlg 对话框
class CFingerDlg : public CDialogEx
{
// 构造
public:
	CFingerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	/*DECLARE_EVENTSINK_MAP()
	afx_msg void OnImageReceivedZkfpengx(BOOL FAR* AImageValid);*/
public:
	CStatic m_staticInfo;
	CStatic m_picCtrl1;
	CStatic m_picCtrl2;
	CStatic m_picCtrl3;
	CStatic m_picCtrl4;
	CStatic m_picCtrl5;
	CStatic m_picCtrl6;
	CStatic m_picCtrl7;
	CStatic m_picCtrl8;
	CStatic m_picCtrl9;
	CStatic m_picCtrl10;
	CStatic m_picCtrl11;
	CStatic m_picCtrl12;
	CString m_name;
	int m_minuCount;
	CZKFPEngX m_zkfpEng;
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnStep1();
	afx_msg void OnBnClickedBtnDatabase();
	afx_msg void OnBnClickedBtnStep12a();
	afx_msg void OnBnClickedBtnStep12b();
	afx_msg void OnBnClickedBtnStep12b2();
};
