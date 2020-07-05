#include "Player.h"
#include "CollisionManager.h"
#include "EventManager.h"
#define SPEED 2

Player::Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, SDL_FRect* goal)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf), m_state(idle), m_dir(0), m_pGoal(goal) 
{
	m_dx = m_dy = m_accel = m_vel = 0.0;
	m_velMax = 5.0;
	m_rotMax = 2.5;
}

void Player::Update()
{
	switch (m_state)
	{
	case idle:
		//if (EVMA::KeyHeld(SDL_SCANCODE_W) || EVMA::KeyHeld(SDL_SCANCODE_S) ||
		//	EVMA::KeyHeld(SDL_SCANCODE_A) || EVMA::KeyHeld(SDL_SCANCODE_D))
		//{
		//	SetState(running);
		//}
		//break;
	case running:
		//if (EVMA::KeyReleased(SDL_SCANCODE_M) || EVMA::KeyReleased(SDL_SCANCODE_S) ||
		//	EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		//{
		//	SetState(idle);
		//	break; // Skip movement parsing below.
		//}
		//if (EVMA::KeyHeld(SDL_SCANCODE_W))
		//{
		//	if (m_dst.y > 0 && !COMA::PlayerCollision({ (int)m_dst.x, (int)(m_dst.y), (int)32, (int)32 }, 0, -SPEED))
		//	{
		//		m_dst.y += -SPEED;
		//	}
		//}
		//else if (EVMA::KeyHeld(SDL_SCANCODE_S))
		//{
		//	if (m_dst.y < 768 - 32 && !COMA::PlayerCollision({ (int)m_dst.x, (int)(m_dst.y), (int)32, (int)32 }, 0, SPEED))
		//	{
		//		m_dst.y += SPEED;
		//	}
		//}
		//if (EVMA::KeyHeld(SDL_SCANCODE_A))
		//{
		//	if (m_dst.x > 0  && !COMA::PlayerCollision({ (int)m_dst.x, (int)m_dst.y, (int)32, (int)32 }, -SPEED, 0))
		//	{
		//		m_dst.x += -SPEED;
		//		m_dir = 1;
		//	}
		//}
		//else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		//{
		//	if (m_dst.x < 1024 - 32 &&  !COMA::PlayerCollision({ (int)m_dst.x, (int)m_dst.y, (int)32, (int)32 }, SPEED, 0))
		//	{
		//		m_dst.x += SPEED;
		//		m_dir = 0;
		//	}
		//}
	case marching:
		if (EVMA::KeyPressed(SDL_SCANCODE_M))
		{
			std::cout << "Press M:" << std::endl;
			//SOMA::PlaySound("arrive", 0, 2);
			if (MAMA::Distance((GetDstP()->x + GetDstP()->w / 2.0f), (m_pGoal->x + m_pGoal->w / 2.0f),
				(GetDstP()->y + GetDstP()->h / 2.0f), (m_pGoal->y + m_pGoal->h / 2.0f)) <= 2.0)
			{
				Stop();
				break;
			}
			else
			{
				double destAngle = MAMA::AngleBetweenPoints((m_pGoal->y + m_pGoal->h / 2.0f) - (GetDstP()->y + GetDstP()->h / 2.0f),
					(m_pGoal->x + m_pGoal->w / 2.0f) - (GetDstP()->x + GetDstP()->w / 2.0f));
				Move2Stop(destAngle);
			}
			GetDstP()->x += (float)m_dx;
			GetDstP()->y += (float)m_dy;
		}
		break;
	}
	// Move the enemy.
	GetDstP()->x += (float)m_dx;
	GetDstP()->y += (float)m_dy;
	Animate();
}

void Player::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, static_cast<SDL_RendererFlip>(m_dir));
}

void Player::Start()
{
	m_sprite = 0;
	m_accel = 0.2;
}

void Player::SetState(int s)
{
	m_state = static_cast<state>(s);
	m_frame = 0;
	if (m_state == idle)
	{
		m_sprite = m_spriteMin = m_spriteMax = 0;
	}
	else // Only other is running for now...
	{
		Start();
		m_sprite = m_spriteMin = 1;
		m_spriteMax = 4;
	}
}

void Player::Stop()
{
	m_dx = m_dy = 0.0;
	m_vel = 0;
	m_frame = 0;
	m_frameMax = 4;
	m_sprite = 4;
}

void Player::SetVs(const double angle)
{
	double destAngle = MAMA::Rad2Deg(angle) + 90;
	m_angle += std::min(std::max(MAMA::Angle180(destAngle - m_angle), -m_rotMax), m_rotMax); // Only rotate slightly towards the destination angle.
	// Now use the new slight rotation to generate dx and dy as normal.
	m_vel += m_accel;
	m_vel = std::min(m_vel, m_velMax);
}

void Player::Move2Stop(const double angle)
{
	SetVs(angle);
	double dist = MAMA::Distance((GetDstP()->x + GetDstP()->w / 2.0f), (m_pGoal->x + m_pGoal->w / 2.0f),
		(GetDstP()->y + GetDstP()->h / 2.0f), (m_pGoal->y + m_pGoal->h / 2.0f));
	double perc = std::max(std::min(((dist - 2.0) / (200.0 - 2.0)), 1.0), 0.1);

	m_frameMax = 12 - (int)(8.0 * perc);

	MAMA::SetDeltas(MAMA::Deg2Rad(m_angle - 90), m_dx, m_dy, m_vel * perc, m_vel * perc);
}
