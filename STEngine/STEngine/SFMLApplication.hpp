#ifndef SFMLAPPLICATION_HPP
#define SFMLAPPLICATION_HPP

#include <functional>

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "FontManager.hpp"
#include "KeyName.hpp"
#include "State.hpp"

template<typename T, typename = decltype(&T::Draw),
	typename = decltype(&T::HandleInput),
	typename = decltype(&T::GetView)
>
static inline std::true_type CanBeStateImpl(int) {};

template<typename T>
static inline std::false_type CanBeStateImpl(...) {};

template<typename T>
struct CanBeState : decltype(CanBeStateImpl<T>(int{}))
{};

class SFMLApplication
{
public:
	SFMLApplication(std::shared_ptr<sf::RenderWindow> target, 
		std::shared_ptr<State> initialState);
	virtual ~SFMLApplication();	
	void Start();
	/**Create a state for use with an app.
	T must have:
		bool Draw(sf::RenderWindow&)
		bool HandleInput(sf::Event&)
		sf::View& GetView()

	T will be initialized with Args...
	*/
	template<typename T, typename...Args>
	static std::shared_ptr<State> MakeState(Args...args);
private:
	/**Determine if the state is sane.  Will also pop states that are qued for
	recycling.*/
	bool StateOk();
	/**Determine if the system is in a state that is drawable.*/
	bool DrawOk();
	/** Handle input is for handling system related input.  Keyboard/Mouse
	input should be handled by a call to _inputMatrix.ProcessEvent;
	\param ev [in] a reference to a polled event.
	\return true if the event was consumed.
	*/
	bool WindowEvent(sf::Event& ev);
	bool InputEvent(sf::Event& ev);
	bool OnResize(sf::Event& ev);
	bool OnClose(sf::Event& ev);
	bool OnMouseEnter(sf::Event& ev);
	bool OnMouseLeave(sf::Event& ev);
	bool OnFocusLost(sf::Event& ev);
	bool OnFocusGained(sf::Event& ev);
	/**Do system draws and then call the top of the stack.*/
	bool Draw();

	/**The actual window of the app.*/
	std::shared_ptr<sf::RenderWindow>   _target;
	/**The texture manager*/
	TextureManager&                     _texMan;
	/**The sound manager*/
	SoundManager&                       _soundMan;
	/**The font manager*/
	FontManager&                        _fontMan;
	/**The current state.*/
	std::shared_ptr<State>              _state;
	/**The current view.*/
	sf::View&                           _view;
};


template<typename T, typename ...Args>
inline std::shared_ptr<State> SFMLApplication::MakeState(Args ...args)
{
	static_assert(CanBeState<T>(), "Class does not have the right functions to"
		" be a state. Needs Draw, HandleInput, and GetView");
	auto ptr = std::make_shared<State>();
	ptr->_self.reset(new State::Model<T>(std::forward<Args>(args)...));
	ptr->_view = &ptr->_self->GetView();
	return ptr;
}

#endif //SFMLAPPLICATION_HPP