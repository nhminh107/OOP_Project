#include "Library.h"

ellipse::ellipse() : Shape() {
	rx = ry = 0;
	center.setX(0);
	center.setY(0);
}

ellipse::~ellipse() {
	rx = ry = 0;
	center.setX(0);
	center.setY(0);
}

void ellipse::updateProperty() {
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

point ellipse::getCenter() {
	return this->center;
}

float ellipse::getRx() {
	return this->rx;
}

float ellipse::getRy() {
	return this->ry;
}

void ellipse::setCenter(float x, float y) {
	this->center.setX(x);
	this->center.setY(y);
}

void ellipse::setRx(float rx) {
	this->rx = rx;
}

void ellipse::setRy(float ry) {
	this->ry = ry;
}
void ellipse::draw(Graphics& graphics) {
	GraphicsState save = graphics.Save();
	Pen penEllipse(Color(this->getStroke().getStrokeColor().opacity * 255,
		this->getStroke().getStrokeColor().r,
		this->getStroke().getStrokeColor().g,
		this->getStroke().getStrokeColor().b),
		this->getStroke().getStrokeWidth());

	SolidBrush fillEllipse(Color(this->getColor().opacity * 255,
		this->getColor().r,
		this->getColor().g,
		this->getColor().b));

	vector<pair<string, vector<float>>> transVct = this->getTransVct();

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
	graphics.FillEllipse(&fillEllipse,
		this->getCenter().getX() - this->getRx(),
		this->getCenter().getY() - this->getRy(),
		2.0 * this->getRx(),
		2.0 * this->getRy());

	graphics.DrawEllipse(&penEllipse,
		this->getCenter().getX() - this->getRx(),
		this->getCenter().getY() - this->getRy(),
		2.0 * this->getRx(),
		2.0 * this->getRy());

	graphics.Restore(save);
}