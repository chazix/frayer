#include "stdafx.h"

#include "LayerSynthesizerMG.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


LSYNTH_DATA_VEC LayerSynthesizerMG::m_LSynthVec;

void LayerSynthesizerMG::Init()
{
	//
	LSYNTH_DATA *lsd;
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('n','o','r','m');
	lsd->disp_name = "�ʏ�";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('a','m','s','k');
	lsd->disp_name = "�A���t�@�}�X�N";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('m','u','l',' ');
	lsd->disp_name = "��Z";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('b','u','r','n');
	lsd->disp_name = "�Ă�����";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('d','a','r','k');
	lsd->disp_name = "��r�i�Áj";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('d','i','f','f');
	lsd->disp_name = "���̐�Βl";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('d','o','d','g');
	lsd->disp_name = "�����Ă�";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('h','L','i','t');
	lsd->disp_name = "�n�[�h���C�g";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('l','i','t','e');
	lsd->disp_name = "��r�i���j";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('o','v','e','r');
	lsd->disp_name = "�I�[�o�[���C";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('s','c','r','n');
	lsd->disp_name = "�X�N���[��";
	m_LSynthVec.push_back(lsd);

	//
	lsd = new LSYNTH_DATA;
	lsd->fourcc = IE_LAYER_SYNTH_FOURCC('s','L','i','t');
	lsd->disp_name = "�\�t�g���C�g";
	m_LSynthVec.push_back(lsd);
}

void LayerSynthesizerMG::End()
{
	LSYNTH_DATA_VEC::iterator itr = m_LSynthVec.begin();
	for(; itr != m_LSynthVec.end(); itr++){
		if(*itr){
			delete (*itr);
		}
	}
	m_LSynthVec.clear();
}

void LayerSynthesizerMG::SetDrawModeComboBox(HWND hCombo)
{
	//
	LSYNTH_DATA_VEC::const_iterator itr = m_LSynthVec.begin();
	for(; itr != m_LSynthVec.end(); itr++){
		//�R���{�{�b�N�X�ɒǉ�
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)(*itr)->disp_name.c_str());
	}

	return;
}

void LayerSynthesizerMG::GetLayerSynthDisplayName(uint32_t fourcc, char* dst)
{
	LSYNTH_DATA_VEC::const_iterator itr = m_LSynthVec.begin();
	for(; itr != m_LSynthVec.end(); itr++){
		if((*itr)->fourcc == fourcc){
			strcpy(dst, (*itr)->disp_name.c_str());
			return;
		}
	}

	strcpy(dst, "");
	return;
}

uint32_t LayerSynthesizerMG::GetLayerSynthFourCC(int index)
{
	assert(0 <= index && index < m_LSynthVec.size());
	return m_LSynthVec[index]->fourcc;
}

int LayerSynthesizerMG::GetLayerSynthIndex(uint32_t fourcc)
{
	//
	int index =0;
	LSYNTH_DATA_VEC::const_iterator itr = m_LSynthVec.begin();
	for(; itr != m_LSynthVec.end(); itr++){
		if((*itr)->fourcc == fourcc){
			return index;
		}
		index++;
	}
	return -1;
}

