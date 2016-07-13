#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "SDL_gamecontroller.h"

class Player
{
public:
	Player();

	static int init();

    void reset(float craftx, float crafty);
	void setName(const char *name);
	void setTexture(m3dTexture *texture);
	void setActive(bool active);
	void setLocal(bool l);
	void setController(SDL_JoystickID id);
	void unsetController();
	
	void setColor(float r, float g, float b);
	void setColor(const float *col);
	const float *getColor() const;
	void bindColor() const;
	void bindColor(float alpha) const;
	
	const m3dTexture &getTexture() const;

	Craft &getCraft();
	
	bool update(float dt);

	bool isActive() const;
	bool isLocal() const;
    bool isFinished() const;
	bool hasController() const;
	bool hasController(SDL_JoystickID id) const;
    const char *getName() const;
	
	void drawHud(const GLint *viewport, int activePlayers, int num);
	
	void setSource(ALuint src);
	ALuint getSource() const;
	
	static void setEngineVolume(float vol);
private:
	char name[20];
	m3dTexture *texture;
	float color[3];
	
	float forceMeter;
	
	Craft craft;

	SDL_JoystickID joyid;

	bool active;
	bool local;
	bool finished;

	static GLuint gauges,needle,fuel;
	static ALuint buffer;
	
	static float engineVolume;
	
	ALuint source;
};

#endif

