#pragma once

#include<Siv3D.hpp>
#include<HamFramework.hpp>

class D2Camera {

public:

	D2Camera(){}

	D2Camera(const Vec2& v) :m_pos(v)
	{
		#ifdef _DEBUG
			globalCamera = this;
		#endif
	}

	//�S�̃}�b�v�̍��W����A�J�����Z���^�[�̂Ƃ��A��ʏ�łǂ��Ɉʒu���邩�̍��W��Ԃ�
	inline Vec2 getDrawPos(const Vec2& globalPos)const 
	{
		return Vec2{ 320.0, 240.0 } + globalPos - m_pos;
	}

	Vec2 m_pos;

	static D2Camera* globalCamera;
};
