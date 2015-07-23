#pragma once
#include<Siv3D.hpp>
#include"Camera.hpp"
#include"Player.hpp"
#include"Stage.hpp"

const size_t testStageIndex = 0;

class GameBase
{
public:
	GameBase() :stage(L"Resource/TestStage/")
	{
	}

	Player m_player = Player(this);

	D2Camera m_camera;

	Stage stage;

	int m_score;

	void cameraUpdate();

	void cityDraw()const
	{
		const Vec2 drawPos = m_camera.getDrawPos(Vec2(0, 0));

		stage.m_stageTexture.draw(drawPos);
	}

	void footholdDraw()const
	{
		stage.drawFootholds(m_camera);
	}

	bool isGameFinished()const
	{
		return m_player.m_pos.x >= 2800;
	}

	void attackToEnemy();

	void update();

	void draw()const;

};
