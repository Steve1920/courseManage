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
ON_NOTIFY(NM_CLICK, IDC_OPTIONAL_HEADER_CTRL, &CPeDialog::OnSubsystemClick)
ON_NOTIFY(NM_CLICK, IDC_SECTION_HEADER_CTRL, &CPeDialog::OnSectionCtrlClick)
ON_WM_NCDESTROY()
ON_WM_SIZE()
ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()
int sectionHeaderOffset = 0;
void CPeDialog::OnSectionCtrlClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	int tmpOffset = sectionHeaderOffset;
	IMAGE_SECTION_HEADER tmpSection;
	int lineSize = sizeof(IMAGE_SECTION_HEADER);
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	NMHDR hdr = pItem->hdr;
	int row = pItem->iRow;
	if (row < 3) {
		return;
	}
	int column = pItem->iColumn;
	tmpOffset += (row - 3) * lineSize;
	CString nameStr;
	nameStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 0, nameStr);
	tmpOffset += sizeof(tmpSection.Name);
	CString virtualSizeStr;
	virtualSizeStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 1, virtualSizeStr);
	tmpOffset += sizeof(tmpSection.Misc);
	CString virtualAddressStr;
	virtualAddressStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 2, virtualAddressStr);
	tmpOffset += sizeof(tmpSection.VirtualAddress);
	CString rawSizeStr;
	rawSizeStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 3, rawSizeStr);
	tmpOffset += sizeof(tmpSection.SizeOfRawData);
	CString rawAddressStr;
	rawAddressStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 4, rawAddressStr);
	tmpOffset += sizeof(tmpSection.PointerToRawData);
	CString relocAddressStr;
	relocAddressStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 5, relocAddressStr);
	tmpOffset += sizeof(tmpSection.PointerToRelocations);
	CString lineNumbersStr;
	lineNumbersStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 6, lineNumbersStr);
	tmpOffset += sizeof(tmpSection.NumberOfLinenumbers);
	CString relocationsNumberStr;
	relocationsNumberStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 7, relocationsNumberStr);
	tmpOffset += sizeof(tmpSection.NumberOfRelocations);
	CString lineNumberNumbersStr;
	lineNumberNumbersStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 8, lineNumberNumbersStr);
	tmpOffset += sizeof(tmpSection.NumberOfLinenumbers);
	CString characteristicsStr;
	characteristicsStr.Format(_T("%08X"), tmpOffset);
	makeItemToCtrl(m_sectionHeadersCtrl, 1, 9, characteristicsStr);
	m_sectionHeadersCtrl.Refresh();
}
void CPeDialog::dynamicCalcItemSize(int width, int height)
{
	int itemHeight = height - 7;
	CRect treeRect;
	CWnd* tree = GetDlgItem(IDC_TREE_HEADERS);
	if (tree) {
		tree->GetWindowRect(&treeRect);
		m_tree.SetWindowPos(NULL, 0, 0, treeRect.Width(), itemHeight, SWP_NOZORDER | SWP_NOMOVE);
		CRect tmpRect;
		CWnd* mainUp = GetDlgItem(IDC_PEMAINPAGE);
		if (mainUp) {
			mainUp->GetWindowRect(&tmpRect);
			m_gridCtrl.SetWindowPos(NULL, 0, 0, width - treeRect.Width() - 6, tmpRect.Height(), SWP_NOZORDER | SWP_NOMOVE);
		}
		CWnd* mainDown = GetDlgItem(IDC_MAIN_DOWN);
		if (mainDown) {
			int downHeight = height - tmpRect.Height() - 9;
			mainDown->GetWindowRect(&tmpRect);
			m_gridCtrlDown.SetWindowPos(NULL, 0, 0, width - treeRect.Width() - 7, downHeight, SWP_NOZORDER | SWP_NOMOVE);
		}
		m_dosHeaderCtrl.SetWindowPos(NULL, 0, 0, width - treeRect.Width() - 7, itemHeight, SWP_NOZORDER | SWP_NOMOVE);
		m_ntHeadersCtrl.SetWindowPos(NULL, 0, 0, width - treeRect.Width() - 7, itemHeight, SWP_NOZORDER | SWP_NOMOVE);
		m_fileHeaderCtrl.SetWindowPos(NULL, 0, 0, width - treeRect.Width() - 7, itemHeight, SWP_NOZORDER | SWP_NOMOVE);
		m_optionalHeaderCtrl.SetWindowPos(NULL, 0, 0, width - treeRect.Width() - 7, itemHeight, SWP_NOZORDER | SWP_NOMOVE);
		m_dataDirectoriesCtrl.SetWindowPos(NULL, 0, 0, width - treeRect.Width() - 7, itemHeight, SWP_NOZORDER | SWP_NOMOVE);
		m_sectionHeadersCtrl.SetWindowPos(NULL, 0, 0, width - treeRect.Width() - 7, itemHeight, SWP_NOZORDER | SWP_NOMOVE);
	}
}
// CPeDialog 消息处理程序
void CPeDialog::moveWindowCenterAndSetsize(int width, int height) {
	//计算各个组件的高、宽
	dynamicCalcItemSize(width, height);
	//再用MoveWindow
	MoveWindow(0, 0, width, height, TRUE);
	CenterWindow();
}
void CPeDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (cx <= 1 || cy <= 1)
		return;
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		CRect rect;
		GetClientRect(&rect);
		ScreenToClient(rect);
		dynamicCalcItemSize(rect.Width(),rect.Height());
		// TODO: 在此处添加消息处理程序代码
	}
}
BOOL CPeDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_sectionAry = NULL;
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
	//计算屏幕宽度，设置对话框长、宽各80%
	int windowX = GetSystemMetrics(SM_CXSCREEN);
	int windowY = GetSystemMetrics(SM_CYSCREEN);
	moveWindowCenterAndSetsize(windowX*0.8, windowY*0.8);
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
	makeItemToCtrl(m_gridCtrl, 1, 1, m_exeFullPath);
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
				m_gridCtrl.SetColumnWidth(col, 350); //设置各列宽
			}
			else {
				m_gridCtrl.SetColumnWidth(col, 100); //设置各列宽
			}
		}
	}
	CFileStatus fStatus;
	CFile::GetStatus(m_exeFullPath, fStatus);
	CString createTimeStr = fStatus.m_ctime.Format("%Y-%m-%d %H:%M:%S");
	CString lmTimeStr = fStatus.m_mtime.Format("%Y-%m-%d %H:%M:%S");
	CString laTimeStr = fStatus.m_atime.Format("%Y-%m-%d %H:%M:%S");
	makeItemToCtrl(m_gridCtrl, 5, 1, createTimeStr);
	makeItemToCtrl(m_gridCtrl, 6, 1, lmTimeStr);
	makeItemToCtrl(m_gridCtrl, 7, 1, laTimeStr);
}

BOOL CPeDialog::queryFileProperty(CString & queryStr,DWORD fileVersionSize, char * lpData, LPVOID *lplpBuffer)
{
	BOOL queryRst = GetFileVersionInfo(
		m_exeFullPath,
		0,
		fileVersionSize,
		lpData
	);
	if (queryRst) {
		UINT tmpSize = 0;
		struct LANGANDCODEPAGE {
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;
		queryRst = VerQueryValue(lpData,
			_T("\\VarFileInfo\\Translation"),
			(LPVOID*)&lpTranslate,
			&tmpSize);
		if (queryRst) {
			CString cs;
			cs.Format(queryStr, lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
			queryRst = VerQueryValue(lpData,
				cs,
				lplpBuffer,
				&tmpSize);
		}
	}
	return queryRst;
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
				m_gridCtrlDown.SetColumnWidth(col, 150); //设置各列宽
			}
		}
	}
	DWORD fileVersionSize = GetFileVersionInfoSize(m_exeFullPath, NULL);
	if (fileVersionSize) {
		BOOL queryRst = false;
		char *lpData = new char[fileVersionSize + 1];
		//block use
		LPVOID  *lplpBuffer = (LPVOID*)new char[200];
		CString queryStr = _T("\\StringFileInfo\\%04x%04x\\CompanyName");
		queryRst = queryFileProperty(queryStr, fileVersionSize, lpData,(LPVOID*)&lplpBuffer);
		CString showStr = (LPCTSTR)lplpBuffer;
		if (queryRst) {
			makeItemToCtrl(m_gridCtrlDown, 1, 1, showStr);
		}
		queryStr = _T("\\StringFileInfo\\%04x%04x\\FileDescription");
		queryRst = queryFileProperty(queryStr, fileVersionSize, lpData, (LPVOID*)&lplpBuffer);
		showStr = (LPCTSTR)lplpBuffer;
		if (showStr.IsEmpty()) {
			CString fileName = m_exeFullPath.Mid(m_exeFullPath.ReverseFind('/') + 1);
			showStr = fileName.Mid(0, fileName.ReverseFind('.'));
		}
		makeItemToCtrl(m_gridCtrlDown, 2, 1, showStr);
		queryStr = _T("\\StringFileInfo\\%04x%04x\\FileVersion");
		queryRst = queryFileProperty(queryStr, fileVersionSize, lpData, (LPVOID*)&lplpBuffer);
		showStr = (LPCTSTR)lplpBuffer;
		if (queryRst) {
			makeItemToCtrl(m_gridCtrlDown, 3, 1, showStr);
		}
		queryStr = _T("\\StringFileInfo\\%04x%04x\\InternalName");
		queryRst = queryFileProperty(queryStr, fileVersionSize, lpData, (LPVOID*)&lplpBuffer);
		showStr = (LPCTSTR)lplpBuffer;
		if (queryRst) {
			makeItemToCtrl(m_gridCtrlDown, 4, 1, showStr);
		}
		queryStr = _T("\\StringFileInfo\\%04x%04x\\LegalCopyright");
		queryRst = queryFileProperty(queryStr, fileVersionSize, lpData, (LPVOID*)&lplpBuffer);
		showStr = (LPCTSTR)lplpBuffer;
		if (queryRst) {
			makeItemToCtrl(m_gridCtrlDown, 5, 1, showStr);
		}
		queryStr = _T("\\StringFileInfo\\%04x%04x\\OriginalFilename");
		queryRst = queryFileProperty(queryStr, fileVersionSize, lpData, (LPVOID*)&lplpBuffer);
		showStr = (LPCTSTR)lplpBuffer;
		if (queryRst) {
			makeItemToCtrl(m_gridCtrlDown, 6, 1, showStr);
		}
		queryStr = _T("\\StringFileInfo\\%04x%04x\\ProductName");
		queryRst = queryFileProperty(queryStr, fileVersionSize, lpData, (LPVOID*)&lplpBuffer);
		showStr = (LPCTSTR)lplpBuffer;
		if (queryRst) {
			makeItemToCtrl(m_gridCtrlDown, 7, 1, showStr);
		}
		delete[] lpData;
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
using namespace std;
using std::vector;
void CPeDialog::OnSubsystemClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	NMHDR hdr =  pItem->hdr;
	int row = pItem->iRow;
	int column = pItem->iColumn;
	if (row == 23 && column == 4) {
		int selIndex = 0;
		CString *ary = new CString[13];
		CString str_0 = _T("Unknown subsystem");
		ary[0] = str_0;
		CString str_1 = _T("Native");
		ary[1] = str_1;
		CString str_2 = _T("Windows GUI");
		ary[2] = str_2;
		CString str_3 = _T("Windows CUI");
		ary[3] = str_3;
		CString str_4 = _T("OS/2 CUI subsystem");
		ary[4] = str_4;
		CString str_5 = _T("POSIX CUI subsystem");
		ary[5] = str_5;
		CString str_6 = _T("Windows CE system");
		ary[6] = str_6;
		CString str_7 = _T("EFI Application");
		ary[7] = str_7;
		CString str_8 = _T("EFI Boot Driver");
		ary[8] = str_8;
		CString str_9 = _T("EFI Runtime Driver");
		ary[9] = str_9;
		CString str_10 = _T("EFI ROM");
		ary[10] = str_10;
		CString str_11 = _T("XBox");
		ary[11] = str_11;
		CString str_12 = _T("Windows Boot Application");
		ary[12] = str_12;
		CString value = m_optionalHeaderCtrl.GetItemText(23, 3);
		value = _T("0x") + value;
		int valInt = 0;
		StrToIntEx(value, STIF_SUPPORT_HEX, &valInt);
		switch (valInt)
		{
		case IMAGE_SUBSYSTEM_UNKNOWN:
			selIndex = 0;
			break;
		case IMAGE_SUBSYSTEM_NATIVE:
			selIndex = 1;
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI:
			selIndex = 2;
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI:
			selIndex = 3;
			break;
		case IMAGE_SUBSYSTEM_OS2_CUI:
			selIndex = 4;
			break;
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			selIndex = 5;
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
			selIndex = 6;
			break;
		case IMAGE_SUBSYSTEM_EFI_APPLICATION:
			selIndex = 7;
			break;
		case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
			selIndex = 8;
			break;
		case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
			selIndex = 9;
			break;
		case IMAGE_SUBSYSTEM_EFI_ROM:
			selIndex = 10;
			break;
		case IMAGE_SUBSYSTEM_XBOX:
			selIndex = 11;
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION:
			selIndex = 12;
			break;
		default:
			selIndex = 0;
			break;
		}
		CShowSelectDlg subSysDlg(ary,13, selIndex, _T("Subsystem"));
		subSysDlg.DoModal();
	}
	if (row == 24 && column == 4) {
		CString value = m_optionalHeaderCtrl.GetItemText(24, 3);
		value = _T("0x") + value;
		int valInt = 0;
		StrToIntEx(value, STIF_SUPPORT_HEX, &valInt);
		vector<CString> strAry;
		vector<int> selVec;
		strAry.push_back(_T("Dll can move"));
		strAry.push_back(_T("Code integrity image"));
		strAry.push_back(_T("Image is NX compatible"));
		strAry.push_back(_T("Image understands isolation and doesn't want it"));
		strAry.push_back(_T("Image does not use SEH"));
		strAry.push_back(_T("Do not bind the image"));
		strAry.push_back(_T("A WDM driver"));
		strAry.push_back(_T("The image is terminal server aware"));
		if ((valInt & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE) == IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE) {
			selVec.push_back(1);
		}
		if ((valInt & IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY) == IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY) {
			selVec.push_back(2);
		}
		if ((valInt & IMAGE_DLLCHARACTERISTICS_NX_COMPAT) == IMAGE_DLLCHARACTERISTICS_NX_COMPAT) {
			selVec.push_back(3);
		}
		if ((valInt & IMAGE_DLLCHARACTERISTICS_NO_ISOLATION) == IMAGE_DLLCHARACTERISTICS_NO_ISOLATION) {
			selVec.push_back(4);
		}
		if ((valInt & IMAGE_DLLCHARACTERISTICS_NO_SEH) == IMAGE_DLLCHARACTERISTICS_NO_SEH) {
			selVec.push_back(5);
		}
		if ((valInt & IMAGE_DLLCHARACTERISTICS_NO_BIND) == IMAGE_DLLCHARACTERISTICS_NO_BIND) {
			selVec.push_back(6);
		}
		if ((valInt & IMAGE_DLLCHARACTERISTICS_WDM_DRIVER) == IMAGE_DLLCHARACTERISTICS_WDM_DRIVER) {
			selVec.push_back(7);
		}
		if ((valInt & IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE) == IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE) {
			selVec.push_back(8);
		}
		CShowCheckboxList dllC(strAry, selVec,_T("DllCharacteristics"));
		dllC.DoModal();
	}
}

void CPeDialog::parsePe()
{
	int result = validPeVerify();
	if (result == 1) {
		int ntOffset = parseDosHeader();
		int sectionSize = 0;
		parseNtHeader(ntOffset, sectionSize);
		sectionHeaderOffset = ntOffset;
		parseSection(ntOffset, sectionSize);
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
void createOptionalHeaderLine64(int row, CString name, int offset, CString &sizeStr, ULONGLONG value, CGridCtrl &optionalCtrl) {
	makeItemToCtrl(optionalCtrl, row, 0, name);
	CString offsetStrMagic;
	offsetStrMagic.Format(_T("%08X"), offset);
	makeItemToCtrl(optionalCtrl, row, 1, offsetStrMagic);
	makeItemToCtrl(optionalCtrl, row, 2, sizeStr);
	CString valueStr;
	if (row == 2 || row == 3) {
		valueStr.Format(_T("%02X"), value);
	}
	else if (row == 1 || (row >= 13 && row <= 18)) {
		valueStr.Format(_T("%04X"), value);
	}
	else {
		valueStr.Format(_T("%08X"), value);
	}
	makeItemToCtrl(optionalCtrl, row, 3, valueStr);
	if (row == 1) {
		CString meaningStr;
		if (value == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
			meaningStr = _T("PE32");
		}
		else if (value == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
			meaningStr = _T("PE64");
		}
		else if (value == IMAGE_ROM_OPTIONAL_HDR_MAGIC) {
			meaningStr = _T("ROM");
		}
		else {
			meaningStr = _T("Unknown value");
		}
		makeItemToCtrl(optionalCtrl, row, 4, meaningStr);
	}
	if (row == 7) {
		makeItemToCtrl(optionalCtrl, row, 4, _T(".text"));
	}
	if (name == _T("Subsystem")) {
		CString meaningStr;
		switch (value)
		{
		case IMAGE_SUBSYSTEM_UNKNOWN:
			meaningStr = _T("Unknown subsystem");
			break;
		case IMAGE_SUBSYSTEM_NATIVE:
			meaningStr = _T("Native");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI:
			meaningStr = _T("Windows GUI");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI:
			meaningStr = _T("Windows CUI");
			break;
		case IMAGE_SUBSYSTEM_OS2_CUI:
			meaningStr = _T("OS/2 CUI subsystem");
			break;
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			meaningStr = _T("POSIX CUI subsystem");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
			meaningStr = _T("Windows CE system");
			break;
		case IMAGE_SUBSYSTEM_EFI_APPLICATION:
			meaningStr = _T("EFI Application");
			break;
		case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
			meaningStr = _T("EFI Boot Driver");
			break;
		case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
			meaningStr = _T("EFI Runtime Driver");
			break;
		case IMAGE_SUBSYSTEM_EFI_ROM:
			meaningStr = _T("EFI ROM");
			break;
		case IMAGE_SUBSYSTEM_XBOX:
			meaningStr = _T("XBox");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION:
			meaningStr = _T("Windows Boot Application");
			break;
		default:
			meaningStr = _T("Unknown subsystem");
			break;
		}
		makeItemToCtrl(optionalCtrl, row, 4, meaningStr);
	}
	if (name == _T("DllCharacteristics")) {
		makeItemToCtrl(optionalCtrl, row, 4, _T("Click here"));
	}
}
void createOptionalHeaderLine(int row, CString name, int offset, CString &sizeStr, LONG value, CGridCtrl &optionalCtrl) {
	makeItemToCtrl(optionalCtrl, row, 0, name);
	CString offsetStrMagic;
	offsetStrMagic.Format(_T("%08X"), offset);
	makeItemToCtrl(optionalCtrl, row, 1, offsetStrMagic);
	makeItemToCtrl(optionalCtrl, row, 2, sizeStr);
	CString valueStr;
	if (row == 2 || row == 3) {
		valueStr.Format(_T("%02X"), value);
	}
	else if (row == 1 || (row >= 13 && row <= 18)) {
		valueStr.Format(_T("%04X"), value);
	}
	else {
		valueStr.Format(_T("%08X"), value);
	}
	makeItemToCtrl(optionalCtrl, row, 3, valueStr);
	if (row == 1) {
		CString meaningStr;
		if (value == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
			meaningStr = _T("PE32");
		}
		else if (value == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
			meaningStr = _T("PE64");
		}
		else if (value == IMAGE_ROM_OPTIONAL_HDR_MAGIC) {
			meaningStr = _T("ROM");
		}
		else {
			meaningStr = _T("Unknown value");
		}
		makeItemToCtrl(optionalCtrl, row, 4, meaningStr);
	}
	if (row == 7) {
		makeItemToCtrl(optionalCtrl, row, 4, _T(".text"));
	}
	if (name == _T("Subsystem")) {
		CString meaningStr;
		switch (value)
		{
		case IMAGE_SUBSYSTEM_UNKNOWN:
			meaningStr = _T("Unknown subsystem");
			break;
		case IMAGE_SUBSYSTEM_NATIVE:
			meaningStr = _T("Native");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI:
			meaningStr = _T("Windows GUI");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI:
			meaningStr = _T("Windows CUI");
			break;
		case IMAGE_SUBSYSTEM_OS2_CUI:
			meaningStr = _T("OS/2 CUI subsystem");
			break;
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			meaningStr = _T("POSIX CUI subsystem");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
			meaningStr = _T("Windows CE system");
			break;
		case IMAGE_SUBSYSTEM_EFI_APPLICATION:
			meaningStr = _T("EFI Application");
			break;
		case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
			meaningStr = _T("EFI Boot Driver");
			break;
		case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
			meaningStr = _T("EFI Runtime Driver");
			break;
		case IMAGE_SUBSYSTEM_EFI_ROM:
			meaningStr = _T("EFI ROM");
			break;
		case IMAGE_SUBSYSTEM_XBOX:
			meaningStr = _T("XBox");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION:
			meaningStr = _T("Windows Boot Application");
			break;
		default:
			meaningStr = _T("Unknown subsystem");
			break;
		}
		makeItemToCtrl(optionalCtrl, row, 4, meaningStr);
	}
	if (name == _T("DllCharacteristics")) {
		makeItemToCtrl(optionalCtrl, row, 4, _T("Click here"));
	}
}
void createSectionHeaderLine(int row, IMAGE_SECTION_HEADER &sectionHeader, CGridCtrl &sectionHeaderCtrl) {
	BYTE tmpName[9];
	memset(tmpName, 0, sizeof(tmpName));
	memcpy(tmpName, sectionHeader.Name, sizeof(sectionHeader.Name));
	CString nameStr(tmpName);
	makeItemToCtrl(sectionHeaderCtrl, row, 0, nameStr);
	CString virtualSizeStr;
	virtualSizeStr.Format(_T("%08X"), sectionHeader.Misc.VirtualSize);
	makeItemToCtrl(sectionHeaderCtrl, row, 1, virtualSizeStr);
	CString virtualAddressStr;
	virtualAddressStr.Format(_T("%08X"), sectionHeader.VirtualAddress);
	makeItemToCtrl(sectionHeaderCtrl, row, 2, virtualAddressStr);
	CString rawSizeStr;
	rawSizeStr.Format(_T("%08X"), sectionHeader.SizeOfRawData);
	makeItemToCtrl(sectionHeaderCtrl, row, 3, rawSizeStr);
	CString rawAddressStr;
	rawAddressStr.Format(_T("%08X"), sectionHeader.PointerToRawData);
	makeItemToCtrl(sectionHeaderCtrl, row, 4, rawAddressStr);
	CString relocAddressStr;
	relocAddressStr.Format(_T("%08X"), sectionHeader.PointerToRelocations);
	makeItemToCtrl(sectionHeaderCtrl, row, 5, relocAddressStr);
	CString lineNumbersStr;
	lineNumbersStr.Format(_T("%08X"), sectionHeader.NumberOfLinenumbers);
	makeItemToCtrl(sectionHeaderCtrl, row, 6, lineNumbersStr);
	CString relocationsNumberStr;
	relocationsNumberStr.Format(_T("%04X"), sectionHeader.NumberOfRelocations);
	makeItemToCtrl(sectionHeaderCtrl, row, 7, relocationsNumberStr);
	CString lineNumberNumbersStr;
	lineNumberNumbersStr.Format(_T("%04X"), sectionHeader.NumberOfLinenumbers);
	makeItemToCtrl(sectionHeaderCtrl, row, 8, lineNumberNumbersStr);
	CString characteristicsStr;
	characteristicsStr.Format(_T("%08X"), sectionHeader.Characteristics);
	makeItemToCtrl(sectionHeaderCtrl, row, 9, characteristicsStr);
}
void CPeDialog::parseSection(int & ntOffset, int & sectionSize)
{
	CFile file;
	BOOL result = file.Open(m_exeFullPath, CFile::modeRead);
	if (result) {
		CString sizeStrAry[4] = { _T("Byte"),_T("Word"),_T("") ,_T("Dword") };
		file.Seek(ntOffset, CFile::begin);
		m_sectionAry = new IMAGE_SECTION_HEADER[sectionSize];
		UINT ret = file.Read(m_sectionAry, sizeof(IMAGE_SECTION_HEADER) * sectionSize);
		m_sectionHeadersCtrl.SetEditable(true);
		m_sectionHeadersCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景
		m_sectionHeadersCtrl.SetRowCount(14);
		m_sectionHeadersCtrl.SetColumnCount(10);
		m_sectionHeadersCtrl.SetFixedRowCount(3);
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 0, _T("Name"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 1, _T("VirtualSize"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 2, _T("VirtualAddress"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 3, _T("RawSize"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 4, _T("RawAddress"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 5, _T("RelocAddress"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 6, _T("Linenumbers"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 7, _T("RelocationsNumber"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 8, _T("LinenumbersNumber"));
		makeItemToCtrl(m_sectionHeadersCtrl, 0, 9, _T("Characteristics"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 0, _T("Byte[8]"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 1, _T("Dword"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 2, _T("Dword"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 3, _T("Dword"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 4, _T("Dword"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 5, _T("Dword"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 6, _T("Dword"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 7, _T("Word"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 8, _T("Word"));
		makeItemToCtrl(m_sectionHeadersCtrl, 2, 9, _T("Dword"));
		if (ret > 0) {
			int tmpRow = 3;
			for (int i = 0; i < sectionSize; i++)
			{
				createSectionHeaderLine(tmpRow, m_sectionAry[i], m_sectionHeadersCtrl);
				tmpRow++;
			}
		}
		file.Close();
		return;
	}
	else {
		MessageBox(_T("打开文件失败"), _T("消息框"), MB_OK);
		return;
	}
}
void CPeDialog::parseNtHeader(int &ntOffset,int &sectionSize)
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
		sectionSize = fileHeader.NumberOfSections;
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
		m_optionalHeaderCtrl.SetEditable(true);
		m_optionalHeaderCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景
		m_optionalHeaderCtrl.SetRowCount(31);
		m_optionalHeaderCtrl.SetColumnCount(5);
		m_optionalHeaderCtrl.SetFixedRowCount(1);
		m_optionalHeaderCtrl.SetFixedColumnCount(3);
		makeItemToCtrl(m_optionalHeaderCtrl, 0, 0, _T("Member"));
		makeItemToCtrl(m_optionalHeaderCtrl, 0, 1, _T("Offset"));
		makeItemToCtrl(m_optionalHeaderCtrl, 0, 2, _T("Size"));
		makeItemToCtrl(m_optionalHeaderCtrl, 0, 3, _T("Value"));
		makeItemToCtrl(m_optionalHeaderCtrl, 0, 4, _T("Meaning"));
		for (int row = 0; row < m_optionalHeaderCtrl.GetRowCount(); row++) {
			for (int col = 0; col < m_optionalHeaderCtrl.GetColumnCount(); col++)
			{
				m_optionalHeaderCtrl.SetRowHeight(row, 25); //设置各行高         
				if (col == 0) {
					m_optionalHeaderCtrl.SetColumnWidth(col, 200); //设置各列宽
				}
				else {
					m_optionalHeaderCtrl.SetColumnWidth(col, 100); //设置各列宽
				}
				if (col == m_optionalHeaderCtrl.GetColumnCount() - 1) {
					m_optionalHeaderCtrl.SetItemState(row, col, m_optionalHeaderCtrl.GetItemState(row, col) | GVIS_READONLY);
				}
			}
		}
		m_optionalHeaderCtrl.SetItemBkColour(1, 3, RGB(174, 197, 232));
		m_optionalHeaderCtrl.SetItemBkColour(1, 4, RGB(174, 197, 232));
		m_optionalHeaderCtrl.SetItemBkColour(23, 3, RGB(174, 197, 232));
		m_optionalHeaderCtrl.SetItemBkColour(23, 4, RGB(174, 197, 232));
		m_optionalHeaderCtrl.SetItemBkColour(24, 3, RGB(174, 197, 232));
		m_optionalHeaderCtrl.SetItemBkColour(24, 4, RGB(174, 197, 232));
		if (fileHeader.Machine == IMAGE_FILE_MACHINE_IA64
			|| fileHeader.Machine == IMAGE_FILE_MACHINE_ALPHA64
			|| fileHeader.Machine == IMAGE_FILE_MACHINE_AMD64) {
			IMAGE_OPTIONAL_HEADER64 image64;
			file.Read(&image64, fileHeader.SizeOfOptionalHeader);
			parseOptionalHeader64(offsetTmp, image64);
			offsetTmp += sizeof(image64);
		}
		else {
			IMAGE_OPTIONAL_HEADER32 image32;
			file.Read(&image32, fileHeader.SizeOfOptionalHeader);
			parseOptionalHeader32(offsetTmp, image32);
			offsetTmp += sizeof(image32);
		}
		ntOffset = offsetTmp;
		file.Close();
		return;
	}
	else {
		MessageBox(_T("打开文件失败"), _T("消息框"), MB_OK);
		return;
	}
}

void createDataDirectoryHeaderLine(int row, CString name, int offset, CString &sizeStr, LONG value, CGridCtrl &optionalCtrl) {
	makeItemToCtrl(optionalCtrl, row, 0, name);
	CString offsetStrMagic;
	offsetStrMagic.Format(_T("%08X"), offset);
	makeItemToCtrl(optionalCtrl, row, 1, offsetStrMagic);
	makeItemToCtrl(optionalCtrl, row, 2, sizeStr);
	CString valueStr;
	valueStr.Format(_T("%08X"), value);
	makeItemToCtrl(optionalCtrl, row, 3, valueStr);
	CString sectionStr;
	switch (row)
	{
	case 3:
		sectionStr = _T(".idata");
		break;
	case 5:
		sectionStr = _T(".rsrc");
		break;
	case 11:
		sectionStr = _T(".reloc");
		break;
	case 13:
		sectionStr = _T(".rdata");
		break;
	case 19:
		sectionStr = _T(".rdata");
		break;
	case 21:
		sectionStr = _T(".rdata");
		break;
	case 25:
		sectionStr = _T(".rdata");
		break;
	default:
		sectionStr = _T("");
		break;
	}
	makeItemToCtrl(optionalCtrl, row, 4, sectionStr);
}
void CPeDialog::parseOptionalHeader32(int offset, IMAGE_OPTIONAL_HEADER32 & image32)
{
	CString sizeStrAry[4] = { _T("Byte"),_T("Word"),_T("") ,_T("Dword") };
	createOptionalHeaderLine(1, _T("Magic"), offset, sizeStrAry[sizeof(image32.Magic) - 1], image32.Magic, m_optionalHeaderCtrl);
	offset += sizeof(image32.Magic);
	createOptionalHeaderLine(2, _T("MajorLinkerVersion"), offset, sizeStrAry[sizeof(image32.MajorLinkerVersion) - 1], image32.MajorLinkerVersion, m_optionalHeaderCtrl);
	offset += sizeof(image32.MajorLinkerVersion);
	createOptionalHeaderLine(3, _T("MinorLinkerVersion"), offset, sizeStrAry[sizeof(image32.MinorLinkerVersion) - 1], image32.MinorLinkerVersion, m_optionalHeaderCtrl);
	offset += sizeof(image32.MinorLinkerVersion);
	createOptionalHeaderLine(4, _T("SizeOfCode"), offset, sizeStrAry[sizeof(image32.SizeOfCode) - 1], image32.SizeOfCode, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfCode);
	createOptionalHeaderLine(5, _T("SizeOfInitializedData"), offset, sizeStrAry[sizeof(image32.SizeOfInitializedData) - 1], image32.SizeOfInitializedData, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfInitializedData);
	createOptionalHeaderLine(6, _T("SizeOfUninitializedData"), offset, sizeStrAry[sizeof(image32.SizeOfUninitializedData) - 1], image32.SizeOfUninitializedData, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfUninitializedData);
	createOptionalHeaderLine(7, _T("AddressOfEntryPoint"), offset, sizeStrAry[sizeof(image32.AddressOfEntryPoint) - 1], image32.AddressOfEntryPoint, m_optionalHeaderCtrl);
	offset += sizeof(image32.AddressOfEntryPoint);
	createOptionalHeaderLine(8, _T("BaseOfCode"), offset, sizeStrAry[sizeof(image32.BaseOfCode) - 1], image32.BaseOfCode, m_optionalHeaderCtrl);
	offset += sizeof(image32.BaseOfCode);
	createOptionalHeaderLine(9, _T("BaseOfData"), offset, sizeStrAry[sizeof(image32.BaseOfData) - 1], image32.BaseOfData, m_optionalHeaderCtrl);
	offset += sizeof(image32.BaseOfData);
	createOptionalHeaderLine(10, _T("ImageBase"), offset, sizeStrAry[sizeof(image32.ImageBase) - 1], image32.ImageBase, m_optionalHeaderCtrl);
	offset += sizeof(image32.ImageBase);
	createOptionalHeaderLine(11, _T("SectionAlignment"), offset, sizeStrAry[sizeof(image32.SectionAlignment) - 1], image32.SectionAlignment, m_optionalHeaderCtrl);
	offset += sizeof(image32.SectionAlignment);
	createOptionalHeaderLine(12, _T("FileAlignment"), offset, sizeStrAry[sizeof(image32.FileAlignment) - 1], image32.FileAlignment, m_optionalHeaderCtrl);
	offset += sizeof(image32.FileAlignment);
	createOptionalHeaderLine(13, _T("MajorOperatingSystemVersion"), offset, sizeStrAry[sizeof(image32.MajorOperatingSystemVersion) - 1], image32.MajorOperatingSystemVersion, m_optionalHeaderCtrl);
	offset += sizeof(image32.MajorOperatingSystemVersion);
	createOptionalHeaderLine(14, _T("MinorOperatingSystemVersion"), offset, sizeStrAry[sizeof(image32.MinorOperatingSystemVersion) - 1], image32.MinorOperatingSystemVersion, m_optionalHeaderCtrl);
	offset += sizeof(image32.MinorOperatingSystemVersion);
	createOptionalHeaderLine(15, _T("MajorImageVersion"), offset, sizeStrAry[sizeof(image32.MajorImageVersion) - 1], image32.MajorImageVersion, m_optionalHeaderCtrl);
	offset += sizeof(image32.MajorImageVersion);
	createOptionalHeaderLine(16, _T("MinorImageVersion"), offset, sizeStrAry[sizeof(image32.MinorImageVersion) - 1], image32.MinorImageVersion, m_optionalHeaderCtrl);
	offset += sizeof(image32.MinorImageVersion);
	createOptionalHeaderLine(17, _T("MajorSubsystemVersion"), offset, sizeStrAry[sizeof(image32.MajorSubsystemVersion) - 1], image32.MajorSubsystemVersion, m_optionalHeaderCtrl);
	offset += sizeof(image32.MajorSubsystemVersion);
	createOptionalHeaderLine(18, _T("MinorSubsystemVersion"), offset, sizeStrAry[sizeof(image32.MinorSubsystemVersion) - 1], image32.MinorSubsystemVersion, m_optionalHeaderCtrl);
	offset += sizeof(image32.MinorSubsystemVersion);
	createOptionalHeaderLine(19, _T("Win32VersionValue"), offset, sizeStrAry[sizeof(image32.Win32VersionValue) - 1], image32.Win32VersionValue, m_optionalHeaderCtrl);
	offset += sizeof(image32.Win32VersionValue);
	createOptionalHeaderLine(20, _T("SizeOfImage"), offset, sizeStrAry[sizeof(image32.SizeOfImage) - 1], image32.SizeOfImage, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfImage);
	createOptionalHeaderLine(21, _T("SizeOfHeaders"), offset, sizeStrAry[sizeof(image32.SizeOfHeaders) - 1], image32.SizeOfHeaders, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfHeaders);
	createOptionalHeaderLine(22, _T("CheckSum"), offset, sizeStrAry[sizeof(image32.CheckSum) - 1], image32.CheckSum, m_optionalHeaderCtrl);
	offset += sizeof(image32.CheckSum);
	createOptionalHeaderLine(23, _T("Subsystem"), offset, sizeStrAry[sizeof(image32.Subsystem) - 1], image32.Subsystem, m_optionalHeaderCtrl);
	offset += sizeof(image32.Subsystem);
	createOptionalHeaderLine(24, _T("DllCharacteristics"), offset, sizeStrAry[sizeof(image32.DllCharacteristics) - 1], image32.DllCharacteristics, m_optionalHeaderCtrl);
	offset += sizeof(image32.DllCharacteristics);
	createOptionalHeaderLine(25, _T("SizeOfStackReserve"), offset, sizeStrAry[sizeof(image32.SizeOfStackReserve) - 1], image32.SizeOfStackReserve, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfStackReserve);
	createOptionalHeaderLine(26, _T("SizeOfStackCommit"), offset, sizeStrAry[sizeof(image32.SizeOfStackCommit) - 1], image32.SizeOfStackCommit, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfStackCommit);
	createOptionalHeaderLine(27, _T("SizeOfHeapReserve"), offset, sizeStrAry[sizeof(image32.SizeOfHeapReserve) - 1], image32.SizeOfHeapReserve, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfHeapReserve);
	createOptionalHeaderLine(28, _T("SizeOfHeapCommit"), offset, sizeStrAry[sizeof(image32.SizeOfHeapCommit) - 1], image32.SizeOfHeapCommit, m_optionalHeaderCtrl);
	offset += sizeof(image32.SizeOfHeapCommit);
	createOptionalHeaderLine(29, _T("LoaderFlags"), offset, sizeStrAry[sizeof(image32.LoaderFlags) - 1], image32.LoaderFlags, m_optionalHeaderCtrl);
	offset += sizeof(image32.LoaderFlags);
	createOptionalHeaderLine(30, _T("NumberOfRvaAndSizes"), offset, sizeStrAry[sizeof(image32.NumberOfRvaAndSizes) - 1], image32.NumberOfRvaAndSizes, m_optionalHeaderCtrl);
	offset += sizeof(image32.NumberOfRvaAndSizes);
	m_dataDirectoriesCtrl.SetEditable(true);
	m_dataDirectoriesCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景
	m_dataDirectoriesCtrl.SetRowCount(31);
	m_dataDirectoriesCtrl.SetColumnCount(5);
	m_dataDirectoriesCtrl.SetFixedRowCount(1);
	m_dataDirectoriesCtrl.SetFixedColumnCount(3);
	makeItemToCtrl(m_dataDirectoriesCtrl, 0, 0, _T("Member"));
	makeItemToCtrl(m_dataDirectoriesCtrl, 0, 1, _T("Offset"));
	makeItemToCtrl(m_dataDirectoriesCtrl, 0, 2, _T("Size"));
	makeItemToCtrl(m_dataDirectoriesCtrl, 0, 3, _T("Value"));
	makeItemToCtrl(m_dataDirectoriesCtrl, 0, 4, _T("Section"));
	for (int row = 0; row < m_dataDirectoriesCtrl.GetRowCount(); row++) {
		for (int col = 0; col < m_dataDirectoriesCtrl.GetColumnCount(); col++)
		{
			m_dataDirectoriesCtrl.SetRowHeight(row, 25); //设置各行高         
			if (col == 0) {
				m_dataDirectoriesCtrl.SetColumnWidth(col, 300); //设置各列宽
			}
			else {
				m_dataDirectoriesCtrl.SetColumnWidth(col, 100); //设置各列宽
			}
			if (col == m_dataDirectoriesCtrl.GetColumnCount() - 1) {
				m_dataDirectoriesCtrl.SetItemState(row, col, m_dataDirectoriesCtrl.GetItemState(row, col) | GVIS_READONLY);
				if (row > 0)
					m_dataDirectoriesCtrl.SetItemBkColour(row, col, RGB(215, 215, 215));
			}
		}
	}
	createDataDirectoryHeaderLine(1, _T("Export Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	createDataDirectoryHeaderLine(2, _T("Export Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);
	createDataDirectoryHeaderLine(3, _T("Import Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	createDataDirectoryHeaderLine(4, _T("Import Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
	createDataDirectoryHeaderLine(5, _T("Resource Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	createDataDirectoryHeaderLine(6, _T("Resource Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);
	createDataDirectoryHeaderLine(7, _T("Exception Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);
	createDataDirectoryHeaderLine(8, _T("Exception Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size);
	createDataDirectoryHeaderLine(9, _T("Security Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);
	createDataDirectoryHeaderLine(10, _T("Security Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
	createDataDirectoryHeaderLine(11, _T("Base Relocation Table RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	createDataDirectoryHeaderLine(12, _T("Base Relocation Table Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
	createDataDirectoryHeaderLine(13, _T("Debug Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
	createDataDirectoryHeaderLine(14, _T("Debug Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);
	createDataDirectoryHeaderLine(15, _T("Architecture Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress);
	createDataDirectoryHeaderLine(16, _T("Architecture Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size);
	createDataDirectoryHeaderLine(17, _T("Reserved"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress);
	createDataDirectoryHeaderLine(18, _T("Reserved"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size);
	createDataDirectoryHeaderLine(19, _T("TLS Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
	createDataDirectoryHeaderLine(20, _T("TLS Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size);
	createDataDirectoryHeaderLine(21, _T("Configuration Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
	createDataDirectoryHeaderLine(22, _T("Configuration Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size);
	createDataDirectoryHeaderLine(23, _T("Bound Import Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress);
	createDataDirectoryHeaderLine(24, _T("Bound Import Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);
	createDataDirectoryHeaderLine(25, _T("Import Address Table Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	createDataDirectoryHeaderLine(26, _T("Import Address Table Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);
	createDataDirectoryHeaderLine(27, _T("Delay Import Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
	createDataDirectoryHeaderLine(28, _T("Delay Import Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size);
	createDataDirectoryHeaderLine(29, _T("COM Runtime Directory RVA"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress);
	createDataDirectoryHeaderLine(30, _T("COM Runtime Directory Size"), offset,
		sizeStrAry[sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size) - 1],
		image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image32.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size);
}

void CPeDialog::parseOptionalHeader64(int offset, IMAGE_OPTIONAL_HEADER64 & image64)
{
	CString sizeStrAry[8] = { _T("Byte"),_T("Word"),_T("") ,_T("Dword"),_T(""),_T(""),_T(""),_T("DWORD64") };
	createOptionalHeaderLine(1, _T("Magic"), offset, sizeStrAry[sizeof(image64.Magic) - 1], image64.Magic, m_optionalHeaderCtrl);
	offset += sizeof(image64.Magic);
	createOptionalHeaderLine(2, _T("MajorLinkerVersion"), offset, sizeStrAry[sizeof(image64.MajorLinkerVersion) - 1], image64.MajorLinkerVersion, m_optionalHeaderCtrl);
	offset += sizeof(image64.MajorLinkerVersion);
	createOptionalHeaderLine(3, _T("MinorLinkerVersion"), offset, sizeStrAry[sizeof(image64.MinorLinkerVersion) - 1], image64.MinorLinkerVersion, m_optionalHeaderCtrl);
	offset += sizeof(image64.MinorLinkerVersion);
	createOptionalHeaderLine(4, _T("SizeOfCode"), offset, sizeStrAry[sizeof(image64.SizeOfCode) - 1], image64.SizeOfCode, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfCode);
	createOptionalHeaderLine(5, _T("SizeOfInitializedData"), offset, sizeStrAry[sizeof(image64.SizeOfInitializedData) - 1], image64.SizeOfInitializedData, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfInitializedData);
	createOptionalHeaderLine(6, _T("SizeOfUninitializedData"), offset, sizeStrAry[sizeof(image64.SizeOfUninitializedData) - 1], image64.SizeOfUninitializedData, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfUninitializedData);
	createOptionalHeaderLine(7, _T("AddressOfEntryPoint"), offset, sizeStrAry[sizeof(image64.AddressOfEntryPoint) - 1], image64.AddressOfEntryPoint, m_optionalHeaderCtrl);
	offset += sizeof(image64.AddressOfEntryPoint);
	createOptionalHeaderLine(8, _T("BaseOfCode"), offset, sizeStrAry[sizeof(image64.BaseOfCode) - 1], image64.BaseOfCode, m_optionalHeaderCtrl);
	offset += sizeof(image64.BaseOfCode);
	createOptionalHeaderLine64(9, _T("ImageBase"), offset, sizeStrAry[sizeof(image64.ImageBase) - 1], image64.ImageBase, m_optionalHeaderCtrl);
	offset += sizeof(image64.ImageBase);
	createOptionalHeaderLine(10, _T("SectionAlignment"), offset, sizeStrAry[sizeof(image64.SectionAlignment) - 1], image64.SectionAlignment, m_optionalHeaderCtrl);
	offset += sizeof(image64.SectionAlignment);
	createOptionalHeaderLine(11, _T("FileAlignment"), offset, sizeStrAry[sizeof(image64.FileAlignment) - 1], image64.FileAlignment, m_optionalHeaderCtrl);
	offset += sizeof(image64.FileAlignment);
	createOptionalHeaderLine(12, _T("MajorOperatingSystemVersion"), offset, sizeStrAry[sizeof(image64.MajorOperatingSystemVersion) - 1], image64.MajorOperatingSystemVersion, m_optionalHeaderCtrl);
	offset += sizeof(image64.MajorOperatingSystemVersion);
	createOptionalHeaderLine(13, _T("MinorOperatingSystemVersion"), offset, sizeStrAry[sizeof(image64.MinorOperatingSystemVersion) - 1], image64.MinorOperatingSystemVersion, m_optionalHeaderCtrl);
	offset += sizeof(image64.MinorOperatingSystemVersion);
	createOptionalHeaderLine(14, _T("MajorImageVersion"), offset, sizeStrAry[sizeof(image64.MajorImageVersion) - 1], image64.MajorImageVersion, m_optionalHeaderCtrl);
	offset += sizeof(image64.MajorImageVersion);
	createOptionalHeaderLine(15, _T("MinorImageVersion"), offset, sizeStrAry[sizeof(image64.MinorImageVersion) - 1], image64.MinorImageVersion, m_optionalHeaderCtrl);
	offset += sizeof(image64.MinorImageVersion);
	createOptionalHeaderLine(16, _T("MajorSubsystemVersion"), offset, sizeStrAry[sizeof(image64.MajorSubsystemVersion) - 1], image64.MajorSubsystemVersion, m_optionalHeaderCtrl);
	offset += sizeof(image64.MajorSubsystemVersion);
	createOptionalHeaderLine(17, _T("MinorSubsystemVersion"), offset, sizeStrAry[sizeof(image64.MinorSubsystemVersion) - 1], image64.MinorSubsystemVersion, m_optionalHeaderCtrl);
	offset += sizeof(image64.MinorSubsystemVersion);
	createOptionalHeaderLine(18, _T("Win32VersionValue"), offset, sizeStrAry[sizeof(image64.Win32VersionValue) - 1], image64.Win32VersionValue, m_optionalHeaderCtrl);
	offset += sizeof(image64.Win32VersionValue);
	createOptionalHeaderLine(19, _T("SizeOfImage"), offset, sizeStrAry[sizeof(image64.SizeOfImage) - 1], image64.SizeOfImage, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfImage);
	createOptionalHeaderLine(20, _T("SizeOfHeaders"), offset, sizeStrAry[sizeof(image64.SizeOfHeaders) - 1], image64.SizeOfHeaders, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfHeaders);
	createOptionalHeaderLine(21, _T("CheckSum"), offset, sizeStrAry[sizeof(image64.CheckSum) - 1], image64.CheckSum, m_optionalHeaderCtrl);
	offset += sizeof(image64.CheckSum);
	createOptionalHeaderLine(22, _T("Subsystem"), offset, sizeStrAry[sizeof(image64.Subsystem) - 1], image64.Subsystem, m_optionalHeaderCtrl);
	offset += sizeof(image64.Subsystem);
	createOptionalHeaderLine(23, _T("DllCharacteristics"), offset, sizeStrAry[sizeof(image64.DllCharacteristics) - 1], image64.DllCharacteristics, m_optionalHeaderCtrl);
	offset += sizeof(image64.DllCharacteristics);
	createOptionalHeaderLine64(24, _T("SizeOfStackReserve"), offset, sizeStrAry[sizeof(image64.SizeOfStackReserve) - 1], image64.SizeOfStackReserve, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfStackReserve);
	createOptionalHeaderLine64(25, _T("SizeOfStackCommit"), offset, sizeStrAry[sizeof(image64.SizeOfStackCommit) - 1], image64.SizeOfStackCommit, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfStackCommit);
	createOptionalHeaderLine64(26, _T("SizeOfHeapReserve"), offset, sizeStrAry[sizeof(image64.SizeOfHeapReserve) - 1], image64.SizeOfHeapReserve, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfHeapReserve);
	createOptionalHeaderLine64(27, _T("SizeOfHeapCommit"), offset, sizeStrAry[sizeof(image64.SizeOfHeapCommit) - 1], image64.SizeOfHeapCommit, m_optionalHeaderCtrl);
	offset += sizeof(image64.SizeOfHeapCommit);
	createOptionalHeaderLine(28, _T("LoaderFlags"), offset, sizeStrAry[sizeof(image64.LoaderFlags) - 1], image64.LoaderFlags, m_optionalHeaderCtrl);
	offset += sizeof(image64.LoaderFlags);
	createOptionalHeaderLine(29, _T("NumberOfRvaAndSizes"), offset, sizeStrAry[sizeof(image64.NumberOfRvaAndSizes) - 1], image64.NumberOfRvaAndSizes, m_optionalHeaderCtrl);
	offset += sizeof(image64.NumberOfRvaAndSizes);
	createDataDirectoryHeaderLine(1, _T("Export Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	createDataDirectoryHeaderLine(2, _T("Export Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);
	createDataDirectoryHeaderLine(3, _T("Import Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	createDataDirectoryHeaderLine(4, _T("Import Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
	createDataDirectoryHeaderLine(5, _T("Resource Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	createDataDirectoryHeaderLine(6, _T("Resource Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);
	createDataDirectoryHeaderLine(7, _T("Exception Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);
	createDataDirectoryHeaderLine(8, _T("Exception Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size);
	createDataDirectoryHeaderLine(9, _T("Security Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);
	createDataDirectoryHeaderLine(10, _T("Security Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
	createDataDirectoryHeaderLine(11, _T("Base Relocation Table RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	createDataDirectoryHeaderLine(12, _T("Base Relocation Table Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
	createDataDirectoryHeaderLine(13, _T("Debug Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
	createDataDirectoryHeaderLine(14, _T("Debug Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);
	createDataDirectoryHeaderLine(15, _T("Architecture Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress);
	createDataDirectoryHeaderLine(16, _T("Architecture Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size);
	createDataDirectoryHeaderLine(17, _T("Reserved"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress);
	createDataDirectoryHeaderLine(18, _T("Reserved"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size);
	createDataDirectoryHeaderLine(19, _T("TLS Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
	createDataDirectoryHeaderLine(20, _T("TLS Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size);
	createDataDirectoryHeaderLine(21, _T("Configuration Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
	createDataDirectoryHeaderLine(22, _T("Configuration Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size);
	createDataDirectoryHeaderLine(23, _T("Bound Import Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress);
	createDataDirectoryHeaderLine(24, _T("Bound Import Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);
	createDataDirectoryHeaderLine(25, _T("Import Address Table Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	createDataDirectoryHeaderLine(26, _T("Import Address Table Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);
	createDataDirectoryHeaderLine(27, _T("Delay Import Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
	createDataDirectoryHeaderLine(28, _T("Delay Import Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size);
	createDataDirectoryHeaderLine(29, _T("COM Runtime Directory RVA"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress);
	createDataDirectoryHeaderLine(30, _T("COM Runtime Directory Size"), offset,
		sizeStrAry[sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size) - 1],
		image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size, m_dataDirectoriesCtrl);
	offset += sizeof(image64.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size);
}


void CPeDialog::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();
	if (m_sectionAry) {
		delete m_sectionAry;
	}
	// TODO: 在此处添加消息处理程序代码
}

void CPeDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 647;   //x宽度  
	lpMMI->ptMinTrackSize.y = 522;   //y高度 
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

