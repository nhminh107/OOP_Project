#include "Library.h"

void linearGradient::setX1(float v) { A.setX(v); }
void linearGradient::setY1(float v) { A.setY(v); }
void linearGradient::setX2(float v) { B.setX(v); }
void linearGradient::setY2(float v) { B.setY(v); }

point linearGradient::getA() const{
	return A;
}
point linearGradient::getB() const {
	return B;
}

linearGradient::linearGradient() {
	A.setX(0);
	A.setY(0);
	B.setX(0);
	B.setY(0);
}
linearGradient::linearGradient(const linearGradient& other) {
	this->A = other.A;
	this->B = other.B;
}

linearGradient& linearGradient::operator=(const linearGradient& other) {
	if (this != &other) {
		this->A = other.A;
		this->B = other.B;
	}
	return *this;
}

void linearGradient::updateElement() {
	stringstream ss1(lineData);
	string tmp = "", attribute = "", value = "", transGradient = "";
	while (ss1 >> attribute) {
		getline(ss1, tmp, '"');
		getline(ss1, value, '"');
		if (attribute == "x1")
			A.setX(stof(value));
		if (attribute == "x2")
			B.setX(stof(value));
		if (attribute == "y1")
			A.setY(stof(value));
		if (attribute == "y2")
			B.setY(stof(value));

		if (attribute == "gradientTransform")
			transGradient = value;
	}
	this->addGradTrans(transGradient);
}