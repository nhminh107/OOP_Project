#include "Library.h"

SVGLine::SVGLine() :Shape() {
	p1.setX(0); p1.setY(0);
	p2.setX(0); p2.setY(0);
}

SVGLine::~SVGLine() {
	p1.setX(0); p1.setY(0);
	p2.setX(0); p2.setY(0);
}

void SVGLine::updateProperty() {
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

void SVGLine::draw(Graphics& graphics) {
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

RectF SVGLine::getBoundingBox() {

	float x1 = p1.getX(); 
	float x2 = p2.getX(); 
	float y1 = p1.getY(); 
	float y2 = p2.getY(); 

	RectF boundingBox; 

	boundingBox.X = min(x1, x2); 
	boundingBox.Y = min(y1, y2); 

	boundingBox.Width = abs(x1 - x2); 
	boundingBox.Height = abs(y1 - y2); 

	if (boundingBox.Width == 0) boundingBox.Width = 0.1f; // Một giá trị rất nhỏ để tránh lỗi
	if (boundingBox.Height == 0) boundingBox.Height = 0.1f;

	return boundingBox;
}
point SVGLine::getP1() {
	return this->p1;
}

point SVGLine::getP2() {
	return this->p2;
}

void SVGLine::setP1(point p1) {
	this->p1 = p1;
}

void SVGLine::setP2(point p2) {
	this->p2 = p2;
}