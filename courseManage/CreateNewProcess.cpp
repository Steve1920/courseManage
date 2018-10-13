// CreateNewProcess.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "CreateNewProcess.h"
#include "afxdialogex.h"


// CreateNewProcess 对话框

IMPLEMENT_DYNAMIC(CreateNewProcess, CDialog)

CreateNewProcess::CreateNewProcess(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_CREATE_NEW_RPOCESS, pParent)
{

}

CreateNewProcess::~CreateNewProcess()
{
}

void CreateNewProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CreateNewProcess, CDialog)
END_MESSAGE_MAP()


// CreateNewProcess 消息处理程序
