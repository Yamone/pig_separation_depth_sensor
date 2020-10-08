
// PigSeparationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PigSeparation.h"
#include "PigSeparationDlg.h"
#include "afxdialogex.h"
#include "NtKinect.h"
#include "Estimate_Weight.h"
#include <opencv2/opencv.hpp>    // OpenCVヘッダ
#include <opencv2/opencv.hpp>    // OpenCVヘッダ
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "math.h"
#pragma comment(lib,"Kinect20.lib") 
#pragma comment(lib,"opencv_world310.lib") 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

float truLen = 0;

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
		HBITMAP hp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), finder.GetFilePath(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		m_picture->ModifyStyle(0xf, SS_BITMAP, SWP_NOSIZE);
		m_picture->SetBitmap(hp);
	}



	CStatic * m_picture2 = (CStatic *)GetDlgItem(IDC_Picture2);
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
	GetDlgItem(IDC_PIG)->SetFont(mf, TRUE);

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
NtKinect kinect;
int rbtControl = 1;
int imgC = 0, imgc2 = 0;
std::string filename; //std::string:文字列クラス

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
	UpdateData();//テキストボックスから文字列を取得する
	CString str = m_control;
	CT2CA t(str);//CStringから文字列への変換
	std::string stream3(t);
	std::ostringstream convert;

	convert << imgC;
	std::string s3 = "s" + stream3 + convert.str();
	std::string figD = "Data/depth" + s3 + ".png";
	std::string figR = "Data/RGB" + s3 + ".png";
	UpdateData(FALSE);
	int chk = 1;

	/////セミナーレポート行列//////////////
	//std::cout <<"  "<< std::endl;
	//cv::Mat A = cv::Mat_<double>(2, 2);
	//A.at<double>(0, 0) = (double)1;
	//A.at<double>(1, 0) = (double)3;
	//A.at<double>(0, 1) = (double)2;
	//A.at<double>(1, 1) = (double)4;
	//std::cout << A << std::endl ;
	//std::cout << "  " << std::endl;
	//cv::Mat B = cv::Mat_<double>(2, 2);
	//B.at<double>(0, 0) = (double)3;
	//B.at<double>(1, 0) = (double)5;
	//B.at<double>(0, 1) = (double)1;
	//B.at<double>(1, 1) = (double)4;
	//std::cout << B << std::endl;
	//std::cout << "  " << std::endl;
	//cv::Mat C= cv::Mat_<double>(2, 2);
	//C=A/B;
	//std::cout << C << std::endl ;
	///////////////////////////////////////


	if (stream3.length() > 0)
	{
		cv::RotatedRect minRect; //RotatedRectは回転を考慮した矩形
								 //しかし、cv::RotatedRectには、中心と、サイズと、傾きが格納されているため、楕円を描画することができる
		cv::RotatedRect minEllipse;
		cv::RotatedRect minBEllipse;
		float xc, yc;
		int zcount = 100000;
		int bc = 1, a, b, u, v;
		int tmpStore;
		while (1)
		{
			//深度画像depthImageを取得する（ボタン1ではプログラムではカラー画像は用いていない）
			kinect.setDepth(false);//depth画像を初期化する
								   //cv::imshow("Depth", kinect.depthImage);

								   //kinect.setRGB();//RGB画像を取得する
								   //cv::imshow("RGB", kinect.rgbImage);//RGB画像を表示する

			kinect.depthImage.convertTo(measure_img, CV_8UC1, 25.0 / 4500);//depth image grayscale (2)channels to color image (3)channels
																		   //この指示で行列のデータを別の型に変換します
																		   //元行列のピクセル値を目的のデータ型に変換します
																		   //CV_8UC1：8ビット、符号なし、１チャンネルのデータつまり１個のunsigned char型のデータタイプを示す定数
																		   //CV_8UC1：出力行列に要求する型，（チャンネル数は元の行列と同じなので）正確には，要求するビット深度
																		   //25.0 / 4500：

			cv::cvtColor(measure_img, result, CV_GRAY2BGR);//measure_imgつまりdepthimageは深度画像であるため、2ビットグレースケール画像であり、RGBにしても変化は起きない
														   //しかし、RGB画像に変換させておく理由は、その後中心位置の取得の際の赤い四角を描いたり、緑やピンクの点を表示させる際、カラーを用いるから



														   //cv::imwrite("depth005.png", result); //depth画像取得　Pigseparetion中のファイルに入る



			cv::rectangle(result, cv::Rect((kinect.depthImage.cols - 1) / 2 - 10, (kinect.depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//中心位置を取得する 赤い四角を描く


																																							//コピーする理由：高輝度と低輝度を使用し、ピクセル値がこれらの値の間にある画像を分離するため。つまり、豚のトップポジションの誤差±100の部分とそうでない部分を分離させるためにコピーする
																																							//このコピーされた画像は後に、二値化画像として用いられ、しきい値10を適用する
			kinect.depthImage.copyTo(separate_img);//コピーされた画像separate_img


			UINT16 d = kinect.depthImage.at<UINT16>((kinect.depthImage.rows - 1) / 2, (kinect.depthImage.cols - 1) / 2); // unit16 16ビット符号なし整数
																														 //intensity of depth image "depth image"の強度
																														 //rows：行列の行数（画像の高さ）を参照するMatクラスのメンバー変数
																														 //cols：行列の列数（画像の幅）を参照するMatクラスのメンバー変数

																														 //トラックバー作成　HIntensityとLIntensity　
																														 //cv::createTrackbar("HIntensity", "separate", &levelh, 800);//to control highest intensity 最高強度を制御する
																														 //cv::createTrackbar("LIntensity", "separate", &levell, 800);//to control lowest intensity 最低強度を制御する

																														 //二値化画像作成
																														 //cv::Mat bin = separate_img > 255;
																														 //cv::rectangle(bin, cv::Rect((kinect.depthImage.cols - 1) / 2 - 10, (kinect.depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(255, 255, 255), 2);//中心位置を取得する 赤い四角を描く
																														 //cv::imshow("binary", bin);

			int val = (int)d * 4500 / 65535; //強度値をミリメートル値に変更（カメラ動作中常に起動）
											 //深度画像では２ビットのグレースケール画像を持つ
											 //65535：最高の白のコード256×256
											 //4500：kinectがmm単位で測定できる最高距離

			int ddd;
			int hd, ld, hdd, ldd;
			if (zcount == 100000)
			{
				ddd = (int)d * 4500 / 65535;//強度値をミリメートル値に変更
				hd = ddd - levelh; //hd　=　kinectと中心（赤い四角）までの距離　－　100  hdはトップポジション（mm）
				ld = ddd + levell; //ld　=　kinectと中心（赤い四角）までの距離　＋　100　ldはダウンポジション（mm）
				hdd = hd * 65535 / 4500;//トップポジションのミリメートルを強度に変える  hddはトップポジション（強度）
				ldd = ld * 65535 / 4500;//ダウンポジションのミリメートルを強度に変える  lddはダウンポジション（強度）
			}
			else
			{
				ddd = zcount;
				hd = ddd - levelh;
				ld = ddd + levell;
				hdd = hd * 65535 / 4500;//ミリメートルを強度に変える
				ldd = ld * 65535 / 4500;
			}


			//中心から最も近い強度値を保存する　コピーの処理を行っている？
			int maxh = 100000;
			for (int y = 0; y < kinect.depthImage.rows; y++) { //yが0から画像の高さ分までの間にy++
				for (int x = 0; x < kinect.depthImage.cols; x++) { //xが0から画像の横幅分までの間にx++
					UINT16 d = kinect.depthImage.at<UINT16>(y, x); //画像のすべての高さ、すべての横幅（すべてのピクセル）において強度値を求める
					int dd = (int)d * 4500 / 65535; //強度値をミリメートル値に変更

					if (dd > hd && dd <= ld)//「kinectと豚の背中までの距離」が「kinectと中心（赤い四角）までの距離－100」より大きく、「kinectと中心（赤い四角）までの距離+100」以下であったら
											//豚の背中をうまく取得できた場合
					{
						separate_img.at<UINT16>(y, x) = d;//強度値を保存
					}
					else//豚の背中以外を取得した場合
					{
						separate_img.at<UINT16>(y, x) = 0;
					}
				}
			}


			cv::Mat binary = separate_img > 10; //画像を二値化 しきい値は10
												//cv::imshow("binary", binary);// 単なる二値化画像







												////////////////////////////////////////////////////////////////
												//膨張//
												//cv::imshow("test1", binary);
												//cv::dilate(binary,binary, cv::Mat(), cv::Point(-1, -1), 1); //8近傍で膨張処理1回
												//cv::imshow("test2", binary);
												///////////////////////////////////////////////////////////////

												////////////////////////////////////////////////////////////////
												//縮小//
												//cv::imshow("test3", binary);
												//cv::erode(binary, binary, cv::Mat(), cv::Point(-1, -1), 1); //8近傍で縮小処理1回
												//cv::imshow("test4", binary);
												///////////////////////////////////////////////////////////////




												//モルフォロジー演算、ノイズの除去
			cv::Mat element(9, 1, CV_8U, cv::Scalar(1)); //element： 構造要素
			cv::Mat element1(3, 3, CV_8U, cv::Scalar(1));
			cv::Mat element3(0, 3, CV_8U, cv::Scalar(1));

			//cv::imshow("test5", binary);
			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element);//morphologyEx:収縮と膨張を基本演算として利用する高度なモルフォロジー変換を行います
																	  //cv::MORPH_OPEN モルフォロジー演算の種類　オープニング
																	  //オープニングの計算　出力＝open（入力,構造要素）＝dilate(erode(入力,構造要素))
																	  //element： 構造要素
																	  //cv::imshow("test6", binary);  //白い部分、黒い部分がそれぞれ広くなった二値化画像

			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element1, cv::Point(-1, -1), 1);
			//cv::imshow("test7", binary); //より白い部分、黒い部分がそれぞれ広くなった二値化画像

			std::vector<std::vector<cv::Point>>contours;//std::vector:各要素は線形に、順序を保ったまま格納される
														//vectorコンテナは可変長配列として実装される。自動的に領域の拡張が行われる。
			std::vector<cv::Vec4i>hierarchy;

			//cv::imshow("test8", binary);
			cv::Canny(binary, binary, 5, 225, 3); //画像のエッジを検出
												  //5:ヒステリシスが存在する処理の1番目の閾値
												  //225:ヒステリシスが存在する処理の2番目の閾値
												  //3:Sobel() オペレータのアパーチャサイズ
												  //cv::imshow("test9", binary); //白い部分の輪郭を抽出した二値化画像

			cv::findContours(binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			//cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//cv::findContours:2値画像中の輪郭を検出します
			//binary:入力画像
			//contours:検出された輪郭
			//hierarchy:オプション．画像のトポロジーに関する情報を含む出力ベクトル．これは，輪郭数と同じ数の要素を持ちます
			//CV_RETR_TREE:すべての輪郭を抽出し，入れ子構造になった輪郭を完全に表現する階層構造を構成します

			//cv::imshow("Contours", binary);		//輪郭線																					 //CV_CHAIN_APPROX_SIMPLE:水平・垂直・斜めの線分を圧縮し，それらの端点のみを残します．例えば，まっすぐな矩形の輪郭線は，4つの点にエンコードされます．
			//cv::imshow("binary4", binary); 白い部分の輪郭をより細くした二値化画像

			//int mindist = 100000;




			for (int i = 0; i < contours.size(); i++)
			{
				if (cv::contourArea(contours[i]) >= 100) //cv::contourArea:輪郭が囲む領域の面積を求めます(輪郭の頂点)
				{



					minRect = cv::minAreaRect(cv::Mat(contours[i])); //minAreaRect:与えられた 2次元点集合を囲む（回転を考慮した）最小の矩形を求めます
																	 //minRectは、豚の場所を確認し、画像から輪郭ベクトル（u,v）を決定するために使用
																	 //contours：輪郭線（等高線）



																	 //cv::minEnclosingCircle    //与えられた2次元点集合を囲む最小の円を求めます





					if (contours[i].size()>100)
					{
						minEllipse = cv::fitEllipse(cv::Mat(contours[i]));//cv::fitEllipse:回転矩形を楕円に変換する処理
																		  //fitEllipse：2次元の点集合に楕円をフィッティングします
																		  //minEllipse：この楕円をフィッティングされた回転矩形
																		  //contours：輪郭線（等高線）








						float x = minEllipse.center.x; //楕円中心のx座標
						float y = minEllipse.center.y; //楕円中心のy座標
						float aa = minEllipse.size.width / 2;    // width >= height
						float bb = minEllipse.size.height / 2;

						//int dist = sqrt(((x - kinect.depthImage.cols / 2)*(x - kinect.depthImage.cols / 2)) + ((y - kinect.depthImage.rows / 2)*(y - kinect.depthImage.rows / 2)));//この部分はなくてもOK
						//dist=√{(x-depthImageの幅)/2の二乗　＋　(y-depthImageの高さ)/2の二乗 }


						//if (dist < mindist) // mindist=100000
						if (x - bb <= kinect.depthImage.cols / 2 + 20 && x + bb >= kinect.depthImage.cols / 2 - 20 && y - aa <= kinect.depthImage.rows / 2 + 20 && y + aa >= kinect.depthImage.cols / 2 - 20)//正方形が中央にあるかどうか
						{
							//mindist = dist;　　//mindist:画像の中心と楕円の中心までの距離//この部分はなくてもOK
							xc = x; yc = y; bc = i; a = aa; b = bb;					//xc:楕円中心のx座標
																					//yc:楕円中心のy座標
																					//bc: i = 0からcontours（輪郭線、等高線）の本数分、繰り返し代入
																					//a:楕円長径の半分
																					//b:楕円短径の半分


																					//fitEllipseは回転矩形から楕円に楕円を描画するために使用されます。
							minBEllipse = cv::fitEllipse(cv::Mat(contours[i]));//fitEllipse：2次元の点集合に楕円をフィッティングします
																			   //minBEllipse：この楕円をフィッティングされた回転矩形




																			   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																			   //cv::Mat circles;     double param1 = 200; double param2 = 100; int minRadius = 0; int maxRadius = 10000000;
																			   //cv::HoughCircles(result, circles, CV_HOUGH_GRADIENT, 1, param1, param2, minRadius, maxRadius);
																			   //std::string a = "Data/circles_" + s3 + ".xyradius";//string:char型文字列
																			   //fp = fopen(a.c_str(), "wt");
																			   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																			   //cv::Point center(cv::contourArea(contours[i][0]), cvRound(circles[i][1]));
																			   //int radius = cv::contourArea(contours[i][2]);
																			   // 円の中心を描画します．
																			   //cv::circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
																			   // 円を描画します．
																			   //cv::circle(result, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);




						}
					}
				}
			}




			if (bc > 0 && a>0 && b>0)
			{
				cv::ellipse(result, minBEllipse, cv::Scalar(0, 0, 255), 2, 8);//楕円の描画
																			  //reslut：画像　
																			  //minBEllipse：2次元の点集合でフィッティングしておいた楕円
																			  //cv::Scalar(0, 0, 255):画像の色
																			  //2:楕円の枠線の太さ
																			  //8:楕円の枠線の種類

				cv::circle(result, cv::Point(xc, yc), 3, cv::Scalar(0, 255, 0), CV_FILLED);//緑の点（円）の描画
																						   //cv::Point(xc, yc)：赤い楕円の中心に来るよう指示
																						   //3：円の半径
																						   //cv::Scalar(0, 0, 255):点の色（緑）
																						   //CV_FILLED:

				std::string s = "Data/coordinate_" + s3 + ".xyz";//string:char型文字列
				fp = fopen(s.c_str(), "wt");


				std::string ss = "Data/uv_" + s3 + ".txt";
				ff = fopen(ss.c_str(), "wt");//ファイル書き込み用　オープン
				fprintf(ff, "%d\n %d\n %d\n %d", (int)yc - a, (int)yc + a, (int)xc - b, (int)xc + b);
				fclose(ff);
				for (int x = xc - b; x < xc + b; x++)
				{
					for (int y = yc - a; y < yc + a; y++)
					{
						UINT16 d = separate_img.at<UINT16>(y, x);//画像の (x , y ) 座標の画素にアクセスする <UNIT16>(y,x)でxとyが反対だけどこれでOK

						int z_coor = (int)d * 4500 / 65535;		//z座標を求める

						if (z_coor != 0)//z座標からx座標、y座標を求める
						{
							x_coor = (x - 512 / 2)*z_coor / 365; //x_coor:x座標
							y_coor = (y - 424 / 2)*z_coor / 365; //y_coor:y座標
																 //512px×424pxの解像度
																 //http://nw.tsuda.ac.jp/lec/kinect2/KinectV2_depth/
																 //http://nextsystemkinectblog.seesaa.net/article/360212446.html
																 //http://visitlab.jp/pdf/FastAccurateCalibrationKinect.pdf

																 //printf("X Y Z( %d , %d , %d )\n", x_coor, y_coor, z_coor);



							if (z_coor < maxh) //maxh = 100000
							{
								maxh = z_coor;
								//(u,v)は豚の背中のなかのトップポジションの座標
								//この(u,v)座標を用いて、常に豚のトップポジションを探し、ピンクの点が打てるようにする
								u = ((x_coor * 365) + (256 * z_coor)) / z_coor;
								v = ((y_coor * 365) + (212 * z_coor)) / z_coor;
							}
							fprintf(fp, "%d %d %d\n", x_coor, y_coor, z_coor); //ファイル書き込み（x,y,z）データ
						}
					}
				}
				cv::circle(result, cv::Point(u, v), 3, cv::Scalar(255, 0, 255), CV_FILLED);//ピンクの点（円）の描画
																						   //cv::Point(u, v)：楕円（豚の背中）の中で最も高い位置（豚を横から見た時最も高くなる箇所）にピンクの点が来るよう指示
																						   //3：円の半径
																						   //cv::Scalar(0, 0, 255):点の色（ピンク）
																						   //CV_FILLED:
				fclose(fp);//ファイル閉じる
						   //ここで、ピンクの点が豚のトップポジションをとっているかチェックしている。
				if (maxh != 100000 && val <= tmpStore + 3 && val >= tmpStore - 3)// && zcount>maxh)
				{
					zcount = maxh;
				}
				else
				{
					zcount = 100000;
				}
			}
			cv::imwrite(figD, kinect.depthImage);//cv::imwrite:指定したファイルに画像を保存します
												 //figD:ファイル名(png形式)
												 //kinect.depthImage:保存される画像

			cv::imwrite(figR, kinect.rgbImage);//kinect.rgbImage:RGBカメラ画像
											   //figR:ファイル名(png形式)
											   //kinect.rgbImage:保存される画像

			cv::imshow("Separate", separate_img);
			cv::imshow("result", result);
			cv::Canny(binary, binary, 200, 225, 3);// edge detection
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
		value = m_sliderCtrl.GetPos(); //間隔を変える
		m_controlText.Format(_T("%d"), value);
		UpdateData(FALSE);
	}
	else {
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}















int threshold = 1;
#define Pi 3.14
void CPigSeparationDlg::OnBnClickedButton3()////////////////Circle Estimation////////////////////////////////////
{
	std::vector<int>xx_val; //vector: 各要素は線形に順序を保ったまま格納される。vectorの各要素は連続して配置されるため要素のランダムアクセスが可能
	std::vector<int>yy_val; //yy_valという名前の箱
	std::vector<int>zz_val;
	std::vector<int>estimate_x; //estimate_xという名前の箱
	std::vector<int>estimate_y;
	std::vector<int>estimate_z;
	std::vector<int>en_x; //estimate_xという名前の箱
	std::vector<int>en_y;
	std::vector<int>en_z;

	AllocConsole();
	FILE* fd, *fs, *fa;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
	int x, y, z, r, g, b;




	FILE *fp;
	CString filepath, fn;
	CString         filter("XYZ Files (coordinate_*.xyz)|coordinate_*.xyz");//座標ファイルを読み込む
	CFileDialog     selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();
		fn = selDlg.GetFileName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0); //string≒vextor各要素は線形に順序を保ったまま格納される。
		fp = fopen(csTempmb.GetBuffer(0), "r");////読み込み用としてファイルをオープン
	}

	FILE *ff;
	CString         filter1("TEXT Files (circleData_*.txt)|circleData_*.txt");//各断面の最高位置を読み取る
	CFileDialog     selDlg1(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter1);
	if (selDlg1.DoModal() == IDOK)
	{
		filepath = selDlg1.GetPathName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		ff = fopen(csTempmb.GetBuffer(0), "r");//読み込み用としてファイルをオープン
	}







	CT2CA t(fn);
	std::string stream3(t); //string≒vextor各要素は線形に順序を保ったまま格納される。
	std::string s3 = stream3;
	s3 = "Data/circle_" + s3 + ".txt"; //s3=Dataの中のcircle_.txtのこと
	fs = fopen(s3.c_str(), "wt"); //書き込み用としてファイルをオープン

								  /////////////////////ファイルの準備///////////////////////////////////////////////////////



	int minx = 100000, maxx = -100000;	int sumz = 0;
	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {//fpファイルの中身をスキャン
		xx_val.push_back(x);//xx_valの箱にxを追加する
		yy_val.push_back(y);
		zz_val.push_back(z);
		if (minx > x) { minx = x; } // 100000>xならばxをminxに代入
		if (maxx < x) { maxx = x; } //-100000<xならばxをmaxxに代入
									//fprintf(fs, "%d %d %d %d %d %d\n", x, y, z);
	}

	while ((fscanf(ff, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF)//ffファイルの中身をスキャン
	{
		estimate_x.push_back(x);//estimate_xの箱にxを追加する
		estimate_y.push_back(y);
		estimate_z.push_back(z);
	}
	/////////////////ファイルから新しい箱に必要な値を格納/////////////////////////////////////



	float sum = 0, count = 0;
	for (int j = minx; j <= maxx; j += value)//100000<j<-100000　この区間の時j=j+30
	{
		std::vector<int>divide_x; //vector: 各要素は線形に順序を保ったまま格納される。vectorの各要素は連続して配置されるため要素のランダムアクセスが可能
		std::vector<int>divide_y;
		std::vector<int>divide_z;
		int index = 0;

		for (int i = 0; i <= xx_val.size(); i++)//0<i<xx_valの箱の中にある値までi++
		{
			if (xx_val[i] > j - 3 && xx_val[i] < j + 3 && xx_val[i] <= 600)// && yy_val[i]>-220 && zz_val[i]>30)		//j値から距離3を取る（x座標分割）// 5は150 OK
			{
				divide_x.push_back(xx_val[i]);//xx_val[i]
				divide_y.push_back(yy_val[i]);
				divide_z.push_back(zz_val[i]);
			}
		}

		float bestr = 0; //bestな半径を0とする
		float bestx = 0;//float型　少数を入れる箱
		float besty = 0;
		float bestcount = 0;
		float last = 1;

		if (divide_x.size() > 10)//divide_xの箱の中にある値が10より大きい時
		{
			for (int p = 0; p < 100; p++)//0<p<100までp++
			{
				int* arr = new int[5];//int型の配列arr（箱は5こ）を設定
				for (int s = 0; s < 5; s++)
				{
					int random = rand() % divide_x.size();// rand() は区間 (0,1) の一様分布した乱数を1つdivide_xの箱の中に返します。
														  //円のランダムな点を得る
					arr[s] = random;//配列5個の箱の中にランダムに取り出した点の値代入
				}


				cv::Mat cirMat = cv::Mat_<double>(5, 3);
				cv::Mat cmMat = cv::Mat_<double>(5, 1);

				for (int n = 0; n < 5; n++)//円のパラメータを見つける
										   //ここでとりあえず5つのランダムなデータを円の方程式に代入
				{
					cirMat.at<double>(n, 0) = (float)divide_y[arr[n]]; //ここでランダムに取り出した5つのx座標の(y,z)座標をcirMatに代入する。
					cirMat.at<double>(n, 1) = (float)divide_z[arr[n]];
					cirMat.at<double>(n, 2) = (float)last;
					cmMat.at<double>(n, 0) = (float)(-(divide_y[arr[n]] * divide_y[arr[n]]) - (divide_z[arr[n]] * divide_z[arr[n]]));
				}

				//std::cout << cirMat << std::endl << std::endl ;
				//std::cout << cmMat << std::endl << std::endl;


				cv::Mat inverx;//(3×5行列)
				inverx = cirMat.inv(cv::DECOMP_SVD);
				//std::cout << inverx << std::endl << std::endl << std::endl << std::endl;
				cv::Mat cresult = inverx*cmMat;//（3×1行列）

											   //ここで、仮のcresult行列つまり円のパラメータが決まる。
				double a = cresult.at<double>(0, 0);
				double b = cresult.at<double>(1, 0);
				double c = cresult.at<double>(2, 0);
				//printf("%d %d %d \n", a, b, c);

				double x1, y1, r; //円について中心座標(x1,y1)半径ｒの円を表す
				x1 = -a / 2;//xの位置
				y1 = -b / 2;//yの位置
				r = sqrt(pow(x1, 2) + pow(y1, 2) - c);//半径＝√(x1^2 + y1^2 - c)

				if (r > 200)continue;
				double d;
				int cou = 0;


				for (int z = 0; z < divide_x.size(); z++)//0<z<divide_xの箱の中にある値　z++
				{
					d = fabs(sqrt(pow(divide_y[z] - x1, 2) + pow(divide_z[z] - y1, 2)) - r);//チェックプロセス
																							//fabs=絶対値
																							//√((divide_y[z]-x1)^2 + (divide_z[z]-y1^2)）-r
					if (d < threshold)//もしd<1ならば
					{
						cou++; //couに１だけ足す。つまりカウントされていく。
					}
				}


				if (cou > bestcount)//もしcou>0ならば
									//エラー半径の値が小さくなる
				{
					//円を描くためのデータを最終決定
					bestr = r;
					bestx = x1;
					besty = y1;
					bestcount = cou;
				}
			}

			double surf_area = (float)Pi*bestr*bestr;//表面積を見つける
													 //std::cout << bestr << std::endl;
			sum += surf_area;
			count++;
			int c = 0;
			for (float m = -4; m <= 4; m += 0.1)//円を描きファイルに保存する
			{
				//ダゴン
				float cx = bestx + (bestr*sin(m));
				float cy = besty + (bestr*cos(m));
				//軸の向きを確認横軸x、縦軸yで合ってる？
				//float cx = bestx + (bestr*cos(m)); //円の中心x座標+弦（y座標）
				//float cy = besty + (bestr*sin(m)); //円の中心y座標+接弦(x座標)
				fprintf(fs, "%d %f %f\n", j - 3, cx, cy);//j-3=x座標
				printf("%d %f %f\n", j - 3, cx, cy);

			}
		}
	}

	fclose(fs);
	fclose(ff);



	//FILE *fu;
	//CString  filter2("TEXT Files (circle_*.txt)|circle_*.txt");
	//CFileDialog     selDlg2(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter2);
	//if (selDlg2.DoModal() == IDOK)
	//{
	//filepath = selDlg2.GetPathName();
	//CStringA csTempmb(filepath.GetBuffer(0));
	//std::string filename = csTempmb.GetBuffer(0);
	//fu = fopen(csTempmb.GetBuffer(0), "r");//読み込み用としてファイルをオープン
	//}

	//int enx;
	//printf("x座標は？ \n");
	//scanf("%d", &enx);


	//while ((fscanf(fu, "%d %lf %lf ",&x ,&y, &z)) != EOF)//ffファイルの中身をスキャン
	//ffファイルの%lf部分のyとzが正しくスキャンされていない
	//{
	//printf("%d %f %f\n",x, y, z);
	//en_x.push_back(x);//estimate_xの箱にxを追加する
	//en_y.push_back(y);
	//en_z.push_back(z);	
	//if (enx == x) {
	//printf("%d %f %f\n", x, y, z);
	//printf("test2");
	//}
	//}

	//printf("%d\n", x);
	//printf("%f\n",z);
	//printf("%d %f %f\n", x, y, z);

	//if (j-3== enx) {
	//printf("%dを見つけた",enx);

	//}






	//printf("finish");
}


//while ((fscanf(fs, "%d %f %f", &x, &y, &z)) != EOF)//ffファイルの中身をスキャン
//{	
//printf("%d\n", &x);
//en_x.push_back(x);//estimate_xの箱にxを追加する
//en_y.push_back(y);
//en_z.push_back(z);
//printf("%d\n", &x);

//if (enx == x) {
//printf("入力した整数は%dです。\n", enx);
//printf("%d %f %f\n", en_x.size(), en_y.size(), en_z.size());
//}

//}

//サーチ方法
//http://blog.systemjp.net/entry/20100519/p3	
//vector<long> v_;
//v_list.push_back(100);
//v_list.push_back(200);
//v_list.push_back(300);
//vector< long >::iterator cIter = find(v_.begin(), v_.end(), 200);
//if (cIter != v_.end()) {
//printf("200は存在してます");
//}


//std::cout << sum / count << std::endl;
//FreeConsole();






















void CPigSeparationDlg::OnBnClickedButton4()///////////////Weight Estimation//////////////////////
{
	AllocConsole();
	FILE *fv;
	freopen_s(&fv, "CONOUT$", "w", stdout);
	freopen_s(&fv, "CONIN$", "r", stdin);

	//★時間をファイル名に入れる
	char date[64];
	time_t timer = time(NULL);
	strftime(date, sizeof(date), "%Y%m%d_%H%M%S", localtime(&timer));//時間を文字列として書式設定する
																	 /* 現在時刻の取得 */
	time(&timer);
	std::stringstream stream5;
	stream5 << "Data/coordinate_" << date << ".xyz";
	std::string s5 = stream5.str();

	std::stringstream stream6;
	stream6 << "Data/depth_" << date << ".png";
	std::string s6 = stream6.str();

	std::stringstream stream7;
	stream7 << "Data/RGB_" << date << ".png";
	std::string s7 = stream7.str();

	std::stringstream stream8;
	stream8 << "Data/uv_" << date << ".txt";
	std::string s8 = stream8.str();
	//★





	FILE *fp, *ff, *fa, *fb, *fc;

	int levelh = 100, levell = 200;
	cv::Mat separate_img;
	cv::Mat measure_img, result;
	int x_coor, y_coor;
	UpdateData();//テキストボックスから文字列を取得
	CString str = m_control;
	CT2CA t(str);//CStringから文字列への変換
	std::string stream3(t);
	std::ostringstream convert;
	convert << imgC;
	std::string s3 = "b" + stream3 + convert.str();
	//std::string figD = "Data/depth" + s3 + ".png";
	//std::string figR = "Data/RGB" + s3 + ".png";
	UpdateData(FALSE);
	//std::string s, ss;
	std::string s1, s2;


	std::vector<int>xx_val;//ファイル読み込みデータを格納するx
	std::vector<int>yy_val;// ファイル読み込みデータを格納するy
	std::vector<int>zz_val;//ファイル読み込みデータを格納するz
	int ue, ve, he, we;




	if (stream3.length() > 0)
	{
		/*s1 = "Data/cross_section" + s3 + ".xyz";
		fa = fopen(s1.c_str(), "wt");
		s2 = "Data / circleData_" + s3 + ".xyz";
		fb = fopen(s2.c_str(), "wt");
		fc = fopen("Data/data.txt", "wt");*/
		fa = fopen("Data/a.txt", "wt");
		fb = fopen("Data/b.txt", "wt");
		fc = fopen("Data/c.txt", "wt");
		int c = 0;
		cv::RotatedRect minRect;
		cv::RotatedRect minBEllipse;
		float xc, yc;
		int zcount = 100000;
		int bc = 0, a, b, u, v;
		int tmpStore;
		while (1)
		{
			kinect.setRGB();
			kinect.setDepth(false);//initialize depth image			 
			kinect.depthImage.convertTo(measure_img, CV_8UC1, 25.0 / 4500);//depth image grayscale (2)channels to color image (3)channels
			cv::cvtColor(measure_img, result, CV_GRAY2BGR);//
			cv::rectangle(result, cv::Rect((kinect.depthImage.cols - 1) / 2 - 10, (kinect.depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//get the center position
			kinect.depthImage.copyTo(separate_img);
			UINT16 d = kinect.depthImage.at<UINT16>((kinect.depthImage.rows - 1) / 2, (kinect.depthImage.cols - 1) / 2);// intensity of depth image
			int val = (int)d * 4500 / 65535;


			int ddd;
			int hd, ld, hdd, ldd;
			if (zcount == 100000)//中心位置から強度値を取る
			{
				ddd = (int)d * 4500 / 65535;//change to millimeters
				hd = ddd - levelh;
				ld = ddd + levell;
				hdd = hd * 65535 / 4500;//to change millimeters to intensity
				ldd = ld * 65535 / 4500;
				tmpStore = ddd;
			}
			else
			{
				ddd = zcount;
				hd = ddd - levelh;
				ld = ddd + levell;
				hdd = hd * 65535 / 4500;//to change millimeters to intensity
				ldd = ld * 65535 / 4500;
			}

			//中心から最も近い強度値を格納する
			int maxh = 100000;
			for (int y = 0; y < kinect.depthImage.rows; y++) {
				for (int x = 0; x < kinect.depthImage.cols; x++) {
					UINT16 d = kinect.depthImage.at<UINT16>(y, x);
					int dd = (int)d * 4500 / 65535;
					if (dd > hd && dd <= ld)
					{
						separate_img.at<UINT16>(y, x) = d;
					}
					else
					{
						separate_img.at<UINT16>(y, x) = 0;
					}
				}
			}


			cv::Mat binary = separate_img > 100;//画像を二値化、しきい値は100
			std::vector<std::vector<cv::Point>>contours; //ストロークの輪郭点
														 //std::vector 各要素は線形に、順序を保ったまま格納される可変長配列
			std::vector<cv::Vec4i>hierarchy;
			cv::Canny(binary, binary, 200, 225, 3);//エッジ検出
			cv::imshow("canny", binary);
			cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//キャニーエッジの結果から輪郭を見つける
			int mindist = 100000;

			for (int i = 0; i < contours.size(); i++)
			{
				if (cv::contourArea(contours[i]) >= 100)
				{
					minRect = cv::minAreaRect(cv::Mat(contours[i]));//輪郭領域
					cv::RotatedRect minEllipse;
					if (contours[i].size()>100)//release small size
					{
						minEllipse = cv::fitEllipse(cv::Mat(contours[i]));//楕円に変更
						float x = minEllipse.center.x; //楕円中心のx座標
						float y = minEllipse.center.y; //楕円中心のy座標
						float aa = minEllipse.size.width / 2;  //楕円（回転矩形）の横幅（x軸方面）÷2 　 width >= height
															   //「widthのところにカーソルを持っていく」cv::Size2fはfloat型で定義されている。それぞれメンバ width と height が矩形の幅と高さを表す．
						float bb = minEllipse.size.height / 2; //楕円（回転矩形）の高さ（y軸方面）÷2

															   //int dist = sqrt(((x - kinect.depthImage.cols / 2)*(x - kinect.depthImage.cols / 2)) + ((y - kinect.depthImage.rows / 2)*(y - kinect.depthImage.rows / 2)));

															   //if (dist < mindist)
						if (x - bb <= kinect.depthImage.cols / 2 + 20 && x + bb >= kinect.depthImage.cols / 2 - 20 && y - aa <= kinect.depthImage.rows / 2 + 20 && y + aa >= kinect.depthImage.cols / 2 - 20)//正方形が中央にあるかどうか
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
				if (xc > 0 && xc <= kinect.depthImage.cols - 1 && yc > 0 && yc <= kinect.depthImage.rows - 1 && a>0 && b>0)
					//0＜楕円の中心のx座標≦depth画像の横幅-1　かつ　0＜楕円の中心のy座標≦depth画像の縦幅-1　かつ　楕円の大きさが0ではない
				{
					cv::ellipse(result, minBEllipse, cv::Scalar(0, 0, 255), 2, 8);//楕円を描く
																				  //cv::circle(result, cv::Point(xc, yc), 3, cv::Scalar(0, 255, 0), CV_FILLED);//楕円の中心
																				  //result：円を描画する画像
																				  //cv::Point(xc,yc)：円の中心座標
																				  //3:円の半径
																				  //cv::Scalar(0, 255, 0)：円の色





																				  //s4 = "Data/coordinate_" + s3 + ".xyz"; //s4は点群データ
					fp = fopen(s5.c_str(), "wt");
					xx_val.clear();//xx_val：ファイル読み込みデータを格納するx
					yy_val.clear();//yy_val：ファイル読み込みデータを格納するy
					zz_val.clear();//zz_val：ファイル読み込みデータを格納するz
					c = 0;
					//s7 = "Data/uv_" + s3 + ".txt";//s7はxc - a,xc + a,yc - b,yc + b　の4つの値を格納
					ff = fopen(s8.c_str(), "wt");
					fprintf(ff, "%d\n %d\n %d\n %d", (int)yc - a, (int)yc + a, (int)xc - b, (int)xc + b);//uv_ファイル名.txtに保存される4つの数字
																										 //(xc,yc)は楕円の中心座標
																										 //a:楕円長径の半分
																										 //b:楕円短径の半分
					fclose(ff);
					/*ue = yc - a;
					ve = yc + a;
					we = xc - b;
					he = xc + b;*/


					ue = yc - a;
					we = yc + a;
					ve = xc - b;
					he = xc + b;
					//ファイルにxyz値を格納する



					for (int x = xc - b; x < xc + b; x++)
					{
						for (int y = yc - a; y < yc + a; y++)
						{
							UINT16 d = separate_img.at<UINT16>(y, x);//画像の (x , y ) 座標の画素にアクセスする <UNIT16>(y,x)でxとyが反対だけどこれでOK
							int z_coor = (int)d * 4500 / 65535;		//z座標取得
							if (z_coor != 0)//z座標からx座標、y座標を求める
							{
								x_coor = (x - 512 / 2)*z_coor / 365; //x_coor:x座標
								y_coor = (y - 424 / 2)*z_coor / 365; //y_coor:y座標
								if (z_coor < maxh)
								{
									maxh = z_coor;//z_coor:z座標
												  //(u,v)は豚の背中のなかのトップポジションの座標
									u = ((x_coor * 365) + (256 * z_coor)) / z_coor;
									v = ((y_coor * 365) + (212 * z_coor)) / z_coor;
								}
								xx_val.push_back(x_coor);
								yy_val.push_back(y_coor);
								zz_val.push_back(z_coor);
								fprintf(fp, "%d %d %d\n", x_coor, y_coor, z_coor);//x,y,z座標をファイルに保存
								c++;
							}
						}
					}
					//cv::circle(result, cv::Point(u, v), 3, cv::Scalar(255, 0, 255), CV_FILLED);
					fclose(fp);

					if (maxh != 100000 && val <= tmpStore + 3 && val >= tmpStore - 3)// && zcount>maxh)//中心強度が変化しているかしていないか
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
		cv::imwrite(s7, kinect.rgbImage);



		int minx = 100000, maxx = -100000, minz = 100000, maxz = -100000, sumz = 0;
		int ic = 0;


		cv::Mat xMat4 = cv::Mat_<double>(1000, 5);//断面に最も近い位置に格納する、4パワー、上面図
		cv::Mat xMat2 = cv::Mat_<double>(1000, 3);//断面に最も近い位置に格納する、2電力、側面図
		cv::Mat yMat = cv::Mat_<double>(1000, 1);
		cv::Mat zMat = cv::Mat_<double>(1000, 1);

		int *xlength = new int[100];



		for (int i = 0; i <xx_val.size(); i++)
		{
			if (minx > xx_val[i]) { minx = xx_val[i]; } //ファイル読み込みデータを格納するx箱が 100000より小さい場合　→　この箱の名前はminx
			if (maxx < xx_val[i]) { maxx = xx_val[i]; } //ファイル読み込みデータを格納するx箱が-100000より大きい場合　→　この箱の名前はmaxx
			if (minz > zz_val[i]) { minz = zz_val[i]; } //ファイル読み込みデータを格納するz箱が 100000より小さい場合　→　この箱の名前はminz
			if (maxz < zz_val[i]) { maxz = zz_val[i]; } //ファイル読み込みデータを格納するz箱が-100000より大きい場合　→　この箱の名前はmaxz
			sumz = sumz + zz_val[i];                      //sumz=0+z座標の全てのデータ
		}

		int avgz = sumz / zz_val.size();
		//std::cout << "Average=" << avgz << std::endl;

		int datasize1 = 0;
		int curxmx = -100000, curxmn = 100000, curymx = -100000, curymn = 100000, curzmx = -100000, curzmn = 100000;
		int earpos = 0;
		for (int j = minx; j <= maxx; j += value)//maxx
		{
			int seg_min = 100000;
			std::vector<int>divide_x;//生の断面データを保存する
			std::vector<int>divide_y;
			std::vector<int>divide_z;
			double ymax = -100000, ymin = 100000;
			float last = 1;
			int index = 0;
			for (int i = 0; i <= xx_val.size(); i++)
			{
				if (xx_val[i] >= j - 3 && xx_val[i] < j + 3 && zz_val[i] > 0)//vminxとの3つの違いを選択
				{
					divide_x.push_back(xx_val[i]);//xx_val[i]
					divide_y.push_back(yy_val[i]);
					divide_z.push_back(zz_val[i]);
					if (yy_val[i] < ymin) { ymin = yy_val[i]; }
					if (yy_val[i] > ymax) { ymax = yy_val[i]; }
				}
			}



			if (divide_y.size() > 10)//　ノイズ除去
			{
				cv::Mat fmax = cv::Mat_<double>(divide_y.size(), 3);//2つの電力推定曲線を描き、デジタル誤差を減らす
				cv::Mat fz = cv::Mat_<double>(divide_y.size(), 1);
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


				for (int i = ymin; i < ymax; i += 1)//見積もり曲線上の頂点のz値を求める
				{
					double valz = zresult.at<double>(0, 0)*i*i + zresult.at<double>(1, 0)*i + zresult.at<double>(2, 0);
					if (valz < minz)
					{
						minz = valz;
						valy = i;
					}
				}



				double mind = 1000000000.00;//見積曲線と断面との間の最も近い位置を見つける
				for (int s = 0; s < divide_x.size(); s++)
				{
					double dist = sqrt(((valy - divide_y[s])*(valy - divide_y[s])) + ((minz - divide_z[s])*(minz - divide_z[s])));
					if (mind >= dist) { mind = dist; index = s; }
				}

				if (divide_z[index] < avgz + 20)
				{
					//store each top position
					fprintf(fb, "%d %d %d %d %d %d\n", divide_x[index], divide_y[index], divide_z[index], 255, 255, 0);

					//4行列
					xMat4.at<double>(ic, 0) = (double)divide_x[index] * divide_x[index] * divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 1) = (double)divide_x[index] * divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 2) = (double)divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 3) = (double)divide_x[index];
					xMat4.at<double>(ic, 4) = (double)1;

					//2行列
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



		for (float i = curxmn; i <curxmx; i++)//頭から尾までの豚の長さを見つける
		{
			int yy = (int)i * i * resulty.at<double>(0, 0) + i * resulty.at<double>(1, 0) + resulty.at<double>(2, 0);
			int zz = (int)i * i * i * i * resultz.at<double>(0, 0) + i * i * i * resultz.at<double>(1, 0) + i * i * resultz.at<double>(2, 0) + i * resultz.at<double>(3, 0) + resultz.at<double>(4, 0);
			float u = ((i * 365) + (256 * zz)) / zz;
			float v = ((yy * 365) + (212 * zz)) / zz;
			//if (u >= k[2] && u <= k[3] && v >= k[0] && v <= k[1])
			{
				if (i > curxmn)
				{
					distance += sqrt(((i - tempx)*(i - tempx)) + ((yy - tempy)*(yy - tempy)) + ((zz - tempz)*(zz - tempz)));
					//distance=distance+√i^2+yy^2+zz^2
				}
				tempx = i; tempy = yy; tempz = zz;
				fprintf(fc, "%d %d %d %d %d %d\n", (int)i, yy, zz, 255, 0, 255);
			}
		}

		distance = distance / 10;
		int dis = (int)distance;
		std::ostringstream convert;
		convert << dis;
		std::string ss = s3 + convert.str() + "kg";
		cv::putText(result, ss.c_str(), cv::Point((kinect.depthImage.cols - 1) / 2 - 15, (kinect.depthImage.rows - 1) / 2 - 15), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0), 1, CV_AA);
		cv::imshow("Weight Estimation", result); //指定したウィンドウWeight Estimation内に、画像resultを表示します
		cv::imwrite(s6, result);//指定したファイルに画像を保存します．
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
	std::vector<int>xx_val;//ファイル読み込みデータを格納するx配列
	std::vector<int>yy_val;//store file read data y
	std::vector<int>zz_val;//store file read data z

	AllocConsole();
	FILE *fd, *fs, *ff;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);

	CString filter("XYZ Files (coordinate*.xyz)|coordinate*.xyz");
	CFileDialog     selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	int x, y, z, r, g, b;
	FILE *fp;
	CString filepath, fn;//CString：char型またはwchar_t型が格納される

	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();//xyzデータファイルを選択するところに移動できる
		fn = selDlg.GetFileName();

		CStringA csTempmb(filepath.GetBuffer(0));//CStringA：char型が格納される
		std::string filename = csTempmb.GetBuffer(0);
		fp = fopen(csTempmb.GetBuffer(0), "r"); // ファイルを読み込み用としてOpen
												//読み込んだ画像ファイル：csTempmb.GetBuffer(0)
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
	//xyzデータ格納箱作成
	std::vector<int>xx_val;
	std::vector<int>yy_val;
	std::vector<int>zz_val;


	AllocConsole();
	FILE *fd, *fs, *ff, *fa, *fw, *fq, *fg;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
	int x, y, z, r, g, b;
	FILE *fp, *fc;
	CString filepath, fn;
	CString  filter("XYZ Files (coordinate*.xyz)|coordinate*.xyz");
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);


	if (selDlg.DoModal() == IDOK)//xyzデータについて
	{
		filepath = selDlg.GetPathName();
		fn = selDlg.GetFileName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fp = fopen(csTempmb.GetBuffer(0), "r");
	}

	CString filepathl;
	CString         filterl("Text Files (uv_*.txt)|uv_*.txt");
	CFileDialog     selDlgl(TRUE, NULL, NULL, OFN_HIDEREADONLY, filterl);

	if (selDlgl.DoModal() == IDOK)//uvデータについて
	{
		filepathl = selDlgl.GetPathName();
		CStringA csTempmb(filepathl.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fc = fopen(csTempmb.GetBuffer(0), "r");
	}

	////////////////ここまでがファイルの読み込み//////////////////////////////////////////////////////




	int k[4]; int c = 0;//stroe uv data in int k array
						//int k配列のストロークuvデータ

	while ((fscanf(fc, "%d", &x)) != EOF) {//uvデータの入ったファイルをスキャン
		std::cout << x << std::endl;//uvデータ4つを出力
		k[c] = x; c++;
	}
	fclose(fc);//uvデータの入ったファイルを閉じる
			   //std::cout << k[0] << " " << k[1] << " " << k[2] << " " << k[3] << std::endl;



	CT2CA t(fn);
	std::string stream3(t);


	std::string s9 = stream3;
	s9 = "Data/data_" + s9 + ".txt";
	ff = fopen(s9.c_str(), "wt");//ファイルを書き込み用としてOpen
								 //ファイルのある場所はpigSeparation→Data　ファイル名data.txt

	std::string s10 = stream3;
	s10 = "Data/taichox_" + s10 + ".txt";
	fw = fopen(s10.c_str(), "wt");

	std::string s11 = stream3;
	s11 = "Data/taichoy_" + s11 + ".txt";
	fq = fopen(s11.c_str(), "wt");

	std::string s12 = stream3;
	s12 = "Data/taichoz_" + s12 + ".txt";
	fg = fopen(s12.c_str(), "wt");

	std::string s3 = stream3;//s3=coordinate_？(例ba0.xyz)
	s3 = "Data/cross_section" + s3 + ".xyz";//ファイル名cross_sectioncoordinate
											//豚を30mmずつ断面の円近似（半円のみ）

	fs = fopen(s3.c_str(), "wt");//ファイルを書き込み用としてOpen





	std::string s4 = stream3;
	s4 = "Data/circleData_" + s4 + ".txt";//ファイル名circleDatacoordinate
	fa = fopen(s4.c_str(), "wt");//ファイルを書き込み用としてOpen
	filename = s3;//配列filenameにs3を代入


				  ///////////////ここまでが新しいファイルの準備など///////////////////////////////////////////////////



	int minx = 100000, maxx = -100000, minz = 100000, maxz = -100000;
int sumz = 0;  
	int ic = 0;


	//////////////////////////////////
	//int curxmx = -100000;
	//int curxmn = 100000;
	//int curxmn = 10;
	//int curymx = -100000, curymn = 100000, curzmx = -100000, curzmn = 100000;
	/////////////////////////////////


	cv::Mat xMat4 = cv::Mat_<double>(1000, 5);//4次方程式
	cv::Mat xMat2 = cv::Mat_<double>(1000, 3);//2次方程式
	cv::Mat yMat = cv::Mat_<double>(1000, 1);//4次方程式
	cv::Mat zMat = cv::Mat_<double>(1000, 1);//2次方程式
	int *xlength = new int[100];//int型の配列xlength（箱は100こ）を設定

								//xyzデータを挿入
	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {
		xx_val.push_back(x);
		yy_val.push_back(y);
		zz_val.push_back(z);
		if (minx > x) { minx = x; }  //100000>x座標ならばx=100000 つまりx座標の最小値は100000
		if (maxx < x) { maxx = x; }  //-100000<x座標ならばx=-100000 つまりx座標の最大値は-100000
		if (minz > z) { minz = z; }  //100000>z座標ならばz=100000 つまりz座標の最小値は100000
		if (maxz < z) { maxz = z; }  //-100000<z座標ならばz=-100000 つまりz座標の最大値は-100000
									 //sumz += z;
	}

	//int datasize1 = 0;
	//int avgz = sumz / zz_val.size();//z座標の平均＝z座標の合計値÷z座標の個数
	//std::cout << "Average z=" << avgz << std::endl;//z座標の平均を出力






	//ここから断面データについて//////////////////////////////////////////////////////////////
	for (int j = minx; j <= maxx; j += value)//100000<j<-100000 j=j+valueつまりj=j+30
	{

		int seg_min = 100000;
		std::vector<int>divide_x;//断面データを保存するための配列
		std::vector<int>divide_y;
		std::vector<int>divide_z;
		double ymax = -100000, ymin = 100000;
		float last = 1;
		int index = 0;

		for (int i = 0; i <= xx_val.size(); i++)//x座標の断面データが格納されている配列を1からスキャン
		{
			//配列をスキャンしていく中で
			if (xx_val[i] >= j - 3 && xx_val[i] < j + 3)//豚の半円を描く際に30mmずつ区切るために、「x座標>j-3 かつ x座標<j+3 なら」が必要
														//if (xx_val[i] >= j - 30 && xx_val[i] < j + 30)
			{
				divide_x.push_back(xx_val[i]);//x座標を断面データ配列に格納
				divide_y.push_back(yy_val[i]);//y座標を断面データ配列に格納
				divide_z.push_back(zz_val[i]);//z座標を断面データ配列に格納

				if (yy_val[i] < ymin) { ymin = yy_val[i]; }//もし　y座標<100000ならy=100000 つまりy座標の最小値は100000
				if (yy_val[i] > ymax) { ymax = yy_val[i]; }//もし　y座標>-100000ならy=-100000 つまりy座標の最大値は-100000
			}
		}




		if (divide_y.size() > 10)//断面データのy座標>10ならば
								 //if (divide_y.size() > 100)//上記と少し変えて100にしたら、豚（半円）の長さが短くなる。つまり、頭と尾が表示されなくなった
		{
			cv::Mat fmax = cv::Mat_<double>(divide_y.size(), 3);//2次曲線を描き、デジタル誤差を減らす
																//divide_y.size() × 3 行列			
			cv::Mat fz = cv::Mat_<double>(divide_y.size(), 1);//divide_y.size() × 1 行列

			for (int n = 0; n < divide_x.size(); n++)//断面データのx座標を0から存在するデータ全てまで++
			{
				//"cross_section.xyzファイルに書き込む" 
				//豚の半円（青色）データをファイルに書き込む
				fprintf(fs, "%d %d %d %d %d %d\n", divide_x[n], divide_y[n], divide_z[n], 100, 255, 255);


				fmax.at<double>(n, 0) = (double)divide_y[n] * divide_y[n];//cv::Mat fmax：二次曲線を描くもの
				fmax.at<double>(n, 1) = (double)divide_y[n];
				fmax.at<double>(n, 2) = (double)last;
				fz.at<double>(n, 0) = (double)divide_z[n];
			}


			if (divide_x.size() > 10)//divide_x：断面データx座標の箱（配列）＞10
			{
				cv::Mat inverx; //行列を作成（fmaxの逆行列をつくるために）(3×divide_y.size)
				int index = 0;
				inverx = fmax.inv(cv::DECOMP_SVD);//fmaxの逆行列を作成	
				cv::Mat zresult = inverx*fz; //zresult行列＝fmax逆行列×fz (3×1)


				double minz = 100000.000;
				double valy;


				for (int i = ymin; i < ymax; i += 1)//見積もり曲線上の頂点のz値を求める
													//見積もり曲線とは豚の最もトップ座標をつなげた線に近いもの
													//本物との違いは2次曲線で近似していること。ノートを確認
				{
					double valz = zresult.at<double>(0, 0)*i*i + zresult.at<double>(1, 0)*i + zresult.at<double>(2, 0);
					if (valz < minz)
					{
						minz = valz;
						valy = i;
					}
				}



				double mind = 1000000000.00;//見積曲線と断面との間の最も近い位置を見つける
				for (int s = 0; s < divide_x.size(); s++)
				{
					double dist = sqrt(((valy - divide_y[s])*(valy - divide_y[s])) + ((minz - divide_z[s])*(minz - divide_z[s])));
					if (mind >= dist) { mind = dist; index = s; }
				}


				//各トップポジションを格納する
			//	if (divide_z[index] >= avgz - 20 && divide_z[index] < avgz + 20)
					//if (divide_z[index] >= avgz - 30 && divide_z[index] < avgz + 23)
				{
					//エラーが出る時はこのfaファイル（circleDatacoordinate）に何の値も入っていないかも？
					//黄色い点で表示される。この点の中間を通るように二次曲線作っているかも？
					fprintf(fa, "%d %d %d %d %d %d\n", divide_x[index], divide_y[index], divide_z[index], 255, 255, 0);//(黄色い点のx座標,y座標,z座標,点の色RGB黄色)

																													   //printf("%d %d %d \n", divide_x[index], divide_y[index], divide_z[index]);

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

					xlength[ic] = divide_x[index];//xlength：int型の配列
												  //（箱は100こ）を設定してるからicも100こまでかな？
					ic++;






					//////////////////////////////////////////////////////
					for (int s = 0; s < ic; s++)
					{
						int val;
						//val = xMat4.at<double>(s, 3);
						val = xlength[s];



						xMat4.resize(ic);
						xMat2.resize(ic);
						yMat.resize(ic);
						zMat.resize(ic);
						cv::Mat inverx4, inverx2;
						inverx4 = xMat4.inv(cv::DECOMP_SVD);// (cv::DECOMP_SVD)：逆行列
															//std::cout << inverx4 << std::endl << std::endl << std::endl ;
						inverx2 = xMat2.inv(cv::DECOMP_SVD);
						cv::Mat resultz = inverx4*zMat;
						cv::Mat resulty = inverx2*yMat;
						int tempx = 0, tempy = 0, tempz = 0;//初期化
						float distance = 0.0;

						//頭から尾までの豚の長さを見つける
						for (float i = 0; i < val; i++)//iが100000から-99999になるまでi++
													   //i=100000 i<-100000 i++
						{
							int yy = (int)i * i * resulty.at<double>(0, 0) + i * resulty.at<double>(1, 0) + resulty.at<double>(2, 0);
							int zz = (int)i * i * i * i * resultz.at<double>(0, 0) + i * i * i * resultz.at<double>(1, 0) + i * i * resultz.at<double>(2, 0) + i * resultz.at<double>(3, 0) + resultz.at<double>(4, 0);

							//float u = ((i * 365) + (256 * zz)) / zz;
							//float v = ((yy * 365) + (212 * zz)) / zz;
							//if (u >= k[2] && u <= k[3] && v >= k[0] && v <= k[1])
							{
								//if (i > curxmn)//もしi>100000ならば
								//{

								//distance += sqrt(((i - tempx)*(i - tempx)) + ((yy - tempy)*(yy - tempy)) + ((zz - tempz)*(zz - tempz)));
								//distance=√( i^2 + yy^2 + zz^2 ) 原点とある座標までの距離
								//}

								tempx = i;  //tempx=体長の線を描くためのx座標データ
								tempy = yy; //tempy=体長の線を描くためのy座標データ 
								tempz = zz; //tempz=体長の線を描くためのz座標データ

								fprintf(ff, "%d %d %d %d %d %d\n", (int)i, yy, zz, 255, 0, 255);//ff：data.txtファイルのこと
																								//data.txtファイル（体長の線を描くためのデータ）にデータを書き込む
								fprintf(fw, "%d\n", (int)i);
								fprintf(fq, "%d\n", yy);
								fprintf(fg, "%d\n", zz);
								//printf("%d %d\n",tempx,(int)i);//tempx=体長の線を描くためのx座標データ=i
								//printf("%d %d\n", tempx, i);   //tempx=体長の線を描くためのx座標データ　i=0
								//printf("%d %d\n", tempy, yy);//tempy=体長の線を描くためのy座標データ=yy
							}
						}
					}
					////////////////////////////////////////////////////////











				}
			}
		}
	}

	////////////////////ここまでがトップ座標を見つけてファイルに格納/////////////////////////////////////////////////////





	//distance = distance / 10;

	//truLen = distance;//もともとtruLen = 0だったものにdistanceを代入する


	UpdateData();
	CString aup;
	//aup.Format(_T("%f"), distance);//%f：浮動小数点数として出力する

	//std::cout << "distance=" << distance << std::endl;

	m_length = aup;
	UpdateData(FALSE);

	//int dataAx, dataAy, dataAz;
	//int dataBx, dataBy, dataBz;
	//int dataCx, dataCy, dataCz;
	//int dataDx, dataDy, dataDz;
	//printf("左前足（x,y,z）は？ \n");
	//scanf("%d %d %d", &dataAx, &dataAy, &dataAz);
	//printf("右前足（x,y,z）は？ \n");
	//scanf("%d %d %d", &dataBx,&dataBy, &dataBz);
	//printf("左後足（x,y,z）は？\n");
	//scanf("%d %d %d", &dataCx, &dataCy, &dataCz);
	//printf("右後足（x,y,z）は？\n");
	//scanf("%d %d %d", &dataDx, &dataDy, &dataDz);

	//CString filepatho;
	//CString         filtero("Point Files (point_*.txt)|point_*.txt");
	//CFileDialog     selDlgo(TRUE, NULL, NULL, OFN_HIDEREADONLY, filtero);

	//if (selDlgo.DoModal() == IDOK)//pointデータについて
	//{
	//filepatho = selDlgo.GetPathName();//uvデータファイルを選択するところに移動できる
	//CStringA csTempmb(filepatho.GetBuffer(0));
	//std::string filename = csTempmb.GetBuffer(0);
	//fo = fopen("Point.txt", "w");//ファイルを読み込み用としてOpen
	//読み込んだ画像ファイル：csTempmb.GetBuffer(0)
	//}

	//fprintf(fo, "%d %d %d\n%d %d %d\n%d %d %d\n%d %d %d\n ", dataAx,dataAy,dataAz,dataBx, dataBy,dataBz, dataCx, dataCy, dataCz, dataDx, dataDy, dataDz);
	//printf("%d %d %d\n%d %d %d\n%d %d %d\n%d %d %d\n ", dataAx, dataAy, dataAz, dataBx, dataBy, dataBz, dataCx, dataCy, dataCz, dataDx, dataDy, dataDz);

	//int p[4]; int p = 0;//stroe uv data in int k array
	//int k配列のストロークuvデータ

	//while ((fscanf(fc, "%d", &x)) != EOF) {//uvデータの入ったファイルをスキャン
	//std::cout << x << std::endl;//uvデータ4つを出力
	//k[c] = x; c++;
	//}

	//fclose(fc);//uvデータの入ったファイルを閉じる
	//std::cout << k[0] << " " << k[1] << " " << k[2] << " " << k[3] << std::endl;

	AfxMessageBox(_T("All results have been saved!"), MB_OK | MB_ICONINFORMATION);//「正常終了」と表示させる
	fclose(fa);
	fclose(ff);
	fclose(fs);
	fclose(fw);
	fclose(fq);
	fclose(fg);
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


	//levellの値がButton1と異なる
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
	std::string figD = "Data/depth" + s3 + ".png";
	std::string figR = "Data/RGB" + s3 + ".png";
	UpdateData(FALSE);


	if (stream3.length() > 0)
	{
		cv::RotatedRect minRect;
		cv::RotatedRect minBEllipse;
		float xc, yc;
		int zcount = 100000;
		int bc = 0, a, b, u, v;
		int tmpStore;
		while (1)
		{
			//RGB画像を取得している
			kinect.setRGB();
			//cv::imshow("RGB", kinect.rgbImage);//RGB画像を表示する

			kinect.setDepth(false);//initialize depth image

			kinect.depthImage.convertTo(measure_img, CV_8UC1, 25.0 / 4500);//depth image grayscale (2)channels to color image (3)channels
			cv::cvtColor(measure_img, result, CV_GRAY2BGR);//
			cv::rectangle(result, cv::Rect((kinect.depthImage.cols - 1) / 2 - 10, (kinect.depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//get the center position
			kinect.depthImage.copyTo(separate_img);
			UINT16 d = kinect.depthImage.at<UINT16>((kinect.depthImage.rows - 1) / 2, (kinect.depthImage.cols - 1) / 2);// intensity of depth image
			int val = (int)d * 4500 / 65535;


			/////////Small pigと異なる部分/////////////
			//cv::line(result, cv::Point(0, kinect.depthImage.rows / 2), cv::Point(kinect.depthImage.cols, kinect.depthImage.rows / 2), cv::Scalar(0, 0, 255), 2);//get the center position
			//cv::line(result, cv::Point(kinect.depthImage.cols / 2, 0), cv::Point(kinect.depthImage.cols / 2, kinect.depthImage.rows), cv::Scalar(0, 0, 255), 2);//get the center position
			///////////////////////////////////////////

			int ddd;
			int hd, ld, hdd, ldd;
			if (zcount == 100000)//take intensity value from center position
			{
				ddd = (int)d * 4500 / 65535;//change to millimeters
				hd = ddd - levelh;
				ld = ddd + levell;
				hdd = hd * 65535 / 4500;//to change millimeters to intensity
				ldd = ld * 65535 / 4500;
				tmpStore = ddd;
			}
			else
			{
				ddd = zcount;
				hd = ddd - levelh;
				ld = ddd + levell;
				hdd = hd * 65535 / 4500;//to change millimeters to intensity
				ldd = ld * 65535 / 4500;
			}


			//store nearest intensity values from center
			int maxh = 100000;
			for (int y = 0; y < kinect.depthImage.rows; y++) {
				for (int x = 0; x < kinect.depthImage.cols; x++) {
					UINT16 d = kinect.depthImage.at<UINT16>(y, x);
					int dd = (int)d * 4500 / 65535;
					if (dd > hd && dd <= ld)
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
			std::vector<std::vector<cv::Point>>contours; // to stroe contours point
			std::vector<cv::Vec4i>hierarchy;
			cv::Canny(binary, binary, 200, 225, 3);// edge detection
			cv::imshow("canny", binary);
			cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//find contours from canny edge result
			int mindist = 100000;

			for (int i = 0; i < contours.size(); i++)
			{
				if (cv::contourArea(contours[i]) >= 100)
				{





					minRect = cv::minAreaRect(cv::Mat(contours[i]));//contour area
																	//minAreaRect:与えられた 2次元点集合を囲む（回転を考慮した）最小の矩形を求めます
																	//minRectは、豚の場所を確認し、画像から輪郭ベクトル（u,v）を決定するために使用
																	//contours：輪郭線（等高線）


					cv::RotatedRect minEllipse;
					if (contours[i].size()>100)//小さなサイズを解放する
					{
						minEllipse = cv::fitEllipse(cv::Mat(contours[i]));//change to ellipse
						float x = minEllipse.center.x;
						float y = minEllipse.center.y;
						float aa = minEllipse.size.width / 2;    // width >= height
						float bb = minEllipse.size.height / 2;

						//int dist = sqrt(((x - kinect.depthImage.cols / 2)*(x - kinect.depthImage.cols / 2)) + ((y - kinect.depthImage.rows / 2)*(y - kinect.depthImage.rows / 2)));

						//if (dist < mindist)
						if (x - bb <= kinect.depthImage.cols / 2 + 20 && x + bb >= kinect.depthImage.cols / 2 - 20 && y - aa <= kinect.depthImage.rows / 2 + 20 && y + aa >= kinect.depthImage.cols / 2 - 20)//square is in the center or not
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
				if (xc > 0 && xc <= kinect.depthImage.cols - 1 && yc > 0 && yc <= kinect.depthImage.rows - 1 && a>0 && b>0)
				{
					cv::ellipse(result, minBEllipse, cv::Scalar(0, 0, 255), 2, 8);//draw ellipse
					cv::circle(result, cv::Point(xc, yc), 3, cv::Scalar(0, 255, 0), CV_FILLED);//center of ellipse
					std::string s = "Data/coordinate_" + s3 + ".xyz";
					fp = fopen(s.c_str(), "wt");


					std::string ss = "Data/uv_" + s3 + ".txt";
					ff = fopen(ss.c_str(), "wt");
					fprintf(ff, "%d\n%d\n%d\n%d", (int)yc - a, (int)yc + a, (int)xc - b, (int)xc + b);
					fclose(ff);


					//store xyz values in file
					for (int x = xc - b; x < xc + b; x++)
					{
						for (int y = yc - a; y < yc + a; y++)
						{
							UINT16 d = separate_img.at<UINT16>(y, x);
							int z_coor = (int)d * 4500 / 65535;		//get z coordinate
							if (z_coor != 0)
							{
								x_coor = (x - 512 / 2)*z_coor / 365;
								y_coor = (y - 424 / 2)*z_coor / 365;
								if (z_coor < maxh)
								{
									maxh = z_coor;
									u = ((x_coor * 365) + (256 * z_coor)) / z_coor;
									v = ((y_coor * 365) + (212 * z_coor)) / z_coor;
								}
								fprintf(fp, "%d %d %d\n", x_coor, y_coor, z_coor);
							}
						}
					}
					cv::circle(result, cv::Point(u, v), 3, cv::Scalar(255, 0, 255), CV_FILLED);
					fclose(fp);

					if (maxh != 100000 && val <= tmpStore + 3 && val >= tmpStore - 3)// && zcount>maxh)//center intensity change or  not
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
		cv::imwrite(figR, kinect.rgbImage);
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
