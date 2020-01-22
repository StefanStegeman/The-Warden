/// @file

#ifndef __CHARACTER_HPP
#define __CHARACTER_HPP

#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "SpriteAnimation.hpp"
#include "LootDrop.hpp"
#include "Items.hpp"
#include "keybinding.hpp"
#include <memory>
#include <vector>

class Character;

struct CollisionBounds {
	int_fast32_t leftCollisionBound;
	int_fast32_t rightCollisionBound;
};

/// \brief
/// Physicscomponent for Characters.
/// \details
/// This class is responsible for updating and managing physics for a Character.
class PhysicsComponent {
	protected:
		sf::Clock clock;

		enum class states { JUMPING, STANDING, FALLING, INSIDE};
		states state = states::FALLING;
		
		bool bottomCollision, topCollision, leftCollision, rightCollision;
		bool hasResistance = false;
	public:
		virtual void processPhysics(sf::Vector2f & velocity);
		virtual void processCollisions(World & world, sf::Vector2f & position, const sf::Vector2f & dimensions, CollisionBounds & collisionBounds, std::vector<Character> & characters);
		virtual void processVelocity(sf::Vector2f & direction, sf::Vector2f & velocity);

		PhysicsComponent & operator=(PhysicsComponent lhs){
			clock = lhs.clock;
			state = lhs.state;
			bottomCollision = lhs.bottomCollision;
			topCollision = lhs.topCollision;
			leftCollision = lhs.leftCollision;
			rightCollision = lhs.rightCollision;
			hasResistance = lhs.hasResistance;
			return *this;
		}
};

/// \brief
/// Inputcomponent for Characters.
/// \details
/// This class is responsible for managing input for a Character.
class InputComponent {
	protected:
		World & world;
		std::vector<Character> & characters;
	public:
		InputComponent(World & world, std::vector<Character> & characters):
			world(world),
			characters(characters)
		{}
		virtual void processInput(const sf::Vector2f & position, sf::Vector2f & direction, std::array< KeyBinding, 3 > & keys) = 0;
		virtual void processItemUsage(std::vector<std::shared_ptr<Item>> & items, Character * ownCharacter) {}
};

/// \brief
/// Graphicscomponent for Characters.
/// \details
/// This class is responsible for managing all graphics related actions.
class GraphicsComponent {
	protected:
		sf::Sprite sprite;
	public:
		GraphicsComponent(const std::string & assetName, AssetManager & assets);

		virtual void processGraphics(sf::RenderWindow & window, const sf::Vector2f & position, sf::View & view) = 0;
		virtual sf::Vector2f getDimensions();
};

/// \brief
/// Character
/// \details
/// This class features a Character. Every character is the same, except for the (possibly) different components.
/// The functioning of a Character is defined completely in its components; The component pattern has been used here.
class Character {
	private:
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Vector2f direction;

		LootDrop lootDrop;
		std::vector<std::shared_ptr<Item>> items;
		bool isPlayerType;

		int_fast16_t experiencePoints = 0;
		int_fast8_t health = 100;
		sf::RectangleShape healthBar;

		std::shared_ptr<InputComponent> input;			//!< A smart pointer to an on the heap allocated InputComponent.
		std::shared_ptr<PhysicsComponent> physics;		//!< A smart pointer to an on the heap allocated PhysicsComponent.
		std::shared_ptr<GraphicsComponent> graphics;	//!< A smart pointer to an on the heap allocated GraphicsComponent.

		CollisionBounds collisionBounds;
	public:
		Character(sf::Vector2f position, std::shared_ptr<InputComponent> input, std::shared_ptr<PhysicsComponent> physics, std::shared_ptr<GraphicsComponent> graphics, std::shared_ptr<Item> startItem, const bool isPlayerType = false);

		void update(sf::RenderWindow & window, World & world, std::vector<Character> & characters, std::array< KeyBinding, 3 > & keys);
		bool isAlive();

		bool isPlayer() const;

		sf::Vector2f getPosition() const;
		sf::FloatRect getBounds() const;

		void attack();
		void draw(sf::RenderWindow & window, sf::View & view);

		bool operator!=(const Character & lhs);
		bool operator==(const Character & lhs);
		Character & operator=(Character lhs);

		int_fast16_t getExperience() const;
		void setExperience(const int_fast16_t & experiencePointsToAdd);
		int_fast8_t getHealth() const;
		void setHealth(const int_fast8_t newHealth);
};

#endif //__CHARACTER_HPP