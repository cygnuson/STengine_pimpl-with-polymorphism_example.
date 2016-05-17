#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "FontManager.hpp"
#include "SFINAE.hpp"

class State : public std::enable_shared_from_this<State>
{
public:
	enum class Flag
	{
		Pop = 1,
		Push,
		Exit,
		None,
	};
	using StatePair = std::pair<Flag, std::shared_ptr<State>>;

	State();
	virtual ~State();
	/**Draw the state. Calls the _self->Draw(...)
	\return A pair that holds a flag and a pointer to a state. the ptr to state
	is only used when the flag requires a new state.*/
	bool Draw(sf::RenderWindow& win, float dt);
	/**Handle input. Calls _self->HandleInput(...)*/
	State::StatePair HandleInput(sf::Event& ev, float dt);
	/**Get the view of this state (will get from _self)*/
	sf::View& GetView();
	/**Make sure the stat is sane.*/
	bool SanityCheck();
	/**Update the state logics.*/
	bool UpdateLogic(float dt);
	/**Create a state for use with an app.
	T must have:
	State::StackPair Draw(sf::RenderWindow&,float)
	bool HandleInput(sf::Event&,float)
	sf::View& GetView()
	bool SanityCheck()
	bool UpdateLogic()

	T will be initialized with Args...
	*/
	template<typename T, typename...Args>
	static std::shared_ptr<State> MakeState(Args...args);
protected:

	class StateBase
	{
	public:
		/**If returning somthing that does not require a state ptr, use nullptr
		*/
		virtual bool Draw(sf::RenderWindow& win, float dt) = 0;
		virtual State::StatePair HandleInput(sf::Event& ev, float dt) = 0;
		virtual sf::View& GetView() = 0;
		virtual bool UpdateLogic(float dt) = 0;
		virtual bool SanityCheck() = 0;
	};

	template<typename T>
	class StateImpl : public StateBase, public T
	{
	public:
		template<typename...Args>
		StateImpl(Args...args);
		virtual bool Draw(sf::RenderWindow& win, float dt);
		virtual State::StatePair HandleInput(sf::Event& ev, float dt);
		virtual sf::View& GetView();
		virtual bool UpdateLogic(float dt);
		virtual bool SanityCheck();
	private:

	};

	friend class SFMLApplication;
	/**The texture manager*/
	TextureManager&  _texMan;
	/**The sound manager*/
	SoundManager&    _soundMan;
	/**The font manager*/
	FontManager&     _fontMan;
	/**A ptr to the actual implimentation of the state.*/
	std::unique_ptr<StateBase> _self;
};

template<typename T, typename ...Args>
inline std::shared_ptr<State> State::MakeState(Args ...args)
{
	static_assert(CanBeState<T>(), "Class does not have the right functions to"
		" be a state. Needs Draw, HandleInput, and GetView, SanityCheck, "
		"UpdateLogic");
	auto ptr = std::make_shared<State>();
	ptr->_self.reset(new State::StateImpl<T>(std::forward<Args>(args)...));
	return ptr;
}
/**************************************************************StateImpl impl*/
template<typename T>
template<typename...Args>
inline State::StateImpl<T>::StateImpl(Args...args)
	:T(std::forward<Args>(args)...)
{

}
template<typename T>
bool State::StateImpl<T>::Draw(sf::RenderWindow & win, float dt)
{
	return T::Draw(win,dt);
}

template<typename T>
State::StatePair State::StateImpl<T>::HandleInput(sf::Event & ev, float dt)
{
	return T::HandleInput(ev,dt);
}

template<typename T>
sf::View & State::StateImpl<T>::GetView()
{
	return T::GetView();
}

template<typename T>
inline bool State::StateImpl<T>::UpdateLogic(float dt)
{
	return true;
}

template<typename T>
inline bool State::StateImpl<T>::SanityCheck()
{
	return true;
}


#endif //STATE_HPP