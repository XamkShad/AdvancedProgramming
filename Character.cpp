#include "Character.h"

// Character base class!! //

// In the constructor, set some default values. //
BaseCharacter::BaseCharacter() : 
	pos_x(0), 
	pos_y(0), 
	max_health(100), 
	current_health(100), 
	damage(0){}

BaseCharacter::~BaseCharacter(){ }

// Every character has to have a move method, right?? //
void BaseCharacter::move(int dx, int dy) {
	pos_x += dx;
	pos_y += dy;
}

// And every character takes damage, right?? //
void BaseCharacter::take_damage(int dmg) {
	current_health -= dmg;
}

// Public getter for the coordinates essentially. //
int BaseCharacter::x() const { return pos_x; }
int BaseCharacter::y() const { return pos_y; }