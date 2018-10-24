#pragma once 
#include "afxcmn.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <windows.h>  
#include <psapi.h> 
#include "processInfo.h"
#include <algorithm>
#include <winver.h>
#include <memory.h>
#include "CreateNewProcess.h"
#include "AboutBox.h"
#include <map>
#pragma comment(lib, "version.lib")
using namespace std;
using std::cout;
typedef vector<ProcessInfo*>::iterator processIter;
typedef vector<ProcessInfo*> processVector;
// CCourseManageMain 对话框

class CCourseManageMain : public CDialogEx
{
	DECLARE_DYNAMIC(CCourseManageMain)

public:
	CCourseManageMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCourseManageMain();
	CMenu m_cMenu;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COURSEMANAGE_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuExitClick();
	CListCtrl m_listCtrl;
	int m_selectLineNumber = -1;
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonKillprocess();
	int KillProcess(DWORD pid);
	afx_msg void OnNMRClickCourseList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void InMenuKillProcess();
	afx_msg void InMenuOpenFolder();
	afx_msg void OnRefreshRightNow();
	void InitProcessList();
	afx_msg void OnCreateNewProcess();
	static DWORD m_SortColum;
	static BOOL m_bAs;
	static int CALLBACK MyListCompar(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	afx_msg void OnLvnColumnclickCourseList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRefreshHigh();
	afx_msg void OnRefreshNormal();
	afx_msg void OnRefreshLow();
	afx_msg void OnRefreshPause();
	BOOL checkMenuItem(UINT itemId);
	afx_msg void OnAboutbox();
};