#include "Library.h"

circle::circle() : Shape() {
	this->setCenter(0, 0);
	this->setRx(0);
	this->setRy(0);
}

circle::~circle() {
	this->setCenter(0, 0);
	this->setRx(0);
	this->setRy(0);
}

void circle::updateProperty() {
	stringstream ss(line_str);
	string attribute, val, temp;

	while (ss >> attribute) {
		getline(ss, temp, '"');
		getline(ss, val, '"');

		if (attribute == "r") {
			this->setRx(stof(val));
			this->setRy(stof(val));
		}
		if (attribute == "cx")
			this->center.setX((stof(val)));
		if (attribute == "cy")
			this->center.setY((stof(val)));
	}
}
point circle::getCenter() {
	return this->center;
}

float circle::getRx() {
	return this->rx;
}

float circle::getRy() {
	return this->ry;
}

void circle::setCenter(float x, float y) {
	this->center.setX(x);
	this->center.setY(y);
}

void circle::setRx(float rx) {
	this->rx = rx;
}

void circle::setRy(float ry) {
	this->ry = ry;
}