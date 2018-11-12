#pragma once
#include "afxcmn.h"


// CCharacteristics 对话框

class CCharacteristics : public CDialogEx
{
	DECLARE_DYNAMIC(CCharacteristics)

public:
	CCharacteristics(CWnd* pParent = NULL);   // 标准构造函数
	CCharacteristics(int value,CWnd* pParent = NULL);
	virtual ~CCharacteristics();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARACTERISTICS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_valueList;
	int m_cValue;
	virtual BOOL OnInitDialog();
};
