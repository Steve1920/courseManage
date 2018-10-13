#pragma once


// CreateNewProcess 对话框

class CreateNewProcess : public CDialog
{
	DECLARE_DYNAMIC(CreateNewProcess)

public:
	CreateNewProcess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CreateNewProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATE_NEW_RPOCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
