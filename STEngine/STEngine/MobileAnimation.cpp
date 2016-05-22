#include "MobileAnimation.hpp"



MobileAnimation::MobileAnimation()
{

}

MobileAnimation::~MobileAnimation()
{

}

void MobileAnimation::SetDirectionAnimation(uint64_t animCode, Animation anim)
{
	_animations[animCode] = anim;
}

void MobileAnimation::SetStillFrameIndex(uint32_t frame)
{
	for (auto& e : _animations)
	{
		e.second.SetStillFrame(frame);
	}
}

Animation& MobileAnimation::GetAnimation(uint64_t animCode)
{
	if (_animations.count(animCode) < 1)
	{
		cg::logger::log_error("There is no animation with that direction ",
			"code. (", animCode, ").");
		throw std::runtime_error("Invalid animation direction code.");
	}
	return _animations[animCode];
}


void MobileAnimation::SetColor(const sf::Color & color)
{
	for (auto& e : _animations)
	{
		e.second.SetColor(color);
	}
}

const sf::Color & MobileAnimation::GetColor() const
{
	return _animations.begin()->second.GetColor();
}

sf::FloatRect MobileAnimation::GetLocalBounds() const
{
	return _animations.begin()->second.GetLocalBounds();
}

sf::FloatRect MobileAnimation::GetGlobalBounds() const
{
	return _animations.begin()->second.GetGlobalBounds();
}

void MobileAnimation::SetPosition(float x, float y)
{
	for (auto& e : _animations)
	{
		e.second.SetPosition(x, y);
	}
}

void MobileAnimation::SetPosition(const sf::Vector2f & position)
{
	for (auto& e : _animations)
	{
		e.second.SetPosition(position);
	}
}

void MobileAnimation::SetRotation(float angle)
{
	for (auto& e : _animations)
	{
		e.second.SetRotation(angle);
	}
}

void MobileAnimation::SetScale(float factorX, float factorY)
{
	for (auto& e : _animations)
	{
		e.second.SetScale(factorX, factorY);
	}
}

void MobileAnimation::SetScale(const sf::Vector2f & factors)
{
	for (auto& e : _animations)
	{
		e.second.SetScale(factors);
	}
}

void MobileAnimation::SetOrigin(float x, float y)
{
	for (auto& e : _animations)
	{
		e.second.SetOrigin(x, y);
	}
}

void MobileAnimation::SetOrigin(const sf::Vector2f & origin)
{
	for (auto& e : _animations)
	{
		e.second.SetOrigin(origin);
	}
}

const sf::Vector2f & MobileAnimation::GetPosition() const
{
	return _animations.begin()->second.GetPosition();
}

float MobileAnimation::GetRotation() const
{
	return _animations.begin()->second.GetRotation();
}

const sf::Vector2f & MobileAnimation::GetScale() const
{
	return _animations.begin()->second.GetScale();
}

const sf::Vector2f & MobileAnimation::GetOrigin() const
{
	return _animations.begin()->second.GetOrigin();
}

void MobileAnimation::Move(float offsetX, float offsetY)
{
	for (auto& e : _animations)
	{
		e.second.Move(offsetX, offsetY);
	}
}

void MobileAnimation::Move(const sf::Vector2f & offset)
{
	for (auto& e : _animations)
	{
		e.second.Move(offset);
	}
}

void MobileAnimation::Rotate(float angle)
{
	for (auto& e : _animations)
	{
		e.second.Rotate(angle);
	}
}

void MobileAnimation::Scale(float factorX, float factorY)
{
	for (auto& e : _animations)
	{
		e.second.Scale(factorX, factorY);
	}
}

void MobileAnimation::Scale(const sf::Vector2f & factor)
{
	for (auto& e : _animations)
	{
		e.second.Scale(factor);
	}
}

const sf::Transform & MobileAnimation::GetInverseTransform() const
{
	return _animations.begin()->second.GetInverseTransform();
}

const sf::Transform & MobileAnimation::GetTransform() const
{
	return _animations.begin()->second.GetTransform();
}

void MobileAnimation::Draw(sf::RenderWindow & win, uint64_t direction,
	bool still)
{
	win.draw(GetAnimation(direction).GetFrame(still));
}


