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
void ellipse::drawEllipse(Graphics& graphics, ellipse* fig) {
	GraphicsState save = graphics.Save();
	Pen penEllipse(Color(fig->getStroke().getStrokeColor().opacity * 255, fig->getStroke().getStrokeColor().r, fig->getStroke().getStrokeColor().g, fig->getStroke().getStrokeColor().b), fig->getStroke().getStrokeWidth());
	SolidBrush fillEllipse(Color(fig->getColor().opacity * 255, fig->getColor().r, fig->getColor().g, fig->getColor().b));
	vector<pair<string, vector<float>>> transVct = fig->getTransVct();

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
	graphics.FillEllipse(&fillEllipse, fig->getCenter().getX() - fig->getRx(), fig->getCenter().getY() - fig->getRy(), 2.0 * fig->getRx(), 2.0 * fig->getRy());
	graphics.DrawEllipse(&penEllipse, fig->getCenter().getX() - fig->getRx(), fig->getCenter().getY() - fig->getRy(), 2.0 * fig->getRx(), 2.0 * fig->getRy());
	graphics.Restore(save);
}