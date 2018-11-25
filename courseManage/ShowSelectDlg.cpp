// ShowSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "ShowSelectDlg.h"
#include "afxdialogex.h"


// CShowSelectDlg 对话框

IMPLEMENT_DYNAMIC(CShowSelectDlg, CDialogEx)

CShowSelectDlg::CShowSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOWSELECT_DLG, pParent)
{

}

CShowSelectDlg::CShowSelectDlg(LPVOID pStrArry, size_t size, int selIndex, CWnd * pParent) : CDialogEx(IDD_SHOWSELECT_DLG, pParent)
{
	m_pStrArry = pStrArry;
	m_size = size;
	m_selIndex = selIndex;
}

CShowSelectDlg::~CShowSelectDlg()
{
}

void CShowSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELECT, m_select);
}


BEGIN_MESSAGE_MAP(CShowSelectDlg, CDialogEx)
END_MESSAGE_MAP()


// CShowSelectDlg 消息处理程序


BOOL CShowSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString (*ary) [] = (CString (*)[])m_pStrArry;
	for (size_t i = 0; i < m_size; i++)
	{
		m_select.InsertString(i, (*ary)[i]);
	}
	m_select.SetCurSel(m_selIndex);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CShowSelectDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_pStrArry) {
		delete m_pStrArry;
	}
	return CDialogEx::DestroyWindow();
}
