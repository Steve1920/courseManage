// ShowCheckboxList.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "ShowCheckboxList.h"
#include "afxdialogex.h"


// CShowCheckboxList 对话框

IMPLEMENT_DYNAMIC(CShowCheckboxList, CDialogEx)

CShowCheckboxList::CShowCheckboxList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLLCHARACTERISTICS, pParent)
{

}

CShowCheckboxList::CShowCheckboxList(vector<CString> &strVec, vector<int> &selVec, CString textName, CWnd * pParent)
	: CDialogEx(IDD_DLLCHARACTERISTICS, pParent)
{
	m_strVec = strVec;
	m_selVec = selVec;
	m_textName = textName;
}

CShowCheckboxList::~CShowCheckboxList()
{
}

void CShowCheckboxList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKBOXLIST, m_checkboxListCtrl);
}


BEGIN_MESSAGE_MAP(CShowCheckboxList, CDialogEx)
END_MESSAGE_MAP()


// CShowCheckboxList 消息处理程序


BOOL CShowCheckboxList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CFont font; //在头文件中声明
	//font.CreatePointFont(1, _T("宋体"));
	font.CreateFont(0,0,0,0, FW_DONTCARE,0,0,0, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,NULL);
	m_checkboxListCtrl.SetFont(&font);
	m_checkboxListCtrl.SetExtendedStyle(WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EDITLABELS);
	int count = m_checkboxListCtrl.GetItemCount();
	m_checkboxListCtrl.InsertColumn(count, (LPCTSTR)NULL);
	vector<CString>::iterator iterStr;
	vector<int>::iterator iterSel;
	for (iterStr = m_strVec.begin(); iterStr != m_strVec.end(); iterStr++)
	{
		count = m_checkboxListCtrl.GetItemCount();
		m_checkboxListCtrl.InsertItem(count, *iterStr);
		iterSel = find(m_selVec.begin(), m_selVec.end(), count + 1);
		if (iterSel != m_selVec.end()) {
			m_checkboxListCtrl.SetCheck(count, true);
		}
	}
	SetWindowText(m_textName);
	m_checkboxListCtrl.SetColumnWidth(0, 312);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
