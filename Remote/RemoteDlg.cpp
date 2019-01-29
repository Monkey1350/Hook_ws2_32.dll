
// RemoteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Remote.h"
#include "RemoteDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <iostream>
#include<fstream>
#include <psapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteDlg �Ի���



CRemoteDlg::CRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_tree);
	DDX_Control(pDX, IDC_MFCSHELLLIST3, m_shelllist);
	DDX_Control(pDX, IDC_LIST2, m_list);
}

BEGIN_MESSAGE_MAP(CRemoteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_NOTIFY(NM_DBLCLK, IDC_MFCSHELLLIST3, &CRemoteDlg::OnNMDblclkMfcshelllist3)
ON_NOTIFY(NM_CLICK, IDC_MFCSHELLLIST3, &CRemoteDlg::OnClickMfcshelllist3)
ON_BN_CLICKED(IDOK, &CRemoteDlg::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &CRemoteDlg::OnBnClickedCancel)
ON_NOTIFY(NM_CLICK, IDC_LIST2, &CRemoteDlg::OnClickList2)
ON_BN_CLICKED(IDC_BUTTON1, &CRemoteDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRemoteDlg ��Ϣ�������

BOOL CRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	DWORD styles = m_list.GetExtendedStyle();
	m_list.SetExtendedStyle(styles | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.DeleteAllItems();//�����
	//set the list's titles
	TCHAR rgtsz[][6] = { _T("����"), _T("PID") };
	LV_COLUMN lvcolumn;
	CRect rect;
	m_list.GetWindowRect(&rect);//��ȡlist�Ŀ��
	for (int i = 0; i < 2; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = rgtsz[i];
		lvcolumn.iSubItem = i;
		lvcolumn.iOrder = i;
		lvcolumn.cx = rect.Width() / 2;
		m_list.InsertColumn(i, &lvcolumn);
	}
	m_tree.Expand(m_tree.GetRootItem(), TVE_EXPAND);
	m_tree.SetRelatedList(&m_shelllist);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRemoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRemoteDlg::OnClickMfcshelllist3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString FilePath;

	for (int i = 0; i < m_shelllist.GetItemCount();i++)
	{
		if (m_shelllist.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED){
			m_shelllist.GetItemPath(FilePath,i);
			GetDlgItem(IDC_EDIT1)->SetWindowTextW(FilePath);
			break;
		}
	}
	*pResult = 0;
}
void CRemoteDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	setlocale(LC_ALL, "chs");
	// 1. ʹ�ÿ�������������
	// 1.1 �������̿���
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) {
		return;
	}
	//typedef struct tagPROCESSENTRY32W
	//{
	//    DWORD   dwSize;                 // �ṹ���С
	//    DWORD   cntUsage;               /// ��ʹ��
	//    DWORD   th32ProcessID;          // this process
	//    ULONG_PTR th32DefaultHeapID;    /// ��ʹ��
	//    DWORD   th32ModuleID;           /// ��ʹ��
	//    DWORD   cntThreads;             // ���̵��̸߳���
	//    DWORD   th32ParentProcessID;    // ������ID
	//    LONG    pcPriClassBase;         // �������ȼ�
	//    DWORD   dwFlags;                /// ��ʹ��
	//    WCHAR   szExeFile[ MAX_PATH ];  // ������
	//} PROCESSENTRY32W;
	PROCESSENTRY32 processInfo;
	// ��ʼ���ṹ��Ľṹ���ֽ��ֶ�
	processInfo.dwSize = sizeof(processInfo);
	// ��ȡ��һ������
	Process32First(hProcSnap, &processInfo);
	int i = 0;
	BOOL bTrue = FALSE;
	BOOL bRet = FALSE;
	TCHAR path[MAX_PATH];
	DWORD dwPathLengh = MAX_PATH;
	HANDLE hProc;
	CString PID = L"";
	do
	{
		hProc = OpenProcess(PROCESS_QUERY_INFORMATION,
			FALSE, processInfo.th32ProcessID);

		PID.Format(L"%d", processInfo.th32ProcessID);
		m_list.InsertItem(i,L"");
		m_list.SetItemText(i, 0, processInfo.szExeFile);
		m_list.SetItemText(i, 1, PID);
		i++;
	} while (Process32Next(hProcSnap, &processInfo));
}


void CRemoteDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString PID,PATH;
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(PATH);
	GetDlgItem(IDC_EDIT2)->GetWindowTextW(PID);
	DWORD pid = _ttoi(PID);
	TCHAR* path = PATH.GetBuffer();
	HANDLE hProcess = OpenProcess(
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		NULL, pid);
	if (!hProcess)
		return ;
	//C:\\tel\\MyRecv.dll
	int nLen = sizeof(WCHAR)*(wcslen(path) + 1);
	LPVOID pBuf = VirtualAllocEx(hProcess, NULL, nLen, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!pBuf)
		return ;
	DWORD dwWrite = 0;
	if (!WriteProcessMemory(hProcess, pBuf, path, nLen, &dwWrite))
		return ;
	// LoadLibrary
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL,
		(LPTHREAD_START_ROUTINE)LoadLibraryW, pBuf, 0, 0);
	WaitForSingleObject(hRemoteThread, -1);
	if (!hRemoteThread)
		return ;
	CloseHandle(hRemoteThread);
	VirtualFreeEx(hProcess, pBuf, 0, MEM_FREE);
	MessageBoxA(0, "ע��ɹ�", "", 0);
}


void CRemoteDlg::OnClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString PID;

	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED){
			PID = m_list.GetItemText(i,1);
			GetDlgItem(IDC_EDIT2)->SetWindowTextW(PID);
			break;
		}
	}
	*pResult = 0;
}


void CRemoteDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	exit(0);
}
