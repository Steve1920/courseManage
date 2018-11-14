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
ON_NOTIFY(NM_CLICK, IDC_FILE_HEADER_CTRL, &CPeDialog::OnCharacteristicsClick)
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
		m_peDetailPages[i]->AutoSize();
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

void CPeDialog::OnCharacteristicsClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	int row = pItem->iRow;
	int column = pItem->iColumn;
	if (row == 7 && column == 4) {
		CString value = m_fileHeaderCtrl.GetItemText(7, 3);
		value = _T("0x") + value;
		int valInt = 0;
		StrToIntEx(value, STIF_SUPPORT_HEX, &valInt);
		CCharacteristics cDialog(valInt);
		cDialog.DoModal();
	}
}

void CPeDialog::parsePe()
{
	int result = validPeVerify();
	if (result == 1) {
		int ntOffset = parseDosHeader();
		parseNtHeader(ntOffset);
	}else if(result == 0){
		MessageBox(_T("非PE格式文件，无法解析"), _T("消息框"), MB_OK);
	}
}

void createDosHeaderLine(int row,CString name,int offset,CString &sizeStr,LONG value,CGridCtrl &dosHeaderCtrl) {
	makeItemToCtrl(dosHeaderCtrl, row, 0, name);
	CString offsetStrMagic;
	offsetStrMagic.Format(_T("%08X"), offset);
	makeItemToCtrl(dosHeaderCtrl, row, 1, offsetStrMagic);
	makeItemToCtrl(dosHeaderCtrl, row, 2, sizeStr);
	CString magicStr;
	if (name == _T("e_lfanew") || name == _T("Signature")) {
		magicStr.Format(_T("%08X"), value);
	}
	else {
		magicStr.Format(_T("%04X"), value);
	}
	makeItemToCtrl(dosHeaderCtrl, row, 3, magicStr);
}
int CPeDialog::parseDosHeader()
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
		CString sizeStrAry[4] = {_T("Byte"),_T("Word"),_T("") ,_T("Dword") };
		int peResult = 0;
		IMAGE_DOS_HEADER dosHeader;
		file.Read(&dosHeader, sizeof(dosHeader));
		createDosHeaderLine(1, _T("e_magic"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_magic) - 1], dosHeader.e_magic, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_magic);
		createDosHeaderLine(2, _T("e_cblp"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_cblp) - 1], dosHeader.e_cblp, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_cblp);
		createDosHeaderLine(3, _T("e_cp"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_cp) - 1], dosHeader.e_cp, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_cp);
		createDosHeaderLine(4, _T("e_crlc"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_crlc) - 1], dosHeader.e_crlc, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_crlc);
		createDosHeaderLine(5, _T("e_cparhdr"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_cparhdr) - 1], dosHeader.e_cparhdr, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_cparhdr);
		createDosHeaderLine(6, _T("e_minalloc"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_minalloc) - 1], dosHeader.e_minalloc, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_minalloc);
		createDosHeaderLine(7, _T("e_maxalloc"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_maxalloc) - 1], dosHeader.e_maxalloc, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_maxalloc);
		createDosHeaderLine(8, _T("e_ss"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_ss) - 1], dosHeader.e_ss, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_ss);
		createDosHeaderLine(9, _T("e_sp"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_sp) - 1], dosHeader.e_sp, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_sp);
		createDosHeaderLine(10, _T("e_csum"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_csum) - 1], dosHeader.e_csum, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_csum);
		createDosHeaderLine(11, _T("e_ip"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_ip) - 1], dosHeader.e_ip, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_ip);
		createDosHeaderLine(12, _T("e_cs"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_cs) - 1], dosHeader.e_cs, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_cs);
		createDosHeaderLine(13, _T("e_lfarlc"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_lfarlc) - 1], dosHeader.e_lfarlc, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_lfarlc);
		createDosHeaderLine(14, _T("e_ovno"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_ovno) - 1], dosHeader.e_ovno, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_ovno);
		createDosHeaderLine(15, _T("e_res"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res[0]) - 1], dosHeader.e_res[0], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res[0]);
		createDosHeaderLine(16, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res[1]) - 1], dosHeader.e_res[1], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res[1]);
		createDosHeaderLine(17, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res[2]) - 1], dosHeader.e_res[2], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res[2]);
		createDosHeaderLine(18, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res[3]) - 1], dosHeader.e_res[3], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res[3]);
		createDosHeaderLine(19, _T("e_oemid"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_oemid) - 1], dosHeader.e_oemid, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_oemid);
		createDosHeaderLine(20, _T("e_oeminfo"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_oeminfo) - 1], dosHeader.e_oeminfo, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_oeminfo);
		createDosHeaderLine(21, _T("e_res2"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[0]) - 1], dosHeader.e_res2[0], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[0]);
		createDosHeaderLine(22, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[1]) - 1], dosHeader.e_res2[1], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[1]);
		createDosHeaderLine(23, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[2]) - 1], dosHeader.e_res2[2], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[2]);
		createDosHeaderLine(24, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[3]) - 1], dosHeader.e_res2[3], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[3]);
		createDosHeaderLine(25, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[4]) - 1], dosHeader.e_res2[4], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[4]);
		createDosHeaderLine(26, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[5]) - 1], dosHeader.e_res2[5], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[5]);
		createDosHeaderLine(27, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[6]) - 1], dosHeader.e_res2[6], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[6]);
		createDosHeaderLine(28, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[7]) - 1], dosHeader.e_res2[7], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[7]);
		createDosHeaderLine(29, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[8]) - 1], dosHeader.e_res2[8], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[8]);
		createDosHeaderLine(30, _T(""), offsetTmp, sizeStrAry[sizeof(dosHeader.e_res2[9]) - 1], dosHeader.e_res2[9], m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_res2[9]);
		createDosHeaderLine(31, _T("e_lfanew"), offsetTmp, sizeStrAry[sizeof(dosHeader.e_lfanew) - 1], dosHeader.e_lfanew, m_dosHeaderCtrl);
		offsetTmp += sizeof(dosHeader.e_lfanew);
		file.Close();
		return dosHeader.e_lfanew;
	}
	else {
		MessageBox(_T("打开文件失败"), _T("消息框"), MB_OK);
		return 0;
	}
}
void createfileHeaderLine(int row, CString name, int offset, CString &sizeStr, LONG value, CGridCtrl &dosHeaderCtrl) {
	makeItemToCtrl(dosHeaderCtrl, row, 0, name);
	CString offsetStrMagic;
	offsetStrMagic.Format(_T("%08X"), offset);
	makeItemToCtrl(dosHeaderCtrl, row, 1, offsetStrMagic);
	makeItemToCtrl(dosHeaderCtrl, row, 2, sizeStr);
	CString magicStr;
	if (row == 3 || row == 4 || row == 5) {
		magicStr.Format(_T("%08X"), value);
	}
	else {
		magicStr.Format(_T("%04X"), value);
	}
	makeItemToCtrl(dosHeaderCtrl, row, 3, magicStr);
	CString meaningStr;
	if (row == 1) {
		switch (value)
		{
		case IMAGE_FILE_MACHINE_UNKNOWN:
			meaningStr = _T("unknown value");
			break;
		case IMAGE_FILE_MACHINE_I386:
			meaningStr = _T("Intel 386.");
			break;
		case IMAGE_FILE_MACHINE_R3000:
			meaningStr = _T("MIPS little-endian, 0x160 big-endian");
			break;
		case IMAGE_FILE_MACHINE_R4000:
			meaningStr = _T("MIPS little-endian");
			break;
		case IMAGE_FILE_MACHINE_R10000:
			meaningStr = _T("MIPS little-endian");
			break;
		case IMAGE_FILE_MACHINE_WCEMIPSV2:
			meaningStr = _T("MIPS little-endian WCE v2");
			break;
		case IMAGE_FILE_MACHINE_ALPHA:
			meaningStr = _T("Alpha_AXP");
			break;
		case IMAGE_FILE_MACHINE_SH3:
			meaningStr = _T("SH3 little-endian");
			break;
		case IMAGE_FILE_MACHINE_SH3DSP:
			meaningStr = _T("");
			break;
		case IMAGE_FILE_MACHINE_SH3E:
			meaningStr = _T("SH3E little-endian");
			break;
		case IMAGE_FILE_MACHINE_SH4:
			meaningStr = _T("SH4 little-endian");
			break;
		case IMAGE_FILE_MACHINE_SH5:
			meaningStr = _T("SH5");
			break;
		case IMAGE_FILE_MACHINE_ARM:
			meaningStr = _T("ARM Little-Endian");
			break;
		case IMAGE_FILE_MACHINE_THUMB:
			meaningStr = _T("ARM Thumb/Thumb-2 Little-Endian");
			break;
		case IMAGE_FILE_MACHINE_ARMNT:
			meaningStr = _T("ARM Thumb-2 Little-Endian");
			break;
		case IMAGE_FILE_MACHINE_AM33:
			meaningStr = _T("");
			break;
		case IMAGE_FILE_MACHINE_POWERPC:
			meaningStr = _T("IBM PowerPC Little-Endian");
			break;
		case IMAGE_FILE_MACHINE_POWERPCFP:
			meaningStr = _T("");
			break;
		case IMAGE_FILE_MACHINE_IA64:
			meaningStr = _T("Intel 64");
			break;
		case IMAGE_FILE_MACHINE_MIPS16:
			meaningStr = _T("MIPS");
			break;
		case IMAGE_FILE_MACHINE_ALPHA64:
			meaningStr = _T("ALPHA64");
			break;
		case IMAGE_FILE_MACHINE_MIPSFPU:
			meaningStr = _T("MIPS");
			break;
		case IMAGE_FILE_MACHINE_MIPSFPU16:
			meaningStr = _T("MIPS");
			break;
		case IMAGE_FILE_MACHINE_TRICORE:
			meaningStr = _T("Infineon");
			break;
		case IMAGE_FILE_MACHINE_CEF:
			meaningStr = _T("");
			break;
		case IMAGE_FILE_MACHINE_EBC:
			meaningStr = _T("EFI Byte Code");
			break;
		case IMAGE_FILE_MACHINE_AMD64:
			meaningStr = _T("AMD64 (K8)");
			break;
		case IMAGE_FILE_MACHINE_M32R:
			meaningStr = _T("M32R little-endian");
			break;
		case IMAGE_FILE_MACHINE_CEE:
			meaningStr = _T("");
			break;
		default:
			meaningStr = _T("");
			break;
		}
	}
	else if(row == 7){
		meaningStr = _T("Click here");
	}
	else {
		meaningStr = _T("");
	}
	makeItemToCtrl(dosHeaderCtrl, row, 4, meaningStr);
}
void CPeDialog::parseNtHeader(int ntOffset)
{
	m_ntHeadersCtrl.SetEditable(true);
	m_ntHeadersCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景
	m_ntHeadersCtrl.SetRowCount(2);
	m_ntHeadersCtrl.SetColumnCount(4);
	m_ntHeadersCtrl.SetFixedRowCount(1);
	m_ntHeadersCtrl.SetFixedColumnCount(3);
	makeItemToCtrl(m_ntHeadersCtrl, 0, 0, _T("Member"));
	makeItemToCtrl(m_ntHeadersCtrl, 0, 1, _T("Offset"));
	makeItemToCtrl(m_ntHeadersCtrl, 0, 2, _T("Size"));
	makeItemToCtrl(m_ntHeadersCtrl, 0, 3, _T("Value"));
	for (int row = 0; row < m_ntHeadersCtrl.GetRowCount(); row++) {
		for (int col = 0; col < m_ntHeadersCtrl.GetColumnCount(); col++)
		{
			m_ntHeadersCtrl.SetRowHeight(row, 25); //设置各行高          
			m_ntHeadersCtrl.SetColumnWidth(col, 100); //设置各列宽
		}
	}
	CFile file;
	BOOL result = file.Open(m_exeFullPath, CFile::modeRead);
	int offsetTmp = ntOffset;
	if (result) {
		CString sizeStrAry[4] = { _T("Byte"),_T("Word"),_T("") ,_T("Dword") };
		file.Seek(ntOffset, CFile::begin);
		DWORD signature;
		IMAGE_FILE_HEADER fileHeader;
		file.Read(&signature, sizeof(signature));
		createDosHeaderLine(1, _T("Signature"), ntOffset, sizeStrAry[sizeof(signature) - 1], signature, m_ntHeadersCtrl);
		offsetTmp += sizeof(signature);
		//parse file header
		file.Read(&fileHeader, sizeof(fileHeader));
		m_fileHeaderCtrl.SetEditable(true);
		m_fileHeaderCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景
		m_fileHeaderCtrl.SetRowCount(8);
		m_fileHeaderCtrl.SetColumnCount(5);
		m_fileHeaderCtrl.SetFixedRowCount(1);
		m_fileHeaderCtrl.SetFixedColumnCount(3);
		makeItemToCtrl(m_fileHeaderCtrl, 0, 0, _T("Member"));
		makeItemToCtrl(m_fileHeaderCtrl, 0, 1, _T("Offset"));
		makeItemToCtrl(m_fileHeaderCtrl, 0, 2, _T("Size"));
		makeItemToCtrl(m_fileHeaderCtrl, 0, 3, _T("Value"));
		makeItemToCtrl(m_fileHeaderCtrl, 0, 4, _T("Meaning"));
		for (int row = 0; row < m_fileHeaderCtrl.GetRowCount(); row++) {
			for (int col = 0; col < m_fileHeaderCtrl.GetColumnCount(); col++)
			{
				m_fileHeaderCtrl.SetRowHeight(row, 25); //设置各行高  
				if (col == 0) {
					m_fileHeaderCtrl.SetColumnWidth(col, 170); //设置各列宽
				}
				else {
					m_fileHeaderCtrl.SetColumnWidth(col, 100); //设置各列宽
				}
				if (col == m_fileHeaderCtrl.GetColumnCount() - 1) {
					m_fileHeaderCtrl.SetItemState(row, col, m_fileHeaderCtrl.GetItemState(row, col) | GVIS_READONLY);
				}
			}
		}
		m_fileHeaderCtrl.SetItemBkColour(1, 3, RGB(174, 197, 232));
		m_fileHeaderCtrl.SetItemBkColour(1, 4, RGB(174, 197, 232));
		m_fileHeaderCtrl.SetItemBkColour(7, 3, RGB(174, 197, 232));
		m_fileHeaderCtrl.SetItemBkColour(7, 4, RGB(174, 197, 232));
		createfileHeaderLine(1, _T("Machine"), offsetTmp, sizeStrAry[sizeof(fileHeader.Machine) - 1], fileHeader.Machine, m_fileHeaderCtrl);
		offsetTmp += sizeof(fileHeader.Machine);
		createfileHeaderLine(2, _T("NumberOfSections"), offsetTmp, sizeStrAry[sizeof(fileHeader.NumberOfSections) - 1], fileHeader.NumberOfSections, m_fileHeaderCtrl);
		offsetTmp += sizeof(fileHeader.NumberOfSections);
		createfileHeaderLine(3, _T("TimeDateStamp"), offsetTmp, sizeStrAry[sizeof(fileHeader.TimeDateStamp) - 1], fileHeader.TimeDateStamp, m_fileHeaderCtrl);
		offsetTmp += sizeof(fileHeader.TimeDateStamp);
		createfileHeaderLine(4, _T("PointerToSymbolTable"), offsetTmp, sizeStrAry[sizeof(fileHeader.PointerToSymbolTable) - 1], fileHeader.PointerToSymbolTable, m_fileHeaderCtrl);
		offsetTmp += sizeof(fileHeader.PointerToSymbolTable);
		createfileHeaderLine(5, _T("NumberOfSymbols"), offsetTmp, sizeStrAry[sizeof(fileHeader.NumberOfSymbols) - 1], fileHeader.NumberOfSymbols, m_fileHeaderCtrl);
		offsetTmp += sizeof(fileHeader.NumberOfSymbols);
		createfileHeaderLine(6, _T("SizeOfOptionalHeader"), offsetTmp, sizeStrAry[sizeof(fileHeader.SizeOfOptionalHeader) - 1], fileHeader.SizeOfOptionalHeader, m_fileHeaderCtrl);
		offsetTmp += sizeof(fileHeader.SizeOfOptionalHeader);
		createfileHeaderLine(7, _T("Characteristics"), offsetTmp, sizeStrAry[sizeof(fileHeader.Characteristics) - 1], fileHeader.Characteristics, m_fileHeaderCtrl);
		offsetTmp += sizeof(fileHeader.Characteristics);
		if (fileHeader.Machine == IMAGE_FILE_MACHINE_IA64
			|| fileHeader.Machine == IMAGE_FILE_MACHINE_ALPHA64
			|| fileHeader.Machine == IMAGE_FILE_MACHINE_AMD64) {
			IMAGE_OPTIONAL_HEADER64 image64;
			file.Read(&image64, sizeof(fileHeader.SizeOfOptionalHeader));
		}
		else {
			IMAGE_OPTIONAL_HEADER32 image32;
			file.Read(&image32, sizeof(fileHeader.SizeOfOptionalHeader));
		}
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
