#include "Library.h"

Line::Line() :Shape() {
	p1.setX(0); p1.setY(0);
	p2.setX(0); p2.setY(0);
}

Line::~Line() {
	p1.setX(0); p1.setY(0);
	p2.setX(0); p2.setY(0);
}

void Line::updateProperty() {
	stringstream ss(line_str);
	string attribute, val, temp;

	while (ss >> attribute) {
		getline(ss, temp, '"');
		getline(ss, val, '"');

		if (attribute == "x1") {
			this->p1.setX(stof(val));
		}
		if (attribute == "y1") {
			this->p1.setY(stof(val));
		}
		if (attribute == "x2") {
			this->p2.setX(stof(val));
		}
		if (attribute == "y2") {
			this->p2.setY(stof(val));
		}
	}
}

point Line::getP1() {
	return this->p1;
}

point Line::getP2() {
	return this->p2;
}

void Line::setP1(point p1) {
	this->p1 = p1;
}

void Line::setP2(point p2) {
	this->p2 = p2;
}