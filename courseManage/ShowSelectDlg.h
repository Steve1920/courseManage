#pragma once
#include "afxwin.h"


// CShowSelectDlg 对话框

class CShowSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowSelectDlg)

public:
	CShowSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	CShowSelectDlg(LPVOID pStrArry,size_t size,int selIndex,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowSelectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOWSELECT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_select;
	LPVOID m_pStrArry; 
	size_t m_size;
	int m_selIndex;
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
