/// @file

#ifndef __EDITOR_HPP
#define __EDITOR_HPP

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <cstring>
#include "World.hpp"
#include "AssetManager.hpp"
#include "ScreenObject.hpp"

/// \brief
/// World Editor
/// \details
/// This class features the ability to edit worlds. This is done using a Drag & Drop interface
/// enabling both players and developers to easily create playable worlds.
class Editor {
private:
	AssetManager & assets;											//!< The AssetManager to use for gathering the needed textures.
	World world;													//!< The World to edit.
	sf::View & view;												//!< The view to use for scrolling through the world.
	sf::RectangleShape tileSelectionBar;

	std::vector< SelectableObject > objects;						//!< Vector with all placeable objects.

	bool isFirstOneSelected(std::vector<SelectableObject> & tiles);
public:
	Editor( AssetManager & assets, const std::string & worldFileName, sf::View & view );

	void editingDone();
	void handleInput(sf::RenderWindow & window);

	void loadObjects(const std::string & editorConfigName = "Editor/editorObjects.txt");

	void draw( sf::RenderWindow & window );
	void drawTileBar( sf::RenderWindow & window );
	void scrollTileBar( const uint_fast16_t & mouseWheelDelta );
};

#endif // Editor.hpp