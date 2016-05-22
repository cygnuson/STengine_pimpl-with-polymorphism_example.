#include "Animation.hpp"

Animation::Animation()
{
	
}

Animation::Animation(const std::string & textureName, sf::Vector2u tileSize, 
	float fps, sf::Vector2u startPosition_t, sf::Vector2u span_t)
{
	_fps = fps;
	/*Any local name with _t postfix is in tiles, and _p is in pixels.*/
	auto& tex = TextureManager::GetInstance()[textureName];
	auto texSize_p = tex.getSize();
	sf::Vector2u texSize_t = { texSize_p.x / tileSize.x,
		texSize_p.y / tileSize.y };
	/*the remaining tiles for the full texture.*/
	sf::Vector2u remaining_t = { texSize_t.x - startPosition_t.x,
		texSize_t.y - startPosition_t.y };

	if ((startPosition_t.x + span_t.x) > texSize_t.x 
		|| (startPosition_t.y + span_t.y) > texSize_t.y)
	{
		/*This will try to get a tile from outsize the bounds of the texture.*/
		cg::logger::log_error("The span of the animation is larger then the ",
			"Texture would allow with that starting position.");
		throw std::runtime_error("The span+start exceeds the texture size.");
	}
	/*If either dimension of the span is 0, it will just go to the end of the
	texture.*/
	if (span_t.x == 0)
	{
		span_t.x = remaining_t.x;
	}
	if (span_t.y == 0)
	{
		span_t.y = remaining_t.y;
	}
	/*set the last position to use.*/
	sf::Vector2u lastPos_t = { 0,0 };
	lastPos_t.x = startPosition_t.x + span_t.x;
	lastPos_t.y = startPosition_t.y + span_t.y;
	auto currentPosition_t = startPosition_t;
	_frames.reserve(
		(lastPos_t.x - startPosition_t.x)*(lastPos_t.x - startPosition_t.x));
	/*load all the frames row by row.*/
	while (currentPosition_t.y < lastPos_t.y)
	{
		while (currentPosition_t.x < lastPos_t.x)
		{
			/*the rect that will be the slice of texture to use.*/
			sf::IntRect rect(currentPosition_t.x*tileSize.x,
			currentPosition_t.y*tileSize.y,
			tileSize.x, tileSize.y);
			_frames.push_back(sf::Sprite(tex, rect));
			++currentPosition_t.x;
		}
		currentPosition_t.x = startPosition_t.x;
		++currentPosition_t.y;
	}
	_stillFrameIndex = -1;
	_currentFrame = 0;
}

Animation::~Animation()
{

}

const sf::Sprite& Animation::GetFrame(bool still)
{
	if (_currentFrame >= _frames.size())
	{
		_currentFrame = 0;
	}
	if (still)
	{
		if (_stillFrameIndex == -1)
		{
			return _frames[_currentFrame];
		}
		else {
			return _frames[_stillFrameIndex];
		}
	}
	if (_frames.size() == 0)
	{
		cg::logger::log_error("Encountered an empty animation object.");
		throw std::runtime_error("Invalid _frame array.");
	}
	if (_clock.getElapsedTime() > sf::seconds(float(1) / _fps))
	{
		_clock.restart();
		return _frames[_currentFrame++];
	}
	return _frames[_currentFrame];
}

void Animation::SetColor(const sf::Color & color)
{
	for (auto& e : _frames)
	{
		e.setColor(color);
	}
}

const sf::Color & Animation::GetColor() const
{
	return _frames[0].getColor();
}

sf::FloatRect Animation::GetLocalBounds() const
{
	return _frames[0].getLocalBounds();
}

sf::FloatRect Animation::GetGlobalBounds() const
{
	return _frames[0].getGlobalBounds();
}

void Animation::SetPosition(float x, float y)
{
	for (auto& e : _frames)
	{
		e.setPosition(x,y);
	}
}

void Animation::SetPosition(const sf::Vector2f & position)
{
	for (auto& e : _frames)
	{
		e.setPosition(position);
	}
}

void Animation::SetRotation(float angle)
{
	for (auto& e : _frames)
	{
		e.setRotation(angle);
	}
}

void Animation::SetScale(float factorX, float factorY)
{
	for (auto& e : _frames)
	{
		e.setScale(factorX,factorY);
	}
}

void Animation::SetScale(const sf::Vector2f & factors)
{
	for (auto& e : _frames)
	{
		e.setScale(factors);
	}
}

void Animation::SetOrigin(float x, float y)
{
	for (auto& e : _frames)
	{
		e.setOrigin(x,y);
	}
}

void Animation::SetOrigin(const sf::Vector2f & origin)
{
	for (auto& e : _frames)
	{
		e.setOrigin(origin);
	}
}

const sf::Vector2f & Animation::GetPosition() const
{
	return _frames[0].getPosition();
}

float Animation::GetRotation() const
{
	return _frames[0].getRotation();
}

const sf::Vector2f & Animation::GetScale() const
{
	return _frames[0].getScale();
}

const sf::Vector2f & Animation::GetOrigin() const
{
	return _frames[0].getOrigin();
}

void Animation::Move(float offsetX, float offsetY)
{
	for (auto& e : _frames)
	{
		e.move(offsetX,offsetY);
	}
}

void Animation::Move(const sf::Vector2f & offset)
{
	for (auto& e : _frames)
	{
		e.move(offset);
	}
}

void Animation::Rotate(float angle)
{
	for (auto& e : _frames)
	{
		e.rotate(angle);
	}
}

void Animation::Scale(float factorX, float factorY)
{
	for (auto& e : _frames)
	{
		e.scale(factorX,factorY);
	}
}

void Animation::Scale(const sf::Vector2f & factor)
{
	for (auto& e : _frames)
	{
		e.scale(factor);
	}
}

const sf::Transform & Animation::GetInverseTransform() const
{
	return _frames[0].getInverseTransform();
}

const sf::Transform & Animation::GetTransform() const
{
	return _frames[0].getTransform();
}

void Animation::Draw(sf::RenderWindow & win)
{
	win.draw(GetFrame());
}

void Animation::SetStillFrame(uint64_t frame)
{
	if (frame > _frames.size())
	{
		cg::logger::log_error("The still frame index is set out of bounds.");
		throw std::runtime_error("Invalid frame index.");
	}
	_stillFrameIndex = frame;
}

