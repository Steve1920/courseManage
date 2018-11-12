// Characteristics.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "Characteristics.h"
#include "afxdialogex.h"


// CCharacteristics 对话框

IMPLEMENT_DYNAMIC(CCharacteristics, CDialogEx)

CCharacteristics::CCharacteristics(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHARACTERISTICS, pParent)
{

}

CCharacteristics::CCharacteristics(int value, CWnd * pParent) : CDialogEx(IDD_CHARACTERISTICS, pParent)
{
	m_cValue = value;
}

CCharacteristics::~CCharacteristics()
{
}

void CCharacteristics::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_C_LIST, m_valueList);
}


BEGIN_MESSAGE_MAP(CCharacteristics, CDialogEx)
END_MESSAGE_MAP()


// CCharacteristics 消息处理程序


BOOL CCharacteristics::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_valueList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EDITLABELS);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
