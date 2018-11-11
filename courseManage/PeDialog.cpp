// PeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "PeDialog.h"
#include "afxdialogex.h"


// CPeDialog 对话框

IMPLEMENT_DYNAMIC(CPeDialog, CDialogEx)

CPeDialog::CPeDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PEDIALOG, pParent)
	, m_exeFullPath(_T(""))
{

}

CPeDialog::CPeDialog(CString & exePath, CWnd * pParent):CDialogEx(IDD_PEDIALOG, pParent)
{
	m_exeFullPath = exePath;
}

CPeDialog::~CPeDialog()
{
}

void CPeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_HEADERS, m_tree);
	DDX_Control(pDX, IDC_PEMAINPAGE, m_gridCtrl);
	DDX_Control(pDX, IDC_MAIN_DOWN, m_gridCtrlDown);
	DDX_Control(pDX, IDC_DOS_HEADER_CTRL, m_dosHeaderCtrl);
	DDX_Control(pDX, IDC_NT_HEADERS_CTRL, m_ntHeadersCtrl);
	DDX_Control(pDX, IDC_FILE_HEADER_CTRL, m_fileHeaderCtrl);
	DDX_Control(pDX, IDC_OPTIONAL_HEADER_CTRL, m_optionalHeaderCtrl);
	DDX_Control(pDX, IDC_DATA_DIRECTORIES_CTRL, m_dataDirectoriesCtrl);
	DDX_Control(pDX, IDC_SECTION_HEADER_CTRL, m_sectionHeadersCtrl);
}


BEGIN_MESSAGE_MAP(CPeDialog, CDialogEx)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_HEADERS, &CPeDialog::OnSelchangedTreeHeaders)
//ON_NOTIFY(NM_CLICK, IDC_TREE_HEADERS, &CPeDialog::OnNMClickTreeHeaders)
ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_HEADERS, &CPeDialog::OnTvnSelchangedTreeHeaders)
END_MESSAGE_MAP()


// CPeDialog 消息处理程序


BOOL CPeDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int pathLength = m_exeFullPath.GetLength();
	int position = m_exeFullPath.ReverseFind('/') + 1;
	CString exeName = m_exeFullPath.Mid(position);
	SetWindowText(_T(" ") + exeName + _T(" PE结构"));
	m_hRoot = m_tree.InsertItem(_T("File:") + exeName);
	m_dosHeader = m_tree.InsertItem(_T("Dos Header"), m_hRoot);
	m_ntHeaders = m_tree.InsertItem(_T("Nt Headers"), m_hRoot);
	m_fileHeader = m_tree.InsertItem(_T("File Header"), m_ntHeaders);
	m_optionalHeader = m_tree.InsertItem(_T("Optional Header"), m_ntHeaders);
	m_dataDirectories = m_tree.InsertItem(_T("Data Directories [x]"), m_optionalHeader);
	m_sectionHeaders = m_tree.InsertItem(_T("Section Headers [x]"), m_hRoot);
	m_tree.Expand(m_hRoot, TVE_EXPAND);
	m_tree.Expand(m_ntHeaders, TVE_EXPAND);
	m_tree.Expand(m_optionalHeader, TVE_EXPAND);
	GridCtrlInit_up();
	GridCtrlInit_down();
	m_peDetailPages[DOS_HEADER] = &m_dosHeaderCtrl;
	m_peDetailPages[NT_HEADER] = &m_ntHeadersCtrl;
	m_peDetailPages[FILE_HEADER] = &m_fileHeaderCtrl;
	m_peDetailPages[OPTIONAL_HEADER] = &m_optionalHeaderCtrl;
	m_peDetailPages[DATA_DIRECTORY] = &m_dataDirectoriesCtrl;
	m_peDetailPages[SECTION_HEADER] = &m_sectionHeadersCtrl;
	for (size_t i = 0; i < 6; i++)
	{
		m_peDetailPages[i]->ShowWindow(FALSE);
	}
	//解析PE结构
	parsePe();
	m_gridCtrl.ShowWindow(TRUE);
	m_gridCtrlDown.ShowWindow(TRUE);
	currentSel = -1;
	UpdateData(FALSE);
	return TRUE;
}
void makeItemToCtrl(CGridCtrl &gridCtrl,int row,int col,CString content) {
	GV_ITEM item;
	item.mask = GVIF_TEXT | GVIF_FORMAT;
	item.row = row;
	item.col = col;
	item.nFormat = DT_CENTER | DT_WORDBREAK | DT_VCENTER;
	item.strText.Format(content, 0);
	gridCtrl.SetItem(&item);
}
void CPeDialog::GridCtrlInit_up()
{
	m_gridCtrl.SetEditable(false);
	m_gridCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景
	m_gridCtrl.SetRowCount(10); //初始为10行
	m_gridCtrl.SetColumnCount(2); //初始化为11列
	m_gridCtrl.SetFixedRowCount(1); //固定行为一行
	m_gridCtrl.SetFixedColumnCount(1); //固定列为一列
	makeItemToCtrl(m_gridCtrl, 0, 0, _T("Property"));
	makeItemToCtrl(m_gridCtrl, 1, 0, _T("File Name"));
	makeItemToCtrl(m_gridCtrl, 2, 0, _T("File Info"));
	makeItemToCtrl(m_gridCtrl, 3, 0, _T("File Size"));
	makeItemToCtrl(m_gridCtrl, 4, 0, _T("PE Size"));
	makeItemToCtrl(m_gridCtrl, 5, 0, _T("Created"));
	makeItemToCtrl(m_gridCtrl, 6, 0, _T("Modified"));
	makeItemToCtrl(m_gridCtrl, 7, 0, _T("Accessed"));
	makeItemToCtrl(m_gridCtrl, 8, 0, _T("MD5"));
	makeItemToCtrl(m_gridCtrl, 9, 0, _T("SHA-1"));
	makeItemToCtrl(m_gridCtrl, 0, 1, _T("Value"));
	for (int row = 0; row < m_gridCtrl.GetRowCount(); row++) {
		for (int col = 0; col < m_gridCtrl.GetColumnCount(); col++)
		{
			m_gridCtrl.SetRowHeight(row, 25); //设置各行高          
			if (col == 1) {
				m_gridCtrl.SetColumnWidth(col, 300); //设置各列宽
			}
			else {
				m_gridCtrl.SetColumnWidth(col, 100); //设置各列宽
			}
		}
	}
}

void CPeDialog::GridCtrlInit_down()
{
	m_gridCtrlDown.SetEditable(true);
	m_gridCtrlDown.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景
	m_gridCtrlDown.SetRowCount(8); //初始为10行
	m_gridCtrlDown.SetColumnCount(2); //初始化为11列
	m_gridCtrlDown.SetFixedRowCount(1);  //固定列为一列
	makeItemToCtrl(m_gridCtrlDown, 0, 0, _T("Property"));
	makeItemToCtrl(m_gridCtrlDown, 1, 0, _T("CompanyName"));
	makeItemToCtrl(m_gridCtrlDown, 2, 0, _T("FileDescription"));
	makeItemToCtrl(m_gridCtrlDown, 3, 0, _T("FileVersion"));
	makeItemToCtrl(m_gridCtrlDown, 4, 0, _T("InternalName"));
	makeItemToCtrl(m_gridCtrlDown, 5, 0, _T("LegalCopyright"));
	makeItemToCtrl(m_gridCtrlDown, 6, 0, _T("OriginalFilename"));
	makeItemToCtrl(m_gridCtrlDown, 7, 0, _T("ProductName"));
	makeItemToCtrl(m_gridCtrlDown, 0, 1, _T("Value"));
	for (int row = 0; row < m_gridCtrlDown.GetRowCount(); row++) {
		for (int col = 0; col < m_gridCtrlDown.GetColumnCount(); col++)
		{
			m_gridCtrlDown.SetRowHeight(row, 25); //设置各行高          
			if (col == 1) {
				m_gridCtrlDown.SetColumnWidth(col, 300); //设置各列宽
			}
			else {
				m_gridCtrlDown.SetColumnWidth(col, 100); //设置各列宽
			}
		}
	}
}

void CPeDialog::hidePrevSel()
{
	if (currentSel == -1) {
		m_gridCtrl.ShowWindow(FALSE);
		m_gridCtrlDown.ShowWindow(FALSE);
	}
	else {
		m_peDetailPages[currentSel]->ShowWindow(FALSE);
	}
}



void CPeDialog::OnTvnSelchangedTreeHeaders(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM itemSel =  m_tree.GetSelectedItem();
	hidePrevSel();
	if (itemSel == m_hRoot) {
		m_gridCtrl.ShowWindow(TRUE);
		m_gridCtrlDown.ShowWindow(TRUE);
		currentSel = -1;
	}
	else if (itemSel == m_dosHeader) {
		m_peDetailPages[DOS_HEADER]->ShowWindow(TRUE);
		currentSel = DOS_HEADER;
	}
	else if (itemSel == m_ntHeaders) {
		m_peDetailPages[NT_HEADER]->ShowWindow(TRUE);
		currentSel = NT_HEADER;
	}
	else if (itemSel == m_fileHeader) {
		m_peDetailPages[FILE_HEADER]->ShowWindow(TRUE);
		currentSel = FILE_HEADER;
	}
	else if (itemSel == m_optionalHeader) {
		m_peDetailPages[OPTIONAL_HEADER]->ShowWindow(TRUE);
		currentSel = OPTIONAL_HEADER;
	}
	else if (itemSel == m_dataDirectories) {
		m_peDetailPages[DATA_DIRECTORY]->ShowWindow(TRUE);
		currentSel = DATA_DIRECTORY;
	}
	else if (itemSel == m_sectionHeaders) {
		m_peDetailPages[SECTION_HEADER]->ShowWindow(TRUE);
		currentSel = SECTION_HEADER;
	}
	*pResult = 0;
}

void CPeDialog::parsePe()
{
	int result = validPeVerify();
	if (result == 1) {
		parseDosHeader();
	}else if(result == 0){
		MessageBox(_T("非PE格式文件，无法解析"), _T("消息框"), MB_OK);
	}
}

void CPeDialog::parseDosHeader()
{
	m_dosHeaderCtrl.SetEditable(true);
	m_dosHeaderCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景
	m_dosHeaderCtrl.SetRowCount(32); 
	m_dosHeaderCtrl.SetColumnCount(4); 
	m_dosHeaderCtrl.SetFixedRowCount(1);  
	m_dosHeaderCtrl.SetFixedColumnCount(3);
	makeItemToCtrl(m_dosHeaderCtrl, 0, 0, _T("Member"));
	makeItemToCtrl(m_dosHeaderCtrl, 0, 1, _T("Offset"));
	makeItemToCtrl(m_dosHeaderCtrl, 0, 2, _T("Size"));
	makeItemToCtrl(m_dosHeaderCtrl, 0, 3, _T("Value"));
	for (int row = 0; row < m_dosHeaderCtrl.GetRowCount(); row++) {
		for (int col = 0; col < m_dosHeaderCtrl.GetColumnCount(); col++)
		{
			m_dosHeaderCtrl.SetRowHeight(row, 25); //设置各行高          
			m_dosHeaderCtrl.SetColumnWidth(col, 100); //设置各列宽
		}
	}
	CFile file;
	BOOL result = file.Open(m_exeFullPath, CFile::modeRead);
	int offsetTmp = 0;
	if (result) {
		int peResult = 0;
		IMAGE_DOS_HEADER dosHeader;
		file.Read(&dosHeader, sizeof(dosHeader));
		makeItemToCtrl(m_dosHeaderCtrl, 1, 0, _T("e_magic"));
		CString offsetStr;
		offsetStr.Format(_T("%08X"), offsetTmp);
		makeItemToCtrl(m_dosHeaderCtrl, 1, 1, offsetStr);
		makeItemToCtrl(m_dosHeaderCtrl, 2, 0, _T("e_cblp"));
		file.Close();
		return;
	}
	else {
		MessageBox(_T("打开文件失败"), _T("消息框"), MB_OK);
		return;
	}
}
//-1 打开文件失败 0 非pe文件 1 是合法PE
int CPeDialog::validPeVerify()
{
	CFile file;
	BOOL result = file.Open(m_exeFullPath, CFile::modeRead);
	if (result) {
		int peResult = 0;
		IMAGE_DOS_HEADER dosHeader;
		file.Read(&dosHeader,sizeof(dosHeader));
		file.Seek(dosHeader.e_lfanew,CFile::begin);
		int pesig = 0;
		file.Read(&pesig, 4);
		if (pesig == IMAGE_NT_SIGNATURE && dosHeader.e_magic == IMAGE_DOS_SIGNATURE) {
			peResult = 1;
		}
		file.Close();
		return peResult;
	}
	else {
		MessageBox(_T("打开文件失败"), _T("消息框"), MB_OK);
		return -1;
	}
}
