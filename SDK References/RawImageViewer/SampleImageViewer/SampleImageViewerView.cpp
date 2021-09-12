
// SampleImageViewerView.cpp : CSampleImageViewerView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "SampleImageViewer.h"
#endif

#include "SampleImageViewerDoc.h"
#include "SampleImageViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleImageViewerView

IMPLEMENT_DYNCREATE(CSampleImageViewerView, CView)

BEGIN_MESSAGE_MAP(CSampleImageViewerView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSampleImageViewerView ����/�Ҹ�

CSampleImageViewerView::CSampleImageViewerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_pImage16 = NULL;
	m_pImage8 = NULL;
}

CSampleImageViewerView::~CSampleImageViewerView()
{
	FreeImageBuf();
}

BOOL CSampleImageViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CSampleImageViewerView �׸���

void CSampleImageViewerView::SetImage8(BYTE * lut, unsigned short * pImage16, unsigned char * pImage8)
{	
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			pImage8[i*m_nWidth + j] = lut[pImage16[i*m_nWidth + j]];
		}
	}
}

void CSampleImageViewerView::FreeImageBuf()
{
	delete[] m_pImage16;
	delete[] m_pImage8;
	delete[] m_pLUT;
	delete m_pBitMapInfo;
}

void CSampleImageViewerView::SetBmpInfo()
{
	m_pBitMapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitMapInfo->bmiHeader.biWidth = m_nWidth;
	m_pBitMapInfo->bmiHeader.biHeight = -m_nHeight;
	m_pBitMapInfo->bmiHeader.biPlanes = 1;
	m_pBitMapInfo->bmiHeader.biBitCount = 8;
	m_pBitMapInfo->bmiHeader.biClrUsed = 256;
	m_pBitMapInfo->bmiHeader.biSizeImage = (m_nWidth)*m_nHeight;
	m_pBitMapInfo->bmiHeader.biXPelsPerMeter = 0x00;
	m_pBitMapInfo->bmiHeader.biYPelsPerMeter = 0x00;
	m_pBitMapInfo->bmiHeader.biCompression = BI_RGB;

	for (int i = 0; i < 256; i++)
	{
		m_pBitMapInfo->bmiColors[i].rgbRed = i;
		m_pBitMapInfo->bmiColors[i].rgbGreen = i;
		m_pBitMapInfo->bmiColors[i].rgbBlue = i;
		m_pBitMapInfo->bmiColors[i].rgbReserved = 0;
	}
}

void CSampleImageViewerView::DrawImage(CDC * pdc, CRect viewRect, CRect displayRect)
{
	SetStretchBltMode(pdc->m_hDC, COLORONCOLOR);

	int ret = StretchDIBits(
		pdc->m_hDC,
		displayRect.left, displayRect.top, displayRect.Width(), displayRect.Height(),// ��´��
		viewRect.left, viewRect.top, viewRect.Width(), viewRect.Height(),			 // ����
		m_pImage8,
		m_pBitMapInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

void CSampleImageViewerView::GetViewRect(const CRect ARect, double AZoom, int AWidth, int AHeight, CRect * AViewRect, CRect * ADisplayRect)
{
	// define initial location
	double qInitPosWidth = (AViewRect->left + AViewRect->right)*0.5 / AWidth,
		qInitPosHeight = (AViewRect->top + AViewRect->bottom)*0.5 / AHeight;

	// proceed
	CRect qRect = ARect;
	qRect.OffsetRect(qRect.left, qRect.top);
	double qScaleWidth = (double)AWidth / ARect.Width(),
		qScaleHeight = (double)AHeight / ARect.Height();

	int qVisWidth,
		qVisHeight,
		qDisplayWidth,
		qDisplayHeight;

	if (qScaleWidth > qScaleHeight) {
		qVisWidth = (int)(AWidth / AZoom);
		qVisHeight = (int)(ARect.Height()*qScaleWidth / AZoom + 0.499999);
		if (qVisHeight > AHeight)
			qVisHeight = AHeight;
		qDisplayWidth = ARect.Width();

		qDisplayHeight = (int)(qVisHeight / qScaleWidth*AZoom + 0.499999);
		if (qDisplayHeight > ARect.Height())
			qDisplayHeight = ARect.Height();
	}
	else {
		qVisHeight = (int)(AHeight / AZoom);										
		qVisWidth = (int)(ARect.Width()*qScaleHeight / AZoom + 0.499999);	

		if (qVisWidth > AWidth)
			qVisWidth = AWidth;

		qDisplayHeight = ARect.Height();
		qDisplayWidth = (int)(qVisWidth / qScaleHeight*AZoom + 0.499999);

		if (qDisplayWidth > ARect.Width())
			qDisplayWidth = ARect.Width();
	};

	// redefine location
	int qNewLeft, qNewRight;
	if (AWidth <= qVisWidth)
	{
		qNewLeft = 0;
		qNewRight = AWidth;
	}
	else
	{
		int qNewHrzCenter = (int)(AWidth*qInitPosWidth + 0.499999);
		qNewLeft = qNewHrzCenter - qVisWidth / 2;

		if (qNewLeft < 0)
			qNewLeft = 0;

		qNewRight = qNewLeft + qVisWidth;
		if (qNewRight > AWidth)
		{
			qNewRight = AWidth;
			qNewLeft = qNewRight - qVisWidth;

			if (qNewLeft < 0)
				qNewLeft *= (-1);
		};
	};


	int    qNewTop, qNewBottom;
	if (AHeight <= qVisHeight) {
		qNewTop = 0;
		qNewBottom = AHeight;
	}
	else {
		int qNewVrtCenter = (int)(AHeight*qInitPosHeight + 0.499999);
		qNewTop = qNewVrtCenter - qVisHeight / 2;
		if (qNewTop < 0) qNewTop = 0;
		qNewBottom = qNewTop + qVisHeight;
		if (qNewBottom > AHeight)
		{
			qNewBottom = AHeight;
			qNewTop = qVisHeight - qNewBottom;

			if (qNewTop < 0)
				qNewTop *= (-1);
		};
	};

	// redefine view-rectangle of the image
	AViewRect->SetRect(qNewLeft, qNewTop, qNewRight, qNewBottom);

	// redefine diaplay's rectangle
 	if (AZoom == 1)
 	{
		ADisplayRect->SetRect(ARect.left, 0,
			ARect.left + qDisplayWidth, qDisplayHeight);
	}
	else//scroll bar area
	{
		ADisplayRect->SetRect(ARect.left, 0,
			ARect.left + qDisplayWidth - 18, qDisplayHeight - 18);
	}
}

void CSampleImageViewerView::SetLUT(BYTE * lut, int nMinIntensity, int nMaxIntensity)
{	
	ZeroMemory(lut, m_nHistogramSize);

	// �ε�ȣ�� ����.  
	for (int i = 0; i <= nMinIntensity; i++)
	{
		lut[i] = 0;
	}

	for (int i = m_nHistogramSize - 1; i >= nMaxIntensity; i--) // �ְ����� 255�� �Ǿ� �ϹǷ� i>=max.. �ε�ȣ ����.
	{
		lut[i] = 255;
	}

	float mult = 255.0f / (float)(nMaxIntensity - nMinIntensity);
	for (int i = nMinIntensity + 1; i < nMaxIntensity; i++)				// 0�� 255���� min�� max�� ���� �ǹǷ�. i=min+1.  i<max.   �ε�ȣ����.
	{
		lut[i] = (BYTE)((i - nMinIntensity) * mult);
	}
}

void CSampleImageViewerView::SetImageInfo(int width, int height, int bit)
{
	m_nWidth = width;
	m_nHeight = height;
	m_nImageSize = width*height*bit / 8;
	m_nBitPerPixel = bit;
	m_nHistogramSize = 1 << bit;

	AllocImageBuf();
	SetBmpInfo();
}

void CSampleImageViewerView::AllocImageBuf()
{
	m_pImage16 = new unsigned short[m_nWidth*m_nHeight];
	m_pImage8 = new unsigned char[m_nImageSize];
	m_pLUT = new BYTE[m_nHistogramSize];
	m_pBitMapInfo = (LPBITMAPINFO)new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
}

BOOL CSampleImageViewerView::ReadRawImage(LPCTSTR lpszFileName, void * pSrc, int nWidth, int nHeight, int nBitAllocated)
{
	CFile file;
	UINT uLength = 0;
	UINT uResult = 0;

	int nSizeOfImage = nWidth*nHeight;
	if (!pSrc)  return FALSE;

	if (file.Open(lpszFileName, CFile::modeRead | CFile::shareDenyNone) == NULL)	return FALSE;
	uLength = UINT(file.GetLength());

	if (uLength != nSizeOfImage*(nBitAllocated / 8)) { file.Close();	return FALSE; }
	uResult = file.Read(pSrc, uLength);

	file.Close();

	if (uResult == 0)  
		return FALSE;	

	return TRUE;
}

void CSampleImageViewerView::OnDraw(CDC* /*pDC*/)
{
	CSampleImageViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rect);
	pDC->SelectClipRgn(&rgn);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bitmap, *pOldBitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);
	pOldBitmap = memDC.SelectObject(&bitmap);

	CRect viewRect(0, 0, m_nWidth-1, m_nHeight-1);
	CRect displayRect;
	GetViewRect(rect, m_nZoom, m_nWidth, m_nHeight, &viewRect, &displayRect);
	DrawImage(&memDC, viewRect, displayRect);
	pDC->BitBlt(0, 0, displayRect.Width(), displayRect.Height(), &memDC, 0, 0, SRCCOPY);


	memDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}


// CSampleImageViewerView �μ�

BOOL CSampleImageViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CSampleImageViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CSampleImageViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CSampleImageViewerView ����

#ifdef _DEBUG
void CSampleImageViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CSampleImageViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSampleImageViewerDoc* CSampleImageViewerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSampleImageViewerDoc)));
	return (CSampleImageViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CSampleImageViewerView �޽��� ó����


void CSampleImageViewerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	SetImageInfo(2500, 3052, 16);	
	if (!ReadRawImage(_T(".\\test.raw"), m_pImage16, m_nWidth, m_nHeight, m_nBitPerPixel))
		return;
	SetLUT(m_pLUT, 258, 65280);
	SetImage8(m_pLUT, m_pImage16, m_pImage8);
	SetZoom(1);
}
