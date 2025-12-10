#include "Library.h"

Circle::Circle() : Shape() {
	this->setCenter(0, 0);
	this->setRx(0);
	this->setRy(0);
}

Circle::~Circle() {
	this->setCenter(0, 0);
	this->setRx(0);
	this->setRy(0);
}

void Circle::updateProperty() {
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
point Circle::getCenter() {
	return this->center;
}

float Circle::getRx() {
	return this->rx;
}

float Circle::getRy() {
	return this->ry;
}

void Circle::setCenter(float x, float y) {
	this->center.setX(x);
	this->center.setY(y);
}

void Circle::setRx(float rx) {
	this->rx = rx;
}

void Circle::setRy(float ry) {
	this->ry = ry;
}