/// @file

#ifndef __SCREEN_OBJECT
#define __SCREEN_OBJECT

#include <SFML/Graphics.hpp>
#include "AssetManager.hpp"
#include "FactoryFunction.hpp"

/// \brief
/// Screen Object
/// \details
/// This class implements a ScreenObject that can be drawn, scaled, moved and checked for collisions. 
/// It has an assetName, sprite and boolean to determine wether or not this object can collide with Character types.
class ScreenObject {
	protected:
		std::string assetName;				//!< The AssetName to get corresponding texture from the AssetManager.
		sf::Sprite sprite;					//!< The Sprite this ScreenObject has to draw and manage.
	public:
		ScreenObject(const std::string & assetName, AssetManager & assets, const sf::Vector2f & position = sf::Vector2f(0,0), const float scale = 1, const float rotation = 0);

		std::string getName() const;
		virtual std::string getConfiguration() const;
		sf::FloatRect getBounds() const;

		sf::Vector2f getPosition() const;
		void setPosition(const sf::Vector2f & newPosition);

		void setRotation(const float angle);
		float getRotation() const;

		void setNewScale(const float newScale);
		sf::Vector2f getScale() const;

		virtual void draw(sf::RenderWindow & window) const;
};

/// \brief
/// Selectable Object
/// \details
/// This class implements a ScreenObject that can be drawn, scaled, moved, clicked and checked for collisions. 
/// It has an assetName, sprite and boolean to determine wether or not this object can collide with Character types.
class Tile : public ScreenObject {
	private:
		bool followMouse = false;			//!< Wether or not to follow the mouse's position.
		bool collidable = true;					//!< Wether or not Character types should collide with this Tile.
		bool hasBeenAdded = false;			//!< Wether or not this objec has been added to the tiles of the world; if it's part of the world.
	public:
		Tile(const std::string & assetName, AssetManager & assets, const sf::Vector2f & position = sf::Vector2f(0,0), const float scale = 1, const bool collidable = true, const float rotation = 0);

		virtual std::string getConfiguration() const override;

		bool isPartOfWorld() const;
		void makePartOfWorld(const bool & added);

		bool isCollidable() const;
		void setCollidable(const bool newCollidable);

		bool setFollowMouse(const bool follow);
		bool isFollowingMouse() const;

		void move(const sf::Vector2f & position);

		Tile& operator=(Tile lhs);
		bool operator==(Tile lhs) const;
		bool operator<(Tile lhs) const;

};

#endif //__SCREEN_OBJECT