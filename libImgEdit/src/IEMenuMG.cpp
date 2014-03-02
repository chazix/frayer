#include "stdafx.h"

#include "IEMenuMG.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


////////////////////////////////////////////
/*!
	���j���[���쐬����B
	@return ���j���[�n���h��
*/
IEMENU IEMenuMG::CreateIEMenu()
{
	return CreateMenu();
}

IEMENU IEMenuMG::CreateIEPopupMenu()
{
	return CreatePopupMenu();
}

/////////////////////////////////////////////
/*!
	���j���[��j������B
	@param[in] ieMenu �j�����郁�j���[
*/
void IEMenuMG::DestroyIEMenu(IEMENU ieMenu)
{
	DestroyMenu(ieMenu);
}

////////////////////////////////////////////
/*!
	���j���[�ɃA�C�e����ǉ�����B
	@param[in] ieMenu ���j���[�n���h��
	@param[in] uItem ���j���[�C���f�b�N�X�܂���ID
	@param[in] fByPosition FALSE �Ȃ� uItem �� ID�A����ȊO�Ȃ�C���f�b�N�X�ł���Ɣ��f���܂�
	@param[in] lpmii �ǉ����郁�j���[�A�C�e��
*/
BOOL IEMenuMG::InsertIEMenuItem(IEMENU ieMenu, UINT uItem, BOOL fByPosition, IEMenuItemInfo *lpmii)
{
	return InsertMenuItem(ieMenu, uItem, fByPosition, lpmii->GetIEMenuItemInfo());
}

/////////////////////////////////////////////
/*!
	�|�b�v�A�b�v���j���[��\��������

*/
BOOL IEMenuMG::TrackPopupIEMenu(IEMENU ieMenu, UINT uFlags, int x, int y,
								int nReserved, HWND hWnd, CONST RECT *prcRect)
{
	return TrackPopupMenu(ieMenu, uFlags, x, y, nReserved, hWnd, prcRect);
}