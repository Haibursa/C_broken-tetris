#include "piece.h"
#include<allegro5/allegro_primitives.h>

piece::piece() {
	x = 0;
	y = 0;
	spin = 1;
	landed = false;
}

piece::piece(int xpos, int ypos) {
	x = xpos;
	y = ypos;
	landed =false;
	spin = 1;

}
void piece::move() {
	y += 3;
}
int piece::turn(char dir) {
	return 0;
}
bool piece::isLanded() {
	return landed;
}
void piece::draw(int map[10][16]) {

}

int piece::geyY() {
	return y;
}
int piece::getX() {
	return x;
}
//this paints the block's final postition
//to the acutal map before the piece is destroyed
void piece::land(int map[10][16]) {

}

bool piece::checkCollisison(int grid[10][16])
{
	return 0;
}

//move the piece based on keyboard input
void piece::MoveSideways(char dir, int grid[10][16]) {

}

