//#if _DEBUG
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

//#endif

#include "Game.h"
#include <SDL.h>
// Program entry point
int main(int argc, char *argv[]) {
	//instantiate game class and initialise, then run it
	Game game;
	game.init();
	game.Run();
    return 0;
}