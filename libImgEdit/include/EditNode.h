#ifndef _EDIT_NODE_H_
#define _EDIT_NODE_H_

#include "LibIEDllHeader.h"


#include <list>

enum NODE_FLAG{
	EDIT_DRAW = 1,
	MOVE_IMG,
};

class _EXPORTCPP IEditNode
{
public:
	IEditNode(){};
	virtual ~IEditNode(){};

	NODE_FLAG node_flag;	//
};

/*!
	@brief �摜�ҏW���A�L��
	edit_img,undo_img,write_map�͕ҏW���C���[�Ɠ����T�C�Y�����ʒu�ɔz�u�����B
*/
class _EXPORTCPP EditNode : public IEditNode
{
public:
	EditNode() {
		node_flag = EDIT_DRAW;
	}

	IplImageExt edit_img;		//���C���[�ɉ��Z����摜
	IplImageExt undo_img;		//���ɖ߂��̂Ɏg�p����摜
	IplImageExt write_map;		//undo���������񂾂��ǂ���
	RECT blt_rect;				//�m�[�h�̎g�p����͈� �X�N���[�����W
	RECT node_rect;				//�m�[�h�̈ʒu �X�N���[�����W
	uint32_t fourcc;			//�m�[�h�̏d�˕�
	bool isBakedWriteMap;		//write_map���m�肵�����ǂ���
};

class _EXPORTCPP MoveNode : public IEditNode
{
public:
	MoveNode() {
		node_flag = MOVE_IMG;
	}

	int move_x; //X�������ړ���
	int move_y; //Y�������ړ���
};

//
enum EDIT_FLAG{
	ADD_EDIT = 1,
	SUB_EDIT,
};

class _EXPORTCPP EditQueueNode
{
public:
	IEditNode* pNode;
	EDIT_FLAG flag;
};

typedef std::list<EditQueueNode> EditQueue;

#endif //_EDIT_NODE_H_