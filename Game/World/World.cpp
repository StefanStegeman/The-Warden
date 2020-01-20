/// @file

#include "World.hpp"
#include "FactoryFunction.hpp"
#include "Editor.hpp"

bool sortByPosition(Tile &lhs, Tile &rhs) { return lhs.getPosition().x < rhs.getPosition().x; }
bool sortByLayer(Tile &lhs, Tile &rhs) { return lhs.getWindowLayer() < rhs.getWindowLayer(); }

/// \brief
/// Create an instance.
/// \details
/// This creates aa world. After initialization, the world configuration file is automatically read. Hence
/// the entire world is loaded after constructing.
/// @param assets The AssetManager to use to retrieve assets (in this case only tiles).
/// @param worldFileName The filename of the world to load. Can be both a new and existing file.
/// @param view The view to use for scrolling through the world.
World::World(AssetManager & assets):
	assets(assets)
{}

/// \brief
/// Load world from configuration.
/// \details
/// This loads a world from a world configuration file. The configuration file should follow a specific syntax.
/// ~~~~~{.txt}
/// background
/// (-1,498) grass 1 1.000000
/// (50,454) tree1 0 1.000000
/// (127,498) grass 1 1.000000
/// ~~~~~
/// This means the background name for this world is called "background". The name "background", can be used with the AssetManager
/// to retrieve its texture from the map. After the texture has been retrieved, one can create a sprite, set the position and draw
/// it on screen. All tiles are read using loadTile().
void World::loadWorld(const std::string & fileName){
	worldFileName = fileName;
	std::ifstream worldFile(worldFileName);
	if(!worldFile){
		throw fileNotFound(worldFileName);
	}
	worldFile >> backgroundName;
	setBackground(backgroundName);
	if ((backgroundName.find("(") != std::string::npos)){
		std::cout << "(!)-- No background specified! World configuration files should always start with a background name." << std::endl;
		worldFile.seekg(0);
	}
	while (!isEmpty(worldFile)){
		try {
			loadTile(worldFile);
		} catch(std::ifstream::failure & e){
			std::cerr << "(!)-- Exception opening, reading or closing file\n";
		} catch (const std::exception & problem){
			std::cerr << "(!)-- " << problem.what() << " in file " << __FILE__ << " at line " << __LINE__ << "." << std::endl;
			//continue;
		} catch (...){
			std::cerr << "(!)-- Something went wrong in " << __FILE__ << " at line " << std::to_string(__LINE__) << std::endl;
		}
	}
	std::cout << "(i)-- Loaded world into memory." << std::endl;
}

/// \brief
/// Load tile.
/// \details
/// This reads the configuration for one tile from a world configuration file. The configuration file should follow a specific syntax.
/// ~~~~~{.txt}
/// background
/// (-1,498) grass 1 1.000000
/// (50,454) tree1 0 1.000000
/// (127,498) grass 1 1.000000
/// ~~~~~
/// This means the background name for this world is called "background". The name "background", can be used with the AssetManager
/// to retrieve its texture from the map. After the texture has been retrieved, one can create a sprite, set the position and draw
/// it on screen. Each tile itself needs a position (in this case a sf::Vector2f()), assetName, collidable option and scale in the world.
/// All these parameters are used to construct a Tile that's stored in the tiles vector.
/// \exception endOfFile() End of file occured. This only happens when there's a mistake in the configuration file. Before reading another object, the program
/// first checks if there's no end of file. However, when there's a mistake in the configuration file, the end of file could occur earlier.
/// \exception invalidPosition() The position given is invalid. This most likely is a syntax error.
void World::loadTile(std::ifstream & input){
	std::string assetName;
	sf::Vector2f position;
	float scale, rotation;
	int windowLayer;
	bool collidable;
	try {
		input >> position >> assetName >> collidable >> scale >> rotation >> windowLayer;
		tiles.push_back(Tile(assetName, assets, position, scale, collidable, rotation, windowLayer));
	} catch (...){
		std::cerr << "(!)-- Syntax mistake in configuration file: \n(" << position.x << ',' << position.y << ") " << assetName << ' ' << collidable << ' ' << scale << ' ' << rotation << ' ' << windowLayer << std::endl;
		std::cerr << "      Note that world configuration files shouldn't end with a newline character." << std::endl;
	}
}

/// \brief
/// Loading done.
/// \details
/// This does everything that needs to be done to save a world. This currently only consists of sorting the tiles.
/// \exception sortingFailed() The tiles could not be sorted. This is most likely because of a std::bad_alloc.
void World::sortWorld(){
	try {
		if(std::is_sorted(tiles.begin(), tiles.end())){
			std::cout << "(i)-- World already sorted!" << std::endl;
		} else {
			std::sort(tiles.begin(), tiles.end(), sortByPosition);
			std::cout << "(i)-- Sorted world!" << std::endl;
		}
	} catch (...){
		throw sortingFailed(__FILE__, __LINE__);
	}
}

/// \brief
/// Draw world.
/// \details
/// This draws the world to the screen. More specifically, it draws the objects that are currently in view to the screen.
/// @param window The window to draw the world to.
void World::draw(sf::RenderWindow & window, sf::View & view, const int_fast8_t windowLayer){
	if(windowLayer == 0){
		background.setPosition((window.getView().getCenter().x-(window.getView().getSize().x*0.5)),(window.getView().getCenter().y-(window.getView().getSize().y*0.5)));
		window.draw(background);
	}

	int_fast32_t leftSide = view.getCenter().x - (view.getSize().x / 2) - 300;
	auto leftIterator = std::find_if(tiles.begin(), tiles.end(), [&leftSide](const Tile & tile)->bool{return tile.getPosition().x > leftSide;});

	int_fast32_t rightSide = view.getCenter().x + (view.getSize().x / 2);
	auto rightIterator = std::find_if(leftIterator, tiles.end(), [&rightSide](const Tile & tile)->bool{return tile.getPosition().x > rightSide;});

	std::for_each(
		leftIterator,
		rightIterator,
		[&window, &windowLayer](Tile & tile){
			if(windowLayer == tile.getWindowLayer()){
				tile.draw(window);
			}
		}
	);
}

/// \brief
/// Add tile to world.
/// \details
/// This adds an object to the world by pushing back to the vector containing all tiles.
/// @param object The Tile to add to the world.
void World::addTile(Tile object){
	tiles.push_back(object);
	sortWorld();
}

/// \brief
/// Get tiles.
/// \details
/// This returns a refrence to all tiles in the world.
std::vector<Tile> & World::getTiles(){
	return tiles;
}

/// \brief
/// Save world to configuration file.
/// \details
/// This function stores the configuration of every tile in the world configuration file of this world.
/// That's done by calling getConfiguration() for all tiles in the tiles vector.
void World::saveWorld(){
	try {
		sortWorld();
		std::ofstream worldFile (worldFileName, std::ofstream::out);
		std::cout << "(i)-- Saving world to " << worldFileName << std::endl;

		worldFile << backgroundName << std::endl;
		for(const Tile & tile : tiles ){
			worldFile << tile.getConfiguration() << std::endl;
		}

		worldFile.close();
	} catch(const sortingFailed & error){
		std::cerr << error.what();
	} catch (const std::exception & error){
		std::cerr << "(!)-- " << error.what() << std::endl;
	} catch (...){
		std::cerr << "(!)-- Something went wrong..." << std::endl;
	}
	std::cout << "(i)-- Saving world succesful!" << std::endl;
}

/// \brief
/// Change background
/// \details
/// This function changes the background to the texture corresponding to the passed parameter.
/// @param backgroundName The name to use for retrieving the texture from the AssetManager.
void World::setBackground(const std::string & backgroundName){
	background.setTexture(assets.getTexture(backgroundName));
}
