
// DisplayGrid-MFCDlg.h : header file
//

#pragma once


// CDisplayGridMFCDlg dialog
class CDisplayGridMFCDlg : public CDialogEx
{
// Construction
public:
	CDisplayGridMFCDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPLAYGRIDMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	CListCtrl m_listCtrl2;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	CProgressCtrl m_PrgCtrl;
	int main0();
	int m_N;
	int m_p; 
	int m_b0;
	int m_Nreps;
	double m_d;
	int m_ntenure;
	int m_niternotimproved;
	int m_niterover;

};
