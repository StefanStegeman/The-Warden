/// @file

#ifndef __ITEMS_HPP
#define __ITEMS_HPP

#include <vector>
#include <cstdint>
#include "Character.hpp"

class Character; 

/// \brief
/// Item
/// \details
/// This class is an abstract virtual class for all items.
class Item{
	public:
		virtual void use(Character * character, std::vector<Character> & characters) = 0;
};

/// \brief
/// Weapon
/// \details
/// This class is responsible for hitting other existing Characters.
class Weapon : public Item{
	private:	
		const int_fast8_t damageFactor;
	public:
		Weapon(const int_fast8_t damageFactor);
		virtual void use(Character * character, std::vector<Character> & characters) override;
};

/// \brief
/// Consumable
/// \details
/// This class is responsible for replenishing health after the Character consumes a consumable.
class Consumable : public Item{
	private:
		const int_fast8_t foodValue;
	public:
		Consumable(const int_fast8_t foodValue);
		virtual void use(Character * character, std::vector<Character> & characters) override;
};

#endif //Items.hpp