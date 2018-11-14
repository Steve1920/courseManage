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
	CFont font; //在头文件中声明
	font.CreatePointFont(5, _T("宋体"));
	m_valueList.SetFont(&font, true);
	m_valueList.SetExtendedStyle(WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE |LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EDITLABELS);
	int count = m_valueList.GetItemCount();
	m_valueList.InsertColumn(count, (LPCTSTR)NULL);
	count = m_valueList.GetItemCount();
	//计算文件特征
	m_valueList.InsertItem(count, _T(" File is executable "));
	if ((m_cValue & IMAGE_FILE_EXECUTABLE_IMAGE) == IMAGE_FILE_EXECUTABLE_IMAGE) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" File is a DLL "));
	if ((m_cValue & IMAGE_FILE_DLL) == IMAGE_FILE_DLL) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" System File "));
	if ((m_cValue & IMAGE_FILE_SYSTEM) == IMAGE_FILE_SYSTEM) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" Relocation info stripped from file "));
	if ((m_cValue & IMAGE_FILE_RELOCS_STRIPPED) == IMAGE_FILE_RELOCS_STRIPPED) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" Line nunbers stripped from file "));
	if ((m_cValue & IMAGE_FILE_LINE_NUMS_STRIPPED) == IMAGE_FILE_LINE_NUMS_STRIPPED) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" Local symbols stripped from file "));
	if ((m_cValue & IMAGE_FILE_LOCAL_SYMS_STRIPPED) == IMAGE_FILE_LOCAL_SYMS_STRIPPED) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" Aggressively trim working set "));
	if ((m_cValue & IMAGE_FILE_AGGRESIVE_WS_TRIM) == IMAGE_FILE_AGGRESIVE_WS_TRIM) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" App can handle >2gb addresses "));
	if ((m_cValue & IMAGE_FILE_LARGE_ADDRESS_AWARE) == IMAGE_FILE_LARGE_ADDRESS_AWARE) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" Bytes of machine word are reversed "));
	if ((m_cValue & IMAGE_FILE_BYTES_REVERSED_LO) == IMAGE_FILE_BYTES_REVERSED_LO) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" 32 bit word machine "));
	if ((m_cValue & IMAGE_FILE_32BIT_MACHINE) == IMAGE_FILE_32BIT_MACHINE) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" Debugging info stripped from file in .DBG file "));
	if ((m_cValue & IMAGE_FILE_DEBUG_STRIPPED) == IMAGE_FILE_DEBUG_STRIPPED) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" If Image is on removable media, copy and run from the swap file "));
	if ((m_cValue & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP) == IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" If Image is on Net, copy and run from the swap file "));
	if ((m_cValue & IMAGE_FILE_NET_RUN_FROM_SWAP) == IMAGE_FILE_NET_RUN_FROM_SWAP) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" File should only be run on a UP machine "));
	if ((m_cValue & IMAGE_FILE_UP_SYSTEM_ONLY) == IMAGE_FILE_UP_SYSTEM_ONLY) m_valueList.SetCheck(count, true);
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T(" Bytes of machine word are reversed "));
	if ((m_cValue & IMAGE_FILE_BYTES_REVERSED_HI) == IMAGE_FILE_BYTES_REVERSED_HI) m_valueList.SetCheck(count, true);
	m_valueList.SetColumnWidth(0, 312);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
