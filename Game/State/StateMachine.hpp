#ifndef __STATEMACHINE_HPP
#define __STATEMACHINE_HPP

#include <memory>
#include "Game.hpp"
#include "Interface.hpp"
#include "Editor.hpp"
#include "Settings.hpp"
#include "States.hpp"

class Game;
class Interface;
class StateMachine;


class Settings;
class State;

struct StateDependantObjects {
	Game & game;
	Interface & interface;
	Editor & editor;
	Settings & settings;

	StateDependantObjects(Game & game, Interface & interface, Editor & editor, Settings & settings):
		game(game),
		interface(interface),
		editor(editor),
		settings(settings)
	{}
};

struct ViewObjects {
	sf::View & view;
	const sf::Event & event;

	ViewObjects(sf::View & view, const sf::Event & event):
		view(view),
		event(event)
	{}
};

class StateMachine {
	private:
		StateDependantObjects stateDependantObjects;
		sf::View &view;
		Music backgroundMusic = Music("background.wav");
		Music editingMusic = Music("editingBackground.wav");
		sf::Vector2f editPosition = sf::Vector2f{960, 540};
		int prevState=0;

		std::shared_ptr<State> currentState;
	public:
		StateMachine(Game & game, Interface & interface, Editor & editor, Settings & settings, sf::View &view);

		void changeState(std::shared_ptr<State> newState);
		
		std::shared_ptr<State> getCurrentState();
		void handleInput(const sf::Event & event);
		void handleEvent(const sf::Event & event);
		void display(const sf::Event & event);
};

#endif //__STATEMACHING_HPP