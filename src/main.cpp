#define GL_GLEXT_PROTOTYPES

#include "SDL.h"
#include "SDL_opengl.h"
//#include "SDL_gamecontroller.h"
#include <AL/alut.h>
#include <AL/al.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>

#include <getopt.h>

#include "antigrav.h"
#include "extensions.h"

void mainLoop(void);

#ifdef HAVE_MULTITEX
MFNGLMULTITEXCOORD2FVPROC mglMultiTexCoord2fv = NULL;
MFNGLACTIVETEXTUREARBPROC mglActiveTextureARB = NULL;
#endif

bool opt_fullscreen = true;
bool opt_fsaa = false;
int opt_width = 1024;
const char *help_msg =
"Usage: antigrav [options]\n\
Options:\n\
  -h, --help\t\tprint this help, then exit\n\
  -f, --fsaa\t\tenable full screen antialiasing\n\
  -w, --windowed\trun in windowed mode\n\
  -r, --resolution=RES\tset resolution to RES, 1024 for 1024x768, 800 for 800x600, etc\n";

int parse_args(int argc, char *argv[])
{
	
	while(true)
	{
		int option_index = 0;
		static struct option long_options[] = {
			{"help", no_argument, 0, 'h'},
			{"fsaa", no_argument, 0, 'f'},
			{"windowed", no_argument, 0, 'w'},
			{"resolution", required_argument, 0, 'r'},
			{0, 0, 0, 0}
		};

		int c = getopt_long(argc, argv, "hfwr:", long_options, &option_index);
		if(c == -1)
			break;
		
		switch(c)
		{
			case 'h':
				printf(help_msg);
				return 1;
				break;
			case 'f':
				opt_fsaa = true;
				break;
			case 'w':
				opt_fullscreen = false;
				break;
			case 'r':
				opt_width = atoi(optarg);
				break;
			default:
				puts(help_msg);
				return 1;
				break;
		}
	}
	
	return 0;
}

void cleanup()
{
	SDL_Quit();
	alutExit();
}

int main(int argc, char *argv[])
{
	SDL_Window *screen;
	
	(void)argc;
	(void)argv;
	
	if(parse_args(argc, argv)) return 0;
	
	atexit(cleanup);

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		fprintf(stderr, "Can't initialize SDL: %s\n", SDL_GetError());
		return -1;
	}
	
// 	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
// 	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
// 	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
// 	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	if(opt_fsaa)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	}

	Uint32 flags = SDL_WINDOW_OPENGL;
	if(opt_fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
	
	int width = opt_width;
	int height = width * 3 / 4;

	screen = SDL_CreateWindow(
		"antigravitaattori",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height,
		flags
	);

	if(screen == NULL)
	{
		fprintf(stderr, "Can't set video mode: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext(screen);
	
#ifdef HAVE_MULTITEX
	mglActiveTextureARB = (MFNGLACTIVETEXTUREARBPROC)SDL_GL_GetProcAddress("glActiveTextureARB");
	mglMultiTexCoord2fv = (MFNGLMULTITEXCOORD2FVPROC)SDL_GL_GetProcAddress("glMultiTexCoord2fv");
	if(mglActiveTextureARB == NULL || mglMultiTexCoord2fv == NULL)
	{
		fprintf(stderr, "Multitexturing extensions not available!\n");
		return -1;
	}
#endif

	if(alutInit(&argc, argv) != AL_TRUE)
	{
		fprintf(stderr, "Can't initialize OpenAL: %s\n", alutGetErrorString(alutGetError()));
		return -1;
	}

	if(chdir(DATADIR) != 0)
	{
		if(chdir("../data") != 0)
		{
			fprintf(stderr, "Can't find data directory in %s or %s\n", DATADIR, "../data");
			return -1;
		}
	}
	
	// disable mouse cursor
	SDL_ShowCursor(SDL_DISABLE);

	Game &game = Game::getInstance();

	if(game.init(screen)) return 1;
	if(Menu::init()) return 1;

	SDL_GameController *controllers[8] = {NULL};
	SDL_GameController *controller = NULL;

	for (int i = 0, j = 0; i < SDL_NumJoysticks(); i++) {
		if (!SDL_IsGameController(i))
			continue;
		
		controller = SDL_GameControllerOpen(i);

		if (controller) {
			controllers[j++] = controller;

		} else {
			fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
		}
	}

	Menu menu(screen);
	while(1) {
		if(menu.show())
			break;
		if(game.gameLoop())
			break;
	}

	for (int i = 0; i < 8; i++) {
		if (!controllers[i])
			break;

		SDL_GameControllerClose(controllers[i]);
	}

	SDL_GL_DeleteContext(glcontext);
	
	return 0;
}

ALuint loadWavBuffer(const char *filename)
{
	ALuint buffer;
	alGenBuffers(1, &buffer);
	if(!alIsBuffer(buffer)) return AL_NONE;
	
	SDL_AudioSpec wav_spec;
	Uint8 *wav_buffer;
	Uint32 wav_length;
	if(SDL_LoadWAV(filename, &wav_spec, &wav_buffer, &wav_length) == NULL)
	{
		fprintf(stderr, "Can't open %s : %s\n", filename, SDL_GetError());
		return AL_NONE;
	}


// 	AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8, and AL_FORMAT_STEREO16.
	int format;
	
	if(wav_spec.channels == 1)
	{
		if(wav_spec.format == AUDIO_U8) format = AL_FORMAT_MONO8;
		else if(wav_spec.format == AUDIO_S16SYS) format = AL_FORMAT_MONO16;
		else
		{
			fprintf(stderr, "Can't open %s : unknown audio format\n", filename);
			SDL_FreeWAV(wav_buffer);
			return AL_NONE;
		}
	} else if(wav_spec.channels == 2)
	{
		if(wav_spec.format == AUDIO_U8) format = AL_FORMAT_STEREO8;
		else if(wav_spec.format == AUDIO_S16SYS) format = AL_FORMAT_STEREO16;
		else
		{
			fprintf(stderr, "Can't open %s : unknown audio format\n", filename);
			SDL_FreeWAV(wav_buffer);
			return AL_NONE;
		}
	} else
	{
		fprintf(stderr, "Can't open %s : unknown audio format\n", filename);
		SDL_FreeWAV(wav_buffer);
		return AL_NONE;
	}

	alBufferData(buffer, format, wav_buffer, wav_length, wav_spec.freq);
	SDL_FreeWAV(wav_buffer);
	
	if(alGetError() != AL_NO_ERROR)
	{
		fprintf(stderr, "Can't open %s : OpenAL error\n", filename);
		return AL_NONE;
	}

	return buffer;
}

