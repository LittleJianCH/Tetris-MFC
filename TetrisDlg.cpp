
// TetrisDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Tetris.h"
#include "TetrisDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx {
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

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTetrisDlg dialog



CTetrisDlg::CTetrisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TETRIS_DIALOG, pParent), _game(20, 35) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTetrisDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

const COLORREF colorTableA[] = {
	RGB(255, 0, 0),
	RGB(0, 255, 0),
	RGB(0, 0, 255),
	RGB(255, 255, 0),
	RGB(255, 0, 255),
	RGB(0, 255, 255)
};

void CTetrisDlg::drawMainScreen() {
	CRect rect;

	CWnd* wnd = GetDlgItem(IDC_PIC_MAIN);
	CPaintDC dc(wnd);
	wnd->GetClientRect(&rect);

	auto screen = _game.getCurrentScreen();
	int height = _game.getHeight();
	int width = _game.getWidth();
	int heightSize = rect.Height() / height;
	int widthSize = rect.Width() / width;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int iType = screen[i][j];

			if (iType > 0 && iType < 8) {
				CClientDC dcc(wnd);
				CBrush brush(colorTableA[iType - 1]);
				dcc.FillRect(
					CRect(
						j * widthSize,
						i * heightSize,
						(j + 1) * widthSize,
						(i + 1) * heightSize),
					&brush);
			}
			if (iType > 0) { // when iType equals 8, draw a hollow rectangle to indicate the position of block falling down
				CBrush* pBrush = CBrush::FromHandle((HBRUSH)
					GetStockObject(NULL_BRUSH));
				CBrush* pOldBrush = dc.SelectObject(pBrush);

				dc.Rectangle(
					j * widthSize,
					i * heightSize,
					(j + 1) * widthSize,
					(i + 1) * heightSize);
				dc.SelectObject(pOldBrush);
			}
		}
	}
	wnd->RedrawWindow();
}

void CTetrisDlg::drawPreviewScreen() {
	auto& nextTetromino = _game.getNextTetromino();

	int type = nextTetromino.getType();

	CRect rect;
	CWnd* wnd = GetDlgItem(IDC_PIC_PREVIEW);
	CPaintDC dc(wnd);
	wnd->GetClientRect(&rect);
	CClientDC dcc(wnd);
	CBrush brush(colorTableA[type - 1]);

	int widthSize = rect.Width() / 5;
	int heightSize = rect.Height() / 4;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (nextTetromino.getBlock(i, j) != 0) {
				dcc.FillRect(
					CRect(
						(j + 1) * widthSize,
						i * heightSize,
						(j + 2) * widthSize,
						(i + 1) * heightSize),
					&brush);

				CBrush* pBrush = CBrush::FromHandle((HBRUSH)
					GetStockObject(NULL_BRUSH));
				CBrush* pOldBrush = dc.SelectObject(pBrush);
				dc.Rectangle(
					(j + 1) * widthSize,
					i * heightSize,
					(j + 2) * widthSize,
					(i + 1) * heightSize);
				dc.SelectObject(pOldBrush);
			}
		}
	}

	wnd->RedrawWindow();
}

void CTetrisDlg::onKeyDown(UINT nChar) {
	if (_game.getState() == GAME_STATE::GO) {
		_game.input(nChar);
	}
	if (nChar == VK_SPACE) {
		OnClickedButtonStop();
	}
	Invalidate(true);
}

BEGIN_MESSAGE_MAP(CTetrisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_PIC_MAIN, &CTetrisDlg::OnStnClickedPicMain)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTetrisDlg::OnClickedButtonStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CTetrisDlg::OnClickedButtonStop)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTetrisDlg message handlers

BOOL CTetrisDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTetrisDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTetrisDlg::OnPaint() {
	if (IsIconic()) {
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
	else {
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcMem;

		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;

		bmpBackground.LoadBitmap(IDB_BITMAP_BACKGROUND);

		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);

		CBitmap* bmpOld = dcMem.SelectObject(&bmpBackground);

		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem,
			0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

		drawMainScreen();
		drawPreviewScreen();

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTetrisDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}



void CTetrisDlg::OnStnClickedPicMain() {
	// TODO: Add your control notification handler code here
}


void CTetrisDlg::OnClickedButtonStart() {
	if (_game.getState() == GAME_STATE::GO) {
		KillTimer(1);
	}

	_game.start();
	SetTimer(1, _game.getSpeed(), NULL);
}


void CTetrisDlg::OnTimer(UINT_PTR nIDEvent) {
	if (!_game.go()) {
		KillTimer(1);
		TCHAR* msg = _T("GAME OVER");
		MessageBox(msg);
	}
	else {
		// show the game score
		CString str;
		str.Format(_T("Score: %d"), _game.getScore());
		GetDlgItem(IDC_STATIC_SCORE)->SetWindowText(str);
	}

	Invalidate(true);
}


void CTetrisDlg::OnClickedButtonStop() {
	_game.switchPauseAndContinue();

	if (_game.getState() == GAME_STATE::PAUSE) {
		KillTimer(1);
	}
	else {
		SetTimer(1, _game.getSpeed(), NULL);
	}
}

void CTetrisDlg::OnClose() {

	if (_game.getState() == GAME_STATE::GO) {
		KillTimer(1);
	}

	CDialogEx::OnClose();
}


BOOL CTetrisDlg::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		onKeyDown((UINT)pMsg->wParam);
		return true;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CTetrisDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_SCORE) {
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	
	return hbr;
}
