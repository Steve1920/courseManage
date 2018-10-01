// CourseManageMain.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "CourseManageMain.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CCourseManageMain, CDialogEx)

CCourseManageMain::CCourseManageMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COURSEMANAGE_MAIN, pParent)
{

}

BOOL CCourseManageMain::ShowProcessList(CListCtrl &pListCtrl, vector<ProcessInfo*> &processVec)
{
	try
	{
		processIter processIter;
		if (processVec.size() > 0) {
			for (processIter = processVec.begin(); processIter != processVec.end(); ++processIter)
			{
				ProcessInfo tmpProcessInfo = *(*processIter);
				int iLine = pListCtrl.GetItemCount();
				pListCtrl.InsertItem(iLine, tmpProcessInfo.getImageName());
				CString processIdStr;
				processIdStr.Format(_T("%d"), tmpProcessInfo.getPid());
				pListCtrl.SetItemText(iLine, 1, processIdStr);
				CString threadCount;
				threadCount.Format(_T("%d"), tmpProcessInfo.getThreadNums());
				pListCtrl.SetItemText(iLine, 2, threadCount);
				/*CString cpuStr;
				cpuStr.Format(_T("%d"), tmpProcessInfo.getCpu());
				pListCtrl.SetItemText(iLine, 3, cpuStr);*/
				CString memoryStr;
				memoryStr.Format(_T("%dK"), tmpProcessInfo.getMemory() / 1024);
				pListCtrl.SetItemText(iLine, 3, memoryStr);
				pListCtrl.SetItemText(iLine, 4, tmpProcessInfo.getDes());
			}
		}

	}
	catch (const std::exception&)
	{
		return(FALSE);
	}
	return(TRUE);
}

BOOL CCourseManageMain::GetProcessInfoVector(vector<ProcessInfo*>& processVec)
{

	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << TEXT("CreateToolhelp32Snapshot (of processes)") << endl;
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		cout << (TEXT("Process32First")) << endl; // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
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
		/*CPUusage usg(pe32.th32ProcessID);
		float cpu = usg.get_cpu_usage();*/
		ProcessInfo *pi = new ProcessInfo(pe32.szExeFile, pe32.th32ProcessID,
			pe32.cntThreads, 0, mem, info);
		processVec.push_back(pi);
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return(TRUE);
}

BOOL CCourseManageMain::DestoryProcessInfoVector(vector<ProcessInfo*>& processVec)
{
	try
	{
		processIter processIter;
		ProcessInfo *tmpProcessInfo = NULL;
		if (processVec.size() > 0) {
			for (processIter = processVec.begin(); processIter != processVec.end(); ++processIter)
			{
				if (tmpProcessInfo) {
					delete tmpProcessInfo;
				}
				tmpProcessInfo = *processIter;
			}
			if (tmpProcessInfo) {
				delete tmpProcessInfo;
			}
		}

	}
	catch (const std::exception&)
	{
		return(FALSE);
	}
	return(TRUE);
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
END_MESSAGE_MAP()


// CCourseManageMain 消息处理程序

BOOL CCourseManageMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_cMenu.LoadMenuW(IDR_MENU_INSERTMAIN);
	this->SetMenu(&m_cMenu);
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCtrl.InsertColumn(0, _T("映像名称"), LVCFMT_LEFT, 160);
	m_listCtrl.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 45);
	m_listCtrl.InsertColumn(2, _T("线程数"), LVCFMT_LEFT, 50);
	//m_listCtrl.InsertColumn(3, _T("CPU"), LVCFMT_LEFT, 45);
	m_listCtrl.InsertColumn(3, _T("内存(当前工作集)"), LVCFMT_LEFT, 110);
	m_listCtrl.InsertColumn(4, _T("描述"), LVCFMT_LEFT, 300);
	processVector processVec;
	GetProcessInfoVector(processVec);
	sort(processVec.begin(), processVec.end(), ProcessInfo());
	ShowProcessList(m_listCtrl, processVec);
	DestoryProcessInfoVector(processVec);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCourseManageMain::OnMenuExitClick()
{
	MessageBox(L"内容", L"标题", NULL);
}


BOOL CCourseManageMain::DestroyWindow()
{
	return CDialogEx::DestroyWindow();
}
int CCourseManageMain::GetMemoryUsage(uint64_t * mem, DWORD processId)
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

int CCourseManageMain::GetInfomation(DWORD processId,CString &info)
{

	TCHAR str[400];
	DWORD size = 400;
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION,
		0, processId);
	if (NULL == hProcess)
		return -1;
	int resultInt = 0;
	if (QueryFullProcessImageName(hProcess, 0,str,&size))
	{
		DWORD fileVersionSize =  GetFileVersionInfoSize(str,NULL);
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
			delete [] lpData;
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


void CCourseManageMain::OnBnClickedButtonKillprocess()
{
	int selected = m_listCtrl.GetSelectionMark();
	if (selected != -1) {
		//kill process
		if (MessageBox(_T("确定要杀掉此进程吗?"),_T("进程管理系统"), MB_YESNO) == IDYES) {
			CString pidStr = m_listCtrl.GetItemText(selected,1);
			KillProcess(_ttol(pidStr));
		}
	}
}

int CCourseManageMain::KillProcess(DWORD pid)
{	
	HANDLE hProcess = OpenProcess(
		PROCESS_TERMINATE,
		FALSE,
		pid
	);
	if (hProcess) {
		TerminateProcess(
			hProcess,
			0
		);
	}
	CloseHandle(hProcess);
	return 0;
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
		//kill process
		/*if (MessageBox(_T("确定要杀掉此进程吗?"), _T("进程管理系统"), MB_YESNO) == IDYES) {
			CString pidStr = m_listCtrl.GetItemText(selected, 1);
			KillProcess(_ttol(pidStr));
		}*/
	}
	*pResult = 0;
}
