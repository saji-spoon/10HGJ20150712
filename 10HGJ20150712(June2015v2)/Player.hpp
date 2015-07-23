#pragma once
#include<Siv3D.hpp>
#include"Camera.hpp"

class GameBase;

class Player
{
public:
	Player(){}

	Player(GameBase* gbp) :m_gb(gbp)
	{}

	Vec2 m_pos = Vec2(320, 340);

	double m_jumpV = 0.0;//‚’¼•ûŒü‚Ì‘¬“x//ƒWƒƒƒ“ƒv

	std::vector<Circle> attackes;
	int attackTimer = 0;

	void move();

	Optional<Line> m_colledLine;

	inline RectF getCol(const Vec2& nowPos)const
	{
		return RectF(nowPos - Vec2(40.0, 40.0), Point(40, 40));
	}

	GameBase* m_gb;

	Optional<Line> getFootholdCollide(const Vec2& pos)const;

	enum class PlayerMoveState
	{
		Falling,
		Rising,
		OnFoothold
	} m_moveState = PlayerMoveState::Falling;

	void update();

	void draw(const D2Camera& camera)const;
};