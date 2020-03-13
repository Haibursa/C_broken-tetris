#include<iostream>
using namespace std;
#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<vector>
#include"line.h"
#include"piece.h"
#include"square.h"
#include"iblock.h"
#include"squiggle.h"
const int SCREEN_W = 500;
const int SCREEN_H = 800;
const int FPS = 60;

//enumeration to help you remember what numbers represent which directions
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,KEY_SPACE
};

bool cullGrid();//the only non-class function

int grid[10][16] = {
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};
int main() {
	//set up allegro
	al_init();
	al_install_keyboard();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	srand(time(NULL));

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_load_font("myfont.ttf", 40, 0);
	bool redraw = true; //variable needed for render section
	bool doexit = false; //handles game loop
	int ticker = 0;
	int moveTimer = 0;
	bool key[4] = { false,false,false,false };
	bool ActivePiece = false;
	int pieceChoice = 0;

	vector<piece*> pieces;
	vector<piece*>::iterator iter;

	//allegro start up stuff
	al_set_target_bitmap(al_get_backbuffer(display));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(font, al_map_rgb(100, 200, 200), 200, 200, NULL, "tetris!");
	al_flip_display();
	al_rest(3);
	while (!doexit) {//game loop
		ALLEGRO_EVENT ev;



		al_wait_for_event(event_queue, & ev);

	//timer section//////////////////////////////////////////
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			ticker++;
			moveTimer++;

			//cull the grid
			if (ticker < 50)
				if (cullGrid() == true)
					ticker = 0;

			if (moveTimer > 5) {
				if (key[KEY_LEFT]) {
					for (iter = pieces.begin(); iter != pieces.end(); iter++) {
						(*iter)->MoveSideways('1', grid);
					}

				}
				if (key[KEY_RIGHT]) {
					for (iter = pieces.begin(); iter != pieces.end(); iter++) {
						(*iter)->MoveSideways('r', grid);
					}

				}
				if (key[KEY_DOWN]) {
					for (iter = pieces.begin(); iter != pieces.end(); iter++) {
						(*iter)->MoveSideways('d', grid);
					}

				}
				if (key[KEY_SPACE]) {
					for (iter = pieces.begin(); iter != pieces.end(); iter++) {
						(*iter)->turn('r');
					}

				}
				moveTimer = 0;
			}
			//create new piece
			if (ActivePiece == false && ticker > 50) {
				pieceChoice = rand() % 3 + 1;//randomly pick next piece type
				if (pieceChoice == 1) {
					line* newline = new line((rand() % 10)*10, 0);
					pieces.push_back(newline);
					cout << "pushed line" << endl;

				}


				ActivePiece = true;
			}

			//check if player has lost
			for (iter = pieces.begin(); iter != pieces.end(); iter++) {
				if ((*iter)->checkCollisison(grid)) {
					cout << "y value is " << (*iter)->geyY() << endl;
					if ((*iter)->checkCollisison(grid)) {
						cout << "game over!" << endl;
						doexit = true;
					}
				}
			}

			//check if a piece has hit the bottom or other piece)
			for (iter = pieces.begin(); iter != pieces.end(); iter++) {
				if ((*iter)->checkCollisison(grid)) {
					(*iter)->land(grid);
					ActivePiece = false;
					iter = pieces.erase(iter);// erase the piece from the vector when it lands(this breaks code)
					break;//fixed it :)
				}
			}


			//moce pieces if they haven't hit bottom
			for (iter = pieces.begin(); iter != pieces.end(); iter++) {
				if (!(*iter)->isLanded())
					(*iter)->move();
			}


			//wipe map
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 16; j++) {
					if (grid[i][j] < 10)
						grid[i][j] = 0;
				}
			}
			redraw = true;

		}//end of timer section

		//render section////////////////////////////////////////
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//draw pieces
			for (iter = pieces.begin(); iter != pieces.end(); iter++) {
				(*iter)->draw(grid);
			}

			for (int i = 0; i < 10; i++) {//do not change number 10
				for (int j = 0; j < 16; j++) {
					if (grid[i][j] == 0)//draws gird lines
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(255, 255, 255), 2);

					if (grid[i][j] == 1) {//draws active line pieces
						al_draw_filled_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(255, 50, 55));
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(25, 50, 55), 4);
					}
					if (grid[i][j] == 10) {//draws inactive line pieces
						al_draw_filled_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(255, 50, 55));
						al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(25, 50, 55), 4);
					}
				}
			}
			al_flip_display();
		}//end render
	}
}

bool cullGrid()
{

	return false;
}
