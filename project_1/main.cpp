#include "globals.h"
#include "Player.h"
#include "Game.h"
#include "Arena.h"
#include "Rat.h"
#include "History.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////


int main()
{
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	Game g(10, 12, 10);

	// Play the game
	g.play();
}
