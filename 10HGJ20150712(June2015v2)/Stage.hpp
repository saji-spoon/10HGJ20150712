#pragma once
#include<Siv3D.hpp>
#include"Camera.hpp"

class Stage
{
public:
	Stage(){}

	//データはdataDirectoryPath下から読む
	Stage(const FilePath& dataDirectoryPath);

	std::vector<Vec2> m_enemies;
	std::vector<Line> m_footholds;

	Texture m_stageTexture;

	void drawFootholds(const D2Camera& camera)const;
};