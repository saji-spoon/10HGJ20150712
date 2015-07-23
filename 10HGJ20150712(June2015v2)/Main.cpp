
# include <Siv3D.hpp>
#include"Resource.hpp"
#include"Player.hpp"
#include"GameBase.hpp"

void Main()
{
	GameBase gb;

	ResourceRegister();

	while (System::Update())
	{
		gb.draw();
	
		if (gb.isGameFinished())
		{
			const Rect rec = Rect(500, 80).setCenter(320, 240);
			rec.draw(Color(0,0,0,128));
			FontAsset(L"Debug").drawCenter(L"Score:" + Format(gb.m_score), Vec2{ 320, 240 });

			
			if (gb.m_score == 400)
			{
				const Rect rec = Rect(60, 108, 520, 80);
				rec.draw(Color(0, 0, 0, 128));
				FontAsset(L"Debug").drawCenter(L"★Complete!Congratulations★", Vec2{ 320, 138 });
			}

			continue;
		}

		gb.update();
	}
}
