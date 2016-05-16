#ifdef _WIN32
#include "../LinkLibraries.hpp"
#endif //_WIN32

#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "FontManager.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"

int main()
{
	std::ofstream logStream("log.txt");
	cg::logger::init(cg::logger::all, std::cout);
	TextureManager& tm = TextureManager::GetInstance();
	FontManager& fm = FontManager::GetInstance();
	SoundManager& sm = SoundManager::GetInstance();

	





	logStream.close();
}