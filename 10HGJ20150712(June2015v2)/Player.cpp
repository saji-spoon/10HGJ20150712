#include"Player.hpp"
#include"GameBase.hpp"
#include"Camera.hpp"

void Player::move()
{
	//����
	//���R���W�������Ă��鑫��@�Ȃ��ꍇ��none

	switch (m_moveState)
	{
	case Player::PlayerMoveState::Falling:
	{
		LOG_ERROR(L"Falling Judge");
		const Vec2 futurePos = m_pos + Vec2(6.0, m_jumpV);

		//�����ʒu�ɂ��āA�����蔻��ƌ������𓱂�
		Optional<Line> line = getFootholdCollide(futurePos);

		//�����ʒu�Ō������̂��Ȃ����A�ړ��O�ł��̐��ƌ�����Ă���ꍇ�͗���������//�����ɓ�{�̐��ƌ����Ȃ���
		if (!line ) break;
		if (line.value().intersects(getCol(m_pos))) break;


		//�����蔻��̕�
		const double colWidth = getCol(m_pos).w;
		//�����̓����蔻��l�p�`�̉E��
		const Line rightBodyLine = Line(futurePos, futurePos - Vec2(0, colWidth));
		//�����̓����蔻��l�p�`�̍���
		const Line leftBodyLine = Line(futurePos - Vec2(colWidth, 0), futurePos - Vec2(colWidth, colWidth));

		const Optional<Vec2> sink1 = rightBodyLine.intersectsAt(line.value());
		const Optional<Vec2> sink2 = leftBodyLine.intersectsAt(line.value());
		
		//�[�_�ɏ���������ꍇ
		if (!sink1 && !sink2)
		{
			//�n�_�ƏI�_�ŏ�ɂ���ق��ɍ��킹��
			m_pos.y = std::min(line.value().begin.y, line.value().end.y) + 1.0;
		}

		//���@��y�ʒu�͂���̌�_�ֈړ��i����̒l���Ȃ���Α��݂�����j���ړ���͂��Ȃ炸����̏�ɒ��_�i���Ӂj���҂�����t���`�ɂȂ�
		m_pos.y = std::min(sink1.value_or(Vec2(0, 600)).y, sink2.value_or(Vec2(0, 600)).y) + 1.0;

		//����̏�ɂ��锻��ɂ��āA�ړ��𑱍s����,���̏ꍇ�����͈�x�I

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

	//�X�V
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

		//�[�_�ɏ���Ă��邩�̎w�W
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

		//camera��DrawPos���ڐA
		const Vec2 futureDrawPos = Vec2{ 320.0, 240.0 } + Vec2(6.0, m_jumpV) + Vec2(0, 120);

		const Line rightBodyLine = Line(futureDrawPos, futureDrawPos - Vec2(0, colWidth));
		//�����蔻��l�p�`�̍���
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

//���@�ʒu��pos�̏ꍇ�ɂ��āA�����蔻��ƐG�鑫��ň�Ԍ��ɂ�����̂��Z�o
Optional<Line> Player::getFootholdCollide(const Vec2& pos)const
{
	Optional<Line> line = none;

	const Line myFoot = Line(m_pos, m_pos - Vec2(getCol(m_pos).w, 0));

	for (const auto& fh : m_gb->stage.m_footholds)
	{
		//�����蔻��Ƒ��ꂪ�G��Ă����炻���Ԃ��A����ȊO��none
		if (getCol(pos).intersects(fh))
		{
			line = fh;
		}
	}

	/*
	#ifdef _DEBUG

		if (line)
		{
			//�����蔻��̉��̐��i���j��\��
			myFoot.drawArrow(3.0, { 2.0, 5.0 }, Palette::Limegreen);

			//line�Ƒ��Ƃ̌�_��\��
			Line(m_pos, m_pos - Vec2(getCol(m_pos).w, 0)).intersectsAt(line.value());
		}
	#endif
	*/

	return line;
}

void Player::draw(const D2Camera& camera)const
{
	const Vec2 drawPos = camera.getDrawPos(m_pos);

	//�{�̂̕`��
	RectF(drawPos - Vec2(40.0, 40.0), Point(40, 40)).draw(Palette::Blue);

	for (const auto& at : attackes)
	{
		at.movedBy(m_pos).movedBy(camera.getDrawPos({ 0, 0 })).draw(Palette::Orange);
	}

	/*
#ifdef _DEBUG
	
	//�i�s�������_
	Circle(drawPos, 5).draw(Palette::White);

	//�����蔻��

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
