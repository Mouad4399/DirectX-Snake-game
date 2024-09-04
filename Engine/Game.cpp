/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Graphics.h"
#include "Colors.h"

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
// Define directions for movement
void draw(double x,double y,Graphics &gfx);
bool isValid(int x, int y, int width, int height, Snake &snake);

std::vector<std::pair<int, int>> path;
std::vector<std::pair<int, int>> path_to_tail;
const std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
bool nopathfounded = false;
int path_index ;

Graphics* fgfx;
bool bfs(Snake &snake, Apple &apple, bool returnpath);
bool findLongestPath(Snake &snake, Apple &apple);

//bool bfs_tail(Snake snake);

int dim = 32;
bool endgame = false;
bool attackapple= false;
int lullcnt= 0;
double maxsnakediff = 0;

bool loopdetected = false;

bool bfs(Snake &snake,Apple &apple , bool returnpath) {

	if (apple.x < 0 || apple.y < 0) {
		return false;
	}
	
	if (returnpath) path.clear();
	bool pathfounded = false;
	//std::cout << apple.x << "," << apple.y << std::endl;
	std::pair<int, int> start = { snake.shead->x, snake.shead->y };
	std::pair<int, int> target = { apple.x,apple.y };
	int width = Graphics::ScreenWidth / dim;
	int height = Graphics::ScreenHeight / dim;
	std::queue<std::pair<int, int>> q;
	std::unordered_map<int, std::pair<int, int>> cameFrom;
	q.push(start);
	cameFrom[start.first * width + start.second] = { -1, -1 };

	while (!q.empty()) {
		auto x = q.front().first;
		auto y = q.front().second;
		q.pop();
		if (x == target.first && y == target.second) {
			pathfounded = true;
			break;
		};
		for (const auto& dir : directions) {
			int nx = x + dir.first, ny = y + dir.second;
			if ((nx == target.first && ny == target.second )|| isValid(nx, ny, width, height, snake) && cameFrom.find(nx * width + ny) == cameFrom.end()) {
				q.push({ nx, ny });
				cameFrom[nx * width + ny] = { x, y };

				draw(nx, ny,*fgfx);
				//Sleep(1000);
			}
		}
	}
	if (!pathfounded) {
		return false;
	}
	if (!returnpath) {
		return true;
	}

	//std::cout << "--------------------------------------------------------------------------------------" << "\n";
	for (auto at = target; at != std::make_pair(-1, -1); at = cameFrom[at.first * width + at.second]) {
		//if (at.first == -1 && at.second == -1) {
		//	std::cout << "holy shit " << "\n";
		//	return false;
		//	/*nopathfounded = true;
		//	break;*/
		//}
		//std::cout << "( " << at.first << " , " << at.second << " )" << "\n";
		path.push_back(at);

	}
	path_index = path.size() - 2;
	//std::reverse(path.begin(), path.end());
	return true;
}


// Function to perform DFS and find the longest path
void dfs(int x, int y, int tx, int ty, std::unordered_map<int, bool> & visited, std::vector<std::pair<int, int>>& ipath, std::vector<std::pair<int, int>>& longestPath,Snake & snake) {
	// If we have reached the target, check if this path is the longest
	if (x == tx && y == ty) {
		if (ipath.size() > longestPath.size()) {
			longestPath = ipath;
		}
		return;
	}

	// Explore all possible directions
	for (auto& dir : directions) {
		int nx = x + dir.first;
		int ny = y + dir.second;

		// Check if the new position is within bounds and not visited
		if ((nx == tx && ny == ty) || isValid(nx, ny, Graphics::ScreenWidth / dim, Graphics::ScreenHeight / dim, snake) && !visited[nx*(Graphics::ScreenWidth / dim) +ny]) {
			// Mark the cell as visited and add it to the path
			visited[nx*(Graphics::ScreenWidth / dim)+ny] = true;
			ipath.push_back({ nx, ny });

			// Recursively explore this path
			dfs(nx, ny, tx, ty, visited, ipath, longestPath,snake);

			// Backtrack: unmark the cell and remove it from the path
			visited[nx*(Graphics::ScreenWidth / dim)+ny] = false;
			ipath.pop_back();
		}
	}
}

// Function to find the longest path between two points
bool findLongestPath(Snake &snake, Apple &apple) {

	int sx = snake.shead->x;
	int sy = snake.shead->y;
	
	int tx = apple.x;
	int ty = apple.y;

	//int raws = 

	std::unordered_map<int, bool> visited;
	std::vector<std::pair<int, int>> ipath;
	std::vector<std::pair<int, int>> longestPath;

	// Mark the start point as visited and add it to the path
	visited[sx*(Graphics::ScreenWidth / dim) + sy] = true;
	ipath.push_back({ sx, sy });

	// Start DFS from the start point
	dfs(sx, sy, tx, ty, visited, ipath, longestPath,snake);
	std::reverse(longestPath.begin(), longestPath.end());
	path = longestPath;
	path_index = path.size() - 2;

	/*std::cout << "Longest Path: ";
	for (auto& point : path) {
		std::cout << "(" << point.first << ", " << point.second << ") ";
	}
	std::cout << std::endl;*/

	return !path.empty();
}
Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{

	for (int x = 0; x < (Graphics::ScreenWidth / dim); ++x) {
		for (int y = 0; y < (Graphics::ScreenHeight / dim); ++y) {
			emptypositions.push_back({ (double)x, (double)y });
		}
	}
	//gfx.PutPixel(99*Graphics::ScreenWidth / dim,99* Graphics::ScreenHeight / dim, Colors::Yellow);
	fgfx = &gfx;
	//snake.stail = new Node(2, 4);
	//snake.stail->next = new Node(3, 4);
	//snake.stail->next->next = new Node(4, 4);
	//snake.shead = snake.stail->next->next;

	snake.stail = new Node(2, 4);
	snake.stail->next = new Node(3, 4);
	snake.shead = snake.stail->next;



	// don't forget to declare the length
	//snake.length = 3;
	snake.length = 2;

	emptypositions.erase(remove(emptypositions.begin(), emptypositions.end(), std::make_pair((double)2, (double)4)), emptypositions.end());
	emptypositions.erase(remove(emptypositions.begin(), emptypositions.end(), std::make_pair((double)3, (double)4)), emptypositions.end());
	//emptypositions.erase(remove(emptypositions.begin(), emptypositions.end(), std::make_pair((double)4, (double)4)), emptypositions.end());


	//for (int i = 1; i <= 5*dim; ++i) {
	//	snake.AddNodeToHead(4 + i, 4);
	//}	
	//
	//for (int i = 1; i <= 50; ++i) {
	//	snake.AddNodeToHead(4 + 5*dim, 4+i);
	//}
	//for (int i = 1; i <= 50; ++i) {
	//	snake.AddNodeToHead(4 + 5*dim-i, 4+50);
	//}

	//snake.AddNodeToHead(5,4);
	//snake.AddNodeToHead(6,4);
	//snake.AddNodeToHead(7,4);


	//snake.AddNodeToHead(7,5);
	//snake.AddNodeToHead(7,6);
	//snake.AddNodeToHead(7,7);


	//snake.AddNodeToHead(6,7);
	//snake.AddNodeToHead(5,7);
	//snake.AddNodeToHead(4,7);
	//snake.AddNodeToHead(3,7);





	//apple.x = 1*dim;
	//apple.y = 1*dim;
	apple.replaceApple(emptypositions);
	//bfs(snake, apple,true);
	//followthetail = true;


	helperapple.x = -1;
	helperapple.y = -1;

	//endgame = true;
	//attackapple = true;

	Apple snaketail;
	snaketail.x = snake.stail->x;
	snaketail.y = snake.stail->y;

	std::cout << bfs(snake, snaketail, true) << std::endl;
	//std::cout << findLongestPath(snake, apple) << std::endl;
	followthetail = true;
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{

	if (wnd.kbd.KeyIsPressed(VK_UP) && (snake.vy != 1)) {

		snake.vx = 0;
		snake.vy = -1;
	}else if (wnd.kbd.KeyIsPressed(VK_DOWN) && (snake.vy != -1)) {

		snake.vx = 0;
		snake.vy = 1;
	}
	else if (wnd.kbd.KeyIsPressed(VK_LEFT) && (snake.vx != 1)) {

		snake.vx = -1;
		snake.vy = 0;
	}else if (wnd.kbd.KeyIsPressed(VK_RIGHT) && (snake.vx != -1)) {

		snake.vx = 1;
		snake.vy = 0;
	}

	if (snake.shead->x < 0 && snake.shead->x >= (Graphics::ScreenWidth / dim) && snake.shead->y < 0 && snake.shead->y >= (Graphics::ScreenHeight / dim)) {
		snake.GameOver = true;
	}
	//std::cout << "first : "<<path[index].first<<"\n";
	//std::cout << "second : "<< path[index].second<<"\n";
	//if (!nopathfounded ) {
	//std::cout << path[path_index].first << " , "<<path[path_index].second<< "\n";

		/*	tailapple.x = snake.stail->x;
		tailapple.y = snake.stail->y;*/



	//if (/*std::sqrt(std::pow(snake.shead->x -apple.x, 2) + std::pow(snake.shead->y - apple.y, 2)) ==1 &&*/ std::sqrt(std::pow(snake.shead->x - snake.stail->x, 2) + std::pow(snake.shead->y - snake.stail->y, 2)) > 2 && endgame==true) {

	//	//std::cout << "--!-- return to regular" << std::endl;
	//	//endgame = false;
	//	attackapple = true;
	//}
	//else {
	//	attackapple = false;
	//}

	/*if (emptypositions.size() <= 5 + 1 ) {
		std::cout << "!!!!!!!!!!!!!!!!FORCED ENDGAME!!!!!!!!!!!!!!!!!!" << std::endl;
		endgame = true;
	}*/

	//if ( (double)(snake.length/ (int)(0.7 *(Graphics::ScreenHeight*Graphics::ScreenWidth / (dim*dim)))) >= 0.7) {
		//std::cout << "!!!!!!!!!!!!!!!! "<< <<" !!!!!!!!!!!!!!!!!!" << std::endl;
	//	endgame = true;
	//}
	if ((snake.length / (double)(Graphics::ScreenHeight*Graphics::ScreenWidth / (dim*dim)) >=0.7) &&(((Graphics::ScreenHeight*Graphics::ScreenWidth / (dim*dim)) - snake.length) <= 30)) {
		std::cout << "!!!!!!!!!!!!!!!!FORCE ENDGAME DUE TO 30 is left !!!!!!!!!!!!!!!!!!" << std::endl;
		endgame = true;
	}
	if (endgame==true && (emptypositions.size() <= maxsnakediff )) {
		std::cout << "!!!!!!!!!!!!!!!!FORCE EAT!!!!!!!!!!!!!!!!!!" << std::endl;
		attackapple = true;
	}

	if (endgame == true) {

		if (std::floor(std::sqrt(std::pow(snake.shead->x - snake.stail->x, 2) + std::pow(snake.shead->y - snake.stail->y, 2))) > maxsnakediff) {
			lullcnt = 0;
			maxsnakediff = std::floor(std::sqrt(std::pow(snake.shead->x - snake.stail->x, 2) + std::pow(snake.shead->y - snake.stail->y, 2)));
			std::cout << "!!!!!!!!!!!!!!!!collect spaces max :"<<maxsnakediff<<"!!!!!!!!!!!!!!!!!!" << std::endl;
		}
	}


	if (lullcnt>=snake.length + maxsnakediff-1 ) {
		std::cout << "!!!!!!!!!!!!!!!! ATTACK APPLE bc we are doing nothing it is a loop!!!!!!!!!!!!!!!!!!" << std::endl;
		attackapple = true;
		lullcnt = 0;
		maxsnakediff = 0;
	}

	if (snake.shead->x == snake.tx && snake.shead->y == snake.ty) {
		//std::cout << "----------------" << std::endl;
		++loopnum;
		if (loopnum > snake.length) {
			std::cout << "!!!!!!!!!!!!- loop detected -!!!!!!!!!!!!!!!!" << "\n";
			loopdetected = true;

			endgame = true;

			/*std::cout << "!!!!!!!!!!!!- generate helper apple -!!!!!!!!!!!!!!!!" << "\n";*/

			/*helperapple.replaceApple();


			while (!isValid(helperapple.x, helperapple.y, Graphics::ScreenWidth, Graphics::ScreenHeight, snake) || (helperapple.x == apple.x && helperapple.y == apple.y)) {
				helperapple.replaceApple();
			}*/


			helperappleready = true;
			loopnum = 0;
		}
	}
	else {
		loopnum = 0;
	}
	
	//if (wnd.mouse.LeftIsPressed() && helperappleready == true) {


	//	helperapple.x = std::floor(wnd.mouse.GetPosX() / (dim));
	//	helperapple.y = std::floor(wnd.mouse.GetPosY() / (dim));
	//	std::cout << "clicked helperapple (" << helperapple.x << " , " << helperapple.y << ")" << std::endl;
	//	if(!isValid(helperapple.x, helperapple.y, Graphics::ScreenWidth, Graphics::ScreenHeight, snake) /*|| (helperapple.x == apple.x && helperapple.y == apple.y)*/) {
	//		std::cout<<"-!- the clicked place is not permissable" <<std::endl;
	//		helperappleready = true;
	//	}
	//	else { 
	//		helperappleready = false; 
	//		std::cout << "!!!!!!!!!!!!- generate helper apple -!!!!!!!!!!!!!!!!" << "\n";
	//	}
	//	
	//}
	if (path_index < 0) {
		followthetail = true;
	}
	if( followthetail==false){
		snake.vx =  path[path_index].first-snake.shead->x;
		snake.vy = path[path_index].second-snake.shead->y ;

		emptypositions.push_back({ (double)snake.stail->x,(double)snake.stail->y });

		--path_index;
		snake.UpdateMovement();


		emptypositions.erase(remove(emptypositions.begin(), emptypositions.end(), std::make_pair((double)snake.shead->x, (double)snake.shead->y)), emptypositions.end());


		std::cout << "I am on my way to apple" << std::endl;

	}
	else {
		//follow the tail;
		bool goodapple = false;
		if (path_index >= 0) {

	
			snake.vx = path[path_index].first - snake.shead->x;
			snake.vy = path[path_index].second - snake.shead->y;

			emptypositions.push_back({ (double)snake.stail->x,(double)snake.stail->y });

			--path_index;
			snake.UpdateMovement();

			emptypositions.erase(remove(emptypositions.begin(), emptypositions.end(), std::make_pair((double)snake.shead->x, (double)snake.shead->y)), emptypositions.end());
		}
		// here examine if you should keep following the tail or not


		
		if (endgame == true) {
			goodapple = false;
		}
		else {
			goodapple = bfs(snake, apple, true);
		}


		if (attackapple == true) {
			goodapple = findLongestPath(snake, apple);
			//goodapple = bfs(snake, apple, true);
		}


		if (!goodapple) {


			goodapple = bfs(snake, helperapple, true);
			if (!goodapple) {
				Apple snaketail;
				snaketail.x = snake.stail->x;
				snaketail.y = snake.stail->y;

				(endgame == true) ? findLongestPath(snake, snaketail): bfs(snake, snaketail, true);
				//followthetail = true;
				followthetail = !endgame;
			}
			else {

				//snake.print();
				Snake FakeSnake = snake;

				//FakeSnake.dim = 200;

				//std::cout <<"look at this mf : "<< FakeSnake.dim << std::endl;
				Apple FakeSnakeTail;

				for (int i = path.size() - 2; i >= 0; --i) {

					FakeSnake.vx = path[i].first - FakeSnake.shead->x;
					FakeSnake.vy = path[i].second - FakeSnake.shead->y;

					FakeSnake.UpdateMovement();
				}
				//FakeSnake.AddNode();
				FakeSnakeTail.x = FakeSnake.stail->x;
				FakeSnakeTail.y = FakeSnake.stail->y;

				//snake.print();

				if (!bfs(FakeSnake, FakeSnakeTail, false)) {
					//follow the tail until

					std::cout << "(helperapple)good apple but can't get the tail after" << std::endl;

					Apple snaketail;
					snaketail.x = snake.stail->x;
					snaketail.y = snake.stail->y;

					(endgame == true) ? findLongestPath(snake, snaketail) : bfs(snake, snaketail, true);
					followthetail = true;
				}
				else {
					std::cout << "(helperapple)yes it is possible to get the apple and the tail" << std::endl;
					followthetail = false;
				}
			}



			/*Apple snaketail;
			snaketail.x = snake.stail->x;
			snaketail.y = snake.stail->y;

			std::cout << bfs(snake, snaketail, true)<<std::endl;
			followthetail = true;*/
		}
		else {

			//snake.print();
			Snake FakeSnake = snake;

			//FakeSnake.dim = 200;

			//std::cout <<"look at this mf : "<< FakeSnake.dim << std::endl;
			Apple FakeSnakeTail;
			
			for (int i = path.size() - 2; i >= 0; --i) {
				
				FakeSnake.vx = path[i].first - FakeSnake.shead->x;
				FakeSnake.vy = path[i].second - FakeSnake.shead->y;

				FakeSnake.UpdateMovement();
			}
			FakeSnake.AddNode();
			FakeSnakeTail.x = FakeSnake.stail->x;
			FakeSnakeTail.y = FakeSnake.stail->y;

			//snake.print();

			if (!bfs(FakeSnake, FakeSnakeTail, false) ) {
				//follow the tail until

				std::cout << "good apple but can't get the tail after" << std::endl;

				Apple snaketail;
				snaketail.x = snake.stail->x;
				snaketail.y = snake.stail->y;

				(endgame == true) ? findLongestPath(snake, snaketail) : bfs(snake, snaketail, true);
				//followthetail = true;
				followthetail = !endgame;
			}
			else {
				std::cout << "yes it is possible to get the apple and the tail" << std::endl;
				followthetail = false;
			}
		}
	}

	/*
	if (wnd.kbd.KeyIsPressed('S')) {

		snake.AddNode();
	}*/

	// machine plays
	//double dx =apple.x- snake.shead->x;
	//double dy =apple.y- snake.shead->y;
	//if (dx != 0) {
	//	snake.vx = snake.vx *(abs(dx) / dx) !=-1 ? (abs(dx) / dx):snake.vx;
	//	snake.vy = 0;
	//}else if (dy != 0) {
	//	snake.vy = snake.vy *(abs(dy) / dy) !=-1 ? (abs(dy) / dy) : snake.vy;
	//	snake.vx = 0;
	//}



	/*if (snake.shead->y +1> Graphics::ScreenHeight / 10) {
		snake.shead->y = 1;
	}*/



	if (snake.shead->x == helperapple.x && snake.shead->y == helperapple.y) {
		std::cout << "helperapple-!-got apple "<< std::endl;

		helperapple.x = -1;
		helperapple.y = -1;
		Apple snaketail;
		snaketail.x = snake.stail->x;
		snaketail.y = snake.stail->y;

		(endgame == true) ? findLongestPath(snake, snaketail) : bfs(snake, snaketail, true);
		followthetail = true;
	}





		if (snake.shead->x == apple.x && snake.shead->y == apple.y) {

			if (endgame == true) {
				lullcnt=0;
			}
			
			++snake.length;
			std::cout << "I got the apple ("<<apple.x<<" , "<<apple.y<<")" << std::endl;


			bool goodapple = false;

			emptypositions.erase(remove(emptypositions.begin(), emptypositions.end(), std::make_pair((double)snake.tx, (double)snake.ty)), emptypositions.end());
			snake.AddNode();


			if (emptypositions.empty()) {
				std::cout << "---WE BEAT THE GAME---" << std::endl;
				while (true) {

				}
			}
			apple.replaceApple(emptypositions);

			//std::cout << "apple ( " << apple.x << " , " << apple.y << "\n";


			/*while (!isValid(apple.x, apple.y, Graphics::ScreenWidth, Graphics::ScreenHeight, snake)) {
				std::cout << "regenerate apple again "<< std::endl;
				apple.replaceApple();
			}*/



			Apple snaketail;
			snaketail.x = snake.stail->x;
			snaketail.y = snake.stail->y;

			(endgame == true) ? findLongestPath(snake, snaketail) : bfs(snake, snaketail, true);
			//followthetail = true;
			followthetail = !endgame;

			//goodapple=bfs(snake, apple, true);

			//if (!goodapple) {
			//	Apple snaketail;
			//	snaketail.x = snake.stail->x;
			//	snaketail.y = snake.stail->y;

			//	std::cout << bfs(snake, snaketail, true) << std::endl;
			//	followthetail = true;
			//}
			//else {

			//	Snake FakeSnake = snake;
			//	for (int i = path.size() - 2; i >= 0; --i) {
			//		FakeSnake.vx = path[i].first - FakeSnake.shead->x;
			//		FakeSnake.vy = path[i].second - FakeSnake.shead->y;

			//		FakeSnake.UpdateMovement();
			//	}
			//	FakeSnake.AddNode();

			//	Apple FakeSnakeTail;
			//	FakeSnakeTail.x = FakeSnake.stail->x;
			//	FakeSnakeTail.y = FakeSnake.stail->y;

			//	if (!bfs(FakeSnake,FakeSnakeTail,false) ) {
			//		//follow the tail until

			//		Apple snaketail;
			//		snaketail.x = snake.stail->x;
			//		snaketail.y = snake.stail->y;

			//		std::cout<<bfs(snake, snaketail,true)<<std::endl;
			//		followthetail = true;
			//	}
			//	else {
			//		followthetail = false;
			//	}
			//}
		}



		if (endgame == true) {

			/*if (path_index==1 && followthetail == ) {
				followthetail = true;
			}*/

			++lullcnt;
		}
}

void Game::ComposeFrame()
{
	//gfx.PutPixel(Graphics::ScreenWidth, Graphics::ScreenHeight, Colors::Yellow);

	for (std::pair<int, int> i : path) {
		draw(i.first, i.second,gfx);
	}

	helperapple.DrawApple(gfx);
	apple.DrawApple(gfx);
	snake.DrawSnake(gfx);

	
	/*std::reverse(path.begin(), path.end());
	return path;*/

}

void draw(double x,double y ,Graphics &gfx) {

	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {

			gfx.PutPixel(x * dim + j, y * dim + i, Colors::Blue);


		}
	}
}

bool isValid(int x , int y , int width , int height , Snake &snake) {
	bool existinsnakebody = false;

	Node * curr = snake.stail;
	while (curr != nullptr) {
		if (x == curr->x && y == curr->y) {
			existinsnakebody = true;
			break;
		}
		curr = curr->next;
	}
	return x >= 0 && x < width && y >= 0 && y < height && !existinsnakebody;
}


