
// PigSeparationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PigSeparation.h"
#include "PigSeparationDlg.h"
#include "afxdialogex.h"
#include <OpenNI.h>
#include "Estimate_Weight.h"
#include <opencv2/opencv.hpp>    // OpenCVヘッダ
#include <opencv2/opencv.hpp>    // OpenCVヘッダ
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace openni;
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

float truLen=0;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()


};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPigSeparationDlg dialog



CPigSeparationDlg::CPigSeparationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PIGSEPARATION_DIALOG, pParent)
	, m_controlText(_T(""))
	, m_volume(_T(""))
	, m_length(_T(""))
	, m_control(_T(""))
	, m_male(0)
	, m_female(0)
	, m_show(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPigSeparationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_controlText);
	DDX_Text(pDX, IDC_EDIT2, m_volume);
	DDX_Text(pDX, IDC_EDIT3, m_length);
	DDX_Text(pDX, IDC_EDIT4, m_control);
	DDX_Radio(pDX, IDC_RADIO1, m_male);

}




void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}




BEGIN_MESSAGE_MAP(CPigSeparationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPigSeparationDlg::OnBnClickedButton1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON3, &CPigSeparationDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPigSeparationDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDOK, &CPigSeparationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CPigSeparationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CPigSeparationDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON6, &CPigSeparationDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_SMALLBUTTON2, &CPigSeparationDlg::OnBnClickedSmallbutton2)
	ON_EN_CHANGE(IDC_EDIT3, &CPigSeparationDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT1, &CPigSeparationDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT4, &CPigSeparationDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT2, &CPigSeparationDlg::OnEnChangeEdit2)
	ON_STN_CLICKED(IDC_PIG, &CPigSeparationDlg::OnStnClickedPig)
END_MESSAGE_MAP()


// CPigSeparationDlg message handlers

BOOL CPigSeparationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	UpdateData();// store slider value
	m_sliderCtrl.SetRange(1, 150, TRUE);//start and end of slider control
	m_sliderCtrl.SetPos(30);//start position
	m_controlText.Format(_T("%d"), 30);
	UpdateData(FALSE);


	CStatic * m_picture;
	CFileFind finder, finder2;
	m_picture = (CStatic *)GetDlgItem(IDC_Picture1);
	//BOOL bWorking = finder.FindFile(L"D:\\Program\\pigSeparation\\Koyologo1.bmp");
	BOOL bWorking = finder.FindFile(L"Koyologo1.bmp");
	if (bWorking)
	{
		finder.FindNextFile();
		HBITMAP hp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),finder.GetFilePath(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		m_picture->ModifyStyle(0xf,SS_BITMAP,SWP_NOSIZE);
		m_picture->SetBitmap(hp);
	}



	CStatic * m_picture2=(CStatic *)GetDlgItem(IDC_Picture2);
	//BOOL bWorking2 = finder2.FindFile(L"D:\\Program\\pigSeparation\\UOMlogo1.bmp");
	BOOL bWorking2 = finder2.FindFile(L"UOMlogo1.bmp");
	if (bWorking2)
	{
		finder2.FindNextFile();
		HBITMAP hp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), finder2.GetFilePath(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		m_picture2->ModifyStyle(0xf, SS_BITMAP, SWP_NOSIZE);
		m_picture2->SetBitmap(hp);
	}

	CStatic *m_label;
	CFont *mf = new CFont;
	mf->CreatePointFont(160, _T("Arial Bold"));
	GetDlgItem(IDC_PIG)->SetFont(mf,TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPigSeparationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPigSeparationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPigSeparationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int value = 30;
int rbtControl = 1;
int imgC = 0, imgc2 = 0;
std::string filename;

void CPigSeparationDlg::OnBnClickedButton1()////////////Small Pig Separation//////////////////////
{

	AllocConsole();
	FILE *fv;
	freopen_s(&fv, "CONOUT$", "w", stdout);
	freopen_s(&fv, "CONIN$", "r", stdin);
	FILE *fd, *fp, *ff;
	int levelh = 100, levell = 100;
	cv::Mat separate_img;
	cv::Mat measure_img, result;
	int x_coor, y_coor;
	UpdateData();
	CString str = m_control;
	CT2CA t(str);
	std::string stream3(t); 
	std::ostringstream convert;

	convert << imgC;
	std::string s3 = "s" + stream3 + convert.str();
	std::string figD = "Data/images/depth" + s3 + ".png";
	std::string figR = "Data/images/RGB" + s3 + ".png";
	std::string figRe = "Data/images/gray" + s3 + ".png";
	UpdateData(FALSE);
	int chk = 1;
	
	if (stream3.length() > 0)
	{
		//std::cout << "There are any strings %d" << stream3.length() << endl;
		cv::RotatedRect minRect; 
		cv::RotatedRect minEllipse;
		cv::RotatedRect minBEllipse;
		float xc, yc;
		int zcount = 100000;
		int bc = 1, a, b, u, v;
		int tmpStore;
		////////////////////////////////////////////////////////////////////////////////
		openni::OpenNI::initialize();

		openni::Device device;
		openni::VideoStream depthStream, colorStream;
		auto ret = device.open(openni::ANY_DEVICE);

		if (ret != openni::STATUS_OK) {

			throw std::runtime_error("");

		}
		depthStream.create(device, openni::SensorType::SENSOR_DEPTH);
		colorStream.create(device, openni::SensorType::SENSOR_COLOR);

		depthStream.start();
		colorStream.start();

		cv::Mat depthImage, colorImage;

		std::vector<openni::VideoStream*> streams;

		streams.push_back(&depthStream);

		/////////////////////////////////////////////////////////////////////////////

		while (1)
		{
			////////////////////////////////////////////////////////////////////////
			int changedIndex;
			openni::OpenNI::waitForAnyStream(&streams[0], streams.size(), &changedIndex);
			openni::VideoFrameRef depthFrame, colorFrame;

			depthStream.readFrame(&depthFrame);

			depthImage = cv::Mat(depthStream.getVideoMode().getResolutionY(),

				depthStream.getVideoMode().getResolutionX(),

				CV_16U, (char*)depthFrame.getData());


			//--------------
			colorStream.readFrame(&colorFrame);
			colorImage = cv::Mat(colorFrame.getHeight(),
				colorFrame.getWidth(),
				CV_8UC3, (unsigned char*)colorFrame.getData());
			///////////////////////////////////////////////////////////////////////

			//Get RGB image

			depthImage.convertTo(measure_img, CV_8U, 255.0 / 10000);//depth image grayscale (2)channels to color image (3)channels
			cv::cvtColor(measure_img, result, CV_GRAY2BGR);//
			cv::rectangle(result, cv::Rect((depthImage.cols - 1) / 2 - 10, (depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//get the center position
			depthImage.copyTo(separate_img);
			UINT16 d = depthImage.at<UINT16>((depthImage.rows - 1) / 2, (depthImage.cols - 1) / 2);// intensity of depth image
			//int val = (int)d * 9000 / 65535;
																								   //cv::line(result, cv::Point(0, depthImage.rows / 2), cv::Point(depthImage.cols, depthImage.rows / 2), cv::Scalar(0, 0, 255), 2);//get the center position
																								   //cv::line(result, cv::Point(depthImage.cols / 2, 0), cv::Point(depthImage.cols / 2, depthImage.rows), cv::Scalar(0, 0, 255), 2);//get the center position


			int ddd;
			int hd, ld, hdd, ldd;
			if (zcount == 100000)//take intensity value from center position
			{
				//ddd = (int)d * 9000 / 65535;
				hd = (d-100) - levelh;
				ld = (d - 100) + levell;
				//hdd = hd * 65535 / 9000;//to change millimeters to intensity
				//ldd = ld * 65535 / 9000;
				hd = (d - 100) - levelh;
				ld = (d - 100) + levell;
				hdd = hd * (d - 100);//to change millimeters to intensity
				ldd = ld * (d - 100);
				
			}
			else
			{
				ddd = zcount;
				hd = ddd - levelh;
				ld = ddd + levell;
				//hdd = hd * d;//to change millimeters to intensity
				//ldd = ld * d;
				hdd = hd * (d - 100);//to change millimeters to intensity
				ldd = ld * (d - 100);
			}


			//store nearest intensity values from center
			int maxh = 100000;
			for (int y = 0; y < depthImage.rows; y++) {
				for (int x = 0; x < depthImage.cols; x++) {
					UINT16 d = depthImage.at<UINT16>(y, x);
					//int dd = (int)d * 9000 / 65535;
					if (d > hd && d <= ld)
					{
						separate_img.at<UINT16>(y, x) = d;
					}
					else
					{
						separate_img.at<UINT16>(y, x) = 0;
					}
				}
			}

			cv::Mat binary = separate_img > 100;
			medianBlur(binary, binary, 15);
			cv::Mat element(9, 1, CV_8U, cv::Scalar(1));
			cv::Mat element1(3, 3, CV_8U, cv::Scalar(1));
			cv::Mat element3(0, 3, CV_8U, cv::Scalar(1));
			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element);
			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element1, cv::Point(-1, -1), 1);
			std::vector<std::vector<cv::Point>>contours; // to stroe contours point
			std::vector<cv::Vec4i>hierarchy;
			cv::Canny(binary, binary, 200, 225, 3);// edge detection
			cv::imshow("canny", binary);
			cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);//find contours from canny edge result


			for (int i = 0; i < contours.size(); i++)
			{
				if (cv::contourArea(contours[i]) >= 100)
				{

					minRect = cv::minAreaRect(cv::Mat(contours[i]));


					cv::RotatedRect minEllipse;
					if (contours[i].size()>100)
					{
						minEllipse = cv::fitEllipse(cv::Mat(contours[i]));//change to ellipse
						float x = minEllipse.center.x;
						float y = minEllipse.center.y;
						float aa = minEllipse.size.width / 2;    // width >= height
						float bb = minEllipse.size.height / 2;

						//int dist = sqrt(((x - depthImage.cols / 2)*(x - depthImage.cols / 2)) + ((y - depthImage.rows / 2)*(y - depthImage.rows / 2)));

						//if (dist < mindist)
						if (x - bb <= depthImage.cols / 2 + 20 && x + bb >= depthImage.cols / 2 - 20 && y - aa <= depthImage.rows / 2 + 20 && y + aa >= depthImage.cols / 2 - 20)//square is in the center or not
						{
							//mindist = dist;
							xc = x; yc = y; bc = i; a = aa; b = bb;
							minBEllipse = cv::fitEllipse(cv::Mat(contours[bc]));
						}
					}
				}
			}





			if (bc > 0)
			{
				if (xc > 0 && xc <= depthImage.cols - 1 && yc > 0 && yc <= depthImage.rows - 1 && a>0 && b>0)
				{
					cv::ellipse(result, minBEllipse, cv::Scalar(0, 0, 255), 2, 8);//draw ellipse
					cv::circle(result, cv::Point(xc, yc), 3, cv::Scalar(0, 255, 0), CV_FILLED);//center of ellipse
					std::string s = "Data/smallseparation/coordinate_" + s3 + ".xyz";
					fp = fopen(s.c_str(), "wt");


					std::string ss = "Data/smallseparation/uv_" + s3 + ".txt";
					ff = fopen(ss.c_str(), "wt");
					fprintf(ff, "%d\n%d\n%d\n%d", (int)yc - a, (int)yc + a, (int)xc - b, (int)xc + b);
					fclose(ff);


					//store xyz values in file
					for (int x = xc - b; x < xc + b; x++)
					{
						for (int y = yc - a; y < yc + a; y++)
						{
							UINT16 d = separate_img.at<UINT16>(y, x);
							//int z_coor = (int)d * 4500 / 65535;		//get z coordinate
							//int z = 1000 / 3;
							//int dd = (int)d * d / 65535;
							if (d != 0)
							{
								x_coor = -1 * (x - 320)*(d-100) / 300;
								y_coor = (y - 240)*(d - 100) / 300;
								if (d < maxh)
								{
									maxh = (d - 100);
									u = ((x_coor * 300) + (320 * (d - 100))) / (d - 100);
									v = ((y_coor * 300) + (240 * (d - 100))) / (d - 100);
								}
								fprintf(fp, "%d %d %d\n", x_coor, y_coor, (d - 100));
							}
						}
					}
					cv::circle(result, cv::Point(u, v), 3, cv::Scalar(255, 0, 255), CV_FILLED);//Draw a point at the coordinates of the pig's top position
					fclose(fp);

					//Check if the top position of pig is correct
					if (maxh != 100000 && d <= tmpStore + 3 && d >= tmpStore - 3)// && zcount>maxh)
					{
						zcount = maxh;
					}
					else
					{
						zcount = 100000;
					}
				}
			}

			cv::imwrite(figD, depthImage);
			cv::imwrite(figR, colorImage);
			//cv::namedWindow("Separate", CV_WINDOW_AUTOSIZE);
			cv::imshow("Separate", separate_img);
			//cv::namedWindow("result", CV_WINDOW_AUTOSIZE);
			cv::imshow("result", result);
			cv::imwrite(figRe, result);
			
			//cv::namedWindow("canny", CV_WINDOW_AUTOSIZE);
			cv::imshow("canny", binary);
		
			//cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//find contours from canny edge result
			if (cv::waitKey(30) >= 0) { break; }
		}
	}
	else
	{
		AfxMessageBox(_T("Firstly, please enter filename!"), MB_ICONSTOP);
	}
	imgC++;
	cv::destroyAllWindows();
}

void CPigSeparationDlg::OnBnClickedButton2()
{
	std::vector<int>xx_val;//store file read data x
	std::vector<int>yy_val;//store file read data y
	std::vector<int>zz_val;//store file read data z


	AllocConsole();
	FILE *fd, *fs, *ff;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
	int x, y, z, r, g, b;
	FILE *fp;
	CString filepath, fn;
	CString         filter("XYZ Files (coordinate*.xyz)|coordinate*.xyz");
	CFileDialog     selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();
		fn = selDlg.GetFileName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fp = fopen(csTempmb.GetBuffer(0), "r");
	}



	CT2CA t(fn);
	std::string stream3(t);
	ff = fopen("data.txt", "wt");
	std::string s3 = stream3;
	s3 = "cross_section" + s3 + ".xyz";
	fs = fopen(s3.c_str(), "wt");


	int minx = 100000, maxx = -100000, minz = 100000, maxz = -100000;	int sumz = 0;
	int ic = 0;


	cv::Mat xMat4 = cv::Mat_<double>(1000, 5);//store nearest position on cross section,4power,top view
	cv::Mat xMat2 = cv::Mat_<double>(1000, 3);//store nearest position on cross section,2power,side view
	cv::Mat yMat = cv::Mat_<double>(1000, 1);
	cv::Mat zMat = cv::Mat_<double>(1000, 1);

	int *xlength = new int[100];


	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {
		xx_val.push_back(x);
		yy_val.push_back(y);
		zz_val.push_back(z);
		if (minx > x) { minx = x; }
		if (maxx < x) { maxx = x; }
		if (minz > z) { minz = z; }
		if (maxz < z) { maxz = z; }
		sumz += z;
	}


	int datasize1 = 0;
	int avgz = sumz / zz_val.size();
	int cccc = 0;

	for (int j = minx; j <= maxx; j += value)//maxx
	{
		int seg_min = 100000;
		std::vector<int>divide_x;//store raw cross section data
		std::vector<int>divide_y;
		std::vector<int>divide_z;
		double ymax = -100000, ymin = 100000;
		float last = 1;
		int index = 0;
		for (int i = 0; i <= xx_val.size(); i++)
		{
			if (xx_val[i]>j && xx_val[i]>j - 3 && xx_val[i] < j + 3 && zz_val[i] != 0)//select 3 difference from vminx
			{
				divide_x.push_back(xx_val[i]);//xx_val[i]
				divide_y.push_back(yy_val[i]);
				divide_z.push_back(zz_val[i]);
				if (yy_val[i] < ymin) { ymin = yy_val[i]; }
				if (yy_val[i] > ymax) { ymax = yy_val[i]; }
			}
		}





		if (divide_y.size() > 10)
		{
			cv::Mat fmax = cv::Mat_<double>(divide_y.size(), 3);//draw 2 power estimation curve to reduce digital error
			cv::Mat fz = cv::Mat_<double>(divide_y.size(), 1);

			for (int n = 0; n < divide_x.size(); n++)
			{
				fprintf(fs, "%d %d %d %d %d %d\n", divide_x[n], divide_y[n], divide_z[n], 100, 255, 255);
				fmax.at<double>(n, 0) = (double)divide_y[n] * divide_y[n];
				fmax.at<double>(n, 1) = (double)divide_y[n];
				fmax.at<double>(n, 2) = (double)last;
				fz.at<double>(n, 0) = (double)divide_z[n];
			}



			cv::Mat inverx; int index = 0;
			inverx = fmax.inv(cv::DECOMP_SVD);
			cv::Mat zresult = inverx*fz;
			double minz = 100000.000, valy;
			cccc++;


			for (int i = ymin; i < ymax; i += 1)//find z value of top position on estimation curve
			{
				double valz = zresult.at<double>(0, 0)*i*i + zresult.at<double>(1, 0)*i + zresult.at<double>(2, 0);
				//fprintf(ff, "%d %d %f %d %d %d\n", j, i, valz, 255, 255, 0);
				if (valz < minz)
				{
					minz = valz;
					valy = i;
				}
			}



			double mind = 10000.00;//find nearest position between estimaion curve and cross section
			for (int s = 0; s < divide_x.size(); s++)
			{
				double dist = sqrt(((valy - divide_y[s])*(valy - divide_y[s])) + ((minz - divide_z[s])*(minz - divide_z[s])));
				if (mind >= dist) { mind = dist; index = s; }
			}


			//store each top position 
			fprintf(ff, "%d %d %d %d %d %d\n", divide_x[index], divide_y[index], divide_z[index], 255, 255, 0);
			xMat4.at<double>(ic, 0) = (double)divide_x[index] * divide_x[index] * divide_x[index] * divide_x[index];
			xMat4.at<double>(ic, 1) = (double)divide_x[index] * divide_x[index] * divide_x[index];
			xMat4.at<double>(ic, 2) = (double)divide_x[index] * divide_x[index];
			xMat4.at<double>(ic, 3) = (double)divide_x[index];
			xMat4.at<double>(ic, 4) = (double)1;

			xMat2.at<double>(ic, 0) = (double)divide_x[index] * divide_x[index];
			xMat2.at<double>(ic, 1) = (double)divide_x[index];
			xMat2.at<double>(ic, 2) = (double)1;

			yMat.at<double>(ic, 0) = (double)divide_y[index];
			zMat.at<double>(ic, 0) = (double)divide_z[index];

			xlength[ic] = divide_x[index];
			ic++;
		}
	}



	int curxmx = -100000, curxmn = 100000, curymx = -100000, curymn = 100000, curzmx = -100000, curzmn = 100000;
	for (int s = 0; s < ic; s++)
	{
		int val;
		val = xlength[s];
		if (curxmn >val) { curxmn = val; }
		if (curxmx < val) { curxmx = val; }
	}



	xMat4.resize(ic);
	xMat2.resize(ic);
	yMat.resize(ic);
	zMat.resize(ic);
	cv::Mat inverx4, inverx2;
	inverx4 = xMat4.inv(cv::DECOMP_SVD);
	inverx2 = xMat2.inv(cv::DECOMP_SVD);
	cv::Mat resultz = inverx4*zMat;
	cv::Mat resulty = inverx2*yMat;
	int tempx = 0, tempy = 0, tempz = 0;
	float distance = 0.0;
	int u, v;
	for (float i = (float)curxmn; i <= curxmx; i += 1)
	{
		int yy = (int)i * i * resulty.at<double>(0, 0) + i * resulty.at<double>(1, 0) + resulty.at<double>(2, 0);
		int zz = (int)i * i * i * i * resultz.at<double>(0, 0) + i * i * i * resultz.at<double>(1, 0) + i * i * resultz.at<double>(2, 0) + i * resultz.at<double>(3, 0) + resultz.at<double>(4, 0);
		//fprintf(ff, "%d %d %d %d %d %d\n", (int)i, yy, zz, 255, 0, 255);

		if (i > curxmn)
		{
			distance += sqrt(((i - tempx)*(i - tempx)) + ((yy - tempy)*(yy - tempy)) + ((zz - tempz)*(zz - tempz)));
		}
		tempx = i; tempy = yy; tempz = zz;
	}




	distance = distance / 10;
	UpdateData();
	CString aup;
	aup.Format(_T("%f"), distance);
	m_length = aup;
	UpdateData(FALSE);

	AfxMessageBox(_T("All results have been saved!"), MB_OK | MB_ICONINFORMATION);

	fclose(ff);
	fclose(fs);
	FreeConsole();
}


void CPigSeparationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar *)&m_sliderCtrl) {
		UpdateData();
		value = m_sliderCtrl.GetPos(); 
		m_controlText.Format(_T("%d"), value);
		UpdateData(FALSE);
	}
	else {
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


int threshold = 30;
#define Pi 3.14
void CPigSeparationDlg::OnBnClickedButton3()////////////////Circle Estimation////////////////////////////////////
{
	std::vector<int>xx_val; 
	std::vector<int>yy_val; 
	std::vector<int>zz_val;
	std::vector<int>estimate_x; 
	std::vector<int>estimate_y;
	std::vector<int>estimate_z;	
	std::vector<int>en_x; 
	std::vector<int>en_y;
	std::vector<int>en_z;

	AllocConsole();
	FILE* fd, *fs, *fa;
	FILE  *fw, *fq, *fg;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
	int x, y, z, r, g, b;




	FILE *fp;
	CString filepath, fn;
	CString         filter("XYZ Files (coordinate_*.xyz)|coordinate_*.xyz");//Read three-dimensional coordinate file
	CFileDialog     selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();
		fn = selDlg.GetFileName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0); 
		fp = fopen(csTempmb.GetBuffer(0), "r");
	}

	FILE *ff;
	CString         filter1("TEXT Files (circleData_*.txt)|circleData_*.txt");//Read the top position of each section
	CFileDialog     selDlg1(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter1);
	if (selDlg1.DoModal() == IDOK)
	{
		filepath = selDlg1.GetPathName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		ff = fopen(csTempmb.GetBuffer(0), "r");
	}


	CT2CA t(fn);
	std::string stream3(t); 
	std::string s3 = stream3;
	s3 = "Data/circle/circle_" + s3 + ".txt"; 
	fs = fopen(s3.c_str(), "wt"); 

	std::string s10 = stream3;
	s10 = "Data/circle/circlex_" + s10 + ".txt";
	fw = fopen(s10.c_str(), "wt");

	std::string s11 = stream3;
	s11 = "Data/circle/circley_" + s11 + ".txt";
	fq = fopen(s11.c_str(), "wt");

	std::string s12 = stream3;
	s12 = "Data/circle/circlez_" + s12 + ".txt";
	fg = fopen(s12.c_str(), "wt");




	int minx = 100000, maxx = -100000;	int sumz = 0;
	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {
		xx_val.push_back(x);
		yy_val.push_back(y);
		zz_val.push_back(z);
		if (minx > x) { minx = x; }
		if (maxx < x) { maxx = x; } 
	}

	while ((fscanf(ff, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF)
	{
		estimate_x.push_back(x);
		estimate_y.push_back(y);
		estimate_z.push_back(z);		
	}




	float sum = 0, count = 0;
	//Split 3D coordinates
	for (int j = minx; j <= maxx; j += value)
	{
		std::vector<int>divide_x; 
		std::vector<int>divide_y;
		std::vector<int>divide_z;
		int index = 0;
		for (int i = 0; i <= xx_val.size(); i++)
		{
			//One section in the divided data is preserved with a width of ± 3 mm
			if (xx_val[i] > j - 3 && xx_val[i] < j + 3)
			{
				divide_x.push_back(xx_val[i]);
				divide_y.push_back(yy_val[i]);
				divide_z.push_back(zz_val[i]);
			}
		}

		float bestr = 0; 
		float bestx = 0;
		float besty = 0;
		float bestcount = 0;
		float last = 1;

		if (divide_x.size() > 10)
		{
			for (int p = 0; p < 100000; p++)
			{
				int* arr = new int[80];//80 points are randomly extracted from one section of the divided data
				for (int s = 0; s < 80; s++)
				{
					int random = rand() % divide_x.size();
					arr[s] = random;
				}


				cv::Mat cirMat = cv::Mat_<double>(80, 3);
				cv::Mat cmMat = cv::Mat_<double>(80, 1);

				for (int n = 0; n < 80; n++)//Find circle parameters
				{
					cirMat.at<double>(n, 0) = (float)divide_y[arr[n]]; 
					cirMat.at<double>(n, 1) = (float)divide_z[arr[n]];
					cirMat.at<double>(n, 2) = (float)last;
					cmMat.at<double>(n, 0) = (float)(-(divide_y[arr[n]] * divide_y[arr[n]]) - (divide_z[arr[n]] * divide_z[arr[n]]));
				}
				
				cv::Mat inverx;
				inverx = cirMat.inv(cv::DECOMP_SVD);
				cv::Mat cresult = inverx*cmMat;

				double a = cresult.at<double>(0, 0);
				double b = cresult.at<double>(1, 0);
				double c = cresult.at<double>(2, 0);

				double x1, y1, r; //(x1,y1)：Center coordinates of circle  r：Radius of circle
				x1 = -a / 2;
				y1 = -b / 2;
				r = sqrt(pow(x1, 2) + pow(y1, 2) - c);

				if (r > 200)continue;
				double d;
				int cou = 0;

				//Make sure the center coordinates and radius of the circle are correct
				for (int z = 0; z < divide_x.size(); z++)
				{
					d = fabs(sqrt(pow(divide_y[z] - x1, 2) + pow(divide_z[z] - y1, 2)) - r);
					if (d < threshold)
					{
						cou++; 
					}
				}
				if (cou > bestcount)
				{
					//Final determination of the center coordinates and radius of the circle
					bestr = r;
					bestx = x1;
					besty = y1;
					bestcount = cou;
					cout <<"count="<< cou << endl;
				}
			}

			double surf_area = (float)Pi*bestr*bestr;//Find the surface area
			sum += surf_area;
			count++;
			int c = 0;
			//Modeling a circle
			for (float m = 0; m <= 2 * M_PI; m += M_PI / 60)
			{
				float cx = bestx + (bestr*sin(m));
				float cy = besty + (bestr*cos(m));
				fprintf(fs, "%d %f %f\n", j - 3, cx, cy);
				fprintf(fw, "%d\n", j-3);
				fprintf(fq, "%f\n", cx);

				fprintf(fg, "%f\n", cy);
			}
			double enshu ;
			enshu = 2 * bestr*M_PI;
			printf("%lf\n", enshu); 
			
		}
	}

	fclose(fs);
	fclose(ff);
	fclose(fw);
	fclose(fq);
	fclose(fg);
	printf("\n");
	
}

void CPigSeparationDlg::OnBnClickedButton4()///////////////Weight Estimation//////////////////////
{
	AllocConsole();
	FILE *fv;
	freopen_s(&fv, "CONOUT$", "w", stdout);
	freopen_s(&fv, "CONIN$", "r", stdin);


	//★Put time in file name
	char date[64];
	time_t timer = time(NULL);
	strftime(date, sizeof(date), "%Y%m%d_%H%M%S", localtime(&timer));
	//Get current time
	time(&timer);
	std::stringstream stream5;
	stream5 << "Data/weight/coordinate_" << date << ".xyz";
	std::string s5 = stream5.str();

	std::stringstream stream6;
	stream6 << "Data/images/weight/depth_" << date << ".png";
	std::string s6 = stream6.str();

	std::stringstream stream7;
	stream7 << "Data/images/weight/RGB_" << date << ".png";
	std::string s7 = stream7.str();

	std::stringstream stream8;
	stream8 << "Data/weight/uv_" << date << ".txt";
	std::string s8 = stream8.str();


	//★

	FILE *fp, *ff, *fa, *fb, *fc;
	
	int levelh = 100, levell = 100;
	cv::Mat separate_img;
	cv::Mat measure_img, result;
	int x_coor, y_coor;
	UpdateData();
	CString str = m_volume;
	CT2CA t(str);
	std::string stream3(t);
	std::ostringstream convert;
	convert << imgC;
	std::string s3 = "b"+stream3 + convert.str();
	UpdateData(FALSE);
	std::string s1, s2;


	std::vector<int>xx_val;
	std::vector<int>yy_val;
	std::vector<int>zz_val;
	int ue, ve, he, we;
	std::cout << stream3.length() << endl;
	if (stream3.length() > 0)
	{
		std::string sc = "Data/weight/cross_weight" + s3 + ".txt";
		fa = fopen(sc.c_str(), "wt");
		std::string sb = "Data/weight/sb" + s3 + ".txt";
		fb = fopen(sb.c_str(), "wt");
		std::string s_curve = "Data/weight/curve" + s3 + ".txt";
		fc = fopen(s_curve.c_str(), "wt");
		//fa = fopen("Data/a.txt", "wt");
		//fb = fopen("Data/b.txt", "wt");
		//fc = fopen("Data/c.txt", "wt");
		int c = 0;
		cv::RotatedRect minRect;
		cv::RotatedRect minBEllipse;
		float xc, yc;
		int zcount = 100000;
		int bc = 0, a, b, u, v;
		int tmpStore;
		////////////////////////////////////////////////////////////////////////////////
		openni::OpenNI::initialize();

		openni::Device device;
		openni::VideoStream depthStream, colorStream;
		auto ret = device.open(openni::ANY_DEVICE);

		if (ret != openni::STATUS_OK) {

			throw std::runtime_error("");

		}
		depthStream.create(device, openni::SensorType::SENSOR_DEPTH);
		colorStream.create(device, openni::SensorType::SENSOR_COLOR);

		depthStream.start();
		colorStream.start();

		cv::Mat depthImage, colorImage;

		std::vector<openni::VideoStream*> streams;

		streams.push_back(&depthStream);
	

		/////////////////////////////////////////////////////////////////////////////
		while (1)
		{
			//////////////////////////////////////////////////////////
			int changedIndex;
			openni::OpenNI::waitForAnyStream(&streams[0], streams.size(), &changedIndex);
			openni::VideoFrameRef depthFrame, colorFrame;

			depthStream.readFrame(&depthFrame);

			depthImage = cv::Mat(depthStream.getVideoMode().getResolutionY(),

				depthStream.getVideoMode().getResolutionX(),

				CV_16U, (char*)depthFrame.getData());


			//--------------
			colorStream.readFrame(&colorFrame);
			colorImage = cv::Mat(colorFrame.getHeight(),
				colorFrame.getWidth(),
				CV_8UC3, (unsigned char*)colorFrame.getData());

			/////////////////////////////////////////////////////////

			depthImage.convertTo(measure_img, CV_8UC1, 255.0 / 2900);//depth image grayscale (2)channels to color image (3)channels
			cv::cvtColor(measure_img, result, CV_GRAY2BGR);//
			cv::rectangle(result, cv::Rect((depthImage.cols - 1) / 2 - 10, (depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//get the center position
			depthImage.copyTo(separate_img);
			UINT16 d = depthImage.at<UINT16>((depthImage.rows - 1) / 2, (depthImage.cols - 1) / 2);// intensity of depth image
			//int val = (int)d * 4500 / 65535;


			int ddd;
			int hd, ld, hdd, ldd;
			if (zcount == 100000)//Get intensity value from center position
			{
				//ddd = (int)d * 4500 / 65535;//change to millimeters
				hd = d - levelh;
				ld = d + levell;
				hdd = hd * d;//to change millimeters to intensity
				ldd = ld * d;
				tmpStore = d;
			}
			else
			{
				ddd = zcount;
				hd = d - levelh;
				ld = d + levell;
				hdd = hd * d;//to change millimeters to intensity
				ldd = ld * d;
			}

			//Store the closest intensity value from the center
			int maxh = 100000;
			for (int y = 0; y < depthImage.rows; y++) {
				for (int x = 0; x < depthImage.cols; x++) {
					UINT16 d = depthImage.at<UINT16>(y, x);
					//int dd = (int)d * 4500 / 65535;
					if (d > hd && d <= ld)
					{
						separate_img.at<UINT16>(y, x) = d;
					}
					else
					{
						separate_img.at<UINT16>(y, x) = 0;
					}
				}
			}


			cv::Mat binary = separate_img > 100;//Binarization
			medianBlur(binary, binary, 15);
			medianBlur(binary, binary, 15);
			medianBlur(binary, binary, 1);
			cv::Mat element(9, 1, CV_8U, cv::Scalar(1));
			cv::Mat element1(3, 3, CV_8U, cv::Scalar(1));
			cv::Mat element3(0, 3, CV_8U, cv::Scalar(1));

			//cv::imshow("test5", binary);
			//cv::imwrite("binary.png", binary);

			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element);
			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element1, cv::Point(-1, -1), 1);

			std::vector<std::vector<cv::Point>>contours; //Stroke contour point
			std::vector<cv::Vec4i>hierarchy;
			
			cv::Canny(binary, binary, 100, 225, 3);//Edge detection
			cv::imshow("canny", binary);
			cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//Find contours
			int mindist = 100000;

			for (int i = 0; i < contours.size(); i++)
			{
				if (cv::contourArea(contours[i]) >= 100)
				{
					minRect = cv::minAreaRect(cv::Mat(contours[i]));//Contour area
					cv::RotatedRect minEllipse;
					if (contours[i].size()>100)//release small size
					{
						minEllipse = cv::fitEllipse(cv::Mat(contours[i]));//Change to ellipse
						float x = minEllipse.center.x; //x：The x coordinate of the center of the ellipse
						float y = minEllipse.center.y; //y：The y coordinate of the center of the ellipse
						float aa = minEllipse.size.width / 2;  
						float bb = minEllipse.size.height / 2;
						//int dist = sqrt(((x - depthImage.cols / 2)*(x - depthImage.cols / 2)) + ((y - depthImage.rows / 2)*(y - depthImage.rows / 2)));
						//if (dist < mindist)
					    if (x - bb <= depthImage.cols / 2 + 20 && x + bb >= depthImage.cols / 2 - 20 && y - aa <= depthImage.rows / 2 + 20 && y + aa >= depthImage.cols / 2 - 20)//Whether the square is in the center or not
						{
							//mindist = dist;
							xc = x; yc = y; bc = i; a = aa; b = bb;
							minBEllipse = cv::fitEllipse(cv::Mat(contours[bc]));
						}
					}
				}
			}



			if (bc > 0)
			{
				if (xc > 0 && xc <= depthImage.cols - 1 && yc > 0 && yc <= depthImage.rows - 1 && a>0 && b>0)
				{
					cv::ellipse(result, minBEllipse, cv::Scalar(0, 0, 255), 2, 8);//Draw an ellipse

					fp = fopen(s5.c_str(), "wt");
					xx_val.clear();
					yy_val.clear();
					zz_val.clear();
					c = 0;
					//s7 = "Data/uv_" + s3 + ".txt";
					ff = fopen(s8.c_str(), "wt");
					fprintf(ff, "%d\n %d\n %d\n %d", (int)yc - a, (int)yc + a, (int)xc - b, (int)xc + b);
					fclose(ff);

					/*ue = yc - a;
					ve = yc + a;
					we = xc - b;
					he = xc + b;*/


					ue = yc - a;
					we = yc + a;
					ve = xc - b;
					he = xc + b;

					//Store xyz value in file
					for (int x = xc - b; x < xc + b; x++)
					{
						for (int y = yc-a; y < yc + a; y++)
						{
							UINT16 d = separate_img.at<UINT16>(y, x);
							//int z_coor = (int)d * 4500 / 65535;//Get z coordinate
							if (d != 0)//
							{
								x_coor = -1 * (x - 320)*d / 365; //x_coor:Find x coordinate from z coordinate
								y_coor = (y - 240)*d / 365; //y_coor:Find y coordinate from z coordinate
								if (d < maxh)
								{
									maxh = d;
									//(u,v) is the coordinates of the pig's top position
									u = ((x_coor * 365) + (320 * d)) / d;
									v = ((y_coor * 365) + (240 * d)) / d;
								}
								xx_val.push_back(x_coor);
								yy_val.push_back(y_coor);
								zz_val.push_back(d);
								fprintf(fp, "%d %d %d\n", x_coor, y_coor, d);//Save x, y, z coordinates to a file
								c++;
							}
						}
					}
					//cv::circle(result, cv::Point(u, v), 3, cv::Scalar(255, 0, 255), CV_FILLED);
					fclose(fp);

					if (maxh != 100000 && d <= tmpStore + 3 && d >= tmpStore - 3)// && zcount>maxh)
					{
						zcount = maxh;
					}

					else
					{
						zcount = 100000;
					}
				}
			}
			cv::imshow("Separate", separate_img);
			cv::imshow("result", result);
			if (cv::waitKey(30) >= 0) { break; }
		}
		cv::imwrite(s7, colorImage);
	


		int minx = 100000, maxx = -100000, minz = 100000, maxz = -100000, sumz = 0;
		int ic = 0;


		cv::Mat xMat4 = cv::Mat_<double>(1000, 5);//Quartic curve, top view
		cv::Mat xMat2 = cv::Mat_<double>(1000, 3);//Quadratic curve, Side view
		cv::Mat yMat = cv::Mat_<double>(1000, 1);
		cv::Mat zMat = cv::Mat_<double>(1000, 1);

		int *xlength = new int[100];



		for (int i = 0; i <xx_val.size(); i++)
		{
			if (minx > xx_val[i]) { minx = xx_val[i]; } 
			if (maxx < xx_val[i]) { maxx = xx_val[i]; } 
			if (minz > zz_val[i]) { minz = zz_val[i]; } 
			if (maxz < zz_val[i]) { maxz = zz_val[i]; } 
			sumz = sumz + zz_val[i];                      
		}

		int avgz = sumz / zz_val.size();
		int datasize1 = 0;
		int curxmx = -100000, curxmn = 100000, curymx = -100000, curymn = 100000, curzmx = -100000, curzmn = 100000;
		int earpos = 0;
		for (int j = minx; j <= maxx; j += value)//maxx
		{
			int seg_min = 100000;
			std::vector<int>divide_x;
			std::vector<int>divide_y;
			std::vector<int>divide_z;
			double ymax = -100000, ymin = 100000;
			float last = 1;
			int index = 0;
			for (int i = 0; i <= xx_val.size(); i++)
			{
				if (xx_val[i] >= j - 3 && xx_val[i] < j + 3 && zz_val[i] > 0)
				{
					divide_x.push_back(xx_val[i]);
					divide_y.push_back(yy_val[i]);
					divide_z.push_back(zz_val[i]);
					if (yy_val[i] < ymin) { ymin = yy_val[i]; }
					if (yy_val[i] > ymax) { ymax = yy_val[i]; }
				}
			}



			if (divide_y.size() > 10)//Noise removal
			{
				cv::Mat fmax = cv::Mat_<double>(divide_y.size(), 5);
				cv::Mat fz = cv::Mat_<double>(divide_y.size(), 2);
				for (int n = 0; n < divide_x.size(); n++)
				{
					fprintf(fa, "%d %d %d %d %d %d\n", divide_x[n], divide_y[n], divide_z[n], 100, 255, 255);
					fmax.at<double>(n, 0) = (double)divide_y[n] * divide_y[n];
					fmax.at<double>(n, 1) = (double)divide_y[n];
					fmax.at<double>(n, 2) = (double)last;
					fz.at<double>(n, 0) = (double)divide_z[n];
				}

				cv::Mat inverx; int index = 0;
				inverx = fmax.inv(cv::DECOMP_SVD);
				cv::Mat zresult = inverx*fz;
				double minz = 100000.000, valy;


				for (int i = ymin; i < ymax; i += 1)//Find the z value of the vertex on the estimate curve
				{
					double valz = zresult.at<double>(0, 0)*i*i + zresult.at<double>(1, 0)*i + zresult.at<double>(2, 0);
					if (valz < minz)
					{
						minz = valz;
						valy = i;
					}
				}



				//double mind = 1000000000.00;//Find the nearest position between the estimate curve and the section
				double mind = 10000000.00;
				for (int s = 0; s < divide_x.size(); s++)
				{
					double dist = sqrt(((valy - divide_y[s])*(valy - divide_y[s])) + ((minz - divide_z[s])*(minz - divide_z[s])));
					if (mind >= dist) { mind = dist; index = s; }
				}

				if (divide_z[index] < avgz + 20)
				{
					//store each top position
					fprintf(fb, "%d %d %d %d %d %d\n", divide_x[index], divide_y[index], divide_z[index], 0, 255, 0);

					//4Matrix
					xMat4.at<double>(ic, 0) = (double)divide_x[index] * divide_x[index] * divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 1) = (double)divide_x[index] * divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 2) = (double)divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 3) = (double)divide_x[index];
					xMat4.at<double>(ic, 4) = (double)1;

					//2Matrix
					xMat2.at<double>(ic, 0) = (double)divide_x[index] * divide_x[index];
					xMat2.at<double>(ic, 1) = (double)divide_x[index];
					xMat2.at<double>(ic, 2) = (double)1;
					//y is side view
					//z is top view
					yMat.at<double>(ic, 0) = (double)divide_y[index];
					zMat.at<double>(ic, 0) = (double)divide_z[index];

					xlength[ic] = divide_x[index];
					ic++;
				}
			}
		}




		for (int s = 0; s < ic; s++)
		{
			int val;
			//val = xMat4.at<double>(s, 3);
			val = xlength[s];
			if (curxmn >val) { curxmn = val; }
			if (curxmx < val) { curxmx = val; }
		}



		xMat4.resize(ic);
		xMat2.resize(ic);
		yMat.resize(ic);
		zMat.resize(ic);
		cv::Mat inverx4, inverx2;
		inverx4 = xMat4.inv(cv::DECOMP_SVD);
		inverx2 = xMat2.inv(cv::DECOMP_SVD);
		cv::Mat resultz = inverx4*zMat;
		cv::Mat resulty = inverx2*yMat;
		int tempx = 0, tempy = 0, tempz = 0;
		float distance = 0.0;



		for (float i = curxmn; i <curxmx; i++)//Find the length of the pig from head to tail
		{
			int yy = (int)i * i * resulty.at<double>(0, 0) + i * resulty.at<double>(1, 0) + resulty.at<double>(2, 0);
			int zz = (int)i * i * i * i * resultz.at<double>(0, 0) + i * i * i * resultz.at<double>(1, 0) + i * i * resultz.at<double>(2, 0) + i * resultz.at<double>(3, 0) + resultz.at<double>(4, 0);
			float u = ((i * 365) + (320 * zz)) / zz;
			float v = ((yy * 365) + (240 * zz)) / zz;
			//if (u >= k[2] && u <= k[3] && v >= k[0] && v <= k[1])
			{
				if (i > curxmn)
				{
					distance += sqrt(((i - tempx)*(i - tempx)) + ((yy - tempy)*(yy - tempy)) + ((zz - tempz)*(zz - tempz)));
					//distance=distance+√i^2+yy^2+zz^2
				}
				tempx = i; tempy = yy; tempz = zz;
				fprintf(fc, "%d %d %d %d %d %d\n", (int)i, yy, zz, 255, 255, 255);
			}
		}

		distance = distance / 10;
		int dis = (int)distance;
		std::ostringstream convert;
		convert << dis;
		std::string ss =  convert.str() + "kg";
		cv::putText(result, ss.c_str(), cv::Point((depthImage.cols - 1) / 2 - 15, (depthImage.rows - 1) / 2 - 15), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0), 1, CV_AA);
		cv::imshow("Weight Estimation", result); 
		cv::imwrite(s6, result);
		fclose(fa);
		fclose(fb);
		fclose(fc);
		UpdateData();
		CString aup;
		aup.Format(_T("%f"), distance);
		m_length = aup;
		UpdateData(FALSE);
		AfxMessageBox(_T("All results have been saved!"), MB_OK | MB_ICONINFORMATION);
		FreeConsole();
	}

	else
	{
		AfxMessageBox(_T("Firstly, please enter filename!"), MB_ICONSTOP);
	}
	imgC++;
	cv::destroyAllWindows();

	/*char AA[20];
	cv::Mat show(500,500,CV_8UC3,cv::Scalar(255,255,255));
	truLen *= 10;
	std::ostringstream convert;
	convert << truLen;
	std::string s3 = convert.str()+"KG";
	cv::putText(show, s3.c_str(), cv::Point(10, 250), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 0, 0), 1, CV_AA);
	cv::imshow("Weight Estimation",show);*/
}

void CPigSeparationDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void CPigSeparationDlg::OnBnClickedButton5()
{
	std::vector<int>xx_val;
	std::vector<int>yy_val;
	std::vector<int>zz_val;

	AllocConsole();
	FILE *fd, *fs, *ff;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
		
	CString filter("XYZ Files (coordinate*.xyz)|coordinate*.xyz");
	CFileDialog     selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	int x, y, z, r, g, b;
	FILE *fp;
	CString filepath, fn;

	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();
		fn = selDlg.GetFileName();

		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fp = fopen(csTempmb.GetBuffer(0), "r"); 												
	}



	CT2CA t(fn);
	std::string stream3(t);
	ff = fopen("data.txt", "wt");
	std::string s3 = stream3;
	s3 = "cross_section" + s3 + ".xyz";
	fs = fopen(s3.c_str(), "wt");


	int minx = 100000, maxx = -100000, minz = 100000, maxz = -100000;	int sumz = 0;
	int ic = 0;


	cv::Mat xMat4 = cv::Mat_<double>(1000, 5);//store nearest position on cross section,4power,top view
	cv::Mat xMat2 = cv::Mat_<double>(1000, 3);//store nearest position on cross section,2power,side view
	cv::Mat yMat = cv::Mat_<double>(1000, 1);
	cv::Mat zMat = cv::Mat_<double>(1000, 1);
	int *xlength = new int[100];
	int *ylength = new int[100];
	int *zlength = new int[100];


	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {
		xx_val.push_back(x);
		yy_val.push_back(y);
		zz_val.push_back(z);
		if (minx > x) { minx = x; }
		if (maxx < x) { maxx = x; }
		if (minz > z) { minz = z; }
		if (maxz < z) { maxz = z; }
		sumz += z;
	}


	int datasize1 = 0;
	int avgz = sumz / zz_val.size();
	int cccc = 0;

	for (int j = minx; j <= maxx; j += 2)//maxx
	{
		int seg_min = 100000;
		std::vector<int>csection_x;//store final cross section data
		std::vector<int>csection_y;
		std::vector<int>csection_z;
		std::vector<int>divide_x;//store raw cross section data
		std::vector<int>divide_y;
		std::vector<int>divide_z;

		int index = 0;
		for (int i = 0; i <= xx_val.size(); i++)
		{
			if (xx_val[i] < j && xx_val[i]>j - 3 && xx_val[i] < j + 3)//select 3 difference from vminx
			{
				divide_x.push_back(xx_val[i]);//xx_val[i]
				divide_y.push_back(yy_val[i]);
				divide_z.push_back(zz_val[i]);
			}
		}



		double ymax = -100000, ymin = 100000;
		float last = 1;
		for (int s = 0; s < divide_x.size(); s++)
		{
			if (divide_z[s] > avgz - 50 && divide_z[s] < avgz + 50)//to reduce some high noise
			{
				csection_x.push_back(divide_x[s]);
				csection_y.push_back(divide_y[s]);
				csection_z.push_back(divide_z[s]);
				if (divide_y[s] < ymin) { ymin = divide_y[s]; }
				if (divide_y[s] > ymax) { ymax = divide_y[s]; }
			}
		}


		if (divide_y.size() > 10)
		{
			cv::Mat fmax = cv::Mat_<double>(csection_y.size(), 3);//draw 2 power estimation curve to reduce digital error
			cv::Mat fz = cv::Mat_<double>(csection_y.size(), 1);

			for (int n = 0; n < csection_x.size(); n++)
			{
				//fprintf(fs, "%d %d %d %d %d %d\n", csection_x[n], csection_y[n], csection_z[n], 0, 0, 255);
				fmax.at<double>(n, 0) = (double)csection_y[n] * csection_y[n];
				fmax.at<double>(n, 1) = (double)csection_y[n];
				fmax.at<double>(n, 2) = (double)last;
				fz.at<double>(n, 0) = (double)csection_z[n];
			}


			cv::Mat inverx; int index = 0;
			inverx = fmax.inv(cv::DECOMP_SVD);
			cv::Mat zresult = inverx*fz;
			double minz = 100000.000, valy;
			cccc++;


			for (int i = ymin; i < ymax; i += 1)//find z value of top position on estimation curve
			{
				double valz = zresult.at<double>(0, 0)*i*i + zresult.at<double>(1, 0)*i + zresult.at<double>(2, 0);
				if (valz < minz)
				{
					minz = valz;
					valy = i;
				}
			}



			double mind = 10000.00;//find nearest position between estimaion curve and cross section
			for (int s = 0; s < csection_x.size(); s++)
			{
				double dist = sqrt(((valy - csection_y[s])*(valy - csection_y[s])) + ((minz - csection_z[s])*(minz - csection_z[s])));
				if (mind >= dist) { mind = dist; index = s; }
			}

			xMat4.at<double>(ic, 0) = (double)csection_x[index] * csection_x[index] * csection_x[index] * csection_x[index];
			xMat4.at<double>(ic, 1) = (double)csection_x[index] * csection_x[index] * csection_x[index];
			xMat4.at<double>(ic, 2) = (double)csection_x[index] * csection_x[index];
			xMat4.at<double>(ic, 3) = (double)csection_x[index];
			xMat4.at<double>(ic, 4) = (double)1;

			xMat2.at<double>(ic, 0) = (double)csection_x[index] * csection_x[index];
			xMat2.at<double>(ic, 1) = (double)csection_x[index];
			xMat2.at<double>(ic, 2) = (double)1;

			yMat.at<double>(ic, 0) = (double)csection_y[index];
			zMat.at<double>(ic, 0) = (double)csection_z[index];

			xlength[ic] = csection_x[index];
			ylength[ic] = csection_y[index];
			zlength[ic] = csection_z[index];


			ic++;
		}
	}



	int curxmx = -100000, curxmn = 100000;
	for (int s = 0; s < ic; s++)
	{
		int val;
		val = xlength[s];
		if (curxmn >val) { curxmn = val; }
		if (curxmx < val) { curxmx = val; }
	}



	xMat4.resize(ic);
	xMat2.resize(ic);
	yMat.resize(ic);
	zMat.resize(ic);
	cv::Mat inverx4, inverx2;
	inverx4 = xMat4.inv(cv::DECOMP_SVD);
	inverx2 = xMat2.inv(cv::DECOMP_SVD);
	cv::Mat resultz = inverx4*zMat;
	cv::Mat resulty = inverx2*yMat;

	int mini = -100000, maxi = 100000;
	for (float i = (float)curxmn; i <= curxmx; i += 1)
	{
		int yy = (int)i * i * resulty.at<double>(0, 0) + i * resulty.at<double>(1, 0) + resulty.at<double>(2, 0);
		int zz = (int)i * i * i * i * resultz.at<double>(0, 0) + i * i * i * resultz.at<double>(1, 0) + i * i * resultz.at<double>(2, 0) + i * resultz.at<double>(3, 0) + resultz.at<double>(4, 0);
		if (maxi < i) { maxi = (int)i; }if (mini>i) { mini = (int)i; }
	}


	for (int i = 0; i < xx_val.size(); i++)
	{
		if (xx_val[i] >= minx && yy_val[i] <= maxi)
		{
			fprintf(ff, "%d %d %d %d %d %d\n", xx_val[i], yy_val[i], zz_val[i], 255, 0, 255);
		}
	}


	AfxMessageBox(_T("All results have been saved!"), MB_OK | MB_ICONINFORMATION);
	fclose(ff);
	fclose(fs);
	FreeConsole();
}

void CPigSeparationDlg::OnBnClickedRadio1()
{
	AfxMessageBox(_T("Male is selected!"), MB_OK | MB_ICONINFORMATION);
	rbtControl = 1;
}

void CPigSeparationDlg::OnBnClickedRadio2()
{
	AfxMessageBox(_T("Female is selected!"), MB_OK | MB_ICONINFORMATION);
	rbtControl = 2;
}

void CPigSeparationDlg::OnBnClickedButton6()//////////////////Length Estimation////////////////////////
{
	std::vector<int>xx_val;
	std::vector<int>yy_val;
	std::vector<int>zz_val;

	AllocConsole();
	FILE *fd, *fs, *ff, *fa;
	//FILE *fw, *fq, *fg;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
	int x, y, z, r, g, b;
	FILE *fp, *fc;
	//FILE *fo;
	CString filepath, fn;
	CString  filter("XYZ Files (coordinate*.xyz)|coordinate*.xyz");
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	
	if (selDlg.DoModal() == IDOK)//About three dimensional data
	{
		filepath = selDlg.GetPathName();
		fn       = selDlg.GetFileName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fp = fopen(csTempmb.GetBuffer(0), "r");
	}



	CString filepathl;
	CString         filterl("Text Files (uv_*.txt)|uv_*.txt");
	CFileDialog     selDlgl(TRUE, NULL, NULL, OFN_HIDEREADONLY, filterl);

	if (selDlgl.DoModal() == IDOK)//About (u,v) data
	{
		filepathl = selDlgl.GetPathName();
		CStringA csTempmb(filepathl.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fc = fopen(csTempmb.GetBuffer(0), "r");
	}

	int k[4]; int c = 0;//stroe uv data in int k array
						
	while ((fscanf(fc, "%d", &x)) != EOF) {// Scan the file containing uv data
		std::cout << x << std::endl;
		k[c] = x; c++;
	}
	fclose(fc);

	CT2CA t(fn);
	std::string stream3(t);

	std::string s9 = stream3;
	s9 = "Data/length/data_" + s9 + ".txt";
	ff = fopen(s9.c_str(), "wt");
	std::string s3 = stream3;
	s3 = "Data/length/cross_section" + s3 + ".xyz";
	fs = fopen(s3.c_str(), "wt");
	std::string s4 = stream3;
	s4 = "Data/length/circleData_" + s4 + ".txt";
	fa = fopen(s4.c_str(), "wt");
	filename = s3;

	int minx = 100000, maxx = -100000, minz = 100000, maxz = -100000;	
	int sumz = 0;  
	int ic = 0;

	//http://opencv.jp/cookbook/opencv_mat.html
	cv::Mat xMat4 = cv::Mat_<double>(1000, 5);
	cv::Mat xMat2 = cv::Mat_<double>(1000, 3);
	cv::Mat yMat = cv::Mat_<double>(1000, 1);
	cv::Mat zMat = cv::Mat_<double>(1000, 1);
	int *xlength = new int[100];

	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {//Scan three-dimensional data
		xx_val.push_back(x);
		yy_val.push_back(y);
		zz_val.push_back(z);
		if (minx > x) { minx = x; }  
		if (maxx < x) { maxx = x; }  
		if (minz > z) { minz = z; } 
		if (maxz < z) { maxz = z; }  
		sumz += z;
	}

	int datasize1 = 0;
	int avgz = sumz / zz_val.size();//Average z coordinate
	std::cout << "Average z=" << avgz << std::endl;

	//Divide three-dimensional data
	for (int j = minx; j <= maxx; j += value)
	{

		int seg_min = 100000;
		std::vector<int>divide_x;
		std::vector<int>divide_y;
		std::vector<int>divide_z;
		double ymax = -100000, ymin = 100000;
		float last = 1;
		int index = 0;

		for (int i = 0; i <= xx_val.size(); i++)
		{
			if(xx_val[i] >= j - 3 && xx_val[i] < j + 3)//select 3 difference from vminx			
			{
				divide_x.push_back(xx_val[i]);
				divide_y.push_back(yy_val[i]);
				divide_z.push_back(zz_val[i]);

				if (yy_val[i] < ymin) { ymin = yy_val[i]; }
				if (yy_val[i] > ymax) { ymax = yy_val[i]; }
			}
		}

		if (divide_y.size() > 10)
		{
			cv::Mat fmax = cv::Mat_<double>(divide_y.size(), 3);// draw a quadratic curve and reduce digital error
			cv::Mat fz = cv::Mat_<double>(divide_y.size(), 1);
			
			for (int n = 0; n < divide_x.size(); n++)
			{
				//Save divided data to a file
				fprintf(fs, "%d %d %d %d %d %d\n", divide_x[n], divide_y[n], divide_z[n], 100, 255, 255);


				fmax.at<double>(n, 0) = (double)divide_y[n] * divide_y[n];
				fmax.at<double>(n, 1) = (double)divide_y[n];
				fmax.at<double>(n, 2) = (double)last;
				fz.at<double>(n, 0) = (double)divide_z[n];
			}


			if (divide_x.size() > 10)
			{
				cv::Mat inverx; 
				int index = 0;
				inverx = fmax.inv(cv::DECOMP_SVD);//Create inverse matrix	
				cv::Mat zresult = inverx*fz; 
				
				
				double minz = 100000.000;
				double valy;


				for (int i = ymin; i < ymax; i += 1)//Find the z value of the vertex on the estimate curve
				//for (int i = 0; i < divide_x.size(); i++)
				{
					double valz = zresult.at<double>(0, 0)*i*i + zresult.at<double>(1, 0)*i + zresult.at<double>(2, 0);
					if (valz < minz)
					{
						minz = valz;
						valy = i;


					}
				}

				double mind = 1000000000.00;//Find the nearest position between the estimate curve and the cross section
				for (int s = 0; s < divide_x.size(); s++)
				{
					double dist = sqrt(((valy - divide_y[s])*(valy - divide_y[s])) + ((minz - divide_z[s])*(minz - divide_z[s])));
					if (mind >= dist) { mind = dist; index = s; }
				}


				//Store each top position
				if (divide_z[index] >= avgz - 20 && divide_z[index] < avgz + 20)
				{
					fprintf(fa, "%d %d %d %d %d %d\n", divide_x[index], divide_y[index], divide_z[index], 0, 255, 0);
					//ic=0
					xMat4.at<double>(ic, 0) = (double)divide_x[index] * divide_x[index] * divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 1) = (double)divide_x[index] * divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 2) = (double)divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 3) = (double)divide_x[index];
					xMat4.at<double>(ic, 4) = (double)1;

					//std::cout << xMat4<< std::endl << std::endl << std::endl;

					xMat2.at<double>(ic, 0) = (double)divide_x[index] * divide_x[index];
					xMat2.at<double>(ic, 1) = (double)divide_x[index];
					xMat2.at<double>(ic, 2) = (double)1;

					yMat.at<double>(ic, 0) = (double)divide_y[index];
					zMat.at<double>(ic, 0) = (double)divide_z[index];

					xlength[ic] = divide_x[index];
					ic++;
				}
			}
		}
	}

	int curxmx = -100000;
	int curxmn = 100000;
	//int curxmn = 10;
	int curymx = -100000, curymn = 100000, curzmx = -100000, curzmn = 100000;
	for (int s = 0; s < ic; s++)
	{
		int val;
		//val = xMat4.at<double>(s, 3);
		val = xlength[s];
		if (curxmn >val) { curxmn = val; } //100000＞xlength  100000=xlength
		if (curxmx < val) { curxmx = val; } //-100000<xlength -100000=xlength
	}

	xMat4.resize(ic);
	xMat2.resize(ic);
	yMat.resize(ic);
	zMat.resize(ic);
	cv::Mat inverx4, inverx2;
	inverx4 = xMat4.inv(cv::DECOMP_SVD);//Create inverse matrix
	//std::cout << inverx4 << std::endl << std::endl << std::endl ;
	inverx2 = xMat2.inv(cv::DECOMP_SVD);
	cv::Mat resultz = inverx4*zMat;
	cv::Mat resulty = inverx2*yMat;
	int tempx = 0, tempy = 0, tempz = 0;//Initialization
	float distance = 0.0;

	//Find the length of the pig from head to tail
	for (float i = curxmn; i <curxmx; i++)
	{
		int yy = (int)i * i * resulty.at<double>(0, 0) + i * resulty.at<double>(1, 0) + resulty.at<double>(2, 0);
		int zz = (int)i * i * i * i * resultz.at<double>(0, 0) + i * i * i * resultz.at<double>(1, 0) + i * i * resultz.at<double>(2, 0) + i * resultz.at<double>(3, 0) + resultz.at<double>(4, 0);

		float u = ((i * 250) + (320 * zz)) / zz;
		float v = ((yy * 250) + (240 * zz)) / zz;
		//float u = ((i * 300) + (320 * zz)) / zz;
		//float v = ((yy * 300) + (240 * zz)) / zz;
		{
			if (i > curxmn)
			{
				distance += sqrt(((i - tempx)*(i - tempx)) + ((yy - tempy)*(yy - tempy)) + ((zz - tempz)*(zz - tempz)));
			}

			tempx = i;  //tempx：The x coordinate for drawing the body length line
			tempy = yy; //tempy：The y coordinate for drawing the body length line
			tempz = zz; //tempz：The z coordinate for drawing the body length line
											 			
			fprintf(ff, "%d %d %d %d %d %d\n", (int)i, yy, zz, 255, 0, 255);

		}
	}

	distance = distance / 10;
	
	truLen = distance;


	UpdateData();
	CString aup;
	aup.Format(_T("%f"), distance);
	//std::cout << "distance=" << distance << std::endl;
	
	m_length = aup;
	UpdateData(FALSE);

	AfxMessageBox(_T("All results have been saved!"), MB_OK | MB_ICONINFORMATION);
	fclose(fa);
	fclose(ff);
	fclose(fs);
	//fclose(fo);
	//fclose(fw);
	//fclose(fq);
	//fclose(fg);
	FreeConsole();
}

/*CString Estimate_Weight::getvalue()
{
return epwo;
std::cout << "d" << std::endl;
}*/



void CPigSeparationDlg::OnBnClickedButton7()
{
	std::vector<int>xx_val;//store file read data x
	std::vector<int>yy_val;//store file read data y
	std::vector<int>zz_val;//store file read data z


	AllocConsole();
	FILE *fd, *fs, *ff;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
	//int x, y, z, r, g, b;
	int x, y, z, r, g, b;
	FILE *fp, *fc;
	CString filepath, fn;
	CString         filter("XYZ Files (coordinate*.xyz)|coordinate*.xyz");
	CFileDialog     selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();
		fn = selDlg.GetFileName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fp = fopen(csTempmb.GetBuffer(0), "r");
	}

	fs = fopen("data.txt", "wt");
	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {
		xx_val.push_back(x);
		yy_val.push_back(y);
		zz_val.push_back(z);
	}




	cv::Mat fxyz = cv::Mat_<double>(50, 3);//draw 2 power estimation curve to reduce digital error
	cv::Mat minus = cv::Mat_<double>(50, 1);
	for (int i = 0; i < 50; i++)
	{
		int value = rand() % xx_val.size();
		fxyz.at<double>(i, 0) = (double)xx_val[value];
		fxyz.at<double>(i, 1) = (double)yy_val[value];
		fxyz.at<double>(i, 2) = (double)zz_val[value];
		minus.at<double>(i, 0) = (double)-1;
	}
	cv::Mat  inverse = fxyz.inv(cv::DECOMP_SVD);
	cv::Mat zresult = inverse*minus;
	std::cout << zresult << std::endl;


	float distance = 0.0;
	for (int i = 0; i < 200; i++)
	{
		int value = rand() % xx_val.size();
		float plane = abs(zresult.at<double>(0, 0)*xx_val[value] + zresult.at<double>(1, 0)*yy_val[value] + zresult.at<double>(2, 0)*zz_val[value] + 1);
		float sqroot = sqrt((zresult.at<double>(0, 0)*zresult.at<double>(0, 0)) + (zresult.at<double>(1, 0)*zresult.at<double>(1, 0)) + (zresult.at<double>(2, 0)*zresult.at<double>(2, 0)));
		fprintf(fs, "%d %d %d\n", xx_val[value], yy_val[value], zz_val[value]);
		distance += plane / sqroot;
	}
	std::cout << "Distance=" << distance / 200;


	fclose(fs);
	fclose(fp);
	//FreeConsole();
}

void CPigSeparationDlg::OnBnClickedButton8()
{
	std::vector<float>xx_val;//store file read data x
	std::vector<float>yy_val;//store file read data y
	std::vector<float>zz_val;//store file read data z


	AllocConsole();
	FILE *fd, *fs, *ff, *fv;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
	//int x, y, z, r, g, b;
	float x, y, z, r, g, b;
	FILE *fp, *fc;
	CString filepath, fn;
	CString         filter("XYZ Files (coordinate*.xyz)|coordinate*.xyz");
	CFileDialog     selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();
		fn = selDlg.GetFileName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fp = fopen(csTempmb.GetBuffer(0), "r");
	}

	fs = fopen("pSqr.txt", "wt");
	ff = fopen("coordinatevalue.txt", "wt");
	fv = fopen("random.txt", "wt");
	while ((fscanf(fp, "%f %f %f", &x, &y, &z)) != EOF) {
		if (z > 5)
		{
			xx_val.push_back(x);
			yy_val.push_back(y);
			zz_val.push_back(z);
			//fprintf(ff, "%f %f %f\n", x, y, z);
		}
	}




	cv::Mat fxyz = cv::Mat_<double>(50, 3);//draw 2 power estimation curve to reduce digital error
	cv::Mat minus = cv::Mat_<double>(50, 1);
	for (int i = 0; i < 50; i++)
	{
		int value = rand() % xx_val.size();
		fxyz.at<double>(i, 0) = (double)xx_val[value];
		fxyz.at<double>(i, 1) = (double)yy_val[value];
		fxyz.at<double>(i, 2) = (double)zz_val[value];
		minus.at<double>(i, 0) = (double)-1;
		fprintf(fv, "%f %f %f\n", xx_val[value], yy_val[value], zz_val[value]);

	}
	cv::Mat  inverse = fxyz.inv(cv::DECOMP_SVD);
	cv::Mat zresult = inverse*minus;
	std::cout << zresult << std::endl;


	float distance = 0.0;
	for (int i = 0; i < 200; i++)
	{
		int value = rand() % xx_val.size();
		float plane = abs(zresult.at<double>(0, 0)*xx_val[value] + zresult.at<double>(1, 0)*yy_val[value] + zresult.at<double>(2, 0)*zz_val[value] + 1);
		float sqroot = sqrt((zresult.at<double>(0, 0)*zresult.at<double>(0, 0)) + (zresult.at<double>(1, 0)*zresult.at<double>(1, 0)) + (zresult.at<double>(2, 0)*zresult.at<double>(2, 0)));
		fprintf(fs, "%f %f\n", plane, sqroot);
		fprintf(ff, "%f %f %f\n", xx_val[value], yy_val[value], zz_val[value]);
		distance += plane / sqroot;
	}
	std::cout << "distance=" << distance / 200;


	fclose(fs);
	fclose(ff);
	fclose(fv);
	fclose(fp);
	//FreeConsole();
}

void CPigSeparationDlg::OnBnClickedSmallbutton2()///////////Big Pig Separation//////////////////////
{
	AllocConsole();
	FILE *fv;
	freopen_s(&fv, "CONOUT$", "w", stdout);
	freopen_s(&fv, "CONIN$", "r", stdin);
	FILE *fd, *fp, *ff;


	int levelh = 100, levell = 300;
	//int levelh= 100, levell = 100;
	cv::Mat separate_img;
	cv::Mat measure_img, result;
	int x_coor, y_coor;
	UpdateData();//to get string from textbox
	CString str = m_control;
	CT2CA t(str);//conversion from CString to string
	std::string stream3(t);
	std::ostringstream convert;
	convert << imgC;
	std::string s3 = "b" + stream3 + convert.str();
	std::string figD = "Data/images/bigdata/depth" + s3 + ".png";
	std::string figR = "Data/images/bigdata/RGB" + s3 + ".png";
	UpdateData(FALSE);


	if (stream3.length() > 0)
	{
		cv::RotatedRect minRect;
		cv::RotatedRect minBEllipse;
		float xc, yc;
		int zcount = 100000;
		int bc = 0, a, b, u, v;
		int tmpStore;
		////////////////////////////////////////////////////////////////////////////////
		openni::OpenNI::initialize();

		openni::Device device;
		openni::VideoStream depthStream, colorStream;
		auto ret = device.open(openni::ANY_DEVICE);

		if (ret != openni::STATUS_OK) {

			throw std::runtime_error("");

		}
		depthStream.create(device, openni::SensorType::SENSOR_DEPTH);
		colorStream.create(device, openni::SensorType::SENSOR_COLOR);

		depthStream.start();
		colorStream.start();

		cv::Mat depthImage, colorImage;

		std::vector<openni::VideoStream*> streams;

		streams.push_back(&depthStream);
	


		/////////////////////////////////////////////////////////////////////////////
		while (1)
		{
			////////////////////////////////////////////////////////////////////////
			int changedIndex;
			openni::OpenNI::waitForAnyStream(&streams[0], streams.size(), &changedIndex);
			openni::VideoFrameRef depthFrame, colorFrame;

			depthStream.readFrame(&depthFrame);

			depthImage = cv::Mat(depthStream.getVideoMode().getResolutionY(),

				depthStream.getVideoMode().getResolutionX(),

				CV_16U, (char*)depthFrame.getData());


			//--------------
			colorStream.readFrame(&colorFrame);
			colorImage = cv::Mat(colorFrame.getHeight(),
				colorFrame.getWidth(),
				CV_8UC3, (unsigned char*)colorFrame.getData());
			///////////////////////////////////////////////////////////////////////

			//Get RGB image
		
			depthImage.convertTo(measure_img, CV_8U, 255.0 / 10000);//depth image grayscale (2)channels to color image (3)channels
			cv::cvtColor(measure_img, result, CV_GRAY2BGR);//
			cv::rectangle(result, cv::Rect((depthImage.cols - 1) / 2 - 10, (depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//get the center position
			depthImage.copyTo(separate_img);
			UINT16 d = depthImage.at<UINT16>((depthImage.rows - 1) / 2, (depthImage.cols - 1) / 2);// intensity of depth image
					
			//cv::line(result, cv::Point(0, depthImage.rows / 2), cv::Point(depthImage.cols, depthImage.rows / 2), cv::Scalar(0, 0, 255), 2);//get the center position
			//cv::line(result, cv::Point(depthImage.cols / 2, 0), cv::Point(depthImage.cols / 2, depthImage.rows), cv::Scalar(0, 0, 255), 2);//get the center position
																																							

			int ddd;
			int hd, ld, hdd, ldd;
			if (zcount == 100000)//take intensity value from center position
			{
				hd = d - levelh;
				ld = d + levell;
				hdd = hd * d;//to change millimeters to intensity
				ldd = ld * d;
				tmpStore = d;
			}
			else
			{
				ddd = zcount;
				hd = ddd - levelh;
				ld = ddd + levell;
				hdd = hd * d;//to change millimeters to intensity
				ldd = ld * d;
			}


			//store nearest intensity values from center
			int maxh = 100000;
			for (int y = 0; y < depthImage.rows; y++) {
				for (int x = 0; x < depthImage.cols; x++) {
					UINT16 d = depthImage.at<UINT16>(y, x);
					
					if (d > hd && d <= ld)
					{
						separate_img.at<UINT16>(y, x) = d;
					}
					else
					{
						separate_img.at<UINT16>(y, x) = 0;
					}
				}
			}

			cv::Mat binary = separate_img > 100;
			medianBlur(binary, binary, 31);
			//Morphological operation     //removal of noise
			cv::Mat element(9, 1, CV_8U, cv::Scalar(1));
			cv::Mat element1(5, 3, CV_8U, cv::Scalar(1));
			cv::Mat element3(1, 3, CV_8U, cv::Scalar(1));

			//cv::imshow("test5", binary);
			//cv::imwrite("binary.png", binary);

			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element);
			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element1, cv::Point(-1, -1), 1);
			std::vector<std::vector<cv::Point>>contours; // to stroe contours point
			std::vector<cv::Vec4i>hierarchy;
			cv::Canny(binary, binary, 200, 225, 3);// edge detection
			cv::imshow("canny", binary);
			cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);//find contours from canny edge result
			

			for (int i = 0; i < contours.size(); i++)
			{
				if (cv::contourArea(contours[i]) >= 100)
				{

					minRect = cv::minAreaRect(cv::Mat(contours[i]));


					cv::RotatedRect minEllipse;
					if (contours[i].size()>100)
					{
						minEllipse = cv::fitEllipse(cv::Mat(contours[i]));//change to ellipse
						float x = minEllipse.center.x;
						float y = minEllipse.center.y;
						float aa = minEllipse.size.width / 2;    // width >= height
						float bb = minEllipse.size.height / 2;

						//int dist = sqrt(((x - depthImage.cols / 2)*(x - depthImage.cols / 2)) + ((y - depthImage.rows / 2)*(y - depthImage.rows / 2)));

						//if (dist < mindist)
						if (x - bb <= depthImage.cols / 2 + 20 && x + bb >= depthImage.cols / 2 - 20 && y - aa <= depthImage.rows / 2 + 20 && y + aa >= depthImage.cols / 2 - 20)//square is in the center or not
						{
							//mindist = dist;
							xc = x; yc = y; bc = i; a = aa; b = bb;
							minBEllipse = cv::fitEllipse(cv::Mat(contours[bc]));
						}
					}
				}
			}


			


			if (bc > 0)
			{
				if (xc > 0 && xc <= depthImage.cols - 1 && yc > 0 && yc <= depthImage.rows - 1 && a>0 && b>0)
				{
					cv::ellipse(result, minBEllipse, cv::Scalar(0, 0, 255), 2, 8);//draw ellipse
					cv::circle(result, cv::Point(xc, yc), 3, cv::Scalar(0, 255, 0), CV_FILLED);//center of ellipse
					std::string s = "Data/bigseparation/coordinate_" + s3 + ".xyz";
					fp = fopen(s.c_str(), "wt");


					std::string ss = "Data/bigseparation/uv_" + s3 + ".txt";
					ff = fopen(ss.c_str(), "wt");
					fprintf(ff, "%d\n%d\n%d\n%d", (int)yc - a, (int)yc + a, (int)xc - b, (int)xc + b);
					fclose(ff);


					//store xyz values in file
					for (int x = xc - b; x < xc + b; x++)
					{
						for (int y = yc - a; y < yc + a; y++)
						{
							UINT16 d = separate_img.at<UINT16>(y, x);
							//int z_coor = (int)d * 4500 / 65535;		//get z coordinate
							if (d != 0)
							{
								x_coor = -1 * (x - 320)*d / 290;
								y_coor = (y - 240)*d / 290;
								if (d < maxh)
								{
									maxh = d;
									u = ((x_coor * 290) + (320 * d)) / d;
									v = ((y_coor * 290) + (240 * d)) / d;
								}
								fprintf(fp, "%d %d %d\n", x_coor, y_coor, d);
							}
						}
					}
					cv::circle(result, cv::Point(u, v), 3, cv::Scalar(255, 0, 255), CV_FILLED);
					fclose(fp);

					if (maxh != 100000 && d <= tmpStore + 3 && d >= tmpStore - 3)// && zcount>maxh)//center intensity change or  not
					{
						zcount = maxh;
					}

					else
					{
						zcount = 100000;
					}
				}
			}
			cv::imshow("Separate", separate_img);
			cv::imshow("result", result);
			if (cv::waitKey(30) >= 0) { break; }
		}
		cv::imwrite(figD, result);
		cv::imwrite(figR, colorImage);
	}

	else
	{
		AfxMessageBox(_T("Firstly, please enter filename!"), MB_ICONSTOP);
	}
	imgC++;
	cv::destroyAllWindows();
}



void CPigSeparationDlg::OnEnChangeEdit3()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}


void CPigSeparationDlg::OnEnChangeEdit1()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}
void CPigSeparationDlg::OnEnChangeEdit2()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}



void CPigSeparationDlg::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CPigSeparationDlg::OnStnClickedPig()
{
	// TODO: Add your control notification handler code here
}
