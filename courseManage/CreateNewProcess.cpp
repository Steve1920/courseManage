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
	, m_exePath(_T(""))
{

}

CreateNewProcess::~CreateNewProcess()
{
}

void CreateNewProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PROCESS_PATH, m_exePath);
}


BEGIN_MESSAGE_MAP(CreateNewProcess, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, &CreateNewProcess::OnBnClickedButtonView)
	ON_BN_CLICKED(IDOK_CREATE_NEW_PROCESS, &CreateNewProcess::OnBnClickedOk)
	ON_EN_CHANGE(IDC_PROCESS_PATH, &CreateNewProcess::OnEnChangeProcessPath)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


static TCHAR BASED_CODE szFilter[] = _T("程序 (*.exe;*.pif;*.com;*.bat;*.cmd)|*.exe|*.pif|*.com|*.bat|*.cmd")
 _T("All Files (*.*)|*.*||");

void CreateNewProcess::OnBnClickedButtonView()
{
	CFileDialog fileDlg(TRUE, _T("") , NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if (fileDlg.DoModal() != IDOK) {
		return;
	}
	m_exePath = fileDlg.GetPathName();
	UpdateData(FALSE);
	CWnd * pOk = GetDlgItem(IDOK_CREATE_NEW_PROCESS);
	pOk->EnableWindow(m_exePath.GetLength());
}


void CreateNewProcess::OnBnClickedOk()
{
	HINSTANCE retInt = ShellExecute(
		NULL,
		_T("open"),
		m_exePath,
		NULL, NULL, SW_SHOW);
	switch ((int)retInt)
	{
	case 0:
		MessageBox(_T("操作系统内存或资源不足。"), _T("错误提示"), MB_OK);
		break;
	case ERROR_PATH_NOT_FOUND:
		MessageBox(_T("找不到指定的路径。"), _T("错误提示"), MB_OK);
		break;
	case ERROR_BAD_FORMAT:
		MessageBox(_T(".exe文件无效（非Win32.exe或.exe映像中的错误）。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_ACCESSDENIED:
		MessageBox(_T("操作系统拒绝访问指定的文件。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_ASSOCINCOMPLETE:
		MessageBox(_T("文件名关联不完整或无效。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_DDEBUSY:
		MessageBox(_T("由于正在处理其他DDE事务，因此无法完成DDE事务。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_DDEFAIL:
		MessageBox(_T("DDE事务失败。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_DDETIMEOUT:
		MessageBox(_T("由于请求超时，无法完成DDE事务。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_DLLNOTFOUND:
		MessageBox(_T("找不到指定的DLL。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_FNF:
		MessageBox(_T("指定的文件或文件夹未找到。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_NOASSOC:
		MessageBox(_T("没有与给定文件扩展名关联的应用程序。如果您尝试打印不可打印的文件，也会返回此错误。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_OOM:
		MessageBox(_T("没有足够的内存来完成操作。"), _T("错误提示"), MB_OK);
		break;
	case SE_ERR_SHARE:
		MessageBox(_T("发生了共享违规。"), _T("错误提示"), MB_OK);
		break;
	default:
		CDialog::OnOK();
		break;
	}
}


void CreateNewProcess::OnEnChangeProcessPath()
{
	GetDlgItemText(IDC_PROCESS_PATH, m_exePath);
	CWnd * pOk = GetDlgItem(IDOK_CREATE_NEW_PROCESS);
	pOk->EnableWindow(m_exePath.GetLength());
}


void CreateNewProcess::OnDropFiles(HDROP hDropInfo)
{
	UINT count;
	TCHAR filePath[MAX_PATH] = { 0 };
	count = DragQueryFile(hDropInfo, 0, filePath, sizeof(filePath));
	if (count == 1) {
		DragQueryFile(hDropInfo, 0XFFFFFFFF, NULL, 0);
		m_exePath = filePath;
		UpdateData(FALSE);
		DragFinish(hDropInfo);
	}
	else
	{
		for (UINT i = 0; i<count; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
			m_exePath = filePath;
		}
		UpdateData(FALSE);
		DragFinish(hDropInfo);
	}
	CDialog::OnDropFiles(hDropInfo);
	CWnd * pOk = GetDlgItem(IDOK_CREATE_NEW_PROCESS);
	pOk->EnableWindow(m_exePath.GetLength());
}
