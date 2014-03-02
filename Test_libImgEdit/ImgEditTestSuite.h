#include <cxxtest/TestSuite.h>

#include "libImgEdit.h"


class ImgEditTestSuite : public CxxTest::TestSuite
{
public:
	ImgEditTestSuite()
	{
		//�����J�����g�f�B���N�g�����擾���AImgEdit�ɓn��
		char cdPath[MAX_PATH];
		if(0 == ::GetCurrentDirectory(MAX_PATH, cdPath)){
			//error
			TS_ASSERT(true);
		}
		m_ImgEdit.SetStartDirectory(cdPath);

		//
		char exePath[MAX_PATH];
		wsprintf(exePath, "%s/Test_libImgEdit.exe", cdPath);
		m_ImgEdit.Init(".\\config.ini", exePath, ".\\log");
	}

	~ImgEditTestSuite()
	{
		//
		m_ImgEdit.End();
	}

	void test_hoge()
	{
	}

private:
	ImgEdit m_ImgEdit;
};