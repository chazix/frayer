#ifndef _IEMENUMG_H_
#define _IEMENUMG_H_

#include "IEMenuItemInfo.h"

class _EXPORTCPP IEMenuMG
{
public:
	////////////////////////////////////////////
	/*!
		���j���[���쐬����B
		@return ���j���[�n���h��
	*/
	static IEMENU CreateIEMenu();

	//////////////////////////////////////////////
	/*!
	
	*/
	static IEMENU CreateIEPopupMenu();

	/////////////////////////////////////////////
	/*!
		���j���[��j������B
		@param[in] ieMenu �j�����郁�j���[
	*/
	static void DestroyIEMenu(IEMENU ieMenu);

	////////////////////////////////////////////
	/*!
		���j���[�ɃA�C�e����ǉ�����B
		@param[in] ieMenu ���j���[�n���h��
		@param[in] uItem ���j���[�C���f�b�N�X�܂���ID
		@param[in] fByPosition FALSE �Ȃ� uItem �� ID�A����ȊO�Ȃ�C���f�b�N�X�ł���Ɣ��f���܂�
		@param[in] lpmii �ǉ����郁�j���[�A�C�e��
	*/
	static BOOL InsertIEMenuItem(IEMENU ieMenu, UINT uItem,
						BOOL fByPosition,  IEMenuItemInfo *lpmii);

	/////////////////////////////////////////////
	/*!
		�|�b�v�A�b�v���j���[��\��������

	*/
	static BOOL TrackPopupIEMenu(IEMENU ieMenu, UINT uFlags, int x, int y,
		int nReserved, HWND hWnd, CONST RECT *prcRect);

	static BOOL TrackPopupIEMenuEx(IEMENU ieMenu, UINT uFlags, int x, int y,
		HWND hWnd, LPTPMPARAMS ptpm);
};

#endif