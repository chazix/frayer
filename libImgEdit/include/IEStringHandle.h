#ifndef _IESTRINGHANDLE_H_
#define _IESTRINGHANDLE_H_

#include "LibIEDllHeader.h"

class _EXPORTCPP IEStringHandle
{
public:
	///////////////////////////////////
	/*!
		val�̌��������߂�B
	*/
	static int GetDigit(int val);

	///////////////////////////////////
	/*!
		�����񂪂��ׂĐ����Ȃ�true��Ԃ��B
		@return
	*/
	static bool IsAllDigitStr(const char* str);

};

#endif //_IESTRINGHANDLE_H_