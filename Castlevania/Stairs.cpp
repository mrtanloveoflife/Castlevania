#include "Stairs.h"

Stairs::Stairs(int state, float uleft, float utop, float uright, float ubottom,
	float dleft, float dtop, float dright, float dbottom)
{
	this->state = state;
	this->uleft = uleft;
	this->utop = utop;
	this->uright = uright;
	this->ubottom = ubottom;

	this->dleft = dleft;
	this->dtop = dtop;
	this->dright = dright;
	this->dbottom = dbottom;
}

void Stairs::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (isUp)
	{
		left = this->uleft;
		top = this->utop;
		right = this->uright;
		bottom = this->ubottom;
	}
	else
	{
		left = this->dleft;
		top = this->dtop;
		right = this->dright;
		bottom = this->dbottom;
	}
}