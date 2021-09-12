
// ImageCAP_sampleDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ImageCAP_sample.h"
#include "ImageCAP_sampleDlg.h"
#include "CalibrationDllEx.h"

#include "wininet.h"
#pragma comment (lib, "WinInet.lib")



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL g_bFileSave;
BOOL g_bDynStat;
BOOL GetDynStat()
{
	return g_bDynStat;
}
BOOL GetFileSaveStat()
{
	return g_bFileSave;
}
BOOL WriteRawImage(LPCTSTR lpszFIleName, void * pSrc, int nWidth, int nHeight, int nBitAllocated);
BOOL WriteRawImage(LPCTSTR lpszFIleName, void * pSrc, int nWidth, int nHeight, int nBitAllocated)
{
	int nSizeOfImage = nWidth*nHeight*(nBitAllocated / 8);
	CFile file;
	if (!file.Open(lpszFIleName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone))	return FALSE;
	file.Write(pSrc, nSizeOfImage);
	file.Close();
	return TRUE;
}

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CImageCAP_sampleDlg ��ȭ ����




CImageCAP_sampleDlg::CImageCAP_sampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageCAP_sampleDlg::IDD, pParent)
	, m_strLog(_T(""))
	, m_nWindowTime(0)
	, m_nAVGFrameNum(0)
	, m_nRadioAcqMode(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_HostIP = 0;
	m_SensorIP = 0;
	m_strDataReferencePath = _T(".\\A_Data");
	m_strCalReferencePath = _T(".\\A_Cal");
}

void CImageCAP_sampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strLog);

	DDX_Control(pDX, IDC_IPADDRESS1, m_ipHost);
	DDX_Control(pDX, IDC_IPADDRESS2, m_ipSensor);
	DDX_Text(pDX, IDC_EDIT2, m_nWindowTime);
	DDX_Text(pDX, IDC_EDIT3, m_nAVGFrameNum);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadioAcqMode);
}

BEGIN_MESSAGE_MAP(CImageCAP_sampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CImageCAP_sampleDlg::OnBnClickedButton1)		
	ON_WM_DESTROY()	
	ON_BN_CLICKED(IDC_BUTTON3, &CImageCAP_sampleDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CImageCAP_sampleDlg::OnBnClickedButton4)
	ON_MESSAGE(WM_IMAGERECV, &CImageCAP_sampleDlg::OnReceiveImage)
	ON_BN_CLICKED(IDC_BUTTON5, &CImageCAP_sampleDlg::OnBnClickedButton5)			
	ON_BN_CLICKED(IDOK, &CImageCAP_sampleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImageCAP_sampleDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON8, &CImageCAP_sampleDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CImageCAP_sampleDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON7, &CImageCAP_sampleDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON10, &CImageCAP_sampleDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CImageCAP_sampleDlg::OnBnClickedButton11)	
END_MESSAGE_MAP()


// CImageCAP_sampleDlg �޽��� ó����

BOOL CImageCAP_sampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	BYTE hip[4] = { 192, 168, 3, 5 };
	BYTE sip[4] = { 192, 168, 3, 240 };
	m_ipHost.SetAddress(hip[0] << 24 | hip[1] << 16 | hip[2] << 8 | hip[3]);
	m_ipSensor.SetAddress(sip[0] << 24 | sip[1] << 16 | sip[2] << 8 | sip[3]);

	

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CImageCAP_sampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CImageCAP_sampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CImageCAP_sampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageCAP_sampleDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.			
	int			 nErrorCode;		
	
	unsigned int nHostIP;
	unsigned int nSensorIP;
	BYTE		 host[4];
	BYTE		 sensor[4];
	unsigned short nWidth, nHeight;



	m_HostIP = 0;
	m_SensorIP = 0;
	
	m_ipHost.GetAddress(host[0], host[1], host[2], host[3]);
	m_ipSensor.GetAddress(sensor[0], sensor[1], sensor[2], sensor[3]);

	nHostIP   = host[0] << 24 | host[1] << 16 | host[2] << 8 | host[3];
	nSensorIP = sensor[0] << 24 | sensor[1] << 16 | sensor[2] << 8 | sensor[3];

	nHostIP = htonl(nHostIP);
	nSensorIP = htonl(nSensorIP);


	nErrorCode = ImageCapConnectSensor(nHostIP, nSensorIP, m_strDataReferencePath);	

	if(nErrorCode == IMAGECAP_OK)
	{
		m_HostIP = nHostIP;
		m_SensorIP = nSensorIP;
	
		ImageCapGetFrameSize(m_SensorIP, &nWidth, &nHeight);
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		
		ImageCapGetCurrentMode(nSensorIP, &m_nDetectorMode);
		if (m_nDetectorMode == IMAGECAP_AED_MODE)
		{		
			m_nRadioAcqMode = 0;
		}
		else if (m_nDetectorMode == IMAGECAP_TRIGGER_MODE)
		{
			m_nRadioAcqMode = 1;
		}		
		else if (m_nDetectorMode == IMAGECAP_AVG_COUNT_MODE)
		{
			m_nRadioAcqMode = 2;
			ImageCapGetAVGModeFrameNum(nSensorIP, &m_nAVGFrameNum);
		}
		else if (m_nDetectorMode == IMAGECAP_CONTINUOUS_MODE)
		{
			m_nRadioAcqMode = 3;			
		}
		else if (m_nDetectorMode == IMAGECAP_CONTINUOUS_BINNING_MODE)
		{
			m_nRadioAcqMode = 4;
		}

		ImageCapGetWindowTime(m_SensorIP, &m_nWindowTime);
		
		UpdateData(FALSE);

	}

	DisplayErrorCode(nErrorCode);
}





void CImageCAP_sampleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.		
	ImageCapDisConnectSensor(m_SensorIP);
}



void CImageCAP_sampleDlg::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
 	if(m_SensorIP == 0) 
 	{
 		DisplayErrorCode(IMAGECAP_NO_SENSOR_ERROR);
 		return;
 	}	

	if (m_nDetectorMode == IMAGECAP_CONTINUOUS_MODE || m_nDetectorMode == IMAGECAP_CONTINUOUS_BINNING_MODE)
	{
		return;
	}
	
	unsigned short* pImage;
	pImage = new unsigned short[m_nWidth*m_nHeight];
	int nErrorCode;

	nErrorCode = ImageCapImageAcquistion(m_SensorIP, pImage, DARK_MODE, m_strCalReferencePath);	
	

	if(nErrorCode == IMAGECAP_NEW_FRAME)
	{
		CString strFileName;
		int nSizeOfImage = m_nWidth * m_nHeight* (16/8);
		CFile *file = new CFile();

		strFileName = _T(".\\dark.raw");

		file->Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone);
		file->Write(pImage, nSizeOfImage);
		file->Close();
		delete file;

		AfxMessageBox(_T("Complete"));
	}	
	else if(nErrorCode == IMAGECAP_NOTREADY_RO)
	{
		AfxMessageBox(_T("Not Ready for Image acquisition"));
	}
	else
	{
		AfxMessageBox(_T("Fail"));
	}

	DisplayErrorCode(nErrorCode);

	MEMORYRELESE(pImage);
}

void CImageCAP_sampleDlg::OnBnClickedButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if(m_SensorIP == 0) 
	{
		DisplayErrorCode(IMAGECAP_NO_SENSOR_ERROR);
		return;
	}

	


	int mode;
	if (m_nRadioAcqMode == 0)
	{
		mode = BRIGHT_MODE;
	}
	else if (m_nRadioAcqMode == 1)
	{
		mode = SOFTTRIGGER_INSTANT_MODE;
	}
	else if (m_nRadioAcqMode == 2)
	{
		mode = BRIGHT_MODE;
	}
	else if (m_nRadioAcqMode == 3)
	{
		mode = DARK_MODE;
	}
	else if (m_nRadioAcqMode == 4)
	{
		mode = DARK_MODE;
	}
	


	AfxMessageBox(_T("Be ready to shoot the X-ray"));


	unsigned short* pImage;
	pImage = new unsigned short[m_nWidth*m_nHeight];

	PTH_PARAM pParam=NULL;
	pParam = new TH_PARAM[1];
	pParam->pDlg = this;	
	pParam->pImage = pImage;
	pParam->nMode  = mode;
	pParam->nDetectorMode = m_nDetectorMode;
	pParam->bCal = TRUE;
	pParam->nDarkNum = 0;


	CWinThread* pThread;
	pThread = AfxBeginThread(ImageAcqThread,pParam,THREAD_PRIORITY_HIGHEST);

}


UINT ImageSaveThread(LPVOID lpParam)
{	
	g_bFileSave = TRUE;
	PTH_PARAM pParam = (PTH_PARAM)lpParam;
	CImageCAP_sampleDlg* pDlg = (CImageCAP_sampleDlg*)pParam->pDlg;

	int nWidth = pDlg->m_nWidth;
	int nHeight = pDlg->m_nHeight;	
	
	CString strFileName = pParam->strFileName;
	WriteRawImage(strFileName, pParam->pImage, nWidth, nHeight, 16);
	g_bFileSave = FALSE;

	return 0;
}

UINT ImageAcqThread(LPVOID lpParam)
{
	PTH_PARAM pParam = (PTH_PARAM)lpParam;	
	unsigned short* pImage = pParam->pImage;
	int nAcqMode = pParam->nMode;
	CImageCAP_sampleDlg* pDlg = (CImageCAP_sampleDlg*)pParam->pDlg;
	
	unsigned short* refImg = NULL;
	int nErrorCode = 0;

	CString strFileName;
	SYSTEMTIME cur_time;
	CString year;
	CTime time;

	year.Format(_T("%d"), time.GetYear());
	year = year.Mid(2, 2);
	
	if (pParam->nDetectorMode == IMAGECAP_CONTINUOUS_MODE || pParam->nDetectorMode == IMAGECAP_CONTINUOUS_BINNING_MODE)
	{
		g_bDynStat = TRUE;
		nErrorCode = ImageCapCommandSend(pDlg->m_SensorIP, nAcqMode);
		if (nErrorCode != IMAGECAP_OK)
		{
			ImageCapStopCapture(pDlg->m_SensorIP);
			SendMessage(pDlg->GetSafeHwnd(), WM_IMAGERECV, (WPARAM)pParam, nErrorCode);
			return 0;
		}

		while (g_bDynStat)
		{
			nErrorCode = ImageCapStartCapture(pDlg->m_SensorIP, &refImg, nAcqMode, pDlg->m_strCalReferencePath);
			memcpy(pImage, refImg, pDlg->m_nWidth*pDlg->m_nHeight * 2);

			if (nErrorCode != IMAGECAP_NEW_FRAME)
				continue;
			
			// do something				
			time = CTime::GetCurrentTime();				
			GetLocalTime(&cur_time);
				
			strFileName.Format(_T(".\\Image_%s%02d%02d_%02d%02d%02d_%03ld.raw"), year, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), cur_time.wMilliseconds);
			pParam->strFileName = strFileName;
			AfxBeginThread(ImageSaveThread, pParam, THREAD_PRIORITY_HIGHEST);				
			
		}
	}
	else
	{
		if (pParam->bCal)
			nErrorCode = ImageCapImageAcquistion(pDlg->m_SensorIP, pImage, nAcqMode, pDlg->m_strCalReferencePath);
		else
			nErrorCode = ImageCapImageAcquistion(pDlg->m_SensorIP, pImage, nAcqMode, NULL);		
	}

	SendMessage(pDlg->GetSafeHwnd(), WM_IMAGERECV, (WPARAM)pParam, nErrorCode);

	return 0;
}

UINT ImageCalThread(LPVOID lpParam)
{
	PTH_PARAM pParam = (PTH_PARAM)lpParam;
	unsigned short* pImage = pParam->pImage;
	int nAcqMode = pParam->nMode;
	CImageCAP_sampleDlg* pDlg = (CImageCAP_sampleDlg*)pParam->pDlg;

	unsigned short* refImg = NULL;
	int nErrorCode = 0;
	int nWidth = pDlg->m_nWidth;
	int nHeight = pDlg->m_nHeight;

	int nAcqCount = 0;
	int *sumImg;
	

	
	g_bDynStat = TRUE;
	nErrorCode = ImageCapCommandSend(pDlg->m_SensorIP, nAcqMode);
	if (nErrorCode != IMAGECAP_OK)
	{
		SendMessage(pDlg->GetSafeHwnd(), WM_IMAGERECV, (WPARAM)pParam, nErrorCode);
		return 0;
	}

	sumImg = new int[nWidth*nHeight];
	memset(sumImg, 0, sizeof(int)*(nWidth*nHeight));

	while (g_bDynStat)
	{
		nErrorCode = ImageCapStartCapture(pDlg->m_SensorIP, &refImg, nAcqMode, NULL);
		if (nErrorCode == IMAGECAP_NEW_FRAME)
		{			
			memcpy(pImage, refImg, pDlg->m_nWidth*pDlg->m_nHeight * 2);

			nAcqCount++;

			// except first frame
			if(nAcqCount == 1)
				continue;

			// sum image
			for (int i = 0; i < nWidth*nHeight; i++)
				sumImg[i] += pImage[i];			

			if (pParam->nDarkNum <= nAcqCount && pParam->nDarkNum != 0)
			{
				ImageCapStopCapture(pDlg->m_SensorIP);
				break;
			}
			
		}
	}
	
	// except last frame
	for (int i = 0; i < nWidth*nHeight; i++)
		sumImg[i] -= pImage[i];

	// avg image
	for (int i = 0; i < nWidth*nHeight; i++)
		pImage[i] = sumImg[i] / (nAcqCount - 2);
	
	delete[] sumImg;
	g_bDynStat = FALSE;
	SendMessage(pDlg->GetSafeHwnd(), WM_IMAGERECV, (WPARAM)pParam, nErrorCode);

	return 0;
}

LRESULT CImageCAP_sampleDlg::OnReceiveImage(WPARAM wParam, LPARAM lParam)
{
	CString strFileName;
	int nSizeOfImage = m_nWidth * m_nHeight* (16/8);

	PTH_PARAM pParam = (PTH_PARAM)wParam;
	unsigned short* pImage = pParam->pImage;

	int i = 0;

	if (pParam->nDetectorMode == IMAGECAP_CONTINUOUS_MODE || pParam->nDetectorMode == IMAGECAP_CONTINUOUS_BINNING_MODE)
	{
		for (i = 0; i < 5; i++)
		{
			if (GetDynStat())	Sleep(1000);
			else break;
		}
		for (i = 0; i < 5; i++)
		{
			if(GetFileSaveStat()) Sleep(1000);
			else break;
		}				
		
		if (!pParam->bCal)
		{
			if (pParam->nDetectorMode == IMAGECAP_CONTINUOUS_MODE)
			{
				// get dark
				if (pParam->nDarkNum > 0)
				{
					strFileName = _T(".\\A_Cal\\con_Trigger_Dark.raw");
					m_strDarkFile = strFileName;
				}
				else
				{
					strFileName = _T(".\\A_Cal\\con_Trigger_Bright.raw");
					m_strBrightFile = strFileName;
				}
			}
			else if (pParam->nDetectorMode == IMAGECAP_CONTINUOUS_BINNING_MODE)
			{
				if (pParam->nDarkNum > 0)
				{
					strFileName = _T(".\\A_Cal\\bin_Trigger_Dark.raw");
					m_strDarkFile = strFileName;
				}
				else
				{
					strFileName = _T(".\\A_Cal\\bin_Trigger_Bright.raw");
					m_strBrightFile = strFileName;
				}
			}
						

			CFile *file = new CFile();
			file->Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone);
			file->Write(pImage, nSizeOfImage);
			file->Close();
			delete file;

			AfxMessageBox(_T("Complete"));
		}		
	}
	else
	{
		if (lParam == IMAGECAP_NEW_FRAME)
		{
			CFile *file = new CFile();
			if (pParam->bCal)
			{
				strFileName = _T(".\\Image.raw");
			}
			else
			{
				strFileName = _T(".\\A_Cal\\bright.raw");
				m_strBrightFile = strFileName;
			}
			file->Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone);
			file->Write(pImage, nSizeOfImage);
			file->Close();
			delete file;

			AfxMessageBox(_T("Complete"));
		}
		else if (lParam == IMAGECAP_CANCEL)
		{
			AfxMessageBox(_T("Cancel"));
		}
		else if (lParam == IMAGECAP_NOTREADY_RO)
		{
			AfxMessageBox(_T("Not Ready for Image acquisition"));
		}
		else if (lParam == IMAGECAP_RECEIVING)
		{
			AfxMessageBox(_T("Time out error"));
		}
		else
		{
			AfxMessageBox(_T("Fail"));
		}
	}

	

	DisplayErrorCode(lParam);
	
	

	delete[] pImage;
	delete[] pParam;

	return 0;
}
void CImageCAP_sampleDlg::OnBnClickedButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_SensorIP == 0) 
	{
		DisplayErrorCode(IMAGECAP_NO_SENSOR_ERROR);
		return;
	}

	int nErrorCode=0;	

	if (m_nDetectorMode == IMAGECAP_CONTINUOUS_MODE || m_nDetectorMode == IMAGECAP_CONTINUOUS_BINNING_MODE)
	{
		g_bDynStat = FALSE;
		nErrorCode = ImageCapStopCapture(m_SensorIP);
	}
	else
	{
		nErrorCode = ImageCapAcquisitionCancel(m_SensorIP);
	}
	
	DisplayErrorCode(nErrorCode);
}

void CImageCAP_sampleDlg::DisplayErrorCode(int nErrorCode)
{
	switch(nErrorCode)
	{
	case IMAGECAP_OK:					  m_strLog = _T("OK"); break;
	case IMAGECAP_SOCKET_LOAD_ERROR:      m_strLog = _T("SOCKET_LOAD_ERROR"); break;
	case IMAGECAP_INVALID_SOCKET_ERROR:   m_strLog = _T("INVALID_SOCKET_ERROR"); break;
	case IMAGECAP_BIND_SOCKET_ERROR:      m_strLog = _T("BIND_SOCKET_ERROR"); break;
	case IMAGECAP_SOCKET_OPTION_ERROR:    m_strLog = _T("SOCKET_OPTION_ERROR"); break;
	case IMAGECAP_COMMAND_SEND_ERROR:     m_strLog = _T("COMMAND_SEND_ERROR"); break;
	case IMAGECAP_COMMAND_RECV_ERROR:     m_strLog = _T("COMMAND_RECV_ERROR"); break;
	case IMAGECAP_RECV_TIMEOUT_ERROR:     m_strLog = _T("RECV_TIMEOUT_ERROR"); break;
	case IMAGECAP_ACK_SEND_ERROR:         m_strLog = _T("ACK_SEND_ERROR"); break;
	case IMAGECAP_NO_SENSOR_ERROR:        m_strLog = _T("NO_SENSOR_ERROR"); break;
	case IMAGECAP_REGISTER_TYPE_ERROR:    m_strLog = _T("REGISTER_TYPE_ERROR"); break;
	case IMAGECAP_GET_HOST_NAME_ERROR:    m_strLog = _T("GET_HOST_NAME_ERROR"); break;
	case IMAGECAP_NIC_CHECK:              m_strLog = _T("NIC_CHECK"); break;
	case IMAGECAP_STOP_WAIT:              m_strLog = _T("STOP_WAIT"); break;
	case IMAGECAP_NEW_FRAME:              m_strLog = _T("NEW_FRAME"); break;
	case IMAGECAP_RECEIVING:              m_strLog = _T("RECEIVING"); break; 
	case IMAGECAP_CANCEL:				  m_strLog = _T("CANCEL"); break; 
	case IMAGECAP_NOTREADY_RO:			  m_strLog = _T("NOTREADY_RO"); break; 
	case IMAGECAP_FWVERSION:			  m_strLog = _T("FW Version error"); break;
	default:							  m_strLog.Format(_T("WinError : %d"), nErrorCode); break;
	}
	UpdateData(FALSE);
}


void CImageCAP_sampleDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog::OnOK();
}


void CImageCAP_sampleDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog::OnCancel();
}



void CImageCAP_sampleDlg::OnBnClickedButton8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int nErrorCode;

	if (m_SensorIP == 0)
	{
		DisplayErrorCode(IMAGECAP_NO_SENSOR_ERROR);
		return;
	}

	
	if (m_nRadioAcqMode == 0)
	{
		m_nDetectorMode = IMAGECAP_AED_MODE;
	}
	else if (m_nRadioAcqMode == 1)
	{
		m_nDetectorMode = IMAGECAP_TRIGGER_MODE;
	}
	else if (m_nRadioAcqMode == 2)
	{
		m_nDetectorMode = IMAGECAP_AVG_COUNT_MODE;
	}
	else if (m_nRadioAcqMode == 3)
	{
		m_nDetectorMode = IMAGECAP_CONTINUOUS_MODE;
	}
	else if (m_nRadioAcqMode == 4)
	{
		m_nDetectorMode = IMAGECAP_CONTINUOUS_BINNING_MODE;
	}

	nErrorCode = ImageCapModeChange(m_SensorIP, m_nDetectorMode);
	
	if(m_nRadioAcqMode == 2)
		ImageCapSetAVGModeFrameNum(m_SensorIP, m_nAVGFrameNum);

	ImageCapGetWindowTime(m_SensorIP, &m_nWindowTime);
	ImageCapGetFrameSize(m_SensorIP, &m_nWidth, &m_nHeight);

	DisplayErrorCode(nErrorCode);

	UpdateData(FALSE);
}


void CImageCAP_sampleDlg::OnBnClickedButton9()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	if (m_SensorIP == 0)
	{
		DisplayErrorCode(IMAGECAP_NO_SENSOR_ERROR);
		return;
	}
	
	int nErrorCode;

	
	nErrorCode = ImageCapSetWindowTime(m_SensorIP, m_nWindowTime);
	DisplayErrorCode(nErrorCode);
	UpdateData(FALSE);
}


void CImageCAP_sampleDlg::OnBnClickedButton7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	int nSizeOfImage = m_nWidth * m_nHeight* (16 / 8);

	unsigned short* pDark = new unsigned short[m_nWidth*m_nHeight];
	unsigned short* pBright = new unsigned short[m_nWidth*m_nHeight];
	

	CFile *file = new CFile();
		
	if (!file->Open(m_strDarkFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone))
	{
		AfxMessageBox(_T("file open fail"));
		return;
	}
	file->Read(pDark, nSizeOfImage);	
	file->Close();

	
	if (!file->Open(m_strBrightFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone))
	{
		AfxMessageBox(_T("file open fail"));
		return;
	}
	file->Read(pBright, nSizeOfImage);
	file->Close();
	
	GenerateReferenceFile(pDark, &pBright, 1, m_nDetectorMode, _T(".\\A_Cal"), m_nWidth, m_nHeight);
	
	delete[] pDark;
	delete[] pBright;
	delete file;

}


void CImageCAP_sampleDlg::OnBnClickedButton10()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_SensorIP == 0)
	{
		DisplayErrorCode(IMAGECAP_NO_SENSOR_ERROR);
		return;
	}

	unsigned short* pImage;
	pImage = new unsigned short[m_nWidth*m_nHeight];
	int nErrorCode=0;

	if (m_nDetectorMode == IMAGECAP_CONTINUOUS_MODE || m_nDetectorMode == IMAGECAP_CONTINUOUS_BINNING_MODE)
	{
		PTH_PARAM pParam = NULL;
		pParam = new TH_PARAM[1];
		pParam->pDlg = this;
		pParam->pImage = pImage;
		pParam->nMode = DARK_MODE;
		pParam->nDetectorMode = m_nDetectorMode;
		pParam->bCal = FALSE;
		pParam->nDarkNum = 10;

		CWinThread* pThread;

		pThread = AfxBeginThread(ImageCalThread, pParam, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		nErrorCode = ImageCapImageAcquistion(m_SensorIP, pImage, DARK_MODE, m_strCalReferencePath);


		if (nErrorCode == IMAGECAP_NEW_FRAME)
		{
			int nSizeOfImage = m_nWidth * m_nHeight* (16 / 8);
			CFile *file = new CFile();

			if (m_nDetectorMode == IMAGECAP_AED_MODE)
				m_strDarkFile = _T(".\\A_Cal\\AED_Dark.raw");
			else if (m_nDetectorMode == IMAGECAP_TRIGGER_MODE)
				m_strDarkFile = _T(".\\A_Cal\\Trigger_Dark.raw");
			else if (m_nDetectorMode == IMAGECAP_AVG_AED_MODE || m_nDetectorMode == IMAGECAP_AVG_COUNT_MODE)
				m_strDarkFile = _T(".\\A_Cal\\avg_Trigger_Dark.raw");

			file->Open(m_strDarkFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone);
			file->Write(pImage, nSizeOfImage);
			file->Close();
			delete file;

			AfxMessageBox(_T("Complete"));
		}
		else if (nErrorCode == IMAGECAP_NOTREADY_RO)
		{
			AfxMessageBox(_T("Not Ready for Image acquisition"));
		}
		else
		{
			AfxMessageBox(_T("Fail"));
		}

		DisplayErrorCode(nErrorCode);

		MEMORYRELESE(pImage);
	}

	
}


void CImageCAP_sampleDlg::OnBnClickedButton11()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_SensorIP == 0)
	{
		DisplayErrorCode(IMAGECAP_NO_SENSOR_ERROR);
		return;
	}

	int mode;
	if (m_nRadioAcqMode == 0)
	{
		mode = BRIGHT_MODE;
	}
	else if (m_nRadioAcqMode == 1)
	{
		mode = SOFTTRIGGER_INSTANT_MODE;
	}
	else if (m_nRadioAcqMode == 2)
	{
		mode = BRIGHT_MODE;
	}
	else if (m_nRadioAcqMode == 3)
	{
		mode = DARK_MODE;
	}
	else if (m_nRadioAcqMode == 4)
	{
		mode = DARK_MODE;
	}


	AfxMessageBox(_T("Be ready to shoot the X-ray"));


	unsigned short* pImage;
	pImage = new unsigned short[m_nWidth*m_nHeight];

	PTH_PARAM pParam = NULL;
	pParam = new TH_PARAM[1];
	pParam->pDlg = this;
	pParam->pImage = pImage;
	pParam->nMode = mode;
	pParam->nDetectorMode = m_nDetectorMode;
	pParam->bCal = FALSE;
	pParam->nDarkNum = 0;


	CWinThread* pThread;

	if(m_nDetectorMode == IMAGECAP_CONTINUOUS_MODE || m_nDetectorMode == IMAGECAP_CONTINUOUS_BINNING_MODE)
		pThread = AfxBeginThread(ImageCalThread, pParam, THREAD_PRIORITY_HIGHEST);
	else
		pThread = AfxBeginThread(ImageAcqThread, pParam, THREAD_PRIORITY_HIGHEST);
}


