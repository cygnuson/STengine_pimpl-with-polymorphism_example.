#ifndef STATE_HPP
#define STATE_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class State : public std::enable_shared_from_this<State>
{
public:
	State(sf::View& view);
	virtual ~State();
	inline virtual bool Draw(sf::RenderWindow& win) = 0;
	inline virtual bool HandleInput(sf::Event& ev) = 0;

	/**If this is true, the state will be popped on the next cycle.*/
	bool             _popMe;
	/**The ascociated view for this state.*/
	sf::View&        _view;
protected:
	/**The texture manager*/
	TextureManager&  _texMan;
	/**The sound manager*/
	SoundManager&    _soundMan;
	/**The font manager*/
	FontManager&     _fontMan;
};

inline State::State(sf::View& view)
	:
	_texMan(TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan(FontManager::GetInstance()),
	_popMe(false),
	_view(view)
{

}

inline State::~State()
{

}


#endif //STATE_HPP


