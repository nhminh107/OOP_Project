#include "Library.h"

SVGRectangle::SVGRectangle() : Shape() {
	width = height = 0;
	root.setX(0);
	root.setY(0);
}

SVGRectangle::~SVGRectangle() {
	width = height = 0;
	root.setX(0);
	root.setY(0);
}

void SVGRectangle::updateProperty() {
	stringstream ss(line_str);
	string attribute, val, temp;

	while (ss >> attribute) {
		getline(ss, temp, '"');
		getline(ss, val, '"');

		if (attribute == "x")
			this->root.setX(stof(val));
		if (attribute == "y")
			this->root.setY(stof(val));
		if (attribute == "width")
			this->width = stof(val);
		if (attribute == "height")
			this->height = stof(val);
	}
}
void SVGRectangle::draw(Graphics& graphics) {
    //Luu trang thai
    GraphicsState save = graphics.Save();

    //Ap dung transform
    Matrix transform;
    this->getTransformMatrix(&transform);
    graphics.MultiplyTransform(&transform);

    //Tinh bounding box
    RectF bound(
        this->getRoot().getX(),
        this->getRoot().getY(),
        this->getWidth(),
        this->getHeight()
    );

    //Chuan bi brush
    Brush* fillBrush = nullptr;

    gradient* grad = this->getGrad();
    if (grad) {
        //Neu co gradient
        fillBrush = grad->createBrush(bound);
    }
    else {
        //Neu khong co gradient
        fillBrush = new SolidBrush(
            Color(this->getColor().opacity * 255,this->getColor().r,this->getColor().g,this->getColor().b)
        );
    }

    //Pen
    stroke s = this->getStroke();
    Pen pen(
        Color(
            s.getStrokeColor().opacity * 255,s.getStrokeColor().r,s.getStrokeColor().g,s.getStrokeColor().b)
            , s.getStrokeWidth()
    );

    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    //Draw
    graphics.FillRectangle(fillBrush, bound);
    graphics.DrawRectangle(&pen, bound);

    //Delete
    delete fillBrush;
    graphics.Restore(save);
}
point SVGRectangle::getRoot() {
	return this->root;
}

float SVGRectangle::getWidth() {
	return this->width;
}

float SVGRectangle::getHeight() {
	return this->height;
}

void SVGRectangle::setRoot(point root) {
	this->root = root;
}

void SVGRectangle::setWidth(float w) {
	this->width = w;
}

void SVGRectangle::setHeight(float h) {
	this->height = h;
}

