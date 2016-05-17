#ifdef _WIN32
#include "../LinkLibraries.hpp"
#endif //_WIN32

#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "FontManager.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"

#include "SFMLApplication.hpp"

class TestState
{
public:
	TestState(const std::string& tex)
	{
		_test = sf::Sprite(TextureManager::GetInstance()[tex]);
	}
	virtual ~TestState()
	{

	}
	bool Draw(sf::RenderWindow& win, float dt)
	{
		win.draw(_test);
		return true;
	}
	bool SanityCheck() {
		return true;
	}
	bool UpdateLogic(float dt)
	{
		return true;
	}
	State::Flag HandleInput(sf::Event& ev, float dt)
	{
		if (ev.type == sf::Event::MouseButtonReleased)
		{
			_stateInQuestion = State::MakeState<TestState>("test2");
			return State::Flag::Push;
		}
		if (ev.type == sf::Event::KeyReleased)
		{
			return State::Flag::Pop;
		}
		return State::Flag::None;
	}
	std::shared_ptr<State> GetState()
	{
		return _stateInQuestion;
	}
	sf::View& GetView()
	{
		return _view;
	}
	sf::Sprite             _test;
	sf::View               _view;
	std::shared_ptr<State> _stateInQuestion;
};

int main()
{
	std::ofstream logStream("log.txt");
	cg::logger::init(cg::logger::all & ~cg::logger::note1, std::cout);
	TextureManager& tm = TextureManager::GetInstance();
	FontManager& fm = FontManager::GetInstance();
	SoundManager& sm = SoundManager::GetInstance();

	tm.MakeTexture("test", "test.jpg");
	tm.MakeTexture("test2", "test2.jpg");
	SFMLApplication app(std::make_shared<sf::RenderWindow>(
		sf::VideoMode(800,600,32),"TestWindow"), 
		State::MakeState<TestState>("test"));

	app.Start();



	logStream.close();
}