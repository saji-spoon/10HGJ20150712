#include"Player.hpp"
#include"GameBase.hpp"
#include"Camera.hpp"

void Player::move()
{
	//判定
	//今コリジョンしている足場　ない場合はnone

	switch (m_moveState)
	{
	case Player::PlayerMoveState::Falling:
	{
		LOG_ERROR(L"Falling Judge");
		const Vec2 futurePos = m_pos + Vec2(6.0, m_jumpV);

		//未来位置について、当たり判定と交わる線を導く
		Optional<Line> line = getFootholdCollide(futurePos);

		//未来位置で交わるものがないか、移動前でその線と交わっている場合は落ち続ける//同時に二本の線と交わらない線
		if (!line ) break;
		if (line.value().intersects(getCol(m_pos))) break;


		//当たり判定の幅
		const double colWidth = getCol(m_pos).w;
		//未来の当たり判定四角形の右辺
		const Line rightBodyLine = Line(futurePos, futurePos - Vec2(0, colWidth));
		//未来の当たり判定四角形の左辺
		const Line leftBodyLine = Line(futurePos - Vec2(colWidth, 0), futurePos - Vec2(colWidth, colWidth));

		const Optional<Vec2> sink1 = rightBodyLine.intersectsAt(line.value());
		const Optional<Vec2> sink2 = leftBodyLine.intersectsAt(line.value());
		
		//端点に乗っかった場合
		if (!sink1 && !sink2)
		{
			//始点と終点で上にあるほうに合わせる
			m_pos.y = std::min(line.value().begin.y, line.value().end.y) + 1.0;
		}

		//自機のy位置はより上の交点へ移動（一方の値がなければ存在する方）→移動後はかならず足場の上に頂点（か辺）がぴったり付く形になる
		m_pos.y = std::min(sink1.value_or(Vec2(0, 600)).y, sink2.value_or(Vec2(0, 600)).y) + 1.0;

		//足場の上にいる判定にして、移動を続行する,その場合処理は一度終

		m_colledLine = line;
		m_moveState = PlayerMoveState::OnFoothold;
		m_jumpV = 0;
	}
		break;
	case Player::PlayerMoveState::Rising:
		if (m_jumpV >= 0)
		{
			m_moveState = PlayerMoveState::Falling;
			m_pos += Vec2(6.0, 0.0);
			return;
		}
		break;
	case Player::PlayerMoveState::OnFoothold:
	{
		LOG_ERROR(L"On Judge");

		if (Input::KeyZ.clicked)
		{
			m_moveState = PlayerMoveState::Rising;
			m_jumpV = -15.0;
			m_pos += Vec2(6.0, -15.0);
			return;
		}


		break;
	}
		break;
	default:
		break;
	}

	//更新
	switch (m_moveState)
	{
	case PlayerMoveState::Falling:
		LOG_ERROR(L"Falling Update");
		m_pos += Vec2(6.0, m_jumpV);

		m_jumpV += 1.0;
		break;
	case PlayerMoveState::Rising:
		m_pos += Vec2(6.0, m_jumpV);
		m_jumpV += 1.0;
		break;
	case PlayerMoveState::OnFoothold:
	{
		LOG_ERROR(L"On Update");
		if (!m_colledLine)
		{
			LOG_ERROR(L"ERROR:no Line crossed");
			return;
			
		};

		const Line myFoot(m_pos, m_pos - Vec2(40, 0));

		const double colWidth = getCol(m_pos).w;
		const Vec2 rightPoint = m_pos;
		const Vec2 leftPoint = m_pos - Vec2(colWidth, 0);

		/*
		const bool isFalledFromFoothold = leftPoint.x >= line.value().end.x;

		if (isFalledFromFoothold)
		{
			LOG_ERROR(L"ERROR:crossedPoint between Foot and collideLine is not determined.");
			break;
		}
		*/

		/*
	#ifdef _DEBUG
		Circle(rightPoint, 5).draw(Palette::Green);
		Circle(leftPoint, 5).draw(Palette::Green);
	#endif
	*/

		//const double distanceFromPoint = std::min(rightPoint.distanceFrom(crossedPoint.value()), leftPoint.distanceFrom(crossedPoint.value()));

		//端点に乗っているかの指標
		const double grad = (-m_colledLine.value().vector().y) / m_colledLine.value().vector().x;
		const double onTheFoothold1 = (grad >= 0) ? Vec2(rightPoint - m_colledLine.value().end).x : Vec2(leftPoint - m_colledLine.value().begin).x;

		//Println(grad);
		//Println(onTheFoothold1);

		if ((grad >= 0) && (onTheFoothold1 > -6.0))
		{
			//Println(L"move verticaly");
			m_pos += Vec2(6.0, 0);
		}
		else if ((grad < 0) && (onTheFoothold1 < -6.0))
		{
			//Println(L"move verticaly");
			m_pos += Vec2(6.0, 0);
		}
		else 
		{
			m_pos += 6.0 * m_colledLine.value().vector().normalized();
			LOG_ERROR(L"SPEED:", 6.0 * m_colledLine.value().vector().normalized());
		}
		
		
		const bool isFalledFromFoothold = (leftPoint.x >= m_colledLine.value().end.x);

		if (isFalledFromFoothold)
		{
			m_moveState = PlayerMoveState::Falling;
			//m_pos += Vec2(6.0, 0);
			m_colledLine = none;
			return;
		}

		break;
	}
	default:
		break;
	}

	/*
	#ifdef _DEBUG
		const double colWidth = getCol(m_pos).w;

		//cameraのDrawPosを移植
		const Vec2 futureDrawPos = Vec2{ 320.0, 240.0 } + Vec2(6.0, m_jumpV) + Vec2(0, 120);

		const Line rightBodyLine = Line(futureDrawPos, futureDrawPos - Vec2(0, colWidth));
		//当たり判定四角形の左辺
		const Line leftBodyLine = Line(futureDrawPos - Vec2(colWidth, 0), futureDrawPos - Vec2(colWidth, colWidth));

		rightBodyLine.draw(Palette::Red);
		leftBodyLine.draw(Palette::Red);

		if (m_moveState == PlayerMoveState::Falling)
		{
			rightBodyLine.draw(Palette::Red);
			leftBodyLine.draw(Palette::Red);
		}

	#endif
	*/
}

void Player::update()
{
	move();

	if (Input::KeyX.clicked && attackTimer == 0)
	{
		attackes.push_back(Circle({ 35, -20 }, 30));
		attackes.push_back(Circle({ 30, -40 }, -30));
		attackes.push_back(Circle({ 10, -60 }, -30));
		attackTimer = 55;
	}

	if (attackTimer > 0)
	{
		--attackTimer;

		if (attackTimer == 1)
		{
			Erase_if(attackes, [](const Circle& c){ return true; });
		}
	}
}

//自機位置がposの場合について、当たり判定と触る足場で一番後ろにあるものを算出
Optional<Line> Player::getFootholdCollide(const Vec2& pos)const
{
	Optional<Line> line = none;

	const Line myFoot = Line(m_pos, m_pos - Vec2(getCol(m_pos).w, 0));

	for (const auto& fh : m_gb->stage.m_footholds)
	{
		//当たり判定と足場が触れていたらそれを返す、それ以外はnone
		if (getCol(pos).intersects(fh))
		{
			line = fh;
		}
	}

	/*
	#ifdef _DEBUG

		if (line)
		{
			//当たり判定の下の線（足）を表示
			myFoot.drawArrow(3.0, { 2.0, 5.0 }, Palette::Limegreen);

			//lineと足との交点を表示
			Line(m_pos, m_pos - Vec2(getCol(m_pos).w, 0)).intersectsAt(line.value());
		}
	#endif
	*/

	return line;
}

void Player::draw(const D2Camera& camera)const
{
	const Vec2 drawPos = camera.getDrawPos(m_pos);

	//本体の描画
	RectF(drawPos - Vec2(40.0, 40.0), Point(40, 40)).draw(Palette::Blue);

	for (const auto& at : attackes)
	{
		at.movedBy(m_pos).movedBy(camera.getDrawPos({ 0, 0 })).draw(Palette::Orange);
	}

	/*
#ifdef _DEBUG
	
	//進行方向頂点
	Circle(drawPos, 5).draw(Palette::White);

	//当たり判定

	String str;
	switch (m_moveState)
	{
	case Player::PlayerMoveState::Falling:
		str = L"Falling";

		break;
	case Player::PlayerMoveState::Rising:
		str = L"Rising";
		break;
	case Player::PlayerMoveState::OnFoothold:
		str = L"On";

		break;
	default:
		str = L"---";
		break;
	}
	//Println(str);

	for (const auto& at : attackes)
	{
		at.movedBy(m_pos).movedBy(camera.getDrawPos({0,0})).draw(Palette::Orange);
	}


#endif
	*/
}
