#include "StateMachine.hpp"

StateMachine::StateMachine(Game & game, Interface & interface, Editor & editor, Settings & settings):
	stateDependantObjects(game, interface, editor, settings),
	currentState(std::make_shared<MenuState>())
{ 
	interface.initialize(this); 
	settings.initialize(this);
}

void StateMachine::changeState(std::shared_ptr<State> newState){
	currentState = newState;
}

std::shared_ptr<State> StateMachine::getCurrentState(){
	return currentState;
}

void StateMachine::handleInput(const sf::Event & event, sf::View & view){
	ViewObjects viewObjects(view, event);
	currentState->handleInput(stateDependantObjects, viewObjects);
}

void StateMachine::handleEvent(const sf::Event & event, sf::View & view){
	ViewObjects viewObjects(view, event);
	currentState->handleEvent(stateDependantObjects, viewObjects);
}

void StateMachine::display(const sf::Event & event, sf::View & view){
	ViewObjects viewObjects(view, event);
	currentState->display(stateDependantObjects, viewObjects);
}