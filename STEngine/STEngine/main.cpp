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
	TestState()
	{
		_test = sf::Sprite(TextureManager::GetInstance()["test"]);
	}
	virtual ~TestState()
	{

	}
	bool Draw(sf::RenderWindow& win)
	{
		win.draw(_test);
		return true;
	}
	bool HandleInput(sf::Event& ev)
	{

		return true;
	}
	sf::View& GetView()
	{
		return _view;
	}
	sf::Sprite _test;
	sf::View   _view;
};

int main()
{
	std::ofstream logStream("log.txt");
	cg::logger::init(cg::logger::all & ~cg::logger::note1, std::cout);
	TextureManager& tm = TextureManager::GetInstance();
	FontManager& fm = FontManager::GetInstance();
	SoundManager& sm = SoundManager::GetInstance();

	tm.MakeTexture("test", "test.jpg");
	SFMLApplication app(std::make_shared<sf::RenderWindow>(
		sf::VideoMode(800,600,32),"TestWindow"), 
		SFMLApplication::MakeState<TestState>());

	app.Start();



	logStream.close();
}