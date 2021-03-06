#pragma once
#include "afxcmn.h"
#include "GridCtrl_src\GridCtrl.h"
#include "Characteristics.h"
#include "ShowSelectDlg.h"
#include "ShowCheckboxList.h"
#include <vector>
// CPeDialog 对话框
void makeItemToCtrl(CGridCtrl &gridCtrl, int row, int col, CString content);
class CPeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPeDialog)

public:
	CPeDialog(CWnd* pParent = NULL);   // 标准构造函数
	CPeDialog(CString &exePath,CWnd* pParent = NULL);
	virtual ~CPeDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEDIALOG };
#endif
//PE项的页面个数
#define ITEM_SIZE 12
/* 各个项页面的下标 */
#define DOS_HEADER 0
#define NT_HEADER 1
#define FILE_HEADER 2
#define OPTIONAL_HEADER 3
#define DATA_DIRECTORY 4
#define SECTION_HEADER 5

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	virtual BOOL OnInitDialog();
	CString m_exeFullPath;
	CGridCtrl *m_peDetailPages[ITEM_SIZE];
	CGridCtrl m_gridCtrl;
	void GridCtrlInit_up();
	void GridCtrlInit_down();
	void hidePrevSel();
	CGridCtrl m_gridCtrlDown;
	HTREEITEM m_hRoot;
	HTREEITEM m_dosHeader;
	HTREEITEM m_ntHeaders;
	HTREEITEM m_fileHeader;
	HTREEITEM m_optionalHeader;
	HTREEITEM m_dataDirectories;
	HTREEITEM m_sectionHeaders;
	afx_msg void OnTvnSelchangedTreeHeaders(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCharacteristicsClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnSubsystemClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnSectionCtrlClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	CGridCtrl m_dosHeaderCtrl;
	CGridCtrl m_ntHeadersCtrl;
	CGridCtrl m_fileHeaderCtrl;
	CGridCtrl m_optionalHeaderCtrl;
	CGridCtrl m_dataDirectoriesCtrl;
	CGridCtrl m_sectionHeadersCtrl;
	void parsePe();
	int parseDosHeader();
	void parseNtHeader(int &ntOffset, int &sectionSize);
	void parseSection(int &ntOffset, int &sectionSize);
	int validPeVerify();
	void parseOptionalHeader32(int offset, IMAGE_OPTIONAL_HEADER32 &image32);
	void parseOptionalHeader64(int offset, IMAGE_OPTIONAL_HEADER64 &image64);
	int currentSel;
	bool isInitResize;
	IMAGE_SECTION_HEADER *m_sectionAry;
	afx_msg void OnNcDestroy();
	void moveWindowCenterAndSetsize(int width, int height);
	void dynamicCalcItemSize(int width, int height);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	BOOL queryFileProperty(CString &queryStr, DWORD fileVersionSize,char *lpData, LPVOID  *lplpBuffer);
};
