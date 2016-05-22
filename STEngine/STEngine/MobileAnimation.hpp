#ifndef MOBILEANIMATION_HPP
#define MOBILEANIMATION_HPP

#include <map>

#include "Animation.hpp"

/**The mobile animation is just a set of static animations that are held
together with a changable direction.*/
class MobileAnimation
{
public:
	/**Create it with no directions*/
	MobileAnimation();
	virtual ~MobileAnimation();
	/**Add a direction to the mob.
	\param anim [in] The animation that will be the direction for the code
	`anim`.
	\param animCode [in] the code that will be used for this animation
	direction.
	*/
	void SetDirectionAnimation(uint64_t animCode, Animation anim);
	/**Set the still frame index for each animation.
	\param frame [in] The frame to set.*/
	void SetStillFrameIndex(uint32_t frame);
	/**Get an animation.
	\param animCode [in] the code for the animation direction to get.
	*/
	Animation& GetAnimation(uint64_t animCode);
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
	/**Draw to a window.
	\param win [in] the window to draw to.
	\param direction [in] the direction to get the frames for.
	\param still [in] true to get the still frame for when there is no moving.
	*/
	virtual void Draw(sf::RenderWindow& win, uint64_t direction, 
		bool still = false);
private:
	std::map<uint64_t, Animation> _animations;
};

#endif //MOBILEANIMATION_HPP