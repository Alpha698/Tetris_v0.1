#include<afxwin.h>
#include<stdio.h>
#include<time.h>
#include"resource.h"


enum ShapeType { line, square, rightL, leftL, pyramide, leftZ, rightZ};	//The type of shape:
#define size 20	//size of shape square


//--------------------------------
// class Shape 
//--------------------------------


class Shape
{
public:
	bool		cells[4][4]; // 4x4 matrix that conatains the shape
	ShapeType	curType; // current type of shape

	void	NewShape( ShapeType ); 
	void	Rotate();
	void	ClearShape( bool c[4][4] ); // clear the 4x4 matrix
};


void Shape::ClearShape( bool c[4][4] )
{
	
	for( int i=0; i<4; i++)
		for( int j=0; j<4; j++)
			c[i][j]=false;
}


void Shape::NewShape( ShapeType type )
{
	ClearShape(cells);
	int i;
	curType=type;

	switch(type)
	{
	case line:	for(i=0; i<4; i++)	
				cells[0][i]=true;
			break;

	case square:	for(i=0; i<2; i++)
				for(int j=0; j<2; j++)
					cells[i][j]=true;	
			break;

	case leftL:	for(i=0; i<3; i++)
				cells[0][i]=true;
			cells[1][2]=true;
			break;

	case rightL:	for(i=0; i<3; i++)
				cells[0][i]=true;
			cells[1][0]=true;
			break;

	case pyramide:	for(i=0; i<3; i++)
				cells[1][i]=true;
			cells[0][1]=true;
			break;

	case leftZ:	cells[0][0]=true; cells[1][0]=true;
				cells[1][1]=true; cells[2][1]=true;
				break;

	case rightZ:	cells[0][1]=true; cells[1][0]=true;
				cells[1][1]=true; cells[2][0]=true;
				break;
	}
}


void Shape::Rotate()
{

	switch(curType)
	{
	case line:
		{
			int k;
			if(cells[0][0]==true)
			{
				ClearShape(cells);
				for(k=0; k<4; k++)
					cells[k][1]=true;
			}
			else
			{
				ClearShape(cells);
				for(k=0; k<4; k++)
					cells[0][k]=true;
			}
		}
	case square: return;
	}

	bool tempShape[4][4];
	ClearShape(tempShape);

	for(int j=3-1 , c=0; j>=0 ; j-- , c++)
		for(int i=0; i<3; i++)
			tempShape[c][i]=cells[i][j];

	ClearShape(cells);

	for(int f=0; f<3; f++)
		for(int d=0; d<3; d++)
			cells[f][d]=tempShape[f][d];
}



//-----------------------------------
// Tetris window class
//-----------------------------------



class TetrisWnd : public CFrameWnd
{
public:
	TetrisWnd();
	void NewShape(); // appearance of new shape at the top
	COLORREF ShapeColor(int color); // some reserved colors
	void SetLevel(int);
	bool CheckLeft(); // check the possibility to move on the left
	bool CheckRight(); // check the possibility to move on the right
	void ShowTitle(); // show the window title
	void CheckForLine(); // check for the completed line
	bool IsAtBottom(); // check the possibility to move on the bottom
	void ClearStock(); // clear the field
	CRect GetRect(); // get the rectangle of shape's placement
	int color; // current color

private:
	CMenu	menu;
	Shape	shape;
	short	stock[20][10];
	int	curY;
	int	curX;
	bool	bGame;
	int	score;
	int	level;
	bool	level1;
	bool	level2;
	bool	level3;
	
protected:
	//{{AFX_VIRTUAL(TetrisWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(TetrisWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGameNew();
	afx_msg void OnGameExit();
	afx_msg void OnClose();
	afx_msg void OnLevelLevel1();
	afx_msg void OnUpdateLevelLevel1(CCmdUI* pCmdUI);
	afx_msg void OnLevelLevel2();
	afx_msg void OnUpdateLevelLevel2(CCmdUI* pCmdUI);
	afx_msg void OnLevelLevel3();
	afx_msg void OnUpdateLevelLevel3(CCmdUI* pCmdUI);
	afx_msg void OnHelpAbout();
	//}}AFX_MSG

	//{{AFX_DATA_MAP(TetrisWnd)
	//}}AFX_DATA_MAP

	//{{AFX_DATA(TetrisWnd)
	//}}AFX_DATA

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(TetrisWnd,CFrameWnd)
	//{{AFX_MSG_MAP(TetrisWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_GAME_NEW, OnGameNew)
	ON_COMMAND(ID_GAME_EXIT, OnGameExit)
	ON_WM_CLOSE()
	ON_COMMAND(ID_LEVEL_LEVEL1, OnLevelLevel1)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_LEVEL1, OnUpdateLevelLevel1)
	ON_COMMAND(ID_LEVEL_LEVEL2, OnLevelLevel2)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_LEVEL2, OnUpdateLevelLevel2)
	ON_COMMAND(ID_LEVEL_LEVEL3, OnLevelLevel3)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_LEVEL3, OnUpdateLevelLevel3)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


TetrisWnd::TetrisWnd()
{
	CString s=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW ,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		AfxGetApp()->LoadIcon(IDI_IC));
	Create(s,"Tetris");
	menu.LoadMenu(IDR_MENU);
	SetMenu(&menu);
	level=700;
	SetLevel(1);
	srand(time(0));
	ClearStock();
}


/////////////////////////////////////////////////////////////////
class MyApp : public CWinApp
{
public:
	virtual BOOL InitInstance()
	{
		m_pMainWnd = new TetrisWnd;
		m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
		return TRUE;
	}
};
MyApp app;
//////////////////////////////////////////////////////////////////


BOOL TetrisWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_THICKFRAME;
	cs.cx = 210;
	cs.cy = 450;
	return CFrameWnd::PreCreateWindow(cs);
}

void TetrisWnd::OnPaint()
{
	CPaintDC dc(this); 	
	int i,j;
	
	CBrush brush;
	brush.CreateSolidBrush(ShapeColor(color));
	dc.SelectObject(&brush);
	
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			if(shape.cells[i][j]==true)
				dc.Rectangle(curX+j*size, curY+i*size, curX+(j+1)*size, curY+(i+1)*size);
		}
	}

	for(i=0; i<20; i++)
	{
		for(j=0; j<10; j++)
		{
			if(stock[i][j]!=-1)
			{
				brush.DeleteObject();
				brush.CreateSolidBrush(ShapeColor(stock[i][j]));
				dc.SelectObject(&brush);
				dc.Rectangle(j*size,i*size,(j+1)*size,(i+1)*size);
			}
		}
	}
}

CRect TetrisWnd::GetRect()
{
	CRect r;
	r.left=curX-size;
	r.right=curX+size*5;
	r.top=curY-size;
	r.bottom=curY+size*5;
	return r;
}

void TetrisWnd::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1 && bGame==true)
	{
		if(IsAtBottom())
		{
			for(int i=0; i<4; i++)
				for(int j=0; j<4; j++)
				{
					if(shape.cells[i][j]==true)
						stock[curY/20+i][curX/20+j]=color;
				}
			NewShape();
			if(IsAtBottom())
			{
				bGame=false;
			}
			InvalidateRect(GetRect());
			CheckForLine();
			return;
		}
		curY+=20;
		InvalidateRect(GetRect());
	}	
	else if(bGame==false)
	{
		KillTimer(1);
		CString s;
		s.Format("%d",score);
		MessageBox("You are lost !\rYour score is : "+s);
	}

	CFrameWnd::OnTimer(nIDEvent);
}

void TetrisWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_CONTROL:
	case VK_UP:			shape.Rotate();	break;
	case VK_LEFT:		if(CheckLeft()) return; curX-=20; break;
	case VK_RIGHT:		if(CheckRight()) return; curX+=20; break;
	case VK_DOWN:		KillTimer(1); SetTimer(1,20,NULL); break;
	}
	InvalidateRect(GetRect());

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void TetrisWnd::OnGameNew()
{
	ClearStock();
	score=0;
	bGame=true;
	SetLevel(1);
	NewShape();
	ShowTitle();
	Invalidate();
}

void TetrisWnd::ClearStock()
{
	for(int i=0; i<20; i++)
		for(int j=0; j<10; j++)
			stock[i][j]=-1;
}

bool TetrisWnd::IsAtBottom()
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			if(shape.cells[i][j]==true)
			{
				if((curY/20+i)>=19 || stock[curY/20+i+1][curX/20+j]!=-1)
					return true;
			}
		}
	}
	return false;
}

void TetrisWnd::CheckForLine()
{
	int i,j,k,c=0;
	bool flag;
	
	for(i=0; i<20; i++)
	{
		flag=true;

		for(j=0; j<10; j++)
			flag &= (stock[i][j]!=-1);

		if(flag==true && j!=0)
		{
			c++;
			score++;
			ShowTitle();

			for(k=i; k>0; k--)
			{
				for(j=0; j<10; j++)
				{
					stock[k][j]=stock[k-1][j];
				}
			}
		}
	}
	for(k=0; k<c; k++)
		for(j=0; j<10; j++)
			stock[k][j]=-1;
	
	Invalidate();
}

void TetrisWnd::ShowTitle()
{
	CString title;
	title.Format("Tetris  score : %d",score);
	SetWindowText(title);
}

bool TetrisWnd::CheckRight()
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			if(shape.cells[i][j]==true)
			{
				if((curX/20+j)>=9 || stock[curY/20+i][curX/20+j+1]!=-1)
					return true;
			}
		}
	}
	return false;
}

bool TetrisWnd::CheckLeft()
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			if(shape.cells[i][j]==true)
			{
				if((curX/20+j)<=0 || stock[curY/20+i][curX/20+j-1]!=-1)
					return true;
			}
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// Menu commands
////////////////////////////////////////////////////////////////////////////////
void TetrisWnd::OnGameExit()
{
	OnClose();
}

void TetrisWnd::OnClose()
{
	KillTimer(1);	
	CFrameWnd::OnClose();
}

void TetrisWnd::OnLevelLevel1()
{
	SetLevel(1);	
}

void TetrisWnd::OnUpdateLevelLevel1(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(level1);	
}

void TetrisWnd::OnLevelLevel2()
{
	SetLevel(2);
}

void TetrisWnd::OnUpdateLevelLevel2(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(level2);
}

void TetrisWnd::OnLevelLevel3()
{
	SetLevel(3);
}

void TetrisWnd::OnUpdateLevelLevel3(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(level3);
}
/////////////////////////////////////////////////////////////////////////////////


void TetrisWnd::SetLevel(int lev)
{
	level1=level2=level3=false;

	switch(lev)
	{
	case 1: level=700; level1=true; break;
	case 2: level=500; level2=true; break;
	case 3: level=300; level3=true; break;
	}
}

void TetrisWnd::OnHelpAbout()
{
	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

COLORREF TetrisWnd::ShapeColor(int color)
{
	switch(color)
	{
	case 0: return RGB(255,0,0);
	case 1: return RGB(0,255,0);
	case 2: return RGB(0,0,255);
	case 3: return RGB(255,0,255);
	case 4: return RGB(0,255,255);
	case 5: return RGB(255,255,0);
	}
	return RGB(255,255,255);
}

void TetrisWnd::NewShape()
{
	KillTimer(1);
	SetTimer(1,level,NULL);
	curY=0;
	curX=60;
	ShapeType t=ShapeType(rand()%7);
	shape.NewShape(t);
	color=rand()%6;
}

