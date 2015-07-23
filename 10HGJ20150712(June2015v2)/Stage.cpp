#include"Stage.hpp"

Stage::Stage(const FilePath& dataDirectoryPath) :m_stageTexture(dataDirectoryPath + L"city.png")
{
	TextReader reader(dataDirectoryPath + L"foothold.txt", TextEncoding::UTF8);
	TextReader readerEnemy(dataDirectoryPath + L"enemy.txt", TextEncoding::UTF8);
	String str;

	if (!reader.isOpened() || !readerEnemy.isOpened())
	{
		LOG_ERROR(L"ERROR:file can't open@Stage");

		return;
	}

	while (reader.readLine(str))
	{
		m_footholds.push_back(Parse<Line>(str));

		OutputLog(LogDescription::Info, L"READ:", Parse<Line>(str));
	}

	while (readerEnemy.readLine(str))
	{
		m_enemies.push_back(Parse<Vec2>(str));

		OutputLog(LogDescription::Info, L"READ:", Parse<Vec2>(str));
	}

	reader.close();
	readerEnemy.close();
}

void Stage::drawFootholds(const D2Camera& camera)const
{
	const Vec2 diff = camera.getDrawPos(Vec2(0, 0));

	for (const auto& f : m_footholds)
	{
		f.movedBy(diff).draw(5.0, Color(36, 61, 99)).draw(3.0, Color(24, 51, 89));

	}
}