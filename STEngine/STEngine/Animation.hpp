#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <cstdint>

#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

#include "TextureManager.hpp"

/**The animation class is a simple animation that will play all frames given
to it.  For an animation that has different directions, see MobileAnimation*/
class Animation
{
public:
	Animation();
	/**Create an animation.
	\param textureName [in] The name ofthe texture to load.
	\param tileSize [in] The size of each tile.
	\param startPosition [in] the starting tile to use. Default is 0,0
	\param span the [in] amount of tiles to the right and down to use. by default,
	 the value will be 0,0 and the entire texture will be used.
	*/
	Animation(const std::string& textureName, sf::Vector2u tileSize, float fps,
		sf::Vector2u startPosition = { 0,0 }, sf::Vector2u span = { 0,0 });
	virtual ~Animation();
	/**\sa sf::Sprite::setColor*/
	void SetColor(const sf::Color &color);
	/**\sa sf::Sprite::getColor*/
	const sf::Color& GetColor() const;
	/**\sa sf::Sprite::getLocalBounds*/
	sf::FloatRect GetLocalBounds() const;
	/**\sa sf::Sprite::getGlobalBounds*/
	sf::FloatRect GetGlobalBounds() const;
	/**\sa sf::Sprite::setPosition*/
	void SetPosition(float x, float y);
	/**\sa sf::Sprite::setPosition*/
	void SetPosition(const sf::Vector2f &position);
	/**\sa sf::Sprite::setRotation*/
	void SetRotation(float angle);
	/**\sa sf::Sprite::setScale*/
	void SetScale(float factorX, float factorY);
	/**\sa sf::Sprite::setScale*/
	void SetScale(const sf::Vector2f &factors);
	/**\sa sf::Sprite::setOrigin*/
	void SetOrigin(float x, float y);
	/**\sa sf::Sprite::setOrigin*/
	void SetOrigin(const sf::Vector2f &origin);
	/**\sa sf::Sprite::getPosition*/
	const sf::Vector2f& GetPosition() const;
	/**\sa sf::Sprite::getRotation*/
	float GetRotation() const;
	/**\sa sf::Sprite::getScale*/
	const sf::Vector2f& GetScale() const;
	/**\sa sf::Sprite::getOrigin*/
	const sf::Vector2f& GetOrigin() const;
	/**\sa sf::Sprite::move*/
	void Move(float offsetX, float offsetY);
	/**\sa sf::Sprite::move*/
	void Move(const sf::Vector2f &offset);
	/**\sa sf::Sprite::rotate*/
	void Rotate(float angle);
	/**\sa sf::Sprite::scale*/
	void Scale(float factorX, float factorY);
	/**\sa sf::Sprite::scale*/
	void Scale(const sf::Vector2f &factor);
	/**\sa sf::Sprite::getInverseTransform*/
	const sf::Transform& GetInverseTransform() const;
	/**\sa sf::Sprite::getTransform*/
	const sf::Transform& GetTransform() const;

	virtual void Draw(sf::RenderWindow& win);
protected:
	sf::Sprite& GetFrame();

	sf::Clock               _clock;
	float                   _fps;
	std::vector<sf::Sprite> _frames;
	uint32_t                _currentFrame;

};

#endif //ANIMATION_HPP