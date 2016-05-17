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

/**********************The state system.***************************************
* A: The app keeps a stack of states.  The stack cannot be alterd by a state.
* B: The State's function `State:Flag HandleInput(sf::Event&,float)` can return
*	a flag that will ask the app to push a new state to the stack, or pop one.
*	remember that if the pop signal is returned from HandleInput, the state
*	returned the flag will be popped.
* C:When the HandleInput function rturns the `Push` flag, or any flag that
*	requires a new state, the app will call `std::shared_ptr<State> GetState()`
*	to get the state that is needed to be pushed. The reason is that the
*	alternative is returning a pair on ever call (thousands of calls), thats
*	not a good idea as it would hinder performance.
* D:The ONLY acceptable way to make a state for the system is to call 
*	`State::MakeState<SomeState>(SomeState's args)`. SomeState is not required
*	to inherit anything. but it MUST have the following functions defined:
*
*	     State::StackPair Draw(sf::RenderWindow&,float)
*	     bool HandleInput(sf::Event&,float)
*	     sf::View& GetView()
*	     bool SanityCheck()
*	     bool UpdateLogic()
*	     std::shared_ptr<State> GetState()
*
*	This list may change, its also at the function comment for 
*		MakeState<..>(...)
* 
* E:The State class is just a wrapper that pipes all calls to the impl in _self
*	which is a pointer to a StateBase which is just a virtual wrapper for 
*	Model<Type> so that it can be stored unirsally in the _self ptr without
*	having a million different State types to keep track of.  This means that
*	ever call on the state from outside that state will have one regular funct
*	call and one additional virtual call. This means that its best to call from
*	inside the state as often as able.
* 
* 
******************************************************************************/

class State : public std::enable_shared_from_this<State>
{
public:
	enum class Flag : std::uint_fast32_t
	{
		Exit = 1,
		Pop,
		Push,
		None,
	};
	State();
	virtual ~State();
	/**Draw the state. Calls the _self->Draw(...)
	\return A pair that holds a flag and a pointer to a state. the ptr to state
	is only used when the flag requires a new state.*/
	bool Draw(sf::RenderWindow& win, float dt);
	/**Handle input. Calls _self->HandleInput(...)*/
	State::Flag HandleInput(sf::Event& ev, float dt);
	/**Get the view of this state (will get from _self)*/
	sf::View& GetView();
	/**Make sure the stat is sane.*/
	bool SanityCheck();
	/**Update the state logics.*/
	bool UpdateLogic(float dt);
	/**Get the new state. This is how the app gets the new states when a state
	needs to create another, and sends the Flag::Push as a return of 
	handleinput().*/
	std::shared_ptr<State> GetState();
	/**Create a state for use with an app.
	T must have:
	State::StackPair Draw(sf::RenderWindow&,float)
	bool HandleInput(sf::Event&,float)
	sf::View& GetView()
	bool SanityCheck()
	bool UpdateLogic()
	std::shared_ptr<State> GetState()

	T will be initialized with Args...
	*/
	template<typename T, typename...Args>
	static std::shared_ptr<State> MakeState(Args...args);
protected:
	/**The StateBase is an abstraction wrapper for Model<T> so that the State
	can practicaly impliment any other state provided that it has the proper
	functions impl.*/
	class StateBase
	{
	public:
		/**If returning somthing that does not require a state ptr, use nullptr
		*/
		virtual bool Draw(sf::RenderWindow& win, float dt) = 0;
		virtual State::Flag HandleInput(sf::Event& ev, float dt) = 0;
		virtual sf::View& GetView() = 0;
		virtual bool UpdateLogic(float dt) = 0;
		virtual bool SanityCheck() = 0;
		virtual std::shared_ptr<State> GetState() = 0;
	};

	/**The actual impl that will inherit from the class that is created with 
	State::MakeState<SomeClass>(SomeClass's args...).*/
	template<typename T>
	class StateImpl : public StateBase, public T
	{
	public:
		template<typename...Args>
		StateImpl(Args...args);
		virtual bool Draw(sf::RenderWindow& win, float dt);
		virtual State::Flag HandleInput(sf::Event& ev, float dt);
		virtual sf::View& GetView();
		virtual bool UpdateLogic(float dt);
		virtual bool SanityCheck();
		virtual std::shared_ptr<State> GetState();
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
State::Flag State::StateImpl<T>::HandleInput(sf::Event & ev, float dt)
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

template<typename T>
inline std::shared_ptr<State> State::StateImpl<T>::GetState()
{
	return T::GetState();
}


#endif //STATE_HPP