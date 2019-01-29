
// RemoteDlg.h : ͷ�ļ�
//

#pragma once
#include "afxshelltreectrl.h"
#include "afxshelllistctrl.h"
#include "afxcmn.h"


// CRemoteDlg �Ի���
class CRemoteDlg : public CDialogEx
{
// ����
public:
	CRemoteDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REMOTE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCShellTreeCtrl m_tree;
	CMFCShellListCtrl m_shelllist;
//	afx_msg void OnNMDblclkMfcshelllist3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickMfcshelllist3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListCtrl m_list;
	afx_msg void OnClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
};
