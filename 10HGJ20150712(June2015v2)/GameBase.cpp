#include"GameBase.hpp"

void GameBase::cameraUpdate()
{
	m_camera.m_pos = m_player.m_pos + Vec2(200.0, -120.0);
}

void GameBase::update()
{
	cameraUpdate();

	m_player.update();

	attackToEnemy();


}


void GameBase::attackToEnemy()
{
	const Vec2 pos = m_player.m_pos;

	for (const auto& ac : m_player.attackes)
	{
		Erase_if(stage.m_enemies, [&](const Vec2& e)
		{
			if (ac.movedBy(pos).intersects(e))
			{
				m_score += 100;
				return true;
			}

			return false;
		});

	}
}

void GameBase::draw()const
{
	cityDraw();

	m_player.draw(m_camera);

	footholdDraw();

	//enemy‚Ì•`‰æ
	for (const auto& e : stage.m_enemies)
	{
		Circle(m_camera.getDrawPos(e), 20).draw(Palette::Darkblue);
	}

	const Vec2 s = stage.m_stageTexture.size;

	Rect({ 102, 400 }, { 475, 80 }).draw(Color(0, 0, 0, 128));

	for (const auto& e : stage.m_enemies)
	{
		RectF(Vec2{102, 400} + Vec2{ e.x / s.x * 475, e.y / s.y * 80 }, { 5, 3 }).draw(Palette::Blue);
	}

	const Vec2 mp = m_player.m_pos;

	RectF(RectF(Vec2{ 102, 400 } +Vec2{ mp.x / s.x * 475, mp.y / s.y * 80 }, { 5, 3 }).draw(Palette::Orange));

	
}
