#include "Library.h"

radialGradient::radialGradient() {
	isLink = false;
	cx = cy = r = fx = fy = 0;
}

radialGradient::radialGradient(const radialGradient& radial) {
	cx = radial.cx;
	cy = radial.cy;
	r = radial.r;
	fx = radial.fx;
	fy = radial.fy;
}
radialGradient& radialGradient::operator=(const radialGradient& radial) {
	if (this != &radial) {
		cx = radial.cx;
		cy = radial.cy;
		r = radial.r;
		fx = radial.fx;
		fy = radial.fy;
	}
	return *this;
}

float radialGradient::getCx() const {
	return cx;
}
float radialGradient::getCy() const {
	return cy;
}
float radialGradient::getR() const {
	return r;
}
float radialGradient::getFx() const {
	return fx;
}
float radialGradient::getFy() const {
	return fy;
}
bool radialGradient::getIsLink() const {
	return this->isLink;
}
void radialGradient::setCX(float v) {
	this->cx = v;
}
void radialGradient::setCY(float v) { 
	this->cy = v;
}
void radialGradient::setR(float v) { 
	this->r = v;
}
void radialGradient::setFX(float v) { 
	this->fx = v;
}
void radialGradient::setFY(float v) { 
	this->fy = v;
}
void radialGradient::setIsLink(bool link) {
	this->isLink = link;
}

void radialGradient::updateElement() {
	stringstream ss(lineData);
	string temp = "", attribute = "", value = "", transformGradient = "";

	while (ss >> attribute) {
		getline(ss, temp, '"');
		getline(ss, value, '"');

		if (attribute == "cx") {
			this->cx = stof(value);
		}
		if (attribute == "cy") {
			this->cy = stof(value);
		}
		if (attribute == "r") {
			this->r = stof(value);
		}
		if (attribute == "fx") {
			this->fx = stof(value);
		}
		if (attribute == "fy") {
			this->fy = stof(value);
		}
		if (attribute == "gradientTransform") {
			transformGradient = value;
		}
		if (attribute == "xlink:href") {
			this->isLink = true;
		}
	}
	this->addGradTrans(transformGradient);
}