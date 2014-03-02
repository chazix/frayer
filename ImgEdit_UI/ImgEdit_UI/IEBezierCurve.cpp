#include "StdAfx.h"

#include "IEBezierCurve.h"

///////////////////////////////////////
/*!
	3���x�W�F�Ȑ��̍��W���X�g���擾
	@param[in] p0 �n�_
	@param[in] p1 ����_1
	@param[in] p2 ����_2
	@param[in] p3 �I�_
	@param[in] DIVIDE ����x
	@return ���W��vector�z��
*/
vector<POINT> IEBezierCurve::GetBezier3CurverPointList(LPPOINT p0, LPPOINT p1, LPPOINT p2, LPPOINT p3, int DIVIDE)
{
	vector<POINT> pList;
	for(int i=0; i < DIVIDE; i++){
		float u = i*1.0/(DIVIDE-1);
		float mP0 = (1-u)*(1-u)*(1-u);
		float mP1 = 3 * u * (1-u) * (1-u);
		float mP2 = 3 * u * u * (1-u);
		float mP3 = u * u * u;
		
		POINT pt;
		pt.x = p0->x*mP0 + p1->x*mP1 + p2->x*mP2 + p3->x*mP3;
		pt.y = p0->y*mP0 + p1->y*mP1 + p2->y*mP2 + p3->y*mP3;
		pList.push_back(pt);
	}

	return pList;
}

//////////////////////////////////////
/*!
	2���x�W�F�Ȑ��̍��W���X�g���擾
	@param[in] p0 �n�_
	@param[in] p1 ����_
	@param[in] p2 �I�_
	@parma[in] DIVIDE ����x
	@return ���W��vector�z��
*/
vector<POINT> IEBezierCurve::GetBezier2CurverPointList(LPPOINT p0, LPPOINT p1, LPPOINT p2, int DIVIDE)
{
	vector<POINT> pList;
	for(int i=0; i < DIVIDE; i++){
		float u = i*1.0/(DIVIDE-1);
		float mP0 = (1-u)*(1-u);
		float mP1 = 2 * u * (1-u);
		float mP2 = u * u;

		POINT pt;
		pt.x = p0->x*mP0 + p1->x*mP1 + p2->x*mP2;
		pt.y = p0->y*mP0 + p1->y*mP1 + p2->y*mP2;
		pList.push_back(pt);
	}

	return pList;
}