#include "Library.h"

circle::circle() : Shape() {
	this->setCenter(0, 0);
    this->setRadius(0);
}

circle::~circle() {
	this->setCenter(0, 0);
    this->setRadius(0);
}

void circle::updateProperty() {
	stringstream ss(line_str);
	string attribute, val, temp;

	while (ss >> attribute) {
		getline(ss, temp, '"');
		getline(ss, val, '"');

		if (attribute == "r") {
			this->setRadius(stof(val));
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

float circle::getRadius() {
	return this->radius;
}

void circle::setCenter(float x, float y) {
	this->center.setX(x);
	this->center.setY(y);
}

void circle::setRadius(float r) {
	this->radius = r;
}

void circle::drawCircle(Graphics& graphics, circle* fig) {
    GraphicsState save = graphics.Save();
    Pen penCircle(Color(fig->getStroke().getStrokeColor().opacity * 255,
        fig->getStroke().getStrokeColor().r,
        fig->getStroke().getStrokeColor().g,
        fig->getStroke().getStrokeColor().b),
        fig->getStroke().getStrokeWidth());

    SolidBrush fillCircle(Color(fig->getColor().opacity * 255,
        fig->getColor().r,
        fig->getColor().g,
        fig->getColor().b));
    vector<pair<string, vector<float>>> transVct = fig->getTransVct();
    for (auto trans : transVct) {
        float x = 0.0f;
        if (!trans.second.empty()) x = trans.second[0];
        float y = x;
        if (trans.second.size() == 2) y = trans.second[1];

        if (trans.first == "translate")
            graphics.TranslateTransform(x, y);
        else if (trans.first == "rotate")
            graphics.RotateTransform(x);
        else
            graphics.ScaleTransform(x, y);
    }

    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    float r = fig->getRadius();
    float d = 2.0f * r;
    float x_pos = fig->getCenter().getX() - r;
    float y_pos = fig->getCenter().getY() - r;

    graphics.FillEllipse(&fillCircle, x_pos, y_pos, d, d);
    graphics.DrawEllipse(&penCircle, x_pos, y_pos, d, d);

    graphics.Restore(save);
}