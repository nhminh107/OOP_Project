#include "Library.h"

Ellipse::Ellipse() : Shape() {
	rx = ry = 0;
	center.setX(0);
	center.setY(0);
}

Ellipse::~Ellipse() {
	rx = ry = 0;
	center.setX(0);
	center.setY(0);
}

void Ellipse::updateProperty() {
	stringstream ss(line_str);
	string property, val, temp;

	while (ss >> property) {
		getline(ss, temp, '"');
		getline(ss, val, '"');

		if (property == "rx")
			this->rx = stof(val);
		if (property == "ry")
			this->ry = stof(val);
		if (property == "cx")
			this->center.setX(stof(val));
		if (property == "cy")
			this->center.setY(stof(val));
	}
}

point Ellipse::getCenter() {
	return this->center;
}

float Ellipse::getRx() {
	return this->rx;
}

float Ellipse::getRy() {
	return this->ry;
}

void Ellipse::setCenter(float x, float y) {
	this->center.setX(x);
	this->center.setY(y);
}

void Ellipse::setRx(float rx) {
	this->rx = rx;
}

void Ellipse::setRy(float ry) {
	this->ry = ry;
}