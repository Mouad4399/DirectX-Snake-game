#include "Snake.h"
#include "Colors.h"
#include<iostream>
#include<cmath>
void Snake::AddNode()
{

	//double nx = stail->x + (abs(stail->x-stail->next->x )>1? -abs(stail->x - stail->next->x)/(stail->x - stail->next->x): (stail->x - stail->next->x));
	//double ny = stail->y + (abs(stail->y - stail->next->y) > 1 ? -abs(stail->y - stail->next->y) / (stail->y - stail->next->y) : (stail->y - stail->next->y));

	// dump ass not real calculation
	//double nx = stail->x + (stail->x - stail->next->x);
	//double ny = stail->y + (stail->y - stail->next->y);


	Node* newSnakeTail = new Node(tx, ty);
	newSnakeTail->next = stail;
	stail = newSnakeTail;

	
	if (   (stail->x >= (Graphics::ScreenWidth / dim)) || (stail->y >=(Graphics::ScreenHeight / dim))   ) {
		std::cout << "help dim :"<<dim<<" help : " << stail->x <<" , "<< stail->y<<  std::endl;
	}
}
void Snake::AddNode(double nx, double ny)
{

	Node* newSnakeTail = new Node(nx, ny);
	newSnakeTail->next = stail;
	stail = newSnakeTail;
}

void Snake::UpdateMovement()
{
	if (GameOver) {
		return;
	}

	//save the position of the last tail

	tx = stail->x;
	ty = stail->y;
	//disconnecting the snake tail
	Node* temp = stail;
	stail = stail->next;
	temp->next = nullptr;

	// conneting it to the head
	shead->next = temp;
	// modifying the coordinates of the new head
	temp->x = shead->x + vx;
	temp->y = shead->y + vy;
	shead=temp;


	//std::cout << "this is the value : "<< (int)(Graphics::ScreenWidth / 10) << " | "<< (Graphics::ScreenWidth / 10);
	//shead->x= ((int)abs(shead->x) % (int) (Graphics::ScreenWidth/10));
	//shead->y= ((int)abs(shead->y) % (int) (Graphics::ScreenHeight/10));

		/*shead->x = (shead->x < 0) ? (Graphics::ScreenWidth / 10)-1 : (shead->x >= Graphics::ScreenWidth/10) ?0:shead->x;
		shead->y = (shead->y < 0) ? (Graphics::ScreenHeight / 10)-1 : (shead->y >= Graphics::ScreenHeight/10) ?0:shead->y;
*/


	


}

void Snake::DrawSnake(Graphics & gfx)
{
	// linked list traversal
	Node* curr = stail;
	while (curr != nullptr) {
		//curr->x = (curr->x < 0) ? Graphics::ScreenWidth / 10 : (curr->x >= Graphics::ScreenWidth/10) ?10:curr->x;
		//curr->y = (curr->y < 0) ? Graphics::ScreenHeight / 10 : (curr->y >= Graphics::ScreenHeight/10) ?10:curr->y;

		if ((shead != curr && shead->x == curr->x && shead->y == curr->y)){
			GameOver = true;
		}
		// draw the rectangle
		for (int i = 0; i < dim; ++i) {
			for (int j = 0; j < dim; ++j) {
				
				gfx.PutPixel(curr->x * dim + j, curr->y * dim + i, GameOver ? Colors::Red : (curr==shead)?Colors::Gray:(curr==stail)?Colors::LightGray:Colors::White );
				
				
			}
		}
		/*if (GameOver) {
			return;
		}*/
		curr = curr->next;

	}


}

void Snake::print()
{
	Node * curr = stail;
	while (curr) {
		std::cout << "( " << curr->x << " , " << curr->y << " )" << std::endl;
		curr = curr->next;
	}
}

void Snake::AddNodeToHead(int x, int y)
{
	Node * newNode = new Node(x, y);

	shead->next = newNode;
	shead = shead->next;
	
}
