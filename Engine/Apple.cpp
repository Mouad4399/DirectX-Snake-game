#include "Apple.h"
#include <ctime>
#include <vector>
#include <utility> // for std::pair
#include "Graphics.h"
#include "Colors.h"


void Apple::replaceApple(std::vector<std::pair<double, double>> emptypositions)
{
	srand(time(0));
	//srand(35);
	int randomchoice = rand() % emptypositions.size();
	x = emptypositions[randomchoice].first;
	y = emptypositions[randomchoice].second;
}

void Apple::DrawApple(Graphics &gfx)
{

	if ((x < 0) || (x >= (Graphics::ScreenWidth / dim)) || (y < 0) || y>=(Graphics::ScreenHeight / dim)) {

		//std::cout << "apple is out of the border" << std::endl;
		return;
	}
	// draw the apple
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {

			gfx.PutPixel(x * dim + j, y * dim + i, Colors::Green);


		}
	}
}
