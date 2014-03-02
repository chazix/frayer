#ifndef _IMGLAYERGROUP_H_
#define _IMGLAYERGROUP_H_

#include "IImgLayer.h"

typedef std::vector<IImgLayer_Ptr> IImgLayer_Vec;

_EXPORTCPP ImgLayerGroup_Ptr CreateIEImgLayerGroup(ImgFile_Ptr parent_file);
//_EXPORT void ReleaseIEImgLayerGroup(ImgLayerGroup_Ptr* ppLayerGroup);

//
class _EXPORTCPP ImgLayerGroup :
	public IImgLayer,
	public std::enable_shared_from_this<ImgLayerGroup>
{
public:
	ImgLayerGroup(ImgFile_Ptr parent_file)
		:IImgLayer(parent_file)
	{
		m_NumOfChildLayers = 0;
		m_MaxLayerIndex = 0;
		m_IsNeedCountChildLayers = false;
		m_IsNeedCountMaxLayerIndex = false;
		m_IsOpen = true;
	}
	~ImgLayerGroup();

	inline int GetLayerType(){
		return IE_LAYER_TYPE::GROUP_LAYER;
	}

	bool Update(const LPRECT enable_lprc, LPRECT updated_lprc);

	void ClearUpdateData();
	void LockUpdateData();
	void UnlockUpdateData();

	void PushUpdateDataToAllLayer(const LPUPDATE_DATA data);

	///////////////////////////////////////
	/*!
		add layer
		@param[in] add_layer �ǉ����郌�C���[�̃|�C���^
		@param[in] index �ǉ���C���f�b�N�X
	*/
	void AddLayer(IImgLayer_Ptr add_layer); //�Ō���ɒǉ�
	void AddLayer(IImgLayer_Ptr add_layer, int index);

	///////////////////////////////////////
	/*!
		remove layer
		@param[in,out] delete_layer ��菜�����C���[�̃|�C���^
		@preturn if find remove layer return true,
	*/
	bool RemoveLayer(IImgLayer_Ptr remove_layer);

	///////////////////////////////////////
	/*!
		���C���[�̕��т�ς���
	*/
	void ChangeLayerLine(int from_index, int to_index);
	void ChangeLayerLine(IImgLayer_Ptr from_layer, int to_index);

	///////////////////////////////////////
	/*!
		�t�@�C�����ێ����Ă��郌�C���[�̐���Ԃ�
		@return ���C���[����
	*/
	inline int GetNumOfChildLayers() const {
		return m_NumOfChildLayers;
	}

	inline int GetMaxLayerIndex() const {
		return m_MaxLayerIndex;
	}

	int CountNumOfChildLayers();
	int CountMaxLayerIndex();

	///////////////////////////////////////
	/*!
		���C���[�̃C���f�b�N�X�𓾂�
		������Ȃ��ꍇ��-1��Ԃ�
		@param[in] find_layer
	*/
	int GetLayerIndex(const IImgLayer_weakPtr find_layer) const;

	///////////////////////////////////////
	/*!
		�C���f�b�N�X���w�肵�ă��C���[�I�u�W�F�N�g�̃|�C���^��Ԃ�
		@return ���C���[�I�u�W�F�N�g�̃|�C���^
	*/
	IImgLayer_Ptr const GetLayerAtIndex(int index) const;

	//////////////////////////////////
	/*!
		return layer group state
		@return display state
	*/
	inline bool IsOpen() const {
		return m_IsOpen;
	}

	/////////////////////////////////
	/*!
		set display state
		@param[in] bl display state
	*/
	inline void SetOpen(bool bl){
		m_IsOpen = bl;
	}

#ifdef _DEBUG
	//////////////////////////////////
	/*!
		dump child layer data
	*/
	void DumpChildLayers(int tab_num=0);
#endif //_DEBUG

private:
	bool m_IsNeedCountChildLayers;
	bool m_IsNeedCountMaxLayerIndex;
	int m_NumOfChildLayers;	//
	int m_MaxLayerIndex; //
	bool m_IsOpen;			//���̊K�w�̃��C���[�������邩?
};

#endif //_IMGLAYERGROUP_H_