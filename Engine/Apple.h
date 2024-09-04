#pragma once


#include "Graphics.h"
#include <time.h>
#include <vector>
#include <utility> // for std::pair
class Apple {
public:

	double x;
	double y;
	int dim=32;
	/*Apple (){
		replaceApple();
	}*/
	void replaceApple(std::vector<std::pair<double, double>> emptypositions);
	void DrawApple(Graphics &gfx);
};
