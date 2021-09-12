
// SampleImageViewerView.h : CSampleImageViewerView Ŭ������ �������̽�
//

#pragma once


class CSampleImageViewerView : public CView
{
protected: // serialization������ ��������ϴ�.
	CSampleImageViewerView();
	DECLARE_DYNCREATE(CSampleImageViewerView)

// Ư���Դϴ�.
public:
	CSampleImageViewerDoc* GetDocument() const;
	unsigned short* m_pImage16;
	unsigned char* m_pImage8;
	BYTE* m_pLUT;
	int m_nWidth;
	int m_nHeight;
	int m_nBitPerPixel;
	int m_nHistogramSize;
	int m_nImageSize;
	int m_nZoom;
	void SetZoom(int zoom) { m_nZoom = zoom; }
	void SetImageInfo(int width, int height, int bit);
	void AllocImageBuf();
	BOOL ReadRawImage(LPCTSTR lpszFileName, void* pSrc, int nWidth, int nHeight, int nBitAllocated);
	void SetLUT(BYTE* lut, int nMinIntensity, int nMaxIntensity);
	void SetImage8(BYTE* lut, unsigned short* pImage16, unsigned char*  pImage8);	
	void FreeImageBuf();

	LPBITMAPINFO m_pBitMapInfo;
	void SetBmpInfo();
	void DrawImage(CDC* pdc, CRect viewRect, CRect displayRect);
	void GetViewRect(const CRect ARect, double AZoom, int AWidth, int AHeight, CRect *AViewRect, CRect *ADisplayRect);
	
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CSampleImageViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // SampleImageViewerView.cpp�� ����� ����
inline CSampleImageViewerDoc* CSampleImageViewerView::GetDocument() const
   { return reinterpret_cast<CSampleImageViewerDoc*>(m_pDocument); }
#endif

