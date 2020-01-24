/// @file
#include <iostream>
#include "Player.hpp"

/// \brief
/// Draw the Character.
/// \details
/// This function draws the Characte in the RenderWindow and sets the View to the position
/// of the Character to keep the player centered.
void PlayerGraphics::processGraphics(sf::RenderWindow & window, const sf::Vector2f & position, sf::View & view){
	sprite.setPosition(position);
	if(position.y < 300){
		view.setCenter(sf::Vector2f(position.x, position.y));
	} else {
		view.setCenter(sf::Vector2f(position.x, 300));
	}
	window.draw(sprite);
}

AnimatedPlayerGraphics::AnimatedPlayerGraphics(const std::string & assetName, AssetManager & assets, SpriteCharacter & characterData):
	AnimatedGraphicsComponent(assetName, assets, characterData)
{}

void AnimatedPlayerGraphics::processViewChanges(sf::View & view, const sf::Vector2f & position){
	if(position.y < 300){
		view.setCenter(sf::Vector2f(position.x, position.y));
	} else {
		view.setCenter(sf::Vector2f(position.x, 300));
	}
}

PlayerGraphics::PlayerGraphics(const std::string & assetName, AssetManager & assets):
	GraphicsComponent(assetName, assets)
{}

void PlayerInput::processInput(const sf::Vector2f & position, sf::Vector2f & direction, std::vector<KeyBinding> & keys){
	direction.x = 0;	//Stand still
	direction.y = 0;
	if(sf::Keyboard::isKeyPressed(keys[0].getKey())){
		direction.x = -1;
	}
	if(sf::Keyboard::isKeyPressed(keys[1].getKey())){
		direction.x = 1;
	}
	if(sf::Keyboard::isKeyPressed(keys[2].getKey())){
		direction.y = -1;
	}
	if(sf::Keyboard::isKeyPressed(keys[3].getKey())){

	}
}

/// \brief
/// Adds a tile in the world
/// \details
/// This is called by the game, it adds a new tile in the vector tiles in the world,
/// on the location of the mouse in the window's view (so not the normal pixel locations)
/// the new tile is a 'crate' object
void PlayerInput::addTile(const sf::Event & event, World & world, sf::RenderWindow & window, sf::View & view){
	sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);		//gets to position based on the mouse coordinates in a view
	world.addTile("crate", position);
}

/// \brief
/// Deletes a tile in the world
/// \details
/// This is called by the game, and it deletes the object where the mouse is located on.
/// The mouse position is called by mapPixelToCoords which uses view to get the right location
/// and not the mouse window location.
void PlayerInput::deleteTile(const sf::Event & event, World & world, sf::RenderWindow & window, sf::View & view){
	std::vector<Tile> & tiles = world.getTiles();
	for(int_fast8_t i = tiles.size() - 1; i >= 0; i--){
		if(tiles.at(i).getBounds().contains(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)))){
			tiles.erase( std::find(tiles.begin(), tiles.end(), tiles.at(i)) );
		}
	}
}

void PlayerInput::processItemUsage(std::vector<std::shared_ptr<Item>> & items, Character * ownCharacter){
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		if(items.at(ownCharacter->getSelectedItemNumber())->use(ownCharacter, characters) && !ownCharacter->getSelectedItem()->isWeapon()){			//If item is broken
			items.erase(std::find(items.begin(), items.end(), items.at(ownCharacter->getSelectedItemNumber())));
			ownCharacter->getSelectedItemNumber()--;
		}
		if(ownCharacter->getSelectedItem()->isWeapon()){
			ownCharacter->getGraphics()->setFightAnimation();
		}
	}
}

void PlayerInput::handleEvent(const sf::Event & event, int_fast16_t & selectedItem){
	if(event.type == sf::Event::MouseWheelMoved){	
		selectedItem += event.mouseWheel.delta;
	}
}

void PlayerInput::handleInteraction(World & world){
	std::vector<Tile> & tiles = world.getTiles();
	for(int_fast8_t i = tiles.size() - 1; i >= 0; i--){
		if(tiles.at(i).isInteractable()){
			if(tiles.at(i).isPassageWay()){
				std::cout<<"Deur\n";
			}
		}
	}
}