#include "Library.h"

group::group() :Shape() {
	figureArray = {};
}

group& group:: operator = (const group& grp) {
	if (this != &grp) {
		this->figureArray = grp.figureArray;
		this->parent = grp.parent;
	}
	return *this;
}

group::group(const group& grp) {
	this->figureArray = grp.figureArray;
}

void group::addFigure(Shape* fig) {
	this->figureArray.push_back(fig);
}

void group::setParent(group* parent) {
	this->parent = parent;
}

void group::setFigureArray(vector<Shape*> figureArrray) {
	this->figureArray = figureArray;
}

group* group::getParent() {
	return this->parent;
}

vector<Shape*> group::getFigureArray() {
	return this->figureArray;
}