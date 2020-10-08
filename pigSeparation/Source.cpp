
// PigSeparationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PigSeparation.h"
#include "PigSeparationDlg.h"
#include "afxdialogex.h"
#include "NtKinect.h"
#include "Estimate_Weight.h"
#include <opencv2/opencv.hpp>    // OpenCV�w�b�_
#include <opencv2/opencv.hpp>    // OpenCV�w�b�_
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
std::string filename; //std::string:������N���X

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
	UpdateData();//�e�L�X�g�{�b�N�X���當������擾����
	CString str = m_control;
	CT2CA t(str);//CString���當����ւ̕ϊ�
	std::string stream3(t);
	std::ostringstream convert;

	convert << imgC;
	std::string s3 = "s" + stream3 + convert.str();
	std::string figD = "Data/depth" + s3 + ".png";
	std::string figR = "Data/RGB" + s3 + ".png";
	UpdateData(FALSE);
	int chk = 1;

	/////�Z�~�i�[���|�[�g�s��//////////////
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
		cv::RotatedRect minRect; //RotatedRect�͉�]���l��������`
								 //�������Acv::RotatedRect�ɂ́A���S�ƁA�T�C�Y�ƁA�X�����i�[����Ă��邽�߁A�ȉ~��`�悷�邱�Ƃ��ł���
		cv::RotatedRect minEllipse;
		cv::RotatedRect minBEllipse;
		float xc, yc;
		int zcount = 100000;
		int bc = 1, a, b, u, v;
		int tmpStore;
		while (1)
		{
			//�[�x�摜depthImage���擾����i�{�^��1�ł̓v���O�����ł̓J���[�摜�͗p���Ă��Ȃ��j
			kinect.setDepth(false);//depth�摜������������
								   //cv::imshow("Depth", kinect.depthImage);

								   //kinect.setRGB();//RGB�摜���擾����
								   //cv::imshow("RGB", kinect.rgbImage);//RGB�摜��\������

			kinect.depthImage.convertTo(measure_img, CV_8UC1, 25.0 / 4500);//depth image grayscale (2)channels to color image (3)channels
																		   //���̎w���ōs��̃f�[�^��ʂ̌^�ɕϊ����܂�
																		   //���s��̃s�N�Z���l��ړI�̃f�[�^�^�ɕϊ����܂�
																		   //CV_8UC1�F8�r�b�g�A�����Ȃ��A�P�`�����l���̃f�[�^�܂�P��unsigned char�^�̃f�[�^�^�C�v�������萔
																		   //CV_8UC1�F�o�͍s��ɗv������^�C�i�`�����l�����͌��̍s��Ɠ����Ȃ̂Łj���m�ɂ́C�v������r�b�g�[�x
																		   //25.0 / 4500�F

			cv::cvtColor(measure_img, result, CV_GRAY2BGR);//measure_img�܂�depthimage�͐[�x�摜�ł��邽�߁A2�r�b�g�O���[�X�P�[���摜�ł���ARGB�ɂ��Ă��ω��͋N���Ȃ�
														   //�������ARGB�摜�ɕϊ������Ă������R�́A���̌㒆�S�ʒu�̎擾�̍ۂ̐Ԃ��l�p��`������A�΂�s���N�̓_��\��������ہA�J���[��p���邩��



														   //cv::imwrite("depth005.png", result); //depth�摜�擾�@Pigseparetion���̃t�@�C���ɓ���



			cv::rectangle(result, cv::Rect((kinect.depthImage.cols - 1) / 2 - 10, (kinect.depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//���S�ʒu���擾���� �Ԃ��l�p��`��


																																							//�R�s�[���闝�R�F���P�x�ƒ�P�x���g�p���A�s�N�Z���l�������̒l�̊Ԃɂ���摜�𕪗����邽�߁B�܂�A�؂̃g�b�v�|�W�V�����̌덷�}100�̕����Ƃ����łȂ������𕪗������邽�߂ɃR�s�[����
																																							//���̃R�s�[���ꂽ�摜�͌�ɁA��l���摜�Ƃ��ėp�����A�������l10��K�p����
			kinect.depthImage.copyTo(separate_img);//�R�s�[���ꂽ�摜separate_img


			UINT16 d = kinect.depthImage.at<UINT16>((kinect.depthImage.rows - 1) / 2, (kinect.depthImage.cols - 1) / 2); // unit16 16�r�b�g�����Ȃ�����
																														 //intensity of depth image "depth image"�̋��x
																														 //rows�F�s��̍s���i�摜�̍����j���Q�Ƃ���Mat�N���X�̃����o�[�ϐ�
																														 //cols�F�s��̗񐔁i�摜�̕��j���Q�Ƃ���Mat�N���X�̃����o�[�ϐ�

																														 //�g���b�N�o�[�쐬�@HIntensity��LIntensity�@
																														 //cv::createTrackbar("HIntensity", "separate", &levelh, 800);//to control highest intensity �ō����x�𐧌䂷��
																														 //cv::createTrackbar("LIntensity", "separate", &levell, 800);//to control lowest intensity �Œይ�x�𐧌䂷��

																														 //��l���摜�쐬
																														 //cv::Mat bin = separate_img > 255;
																														 //cv::rectangle(bin, cv::Rect((kinect.depthImage.cols - 1) / 2 - 10, (kinect.depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(255, 255, 255), 2);//���S�ʒu���擾���� �Ԃ��l�p��`��
																														 //cv::imshow("binary", bin);

			int val = (int)d * 4500 / 65535; //���x�l���~�����[�g���l�ɕύX�i�J�������쒆��ɋN���j
											 //�[�x�摜�ł͂Q�r�b�g�̃O���[�X�P�[���摜������
											 //65535�F�ō��̔��̃R�[�h256�~256
											 //4500�Fkinect��mm�P�ʂő���ł���ō�����

			int ddd;
			int hd, ld, hdd, ldd;
			if (zcount == 100000)
			{
				ddd = (int)d * 4500 / 65535;//���x�l���~�����[�g���l�ɕύX
				hd = ddd - levelh; //hd�@=�@kinect�ƒ��S�i�Ԃ��l�p�j�܂ł̋����@�|�@100  hd�̓g�b�v�|�W�V�����imm�j
				ld = ddd + levell; //ld�@=�@kinect�ƒ��S�i�Ԃ��l�p�j�܂ł̋����@�{�@100�@ld�̓_�E���|�W�V�����imm�j
				hdd = hd * 65535 / 4500;//�g�b�v�|�W�V�����̃~�����[�g�������x�ɕς���  hdd�̓g�b�v�|�W�V�����i���x�j
				ldd = ld * 65535 / 4500;//�_�E���|�W�V�����̃~�����[�g�������x�ɕς���  ldd�̓_�E���|�W�V�����i���x�j
			}
			else
			{
				ddd = zcount;
				hd = ddd - levelh;
				ld = ddd + levell;
				hdd = hd * 65535 / 4500;//�~�����[�g�������x�ɕς���
				ldd = ld * 65535 / 4500;
			}


			//���S����ł��߂����x�l��ۑ�����@�R�s�[�̏������s���Ă���H
			int maxh = 100000;
			for (int y = 0; y < kinect.depthImage.rows; y++) { //y��0����摜�̍������܂ł̊Ԃ�y++
				for (int x = 0; x < kinect.depthImage.cols; x++) { //x��0����摜�̉������܂ł̊Ԃ�x++
					UINT16 d = kinect.depthImage.at<UINT16>(y, x); //�摜�̂��ׂĂ̍����A���ׂẲ����i���ׂẴs�N�Z���j�ɂ����ċ��x�l�����߂�
					int dd = (int)d * 4500 / 65535; //���x�l���~�����[�g���l�ɕύX

					if (dd > hd && dd <= ld)//�ukinect�Ɠ؂̔w���܂ł̋����v���ukinect�ƒ��S�i�Ԃ��l�p�j�܂ł̋����|100�v���傫���A�ukinect�ƒ��S�i�Ԃ��l�p�j�܂ł̋���+100�v�ȉ��ł�������
											//�؂̔w�������܂��擾�ł����ꍇ
					{
						separate_img.at<UINT16>(y, x) = d;//���x�l��ۑ�
					}
					else//�؂̔w���ȊO���擾�����ꍇ
					{
						separate_img.at<UINT16>(y, x) = 0;
					}
				}
			}


			cv::Mat binary = separate_img > 10; //�摜���l�� �������l��10
												//cv::imshow("binary", binary);// �P�Ȃ��l���摜







												////////////////////////////////////////////////////////////////
												//�c��//
												//cv::imshow("test1", binary);
												//cv::dilate(binary,binary, cv::Mat(), cv::Point(-1, -1), 1); //8�ߖT�Ŗc������1��
												//cv::imshow("test2", binary);
												///////////////////////////////////////////////////////////////

												////////////////////////////////////////////////////////////////
												//�k��//
												//cv::imshow("test3", binary);
												//cv::erode(binary, binary, cv::Mat(), cv::Point(-1, -1), 1); //8�ߖT�ŏk������1��
												//cv::imshow("test4", binary);
												///////////////////////////////////////////////////////////////




												//�����t�H���W�[���Z�A�m�C�Y�̏���
			cv::Mat element(9, 1, CV_8U, cv::Scalar(1)); //element�F �\���v�f
			cv::Mat element1(3, 3, CV_8U, cv::Scalar(1));
			cv::Mat element3(0, 3, CV_8U, cv::Scalar(1));

			//cv::imshow("test5", binary);
			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element);//morphologyEx:���k�Ɩc������{���Z�Ƃ��ė��p���鍂�x�ȃ����t�H���W�[�ϊ����s���܂�
																	  //cv::MORPH_OPEN �����t�H���W�[���Z�̎�ށ@�I�[�v�j���O
																	  //�I�[�v�j���O�̌v�Z�@�o�́�open�i����,�\���v�f�j��dilate(erode(����,�\���v�f))
																	  //element�F �\���v�f
																	  //cv::imshow("test6", binary);  //���������A�������������ꂼ��L���Ȃ�����l���摜

			cv::morphologyEx(binary, binary, cv::MORPH_OPEN, element1, cv::Point(-1, -1), 1);
			//cv::imshow("test7", binary); //��蔒�������A�������������ꂼ��L���Ȃ�����l���摜

			std::vector<std::vector<cv::Point>>contours;//std::vector:�e�v�f�͐��`�ɁA������ۂ����܂܊i�[�����
														//vector�R���e�i�͉ϒ��z��Ƃ��Ď��������B�����I�ɗ̈�̊g�����s����B
			std::vector<cv::Vec4i>hierarchy;

			//cv::imshow("test8", binary);
			cv::Canny(binary, binary, 5, 225, 3); //�摜�̃G�b�W�����o
												  //5:�q�X�e���V�X�����݂��鏈����1�Ԗڂ�臒l
												  //225:�q�X�e���V�X�����݂��鏈����2�Ԗڂ�臒l
												  //3:Sobel() �I�y���[�^�̃A�p�[�`���T�C�Y
												  //cv::imshow("test9", binary); //���������̗֊s�𒊏o������l���摜

			cv::findContours(binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			//cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//cv::findContours:2�l�摜���̗֊s�����o���܂�
			//binary:���͉摜
			//contours:���o���ꂽ�֊s
			//hierarchy:�I�v�V�����D�摜�̃g�|���W�[�Ɋւ�������܂ޏo�̓x�N�g���D����́C�֊s���Ɠ������̗v�f�������܂�
			//CV_RETR_TREE:���ׂĂ̗֊s�𒊏o���C����q�\���ɂȂ����֊s�����S�ɕ\������K�w�\�����\�����܂�

			//cv::imshow("Contours", binary);		//�֊s��																					 //CV_CHAIN_APPROX_SIMPLE:�����E�����E�΂߂̐��������k���C�����̒[�_�݂̂��c���܂��D�Ⴆ�΁C�܂������ȋ�`�̗֊s���́C4�̓_�ɃG���R�[�h����܂��D
			//cv::imshow("binary4", binary); ���������̗֊s�����ׂ�������l���摜

			//int mindist = 100000;




			for (int i = 0; i < contours.size(); i++)
			{
				if (cv::contourArea(contours[i]) >= 100) //cv::contourArea:�֊s���͂ޗ̈�̖ʐς����߂܂�(�֊s�̒��_)
				{



					minRect = cv::minAreaRect(cv::Mat(contours[i])); //minAreaRect:�^����ꂽ 2�����_�W�����͂ށi��]���l�������j�ŏ��̋�`�����߂܂�
																	 //minRect�́A�؂̏ꏊ���m�F���A�摜����֊s�x�N�g���iu,v�j�����肷�邽�߂Ɏg�p
																	 //contours�F�֊s���i�������j



																	 //cv::minEnclosingCircle    //�^����ꂽ2�����_�W�����͂ލŏ��̉~�����߂܂�





					if (contours[i].size()>100)
					{
						minEllipse = cv::fitEllipse(cv::Mat(contours[i]));//cv::fitEllipse:��]��`��ȉ~�ɕϊ����鏈��
																		  //fitEllipse�F2�����̓_�W���ɑȉ~���t�B�b�e�B���O���܂�
																		  //minEllipse�F���̑ȉ~���t�B�b�e�B���O���ꂽ��]��`
																		  //contours�F�֊s���i�������j








						float x = minEllipse.center.x; //�ȉ~���S��x���W
						float y = minEllipse.center.y; //�ȉ~���S��y���W
						float aa = minEllipse.size.width / 2;    // width >= height
						float bb = minEllipse.size.height / 2;

						//int dist = sqrt(((x - kinect.depthImage.cols / 2)*(x - kinect.depthImage.cols / 2)) + ((y - kinect.depthImage.rows / 2)*(y - kinect.depthImage.rows / 2)));//���̕����͂Ȃ��Ă�OK
						//dist=��{(x-depthImage�̕�)/2�̓��@�{�@(y-depthImage�̍���)/2�̓�� }


						//if (dist < mindist) // mindist=100000
						if (x - bb <= kinect.depthImage.cols / 2 + 20 && x + bb >= kinect.depthImage.cols / 2 - 20 && y - aa <= kinect.depthImage.rows / 2 + 20 && y + aa >= kinect.depthImage.cols / 2 - 20)//�����`�������ɂ��邩�ǂ���
						{
							//mindist = dist;�@�@//mindist:�摜�̒��S�Ƒȉ~�̒��S�܂ł̋���//���̕����͂Ȃ��Ă�OK
							xc = x; yc = y; bc = i; a = aa; b = bb;					//xc:�ȉ~���S��x���W
																					//yc:�ȉ~���S��y���W
																					//bc: i = 0����contours�i�֊s���A�������j�̖{�����A�J��Ԃ����
																					//a:�ȉ~���a�̔���
																					//b:�ȉ~�Z�a�̔���


																					//fitEllipse�͉�]��`����ȉ~�ɑȉ~��`�悷�邽�߂Ɏg�p����܂��B
							minBEllipse = cv::fitEllipse(cv::Mat(contours[i]));//fitEllipse�F2�����̓_�W���ɑȉ~���t�B�b�e�B���O���܂�
																			   //minBEllipse�F���̑ȉ~���t�B�b�e�B���O���ꂽ��]��`




																			   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																			   //cv::Mat circles;     double param1 = 200; double param2 = 100; int minRadius = 0; int maxRadius = 10000000;
																			   //cv::HoughCircles(result, circles, CV_HOUGH_GRADIENT, 1, param1, param2, minRadius, maxRadius);
																			   //std::string a = "Data/circles_" + s3 + ".xyradius";//string:char�^������
																			   //fp = fopen(a.c_str(), "wt");
																			   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																			   //cv::Point center(cv::contourArea(contours[i][0]), cvRound(circles[i][1]));
																			   //int radius = cv::contourArea(contours[i][2]);
																			   // �~�̒��S��`�悵�܂��D
																			   //cv::circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
																			   // �~��`�悵�܂��D
																			   //cv::circle(result, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);




						}
					}
				}
			}




			if (bc > 0 && a>0 && b>0)
			{
				cv::ellipse(result, minBEllipse, cv::Scalar(0, 0, 255), 2, 8);//�ȉ~�̕`��
																			  //reslut�F�摜�@
																			  //minBEllipse�F2�����̓_�W���Ńt�B�b�e�B���O���Ă������ȉ~
																			  //cv::Scalar(0, 0, 255):�摜�̐F
																			  //2:�ȉ~�̘g���̑���
																			  //8:�ȉ~�̘g���̎��

				cv::circle(result, cv::Point(xc, yc), 3, cv::Scalar(0, 255, 0), CV_FILLED);//�΂̓_�i�~�j�̕`��
																						   //cv::Point(xc, yc)�F�Ԃ��ȉ~�̒��S�ɗ���悤�w��
																						   //3�F�~�̔��a
																						   //cv::Scalar(0, 0, 255):�_�̐F�i�΁j
																						   //CV_FILLED:

				std::string s = "Data/coordinate_" + s3 + ".xyz";//string:char�^������
				fp = fopen(s.c_str(), "wt");


				std::string ss = "Data/uv_" + s3 + ".txt";
				ff = fopen(ss.c_str(), "wt");//�t�@�C���������ݗp�@�I�[�v��
				fprintf(ff, "%d\n %d\n %d\n %d", (int)yc - a, (int)yc + a, (int)xc - b, (int)xc + b);
				fclose(ff);
				for (int x = xc - b; x < xc + b; x++)
				{
					for (int y = yc - a; y < yc + a; y++)
					{
						UINT16 d = separate_img.at<UINT16>(y, x);//�摜�� (x , y ) ���W�̉�f�ɃA�N�Z�X���� <UNIT16>(y,x)��x��y�����΂����ǂ����OK

						int z_coor = (int)d * 4500 / 65535;		//z���W�����߂�

						if (z_coor != 0)//z���W����x���W�Ay���W�����߂�
						{
							x_coor = (x - 512 / 2)*z_coor / 365; //x_coor:x���W
							y_coor = (y - 424 / 2)*z_coor / 365; //y_coor:y���W
																 //512px�~424px�̉𑜓x
																 //http://nw.tsuda.ac.jp/lec/kinect2/KinectV2_depth/
																 //http://nextsystemkinectblog.seesaa.net/article/360212446.html
																 //http://visitlab.jp/pdf/FastAccurateCalibrationKinect.pdf

																 //printf("X Y Z( %d , %d , %d )\n", x_coor, y_coor, z_coor);



							if (z_coor < maxh) //maxh = 100000
							{
								maxh = z_coor;
								//(u,v)�͓؂̔w���̂Ȃ��̃g�b�v�|�W�V�����̍��W
								//����(u,v)���W��p���āA��ɓ؂̃g�b�v�|�W�V������T���A�s���N�̓_���łĂ�悤�ɂ���
								u = ((x_coor * 365) + (256 * z_coor)) / z_coor;
								v = ((y_coor * 365) + (212 * z_coor)) / z_coor;
							}
							fprintf(fp, "%d %d %d\n", x_coor, y_coor, z_coor); //�t�@�C���������݁ix,y,z�j�f�[�^
						}
					}
				}
				cv::circle(result, cv::Point(u, v), 3, cv::Scalar(255, 0, 255), CV_FILLED);//�s���N�̓_�i�~�j�̕`��
																						   //cv::Point(u, v)�F�ȉ~�i�؂̔w���j�̒��ōł������ʒu�i�؂������猩�����ł������Ȃ�ӏ��j�Ƀs���N�̓_������悤�w��
																						   //3�F�~�̔��a
																						   //cv::Scalar(0, 0, 255):�_�̐F�i�s���N�j
																						   //CV_FILLED:
				fclose(fp);//�t�@�C������
						   //�����ŁA�s���N�̓_���؂̃g�b�v�|�W�V�������Ƃ��Ă��邩�`�F�b�N���Ă���B
				if (maxh != 100000 && val <= tmpStore + 3 && val >= tmpStore - 3)// && zcount>maxh)
				{
					zcount = maxh;
				}
				else
				{
					zcount = 100000;
				}
			}
			cv::imwrite(figD, kinect.depthImage);//cv::imwrite:�w�肵���t�@�C���ɉ摜��ۑ����܂�
												 //figD:�t�@�C����(png�`��)
												 //kinect.depthImage:�ۑ������摜

			cv::imwrite(figR, kinect.rgbImage);//kinect.rgbImage:RGB�J�����摜
											   //figR:�t�@�C����(png�`��)
											   //kinect.rgbImage:�ۑ������摜

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
		value = m_sliderCtrl.GetPos(); //�Ԋu��ς���
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
	std::vector<int>xx_val; //vector: �e�v�f�͐��`�ɏ�����ۂ����܂܊i�[�����Bvector�̊e�v�f�͘A�����Ĕz�u����邽�ߗv�f�̃����_���A�N�Z�X���\
	std::vector<int>yy_val; //yy_val�Ƃ������O�̔�
	std::vector<int>zz_val;
	std::vector<int>estimate_x; //estimate_x�Ƃ������O�̔�
	std::vector<int>estimate_y;
	std::vector<int>estimate_z;
	std::vector<int>en_x; //estimate_x�Ƃ������O�̔�
	std::vector<int>en_y;
	std::vector<int>en_z;

	AllocConsole();
	FILE* fd, *fs, *fa;
	freopen_s(&fd, "CONOUT$", "w", stdout);
	freopen_s(&fd, "CONIN$", "r", stdin);
	int x, y, z, r, g, b;




	FILE *fp;
	CString filepath, fn;
	CString         filter("XYZ Files (coordinate_*.xyz)|coordinate_*.xyz");//���W�t�@�C����ǂݍ���
	CFileDialog     selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();
		fn = selDlg.GetFileName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0); //string��vextor�e�v�f�͐��`�ɏ�����ۂ����܂܊i�[�����B
		fp = fopen(csTempmb.GetBuffer(0), "r");////�ǂݍ��ݗp�Ƃ��ăt�@�C�����I�[�v��
	}

	FILE *ff;
	CString         filter1("TEXT Files (circleData_*.txt)|circleData_*.txt");//�e�f�ʂ̍ō��ʒu��ǂݎ��
	CFileDialog     selDlg1(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter1);
	if (selDlg1.DoModal() == IDOK)
	{
		filepath = selDlg1.GetPathName();
		CStringA csTempmb(filepath.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		ff = fopen(csTempmb.GetBuffer(0), "r");//�ǂݍ��ݗp�Ƃ��ăt�@�C�����I�[�v��
	}







	CT2CA t(fn);
	std::string stream3(t); //string��vextor�e�v�f�͐��`�ɏ�����ۂ����܂܊i�[�����B
	std::string s3 = stream3;
	s3 = "Data/circle_" + s3 + ".txt"; //s3=Data�̒���circle_.txt�̂���
	fs = fopen(s3.c_str(), "wt"); //�������ݗp�Ƃ��ăt�@�C�����I�[�v��

								  /////////////////////�t�@�C���̏���///////////////////////////////////////////////////////



	int minx = 100000, maxx = -100000;	int sumz = 0;
	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {//fp�t�@�C���̒��g���X�L����
		xx_val.push_back(x);//xx_val�̔���x��ǉ�����
		yy_val.push_back(y);
		zz_val.push_back(z);
		if (minx > x) { minx = x; } // 100000>x�Ȃ��x��minx�ɑ��
		if (maxx < x) { maxx = x; } //-100000<x�Ȃ��x��maxx�ɑ��
									//fprintf(fs, "%d %d %d %d %d %d\n", x, y, z);
	}

	while ((fscanf(ff, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF)//ff�t�@�C���̒��g���X�L����
	{
		estimate_x.push_back(x);//estimate_x�̔���x��ǉ�����
		estimate_y.push_back(y);
		estimate_z.push_back(z);
	}
	/////////////////�t�@�C������V�������ɕK�v�Ȓl���i�[/////////////////////////////////////



	float sum = 0, count = 0;
	for (int j = minx; j <= maxx; j += value)//100000<j<-100000�@���̋�Ԃ̎�j=j+30
	{
		std::vector<int>divide_x; //vector: �e�v�f�͐��`�ɏ�����ۂ����܂܊i�[�����Bvector�̊e�v�f�͘A�����Ĕz�u����邽�ߗv�f�̃����_���A�N�Z�X���\
		std::vector<int>divide_y;
		std::vector<int>divide_z;
		int index = 0;

		for (int i = 0; i <= xx_val.size(); i++)//0<i<xx_val�̔��̒��ɂ���l�܂�i++
		{
			if (xx_val[i] > j - 3 && xx_val[i] < j + 3 && xx_val[i] <= 600)// && yy_val[i]>-220 && zz_val[i]>30)		//j�l���狗��3�����ix���W�����j// 5��150 OK
			{
				divide_x.push_back(xx_val[i]);//xx_val[i]
				divide_y.push_back(yy_val[i]);
				divide_z.push_back(zz_val[i]);
			}
		}

		float bestr = 0; //best�Ȕ��a��0�Ƃ���
		float bestx = 0;//float�^�@���������锠
		float besty = 0;
		float bestcount = 0;
		float last = 1;

		if (divide_x.size() > 10)//divide_x�̔��̒��ɂ���l��10���傫����
		{
			for (int p = 0; p < 100; p++)//0<p<100�܂�p++
			{
				int* arr = new int[5];//int�^�̔z��arr�i����5���j��ݒ�
				for (int s = 0; s < 5; s++)
				{
					int random = rand() % divide_x.size();// rand() �͋�� (0,1) �̈�l���z����������1��divide_x�̔��̒��ɕԂ��܂��B
														  //�~�̃����_���ȓ_�𓾂�
					arr[s] = random;//�z��5�̔��̒��Ƀ����_���Ɏ��o�����_�̒l���
				}


				cv::Mat cirMat = cv::Mat_<double>(5, 3);
				cv::Mat cmMat = cv::Mat_<double>(5, 1);

				for (int n = 0; n < 5; n++)//�~�̃p�����[�^��������
										   //�����łƂ肠����5�̃����_���ȃf�[�^���~�̕������ɑ��
				{
					cirMat.at<double>(n, 0) = (float)divide_y[arr[n]]; //�����Ń����_���Ɏ��o����5��x���W��(y,z)���W��cirMat�ɑ������B
					cirMat.at<double>(n, 1) = (float)divide_z[arr[n]];
					cirMat.at<double>(n, 2) = (float)last;
					cmMat.at<double>(n, 0) = (float)(-(divide_y[arr[n]] * divide_y[arr[n]]) - (divide_z[arr[n]] * divide_z[arr[n]]));
				}

				//std::cout << cirMat << std::endl << std::endl ;
				//std::cout << cmMat << std::endl << std::endl;


				cv::Mat inverx;//(3�~5�s��)
				inverx = cirMat.inv(cv::DECOMP_SVD);
				//std::cout << inverx << std::endl << std::endl << std::endl << std::endl;
				cv::Mat cresult = inverx*cmMat;//�i3�~1�s��j

											   //�����ŁA����cresult�s��܂�~�̃p�����[�^�����܂�B
				double a = cresult.at<double>(0, 0);
				double b = cresult.at<double>(1, 0);
				double c = cresult.at<double>(2, 0);
				//printf("%d %d %d \n", a, b, c);

				double x1, y1, r; //�~�ɂ��Ē��S���W(x1,y1)���a���̉~��\��
				x1 = -a / 2;//x�̈ʒu
				y1 = -b / 2;//y�̈ʒu
				r = sqrt(pow(x1, 2) + pow(y1, 2) - c);//���a����(x1^2 + y1^2 - c)

				if (r > 200)continue;
				double d;
				int cou = 0;


				for (int z = 0; z < divide_x.size(); z++)//0<z<divide_x�̔��̒��ɂ���l�@z++
				{
					d = fabs(sqrt(pow(divide_y[z] - x1, 2) + pow(divide_z[z] - y1, 2)) - r);//�`�F�b�N�v���Z�X
																							//fabs=��Βl
																							//��((divide_y[z]-x1)^2 + (divide_z[z]-y1^2)�j-r
					if (d < threshold)//����d<1�Ȃ��
					{
						cou++; //cou�ɂP���������B�܂�J�E���g����Ă����B
					}
				}


				if (cou > bestcount)//����cou>0�Ȃ��
									//�G���[���a�̒l���������Ȃ�
				{
					//�~��`�����߂̃f�[�^���ŏI����
					bestr = r;
					bestx = x1;
					besty = y1;
					bestcount = cou;
				}
			}

			double surf_area = (float)Pi*bestr*bestr;//�\�ʐς�������
													 //std::cout << bestr << std::endl;
			sum += surf_area;
			count++;
			int c = 0;
			for (float m = -4; m <= 4; m += 0.1)//�~��`���t�@�C���ɕۑ�����
			{
				//�_�S��
				float cx = bestx + (bestr*sin(m));
				float cy = besty + (bestr*cos(m));
				//���̌������m�F����x�A�c��y�ō����Ă�H
				//float cx = bestx + (bestr*cos(m)); //�~�̒��Sx���W+���iy���W�j
				//float cy = besty + (bestr*sin(m)); //�~�̒��Sy���W+�ڌ�(x���W)
				fprintf(fs, "%d %f %f\n", j - 3, cx, cy);//j-3=x���W
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
	//fu = fopen(csTempmb.GetBuffer(0), "r");//�ǂݍ��ݗp�Ƃ��ăt�@�C�����I�[�v��
	//}

	//int enx;
	//printf("x���W�́H \n");
	//scanf("%d", &enx);


	//while ((fscanf(fu, "%d %lf %lf ",&x ,&y, &z)) != EOF)//ff�t�@�C���̒��g���X�L����
	//ff�t�@�C����%lf������y��z���������X�L��������Ă��Ȃ�
	//{
	//printf("%d %f %f\n",x, y, z);
	//en_x.push_back(x);//estimate_x�̔���x��ǉ�����
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
	//printf("%d��������",enx);

	//}






	//printf("finish");
}


//while ((fscanf(fs, "%d %f %f", &x, &y, &z)) != EOF)//ff�t�@�C���̒��g���X�L����
//{	
//printf("%d\n", &x);
//en_x.push_back(x);//estimate_x�̔���x��ǉ�����
//en_y.push_back(y);
//en_z.push_back(z);
//printf("%d\n", &x);

//if (enx == x) {
//printf("���͂���������%d�ł��B\n", enx);
//printf("%d %f %f\n", en_x.size(), en_y.size(), en_z.size());
//}

//}

//�T�[�`���@
//http://blog.systemjp.net/entry/20100519/p3	
//vector<long> v_;
//v_list.push_back(100);
//v_list.push_back(200);
//v_list.push_back(300);
//vector< long >::iterator cIter = find(v_.begin(), v_.end(), 200);
//if (cIter != v_.end()) {
//printf("200�͑��݂��Ă܂�");
//}


//std::cout << sum / count << std::endl;
//FreeConsole();






















void CPigSeparationDlg::OnBnClickedButton4()///////////////Weight Estimation//////////////////////
{
	AllocConsole();
	FILE *fv;
	freopen_s(&fv, "CONOUT$", "w", stdout);
	freopen_s(&fv, "CONIN$", "r", stdin);

	//�����Ԃ��t�@�C�����ɓ����
	char date[64];
	time_t timer = time(NULL);
	strftime(date, sizeof(date), "%Y%m%d_%H%M%S", localtime(&timer));//���Ԃ𕶎���Ƃ��ď����ݒ肷��
																	 /* ���ݎ����̎擾 */
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
	//��





	FILE *fp, *ff, *fa, *fb, *fc;

	int levelh = 100, levell = 200;
	cv::Mat separate_img;
	cv::Mat measure_img, result;
	int x_coor, y_coor;
	UpdateData();//�e�L�X�g�{�b�N�X���當������擾
	CString str = m_control;
	CT2CA t(str);//CString���當����ւ̕ϊ�
	std::string stream3(t);
	std::ostringstream convert;
	convert << imgC;
	std::string s3 = "b" + stream3 + convert.str();
	//std::string figD = "Data/depth" + s3 + ".png";
	//std::string figR = "Data/RGB" + s3 + ".png";
	UpdateData(FALSE);
	//std::string s, ss;
	std::string s1, s2;


	std::vector<int>xx_val;//�t�@�C���ǂݍ��݃f�[�^���i�[����x
	std::vector<int>yy_val;// �t�@�C���ǂݍ��݃f�[�^���i�[����y
	std::vector<int>zz_val;//�t�@�C���ǂݍ��݃f�[�^���i�[����z
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
			if (zcount == 100000)//���S�ʒu���狭�x�l�����
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

			//���S����ł��߂����x�l���i�[����
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


			cv::Mat binary = separate_img > 100;//�摜���l���A�������l��100
			std::vector<std::vector<cv::Point>>contours; //�X�g���[�N�̗֊s�_
														 //std::vector �e�v�f�͐��`�ɁA������ۂ����܂܊i�[�����ϒ��z��
			std::vector<cv::Vec4i>hierarchy;
			cv::Canny(binary, binary, 200, 225, 3);//�G�b�W���o
			cv::imshow("canny", binary);
			cv::findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//�L���j�[�G�b�W�̌��ʂ���֊s��������
			int mindist = 100000;

			for (int i = 0; i < contours.size(); i++)
			{
				if (cv::contourArea(contours[i]) >= 100)
				{
					minRect = cv::minAreaRect(cv::Mat(contours[i]));//�֊s�̈�
					cv::RotatedRect minEllipse;
					if (contours[i].size()>100)//release small size
					{
						minEllipse = cv::fitEllipse(cv::Mat(contours[i]));//�ȉ~�ɕύX
						float x = minEllipse.center.x; //�ȉ~���S��x���W
						float y = minEllipse.center.y; //�ȉ~���S��y���W
						float aa = minEllipse.size.width / 2;  //�ȉ~�i��]��`�j�̉����ix�����ʁj��2 �@ width >= height
															   //�uwidth�̂Ƃ���ɃJ�[�\���������Ă����vcv::Size2f��float�^�Œ�`����Ă���B���ꂼ�ꃁ���o width �� height ����`�̕��ƍ�����\���D
						float bb = minEllipse.size.height / 2; //�ȉ~�i��]��`�j�̍����iy�����ʁj��2

															   //int dist = sqrt(((x - kinect.depthImage.cols / 2)*(x - kinect.depthImage.cols / 2)) + ((y - kinect.depthImage.rows / 2)*(y - kinect.depthImage.rows / 2)));

															   //if (dist < mindist)
						if (x - bb <= kinect.depthImage.cols / 2 + 20 && x + bb >= kinect.depthImage.cols / 2 - 20 && y - aa <= kinect.depthImage.rows / 2 + 20 && y + aa >= kinect.depthImage.cols / 2 - 20)//�����`�������ɂ��邩�ǂ���
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
					//0���ȉ~�̒��S��x���W��depth�摜�̉���-1�@���@0���ȉ~�̒��S��y���W��depth�摜�̏c��-1�@���@�ȉ~�̑傫����0�ł͂Ȃ�
				{
					cv::ellipse(result, minBEllipse, cv::Scalar(0, 0, 255), 2, 8);//�ȉ~��`��
																				  //cv::circle(result, cv::Point(xc, yc), 3, cv::Scalar(0, 255, 0), CV_FILLED);//�ȉ~�̒��S
																				  //result�F�~��`�悷��摜
																				  //cv::Point(xc,yc)�F�~�̒��S���W
																				  //3:�~�̔��a
																				  //cv::Scalar(0, 255, 0)�F�~�̐F





																				  //s4 = "Data/coordinate_" + s3 + ".xyz"; //s4�͓_�Q�f�[�^
					fp = fopen(s5.c_str(), "wt");
					xx_val.clear();//xx_val�F�t�@�C���ǂݍ��݃f�[�^���i�[����x
					yy_val.clear();//yy_val�F�t�@�C���ǂݍ��݃f�[�^���i�[����y
					zz_val.clear();//zz_val�F�t�@�C���ǂݍ��݃f�[�^���i�[����z
					c = 0;
					//s7 = "Data/uv_" + s3 + ".txt";//s7��xc - a,xc + a,yc - b,yc + b�@��4�̒l���i�[
					ff = fopen(s8.c_str(), "wt");
					fprintf(ff, "%d\n %d\n %d\n %d", (int)yc - a, (int)yc + a, (int)xc - b, (int)xc + b);//uv_�t�@�C����.txt�ɕۑ������4�̐���
																										 //(xc,yc)�͑ȉ~�̒��S���W
																										 //a:�ȉ~���a�̔���
																										 //b:�ȉ~�Z�a�̔���
					fclose(ff);
					/*ue = yc - a;
					ve = yc + a;
					we = xc - b;
					he = xc + b;*/


					ue = yc - a;
					we = yc + a;
					ve = xc - b;
					he = xc + b;
					//�t�@�C����xyz�l���i�[����



					for (int x = xc - b; x < xc + b; x++)
					{
						for (int y = yc - a; y < yc + a; y++)
						{
							UINT16 d = separate_img.at<UINT16>(y, x);//�摜�� (x , y ) ���W�̉�f�ɃA�N�Z�X���� <UNIT16>(y,x)��x��y�����΂����ǂ����OK
							int z_coor = (int)d * 4500 / 65535;		//z���W�擾
							if (z_coor != 0)//z���W����x���W�Ay���W�����߂�
							{
								x_coor = (x - 512 / 2)*z_coor / 365; //x_coor:x���W
								y_coor = (y - 424 / 2)*z_coor / 365; //y_coor:y���W
								if (z_coor < maxh)
								{
									maxh = z_coor;//z_coor:z���W
												  //(u,v)�͓؂̔w���̂Ȃ��̃g�b�v�|�W�V�����̍��W
									u = ((x_coor * 365) + (256 * z_coor)) / z_coor;
									v = ((y_coor * 365) + (212 * z_coor)) / z_coor;
								}
								xx_val.push_back(x_coor);
								yy_val.push_back(y_coor);
								zz_val.push_back(z_coor);
								fprintf(fp, "%d %d %d\n", x_coor, y_coor, z_coor);//x,y,z���W���t�@�C���ɕۑ�
								c++;
							}
						}
					}
					//cv::circle(result, cv::Point(u, v), 3, cv::Scalar(255, 0, 255), CV_FILLED);
					fclose(fp);

					if (maxh != 100000 && val <= tmpStore + 3 && val >= tmpStore - 3)// && zcount>maxh)//���S���x���ω����Ă��邩���Ă��Ȃ���
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


		cv::Mat xMat4 = cv::Mat_<double>(1000, 5);//�f�ʂɍł��߂��ʒu�Ɋi�[����A4�p���[�A��ʐ}
		cv::Mat xMat2 = cv::Mat_<double>(1000, 3);//�f�ʂɍł��߂��ʒu�Ɋi�[����A2�d�́A���ʐ}
		cv::Mat yMat = cv::Mat_<double>(1000, 1);
		cv::Mat zMat = cv::Mat_<double>(1000, 1);

		int *xlength = new int[100];



		for (int i = 0; i <xx_val.size(); i++)
		{
			if (minx > xx_val[i]) { minx = xx_val[i]; } //�t�@�C���ǂݍ��݃f�[�^���i�[����x���� 100000��菬�����ꍇ�@���@���̔��̖��O��minx
			if (maxx < xx_val[i]) { maxx = xx_val[i]; } //�t�@�C���ǂݍ��݃f�[�^���i�[����x����-100000���傫���ꍇ�@���@���̔��̖��O��maxx
			if (minz > zz_val[i]) { minz = zz_val[i]; } //�t�@�C���ǂݍ��݃f�[�^���i�[����z���� 100000��菬�����ꍇ�@���@���̔��̖��O��minz
			if (maxz < zz_val[i]) { maxz = zz_val[i]; } //�t�@�C���ǂݍ��݃f�[�^���i�[����z����-100000���傫���ꍇ�@���@���̔��̖��O��maxz
			sumz = sumz + zz_val[i];                      //sumz=0+z���W�̑S�Ẵf�[�^
		}

		int avgz = sumz / zz_val.size();
		//std::cout << "Average=" << avgz << std::endl;

		int datasize1 = 0;
		int curxmx = -100000, curxmn = 100000, curymx = -100000, curymn = 100000, curzmx = -100000, curzmn = 100000;
		int earpos = 0;
		for (int j = minx; j <= maxx; j += value)//maxx
		{
			int seg_min = 100000;
			std::vector<int>divide_x;//���̒f�ʃf�[�^��ۑ�����
			std::vector<int>divide_y;
			std::vector<int>divide_z;
			double ymax = -100000, ymin = 100000;
			float last = 1;
			int index = 0;
			for (int i = 0; i <= xx_val.size(); i++)
			{
				if (xx_val[i] >= j - 3 && xx_val[i] < j + 3 && zz_val[i] > 0)//vminx�Ƃ�3�̈Ⴂ��I��
				{
					divide_x.push_back(xx_val[i]);//xx_val[i]
					divide_y.push_back(yy_val[i]);
					divide_z.push_back(zz_val[i]);
					if (yy_val[i] < ymin) { ymin = yy_val[i]; }
					if (yy_val[i] > ymax) { ymax = yy_val[i]; }
				}
			}



			if (divide_y.size() > 10)//�@�m�C�Y����
			{
				cv::Mat fmax = cv::Mat_<double>(divide_y.size(), 3);//2�̓d�͐���Ȑ���`���A�f�W�^���덷�����炷
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


				for (int i = ymin; i < ymax; i += 1)//���ς���Ȑ���̒��_��z�l�����߂�
				{
					double valz = zresult.at<double>(0, 0)*i*i + zresult.at<double>(1, 0)*i + zresult.at<double>(2, 0);
					if (valz < minz)
					{
						minz = valz;
						valy = i;
					}
				}



				double mind = 1000000000.00;//���ϋȐ��ƒf�ʂƂ̊Ԃ̍ł��߂��ʒu��������
				for (int s = 0; s < divide_x.size(); s++)
				{
					double dist = sqrt(((valy - divide_y[s])*(valy - divide_y[s])) + ((minz - divide_z[s])*(minz - divide_z[s])));
					if (mind >= dist) { mind = dist; index = s; }
				}

				if (divide_z[index] < avgz + 20)
				{
					//store each top position
					fprintf(fb, "%d %d %d %d %d %d\n", divide_x[index], divide_y[index], divide_z[index], 255, 255, 0);

					//4�s��
					xMat4.at<double>(ic, 0) = (double)divide_x[index] * divide_x[index] * divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 1) = (double)divide_x[index] * divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 2) = (double)divide_x[index] * divide_x[index];
					xMat4.at<double>(ic, 3) = (double)divide_x[index];
					xMat4.at<double>(ic, 4) = (double)1;

					//2�s��
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



		for (float i = curxmn; i <curxmx; i++)//��������܂ł̓؂̒�����������
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
					//distance=distance+��i^2+yy^2+zz^2
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
		cv::imshow("Weight Estimation", result); //�w�肵���E�B���h�EWeight Estimation���ɁA�摜result��\�����܂�
		cv::imwrite(s6, result);//�w�肵���t�@�C���ɉ摜��ۑ����܂��D
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
	std::vector<int>xx_val;//�t�@�C���ǂݍ��݃f�[�^���i�[����x�z��
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
	CString filepath, fn;//CString�Fchar�^�܂���wchar_t�^���i�[�����

	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();//xyz�f�[�^�t�@�C����I������Ƃ���Ɉړ��ł���
		fn = selDlg.GetFileName();

		CStringA csTempmb(filepath.GetBuffer(0));//CStringA�Fchar�^���i�[�����
		std::string filename = csTempmb.GetBuffer(0);
		fp = fopen(csTempmb.GetBuffer(0), "r"); // �t�@�C����ǂݍ��ݗp�Ƃ���Open
												//�ǂݍ��񂾉摜�t�@�C���FcsTempmb.GetBuffer(0)
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
	//xyz�f�[�^�i�[���쐬
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


	if (selDlg.DoModal() == IDOK)//xyz�f�[�^�ɂ���
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

	if (selDlgl.DoModal() == IDOK)//uv�f�[�^�ɂ���
	{
		filepathl = selDlgl.GetPathName();
		CStringA csTempmb(filepathl.GetBuffer(0));
		std::string filename = csTempmb.GetBuffer(0);
		fc = fopen(csTempmb.GetBuffer(0), "r");
	}

	////////////////�����܂ł��t�@�C���̓ǂݍ���//////////////////////////////////////////////////////




	int k[4]; int c = 0;//stroe uv data in int k array
						//int k�z��̃X�g���[�Nuv�f�[�^

	while ((fscanf(fc, "%d", &x)) != EOF) {//uv�f�[�^�̓������t�@�C�����X�L����
		std::cout << x << std::endl;//uv�f�[�^4���o��
		k[c] = x; c++;
	}
	fclose(fc);//uv�f�[�^�̓������t�@�C�������
			   //std::cout << k[0] << " " << k[1] << " " << k[2] << " " << k[3] << std::endl;



	CT2CA t(fn);
	std::string stream3(t);


	std::string s9 = stream3;
	s9 = "Data/data_" + s9 + ".txt";
	ff = fopen(s9.c_str(), "wt");//�t�@�C�����������ݗp�Ƃ���Open
								 //�t�@�C���̂���ꏊ��pigSeparation��Data�@�t�@�C����data.txt

	std::string s10 = stream3;
	s10 = "Data/taichox_" + s10 + ".txt";
	fw = fopen(s10.c_str(), "wt");

	std::string s11 = stream3;
	s11 = "Data/taichoy_" + s11 + ".txt";
	fq = fopen(s11.c_str(), "wt");

	std::string s12 = stream3;
	s12 = "Data/taichoz_" + s12 + ".txt";
	fg = fopen(s12.c_str(), "wt");

	std::string s3 = stream3;//s3=coordinate_�H(��ba0.xyz)
	s3 = "Data/cross_section" + s3 + ".xyz";//�t�@�C����cross_sectioncoordinate
											//�؂�30mm���f�ʂ̉~�ߎ��i���~�̂݁j

	fs = fopen(s3.c_str(), "wt");//�t�@�C�����������ݗp�Ƃ���Open





	std::string s4 = stream3;
	s4 = "Data/circleData_" + s4 + ".txt";//�t�@�C����circleDatacoordinate
	fa = fopen(s4.c_str(), "wt");//�t�@�C�����������ݗp�Ƃ���Open
	filename = s3;//�z��filename��s3����


				  ///////////////�����܂ł��V�����t�@�C���̏����Ȃ�///////////////////////////////////////////////////



	int minx = 100000, maxx = -100000, minz = 100000, maxz = -100000;
int sumz = 0;  
	int ic = 0;


	//////////////////////////////////
	//int curxmx = -100000;
	//int curxmn = 100000;
	//int curxmn = 10;
	//int curymx = -100000, curymn = 100000, curzmx = -100000, curzmn = 100000;
	/////////////////////////////////


	cv::Mat xMat4 = cv::Mat_<double>(1000, 5);//4��������
	cv::Mat xMat2 = cv::Mat_<double>(1000, 3);//2��������
	cv::Mat yMat = cv::Mat_<double>(1000, 1);//4��������
	cv::Mat zMat = cv::Mat_<double>(1000, 1);//2��������
	int *xlength = new int[100];//int�^�̔z��xlength�i����100���j��ݒ�

								//xyz�f�[�^��}��
	while ((fscanf(fp, "%d %d %d %d %d %d", &x, &y, &z, &r, &g, &b)) != EOF) {
		xx_val.push_back(x);
		yy_val.push_back(y);
		zz_val.push_back(z);
		if (minx > x) { minx = x; }  //100000>x���W�Ȃ��x=100000 �܂�x���W�̍ŏ��l��100000
		if (maxx < x) { maxx = x; }  //-100000<x���W�Ȃ��x=-100000 �܂�x���W�̍ő�l��-100000
		if (minz > z) { minz = z; }  //100000>z���W�Ȃ��z=100000 �܂�z���W�̍ŏ��l��100000
		if (maxz < z) { maxz = z; }  //-100000<z���W�Ȃ��z=-100000 �܂�z���W�̍ő�l��-100000
									 //sumz += z;
	}

	//int datasize1 = 0;
	//int avgz = sumz / zz_val.size();//z���W�̕��ρ�z���W�̍��v�l��z���W�̌�
	//std::cout << "Average z=" << avgz << std::endl;//z���W�̕��ς��o��






	//��������f�ʃf�[�^�ɂ���//////////////////////////////////////////////////////////////
	for (int j = minx; j <= maxx; j += value)//100000<j<-100000 j=j+value�܂�j=j+30
	{

		int seg_min = 100000;
		std::vector<int>divide_x;//�f�ʃf�[�^��ۑ����邽�߂̔z��
		std::vector<int>divide_y;
		std::vector<int>divide_z;
		double ymax = -100000, ymin = 100000;
		float last = 1;
		int index = 0;

		for (int i = 0; i <= xx_val.size(); i++)//x���W�̒f�ʃf�[�^���i�[����Ă���z���1����X�L����
		{
			//�z����X�L�������Ă�������
			if (xx_val[i] >= j - 3 && xx_val[i] < j + 3)//�؂̔��~��`���ۂ�30mm����؂邽�߂ɁA�ux���W>j-3 ���� x���W<j+3 �Ȃ�v���K�v
														//if (xx_val[i] >= j - 30 && xx_val[i] < j + 30)
			{
				divide_x.push_back(xx_val[i]);//x���W��f�ʃf�[�^�z��Ɋi�[
				divide_y.push_back(yy_val[i]);//y���W��f�ʃf�[�^�z��Ɋi�[
				divide_z.push_back(zz_val[i]);//z���W��f�ʃf�[�^�z��Ɋi�[

				if (yy_val[i] < ymin) { ymin = yy_val[i]; }//�����@y���W<100000�Ȃ�y=100000 �܂�y���W�̍ŏ��l��100000
				if (yy_val[i] > ymax) { ymax = yy_val[i]; }//�����@y���W>-100000�Ȃ�y=-100000 �܂�y���W�̍ő�l��-100000
			}
		}




		if (divide_y.size() > 10)//�f�ʃf�[�^��y���W>10�Ȃ��
								 //if (divide_y.size() > 100)//��L�Ə����ς���100�ɂ�����A�؁i���~�j�̒������Z���Ȃ�B�܂�A���Ɣ����\������Ȃ��Ȃ���
		{
			cv::Mat fmax = cv::Mat_<double>(divide_y.size(), 3);//2���Ȑ���`���A�f�W�^���덷�����炷
																//divide_y.size() �~ 3 �s��			
			cv::Mat fz = cv::Mat_<double>(divide_y.size(), 1);//divide_y.size() �~ 1 �s��

			for (int n = 0; n < divide_x.size(); n++)//�f�ʃf�[�^��x���W��0���瑶�݂���f�[�^�S�Ă܂�++
			{
				//"cross_section.xyz�t�@�C���ɏ�������" 
				//�؂̔��~�i�F�j�f�[�^���t�@�C���ɏ�������
				fprintf(fs, "%d %d %d %d %d %d\n", divide_x[n], divide_y[n], divide_z[n], 100, 255, 255);


				fmax.at<double>(n, 0) = (double)divide_y[n] * divide_y[n];//cv::Mat fmax�F�񎟋Ȑ���`������
				fmax.at<double>(n, 1) = (double)divide_y[n];
				fmax.at<double>(n, 2) = (double)last;
				fz.at<double>(n, 0) = (double)divide_z[n];
			}


			if (divide_x.size() > 10)//divide_x�F�f�ʃf�[�^x���W�̔��i�z��j��10
			{
				cv::Mat inverx; //�s����쐬�ifmax�̋t�s������邽�߂Ɂj(3�~divide_y.size)
				int index = 0;
				inverx = fmax.inv(cv::DECOMP_SVD);//fmax�̋t�s����쐬	
				cv::Mat zresult = inverx*fz; //zresult�s��fmax�t�s��~fz (3�~1)


				double minz = 100000.000;
				double valy;


				for (int i = ymin; i < ymax; i += 1)//���ς���Ȑ���̒��_��z�l�����߂�
													//���ς���Ȑ��Ƃ͓؂̍ł��g�b�v���W���Ȃ������ɋ߂�����
													//�{���Ƃ̈Ⴂ��2���Ȑ��ŋߎ����Ă��邱�ƁB�m�[�g���m�F
				{
					double valz = zresult.at<double>(0, 0)*i*i + zresult.at<double>(1, 0)*i + zresult.at<double>(2, 0);
					if (valz < minz)
					{
						minz = valz;
						valy = i;
					}
				}



				double mind = 1000000000.00;//���ϋȐ��ƒf�ʂƂ̊Ԃ̍ł��߂��ʒu��������
				for (int s = 0; s < divide_x.size(); s++)
				{
					double dist = sqrt(((valy - divide_y[s])*(valy - divide_y[s])) + ((minz - divide_z[s])*(minz - divide_z[s])));
					if (mind >= dist) { mind = dist; index = s; }
				}


				//�e�g�b�v�|�W�V�������i�[����
			//	if (divide_z[index] >= avgz - 20 && divide_z[index] < avgz + 20)
					//if (divide_z[index] >= avgz - 30 && divide_z[index] < avgz + 23)
				{
					//�G���[���o�鎞�͂���fa�t�@�C���icircleDatacoordinate�j�ɉ��̒l�������Ă��Ȃ������H
					//���F���_�ŕ\�������B���̓_�̒��Ԃ�ʂ�悤�ɓ񎟋Ȑ�����Ă��邩���H
					fprintf(fa, "%d %d %d %d %d %d\n", divide_x[index], divide_y[index], divide_z[index], 255, 255, 0);//(���F���_��x���W,y���W,z���W,�_�̐FRGB���F)

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

					xlength[ic] = divide_x[index];//xlength�Fint�^�̔z��
												  //�i����100���j��ݒ肵�Ă邩��ic��100���܂ł��ȁH
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
						inverx4 = xMat4.inv(cv::DECOMP_SVD);// (cv::DECOMP_SVD)�F�t�s��
															//std::cout << inverx4 << std::endl << std::endl << std::endl ;
						inverx2 = xMat2.inv(cv::DECOMP_SVD);
						cv::Mat resultz = inverx4*zMat;
						cv::Mat resulty = inverx2*yMat;
						int tempx = 0, tempy = 0, tempz = 0;//������
						float distance = 0.0;

						//��������܂ł̓؂̒�����������
						for (float i = 0; i < val; i++)//i��100000����-99999�ɂȂ�܂�i++
													   //i=100000 i<-100000 i++
						{
							int yy = (int)i * i * resulty.at<double>(0, 0) + i * resulty.at<double>(1, 0) + resulty.at<double>(2, 0);
							int zz = (int)i * i * i * i * resultz.at<double>(0, 0) + i * i * i * resultz.at<double>(1, 0) + i * i * resultz.at<double>(2, 0) + i * resultz.at<double>(3, 0) + resultz.at<double>(4, 0);

							//float u = ((i * 365) + (256 * zz)) / zz;
							//float v = ((yy * 365) + (212 * zz)) / zz;
							//if (u >= k[2] && u <= k[3] && v >= k[0] && v <= k[1])
							{
								//if (i > curxmn)//����i>100000�Ȃ��
								//{

								//distance += sqrt(((i - tempx)*(i - tempx)) + ((yy - tempy)*(yy - tempy)) + ((zz - tempz)*(zz - tempz)));
								//distance=��( i^2 + yy^2 + zz^2 ) ���_�Ƃ�����W�܂ł̋���
								//}

								tempx = i;  //tempx=�̒��̐���`�����߂�x���W�f�[�^
								tempy = yy; //tempy=�̒��̐���`�����߂�y���W�f�[�^ 
								tempz = zz; //tempz=�̒��̐���`�����߂�z���W�f�[�^

								fprintf(ff, "%d %d %d %d %d %d\n", (int)i, yy, zz, 255, 0, 255);//ff�Fdata.txt�t�@�C���̂���
																								//data.txt�t�@�C���i�̒��̐���`�����߂̃f�[�^�j�Ƀf�[�^����������
								fprintf(fw, "%d\n", (int)i);
								fprintf(fq, "%d\n", yy);
								fprintf(fg, "%d\n", zz);
								//printf("%d %d\n",tempx,(int)i);//tempx=�̒��̐���`�����߂�x���W�f�[�^=i
								//printf("%d %d\n", tempx, i);   //tempx=�̒��̐���`�����߂�x���W�f�[�^�@i=0
								//printf("%d %d\n", tempy, yy);//tempy=�̒��̐���`�����߂�y���W�f�[�^=yy
							}
						}
					}
					////////////////////////////////////////////////////////











				}
			}
		}
	}

	////////////////////�����܂ł��g�b�v���W�������ăt�@�C���Ɋi�[/////////////////////////////////////////////////////





	//distance = distance / 10;

	//truLen = distance;//���Ƃ���truLen = 0���������̂�distance��������


	UpdateData();
	CString aup;
	//aup.Format(_T("%f"), distance);//%f�F���������_���Ƃ��ďo�͂���

	//std::cout << "distance=" << distance << std::endl;

	m_length = aup;
	UpdateData(FALSE);

	//int dataAx, dataAy, dataAz;
	//int dataBx, dataBy, dataBz;
	//int dataCx, dataCy, dataCz;
	//int dataDx, dataDy, dataDz;
	//printf("���O���ix,y,z�j�́H \n");
	//scanf("%d %d %d", &dataAx, &dataAy, &dataAz);
	//printf("�E�O���ix,y,z�j�́H \n");
	//scanf("%d %d %d", &dataBx,&dataBy, &dataBz);
	//printf("���㑫�ix,y,z�j�́H\n");
	//scanf("%d %d %d", &dataCx, &dataCy, &dataCz);
	//printf("�E�㑫�ix,y,z�j�́H\n");
	//scanf("%d %d %d", &dataDx, &dataDy, &dataDz);

	//CString filepatho;
	//CString         filtero("Point Files (point_*.txt)|point_*.txt");
	//CFileDialog     selDlgo(TRUE, NULL, NULL, OFN_HIDEREADONLY, filtero);

	//if (selDlgo.DoModal() == IDOK)//point�f�[�^�ɂ���
	//{
	//filepatho = selDlgo.GetPathName();//uv�f�[�^�t�@�C����I������Ƃ���Ɉړ��ł���
	//CStringA csTempmb(filepatho.GetBuffer(0));
	//std::string filename = csTempmb.GetBuffer(0);
	//fo = fopen("Point.txt", "w");//�t�@�C����ǂݍ��ݗp�Ƃ���Open
	//�ǂݍ��񂾉摜�t�@�C���FcsTempmb.GetBuffer(0)
	//}

	//fprintf(fo, "%d %d %d\n%d %d %d\n%d %d %d\n%d %d %d\n ", dataAx,dataAy,dataAz,dataBx, dataBy,dataBz, dataCx, dataCy, dataCz, dataDx, dataDy, dataDz);
	//printf("%d %d %d\n%d %d %d\n%d %d %d\n%d %d %d\n ", dataAx, dataAy, dataAz, dataBx, dataBy, dataBz, dataCx, dataCy, dataCz, dataDx, dataDy, dataDz);

	//int p[4]; int p = 0;//stroe uv data in int k array
	//int k�z��̃X�g���[�Nuv�f�[�^

	//while ((fscanf(fc, "%d", &x)) != EOF) {//uv�f�[�^�̓������t�@�C�����X�L����
	//std::cout << x << std::endl;//uv�f�[�^4���o��
	//k[c] = x; c++;
	//}

	//fclose(fc);//uv�f�[�^�̓������t�@�C�������
	//std::cout << k[0] << " " << k[1] << " " << k[2] << " " << k[3] << std::endl;

	AfxMessageBox(_T("All results have been saved!"), MB_OK | MB_ICONINFORMATION);//�u����I���v�ƕ\��������
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


	//levell�̒l��Button1�ƈقȂ�
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
			//RGB�摜���擾���Ă���
			kinect.setRGB();
			//cv::imshow("RGB", kinect.rgbImage);//RGB�摜��\������

			kinect.setDepth(false);//initialize depth image

			kinect.depthImage.convertTo(measure_img, CV_8UC1, 25.0 / 4500);//depth image grayscale (2)channels to color image (3)channels
			cv::cvtColor(measure_img, result, CV_GRAY2BGR);//
			cv::rectangle(result, cv::Rect((kinect.depthImage.cols - 1) / 2 - 10, (kinect.depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//get the center position
			kinect.depthImage.copyTo(separate_img);
			UINT16 d = kinect.depthImage.at<UINT16>((kinect.depthImage.rows - 1) / 2, (kinect.depthImage.cols - 1) / 2);// intensity of depth image
			int val = (int)d * 4500 / 65535;


			/////////Small pig�ƈقȂ镔��/////////////
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
																	//minAreaRect:�^����ꂽ 2�����_�W�����͂ށi��]���l�������j�ŏ��̋�`�����߂܂�
																	//minRect�́A�؂̏ꏊ���m�F���A�摜����֊s�x�N�g���iu,v�j�����肷�邽�߂Ɏg�p
																	//contours�F�֊s���i�������j


					cv::RotatedRect minEllipse;
					if (contours[i].size()>100)//�����ȃT�C�Y���������
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
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A���̃R���g���[����
	// ���̒ʒm�𑗐M����ɂ́ACDialogEx::OnInitDialog() �֐����I�[�o�[���C�h���A
	// CRichEditCtrl().SetEventMask() ��
	// OR ��Ԃ� ENM_CHANGE �t���O���}�X�N�ɓ���ČĂяo���K�v������܂��B

	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����Ă��������B
}


void CPigSeparationDlg::OnEnChangeEdit1()
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A���̃R���g���[����
	// ���̒ʒm�𑗐M����ɂ́ACDialogEx::OnInitDialog() �֐����I�[�o�[���C�h���A
	// CRichEditCtrl().SetEventMask() ��
	// OR ��Ԃ� ENM_CHANGE �t���O���}�X�N�ɓ���ČĂяo���K�v������܂��B

	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����Ă��������B
}
