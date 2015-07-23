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

	//全体マップの座標から、カメラセンターのとき、画面上でどこに位置するかの座標を返す
	inline Vec2 getDrawPos(const Vec2& globalPos)const 
	{
		return Vec2{ 320.0, 240.0 } + globalPos - m_pos;
	}

	Vec2 m_pos;

	static D2Camera* globalCamera;
};
