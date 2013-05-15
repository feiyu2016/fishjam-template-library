//以下是CPP文件
// TextProgressCtrl.cpp : implementation file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright 1998.
//
// Modified : 26/05/98 Jeremy Davis, jmd@jvf.co.uk
//                Added colour routines
//
// TextProgressCtrl is a drop-in replacement for the standard 
// CProgressCtrl that displays text in a progress control.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is not sold for
// profit without the authors written consent, and providing that this
// notice and the authors name is included. If the source code in 
// this file is used in any commercial application then an email to
// the me would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 

#include "stdafx.h"
#include "TextProgressCtrl.h"
#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl

CTextProgressCtrl::CTextProgressCtrl()
{
    m_nPos            = 0;
    m_nStepSize        = 1;
    m_nMax            = 100;
    m_nMin            = 0;
    m_bShowText        = TRUE;
    m_strText.Empty();
    m_colFore        = ::GetSysColor(COLOR_HIGHLIGHT);
    m_colBk            = ::GetSysColor(COLOR_WINDOW);
    m_colTextFore    = ::GetSysColor(COLOR_HIGHLIGHT);
    m_colTextBk        = ::GetSysColor(COLOR_WINDOW);

    m_nBarWidth = -1;
}

CTextProgressCtrl::~CTextProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CTextProgressCtrl, CProgressCtrl)
    //{{AFX_MSG_MAP(CTextProgressCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
    //ON_MESSAGE(WM_SETTEXT, OnSetText)
    //ON_MESSAGE(WM_GETTEXT, OnGetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers

LRESULT CTextProgressCtrl::OnSetText(UINT, LPCTSTR szText)
{
    LRESULT result = Default();

    if ( (!szText && m_strText.GetLength()) ||
         (szText && (m_strText != szText))   )
    {
        m_strText = szText;
        Invalidate();
    }

    return result;
}

LRESULT CTextProgressCtrl::OnGetText(UINT cchTextMax, LPTSTR szText)
{
    if(SUCCEEDED(StringCchCopy(szText, cchTextMax, m_strText)))
    //if (!_tcsncpy(szText, m_strText, cchTextMax))
        return 0;
    else 
        return min(cchTextMax, (UINT) m_strText.GetLength());
}

BOOL CTextProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{    
     return TRUE;
}

void CTextProgressCtrl::OnSize(UINT nType, int cx, int cy) 
{
    CProgressCtrl::OnSize(nType, cx, cy);
    
    m_nBarWidth    = -1;   // Force update if SetPos called
}

void CTextProgressCtrl::OnPaint() 
{
    if (m_nMin >= m_nMax) 
        return;

    CRect LeftRect, RightRect, ClientRect;
    GetClientRect(ClientRect);

    double Fraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));

    CPaintDC PaintDC(this); // device context for painting
    CMemDC dc(&PaintDC);
    //CPaintDC dc(this);    // device context for painting (if not double buffering)

    LeftRect = RightRect = ClientRect;

    LeftRect.right = LeftRect.left + (int)((LeftRect.right - LeftRect.left)*Fraction);
    dc.FillSolidRect(LeftRect, m_colFore);

    RightRect.left = LeftRect.right;
    dc.FillSolidRect(RightRect, m_colBk);

    if (m_bShowText && Fraction > 0)
    {
        CString str;
        if (m_strText.GetLength())
            str = m_strText;
        else
            str.Format(TEXT("%d%%"), (int)(Fraction*100.0));

        dc.SetBkMode(TRANSPARENT);

        CRgn rgn;
        rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_colTextBk);

        dc.DrawText(str, ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        rgn.DeleteObject();
        rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_colTextFore);

        dc.DrawText(str, ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void CTextProgressCtrl::SetForeColour(COLORREF col)
{
    m_colFore = col;
}

void CTextProgressCtrl::SetBkColour(COLORREF col)
{
    m_colBk = col;
}

void CTextProgressCtrl::SetTextForeColour(COLORREF col)
{
    m_colTextFore = col;
}

void CTextProgressCtrl::SetTextBkColour(COLORREF col)
{
    m_colTextBk = col;
}

COLORREF CTextProgressCtrl::GetForeColour()
{
    return m_colFore;
}

COLORREF CTextProgressCtrl::GetBkColour()
{
    return m_colBk;
}

COLORREF CTextProgressCtrl::GetTextForeColour()
{
    return m_colTextFore;
}

COLORREF CTextProgressCtrl::GetTextBkColour()
{
    return m_colTextBk;
}
/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers

void CTextProgressCtrl::SetShowText(BOOL bShow)
{ 
    if (::IsWindow(m_hWnd) && m_bShowText != bShow)
        Invalidate();

    m_bShowText = bShow;
}


void CTextProgressCtrl::SetRange(int nLower, int nUpper)
{
    m_nMax = nUpper;
    m_nMin = nLower;
}

int CTextProgressCtrl::SetPos(int nPos) 
{    
    if (!::IsWindow(m_hWnd))
        return -1;

    int nOldPos = m_nPos;
    m_nPos = nPos;

    CRect rect;
    GetClientRect(rect);

    double Fraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));
    int nBarWidth = (int) (Fraction * rect.Width());

    if (nBarWidth != m_nBarWidth)
    {
        m_nBarWidth = nBarWidth;
        RedrawWindow();
    }

    return nOldPos;
}

int CTextProgressCtrl::StepIt() 
{    
   return SetPos(m_nPos + m_nStepSize);
}

int CTextProgressCtrl::OffsetPos(int nPos)
{
    return SetPos(m_nPos + nPos);
}

int CTextProgressCtrl::SetStep(int nStep)
{
    int nOldStep = nStep;
    m_nStepSize = nStep;
    return nOldStep;
} 
