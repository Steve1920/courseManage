// CourseManageRegist.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "CourseManageRegist.h"
#include "afxdialogex.h"


// CCourseManageRegist 对话框

IMPLEMENT_DYNAMIC(CCourseManageRegist, CDialogEx)

CCourseManageRegist::CCourseManageRegist(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COURSEMANAGE_REGIST, pParent)
{

}

CCourseManageRegist::~CCourseManageRegist()
{
}

void CCourseManageRegist::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCourseManageRegist, CDialogEx)
END_MESSAGE_MAP()


// CCourseManageRegist 消息处理程序
