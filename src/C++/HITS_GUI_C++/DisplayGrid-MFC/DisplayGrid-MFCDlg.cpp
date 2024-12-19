
// DisplayGrid-MFCDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DisplayGrid-MFC.h"
#include "DisplayGrid-MFCDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#include "pch.h" 

#include "stdio.h" 

#include <ctime> 

#include <stdlib.h>

#include "math.h"

//#define Tmax  5000 
#define Tmax  50000 

#define pmax  20 

#define Itmax  1000 

#define EPSILON 1.3e-14
//#define EPSILON 1e-7



const int ntenure = 50;

const int niterover = 200;

const int niternotimproved = 50;



void bsort(int np, int nat[pmax], int snat[pmax]) {

	//locals 

	int itmp;

	//int bnat[pmax] 



	//save nat in snat and sort snat in increasing order 

	for (int i = 0; i < np; i++) {

		snat[i] = nat[i];

	}



	for (int i = 0; i < (np - 1); i++) {

		for (int j = i + 1; j < np; j++) {

			if (snat[i] > snat[j]) {

				itmp = snat[i];

				snat[i] = snat[j];

				snat[j] = itmp;

			}

		}

	}

}





void gelim(int np, double sk[pmax][pmax], double r1[pmax], double u[pmax]) {

	//locals 

	double bsk[pmax][pmax], br1[pmax];



	//np = p 

	//sk = A 

	//r1 = b 

	//x = u 



	//save sk in bsk and r1 in br1 because they will be overwritten 

	for (int i = 0; i < np; i++) {

		br1[i] = r1[i];

		for (int j = 0; j < np; j++) {

			bsk[i][j] = sk[i][j];

		}

	}



	for (int i = 0; i < (np - 1); i++) {

		for (int j = i + 1; j < np; j++) {

			double p = sk[j][i] / sk[i][i];

			r1[j] = r1[j] - p * r1[i];

			for (int k = 0; k < np; k++) {

				sk[j][k] = sk[j][k] - p * sk[i][k];

			}

		}

	}



	for (int l = np - 1; l >= 0; l--) {

		u[l] = r1[l] / sk[l][l];

		for (int j = l - 1; j >= 0; j--) {

			double p1 = sk[j][l] / sk[l][l];

			sk[j][l] = 0.;

			r1[j] = r1[j] - p1 * r1[l];

		}

	}



	//!restore sk from bsk and r1 from br1 

	for (int i = 0; i < np; i++) {

		r1[i] = br1[i];

		for (int j = 0; j < np; j++) {

			sk[i][j] = bsk[i][j];

		}

	}



}





double EvaluateScore1(double w[pmax], int T, int p,

	double y[Tmax], double Z[pmax][Tmax], double b0,

	int data_y[Tmax]) {



	//locals 

	double beta[pmax];

	double sump[Tmax];

	int firep[Tmax], sfire;

	double hfire;



	//initializations 

	//hfire = 0.0f;
	hfire = 0.0;

	for (int i = 0; i < T; i++) {

		//sump[i] = 0.0f;

		//firep[i] = 0.0f;

		sump[i] = 0.0;

		firep[i] = 0.0;

	}



	for (int i = 0; i < p; i++) {

		beta[i] = w[i];

	}

	//FILE *pff;
	//pff = fopen("C:\\Storage\\Kostas\\Research\\VisualStudio2017\\HITS_Horowitz_Cpp\\HITS_Horowitz_Cpp\\diary.txt", "a");


	sfire = 0;

	for (int i = 0; i < T; i++) {

		//sump[i] = 0.0f;
		sump[i] = 0.0;

		for (int j = 0; j < p; j++) {

			//sump[i] = sump[i] + beta[j] * Z[j][i]; 

			sump[i] = sump[i] + beta[j] * Z[j + 1][i];

		}

		//sump[i] = sump[i] + b0; 

		sump[i] = sump[i] + b0 * Z[0][i];

		//fprintf(pff, "%.15lf\n", sump[i]);
		//if (sump[i] >= 0.0f) {
		//if (sump[i] >= 0.0) { //old code
		if ((sump[i] * data_y[i]) > EPSILON || fabs(sump[i]) < EPSILON) { //new code

			firep[i] = 1;

		}

		else

		{

			firep[i] = -1;

		}

		//sfire = sfire + data_y[i] * firep[i]; //old code
		sfire = sfire + firep[i]; //new code

	}

	//sfire is score in - 1 1 range 

	//hfire is score in 0 1 range 

	//EvaluateScore1 = sfire 

	hfire = (double)sfire / (double)T;

	hfire = (1.0 + hfire) / 2.0;


	//fclose(pff);
	return hfire;

}



double get_random() { return (double)rand() / (double)RAND_MAX; }




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

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDisplayGridMFCDlg dialog



CDisplayGridMFCDlg::CDisplayGridMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DISPLAYGRIDMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDisplayGridMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_LIST2, m_listCtrl2);
	DDX_Control(pDX, IDC_PROGRESS1, m_PrgCtrl);
}

BEGIN_MESSAGE_MAP(CDisplayGridMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDisplayGridMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDisplayGridMFCDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CDisplayGridMFCDlg::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()


// CDisplayGridMFCDlg message handlers

BOOL CDisplayGridMFCDlg::OnInitDialog()
{
	/*CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control*/

	//https://www.tutorialspoint.com/mfc/mfc_list_control.htm#

	CDialogEx::OnInitDialog();

	// Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);         // Set small icon

	// TODO: Add extra initialization here
	// Ask Mfc to create/insert a column
	/*m_listCtrl.InsertColumn(
		0,              // Rank/order of item 
		L"ID",          // Caption for this header 
		LVCFMT_LEFT,    // Relative position of items under header 
		100);           // Width of items under header

	m_listCtrl.InsertColumn(1, L"Name", LVCFMT_CENTER, 80);
	m_listCtrl.InsertColumn(2, L"Age", LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(3, L"Address", LVCFMT_LEFT, 80);*/

	FILE *pParams;
	pParams = fopen("params.txt", "r");
	fscanf(pParams, "n =%d\n", &m_N);
	fscanf(pParams, "p =%d\n", &m_p);
	int T = m_N;
	int p = m_p;
	fscanf(pParams, "b0 =%d\n", &m_b0);
	double b0 = m_b0;
	fscanf(pParams, "Nreps =%d\n", &m_Nreps);
	int Nreps = m_Nreps;
	fscanf(pParams, "d =%lf\n", &m_d);
	double d = m_d;
	fclose(pParams);

#define NUMBER_OF_STRING 20
#define MAX_STRING_SIZE 8
//#define MAX_STRING_SIZE 40
	char names[NUMBER_OF_STRING][MAX_STRING_SIZE] = { 0 };
	char name[MAX_STRING_SIZE];

	FILE *pNames;
	pNames = fopen("names.txt", "r");
	int pp = p + 1;
	for (int ii = 0; ii < pp; ii++) {
		fscanf(pNames, "%s", name);
		//int res = strcmp(name, "OBS");
		for (int jj = 0; jj < strlen(name); jj++) {
			names[ii][jj] = name[jj];
		}		
		//int res2 = strcmp(names[0], "OBS");
	}
	//fscanf(pNames, "%s", name);
	int temp = 1;
	
	for (int ii = 0; ii <= p; ii++) {
		char buf2[8];
		for (int jj = 0; jj < 8; jj++) {
			buf2[jj] = names[ii][jj];
		}
		
		char *vIn = buf2;
		wchar_t * vOut = new wchar_t[strlen(vIn) + 1];
		mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));

		m_listCtrl.InsertColumn(ii, vOut, LVCFMT_CENTER, 80);


	}
	
	//m_listCtrl.InsertColumn(0, L"OBS", LVCFMT_CENTER, 80);

	//m_listCtrl.InsertColumn(
	//	1,              // Rank/order of item
	//	L"HW",          // Caption for this header
	//	LVCFMT_LEFT,    // Relative position of items under header
	//	80);           // Width of items under header

	//m_listCtrl.InsertColumn(2, L"CHILD", LVCFMT_CENTER, 80);
	//m_listCtrl.InsertColumn(3, L"YCHILD", LVCFMT_LEFT, 80);
	//m_listCtrl.InsertColumn(4, L"EDU", LVCFMT_LEFT, 80);
	//m_listCtrl.InsertColumn(5, L"AGE", LVCFMT_LEFT, 80);
	//m_listCtrl.InsertColumn(6, L"AGE2", LVCFMT_LEFT, 80);
	//m_listCtrl.InsertColumn(7, L"INTCPT", LVCFMT_LEFT, 80);

	int nItem;

	/*nItem = m_listCtrl.InsertItem(0, L"1");
	m_listCtrl.SetItemText(nItem, 1, L"Mark");
	m_listCtrl.SetItemText(nItem, 2, L"45");
	m_listCtrl.SetItemText(nItem, 3, L"Address 1");

	nItem = m_listCtrl.InsertItem(0, L"2");
	m_listCtrl.SetItemText(nItem, 1, L"Allan");
	m_listCtrl.SetItemText(nItem, 2, L"29");
	m_listCtrl.SetItemText(nItem, 3, L"Address 2");

	nItem = m_listCtrl.InsertItem(0, L"3");
	m_listCtrl.SetItemText(nItem, 1, L"Ajay");
	m_listCtrl.SetItemText(nItem, 2, L"37");
	m_listCtrl.SetItemText(nItem, 3, L"Address 3");*/

	/*nItem = m_listCtrl.InsertItem(0, L"1");
	m_listCtrl.SetItemText(nItem, 1, L"10.787");
	m_listCtrl.SetItemText(nItem, 2, L"3.0");
	m_listCtrl.SetItemText(nItem, 3, L"0.900");
	m_listCtrl.SetItemText(nItem, 4, L"8");
	m_listCtrl.SetItemText(nItem, 5, L"1");
	m_listCtrl.SetItemText(nItem, 6, L"1");
	m_listCtrl.SetItemText(nItem, 7, L"0");
	m_listCtrl.SetItemText(nItem, 8, L"1");*/

	FILE *pf;
	//pf = fopen("X_Martins.txt", "r");
	pf = fopen("X.txt", "r");
	float ftemp;
	float X[2339][8];
	//float* XXX = NULL;
	//XXX = new float[T];
	float** XXX = new float*[T];
	for (int i = 0; i < T; ++i)
		XXX[i] = new float[p+1];

	//https://www.geeksforgeeks.org/gcvt-convert-float-value-string-c/#:~:text=This%20function%20is%20used%20to,It%20is%20number%20of%20digits.
	float xx = 123.4567;
	char buf[15]; 
	//wchar_t buf[15];


	_gcvt(xx, 6, buf);
	char *vIn = buf;
	wchar_t * vOut = new wchar_t[strlen(vIn) + 1];
	mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));

	/*char * vIn = "FOO";
	wchar_t * vOut = new wchar_t[strlen(vIn) + 1];
	mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));*/

	for (int i = 0; i < T; i++) {
		for (int j = 0; j < (p+1); j++) {
			fscanf(pf, "%f", &ftemp);
			//X[i][j] = ftemp;
			XXX[i][j] = ftemp;
		}
		fscanf(pf, "\n");
	}


	for (int i = 0; i < T; i++) {

		//xx = X[i][0];
		xx = XXX[i][0];
		_gcvt(xx, 6, buf);
		char *vIn = buf;
		wchar_t * vOut = new wchar_t[strlen(vIn) + 1];
		mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));
		nItem = m_listCtrl.InsertItem(0, vOut);

		for (int j = 1; j < (p+1); j++) {
			//xx = X[i][j];
			xx = XXX[i][j];
			_gcvt(xx, 6, buf);
			char *vIn = buf;
			wchar_t * vOut = new wchar_t[strlen(vIn) + 1];
			mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));
			//nItem = m_listCtrl.InsertItem(0, vOut);

			//nItem = m_listCtrl.InsertItem(0, vOut);
			//nItem = m_listCtrl.InsertItem(0, L"1");
			/*m_listCtrl.SetItemText(nItem, 1, L"10.787");
			m_listCtrl.SetItemText(nItem, 2, L"3.0");
			m_listCtrl.SetItemText(nItem, 3, L"0.900");
			m_listCtrl.SetItemText(nItem, 4, L"8");
			m_listCtrl.SetItemText(nItem, 5, L"1");
			m_listCtrl.SetItemText(nItem, 6, L"1");
			m_listCtrl.SetItemText(nItem, 7, L"0");
			m_listCtrl.SetItemText(nItem, 8, L"1");*/
			m_listCtrl.SetItemText(nItem, j,vOut);
		}
	}

	//now display y values two in second control
	m_listCtrl2.InsertColumn(0, L"OBS", LVCFMT_CENTER, 80);

	m_listCtrl2.InsertColumn(
		1,              // Rank/order of item
		L"y",          // Caption for this header
		LVCFMT_LEFT,    // Relative position of items under header
		80);           // Width of items under header

	FILE *pf2;
	//pf2 = fopen("y_Martins.txt", "r");
	pf2 = fopen("y.txt", "r");
	//float ftemp;
	//float y[2339][2];
	float** yyy = new float*[T];
	for (int i = 0; i < T; ++i)
		yyy[i] = new float[2];

	//https://www.geeksforgeeks.org/gcvt-convert-float-value-string-c/#:~:text=This%20function%20is%20used%20to,It%20is%20number%20of%20digits.
	xx = 123.4567;
	//char buf[15];
	//wchar_t buf[15];


	_gcvt(xx, 6, buf);
	char *vIn2 = buf;
	wchar_t * vOut2 = new wchar_t[strlen(vIn2) + 1];
	mbstowcs_s(NULL, vOut2, strlen(vIn2) + 1, vIn2, strlen(vIn2));

	/*char * vIn = "FOO";
	wchar_t * vOut = new wchar_t[strlen(vIn) + 1];
	mbstowcs_s(NULL, vOut, strlen(vIn) + 1, vIn, strlen(vIn));*/

	for (int i = 0; i < T; i++) {
		for (int j = 0; j < 2; j++) {
			fscanf(pf2, "%f", &ftemp);
			//y[i][j] = ftemp;
			yyy[i][j] = ftemp;
		}
		fscanf(pf2, "\n");
	}

	for (int i = 0; i < T; i++) {

		//xx = y[i][0];
		xx = yyy[i][0];
		_gcvt(xx, 6, buf);
		char *vIn2 = buf;
		wchar_t * vOut2 = new wchar_t[strlen(vIn2) + 1];
		mbstowcs_s(NULL, vOut2, strlen(vIn2) + 1, vIn2, strlen(vIn2));
		nItem = m_listCtrl2.InsertItem(0, vOut2);

		for (int j = 1; j < 2; j++) {
			//xx = y[i][j];
			xx = yyy[i][j];
			_gcvt(xx, 6, buf);
			char *vIn2 = buf;
			wchar_t * vOut2 = new wchar_t[strlen(vIn2) + 1];
			mbstowcs_s(NULL, vOut2, strlen(vIn2) + 1, vIn2, strlen(vIn2));
			//nItem = m_listCtrl.InsertItem(0, vOut);

			//nItem = m_listCtrl.InsertItem(0, vOut);
			//nItem = m_listCtrl.InsertItem(0, L"1");
			/*m_listCtrl.SetItemText(nItem, 1, L"10.787");
			m_listCtrl.SetItemText(nItem, 2, L"3.0");
			m_listCtrl.SetItemText(nItem, 3, L"0.900");
			m_listCtrl.SetItemText(nItem, 4, L"8");
			m_listCtrl.SetItemText(nItem, 5, L"1");
			m_listCtrl.SetItemText(nItem, 6, L"1");
			m_listCtrl.SetItemText(nItem, 7, L"0");
			m_listCtrl.SetItemText(nItem, 8, L"1");*/
			m_listCtrl2.SetItemText(nItem, j, vOut2);
		}
	}

	//m_PrgCtrl.StepIt();
	//main0(); //does all the things...
	//move it to button click OK
	return TRUE; // return TRUE unless you set the focus to a control
}

void CDisplayGridMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDisplayGridMFCDlg::OnPaint()
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
HCURSOR CDisplayGridMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CDisplayGridMFCDlg::main0() {
	

		/*#define Tmax  1000

		#define pmax  10

		#define Itmax  1000*/



		int p, T;

		double y[Tmax], Z[pmax][Tmax], b0;

		int data_y[Tmax];

		int id[Tmax];



		//locals 

		double A[pmax][pmax], b[pmax], bb[pmax];

		int i, j, l, ii, jj, ll, mii, mjj, mll, dii, djj, dll;

		int n, k;

		double mA[pmax][pmax], mb[pmax], bmb[pmax];

		double x[pmax], score1, MaxScore0, MaxScore, MaxScoreEver, temp;

		//int p, T; 



		int row;

		int dummy, dummy1, dummy2;

		double Af[pmax][pmax], bf[pmax], bbf[pmax];

		int mqq, qq, q;

		double ArgMax[pmax], MaxScoreF, MaxScoreFF;



		float time_begin, time_middle, time_end;



		int nat[pmax], natsol[pmax], natcur[pmax], natever[pmax];



		bool anstrivial;



		double xlbound[pmax], xubound[pmax];



		int Mat[pmax][Itmax], snat[pmax], snatsol[pmax], snatcur[pmax];



		bool anstabu;









		//std::cout << "Hello World!\n";  

		printf("Hello World!\n");



		//input files 

		FILE* pf1;

		//pf1 = fopen("Horowitz93fortran_y.txt", "r"); 

		//pf1 = fopen("y_Mroz.txt", "r"); 

		//pf1 = fopen("y_Martins.txt", "r");
		
		pf1 = fopen("y.txt", "r");

		//pf1 = fopen("y_Gerfin.txt", "r");

		FILE* pf2;

		//pf2 = fopen("Horowitz93fortran_z_intcpt.txt", "r"); 

		//pf2 = fopen("X_Mroz.txt", "r"); 

		//pf2 = fopen("X_Martins.txt", "r");

		pf2 = fopen("X.txt", "r");

		//pf2 = fopen("X_Gerfin.txt", "r");

		//output files 

		FILE* pf3;

		//pf3 = fopen("Horowitz93fortran_tabusearch_opt_res_iter100.out","w"); 

		//pf3 = fopen("Mroz87Cpp_tabusearch_opt_res_iter200.out", "w"); 

		//pf3 = fopen("Martins03Cpp_tabusearch_opt_res_iter200.out", "w");

		pf3 = fopen("DemoExample_Cpp_tabusearch_opt_res_iter200.out", "w");

		//pf3 = fopen("Gerfin96Cpp_tabusearch_opt_res_iter200.out", "w");

		FILE* pf4;

		//pf4 = fopen("Horowitz93fortran_tabusearch_opt_info_iter100.out", "w"); 

		//pf4 = fopen("Mroz87Cpp_tabusearch_opt_info_iter200.out", "w"); 

		//pf4 = fopen("Martins03Cpp_tabusearch_opt_info_iter200.out", "w");

		pf4 = fopen("DemoExample_Cpp_tabusearch_opt_info_iter200.out", "w");

		//pf4 = fopen("Gerfin96Cpp_tabusearch_opt_info_iter200.out", "w");

		FILE* pf5;

		//pf5 = fopen("Horowitz93fortran_tabusearch_opt_log_iter100.out", "w"); 

		//pf5 = fopen("Mroz87Cpp_tabusearch_opt_log_iter200.out", "w"); 

		//pf5 = fopen("Martins03Cpp_tabusearch_opt_log_iter200.out", "w");

		pf5 = fopen("DemoExample_Cpp_tabusearch_opt_log_iter200.out", "w");

		//pf5 = fopen("Gerfin96Cpp_tabusearch_opt_log_iter200.out", "w");



		printf("this is Discrete tabu search algorithm\n");

		printf("for maximum score estimation\n");

		fprintf(pf3, "this is Discrete tabu search algorithm\n");

		fprintf(pf3, "for maximum score estimation\n");

		fprintf(pf4, "this is Discrete tabu search algorithm\n");

		fprintf(pf4, "for maximum score estimation\n");

		fprintf(pf5, "this is Discrete tabu search algorithm\n");

		fprintf(pf5, "for maximum score estimation\n");



		printf("Give dimension of sample T and number of parameters p\n");

		printf("(intercept is indeed counted in p due to normalization)\n");



		//scanf("%d %d", &T, &p);
		FILE *pParams;
		pParams = fopen("params.txt", "r");
		fscanf(pParams, "n =%d\n", &m_N);
		fscanf(pParams, "p =%d\n", &m_p);

		T = m_N;
		p = m_p;
		//T = 2339;
		//p = 7;



		//read y and Z 

		int pp = p + 1;

		//read y 

		for (int i = 0; i < T; i++) {

			fscanf(pf1, "%d %d\n", &id[i], &data_y[i]);

			id[i] = id[i] - 1; //make id numbers from 1..n to 0..n-1 

		}

		for (int i = 0; i < T; i++) {

			for (int j = 0; j < pp; j++) {

				fscanf(pf2, "%lf", &Z[j][i]);

			}

			fscanf(pf2, "\n");

		}

		//read Z 

		for (int j = 1; j < pp + 1; j++) {

			for (int i = 0; i < T; i++) {

				Z[j - 1][i] = Z[j][i];

			}

		}

		//read b0 

		printf("Give case for coefficient of variable 0, b(0): 1 or -1\n");

		//scanf("%lf", &b0);
		fscanf(pParams, "b0 =%d\n", &m_b0);
		b0 = m_b0;
		//b0 = -1;


		//compute y as data_y but as a double (trivial, y not used anywhere) 

		for (int i = 0; i < T; i++) {

			y[i] = data_y[i];

		}


		//int Nreps = 4;
		fscanf(pParams, "Nreps =%d\n", &m_Nreps);
		int Nreps = m_Nreps;
		fscanf(pParams, "d =%lf\n", &m_d);
		double d = m_d;
		fclose(pParams);
		//int Nreps = 10;
		//input of data_y,Z,b0 and definition of y is ok! 

		//CProgressCtrl *myCntrl;
		/*m_PrgCtrl.EnableWindow(TRUE);
		m_PrgCtrl.SetRange(0, 100);
		m_PrgCtrl.SetPos(50);
		m_PrgCtrl.SetStep(25);*/

		//m_PrgCtrl.EnableWindow(TRUE);
		DisableProcessWindowsGhosting();

		for (int iReps = 1; iReps <= Nreps; iReps++) {

			
			//m_PrgCtrl.SetPos(25*iReps);
			/*for (int ii = 1; ii < 10; ii++) {
				Sleep(500);
				m_PrgCtrl.SetPos(ii*10);

			}*/
			//Sleep(2000);
			//m_PrgCtrl.SetPos(iReps * 25);
			m_PrgCtrl.SetPos((int)(iReps*(100/Nreps)));
			printf("==========%d==========\n", iReps);
			fprintf(pf3, "==========%d==========\n", iReps);
			fprintf(pf4, "==========%d==========\n", iReps);
			fprintf(pf5, "==========%d==========\n", iReps);

			double rr = 0;
			//srand(time(NULL)); // randomize seed
			rr = get_random(); // call the function to get a different value of n every time

			printf("%f\n", rr);  // print your number

			std::clock_t c_start = std::clock();

			//Start process... 

			//probit initial solution for clogit_aironly dataset 

			//mii = 48 

			//mjj = 100 

			//mll = 160 

			//naive initial solution for clogit_aironly_dataset 



			/*mii = 83 - 1;

			mjj = 599 - 1;

			mll = 827 - 1;

			mqq = 100 - 1;*/

			/*mii = 123 - 1;

			mjj = 312 - 1;

			mll = 514 - 1;

			mqq = 778 - 1;*/

			/*mii = 150 - 1;

			mjj = 250 - 1;

			mll = 450 - 1;

			mqq = 550 - 1;

			int mrr = 650 - 1;*/





			/*nat[0]=mii;

			nat[1]=mjj;

			nat[2]=mll;

			nat[3]=mqq;*/

			/*nat[0] = mii;

			nat[1] = mjj;

			nat[2] = mll;

			nat[3] = mqq;

			nat[4] = mrr;*/

			for (int j = 0; j < (p - 1); j++) {

				rr = get_random();
				nat[j] = 0 + floor(rr*T);

			}
			/*nat[0] = 782;

			nat[1] = 215;

			nat[2] = 222;

			nat[3] = 247;

			nat[4] = 519;

			nat[5] = 690;

			nat[6] = 791;*/

			/*nat[0] = 205 - 1;

			nat[1] = 216 - 1;

			nat[2] = 223 - 1;

			nat[3] = 248 - 1;

			nat[4] = 520 - 1;

			nat[5] = 691 - 1;

			nat[6] = 792 - 1;*/







			for (int j = 0; j < (p - 1); j++) {

				//xlbound[j] = -1e4;

				//xubound[j] = +1e4;

				xlbound[j] = -d;

				xubound[j] = d;
			}

			//now on, extract intercept from number of parameters 

			p = p - 1;



			bsort(p, nat, snat);



			for (int j = 0; j < p; j++) {

				nat[j] = snat[j];

			}



			for (int j = 0; j < p; j++) {

				natsol[j] = nat[j];

			}

			//mqq=4 

			for (int j = 0; j < p; j++) {

				if (y[id[nat[j]]] >= 0) {

					mb[j] = 1;

				}

				else {

					mb[j] = 0;

				}

			}

			//compute mA 

			for (j = 0; j < p; j++) {

				for (i = 0; i < p; i++) {

					mA[i][j] = (1 - 2 * mb[i])*Z[j + 1][id[nat[i]]];

				}

			}

			//don't overwrite mb, define bmb 

			for (int i = 0; i < p; i++) {

				bmb[i] = b0 * (-1 + 2 * mb[i])*Z[0][id[nat[i]]];

			}



			gelim(p, mA, bmb, x);



			for (int i = 0; i < p; i++) {

				if (x[i] < xlbound[i] || x[i] > xubound[i]) {

					printf("Initial Solution is out of bounds\n");

					printf("Increase bounds or try another initial solution\n");

					fprintf(pf3, "Initial Solution is out of bounds\n");

					fprintf(pf3, "Increase bounds or try another initial solution\n");

					fprintf(pf4, "Initial Solution is out of bounds\n");

					fprintf(pf4, "Increase bounds or try another initial solution\n");

					fprintf(pf5, "Initial Solution is out of bounds\n");

					fprintf(pf5, "Increase bounds or try another initial solution\n");

					return 0;

				}



			}



			score1 = EvaluateScore1(x, T, p, y, Z, b0, data_y);

			MaxScore0 = score1;



			//ii = mii; 

			//jj = mjj; 

			//ll = mll; 

			MaxScore = MaxScore0;

			MaxScoreEver = MaxScore0;

			printf("Initial score is %lf\n", MaxScore0);

			fprintf(pf3, "Initial score is %lf\n", MaxScore0);

			fprintf(pf4, "Initial score is %lf\n", MaxScore0);

			fprintf(pf5, "Initial score is %lf\n", MaxScore0);

			printf("Initial attributes: ");

			for (int j = 0; j < p; j++) {

				printf("%d ", nat[j]);

			}

			printf("\n");

			fprintf(pf3, "Initial attributes: ");

			for (int j = 0; j < p; j++) {

				fprintf(pf3, "%d ", nat[j]);

			}

			fprintf(pf3, "\n");

			fprintf(pf4, "Initial attributes: ");

			for (int j = 0; j < p; j++) {

				fprintf(pf4, "%d ", nat[j]);

			}

			fprintf(pf4, "\n");

			fprintf(pf5, "Initial attributes: ");

			for (int j = 0; j < p; j++) {

				fprintf(pf5, "%d ", nat[j]);

			}

			fprintf(pf5, "\n");



			for (int j = 0; j < p; j++) {

				for (int i = 0; i < ntenure; i++) {

					Mat[j][i] = -1;

				}

			}

			//ready to start loop of BMNH discrete tabu search for max score estimation 

			//Start of iterative improvement / deterioration tabusearch procedure 

			//Rewrite of this part and on  ... OK done! 

			int csubstitutionstotal = 0;

			int cbdsubseq = 0;

		label999: double mytemp = 1.0;

			int cbd = 0;

			int cinfsd = 0;

			double dz = -1.;

			double dzmax = -1.;

			n = T;

			k = p;

			row = 0;



			bsort(p, natsol, snatsol);



			for (int i = 0; i < p; i++) {

				natsol[i] = snatsol[i];

			}

			//compute b 

			for (int i = 0; i < p; i++) {

				if (y[id[natsol[i]]] >= 0) {

					b[i] = 1;

				}

				else

				{

					b[i] = 0;

				}

			}

			//compute A 

			for (int j = 0; j < p; j++) {

				for (int i = 0; i < p; i++) {

					A[i][j] = (1 - 2 * b[i]) * Z[j + 1][id[natsol[i]]];

				}

			}

			//don't overwrite b, define bb 

			for (int i = 0; i < p; i++) {

				bb[i] = b0 * (-1 + 2 * b[i])*Z[0][id[natsol[i]]];

			}



			for (int i = 0; i < p; i++) {

				for (int j = 0; j < n; j++) {

					anstrivial = false;

					for (int iii = 0; iii < p; iii++) {

						if (nat[iii] == id[j]) {

							anstrivial = true;

						}

					}

					if (anstrivial) {

						goto label100; //generate another j point which is likely to be ok 

					}

					//insert j in pBYp linear system, remove nat(i) from i position of pBYp linear system 

					row = row + 1;

					//nat 

					for (int iii = 0; iii < p; iii++) {

						if (iii == i) {

							//nat[iii] = id[j]; //fortran 

							nat[iii] = id[j]; //C++ 

						}

						else

						{

							nat[iii] = natsol[iii];

						}

					}

					//check tabu-active status of trial point nat through membership in Mat - ntenure - last columns 

					bsort(p, nat, snat);

					for (int jjj = 0; jjj < ntenure; jjj++) {

						anstabu = false;

						int ct = 0;

						for (int iii = 0; iii < p; iii++) {

							if (snat[iii] == Mat[iii][csubstitutionstotal + jjj]) {

								ct = ct + 1;

							}

						}

						if (ct == p) {

							anstabu = true;

							int jjjt = jjj;

						}

						if (anstabu) {

							goto label100; //generate another j point which is likely to be ok 

						}



					}



					//Now on, it is assumed that the trial solution is not tabu - active 

					//b 

					for (int iii = 0; iii < p; iii++) {

						if (y[nat[iii]] >= 0) {

							b[iii] = 1;

						}

						else

						{

							b[iii] = 0;

						}

					}

					//A 

					for (int iii = 0; iii < p; iii++) {

						for (int jjj = 0; jjj < p; jjj++) {

							A[jjj][iii] = (1 - 2 * b[jjj])*Z[iii + 1][id[nat[jjj]]];

						}

					}

					//bb 

					for (int iii = 0; iii < p; iii++) {

						bb[iii] = b0 * (-1 + 2 * b[iii])*Z[0][id[nat[iii]]];

					}



					gelim(p, A, bb, x);



					for (int iii = 0; iii < p; iii++) {

						if (x[iii] < xlbound[iii] || x[iii] > xubound[iii]) {

							cinfsd = cinfsd + 1;

							goto label100; //generate another j point which is likely to be ok 

										   //x trial point out of bounds  

						}

					}



					score1 = EvaluateScore1(x, T, p, y, Z, b0, data_y);

					dz = score1 - MaxScore;

					//benefitial swap 

					if (dz > 0) {

						cbd = cbd + 1;

						cbdsubseq = 0;

					}

					//Store Best solution ever 

					if (score1 > MaxScoreEver) {

						MaxScoreEver = score1;

						for (int iii = 0; iii < p; iii++) {

							natever[iii] = nat[iii];

						}

					}

					//choose biggest dz <=>0 to achieve BMNH 

					if (dz > dzmax) {

						dzmax = dz;

						for (int iii = 0; iii < p; iii++) {

							natcur[iii] = nat[iii];

						}

					}



				label100: double mytemp = 1.;

				} //j loop 

				if (i == 1) {

					double mytest = 1.0;

				}

			} // i loop 



			printf("%d neighbourhood moves checked for the current steps\n", row);

			fprintf(pf3, "%d neighbourhood moves checked for the current steps\n", row);

			fprintf(pf4, "%d neighbourhood moves checked for the current steps\n", row);

			printf("%d better domains than current solution\n", cbd);

			fprintf(pf3, "%d better domains than current solution\n", cbd);

			fprintf(pf4, "%d better domains than current solution\n", cbd);

			printf(" out of bounds domains rejected\n", cinfsd);

			fprintf(pf3, " out of bounds domains rejected\n", cinfsd);

			fprintf(pf4, " out of bounds domains rejected\n", cinfsd);



			// now get middle time 

			std::clock_t c_middle = std::clock();

			long double time_middle_ms = 1000.0 * (c_middle - c_start) / CLOCKS_PER_SEC;

			csubstitutionstotal = csubstitutionstotal + 1;



			if (cbd == 0) {

				cbdsubseq = cbdsubseq + 1;

			}

			printf("Elapsed for iteration %d is %lf ms \n", csubstitutionstotal, time_middle_ms);

			fprintf(pf3, "Elapsed for iteration %d is %lf ms \n", csubstitutionstotal, time_middle_ms);

			fprintf(pf4, "Elapsed for iteration %d is %lf ms \n", csubstitutionstotal, time_middle_ms);



			if (csubstitutionstotal == niterover || cbdsubseq == niternotimproved) {

				goto label1001;

			}

			else

			{

				//make update of the current step and move to 

				//natsol 

				for (int iii = 0; iii < p; iii++) {

					natsol[iii] = natcur[iii];

				}

				for (int iii = 0; iii < p; iii++) {

					nat[iii] = natsol[iii];

				}

				//MaxScore 

				MaxScore = MaxScore + dzmax;



				printf(" MaxScore = %.6f dzmax = %.6f Max Score Ever = %.6f \n", MaxScore, dzmax, MaxScoreEver);

				fprintf(pf3, " MaxScore = %.6f dzmax = %.6f Max Score Ever = %.6f \n", MaxScore, dzmax, MaxScoreEver);

				fprintf(pf4, " MaxScore = %.6f dzmax = %.6f Max Score Ever = %.6f \n", MaxScore, dzmax, MaxScoreEver);

				fprintf(pf5, " MaxScore = %.6f dzmax = %.6f Max Score Ever = %.6f \n", MaxScore, dzmax, MaxScoreEver);



				printf("Attributes are ");

				for (int iii = 0; iii < p; iii++) {

					printf("%d ", natsol[iii]);

				}

				printf("\n");

				fprintf(pf3, "Attributes are ");

				for (int iii = 0; iii < p; iii++) {

					fprintf(pf3, "%d ", natsol[iii]);

				}

				fprintf(pf3, "\n");

				fprintf(pf4, "Attributes are ");

				for (int iii = 0; iii < p; iii++) {

					fprintf(pf4, "%d ", natsol[iii]);

				}

				fprintf(pf4, "\n");



				//Update tabu history matrix Mat with natsol sorted as snatsol 

				bsort(p, natsol, snatsol);

				for (int j = 0; j < p; j++) {

					//Mat[j][ntenure + csubstitutionstotal] = snatsol[j]; 

					Mat[j][ntenure + csubstitutionstotal - 1] = snatsol[j];

				}



				goto label999;



			}



			//finally 

		label1001: double mytemp2 = 1.0;

			//bf 

			for (int iii = 0; iii < p; iii++) {

				if (y[id[natever[iii]]] >= 0) {

					bf[iii] = 1;

				}

				else

				{

					bf[iii] = 0;

				}

			}

			//Af 

			for (int j = 0; j < p; j++) {

				for (int jjj = 0; jjj < p; jjj++) {

					Af[jjj][j] = (1 - 2 * bf[jjj])*Z[j + 1][id[natever[jjj]]];

				}

			}

			//bbf 

			for (int j = 0; j < p; j++) {

				bbf[j] = b0 * (-1 + 2 * bf[j])*Z[0][id[natever[j]]];

			}



			gelim(p, Af, bbf, x);



			score1 = EvaluateScore1(x, T, p, y, Z, b0, data_y);



			for (int j = 0; j < p; j++) {

				ArgMax[j] = x[j];

			}

			MaxScoreF = MaxScoreEver;

			MaxScoreFF = score1;



			printf("Argmax is ");

			for (int j = 0; j < p; j++) {

				printf("%.6f ", ArgMax[j]);

			}

			printf(" \n");

			fprintf(pf3, "Argmax is ");

			for (int j = 0; j < p; j++) {

				fprintf(pf3, "%.6f ", ArgMax[j]);

			}

			fprintf(pf3, " \n");

			fprintf(pf4, "Argmax is ");

			for (int j = 0; j < p; j++) {

				fprintf(pf4, "%.6f ", ArgMax[j]);

			}

			fprintf(pf4, " \n");

			fprintf(pf5, "Argmax is ");

			for (int j = 0; j < p; j++) {

				fprintf(pf5, "%.6f ", ArgMax[j]);

			}

			fprintf(pf5, " \n");



			printf("Max Score Ever is %.6f \n", MaxScoreF);

			fprintf(pf3, "Max Score Ever is %.6f \n", MaxScoreF);

			fprintf(pf4, "Max Score Ever is %.6f \n", MaxScoreF);

			fprintf(pf5, "Max Score Ever is %.6f \n", MaxScoreF);



			printf("MaxScore at Argmax is %.6f \n", MaxScoreFF);

			fprintf(pf3, "MaxScore at Argmax is %.6f \n", MaxScoreFF);

			fprintf(pf4, "MaxScore at Argmax is %.6f \n", MaxScoreFF);

			fprintf(pf5, "MaxScore at Argmax is %.6f \n", MaxScoreFF);



			printf("coefficient of variable 0 b(0) is %.6f \n", b0);

			fprintf(pf3, "coefficient of variable 0 b(0) is %.6f \n", b0);

			fprintf(pf4, "coefficient of variable 0 b(0) is %.6f \n", b0);

			fprintf(pf5, "coefficient of variable 0 b(0) is %.6f \n", b0);



			printf("attributes are ");

			for (int j = 0; j < p; j++) {

				printf("%d ", natever[j]);

			}

			printf(" \n");

			fprintf(pf3, "attributes are ");

			for (int j = 0; j < p; j++) {

				fprintf(pf3, "%d ", natever[j]);

			}

			fprintf(pf3, " \n");

			fprintf(pf4, "attributes are ");

			for (int j = 0; j < p; j++) {

				fprintf(pf4, "%d ", natever[j]);

			}

			fprintf(pf4, " \n");

			fprintf(pf5, "attributes are ");

			for (int j = 0; j < p; j++) {

				fprintf(pf5, "%d ", natever[j]);

			}

			fprintf(pf5, " \n");



			// now get final time 

			std::clock_t c_end = std::clock();

			long double time_elapsed_ms = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;



			printf("ntenure= %d niterover = %d niternotimproved = %d \n", ntenure, niterover, niternotimproved);

			fprintf(pf3, "ntenure= %d niterover = %d niternotimproved = %d \n", ntenure, niterover, niternotimproved);

			fprintf(pf4, "ntenure= %d niterover = %d niternotimproved = %d \n", ntenure, niterover, niternotimproved);

			fprintf(pf5, "ntenure= %d niterover = %d niternotimproved = %d \n", ntenure, niterover, niternotimproved);



			printf("Time of computation was %lf  seconds\n", time_elapsed_ms / 1000.0);

			fprintf(pf3, "Time of computation was %lf  seconds\n", time_elapsed_ms / 1000.0);

			fprintf(pf4, "Time of computation was %lf  seconds\n", time_elapsed_ms / 1000.0);

			fprintf(pf5, "Time of computation was %lf  seconds\n", time_elapsed_ms / 1000.0);



			//fclose(pf3);

			//fclose(pf4);

			//fclose(pf5);

			//return 0;

			////p = 7;

			p = m_p;
		} //iReps

		fclose(pf3);

		fclose(pf4);

		fclose(pf5);


		return 0;

}

void CDisplayGridMFCDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_PrgCtrl.EnableWindow(TRUE);
	m_PrgCtrl.SetRange(0, 100);
	m_PrgCtrl.SetPos(75);
	m_PrgCtrl.SetStep(25);
	for (int ii = 1; ii < 10; ii++) {
		Sleep(250);
		m_PrgCtrl.SetPos(ii * 10);
		Sleep(250);
	}

	main0();
	//CDialogEx::OnOK();
}


void CDisplayGridMFCDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();
	CDialogEx::OnOK();
}


void CDisplayGridMFCDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
