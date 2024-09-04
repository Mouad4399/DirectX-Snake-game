#pragma once
#include"Node.h"
#include "Graphics.h"

// important for cout
#include <iostream>
#include <string>

class Snake {

public:
	Node* shead= nullptr;
	Node* stail = nullptr;

	double tx;
	double ty;

	int dim=32;
	double vx = 1;
	double vy = 0;

	bool GameOver = false;

	int length;

	//Snake() {


	//	//stail = new Node(2, 4);
	//	//stail->next = new Node(3, 4);
	//	//stail->next->next =  new Node(4, 4);
	//	//shead = stail->next->next;

	//	//AddNode();
	//	//UpdateMovement();



	//	//// display the linked list
	//	//Node* curr = stail;
	//	//while (curr!= nullptr) {

	//	//	std::cout << "x :" << (curr->x) << " | y :" << (curr->y) << "\n";
	//	//	curr=curr->next;

	//	//}

	//}


	Snake() {

	}
	Snake(const Snake &other) {
		shead = new Node(other.stail->x, other.stail->y);
		stail = shead;
		Node * curr = other.stail->next;

		while (curr) {
			AddNodeToHead(curr->x, curr->y);
			curr = curr->next;
		}
	}

	Snake& operator=(const Snake &other) {

		if (this != &other) {
			shead = new Node(other.stail->x, other.stail->y);
			stail = shead;
			Node * curr = other.stail->next;

			while (curr) {
				AddNodeToHead(curr->x, curr->y);
				curr = curr->next;
			}
		}
		return *this;
	}
	void AddNode();
	void AddNode(double nx , double ny);
	void UpdateMovement();
	void DrawSnake(Graphics &gfx);
	void print();
	void AddNodeToHead(int x, int y);

};