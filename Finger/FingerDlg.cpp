#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)
//#define _CRT_SECURE_NO_WARNINGS


// FingerDlg.cpp: 实现文件
//

#include "pch.h"

#include "Resource.h"
#include "framework.h"
#include "Finger.h"
#include "FingerDlg.h"
#include "afxdialogex.h"
#include<io.h>//for filelength()


#include <iomanip>
#include<fstream>
using namespace std;
#define STEP_IMG_1 "temp\\Step1_Source.bmp"
#define STEP_TXT_1 "temp\\Step1_Source.txt"
#define STEP_TXT_2 "temp\\Step2_MidFilter.txt"
#define STEP_IMG_2 "temp\\Step2_MidFilter.bmp"
#define STEP_TXT_3 "temp\\Step3_Normalize.txt"
#define STEP_IMG_3 "temp\\Step3_Normalize.bmp"
#define STEP_IMG_4 "temp\\Step4_Direction.bmp"
#define STEP_TXT_4 "temp\\Step4_Direction.txt"
#define STEP_TXT_5 "temp\\Step5_Frequency.txt"
#define STEP_IMG_5 "temp\\Step5_Frequency.bmp"
#define STEP_TXT_6 "temp\\Step6_Mask.txt"
#define STEP_IMG_6 "temp\\Step6_Mask.bmp"
#define STEP_TXT_7 "temp\\Step7_GaborEnhance.txt"
#define STEP_IMG_7 "temp\\Step7_GaborEnhance.bmp"
#define STEP_TXT_8 "temp\\Step8_Binary.txt"
#define STEP_IMG_8 "temp\\Step8_Binary.bmp"
#define STEP_TXT_9 "temp\\Step9_Thinning.txt"
#define STEP_IMG_9 "temp\\Step9_Thinning.bmp"
#define STEP_TXT_10 "temp\\Step10_MinuExtract.txt"
#define STEP_IMG_10 "temp\\Step10_MinuExtract.bmp"
#define STEP_TXT_11 "temp\\Step11_MinuFilter.txt"
#define STEP_IMG_11 "temp\\Step11_MinuFilter.bmp"
#define STEP_IMG_11_MDL "temp\\Step11_MinuFilter_MDL.mdl"
#define STEP_IMG_12 "temp\\Step12_Result.bmp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFingerDlg 对话框



CFingerDlg::CFingerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINGER_DIALOG, pParent)
	, m_name(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFingerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_STATIC_IMG_1, m_picCtrl1);
	DDX_Control(pDX, IDC_STATIC_IMG_2, m_picCtrl2);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Control(pDX, IDC_ZKFPENGX, m_zkfpEng);
	DDX_Control(pDX, IDC_STATIC_IMG_3, m_picCtrl3);
	DDX_Control(pDX, IDC_STATIC_IMG_4, m_picCtrl4);
	DDX_Control(pDX, IDC_STATIC_IMG_5, m_picCtrl5);
	DDX_Control(pDX, IDC_STATIC_IMG_6, m_picCtrl6);
	DDX_Control(pDX, IDC_STATIC_IMG_7, m_picCtrl7);
	DDX_Control(pDX, IDC_STATIC_IMG_8, m_picCtrl8);
	DDX_Control(pDX, IDC_STATIC_IMG_9, m_picCtrl9);
	DDX_Control(pDX, IDC_STATIC_IMG_10, m_picCtrl10);
	DDX_Control(pDX, IDC_STATIC_IMG_11, m_picCtrl11);
	DDX_Control(pDX, IDC_STATIC_IMG_12, m_picCtrl12);
}

BEGIN_MESSAGE_MAP(CFingerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CFingerDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_STEP_1, &CFingerDlg::OnBnClickedBtnStep1)
	ON_BN_CLICKED(IDC_BTN_DATABASE, &CFingerDlg::OnBnClickedBtnDatabase)
	ON_BN_CLICKED(IDC_BTN_STEP_12A, &CFingerDlg::OnBnClickedBtnStep12a)
	ON_BN_CLICKED(IDC_BTN_STEP_12B, &CFingerDlg::OnBnClickedBtnStep12b)
	ON_BN_CLICKED(IDC_BTN_STEP_12B2, &CFingerDlg::OnBnClickedBtnStep12b2)
END_MESSAGE_MAP()
//BEGIN_EVENTSINK_MAP(CFingerDlg,CDialog)
//	ON_EVENT(CFingerDlg,IDC_ZKFPENGX,8,OnImageReceivedZkfpengx,VTS_PBOOL)
//END_EVENTSINK_MAP()


// CFingerDlg 消息处理程序

BOOL CFingerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	InitDatabase();//创建并初始化指纹库

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFingerDlg::OnPaint()
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
HCURSOR CFingerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//void CFingerDlg::OnImageReceivedZkfpengx(BOOL FAR* AImageValid)
//{
//	HDC hdc = this->GetDC()->m_hDC;//获得显示设备上下文环境的句柄
//	int x = 160;//图像绘制区左上角横坐标
//	int y = 80;//图像绘制区左上角纵坐标
//	int width = m_zkfpEng.get_ImageWidth();//图像绘制区宽度
//	int height = m_zkfpEng.get_ImageHeight();//图像绘制区高度
//	m_zkfpEng.PrintImageAt(int(hdc), x, y, width, height);//绘制图像
//}




wchar_t *ToWideChar(char *str)//多字节字符串转换为Unicode宽字符串
{
	int num = MultiByteToWideChar(0, 0, str, -1, NULL, 0);
	wchar_t *wideStr = new wchar_t[num];
	MultiByteToWideChar(0, 0, str, -1, wideStr, num);
	return wideStr;
}


//参数:picCtrl(图像控件)，filename(图像文件路径)
int ShowImageInCtrl(CStatic &picCtrl, char *filename)
{
	//载入图像
	CImage image;
	HRESULT hResult = image.Load(ToWideChar(filename));

	//获得图像参数
	int width = image.GetWidth();//图像宽度
	int height = image.GetHeight();//图像高度

	//设置显示区域
	CRect rect;//定义矩形对象
	picCtrl.GetClientRect(&rect);//获得pictrue控件所在的矩形区域
	CDC *pDc = picCtrl.GetWindowDC();//获得pictrue控件的设备环境句柄
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置位图拉伸模式

	//显示图像
	image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//将图像画到Picture控件表示的矩形区域

	//更新控件显示
	picCtrl.Invalidate(false);

	//释放变量空间
	image.Destroy();
	picCtrl.ReleaseDC(pDc);//释放picture控件的设备环境句柄

	return 0;
}


int ReadBMPImgFilePara(char *fileName, int &width, int &height, int &depth)
{
	//载入图像
	CImage image;
	HRESULT hResult = image.Load(ToWideChar(fileName));
	if (FAILED(hResult) || image.IsNull())//图像损坏或文件不存在等因素均可导致载入失败
	{
		return -1;
	}

	//获得图像参数
	width = image.GetWidth();//获得以像素为单位的图像宽度
	height = image.GetHeight();//获得以像素为单位的图像高度
	depth = image.GetBPP();//获得以bit为单位的图像深度（每个像素的位数）

	if (depth != 8)//目前系统仅支持8位位图，其他类型位图暂不支持
	{
		return -2;
	}

	//释放变量空间
	image.Destroy();
	return 0;
}

int ReadBMPImgFileData(char *fileName, unsigned char *data)
{
	//载入图像
	CImage image;
	HRESULT hResult = image.Load(ToWideChar(fileName));
	if (FAILED(hResult) || image.IsNull())//图像损坏或文件不存在等因素均可导致载入失败
	{
		return -1;
	}

	//获得图像参数
	int width = image.GetWidth();//获得以像素为单位的图像宽度
	int height = image.GetHeight();//获得以像素为单位的图像高度
	int depth = image.GetBPP();//获得以bit为单位的图像深度（每个像素的位数）

	if (depth != 8)//目前系统仅支持8位位图，其他类型位图暂不支持
	{
		return -2;
	}

	//初始化结果数组
	memset(data, 0, width*height);

	//读取图像数据
	int pitch = image.GetPitch();//每行行首像素的地址偏移量
	unsigned char* pData1 = (unsigned char*)image.GetBits();//图像位图数据区地址
	unsigned char* pData2 = data;
	unsigned char gray = 0;

	unsigned char *pRow1, *pRow2, *pPix1, *pPix2;
	for (int y = 0; y < height; y++)//逐行扫描
	{
		pRow1 = pData1 + pitch * y;
		pRow2 = pData2 + width * y;
		for (int x = 0; x < width; x++)//逐列扫描
		{
			//获得源图像素灰度值
			pPix1 = pRow1 + x;
			gray = *pPix1;

			//保存到结果数组
			pPix2 = pRow2 + x;
			*pPix2 = gray;
		}//end for(x)
	}//end for(y)
	//在遍历图像像素时,使用的是从图像位图数据区中根据地址偏移量直接读取像素值的方法

	//释放变量空间
	image.Destroy();

	return 0;
}


//保存位图数据到文本文件
//dstFile(目标文件)，data(无符号字符型数据数组)，width(图像宽度)，height(图像高度)
int SaveDataToTextFile(char *dstFile, unsigned char *data, int width, int height)
{
	//打开目标文件
	ofstream fout(dstFile, ios::out);//使用覆盖写入方法
	if (!fout)//or if(fout.fail())
	{
		return -1;
	}

	//按指定格式向文件写入数据
	int space = 5;//每个无符号字符型数据均用5个字符位等宽存储
	for (int i = 0; i < height*width; i++)//遍历数组
	{
		fout << setw(space) << int(data[i]);//等宽写入数据
		if (i%width == (width - 1))//行尾
		{
			fout << endl;//换行
		}
	}

	//关闭文件
	fout.close();

	return 0;
}

//dstFile(目标文件)，data(浮点型数据数组)，width(图像宽度)，height(图像高度)
int SaveDataToTextFile(char *dstFile, float *data, int width, int height)
{
	//打开目标文件
	ofstream fout(dstFile, ios::out);//使用覆盖写入方法
	if (!fout)//or if(fout.fail())
	{
		return -1;
	}

	//按指定格式向文件写入数据
	int preci = 6;//每个浮点型数据均保留6个小数位
	int space = 16;//每个浮点型数据均用16个字符位等宽存储
	fout.precision(preci);
	for (int i = 0; i < height*width; i++)//遍历数组
	{
		fout << " " << setw(space) << data[i];//等宽写入数据
		if (i%width == (width - 1))//行尾
		{
			fout << endl;//换行
		}
	}

	//关闭文件
	fout.close();

	return 0;
}


//载入图像单步测试
//info（返回操作结果状态，成功时返回图像参数信息，失败时返回错误提示信息）
int Step1_LoadBmpImage(char *info)
{
	CString strFile;
	char filename[MAX_PATH] = { 0 };
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"), NULL);
	
	if (dlgFile.DoModal() == IDOK)//如果用户在文件对话框中点击了“确定”按钮
	{
		strFile = dlgFile.GetPathName();

		int num = WideCharToMultiByte(0, 0, strFile, -1, NULL, 0, NULL, false);
		char* chStr = new char[num];
		WideCharToMultiByte(0, 0, strFile, -1, chStr, num, NULL, false);
		strcpy(filename, chStr);
	}

	//复制图像
	CopyFile(ToWideChar(filename), ToWideChar(STEP_IMG_1), false);

	//读取图像参数并检测有效性
	int iWidth, iHeight, iDepth;
	int flag = ReadBMPImgFilePara(filename, iWidth, iHeight, iDepth);
	if (flag != 0)
	{
		sprintf(info, "图像加载失败.");
		return -1;
	}

	//读取图像数据
	unsigned char *data = new unsigned char[iWidth*iHeight];
	flag = ReadBMPImgFileData(filename, data);
	if (flag != 0)
	{
		sprintf(info, "图像数据读取失败.");
		delete[] data;//释放内存
		return -2;
	}

	//保存数据（TXT文件）
	flag = SaveDataToTextFile(STEP_TXT_1, data, iWidth, iHeight);
	if (flag != 0)
	{
		sprintf(info, "数据保存失败.");
		delete[] data;//释放内存
		return -3;
	}

	//返回图像参数信息//sprintf把字符串的内容赋给info
	sprintf(info, "源图[%s],宽度[%d],高度[%d],深度[%d b]", filename, iWidth, iHeight, iDepth);

	//释放内存
	delete[] data;

	return 0;
}


void InitDatabase()//创建并初始化指纹库
{
	_mkdir(TEMP_DIR);//创建临时文件夹
	_mkdir(DB_DIR);//创建指纹库文件夹
	InitFile(DB_INDEX_TXT);//创建索引文件
}

void InitFile(char *filename)//创建并初始化文件
{
	FILE *index = fopen(filename, "w");//若文件不存在则创建，若已存在则清空其内容
	fclose(index);//关闭文件
}


//参数：info(返回指纹库统计信息，用于系统界面显示)
void GetDatabaseInfo(char *info)//获取指纹库统计信息
{
	//遍历指纹库
	int pNo = 0;//指纹序号
	char name[MAX_PATH] = { 0 };//登记人姓名
	char srcFile[MAX_PATH] = { 0 };//指纹图像文件
	char mdlFile[MAX_PATH] = { 0 };//指纹特征文件
	FILE *index = fopen(DB_INDEX_TXT, "r");//打开索引文件
	while (!feof(index))
	{
		//fscanf把字符串中的值赋给对应的变量
		fscanf(index, "%d %s %s %s\n", &pNo, srcFile, mdlFile, name);
	}
	fclose(index);//关闭文件

	//统计指纹库信息//sprintf把字符串的内容赋给info
	sprintf(info, "当前指纹库中共有%d条记录.", pNo);
}


//从文本文件读取无符号字符类型的图像位图数据
//srcFile(源文件),data(无符号字符型数据数组),iWidth(图像宽度),iHeight(图像高度)
int ReadDataFromTextFile(char *srcFile, unsigned char *data, int iWidth, int iHeight)
{
	//打开源文件
	ifstream fin(srcFile, ios::in);
	if (!fin)//or if(fin.fail())
	{
		return -1;
	}

	//读取数据
	int d = 0;
	for (int i = 0; i < iHeight*iWidth; i++)
	{
		fin >> d;
		data[i] = (unsigned char)d;
	}

	//关闭文件
	fin.close();

	return 0;
}

//从文本文件读取浮点数类型的其他数据
//srcFile(源文件),data(浮点型数据数组),iWidth(图像宽度),iHeight(图像高度)
int ReadDataFromTextFile(char *srcFile, float *data, int iWidth, int iHeight)
{
	//打开源文件
	ifstream fin(srcFile, ios::in);
	if (!fin)//or if(fin.fail())
	{
		return -1;
	}

	//读取数据
	for (int i = 0; i < iHeight*iWidth; i++)
	{
		fin >> data[i];
	}

	//关闭文件
	fin.close();

	return 0;
}


//使用冒泡法对数组进行升序排序
//参数:data(数组)，dsize(数组长度)
void Sort(unsigned char *data, int dsize)
{
	unsigned char temp = 0;
	for (int i = 0; i < dsize; i++)
	{
		for (int j = dsize - 1; j > i; j--)
		{
			if (data[j] < data[j - 1])//升序排序
			{
				temp = data[j];
				data[j] = data[j - 1];
				data[j - 1] = temp;
			}
		}
	}
}


//参数：ucImg(源图数据)，ucDstImg(结果图像数据),iWidth(图像宽度),iHeight(图像高度)
int MidFilter(unsigned char *ucImg, unsigned char *ucDstImg, int iWidth, int iHeight)
{
	//Step1:结果图像数据初始化
	memset(ucDstImg, 0, iWidth*iHeight);

	//Step2:中心区域滤波(使用3*3邻域)
	unsigned char *pUp, *pDown, *pImg;//用来确定3*3邻域的3个图像数据指针(下文简称"邻域指针")
	unsigned char x[9];//3*3邻域图像数据数组
	for (int i = 1; i < iHeight - 1; i++)//遍历第2行到倒数第2行
	{
		//初始化邻域指针
		pUp = ucImg + (i - 1)*iWidth;
		pImg = ucImg + i * iWidth;
		pDown = ucImg + (i + 1)*iWidth;

		for (int j = 1; j < iWidth - 1; j++)//遍历第2列到倒数第2列
		{
			//移动邻域指针
			pUp++;
			pImg++;
			pDown++;

			//获取3*3邻域数据
			x[0] = *(pUp - 1);
			x[1] = *(pImg - 1);
			x[2] = *(pDown - 1);

			x[3] = *pUp;
			x[4] = *pImg;
			x[5] = *pDown;

			x[6] = *(pUp + 1);
			x[7] = *(pImg + 1);
			x[8] = *(pDown + 1);

			//数组排序
			Sort(x, 9);

			//结果图像数据取邻域中值
			*(ucDstImg + i * iWidth + j) = x[4];
		}
	}

	//Step3:第1行和最后1行滤波(使用2*3邻域)

	//第1行
	pDown = ucImg + iWidth;//邻域指针初始化
	for (int j = 1; j < iWidth - 1; j++)//第1行遍历第2列到倒数第2列
	{
		//获取2*3邻域数据
		x[0] = *(ucImg + j - 1);
		x[1] = *(ucImg + j);
		x[2] = *(ucImg + j + 1);

		x[3] = *(pDown + j - 1);
		x[4] = *(pDown + j);
		x[5] = *(pDown + j + 1);

		//数组排序
		Sort(x,6);

		//结果取中值
		*(ucDstImg + j) = x[3];
	}

	//最后1行(倒数第1行)
	pUp = ucImg + iWidth * (iHeight - 2);//邻域指针初始化
	pDown = ucImg + iWidth * (iHeight - 1);//邻域指针初始化
	for (int j = 1; j < iWidth - 1; j++)//最后1行遍历第2列到倒数第2列
	{
		//获取2*3邻域数据
		x[0] = *(pDown + j - 1);
		x[1] = *(pDown + j);
		x[2] = *(pDown + j + 1);

		x[3] = *(pUp + j - 1);
		x[4] = *(pUp + j);
		x[5] = *(pUp + j + 1);

		//数组排序
		Sort(x,6);

		//结果取中值
		*(ucDstImg + iWidth * (iHeight - 1) + j) = x[3];
	}

	//Step4:4个角点滤波(使用2*2邻域)

	//左上角点
	x[0] = *(ucImg);//获取2*2邻域数据
	x[1] = *(ucImg+1);
	x[2] = *(ucImg+iWidth);
	x[3] = *(ucImg + iWidth + 1);
	Sort(x, 4);//数组排序
	*(ucDstImg) = x[2];//结果取中值

	//右上角点
	x[0] = *(ucImg + iWidth - 1);//获取2*2邻域数据
	x[1] = *(ucImg + iWidth - 2);
	x[2] = *(ucImg + 2 * iWidth - 1);
	x[3] = *(ucImg + 2 * iWidth - 2);
	Sort(x, 4);//数组排序
	*(ucDstImg + iWidth - 1) = x[2];//结果取中值

	//左下角点
	x[0] = *(ucImg + (iHeight - 1)*iWidth);//获取2*2邻域数据
	x[1] = *(ucImg + (iHeight - 2)*iWidth);
	x[2] = *(ucImg + (iHeight - 1)*iWidth + 1);
	x[3] = *(ucImg + (iHeight - 2)*iWidth + 1);
	Sort(x, 4);//数组排序
	*(ucDstImg + (iHeight - 1)*iWidth) = x[2];//结果取中值

	//右下角点
	x[0] = *(ucImg + (iHeight - 0)*iWidth - 1);//获取2*2邻域数据
	x[1] = *(ucImg + (iHeight - 1)*iWidth - 1);
	x[2] = *(ucImg + (iHeight - 0)*iWidth - 2);
	x[3] = *(ucImg + (iHeight - 1)*iWidth - 2);
	Sort(x, 4);//数组排序
	*(ucDstImg + (iHeight - 0)*iWidth - 1) = x[2];//结果取中值

	return 0;
}


//将位图数据写入BMP图像文件
//dstFileName(目标文件名),pusImgData(待保存数据数组)
int WriteBMPImgFile(char *dstFileName, unsigned char **pusImgData)
{
	//打开文件
	FILE *fp = fopen(dstFileName, "r+b");
	if (!fp)
	{
		return -1;
	}

	//读取图像参数信息(用于定位数据区以写入数据,见参考资料中"BMP图像文件组成")
	int imgType, iWidth, iHeight;//图像深度/宽度/高度
	int iStartPos = 0;//位图数据区起始地址
	fseek(fp, 10L, SEEK_SET);
	fread((char*)(&iStartPos), 4, 1, fp);//获取位图数据区起始地址(以字节为单位)
	fseek(fp, 18L, SEEK_SET);
	fread((char*)(&iWidth), 4, 1, fp);//获取图像宽度(以像素为单位)
	fread((char*)(&iHeight), 4, 1, fp);//获取图像高度(以像素为单位)
	unsigned short temp;
	fseek(fp, 28L, SEEK_SET);
	fread((char*)(&temp), 2, 1, fp);//获取图像深度(每个像素的位数,以位为单位)
	imgType = temp;
	if (imgType != 8)//目前仅支持8位BMP位图图像
	{
		return -2;
	}

	//向数据区写入数据
	unsigned char *usImgData = *pusImgData;//待保存数据数组地址
	int iWidthInFile = 0;//文件中每行像素宽度/长度(以字节为单位)
	if (iWidth % 4 > 0)
	{
		iWidthInFile = iWidth - iWidth % 4 + 4;
	}
	else
	{
		iWidthInFile = iWidth;
	}
	for (int i = iHeight - 1; i >= 0; i--)//从最后一行到第一行倒序存储
	{
		fseek(fp, iStartPos, SEEK_SET);//定位行首地址
		fwrite((usImgData + i * iWidth), 1, iWidth, fp);//写入一行数据
		iStartPos += iWidthInFile;//调整行首地址
	}

	//关闭文件
	fclose(fp);

	return 0;
}


//保存数据到BMP图像文件
//srcFile(源文件名),dstFile(目标文件名),data(待保存数据数组)
int SaveDataToImageFile(char *srcFile, char *dstFile, unsigned char *data)
{
	//文件复制
	CopyFile(ToWideChar(srcFile), ToWideChar(dstFile), false);

	//写入数据
	WriteBMPImgFile(dstFile, &data);

	return 0;
}

//保存数据到图像文件
//srcFile(源文件名)，dstFile(目标文件名),data(待保存数据数组),scale(转换比例)
int SaveDataToImageFile(char *srcFile, char *dstFile, float *data, float scale)
{
	//读取图像参数
	int iWidth, iHeight, iDepth;//图像宽度/高度/深度
	ReadBMPImgFilePara(srcFile, iWidth, iHeight, iDepth);

	//文件复制
	CopyFile(ToWideChar(srcFile), ToWideChar(dstFile), false);

	//数据转换
	unsigned char *tmpData = new unsigned char[iWidth*iHeight];
	for (int i = 0; i<int(iWidth*iHeight); i++)
	{
		tmpData[i] = unsigned char((scale*data[i]));
	}

	//写入数据
	WriteBMPImgFile(dstFile, &tmpData);

	//释放内存空间
	delete[] tmpData;

	return 0;
}


//中值滤波单步测试
//参数:info(返回操作成功或失败提示信息)
int Step2_MidFilter(char *info)
{
	//设置输入输出文件名
	char srcTxtFile[MAX_PATH] = { STEP_TXT_1 };//源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_1 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_2 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_2 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;//图像宽度/高度/深度
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	//中值滤波
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	MidFilter(image1, image2, iWidth, iHeight);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	SaveDataToImageFile(srcImgFile, dstImgFile, image2);

	//释放内存空间
	delete[] image1;
	delete[] image2;

	//返回操作成败状态
	sprintf(info, "完成中值滤波.");
	return 0;
}


//直方图均衡化
//ucImg(源图数据),ucNormImg(结果图数据),iWidth(图像宽度),iHeight(图像高度)
int HistoNormalize(unsigned char *ucImg, unsigned char *ucNormImg, int iWidth, int iHeight)
{
	//构建源图灰度直方图
	unsigned int Histogram[256];
	memset(Histogram, 0, 256 * sizeof(int));
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			Histogram[ucImg[i*iWidth + j]]++;
		}
	}

	//计算源图的灰度均值和方差
	double dMean = 0;
	for (int i = 1; i < 255; i++)
	{
		dMean += i * Histogram[i];
	}
	dMean = int(dMean / (iWidth*iHeight));
	double dSigma = 0;
	for (int i = 0; i < 255; i++)
	{
		dSigma += Histogram[i] * (i - dMean)*(i - dMean);
	}
	dSigma /= (iWidth*iHeight);
	dSigma = sqrt(dSigma);

	//对各像素进行均衡化操作
	double dMean0 = 128, dSigma0 = 128;//预设灰度均值和方差(此处均设128)
	double dCoeff = dSigma0 / dSigma;//转换系数
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			double dVal = ucImg[i*iWidth + j];
			dVal = dMean0 + dCoeff * (dVal - dMean0);
			if (dVal < 0)
			{
				dVal = 0;
			}
			else if (dVal > 255)
			{
				dVal = 255;
			}
			ucNormImg[i*iWidth + j] = (unsigned char)dVal;
		}
	}

	return 0;
}


//直方图均衡化单步测试
//参数:info(返回操作成功或失败提示信息)
int Step3_Normalize(char *info)
{
	//设置输入输出文件名
	char srcTxtFile[MAX_PATH] = { STEP_TXT_2 };//源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_2 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_3 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_3 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;//图像宽度/高度/深度
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	//直方图均衡化
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	HistoNormalize(image1, image2, iWidth, iHeight);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	SaveDataToImageFile(srcImgFile, dstImgFile, image2);

	//释放空间
	delete[] image1;
	delete[] image2;

	//返回操作成败状态
	sprintf(info, "完成直方图均衡化.");
	return 0;
}

//指纹脊线方向计算
//ucImg(图像数据),fDirc(脊线方向数据),iWidth(图像宽度),iHeight(图像高度)
int ImgDirection(unsigned char *ucImg, float *fDirc, int iWidth, int iHeight)
{
	//定义变量
	const int SEMISIZ = 7;//领域窗口区域半径
	int dx[SEMISIZ * 2 + 1][SEMISIZ * 2 + 1];
	int dy[SEMISIZ * 2 + 1][SEMISIZ * 2 + 1];
	float fx, fy;

	//结果初始化
	memset(fDirc, 0, iWidth*iHeight * sizeof(float));

	//计算每一像素的脊线方向值
	for (int y = SEMISIZ + 1; y < iHeight - SEMISIZ - 1; y++)//逐行遍历(除去边缘)
	{
		for (int x = SEMISIZ + 1; x < iWidth - SEMISIZ - 1; x++)//逐列遍历(除去边缘)
		{
			//计算以当前像素为中心的领域窗口区内每一像素的梯度
			for (int j = 0; j < SEMISIZ * 2 + 1; j++)
			{
				for (int i = 0; i < SEMISIZ * 2 + 1; i++)
				{
					int index1 = (y + j - SEMISIZ)*iWidth + x + i - SEMISIZ;
					int index2 = (y + j - SEMISIZ)*iWidth + x + i - SEMISIZ - 1;
					int index3 = (y + j - SEMISIZ - 1)*iWidth + x + i - SEMISIZ;
					dx[i][j] = int(ucImg[index1] - ucImg[index2]);
					dy[i][j] = int(ucImg[index1] - ucImg[index3]);
				}
			}

			//计算当前像素的脊线方向值
			fx = 0.0;
			fy = 0.0;
			for (int j = 0; j < SEMISIZ * 2 + 1; j++)
			{
				for (int i = 0; i < SEMISIZ * 2 + 1; i++)
				{
					fx += 2 * dx[i][j] * dy[i][j];
					fy += (dx[i][j] * dx[i][j] - dy[i][j] * dy[i][j]);
				}
			}
			fDirc[y*iWidth + x] = atan2(fx, fy);
		}
	}

	return 0;
}


//脊线方向低通滤波
//fDirc(脊线方向原始数据),fFitDirc(脊线方向滤波结果数据),iWidth(图像宽度),iHeight(图像高度)
int DircLowPass(float *fDirc, float *fFitDirc, int iWidth, int iHeight)
{
	//定义变量
	const int DIR_FILTER_SIZE = 2;
	int blocksize = 2 * DIR_FILTER_SIZE + 1;
	int imgsize = iWidth * iHeight;

	float* filter = new float[blocksize*blocksize];//使用5*5滤波器
	float* phix = new float[imgsize];
	float* phiy = new float[imgsize];
	float* phi2x = new float[imgsize];
	float* phi2y = new float[imgsize];

	//结果初始化
	memset(fFitDirc, 0, sizeof(float)*iWidth*iHeight);

	//设置5*5高斯低通滤波器模板
	float tempSum = 0.0;
	for (int y = 0; y < blocksize; y++)
	{
		for (int x = 0; x < blocksize; x++)
		{
			filter[y*blocksize + x] = (float)(blocksize - (abs(DIR_FILTER_SIZE - x) + abs(DIR_FILTER_SIZE - y)));
			tempSum += filter[y*blocksize + x];
		}
	}

	for (int y = 0; y < blocksize; y++)
	{
		for (int x = 0; x < blocksize; x++)
		{
			filter[y*blocksize + x] /= tempSum;
		}
	}

	//计算各像素点的方向正弦值和余弦值
	for (int y = 0; y < iHeight; y++)
	{
		for (int x = 0; x < iWidth; x++)
		{
			phix[y*iWidth + x] = cos(fDirc[y*iWidth + x]);
			phiy[y*iWidth + x] = sin(fDirc[y*iWidth + x]);
		}
	}

	//对所有像素进行方向低通滤波
	memset(phi2x, 0, sizeof(float)*imgsize);
	memset(phi2y, 0, sizeof(float)*imgsize);
	float nx, ny;
	int val;
	for (int y = 0; y <= iHeight - blocksize; y++)//逐行遍历(除去边缘区段)
	{
		for (int x = 0; x <= iWidth - blocksize; x++)//逐列遍历(除去边缘区段)
		{
			//对以当前像素为中心的滤波窗口内的所有像素值进行加权累加
			nx = 0.0;
			ny = 0.0;
			for (int j = 0; j < blocksize; j++)
			{
				for (int i = 0; i < blocksize; i++)
				{
					val = (x + i) + (j + y)*iWidth;
					nx += filter[j*blocksize + i] * phix[val];//方向余弦加权累加
					ny += filter[j*blocksize + i] * phiy[val];//方向正弦加权累加
				}
			}
			//将累加结果作为当前像素的新的方向正弦值和余弦值
			val = x + y * iWidth;
			phi2x[val] = nx;
			phi2y[val] = ny;
		}
	}

	//根据加权累加结果,计算各像素的方向滤波结果值
	for (int y = 0; y < iHeight - blocksize; y++)
	{
		for (int x = 0; x < iWidth - blocksize; x++)
		{
			val = x + y * iWidth;
			fFitDirc[val] = atan2(phi2y[val], phi2x[val])*0.5;
		}
	}

	delete[] phi2y;
	delete[] phi2x;
	delete[] phiy;
	delete[] phix;

	return 0;
}


//指纹脊线方向计算单步测试
//参数:info(返回操作成功或失败提示信息)
int Step4_Direction(char *info)
{
	//设置输入输出文件名
	char srcTxtFile[MAX_PATH] = { STEP_TXT_3 };//源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_3 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_4 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_4 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;//图像宽度/高度/深度
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	//方向计算
	float *tmpDirections = new float[iWidth*iHeight];
	ImgDirection(image1, tmpDirections, iWidth, iHeight);

	//方向低通滤波
	float *directions = new float[iWidth*iHeight];
	DircLowPass(tmpDirections, directions, iWidth, iHeight);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, directions, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	const int DIRECTION_SCALE = 100;//方向结果转换比例(仅用于结果显示)
	SaveDataToImageFile(srcImgFile, dstImgFile, directions, DIRECTION_SCALE);

	//释放空间
	delete[] image1;
	delete[] tmpDirections;
	delete[] directions;

	//返回操作成败状态
	sprintf(info, "完成方向计算.");
	return 0;
}

//指纹脊线频率计算
//ucImg(源图数据),fDirection(脊线方向数据),fFrequency(脊线频率结果数据),iWidth(图像宽度),iHeight(图像高度)
int Frequency(unsigned char * ucImg, float * fDirection, float * fFrequency, int iWidth, int iHeight)
{
	if (iWidth <= 0 || iHeight <= 0 || !ucImg || !fDirection || !fFrequency)
	{
		return -1;
	}
	//窗口大小
	const int SIZE_L = 32;
	const int SIZE_W = 16;
	const int SIZE_L2 = 16;
	const int SIZE_W2 = 8;

	//正弦波峰值点
	int peak_pos[SIZE_L];
	int peak_cnt;
	float peak_freq;
	float Xsig[SIZE_L];

	//方向
	float dir = 0.0;
	float cosdir = 0.0;
	float sindir = 0.0;
	float maxPeak, minPeak;

	//结果初始化
	float *frequency1 = new float[iWidth*iHeight];
	memset(fFrequency, 0, sizeof(float)*iWidth*iHeight);
	memset(frequency1, 0, sizeof(float)*iWidth*iHeight);

	int x, y;
	int d, k;
	int u, v;

	for (y = SIZE_L2; y < iHeight - SIZE_L2; y++)//逐行遍历(除去边缘区段)
	{
		for (x = SIZE_L2; x < iWidth - SIZE_L2; x++)//逐列遍历(除去边缘区段)
		{
			//当前像素的脊线方向
			dir = fDirection[(y + SIZE_W2)*iWidth + (x + SIZE_W2)];
			cosdir = -sin(dir);
			sindir = cos(dir);

			//计算以当前像素为中心的L*W邻域窗口的幅值序列,X[0]...X[L-1]
			for (k = 0; k < SIZE_L; k++)
			{
				Xsig[k] = 0.0;
				for (d = 0; d < SIZE_W; d++)
				{
					u = (int)(x + (d - SIZE_W2)*cosdir + (k - SIZE_L2)*sindir);
					v = (int)(y + (d - SIZE_W2)*sindir - (k - SIZE_L2)*cosdir);
					//边界点处理
					if (u < 0)
					{
						u = 0;
					}
					else if (u > iWidth - 1)
					{
						u = iWidth - 1;
					}
					if (v < 0)
					{
						v = 0;
					}
					else if (v > iHeight - 1)
					{
						v = iHeight - 1;
					}

					Xsig[k] += ucImg[u + v * iWidth];
				}
				Xsig[k] /= SIZE_W;
			}

			//确定幅值序列变化范围
			maxPeak = minPeak = Xsig[0];
			for (k = 0; k < SIZE_L; k++)
			{
				if (minPeak > Xsig[k])
				{
					minPeak = Xsig[k];
				}
				if (maxPeak < Xsig[k])
				{
					maxPeak = Xsig[k];
				}
			}

			//确定峰值点位置
			peak_cnt = 0;
			if ((maxPeak - minPeak) > 64)
			{
				for (k = 0; k < SIZE_L; k++)
				{
					if ((Xsig[k - 1] < Xsig[k]) && (Xsig[k] >= Xsig[k + 1]))
					{
						peak_pos[peak_cnt++] = k;
					}
				}
			}

			//计算峰值点平均间距
			peak_freq = 0.0;
			if (peak_cnt >= 2)
			{
				for (k = 0; k < peak_cnt - 1; k++)
				{
					peak_freq += (peak_pos[k + 1] - peak_pos[k]);
				}
				peak_freq /= peak_cnt - 1;
			}
			//计算当前像素的频率
			if (peak_freq<3.0 || peak_freq>25.0)
			{
				frequency1[x + y * iWidth] = 0.0;
			}
			else
			{
				frequency1[x + y * iWidth] = 1.0 / peak_freq;
			}
		}
	}

	//对频率进行均值滤波
	for (y = SIZE_L2; y < iHeight - SIZE_L2; y++)//逐行遍历(除去边缘区段)
	{
		for (x = SIZE_L2; x < iWidth - SIZE_L2; x++)//逐列遍历(除去边缘区段)
		{
			k = x + y * iWidth;//当前像素位置(在频率数组中的数组下标)
			peak_freq = 0.0;
			//使用以当前像素为中心的5*5邻域窗口进行均值滤波
			for (v = -2; v <= 2; v++)
			{
				for (u = -2; u <= 2; u++)
				{
					peak_freq += frequency1[(x + u) + (y + v)*iWidth];//求频率累加和
				}
			}
			fFrequency[k] = peak_freq / 25;//当前像素频率等于窗口内频率均值
		}
	}

	delete[] frequency1;

	return 0;
}


//指纹脊线频率计算单步测试
//参数:info(返回操作成功或失败提示信息)
int Step5_Frequency(char *info)
{
	//设置输入输出文件名
	char srcTxtFile_Img[MAX_PATH] = { STEP_TXT_3 };//图像源数据文件名
	char srcTxtFile_Dir[MAX_PATH] = { STEP_TXT_4 };//方向源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_4 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_5 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_5 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;
	int flag = ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取图像源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Img, image1, iWidth, iHeight);

	//读取方向源数据
	float *direction = new float[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Dir, direction, iWidth, iHeight);

	//频率计算
	float *frequency = new float[iWidth*iHeight];
	Frequency(image1, direction, frequency, iWidth, iHeight);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, frequency, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	const int FREQUENCY_SCALE = 1000;//频率结果转换比例(仅用于结果显示)
	SaveDataToImageFile(srcImgFile, dstImgFile, frequency, FREQUENCY_SCALE);

	//释放内存空间
	delete[] image1;
	delete[] direction;
	delete[] frequency;

	//返回操作成败状态
	sprintf(info, "完成频率计算.");
	return 0;
}

//指纹掩码计算
//ucImg(源图数据),fDirection(脊线方向数据),fFrequency(脊线频率数据)
//ucMask(掩码结果数据),iWidth(图像宽度),iHeight(图像高度)
int GetMask(unsigned char * ucImg, float * fDirection, float * fFrequency, unsigned char * ucMask, int iWidth, int iHeight)
{
	if (!ucImg || !fDirection || !fFrequency || !ucMask || iWidth <= 0 || iHeight <= 0)
	{
		return -1;
	}
	//第1步:阈值分割(像素频率位于指定范围之内则设为前景点,否则设为背景点)
	float freqMin = 1.0 / 25.0;
	float freqMax = 1.0 / 3.0;
	int x, y, k;
	int pos, posout;

	memset(ucMask, 0, iWidth*iHeight);//结果初始化(全为背景点)

	for (y = 0; y < iHeight; y++)//逐行遍历
	{
		for (x = 0; x < iWidth; x++)//逐列遍历
		{
			pos = x + y * iWidth;
			posout = x + y * iWidth;
			ucMask[posout] = 0;
			if (fFrequency[pos] >= freqMin && fFrequency[pos] <= freqMax)
			{
				ucMask[posout] = 255;//频率位于指定范围之内则设为前景点
			}
		}
	}

	//第2步:填充孔洞(利用形态学的膨胀原理)
	for (k = 0; k < 4; k++)//重复膨胀多次(次数自定)
	{
		//标记前景点
		for (y = 1; y < iHeight - 1; y++)//逐行遍历
		{
			for (x = 1; x < iWidth - 1; x++)//逐列遍历
			{
				//前景点的上下左右四个相邻点都标记为前景点(将8位像素值的最高位设为1予以标记)
				if (ucMask[x + y * iWidth] == 0xFF)//前景点
				{
					ucMask[x - 1 + y * iWidth] |= 0x80;
					ucMask[x + 1 + y * iWidth] |= 0x80;
					ucMask[x + (y - 1) * iWidth] |= 0x80;
					ucMask[x + (y + 1) * iWidth] |= 0x80;
				}
			}
		}
		//判断和设置前景点
		for (y = 1; y < iHeight - 1; y++)//逐行遍历
		{
			for (x = 1; x < iWidth - 1; x++)//逐列遍历
			{
				//将标记为前景点的像素都设为前景点
				if (ucMask[x + y * iWidth])
				{
					ucMask[x + y * iWidth] = 0xFF;//设为前景点(像素值为255)
				}
			}
		}
	}

	//第3步:去除边缘和孤立点(利用形态学的腐蚀原理)
	for (k = 0; k < 12; k++)//重复腐蚀多次(次数自定)
	{
		//标记背景点
		for (y = 1; y < iHeight - 1; y++)//逐行遍历
		{
			for (x = 1; x < iWidth - 1; x++)//逐列遍历
			{
				//背景点的上下左右四个相邻点都标记为潜在背景点(将8位像素值的后7位都设为0予以标记)
				if (ucMask[x + y * iWidth] == 0x0)//背景点
				{
					ucMask[x - 1 + y * iWidth] &= 0x80;
					ucMask[x + 1 + y * iWidth] &= 0x80;
					ucMask[x + (y - 1) * iWidth] &= 0x80;
					ucMask[x + (y + 1) * iWidth] &= 0x80;
				}
			}
		}
		//判断和设置背景点
		for (y = 1; y < iHeight - 1; y++)//逐行遍历
		{
			for (x = 1; x < iWidth - 1; x++)//逐列遍历
			{
				//只要不是前景点的所有像素都设为背景点
				if (ucMask[x + y * iWidth] != 0xFF)//非前景点
				{
					ucMask[x + y * iWidth] = 0x0;//设为背景点(像素值为0)
				}
			}
		}
	}

	return 0;
}


//指纹掩码计算单步测试
//参数:info(返回操作成功或失败提示信息)
int Step6_Mask(char *info)
{
	//设置输入输出文件名
	char srcTxtFile_Img[MAX_PATH] = { STEP_TXT_3 };//图像源数据文件名
	char srcTxtFile_Dir[MAX_PATH] = { STEP_TXT_4 };//方向源数据文件名
	char srcTxtFile_Fre[MAX_PATH] = { STEP_TXT_5 };//频率源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_5 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_6 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_6 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取图像源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Img, image1, iWidth, iHeight);

	//读取方向源数据
	float *direction = new float[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Dir, direction, iWidth, iHeight);

	//读取频率源数据
	float *frequency = new float[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Fre, frequency, iWidth, iHeight);

	//掩码计算
	unsigned char *mask = new unsigned char[iWidth*iHeight];
	GetMask(image1, direction, frequency, mask, iWidth, iHeight);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, mask, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	SaveDataToImageFile(srcImgFile, dstImgFile, mask);

	//释放空间
	delete[] image1;
	delete[] mask;
	delete[] direction;
	delete[] frequency;

	//返回操作成败状态
	sprintf(info, "完成掩码计算.");
	return 0;
}

//Gabor滤波增强
//ucImg(源图数据),fDirection(脊线方向数据),fFrequency(脊线频率数据),ucMask(掩码数据),ucImgEnhanced(滤波增强结果数据),iWidth(图像宽度),iHeight(图像高度)
int GaborEnhance(unsigned char * ucImg, float * fDirection, float * fFrequency, unsigned char * ucMask, unsigned char * ucImgEnhanced, int iWidth, int iHeight)
{
	//定义变量
	const float PI = 3.141592654;
	int i, j, u, v;
	int wg2 = 5;//使用11*11的Gabor滤波器,半边长为5
	float sum, f, g;
	float x2, y2;
	float dx2 = 1.0 / (4.0*4.0);//沿x轴方向的高斯包络常量
	float dy2 = 1.0 / (4.0*4.0);//沿y轴方向的高斯包络常量

	//结果初始化
	memset(ucImgEnhanced, 0, iWidth*iHeight);

	//Gabor滤波
	for (j = wg2; j < iHeight - wg2; j++)//逐行遍历(除去边缘区段)
	{
		for (i = wg2; i < iWidth - wg2; i++)//逐列遍历(除去边缘区段)
		{
			//跳过背景点
			if (ucMask[i + j * iWidth] == 0)//掩码为0表示背景点
			{
				continue;
			}

			//获取当前像素的方向和频率
			g = fDirection[i + j * iWidth];//当前像素的脊线方向
			f = fFrequency[i + j * iWidth];//当前像素的脊线频率
			g += PI / 2;//垂直方向

			//对当前像素进行Gabor滤波
			sum = 0.0;
			for (v = -wg2; v <= wg2; v++)//滤波窗口逐行遍历
			{
				for (u = -wg2; u <= wg2; u++)//滤波窗口逐列遍历
				{
					x2 = -u * sin(g) + v * cos(g);//x坐标旋转
					y2 = u * cos(g) + v * sin(g);//y坐标旋转
					sum += exp(-0.5*(x2*x2*dx2 + y2 * y2*dy2))*cos(2 * PI*x2*f)*ucImg[(i - u) + (j - v)*iWidth];//窗口内滤波值累加
				}
			}
			//边界值处理
			if (sum > 255.0)
			{
				sum = 255.0;
			}
			if (sum < 0.0)
			{
				sum = 0.0;
			}

			//得到当前像素的滤波结果
			ucImgEnhanced[i + j * iWidth] = (unsigned char)sum;
		}
	}
	return 0;
}


//Gabor滤波增强单步测试
//参数:info(返回操作成功或失败提示信息)
int Step7_GaborEnhance(char *info)
{
	//设置输入输出文件名
	char srcTxtFile_Img[MAX_PATH] = { STEP_TXT_3 };//图像源数据文件名
	char srcTxtFile_Dir[MAX_PATH] = { STEP_TXT_4 };//方向源数据文件名
	char srcTxtFile_Fre[MAX_PATH] = { STEP_TXT_5 };//频率源数据文件名
	char srcTxtFile_Msk[MAX_PATH] = { STEP_TXT_6 };//掩码源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_6 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_7 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_7 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取图像源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Img, image1, iWidth, iHeight);

	//读取方向源数据
	float *direction = new float[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Dir, direction, iWidth, iHeight);

	//读取频率源数据
	float *frequency = new float[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Fre, frequency, iWidth, iHeight);

	//读取掩码源数据
	unsigned char *mask = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Msk, mask, iWidth, iHeight);

	//Gabor滤波增强
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	GaborEnhance(image1, direction, frequency, mask, image2, iWidth, iHeight);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	SaveDataToImageFile(srcImgFile, dstImgFile, image2);

	//释放内存空间
	delete[] image1;
	delete[] direction;
	delete[] frequency;
	delete[] mask;
	delete[] image2;

	//返回操作成败状态
	sprintf(info, "完成Gabor滤波增强.");
	return 0;
}

//图像二值化
//ucImage(源图数据),ucBinImage(结果图数据),iWidth(图像宽度),iHeight(图像高度),uThreshold(二值化灰度阈值)
int BinaryImg(unsigned char * ucImage, unsigned char * ucBinImage, int iWidth, int iHeight, unsigned char uThreshold)
{
	//分别定义指向源图数据和结果图数据的数据指针
	unsigned char *pStart = ucImage, *pEnd = ucImage + iWidth * iHeight;
	unsigned char *pDest = ucBinImage;
	//逐一遍历所有元素
	while (pStart < pEnd)
	{
		*pDest = *pStart > uThreshold ? 1 : 0;//二值化
		pStart++;//源图数据指针后移
		pDest++;//结果图数据指针后移
	}
	return 0;
}


//二值图显示转换([0,1]->[0,255])
//ucImage(源图数据),ucGrayImage(结果图数据),iWidth(图像宽度),iHeight(图像高度)
int BinaryToGray(unsigned char * ucBinImg, unsigned char * ucGrayImg, int iWidth, int iHeight)
{
	//分别定义指向源图数据和结果图数据的数据指针
	unsigned char *pStart = ucBinImg, *pEnd = ucBinImg + iWidth * iHeight;
	unsigned char *pDest = ucGrayImg;

	//逐一遍历所有元素
	while (pStart < pEnd)
	{
		*pDest = *pStart > 0 ? 255 : 0;//取值转换([0,1]->[0,255])
		pStart++;//源图数据指针后移
		pDest++;//结果图数据指针后移
	}

	return 0;
}


//二值化单步测试
//参数:info(返回操作成功或失败提示信息)
int Step8_Binary(char *info)
{
	//设置输入输出文件名
	char srcTxtFile[MAX_PATH] = { STEP_TXT_7 };//源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_7 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_8 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_8 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	//二值化
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	BinaryImg(image1, image2, iWidth, iHeight, 128);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	BinaryToGray(image2, image1, iWidth, iHeight);//二值图显示转换
	SaveDataToImageFile(srcImgFile, dstImgFile, image1);

	//释放内存空间
	delete[] image1;
	delete[] image2;

	//返回操作成败状态
	sprintf(info, "完成二值化.");
	return 0;
}

//图像细化
//ucBinedImg(源图数据),ucThinnedImage(结果图数据),iWidth(图像宽度),iHeight(图像高度),iIterativeLimit(最大迭代次数)
int Thinning(unsigned char * ucBinedImg, unsigned char * ucThinnedImage, int iWidth, int iHeight, int iIterativeLimit)
{
	if (iWidth <= 0 || iHeight <= 0 || iIterativeLimit <= 0 || !ucBinedImg || !ucThinnedImage)
	{
		return -1;
	}
	//定义变量
	unsigned char x1, x2, x3, x4, x5, x6, x7, x8, xp;
	unsigned char g1, g2, g3, g4;
	unsigned char b1, b2, b3, b4;
	unsigned char np1, np2, npm;
	unsigned char *pUp, *pDown, *pImg;
	int iDeletedPoints = 0;

	//结果初始化
	memcpy(ucThinnedImage, ucBinedImg, iWidth*iHeight);

	for (int it = 0; it < iIterativeLimit; it++)//重复执行,一直到无点可删为止
	{
		iDeletedPoints = 0;//初始化本次迭代所删点数

		//本次迭代内第1次遍历(使用条件G1&G2&G3)
		for (int i = 1; i < iHeight - 1; i++)//逐行遍历
		{
			//初始化邻域指针
			pUp = ucBinedImg + (i - 1)*iWidth;
			pImg = ucBinedImg + i * iWidth;
			pDown = ucBinedImg + (i + 1)*iWidth;

			for (int j = 1; j < iWidth - 1; j++)//逐列遍历
			{
				//调整邻域指针
				pUp++;
				pImg++;//当前像素(窗口中心像素)
				pDown++;

				//跳过背景点(背景点像素值为0)
				if (!*pImg)
				{
					continue;
				}

				//获取3*3邻域窗口内所有9个像素的灰度值
				x6 = *(pUp - 1);
				x5 = *(pImg - 1);
				x4 = *(pDown - 1);

				x7 = *pUp;
				xp = *pImg;
				x3 = *pDown;

				x8 = *(pUp + 1);
				x1 = *(pImg + 1);
				x2 = *(pDown + 1);

				//判断条件G1
				b1 = !x1 && (x2 == 1 || x3 == 1);
				b2 = !x3 && (x4 == 1 || x5 == 1);
				b3 = !x5 && (x6 == 1 || x7 == 1);
				b4 = !x7 && (x8 == 1 || x1 == 1);

				g1 = (b1 + b2 + b3 + b4) == 1;

				//判断条件G2
				np1 = x1 || x2;
				np1 += x3 || x4;
				np1 += x5 || x6;
				np1 += x7 || x8;
				np2 = x2 || x3;
				np2 += x4 || x5;
				np2 += x6 || x7;
				np2 += x8 || x1;

				npm = np1 > np2 ? np2 : np1;
				g2 = npm >= 2 && npm <= 3;

				//判断条件G3和G4
				g3 = (x1 && (x2 || x3 || !x8)) == 0;
				g4 = (x5 && (x6 || x7 || !x4)) == 0;

				//组合判断(使用条件G1&G2&G3)
				if (g1&&g2&&g3)
				{
					ucThinnedImage[iWidth*i + j] = 0;//删掉当前元素(置为背景点)
					++iDeletedPoints;//本次迭代所删点数加1
				}
			}//end for(j) 列
		}//end for(i) 行

		//结果同步
		memcpy(ucBinedImg, ucThinnedImage, iWidth*iHeight);

		//本次迭代内第2次遍历(使用条件G1&G2&G4)
		for (int i = 1; i < iHeight - 1; i++)//逐行遍历
		{
			//初始化邻域指针
			pUp = ucBinedImg + (i - 1)*iWidth;
			pImg = ucBinedImg + i * iWidth;
			pDown = ucBinedImg + (i + 1)*iWidth;

			for (int j = 1; j < iWidth - 1; j++)//逐列遍历
			{
				//调整邻域指针
				pUp++;
				pImg++;//当前像素(窗口中心像素)
				pDown++;

				//跳过背景点(背景点像素值为0)
				if (!*pImg)
				{
					continue;
				}

				//获取3*3邻域窗口内所有9个像素的灰度值
				x6 = *(pUp - 1);
				x5 = *(pImg - 1);
				x4 = *(pDown - 1);

				x7 = *pUp;
				xp = *pImg;
				x3 = *pDown;

				x8 = *(pUp + 1);
				x1 = *(pImg + 1);
				x2 = *(pDown + 1);

				//判断条件G1
				b1 = !x1 && (x2 == 1 || x3 == 1);
				b2 = !x3 && (x4 == 1 || x5 == 1);
				b3 = !x5 && (x6 == 1 || x7 == 1);
				b4 = !x7 && (x8 == 1 || x1 == 1);

				g1 = (b1 + b2 + b3 + b4) == 1;

				//判断条件G2
				np1 = x1 || x2;
				np1 += x3 || x4;
				np1 += x5 || x6;
				np1 += x7 || x8;
				np2 = x2 || x3;
				np2 += x4 || x5;
				np2 += x6 || x7;
				np2 += x8 || x1;

				npm = np1 > np2 ? np2 : np1;
				g2 = npm >= 2 && npm <= 3;

				//判断条件G3和G4
				g3 = (x1 && (x2 || x3 || !x8)) == 0;
				g4 = (x5 && (x6 || x7 || !x4)) == 0;

				//组合判断(使用条件G1&G2&G4)
				if (g1&&g2&&g4)
				{
					ucThinnedImage[iWidth*i + j] = 0;//删掉当前元素(置为背景点)
					++iDeletedPoints;//本次迭代所删点数加1
				}
			}//end for(j) 列
		}//end for(i) 行

		//结果同步
		memcpy(ucBinedImg, ucThinnedImage, iWidth*iHeight);

		//如果本次迭代已无点可删,则停止迭代
		if (iDeletedPoints == 0)
		{
			break;
		}
	}

	//清除边缘区段
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			if (i < 16)//上边缘
			{
				ucThinnedImage[i*iWidth + j] = 0;
			}
			else if (i >= iHeight - 16)//下边缘
			{
				ucThinnedImage[i*iWidth + j] = 0;
			}
			else if (j < 16)//左边缘
			{
				ucThinnedImage[i*iWidth + j] = 0;
			}
			else if (j >= iWidth - 16)//右边缘
			{
				ucThinnedImage[i*iWidth + j] = 0;
			}
		}
	}

	return 0;
}


//细化单步测试
//参数:info(返回操作成功或失败提示信息)
int Step9_Thinning(char *info)
{
	//设置输入输出文件名
	char srcTxtFile[MAX_PATH] = { STEP_TXT_8 };//源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_8 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_9 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_9 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	//细化
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	Thinning(image1, image2, iWidth, iHeight, 200);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	BinaryToGray(image2, image1, iWidth, iHeight);//二值图显示转换
	SaveDataToImageFile(srcImgFile, dstImgFile, image1);

	//释放内存空间
	delete[] image1;
	delete[] image2;

	//返回操作成败状态
	sprintf(info, "完成细化.");
	return 0;
}

//指纹特征提取
//ucThinImg(源图数据),ucMinuImg(结果图数据),iWidth(图像宽度),iHeight(图像高度)
int Extract(unsigned char * ucThinImg, unsigned char * ucMinuImg, int iWidth, int iHeight)
{
	//定义变量
	unsigned char *pDest = ucMinuImg;//结果图数据指针
	unsigned char *pUp, *pDown, *pImg;//源图邻域指针
	unsigned char x1, x2, x3, x4, x5, x6, x7, x8;//八邻点
	unsigned char nc;//八邻点中黑点数量
	int iMinuCount = 0;//特征点数量

	//结果初始化(全设为0,表示都是非特征点)
	memset(pDest, 0, iWidth*iHeight);
	
	//遍历源图以提取指纹特征
	for (int i = 1; i < iHeight - 1; i++)//逐行遍历
	{
		//初始化邻域指针
		pUp = ucThinImg + (i - 1)*iWidth;
		pImg = ucThinImg + i * iWidth;
		pDown = ucThinImg + (i + 1)*iWidth;

		for (int j = 1; j < iWidth - 1; j++)//逐列遍历
		{
			//调整邻域指针
			pUp++;
			pImg++;
			pDown++;

			//跳过背景点(背景点像素值为0)
			if (!*pImg)
			{
				continue;
			}

			//获取3*3邻域窗口内所有8个邻点像素的灰度值
			x6 = *(pUp - 1);
			x5 = *(pImg - 1);
			x4 = *(pDown - 1);

			x7 = *pUp;
			x3 = *pDown;

			x8 = *(pUp + 1);
			x1 = *(pImg + 1);
			x2 = *(pDown + 1);

			//统计八邻点中黑点数量
			nc = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;

			//特征点判断
			if (nc == 1)//端点
			{
				pDest[i*iWidth + j] = 1;//结果图中对应像素点设为1(表示端点)
				++iMinuCount;//特征点数量加1
			}
			else if (nc == 3)//分叉点
			{
				pDest[i*iWidth + j] = 3;//结果图中对应像素点设为3(表示分叉点)
				++iMinuCount;//特征点数量加1
			}
		}
	}
	
	return iMinuCount;//返回特征点数量
}


//特征提取单步测试
//参数:minuCount(返回特征点数量),info(返回操作成功或失败提示信息)
int Step10_MinuExtract(int &minuCount, char *info)
{
	//结果初始化
	minuCount = 0;

	//设置输入输出文件名
	char srcTxtFile[MAX_PATH] = { STEP_TXT_9 };//源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_9 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_10 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_10 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile, image1, iWidth, iHeight);

	//特征提取
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	minuCount = Extract(image1, image2, iWidth, iHeight);

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	BinaryToGray(image2, image1, iWidth, iHeight);//二值图显示转换
	SaveDataToImageFile(srcImgFile, dstImgFile, image1);

	//释放内存空间
	delete[] image1;
	delete[] image2;

	//返回成败状态
	sprintf(info, "完成特征提取.");
	return 0;
}

struct NEIGHBOR
{
	int x;//横坐标(列坐标)
	int y;//纵坐标(行坐标)
	int type;//特征点类型(1-端点,3-分叉点)
	float Theta;//两点连线角度(弧度)
	float Theta2Ridge;//两点脊线方向夹角(弧度)
	float ThetaThisNibor;//相邻特征点的脊线方向(弧度)
	int distance;//两点距离(像素数量)
};

//特征点结构
struct MINUTIAE
{
	int x;//横坐标(列坐标)
	int y;//纵坐标(行坐标)
	int type;//特征点类型(1-端点,3-分叉点)
	float theta;//该点处的脊线方向(弧度)
	NEIGHBOR *neibors;//相邻特征点序列
};


//去除边缘特征点
//minutiaes(特征点数组),count(特征点数量),ucImg(源图数据),iWidth(图像宽度),iHeight(图像高度)
int CutEdge(MINUTIAE * minutiaes, int count, unsigned char * ucImg, int iWidth, int iHeight)
{
	//定义变量
	int minuCount = count;
	int x, y, type;
	bool del;

	//初始化标记数组
	int *pFlag = new int[minuCount];//标记数组(标记值:0-保留,1-删除)
	memset(pFlag, 0, sizeof(int)*minuCount);//初始化(全0全保留)

	//遍历所有特征点
	for (int i = 0; i < minuCount; i++)
	{
		//获取当前特征点信息
		y = minutiaes[i].y - 1;//纵坐标(行坐标)
		x = minutiaes[i].x - 1;//横坐标(列坐标)
		type = minutiaes[i].type;//特征点类型(1-端点,3-分叉点)

		//将当前特征点的删除标记初始化为true
		del = true;

		//根据当前特征点的位置判断其是否边缘特征点
		if (x < iWidth / 2)//如果位于图像左半图
		{
			if (abs(iWidth / 2 - x) > abs(iHeight / 2 - y))//如果位于图像左半图的左侧
			{
				//在特征图中查找当前特征点同一行左侧是否还有其他特征点
				while (--x >= 0)//逐一左移查找
				{
					//如果在左侧存在其他特征点,则说明当前特征点不是边缘特征点,就无须删除
					if (ucImg[x + y * iWidth] > 0)//特征图像素值(0-非特征点,1-端点,3-分叉点)
					{
						del = false;//删除标记置为false
						break;//停止当前特征点的左移查找过程
					}
				}
			}
			else//如果位于图像左半图的右侧
			{
				if (y > iHeight / 2)//如果位于图像左半图的右下侧
				{
					//在特征图中查找当前特征点同一列下侧是否还有其他特征点
					while (++y < iHeight)//逐一下移查找
					{
						//如果在下侧存在其他特征点,则说明当前特征点不是边缘特征点,就无须删除
						if (ucImg[x + y * iWidth] > 0)//特征图像素值(0-非特征点,1-端点,3-分叉点)
						{
							del = false;//删除标记置为false
							break;//停止当前特征点的下移查找过程
						}
					}
				}
				else//如果位于图像左半图的右上侧
				{
					//在特征图中查找当前特征点同一列上侧是否还有其他特征点
					while (--y >= 0)//逐一上移查找
					{
						//如果在上侧存在其他特征点,则说明当前特征点不是边缘特征点,就无须删除
						if (ucImg[x + y * iWidth] > 0)//特征图像素值(0-非特征点,1-端点,3-分叉点)
						{
							del = false;//删除标记置为false
							break;//停止当前特征点的上移查找过程
						}
					}
				}
			}
		}
		else//如果位于图像右半图
		{
			if (abs(iWidth / 2 - x) > abs(iHeight / 2 - y))//如果位于图像右半图的右侧
			{
				//在特征图中查找当前特征点同一行右侧是否还有其他特征点
				while (++x < iWidth)//逐一右移查找
				{
					//如果在右侧存在其他特征点,则说明当前特征点不是边缘特征点,就无须删除
					if (ucImg[x + y * iWidth] > 0)//特征图像素值(0-非特征点,1-端点,3-分叉点)
					{
						del = false;//删除标记置为false
						break;//停止当前特征点的左移查找过程
					}
				}
			}
			else//如果位于图像右半图的左侧
			{
				if (y > iHeight / 2)//如果位于图像右半图的左下侧
				{
					//在特征图中查找当前特征点同一列下侧是否还有其他特征点
					while (++y < iHeight)//逐一下移查找
					{
						//如果在下侧存在其他特征点,则说明当前特征点不是边缘特征点,就无须删除
						if (ucImg[x + y * iWidth] > 0)//特征图像素值(0-非特征点,1-端点,3-分叉点)
						{
							del = false;//删除标记置为false
							break;//停止当前特征点的下移查找过程
						}
					}
				}
				else//如果位于图像右半图的左上侧
				{
					//在特征图中查找当前特征点同一列上侧是否还有其他特征点
					while (--y >= 0)//逐一上移查找
					{
						//如果在上侧存在其他特征点,则说明当前特征点不是边缘特征点,就无须删除
						if (ucImg[x + y * iWidth] > 0)//特征图像素值(0-非特征点,1-端点,3-分叉点)
						{
							del = false;//删除标记置为false
							break;//停止当前特征点的上移查找过程
						}
					}
				}
			}
		}

		//如果当前特征点是边缘特征点,则予以删除(在标记数组中将其标记设置为1)
		if (del)//如果当前特征点的删除标记为true,则表明其为边缘特征点,应予以删除
		{
			pFlag[i] = 1;//在标记数组中将其标记设置为1(表示删除)
			continue;//继续判断结构数组中的下一个特征点
		}
	}

	//重组特征点结构数组(在当前结构数组中将所有有效特征点前移)
	int newCount = 0;//有效特征点数量(同时也是重组后的有效特征点数组下标)
	for (int i = 0; i < minuCount; i++)
	{
		if (pFlag[i] == 0)//如果当前特征点需要保留(标记值:0-保留,1-删除)
		{
			memcpy(&minutiaes[newCount], &minutiaes[i], sizeof(MINUTIAE));//特征点结构整体复制
			newCount++;//有效特征点下标后移(有效特征点数量加1)
		}
	}

	delete[] pFlag;
	pFlag = NULL;

	//返回有效特征点数量
	return newCount;
}


//特征过滤
//minuData(特征图数据),thinData(细化图数据),minutiaes(特征点数组),minuCount(特征点数量),iWidth(图像宽度),iHeight(图像高度)
int MinuFilter(unsigned char *minuData, unsigned char *thinData, MINUTIAE *minutiaes, int &minuCount, int iWidth, int iHeight)
{
	//第1步:计算细化图中各点方向
	float *dir = new float[iWidth*iHeight];
	memset(dir, 0, iWidth*iHeight * sizeof(float));

	ImgDirection(thinData, dir, iWidth, iHeight);//计算脊线方向

	//第2步:从特征图中提取特征点数据
	unsigned char *pImg;
	unsigned char val;
	int temp = 0;
	for (int i = 1; i < iHeight - 1; i++)//逐行遍历
	{
		pImg = minuData + i * iWidth;
		for (int j = 1; j < iWidth - 1; j++)//逐列遍历
		{
			//获取特征图数据
			++pImg;//特征图指针后移
			val = *pImg;//特征图像素值(0-非特征点,1-端点,3-分叉点)

			//提取特征点数据
			if (val > 0)
			{
				minutiaes[temp].x = j + 1;//横坐标(从1开始)
				minutiaes[temp].y = i + 1;//纵坐标(从1开始)
				minutiaes[temp].theta = dir[i*iWidth+j];//脊线方向
				minutiaes[temp].type = int(val);//特征点类型(1-端点,3-分叉点)
				++temp;//特征点数组指针后移
			}
		}
	}
	delete[] dir;

	//第3步:去除边缘特征点
	minuCount = CutEdge(minutiaes, minuCount, thinData, iWidth, iHeight);

	//第4步:去除毛刺/小孔/间断等伪特征点

	//初始化标记数组
	int *pFlag = new int[minuCount];//标记数组(标记值:0-保留,1-删除)
	memset(pFlag, 0, sizeof(int)*minuCount);

	//遍历所有特征点
	int x1, x2, y1, y2, type1, type2;
	for (int i = 0; i < minuCount; i++)//特征点1遍历
	{
		//获取特征点1的信息
		x1 = minutiaes[i].x;
		y1 = minutiaes[i].y;
		type1 = minutiaes[i].type;//特征点类型(1-端点,3-分叉点)

		for (int j = i+1; j < minuCount; j++)//特征点2遍历
		{
			//跳过已删特征点
			if (pFlag[j] == 1)
			{
				continue;
			}

			//获取特征点2的信息
			x2 = minutiaes[j].x;
			y2 = minutiaes[j].y;
			type2 = minutiaes[j].type;//特征点类型(1-端点,3-分叉点)

			//计算两点间距
			int r = (int)sqrt(float((y1 - y2)*(y1 - y2) + (x1 - x2)*(x1 - x2)));

			//删除间距过小的特征点
			if (r <= 4)//如果间距不大于4则认为间距过小(间距阈值可自定)
			{
				if (type1 == type2)//如果两点类型相同
				{
					if (type1 == 1)//如果两点都是端点,则认定为"短线或纹线间断"
					{
						pFlag[i] = pFlag[j] = 1;//同时删掉两点
					}
					else//如果两点都是分叉点,则认定为"小孔"
					{
						pFlag[j] = 1;//只删掉点2
					}
				}
				else if (type1 == 1)//如果点1是端点而点2是分叉点,则点1为"毛刺"
				{
					pFlag[i] = 1;//只删掉点1
				}
				else//如果点1是分叉点而点2是端点,则认定点2为"毛刺"
				{
					pFlag[j] = 1;//只删掉点2
				}
			}
		}
	}

	//重组特征点结构数组(在当前结构数组中将所有有效特征点前移)
	int newCount = 0;//有效特征点数量(同时也是重组后的有效特征点数组下标)
	for (int i = 0; i < minuCount; i++)
	{
		if (pFlag[i] == 0)//如果当前特征点需要保留(标记值:0-保留,1-删除)
		{
			memcpy(&minutiaes[newCount], &minutiaes[i], sizeof(MINUTIAE));//特征点结构整体复制
			newCount++;//有效特征点下标后移(有效特征点数量加1)
		}
	}
	delete[] pFlag;
	minuCount = newCount;//保存有效特征点数量

	//返回结果
	return 0;
}


//保存特征模板文件
//minutiaes(特征点数组),count(特征点数量),fileName(模板文件名)
int SaveMinutiae(MINUTIAE * minutiaes, int count, char * fileName)
{
	//打开文件(二进制写入方式)
	FILE *fp = fopen(fileName, "wb");
	if (!fp)
	{
		return -1;
	}

	//将所有特征点的结构体数据整体写入文件
	const static int TemplateFileFlag = 0x3571027f;
	fwrite(&TemplateFileFlag, sizeof(int), 1, fp);//写入特征模板文件标记
	fwrite(&count, sizeof(int), 1, fp);//写入特征点数量
	for (int i = 0; i < count; i++)//逐一写入所有特征点
	{
		fwrite(&(minutiaes[i]), sizeof(MINUTIAE), 1, fp);//将特征点结构整体写入
	}

	//关闭文件
	fclose(fp);
	return 0;
}


//特征过滤单步测试
//参数:minuCount(特征点数量),info(返回操作成功或失败提示信息)
int Step11_MinuFilter(int &minuCount, char *info)
{

	//设置输入输出文件名
	char srcTxtFile_Minu[MAX_PATH] = { STEP_TXT_10 };//特征图源数据文件名
	char srcTxtFile_Thin[MAX_PATH] = { STEP_TXT_9 };//细化图源数据文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_10 };//源图文件名
	char dstTxtFile[MAX_PATH] = { STEP_TXT_11 };//结果数据文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_11 };//结果图文件名
	char dstMdlFile[MAX_PATH] = { STEP_IMG_11_MDL };//结果模板文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取特征图源数据
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Minu, image1, iWidth, iHeight);

	//读取细化图源数据
	unsigned char *thin = new unsigned char[iWidth*iHeight];
	ReadDataFromTextFile(srcTxtFile_Thin, thin, iWidth, iHeight);

	//特征过滤
	MINUTIAE *minutiaes = new MINUTIAE[minuCount];
	memset(minutiaes, sizeof(MINUTIAE), minuCount);
	MinuFilter(image1, thin, minutiaes, minuCount, iWidth, iHeight);

	//保存模板文件
	SaveMinutiae(minutiaes, minuCount, dstMdlFile);

	//生成结果图像
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	memset(image2, 0, iWidth*iHeight);
	for (int i = 0; i < minuCount; i++)
	{
		image2[(minutiaes[i].y - 1)*iWidth + (minutiaes[i].x - 1)] = 0xff;
	}

	//保存结果数据(TXT文件)
	SaveDataToTextFile(dstTxtFile, image2, iWidth, iHeight);

	//保存结果图像(BMP文件,仅用于显示)
	SaveDataToImageFile(srcImgFile, dstImgFile, image2);

	//释放空间
	delete[] image1;
	delete[] thin;
	delete[] minutiaes;
	delete[] image2;

	//返回成败状态
	sprintf(info, "完成特征过滤.");
	return 0;
}

//获得新指纹编号
int GetNewIndexInDB()
{
	//从index文件中读取最后一条记录的编号
	int sNo = 0;
	char name[MAX_PATH] = { 0 }, srcFile[MAX_PATH] = { 0 }, mdlFile[MAX_PATH] = { 0 };
	FILE *index = fopen(DB_INDEX_TXT, "r");
	if (!index)
	{
		return -1;
	}
	while (!feof(index))
	{
		fscanf(index, "%d %s %s %s \n", &sNo, srcFile, mdlFile, name);
	}
	fclose(index);

	//生成新编号
	sNo = sNo + 1;

	//返回新编号
	return sNo;
}


//特征入库单步测试
//参数:userName(登记人姓名)info(返回操作成功或失败提示信息)
int Step12_Enroll(char *userName,char *info)
{
	//设置输入文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_1 };//源图文件名
	char srcMdlFile[MAX_PATH] = { STEP_IMG_11_MDL };//模板文件名

	//获得数据库内新指纹记录编号
	int sNo = GetNewIndexInDB();

	//设置用户名/源图文件名/模板文件名等需要存入数据库的指纹登记信息
	char regName[MAX_PATH] = { 0 };//登记人姓名
	char dstImgFile[MAX_PATH] = { 0 };//源图文件
	char dstMdlFile[MAX_PATH] = { 0 };//源模板文件
	sprintf(regName, userName);//直接复制界面输入内容
	sprintf(dstImgFile, "%s%d.bmp", DB_DIR, sNo);//存入数据库文件夹内,以编号命名
	sprintf(dstMdlFile, "%s%d.mdl", DB_DIR, sNo);//存入数据库文件夹内,以编号命名

	//保存原始图像
	CopyFile(ToWideChar(srcImgFile), ToWideChar(dstImgFile), false);

	//保存模板文件
	CopyFile(ToWideChar(srcMdlFile), ToWideChar(dstMdlFile), false);

	//指纹登记信息写入数据库索引文件
	FILE *index = fopen(DB_INDEX_TXT, "a");//追加模式写入
	fprintf(index, "%d %s %s %s\n", sNo, dstImgFile, dstMdlFile, regName);
	fclose(index);

	sprintf(info, "完成指纹入库.");
	return 0;
}


//宽字符串转换为多字节字符串
char *ToChar(wchar_t *str)
{
	int num = WideCharToMultiByte(0, 0, str, -1, NULL, 0, NULL, false);
	char *chStr = new char[num];
	WideCharToMultiByte(0, 0, str, -1, chStr, num, NULL, false);
	return chStr;
}

//计算线段倾斜度
//x1(端点1横坐标),y1(端点1纵坐标),x2(端点2横坐标),y2(端点2纵坐标)
float Angle2Points(int x1, int y1, int x2, int y2)
{
	const float PI = 3.141592654;
	float diffY, diffX;
	float theta = 0.0;

	diffY = y2 - y1;
	diffX = x2 - x1;

	if (diffY < 0 && diffX > 0)
	{
		theta = atan2(-1 * diffY, diffX);
	}
	else if (diffY < 0 && diffX < 0)
	{
		theta = PI - atan2(-1 * diffY, -1 * diffX);
	}
	else if (diffY > 0 && diffX < 0)
	{
		theta = atan2(diffY, -1 * diffX);
	}
	else if (diffY > 0 && diffX > 0)
	{
		theta = PI - atan2(diffY, diffX);
	}
	else if (diffX == 0)
	{
		theta = PI / 2;
	}
	else
	{
		theta = 0.0;
	}

	return theta;
}


//构建特征点相邻关系
//minutiae(特征点数组),minuCount(特征点数量)
int BuildNabors(MINUTIAE * minutiae, int minuCount)
{
	//定义变量
	const int MAX_NEIGHBOR_EACH = 10;//每个特征点最多保存10个相邻特征点
	int x1, x2, y1, y2;
	int *pFlag = new int[minuCount];//相邻标记数组(标记值:0-不相邻,1-相邻)

	//遍历特征点数组
	for (int i = 0; i < minuCount; i++)
	{
		//获取当前特征点信息
		x1 = minutiae[i].x;
		y1 = minutiae[i].y;

		//初始化当前特征点的相邻标记数组
		memset(pFlag, 0, sizeof(int)*minuCount);//初始化为全0(不相邻)
		pFlag[i] = 1;//将自身标记为"相邻"

		//为当前特征点创建并初始化相邻特征点结构数组
		minutiae[i].neibors = new NEIGHBOR[MAX_NEIGHBOR_EACH];//创建相邻特征点结构数组

		if (minutiae[i].neibors == NULL)
		{
			return -1;
		}
		memset(minutiae[i].neibors, 0, sizeof(NEIGHBOR)*MAX_NEIGHBOR_EACH);//初始化数组

		//查找和保存10个相邻特征点
		for (int neighborNo = 0; neighborNo < MAX_NEIGHBOR_EACH; neighborNo++)//重复10次
		{
			//初始化最小间距和对应特征点下标
			int minDistance = 1000;//最小间距
			int minNo = 0;//最小间距对应的特征点下标

			//查找相邻特征点之外的最近的不相邻特征点
			for (int j = 0; j < minuCount; j++)//每次都遍历所有特征点
			{
				//跳过已找到的相邻特征点
				if (pFlag[j] == 1)//(标记值:0-不相邻,1-相邻)
				{
					continue;
				}

				//获取特征点2的信息
				x2 = minutiae[j].x;
				y2 = minutiae[j].y;

				//计算两点间距
				int r = (int)sqrt(float((y1 - y2)*(y1 - y2) + (x1 - x2)*(x1 - x2)));

				//查找最小间距
				if (r < minDistance)
				{
					minNo = j;
					minDistance = r;
				}
			}

			//保存查找结果
			pFlag[minNo] = 1;//将找到的最近的不相邻特征点的标记设置为"相邻"
			minutiae[i].neibors[neighborNo].x = minutiae[minNo].x;//横坐标(列坐标)
			minutiae[i].neibors[neighborNo].y = minutiae[minNo].y;//纵坐标(行坐标)
			minutiae[i].neibors[neighborNo].type = minutiae[minNo].type;//特征点类型(1-端点,3-分叉点)
			minutiae[i].neibors[neighborNo].Theta = Angle2Points(minutiae[minNo].x, minutiae[minNo].y, x1, y1);//两点连线角度(弧度)
			minutiae[i].neibors[neighborNo].Theta2Ridge = minutiae[minNo].theta - minutiae[i].theta;//两点脊线方向夹角(弧度)
			minutiae[i].neibors[neighborNo].ThetaThisNibor = minutiae[minNo].theta;//相邻特征点的脊线方向(弧度)
			minutiae[i].neibors[neighborNo].distance = minDistance;//两点距离
		}
	}

	delete[] pFlag;
	return 0;
}


//特征匹配相似度计算
//iWidth(图像宽度),iHeight(图像高度),minutiae1(指纹一的特征点数组),account1(指纹一的特征点数量),minutiae2(指纹二的特征点数组),account2(指纹二的特征点数量)
float MinuSimilarity(int iWidth, int iHeight, MINUTIAE *minutiae1, int count1, MINUTIAE *minutiae2, int count2)
{
	const int MAX_SIMILAR_PAIR = 100;//最多保存100对配对相似特征点
	const int MAX_NEIGHBOR_EACH = 10;//每个特征点最多保存10个相邻特征点

	//构建特征点相邻关系
	BuildNabors(minutiae1, count1);
	BuildNabors(minutiae2, count2);

	//初始化特征点匹配结果(相似特征点点对)
	int similarPair[MAX_SIMILAR_PAIR][2];
	memset(similarPair, 0, 100 * 2 * sizeof(int));

	//选择基准特征和参考特征
	MINUTIAE * baseMinutiae;//基准特征点数组(含特征点较少)
	MINUTIAE * refMinutiae;//参考特征点数组(含特征点较多)
	int baseAccount, refAccount;
	if (count1 < count2)
	{
		baseMinutiae = minutiae1;
		baseAccount = count1;
		refMinutiae = minutiae2;
		refAccount = count2;
	}
	else
	{
		baseMinutiae = minutiae2;
		baseAccount = count2;
		refMinutiae = minutiae1;
		refAccount = count1;
	}

	//方便起见,下文中"特征点"均简称为"点",例如"基准点/参考点/相邻点/相似点"等
	NEIGHBOR *baseNeighbors = NULL;//基准点的相邻点指针
	NEIGHBOR *refNeighbors = NULL;//参考点的相邻点指针
	int similarMinutiae = 0;//相似点对数量(数组下标)
	float baseTheta, refTheta;//基准方向和参考方向

	//特征点匹配(在参考点数组中寻找与每一个基准点对应的最相似的参考点)
	for (int i = 0; i < baseAccount; i++)//逐一遍历基准点数组
	{
		//获取当前基准点信息
		baseNeighbors = baseMinutiae[i].neibors;//基准点的相邻点数组
		baseTheta = baseMinutiae[i].theta;//基准点的脊线方向

		//在参考点数组中寻找与当前基准点最相似的参考点
		int refSimilarNo = 0;
		int maxSimilarNeibors = 0;
		for (int j = 0; j < refAccount; j++)//逐一遍历参考点数组
		{
			//跳过与当前基准点类型不同的参考点
			if (refMinutiae[j].type != baseMinutiae[i].type)
			{
				continue;
			}

			//获取当前参考点信息
			refNeighbors = refMinutiae[j].neibors;//参考点的相邻点数组
			refTheta = refMinutiae[j].theta;//参考点的脊线方向

			//统计相似相邻点数量
			int thisSimilarNeigbors = 0;//相似相邻点数量初始化为0
			for (int m = 0; m < MAX_NEIGHBOR_EACH; m++)//逐一遍历当前基准点的相邻点数组
			{
				//在当前参考点的相邻点数组中查找与当前基准点的当前相似点相似的相邻点
				for (int n = 0; n < MAX_NEIGHBOR_EACH; n++)//逐一遍历当前参考点的相邻点数组
				{
					//跳过类型不同的相邻点
					if (baseNeighbors[m].type != refNeighbors[n].type)
					{
						continue;
					}

					//计算两个相邻点之间的距离差和不同角度差(弧度)
					int dist = abs(int(baseNeighbors[m].distance - refNeighbors[n].distance));
					float theta1 = abs(float((baseNeighbors[m].Theta - baseTheta) - (refNeighbors[n].Theta - refTheta)));
					float theta2 = abs(float(baseNeighbors[m].Theta2Ridge - refNeighbors[n].Theta2Ridge));
					float theta3 = abs(float((baseNeighbors[m].Theta - baseNeighbors[m].ThetaThisNibor) - (refNeighbors[n].Theta - refNeighbors[n].ThetaThisNibor)));

					//如果距离差小于指定阈值(此处取4)并且角度差均小于指定阈值(此处取0.15),则认为这两个相邻点相似
					if (dist < 4 && theta1 < 0.15f&&theta2 < 0.15f&&theta3 < 0.15f)
					{
						++thisSimilarNeigbors;//相似相邻点数量加1
						break;//转向当前基准点的下一个相邻点继续查找
					}
				}
			}

			//如果3对以上相邻点相似,则认为当前基准点与当前参考点相似,保存匹配结果
			if ((thisSimilarNeigbors >= MAX_NEIGHBOR_EACH * 3 / 10) && (similarMinutiae < MAX_SIMILAR_PAIR))
			{
				similarPair[similarMinutiae][0] = i;//保存当前基准点下标
				similarPair[similarMinutiae][1] = refSimilarNo;//保存当前参考点下标
				++similarMinutiae;//相似点对数量加1(数组下标后移)
			}
		}
	}

	//计算特征匹配相似度
	float similarity = similarMinutiae / 8.0f;
	//如果相似特征点数量小于指定下限阈值(此处取2),则认为两个指纹完全不匹配
	similarity = similarMinutiae < 2 ? 0.0f : similarity;//边界值处理
	//如果相似特征点数量大于指定上限阈值(此处取8),则认为两个指纹完全匹配
	similarity = similarMinutiae > 8 ? 1.0f : similarity;//边界值处理

	//返回特征匹配相似度
	return similarity;
}


//读取特征模板文件
//fileName(特征模板文件名),minutiae(特征点数组)
//读取特征模板文件
int ReadMinutiae(char * fileName, MINUTIAE ** minutiae)
{
	//打开文件(二进制读取方式)
	FILE *fp = fopen(fileName, "rb");

	if (!fp)
	{
		return -1;
	}

	//注意整体读取所有特征点的结构体数组
	const static int TemplateFileFlag = 0x3571027f;
	int flag;
	fread(&flag, sizeof(int), 1, fp);//读取特征模板文件标记
	if (flag != TemplateFileFlag)
	{
		return -2;
	}

	int account;
	fread(&account, sizeof(int), 1, fp);//读取特征点数量

	*minutiae = new MINUTIAE[account];//创建特征点结构数组
	if (*minutiae == NULL)
	{
		return -3;
	}

	for (int i = 0; i < account; i++)//逐一读取所有特征点
	{
		fread(&((*minutiae)[i]), sizeof(MINUTIAE), 1, fp);//整体读取特征点结构
	}

	//关闭文件
	fclose(fp);
	return account;
}


//特征匹配单步测试
//参数:info(返回操作成功或失败提示信息)
int Step12_Match(char *info)
{
	//设置输入输出文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_1 };//源图文件名
	char srcMdlFile[MAX_PATH] = { STEP_IMG_11_MDL };//模板文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_12 };//结果图文件名

	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取指纹特征数据
	MINUTIAE *minu = NULL;
	int minuAccount = ReadMinutiae(srcMdlFile, &minu);

	//特征匹配(此处仅测试自身与自身匹配)
	float similar = MinuSimilarity(iWidth, iHeight, minu, minuAccount, minu, minuAccount);

	//释放内存空间
	delete[] minu;

	//保存匹配结果图像
	CopyFile(ToWideChar(srcImgFile), ToWideChar(dstImgFile), false);//复制文件

	//返回操作成败状态
	const float SIMILAR_THRED = 0.1;//匹配相似度阈值(小于阈值则认为不匹配)
	if (similar < SIMILAR_THRED)
	{
		sprintf(info, "匹配失败.");
		return -3;
	}

	sprintf(info, "匹配成功:相似度[%.2f].", similar);
	return 0;
}


//指纹库预检(判断指纹库是否为空)
bool EmptyDB()
{
	//打开文件
	char indexFile[MAX_PATH] = { DB_INDEX_TXT };//指纹库索引文件
	FILE *pFile = fopen(indexFile, "r");
	if (!pFile)//若索引文件不存在或存在异常,则指纹库为空
	{
		return true;
	}

	//判断指纹库是否为空(索引文件内容是否为空)
	int size = _filelength(_fileno(pFile));//文件长度(以字节为单位)
	bool bEmpty = (size < 1) ? true : false;//若索引文件内容为空,则指纹库为空

	//关闭文件
	fclose(pFile);

	//返回判断结果
	return bEmpty;
}


//指纹识别单步测试
//参数:info(返回操作成功或失败提示信息)
int Step12_Identify(char *info)
{
	//设置输入输出文件名
	char srcImgFile[MAX_PATH] = { STEP_IMG_1 };//源图文件名
	char srcMdlFile[MAX_PATH] = { STEP_IMG_11_MDL };//源图指纹特征文件名
	char dstImgFile[MAX_PATH] = { STEP_IMG_12 };//结果图文件名
	char dstMdlFile[MAX_PATH] = { 0 };//结果图指纹特征文件名

	//指纹库预检(判断指纹库是否为空)
	EmptyDB();

	//读取图像参数
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);

	//读取指纹特征数据
	MINUTIAE *minu1 = NULL, *minu2 = NULL;
	int minuAccount1 = 0, minuAccount2 = 0;
	minuAccount1 = ReadMinutiae(srcMdlFile, &minu1);

	//特征识别
	int id = 0;
	char name[MAX_PATH] = { 0 }, src[MAX_PATH] = { 0 }, mdl[MAX_PATH] = { 0 };
	float maxSimilar = -1;
	int maxID = -1;
	char maxName[MAX_PATH] = { 0 }, maxSrc[MAX_PATH] = { 0 };

	FILE *index = fopen(DB_INDEX_TXT, "r");//打开指纹库索引文件

	while (!feof(index))
	{
		//读取指纹记录信息(指纹编号/指纹图像文件名/特征模板文件名/登记人姓名/登记人学号)
		fscanf(index, "%d %s %s %s\n", &id, src, mdl, name);
		sprintf(dstMdlFile, mdl);

		//从特征模板文件中读取指纹特征
		minuAccount2 = ReadMinutiae(dstMdlFile, &minu2);

		//特征匹配
		float similar = MinuSimilarity(iWidth, iHeight, minu1, minuAccount1, minu2, minuAccount2);

		//保存相似度最高的指纹记录信息
		if (similar > maxSimilar)
		{
			maxSimilar = similar;
			maxID = id;
			sprintf(maxName, name);
			sprintf(maxSrc, src);
		}
	}

	fclose(index);//关闭文件

	//释放内存空间
	delete[] minu1;
	delete[] minu2;

	//返回识别结果
	const float SIMILAR_THRED = 0.1;//匹配相似度阈值(小于阈值则认为不匹配)
	if (maxID < 0 || maxSimilar < SIMILAR_THRED)
	{
		sprintf(info, "识别失败.");
		return -2;
	}
	CopyFile(ToWideChar(maxSrc), ToWideChar(dstImgFile), false);//复制识别结果图像
	sprintf(info, "识别成功!\n识别结果:姓名[%s],目标指纹[%s],相似度[%.2f].", maxName, maxSrc, maxSimilar);

	return 0;
}




int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
//按钮:载入图像
void CFingerDlg::OnBnClickedBtnStep1()
{
	char info[MAX_PATH] = { 0 };
	Step1_LoadBmpImage(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	ShowImageInCtrl(m_picCtrl1, STEP_IMG_1);
	flag1 = 1;
}


//按钮:指纹库信息
void CFingerDlg::OnBnClickedBtnDatabase()
{
	char info[MAX_PATH] = { 0 };//初始化显示信息
	GetDatabaseInfo(info);//获取指纹库信息
	m_staticInfo.SetWindowText(ToWideChar(info));//显示指纹库信息
}


//特征提取
void CFingerDlg::OnBnClickedBtnStep12b2()
{
	if (flag1 == 0)
	{
		MessageBox(_T("尚未载入图像！"), _T("提示"));
		return;
	}
	//中值滤波
	char info[MAX_PATH] = { 0 };
	m_staticInfo.SetWindowText(ToWideChar(info));

	int x2=Step2_MidFilter(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x2)
	{
		ShowImageInCtrl(m_picCtrl2, STEP_IMG_2);
	}

	//均衡化
	int x3=Step3_Normalize(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x3)
	{
		ShowImageInCtrl(m_picCtrl3, STEP_IMG_3);
	}
	
	//方向计算
	int x4=Step4_Direction(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x4)
	{
		ShowImageInCtrl(m_picCtrl4, STEP_IMG_4);
	}
	
	//频率计算
	int x5=Step5_Frequency(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x5)
	{
		ShowImageInCtrl(m_picCtrl5, STEP_IMG_5);
	}

	//掩码计算
	int x6=Step6_Mask(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x6)
	{
		ShowImageInCtrl(m_picCtrl6, STEP_IMG_6);
	}
	
	//Gabor增强
	int x7=Step7_GaborEnhance(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x7)
	{
		ShowImageInCtrl(m_picCtrl7, STEP_IMG_7);
	}
	
	//二值化
	int x8=Step8_Binary(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x8)
	{
		ShowImageInCtrl(m_picCtrl8, STEP_IMG_8);
	}
	
	//细化
	int x9=Step9_Thinning(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x9)
	{
		ShowImageInCtrl(m_picCtrl9, STEP_IMG_9);
	}
	
	//特征提取
	int x10=Step10_MinuExtract(m_minuCount, info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x10)
	{
		ShowImageInCtrl(m_picCtrl10, STEP_IMG_10);
	}

	//特征过滤
	int x11=Step11_MinuFilter(m_minuCount, info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	if (!x10)
	{
		ShowImageInCtrl(m_picCtrl11, STEP_IMG_11);
	}
	flag2 = 1;
}


//特征入库
void CFingerDlg::OnBnClickedBtnStep12a()
{
	if (flag2 == 0)
	{
		MessageBox(_T("尚未完成指纹提取！"), _T("提示"));
		return;
	}
	char info[MAX_PATH] = { 0 };
	//获取界面输入内容
	UpdateData(true);
	if (m_name.GetLength() == 0)
	{
		MessageBox(_T("请输入姓名！"), _T("提示"));
		return;
	}
	Step12_Enroll(ToChar(m_name.GetBuffer()),info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	flag3 = 1;
}

//指纹匹配
void CFingerDlg::OnBnClickedBtnStep12b()
{
	if (flag3 == 0)
	{
		MessageBox(_T("尚未完成指纹入库！"), _T("提示"));
		return;
	}
	char info[MAX_PATH] = { 0 };
	Step12_Identify(info);
	m_staticInfo.SetWindowText(ToWideChar(info));
	ShowImageInCtrl(m_picCtrl12, STEP_IMG_12);
}


//退出并且删除temp目录和Database目录
void CFingerDlg::OnBnClickedBtnExit()
{
	CString strDir(TEMP);

	if (strDir.IsEmpty())
	{
		RemoveDirectory(strDir);
		return;
	}

	//首先删除文件及子文件夹
	CFileFind   ff;
	BOOL bFound = ff.FindFile(strDir + _T("\\*"), 0);
	while (bFound)
	{
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))        continue;

		//去掉文件(夹)只读等属性
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory())
		{
			//递归删除子文件夹
			//DeleteDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else
		{
			DeleteFile(ff.GetFilePath());   //删除文件
		}

	}

	ff.Close();

	//然后删除该文件夹
	RemoveDirectory(strDir);



	CString strDir2(DB);

	if (strDir2.IsEmpty())
	{
		RemoveDirectory(strDir2);
		return;
	}

	//首先删除文件及子文件夹
	CFileFind   ff2;
	BOOL bFound2 = ff2.FindFile(strDir2 + _T("\\*"), 0);
	while (bFound2)
	{
		bFound2 = ff2.FindNextFile();
		if (ff2.GetFileName() == _T(".") || ff2.GetFileName() == _T(".."))        continue;

		//去掉文件(夹)只读等属性
		SetFileAttributes(ff2.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff2.IsDirectory())
		{
			//递归删除子文件夹
			//DeleteDirectory(ff.GetFilePath());
			RemoveDirectory(ff2.GetFilePath());
		}
		else
		{
			DeleteFile(ff2.GetFilePath());   //删除文件
		}

	}

	ff.Close();

	//然后删除该文件夹
	RemoveDirectory(strDir2);


	OnOK();//关闭当前对话框（系统界面）
}