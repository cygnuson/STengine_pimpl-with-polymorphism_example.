#include "Animation.hpp"

Animation::Animation()
{

}

Animation::Animation(const std::string & textureName, sf::Vector2u tileSize, 
	sf::Vector2u startPosition, sf::Vector2u span)
{

}

Animation::~Animation()
{

}

sf::Sprite Animation::GetFrame()
{
	return sf::Sprite();
}

void Animation::Move(sf::Vector2f amt)
{
	for (auto& e : _frames)
	{
		e.move(amt);
	}
}

void Animation::Draw(sf::RenderWindow & win)
{
	win.draw(GetFrame());
}

