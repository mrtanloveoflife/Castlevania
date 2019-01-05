#include "Brick.h"

Brick::Brick(float left, float top, float right, float bottom)
{
	this->left = left; 
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

void Brick::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = this->left;
	top = this->top;
	right = this->right;
	bottom = this->bottom;
}