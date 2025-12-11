#include "Library.h"

line::line() :Shape() {
	p1.setX(0); p1.setY(0);
	p2.setX(0); p2.setY(0);
}

line::~line() {
	p1.setX(0); p1.setY(0);
	p2.setX(0); p2.setY(0);
}

void line::updateProperty() {
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

void line::draw(Graphics& graphics) {
	GraphicsState save = graphics.Save();
	Pen penLine(Color(getStroke().getStrokeColor().opacity * 255, getStroke().getStrokeColor().r, getStroke().getStrokeColor().g, getStroke().getStrokeColor().b), getStroke().getStrokeWidth());
	vector<pair<string, vector<float>>> transVct = getTransVct();

	for (auto trans : transVct) {
		float x = 0.0f;
		if (!trans.second.empty())
			x = trans.second[0];
		float y = x;
		if (trans.second.size() == 2)
			y = trans.second[1];
		if (trans.first == "translate")
			graphics.TranslateTransform(x, y);
		else if (trans.first == "rotate")
			graphics.RotateTransform(x);
		else graphics.ScaleTransform(x, y);
	}

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.DrawLine(&penLine, getP1().getX(), getP1().getY(), getP2().getX(), getP2().getY());
	graphics.Restore(save);
}

point line::getP1() {
	return this->p1;
}

point line::getP2() {
	return this->p2;
}

void line::setP1(point p1) {
	this->p1 = p1;
}

void line::setP2(point p2) {
	this->p2 = p2;
}