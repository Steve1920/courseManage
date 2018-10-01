#pragma once


// CCourseManageRegist 对话框

class CCourseManageRegist : public CDialogEx
{
	DECLARE_DYNAMIC(CCourseManageRegist)

public:
	CCourseManageRegist(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCourseManageRegist();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COURSEMANAGE_REGIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
