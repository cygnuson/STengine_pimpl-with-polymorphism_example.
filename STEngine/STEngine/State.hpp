#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class State : public std::enable_shared_from_this<State>
{
protected:
	class Base; //forward declare
public:
	State();
	virtual ~State();
	/**Draw the state. Calls the _self->Draw(...)*/
	inline bool Draw(sf::RenderWindow& win);
	/**Handle input. Calls _self->HandleInput(...)*/
	inline bool HandleInput(sf::Event& ev);
	/**Make sure the stat is sane.*/
	inline bool SanityCheck();
	/**If this is true, the state will be popped on the next cycle.*/
	bool             _popMe;
	/**The ascociated view for this state.*/
	sf::View*        _view = nullptr;
protected:
	friend class SFMLApplication;
	/**The texture manager*/
	TextureManager&  _texMan;
	/**The sound manager*/
	SoundManager&    _soundMan;
	/**The font manager*/
	FontManager&     _fontMan;

	class Base
	{
	public:
		virtual inline bool Draw(sf::RenderWindow& win) = 0;
		virtual inline bool HandleInput(sf::Event& ev) = 0;
		virtual inline sf::View& GetView() = 0;
	};

	template<typename T>
	class Model : public Base, public T
	{
	public:
		template<typename...Args>
		Model(Args...args);
		virtual inline bool Draw(sf::RenderWindow& win);
		virtual inline bool HandleInput(sf::Event& ev);
		virtual inline sf::View& GetView();
	private:

	};
	std::unique_ptr<Base> _self;
};

inline State::State()
	:
	_texMan(TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan(FontManager::GetInstance()),
	_popMe(false)
{

}

inline State::~State()
{

}

inline bool State::Draw(sf::RenderWindow & win)
{
	return _self->Draw(win);
}

inline bool State::HandleInput(sf::Event & ev)
{
	return _self->HandleInput(ev);
}
inline bool State::SanityCheck()
{
	return _view != nullptr;
}
/******************************************************************Model impl*/
template<typename T>
template<typename...Args>
inline State::Model<T>::Model(Args...args)
	:T(std::forward<Args>(args)...)
{

}
template<typename T>
inline bool State::Model<T>::Draw(sf::RenderWindow & win)
{
	return T::Draw(win);
}

template<typename T>
inline bool State::Model<T>::HandleInput(sf::Event & ev)
{
	return T::HandleInput(ev);
}

template<typename T>
inline sf::View & State::Model<T>::GetView()
{
	return T::GetView();
}


#endif //STATE_HPP