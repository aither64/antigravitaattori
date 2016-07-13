#ifndef _MENU_H_
#define _MENU_H_

#include "SDL.h"

class Menu {
	public:
		Menu(SDL_Window *window);
		static int init();

		int show();
	private:
		void togglePlayer(int p);
		void togglePlayer(Player &plr);
		void drawPlayer(int p);
		void update();

		SDL_Window *window;
		int width, height;
		float anim[4];
		float startanim;
		bool canstart;
		static const float ANIMLEN;
		static GLuint keys[4];
};

#endif

