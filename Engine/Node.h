#pragma once


class Node {
public:
	Node* next= nullptr;
	double x;
	double y;
	Node(double x,double y) {
		this->x = x;
		this->y = y;
	}
};
