// CDlgImage.cpp : implementation file
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CDlgImage.h"


// CDlgImage dialog

IMPLEMENT_DYNAMIC(CDlgImage, CDialogEx)

CDlgImage::CDlgImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlgImage, pParent)
	, m_IsCenterCalculated(false)
	,m_xCenter(0)
	,m_yCenter(0)
	, m_nRadius(0)
{

}

CDlgImage::~CDlgImage()
{
}

void CDlgImage::InitImage()
{
	// width of image
	int nWidth = 640;

	// height of image
	int nHeight = 480;

	// color scale
	int nBpp = 8;


	m_image.Create(nWidth, -nHeight, nBpp);

	if (nBpp == 8)
	{
		// color values from 0 .. 255
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}
	// address of image memory's first cell (0,0)
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	// set value of cells in array of image
	memset(fm, 0x00, nWidth * nHeight);

}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImage, CDialogEx)

	ON_WM_PAINT()
END_MESSAGE_MAP()



BOOL CDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitImage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	if (m_image) {
		m_image.Draw(dc, 0, 0);

		if (m_IsCenterCalculated) {
			drawOutlineAtCenterManual(&dc);
			drawPlusAtCenter(&dc);			
		}
	}
	
}


void CDlgImage::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
	m_IsCenterCalculated = false;

	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;

	int nPitch = m_image.GetPitch();

	for (int j = y; j < y + nRadius * 2; j++)
		for (int i = x; i < x + nRadius * 2; i++)
			if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
				fm[j * nPitch + i] = rand() % 0xff;
}

void CDlgImage::calcCenterCircle()
{

	// width of image
	int nWidth = m_image.GetWidth();

	// height of image
	int nHeight = m_image.GetHeight();

	// address of image memory's first cell (0,0)
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	int nPitch = m_image.GetPitch();

	int nTh = 0x80;
	int nSumX = 0;
	int nSumY = 0;
	int nCount = 0;

	CRect rect(0, 0, nWidth, nHeight);

	for (int j = rect.top; j < rect.bottom; j++)
		for (int i = rect.left; i < rect.right; i++)
			if (fm[j * nPitch + i] > nTh)
			{
				nSumX += i;
				nSumY += j;
				nCount++;
			}

	m_xCenter = nSumX / nCount;
	m_yCenter = nSumY / nCount;

	m_IsCenterCalculated = true;
}

void CDlgImage::drawOutlineAtCenter(CDC* pDC)
{
	CPen nPen;
	nPen.CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
	CPen* pOldPen = pDC->SelectObject(&nPen);
	
	CRect rect(m_xCenter - m_nRadius, m_yCenter - m_nRadius, m_xCenter + m_nRadius, m_yCenter + m_nRadius);

	pDC->Ellipse(rect);

	pDC->SelectObject(pOldPen);

}

void CDlgImage::drawOutlineAtCenterManual(CDC* pDC)
{
	int nSizeOfCenterShape = 13;

	CPen nPen;
	nPen.CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
	CPen* pOldPen = pDC->SelectObject(&nPen);



	CPoint pts[5];
	pts[0] = CPoint(m_xCenter - nSizeOfCenterShape, m_yCenter - nSizeOfCenterShape);
	pts[1] = CPoint(m_xCenter + nSizeOfCenterShape, m_yCenter - nSizeOfCenterShape);
	
	pts[2] = CPoint(m_xCenter + nSizeOfCenterShape, m_yCenter + nSizeOfCenterShape);
	pts[3] = CPoint(m_xCenter - nSizeOfCenterShape, m_yCenter + nSizeOfCenterShape);
	pts[4] = CPoint(m_xCenter - nSizeOfCenterShape, m_yCenter - nSizeOfCenterShape);
	pDC->Polyline(pts, 5);

	pDC->SelectObject(pOldPen);

}

void CDlgImage::drawPlusAtCenter(CDC* pDC)
{
	int nSizeOfCenterShape = 10;
	CPen nPen;
	nPen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&nPen);
	CPoint pts[2];
	pts[0] = CPoint(m_xCenter - nSizeOfCenterShape, m_yCenter);
	pts[1] = CPoint(m_xCenter + nSizeOfCenterShape, m_yCenter);
	pDC->Polyline(pts, 2);

	pts[0] = CPoint(m_xCenter, m_yCenter - nSizeOfCenterShape);
	pts[1] = CPoint(m_xCenter, m_yCenter + nSizeOfCenterShape);
	pDC->Polyline(pts, 2);

	pDC->SelectObject(pOldPen);

}

void CDlgImage::drawCircleRandomPosition(int nRadius, int nGray)
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	m_nRadius = nRadius;
	int x, y;
	getRandompoint(x, y, nRadius);

	drawCircle(fm, x, y, nRadius, nGray);
}

void CDlgImage::getRandompoint(int& x, int& y, int nRadius)
{
	// width of image
	int nWidth = m_image.GetWidth();

	// height of image
	int nHeight =m_image.GetHeight();

	int xMin, xMax, yMin, yMax;

	xMin = yMin = 0;
	xMax = nWidth - 2 * nRadius;
	yMax = nHeight - 2 * nRadius;

	x = rand() % (xMax - xMin) + xMin;
	y = rand() % (yMax - yMin) + yMin;
}

BOOL CDlgImage::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	BOOL bRet = false;

	double dx = i - nCenterX;

	double dy = j - nCenterY;

	double nDistance = dx * dx + dy * dy;

	if (nDistance < nRadius * nRadius)
		bRet = true;

	return bRet;
}
