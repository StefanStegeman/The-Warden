/// @file

#ifndef __WORLD_HPP
#define __WORLD_HPP

#include <vector>
#include "ScreenObject.hpp"
#include "Exceptions.hpp"
#include <string>

/// \brief
/// World
/// \details
/// This class is the entire world. It consists of all the tiles specified in the world configuration file.
/// It's responsible for loading, drawing and saving the world.
class World {
	private:
		AssetManager & assets;							//!< The AssetManager to use for gathering the needed textures.
		std::vector<Tile> tiles;						//!< The tiles the world consists of.
		std::vector<InteractableObject> interactables; 	//!< The objects which a Character can interact with.

		std::string worldFileName;						//!< The filename of the world configuration file.
		std::string backgroundName;						//!< The backgroundname used for retrieving the texture from the AssetManager.
		sf::Sprite background;							//!< The background sprite.

		void loadWorld();
		void loadObject(std::ifstream & input);
		void sortWorld();
	public:
		World(AssetManager & assets);

		void saveWorld();
		void loadWorld(const std::string & fileName);

		void addTile(Tile object);
		void addInteractable(InteractableObject & object);
		void setBackground(const std::string & backgroundName);

		std::vector<Tile> & getTiles();
		std::vector<InteractableObject> & getInteractables();
		void draw(sf::RenderWindow & window, sf::View & view, const int_fast8_t windowLayer);

		World & operator=(World lhs){
			tiles = lhs.tiles;
			worldFileName = lhs.worldFileName;
			backgroundName = lhs.backgroundName;
			background = lhs.background;
			return *this;
		}
};

#endif //__WORLD_HPP