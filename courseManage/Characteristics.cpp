// Characteristics.cpp : 实现文件
//

#include "stdafx.h"
#include "courseManage.h"
#include "Characteristics.h"
#include "afxdialogex.h"


// CCharacteristics 对话框

IMPLEMENT_DYNAMIC(CCharacteristics, CDialogEx)

CCharacteristics::CCharacteristics(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHARACTERISTICS, pParent)
{

}

CCharacteristics::CCharacteristics(int value, CWnd * pParent) : CDialogEx(IDD_CHARACTERISTICS, pParent)
{
	m_cValue = value;
}

CCharacteristics::~CCharacteristics()
{
}

void CCharacteristics::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_C_LIST, m_valueList);
}


BEGIN_MESSAGE_MAP(CCharacteristics, CDialogEx)
END_MESSAGE_MAP()


// CCharacteristics 消息处理程序


BOOL CCharacteristics::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_valueList.SetExtendedStyle(WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE |LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EDITLABELS);
	int count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T("asdasdadssad"));
	m_valueList.SetItemText(count,0,_T("asdasdqweqwe"));
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T("asdasdadssad"));
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T("asdasdadssad"));
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T("asdasdadssad"));
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count, _T("asdasdadssad"));
	count = m_valueList.GetItemCount();
	m_valueList.InsertItem(count,_T("asdasdadssad"));
	UpdateData(FALSE);
	//TO_DO 根据整数值计算文件特征
	/* java解析辅助代码
	IMAGE_FILE_RELOCS_STRIPPED           0x0001//Relocation info stripped from file.
	IMAGE_FILE_EXECUTABLE_IMAGE          0x0002//File is executable  (i.e. no unresolved external references).
	IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004//Line nunbers stripped from file.
	IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008//Local symbols stripped from file.
	IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010//Aggressively trim working set
	IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020//App can handle >2gb addresses
	IMAGE_FILE_BYTES_REVERSED_LO         0x0080//Bytes of machine word are reversed.
	IMAGE_FILE_32BIT_MACHINE             0x0100//32 bit word machine.
	IMAGE_FILE_DEBUG_STRIPPED            0x0200//Debugging info stripped from file in .DBG file
	IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400//If Image is on removable media, copy and run from the swap file.
	IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800//If Image is on Net, copy and run from the swap file.
	IMAGE_FILE_SYSTEM                    0x1000//System File.
	IMAGE_FILE_DLL                       0x2000//File is a DLL.
	IMAGE_FILE_UP_SYSTEM_ONLY            0x4000//File should only be run on a UP machine
	IMAGE_FILE_BYTES_REVERSED_HI         0x8000//Bytes of machine word are reversed.
	BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(new File("pe_struct1.txt"))));
	String temp = null;
	while((temp = br.readLine()) != null){
	String info = temp.split("//")[1];
	String value = temp.split("[ ]+")[0];
	System.out.println(info + ";" + value);
	}
	br.close();
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
