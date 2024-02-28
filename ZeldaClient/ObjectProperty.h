#pragma once

namespace PurahEngine
{
	enum class ObjectProperty : unsigned int
	{
		eConductor				= 0x01 << 0,	// ������
		eEnableHandling			= 0x01 << 1,	// ��Ʈ���ڵ� ���� ����
		ePortable				= 0x01 << 2,	// �÷��̾ �� �� �ִ°�
		eFlammable				= 0x01 << 3,	// ������
		e4						= 0x01 << 4,	// 
		e5						= 0x01 << 5,	// 
		e6						= 0x01 << 6,	// 
		e7						= 0x01 << 7,	// 
	};
}
