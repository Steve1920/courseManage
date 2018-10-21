// CourseManageMain.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "CourseManageMain.h"
#include "afxdialogex.h"

int GetMemoryUsage(uint64_t * mem, DWORD processId)
{
	PROCESS_MEMORY_COUNTERS pmc;
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processId);
	if (NULL == hProcess)
		return -1;
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		if (mem) *mem = pmc.WorkingSetSize;
		CloseHandle(hProcess);
		return 0;
	}
	CloseHandle(hProcess);
	return -1;
}


int GetInfomation(DWORD processId, CString &info)
{

	TCHAR str[400];
	DWORD size = 400;
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION,
		0, processId);
	if (NULL == hProcess)
		return -1;
	int resultInt = 0;
	if (QueryFullProcessImageName(hProcess, 0, str, &size))
	{
		DWORD fileVersionSize = GetFileVersionInfoSize(str, NULL);
		if (fileVersionSize) {
			char * lpData = new char[fileVersionSize + 1];
			BOOL queryRst = GetFileVersionInfo(
				str,
				0,
				fileVersionSize,
				lpData
			);
			if (queryRst) {
				LPVOID  *lplpBuffer = (LPVOID*)new char[200];
				memset(lplpBuffer, 0, 200);
				UINT tmpSize = 0;
				struct LANGANDCODEPAGE {
					WORD wLanguage;
					WORD wCodePage;
				} *lpTranslate;
				queryRst = VerQueryValue(lpData,
					_T("\\VarFileInfo\\Translation"),
					(LPVOID*)&lpTranslate,
					&tmpSize);
				memset(lplpBuffer, 0, 200);
				CString cs;
				cs.Format(_T("\\StringFileInfo\\%04x%04x\\FileDescription"), lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
				queryRst = VerQueryValue(lpData,
					cs,
					(LPVOID*)&lplpBuffer,
					&tmpSize);
				if (queryRst) {
					info = (LPCTSTR)lplpBuffer;
				}
			}
			delete[] lpData;
		}
		else {
			resultInt = -1;
		}
	}
	else {
		resultInt = -1;
	}
	CloseHandle(hProcess);
	return resultInt;
}

IMPLEMENT_DYNAMIC(CCourseManageMain, CDialogEx)

CCourseManageMain::CCourseManageMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COURSEMANAGE_MAIN, pParent)
{

}

CCourseManageMain::~CCourseManageMain()
{

}

void CCourseManageMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COURSE_LIST, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CCourseManageMain, CDialogEx)
	ON_COMMAND(ID_32772, &CCourseManageMain::OnMenuExitClick)
//	ON_NOTIFY(NM_CLICK, IDC_COURSE_LIST, &CCourseManageMain::OnNMClickCourseList)
ON_BN_CLICKED(IDC_BUTTON_KILLPROCESS, &CCourseManageMain::OnBnClickedButtonKillprocess)
ON_NOTIFY(NM_RCLICK, IDC_COURSE_LIST, &CCourseManageMain::OnNMRClickCourseList)
ON_COMMAND(ID__KILL_PROCESS, &CCourseManageMain::InMenuKillProcess)
ON_COMMAND(ID__32782, &CCourseManageMain::InMenuOpenFolder)
ON_COMMAND(ID_32774, &CCourseManageMain::OnRefreshRightNow)
ON_COMMAND(ID_32771, &CCourseManageMain::OnCreateNewProcess)
ON_NOTIFY(LVN_COLUMNCLICK, IDC_COURSE_LIST, &CCourseManageMain::OnLvnColumnclickCourseList)
ON_COMMAND(ID_REFRESH_HIGH, &CCourseManageMain::OnRefreshHigh)
ON_COMMAND(ID_REFRESH_NORMAL, &CCourseManageMain::OnRefreshNormal)
ON_COMMAND(ID_REFRESH_LOW, &CCourseManageMain::OnRefreshLow)
ON_COMMAND(ID_REFRESH_PAUSE, &CCourseManageMain::OnRefreshPause)
END_MESSAGE_MAP()

BOOL g_exitFlag = false;
int m_refreshInterval = 2000;//进程列表刷新频率
// CCourseManageMain 消息处理程序
void refreshRightNow(CListCtrl & m_listCtrl) {
	map<CString, int> processMap;
	map<CString, int>::iterator processIter;
	int count = m_listCtrl.GetItemCount();   //行数
	for (int i = 0; i<count; i++)
	{
		CString pName = m_listCtrl.GetItemText(i, 0);
		CString pid = m_listCtrl.GetItemText(i, 1);
		processMap[pName + _T("_") + pid] = i;
	}
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << TEXT("CreateToolhelp32Snapshot (of processes)") << endl;
		return;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		cout << (TEXT("Process32First")) << endl; // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return;
	}
	BOOL changeFlag = false;
	do
	{
		CString key(pe32.szExeFile);
		CString pidStr;
		pidStr.Format(_T("%d"), pe32.th32ProcessID);
		key += _T("_") + pidStr;
		processIter = processMap.find(key);
		uint64_t mem = 0;
		int retInt = GetMemoryUsage(&mem, pe32.th32ProcessID);
		if (retInt == -1) continue;
		if (processIter != processMap.end()) {
			int nItem = processMap[key];
			CString threadNums;
			threadNums.Format(_T("%d"), pe32.cntThreads);
			CString memStr;
			memStr.Format(_T("%dK"), mem / 1024);
			if (threadNums != m_listCtrl.GetItemText(nItem, 2)) {
				m_listCtrl.SetItemText(nItem, 2, threadNums);
			}
			if (memStr != m_listCtrl.GetItemText(nItem, 4)) {
				m_listCtrl.SetItemText(nItem, 4, memStr);
			}
			processMap.erase(key);
		}
		else {
			CString info;
			GetInfomation(pe32.th32ProcessID, info);
			if (info == _T("")) {
				info = pe32.szExeFile;
			}
			int iLine = m_listCtrl.GetItemCount();
			m_listCtrl.InsertItem(iLine, pe32.szExeFile);
			CString processIdStr;
			processIdStr.Format(_T("%d"), pe32.th32ProcessID);
			m_listCtrl.SetItemText(iLine, 1, processIdStr);
			CString threadCount;
			threadCount.Format(_T("%d"), pe32.cntThreads);
			m_listCtrl.SetItemText(iLine, 2, threadCount);
			CString cpuStr(_T("查询中..."));
			m_listCtrl.SetItemText(iLine, 3, cpuStr);
			CString memoryStr;
			memoryStr.Format(_T("%dK"), mem / 1024);
			m_listCtrl.SetItemText(iLine, 4, memoryStr);
			m_listCtrl.SetItemText(iLine, 5, info);
			changeFlag = true;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	//根据processMap更新m_listCtrl的列表
	int size = processMap.size();
	if (size > 0) {
		for (processIter = processMap.begin(); processIter != processMap.end(); ++processIter)
			m_listCtrl.DeleteItem((*processIter).second);
	}
	if (changeFlag) {
		int count = m_listCtrl.GetItemCount();   //行数
		for (int i = 0; i<count; i++)
		{
			m_listCtrl.SetItemData(i, i);
		}
		m_listCtrl.SortItems(CCourseManageMain::MyListCompar, (LPARAM)&m_listCtrl);
	}
}
UINT AFX_CDECL refreshProcessList(LPVOID param) {
	while (!g_exitFlag) {
		refreshRightNow(*(CListCtrl*)param);
		Sleep(m_refreshInterval);
	}
	return 0;
}
BOOL CCourseManageMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cMenu.LoadMenu(IDR_MENU_INSERTMAIN);
	this->SetMenu(&m_cMenu);
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCtrl.InsertColumn(0, _T("映像名称"), LVCFMT_LEFT, 160);
	m_listCtrl.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 45);
	m_listCtrl.InsertColumn(2, _T("线程数"), LVCFMT_LEFT, 50);
	m_listCtrl.InsertColumn(3, _T("安全等级"), LVCFMT_LEFT, 60);
	m_listCtrl.InsertColumn(4, _T("内存(当前工作集)"), LVCFMT_LEFT, 110);
	m_listCtrl.InsertColumn(5, _T("描述"), LVCFMT_LEFT, 300);
	InitProcessList();
	AfxBeginThread(refreshProcessList,(LPVOID)&m_listCtrl);
	checkMenuItem(ID_REFRESH_NORMAL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCourseManageMain::OnMenuExitClick()
{
	CDialog::OnCancel();
}


BOOL CCourseManageMain::DestroyWindow()
{
	return CDialogEx::DestroyWindow();
}


void CCourseManageMain::OnBnClickedButtonKillprocess()
{
	int selected = m_listCtrl.GetSelectionMark();
	if (selected != -1) {
		//kill process
		if (MessageBox(_T("确定要杀掉此进程吗?"),_T("进程管理系统"), MB_YESNO) == IDYES) {
			CString pidStr = m_listCtrl.GetItemText(selected,1);
			int result = KillProcess(_ttol(pidStr));
			if (result) {
				m_listCtrl.DeleteItem(selected);
			} else {
				MessageBox(_T("进程关闭失败!"), _T("进程管理系统"), MB_OK);
			}
		}
	}
	else {
		MessageBox(_T("还未选择进程!"), _T("进程管理系统"), MB_OK);
	}
}

int CCourseManageMain::KillProcess(DWORD pid)
{	
	HANDLE hProcess = OpenProcess(
		PROCESS_TERMINATE,
		FALSE,
		pid
	);
	BOOL result = FALSE;
	if (hProcess) {
		result = TerminateProcess(
			hProcess,
			0
		);
	}
	CloseHandle(hProcess);
	return result ? 1 : 0;
}


void CCourseManageMain::OnNMRClickCourseList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int selected = m_listCtrl.GetSelectionMark();
	if (selected != -1) {
		POINT point;
		HMENU hMenu, hSubMenu;
		GetCursorPos(&point);
		hMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU_RCLICK));
		hSubMenu = GetSubMenu(hMenu, 0);
		SetMenuDefaultItem(hSubMenu, -1, FALSE);
		SetForegroundWindow();
		TrackPopupMenu(hSubMenu, 0,
			point.x, point.y, 0, m_hWnd, NULL);
	}
	*pResult = 0;
}


void CCourseManageMain::InMenuKillProcess()
{
	int selected = m_listCtrl.GetSelectionMark();
	if (selected != -1 && MessageBox(_T("确定要杀掉此进程吗?"), _T("进程管理系统"), MB_YESNO) == IDYES) {
		CString pidStr = m_listCtrl.GetItemText(selected, 1);
		int result = KillProcess(_ttol(pidStr));
		if (result) {
			m_listCtrl.DeleteItem(selected);
			//OnRefreshRightNow();
		}
		else {
			MessageBox(_T("进程关闭失败!"), _T("进程管理系统"), MB_OK);
		}
	}
}


void CCourseManageMain::InMenuOpenFolder()
{
	int selected = m_listCtrl.GetSelectionMark();
	CString pidStr = m_listCtrl.GetItemText(selected, 1);
	DWORD processId = _ttol(pidStr);
	TCHAR str[400] = {0};
	DWORD size = 400;
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION,
		0, processId);
	if (NULL == hProcess)
		return;
	if (QueryFullProcessImageName(hProcess, 0, str, &size))
	{
		TCHAR newPath[400] = {0};
		TCHAR *p_index = _tcsrchr(str, '\\');
		if (p_index) {
			memcpy_s(newPath, 400, str, (int)p_index - (int)str);
			ShellExecute(
			NULL,
			_T("open"),
				newPath,
			NULL, NULL, SW_SHOWNORMAL);
		}
	}
	CloseHandle(hProcess);
}


void CCourseManageMain::OnRefreshRightNow()
{
	refreshRightNow(m_listCtrl);
}

void CCourseManageMain::InitProcessList()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << TEXT("CreateToolhelp32Snapshot (of processes)") << endl;
		return;
	}
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		cout << (TEXT("Process32First")) << endl; // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return;
	}
	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		uint64_t mem = 0;
		int retInt = GetMemoryUsage(&mem, pe32.th32ProcessID);
		if (retInt == -1) continue;
		CString info;
		GetInfomation(pe32.th32ProcessID, info);
		if (info == _T("")) {
			info = pe32.szExeFile;
		}
		int iLine = m_listCtrl.GetItemCount();
		m_listCtrl.InsertItem(iLine, pe32.szExeFile);
		CString processIdStr;
		processIdStr.Format(_T("%d"), pe32.th32ProcessID);
		m_listCtrl.SetItemText(iLine, 1, processIdStr);
		CString threadCount;
		threadCount.Format(_T("%d"), pe32.cntThreads);
		m_listCtrl.SetItemText(iLine, 2, threadCount);
		CString cpuStr(_T("查询中..."));
		m_listCtrl.SetItemText(iLine, 3, cpuStr);
		CString memoryStr;
		memoryStr.Format(_T("%dK"), mem / 1024);
		m_listCtrl.SetItemText(iLine, 4, memoryStr);
		m_listCtrl.SetItemText(iLine, 5, info);
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	//排序
	m_SortColum = 1;
	int count = m_listCtrl.GetItemCount();   //行数
	for (int i = 0; i<count; i++)
	{
		m_listCtrl.SetItemData(i, i);
	}
	m_listCtrl.SortItems(MyListCompar, (LPARAM)&m_listCtrl);
}


void CCourseManageMain::OnCreateNewProcess()
{
	CreateNewProcess createProcess;
	INT_PTR result = createProcess.DoModal();
	switch (result)
	{
	default:
		break;
	}
}
DWORD CCourseManageMain::m_SortColum = 0;
BOOL CCourseManageMain::m_bAs = TRUE;
int CCourseManageMain::MyListCompar(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*)lParamSort;
	int   iCompRes;
	CString  szComp1 = pListCtrl->GetItemText(lParam1, m_SortColum);
	CString  szComp2 = pListCtrl->GetItemText(lParam2, m_SortColum);
	switch (m_SortColum)
	{
	case(1):
	case(2):
		iCompRes = _ttoi(szComp1) <= _ttoi(szComp2) ? -1 : 1;
		break;
	case(4):
		szComp1.Remove('K');
		szComp2.Remove('K');
		iCompRes = _ttoi(szComp1) <= _ttoi(szComp2) ? -1 : 1;
		break;
	default:
		iCompRes = szComp1.Compare(szComp2);
		break;
	}
	if (m_bAs)
		return   iCompRes;
	else
		return   -iCompRes;
}


void CCourseManageMain::OnLvnColumnclickCourseList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_SortColum = pNMListView->iSubItem;
	m_bAs = !m_bAs;//升序还是降序
	int count = m_listCtrl.GetItemCount();   //行数
	for (int i = 0; i<count; i++)
	{
		m_listCtrl.SetItemData(i, i);
	}
	m_listCtrl.SortItems(MyListCompar, (LPARAM)&m_listCtrl);
	*pResult = 0;
}

BOOL CCourseManageMain::checkMenuItem(UINT itemId) {
	BOOL result = CheckMenuRadioItem(m_cMenu.m_hMenu, ID_REFRESH_HIGH, ID_REFRESH_PAUSE, itemId, MF_BYCOMMAND);
	switch (itemId)
	{
	case ID_REFRESH_HIGH:
		m_refreshInterval = 1000;
		break;
	case ID_REFRESH_NORMAL:
		m_refreshInterval = 2000;
		break;
	case ID_REFRESH_LOW:
		m_refreshInterval = 3000;
		break;
	case ID_REFRESH_PAUSE:
		g_exitFlag = true;
		break;
	default:
		break;
	}
	if (itemId != ID_REFRESH_PAUSE) {
		if (g_exitFlag) {
			AfxBeginThread(refreshProcessList, (LPVOID)&m_listCtrl);
			g_exitFlag = false;
		}
	}
	return result;
}

void CCourseManageMain::OnRefreshHigh()
{
	checkMenuItem(ID_REFRESH_HIGH);
}


void CCourseManageMain::OnRefreshNormal()
{
	checkMenuItem(ID_REFRESH_NORMAL);
}


void CCourseManageMain::OnRefreshLow()
{
	checkMenuItem(ID_REFRESH_LOW);
}


void CCourseManageMain::OnRefreshPause()
{
	checkMenuItem(ID_REFRESH_PAUSE);
}
