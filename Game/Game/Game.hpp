#ifndef __GAME_HPP
#define __GAME_HPP

#include <string>
#include "Editor.hpp"
#include "World.hpp"
#include <SFML/Graphics.hpp>
#include "Character.hpp"

class Game {
	private:
		AssetManager assets;
		World world;
		Editor editor;
		Character speler;

		sf::Event event;

		sf::RenderWindow window{ sf::VideoMode{ 1000, 580 }, "The Warden", sf::Style::Resize};
		sf::View view = sf::View(sf::FloatRect(0.f, 0.f, 1000.f, 580.f));

		enum class states {PLAYING, EDITING, PAUSED};
		states state = states::PLAYING;
	public:
		Game(const std::string & objectConfigurationFile);
		~Game(){
			window.close();
		}

		void startWorld(const std::string & worldName);
		void editWorld(const std::string & worldName);

		void handleInput();
		void display();
};


#endif //__GAME_HPP