#include "main.hpp"
#include "Game.hpp"

int main(int argc, char** argv) {
	UNUSED(argc);
	UNUSED(argv);

	Game* game = new Game();
	game->startWithTimeStep(1.0f/60.0f);

	std::cout << "reached end" << std::endl;
	return 0;
}
