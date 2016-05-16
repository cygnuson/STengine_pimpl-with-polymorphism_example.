#include "SFMLApplication.hpp"



SFMLApplication::SFMLApplication(std::shared_ptr<sf::RenderWindow> target)
	:
	_target(target), 
	_inputMatrix(InputMatrix::GetInstance()), 
	_texMan(TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan(FontManager::GetInstance()) 
{
	
}


SFMLApplication::~SFMLApplication()
{

}

void SFMLApplication::Start()
{
	while (_target->isOpen())
	{
		sf::Event ev;
		while (_target->pollEvent(ev))
		{
			if (HandleInput(ev))
			{
				/*system input was handled by the function, no need to keep going.*/
				continue;
			}
			else if (_inputMatrix.ProcessEvent(ev))
			{
				/*Input was consumed so we continue here.*/
				continue;
			}
			else
			{
				/*Other eevnt handling.*/
			}
		}
	}
}
bool SFMLApplication::HandleInput(sf::Event& ev)
{

}
void SFMLApplication::Draw()
{

}