#pragma once
#include "afxcmn.h"
#include <vector>
#include <algorithm>
using namespace std;
using std::vector;

// CShowCheckboxList 对话框

class CShowCheckboxList : public CDialogEx
{
	DECLARE_DYNAMIC(CShowCheckboxList)

public:
	CShowCheckboxList(CWnd* pParent = NULL);   // 标准构造函数
	CShowCheckboxList(vector<CString> &strVec, vector<int> &selVec, CString textName, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowCheckboxList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLLCHARACTERISTICS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_checkboxListCtrl;
	vector<CString> m_strVec;
	vector<int> m_selVec;
	CString m_textName;
};
