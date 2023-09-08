#pragma once
#include "afxdialogex.h"


// CDlgImage dialog

class CDlgImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImage)

public:
	CDlgImage(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgImage();

	CImage m_image;

private:
	void InitImage();
	bool m_IsCenterCalculated;
	int m_xCenter, m_yCenter;
	int m_nRadius;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgImage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	// draw circle at given position
	void drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
	// draw circle at random position
	void drawCircleRandomPosition(int nRadius, int nGray);
	// generate random position
	void getRandompoint(int& x, int& y, int nRadius);
	// check if the point is in the circle that is being drawn
	BOOL isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	// calc center position of the drawn circle
	void calcCenterCircle();
	// draw + at the center of circle
	void drawPlusAtCenter(CDC* pDC);
	// draw outline at the center of circle
	void drawOutlineAtCenter(CDC* pDC);
	// draw outline at the center of circle
	void drawOutlineAtCenterManual(CDC* pDC);

};
