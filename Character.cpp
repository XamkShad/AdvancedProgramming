#include "Character.h"

BaseCharacter::BaseCharacter() : 
	pos_x(0), 
	pos_y(0), 
	max_health(100), 
	current_health(100), 
	damage(0){}

BaseCharacter::~BaseCharacter(){ }

void BaseCharacter::move(int dx, int dy) {
	pos_x += dx;
	pos_y += dy;
}

void BaseCharacter::take_damage(int dmg) {
	current_health -= dmg;
}

int BaseCharacter::x() const { return pos_x; }
int BaseCharacter::y() const { return pos_y; }