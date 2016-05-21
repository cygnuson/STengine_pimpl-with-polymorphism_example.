#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
/**The animation class is a simple animation that will play all frames given
to it.  For an animation that has different directions, see MobileAnimation*/
class Animation
{
public:
	Animation();
	/**Create an animation.
	\param textureName The name ofthe texture to load.
	\param tileSize The size of each tile.
	\param startPosition the starting tile to use. Default is 0,0
	\param span the amount of tiles to the right and down to use. by default,
	 the value will be 0,0 and the entire texture will be used.
	*/
	Animation(const std::string& textureName, sf::Vector2u tileSize,
		sf::Vector2u startPosition = { 0,0 }, sf::Vector2u span = { 0,0 });
	virtual ~Animation();
	void Move(sf::Vector2f amt);
	virtual void Draw(sf::RenderWindow& win);
protected:
	sf::Sprite GetFrame();

	sf::Clock               _clock;
	float                   _fps;
	std::vector<sf::Sprite> _frames;

	
};

#endif //ANIMATION_HPP