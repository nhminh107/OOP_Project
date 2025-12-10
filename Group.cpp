#include "Library.h"

SVGGroup::SVGGroup() :Shape() {
	figureArray = {};
}

SVGGroup& SVGGroup:: operator = (const SVGGroup& grp) {
	if (this != &grp) {
		this->figureArray = grp.figureArray;
		this->parent = grp.parent;
	}
	return *this;
}

SVGGroup::SVGGroup(const SVGGroup& grp) {
	this->figureArray = grp.figureArray;
}

void SVGGroup::addFigure(Shape* fig) {
	this->figureArray.push_back(fig);
}

void SVGGroup::setParent(SVGGroup* parent) {
	this->parent = parent;
}

void SVGGroup::setFigureArray(vector<Shape*> figureArrray) {
	this->figureArray = figureArray;
}

SVGGroup* SVGGroup::getParent() {
	return this->parent;
}

vector<Shape*> SVGGroup::getFigureArray() {
	return this->figureArray;
}