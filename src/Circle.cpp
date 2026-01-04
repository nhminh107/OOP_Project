#include "Library.h"

SVGCircle::SVGCircle() : Shape() {
    this->setCenter(0, 0);
    this->setRadius(0);
}

SVGCircle::~SVGCircle() {
    this->setCenter(0, 0);
    this->setRadius(0);
}

void SVGCircle::updateProperty() {
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
point SVGCircle::getCenter() {
    return this->center;
}

float SVGCircle::getRadius() {
    return this->radius;
}

void SVGCircle::setCenter(float x, float y) {
    this->center.setX(x);
    this->center.setY(y);
}

void SVGCircle::setRadius(float r) {
    this->radius = r;
}

void SVGCircle::draw(Graphics& graphics)
{
    //Luu trang thai
    GraphicsState save = graphics.Save();

    //Ap dung transform
    Matrix mat;
    mat.SetElements(1, 0, 0, 1, 0, 0);
    this->getTransformMatrix(&mat);
    graphics.MultiplyTransform(&mat);

    //Tao pen
    stroke s = this->getStroke();
    color sc = s.getStrokeColor();

    Pen penCircle(
        Color(sc.opacity * 255,sc.r,sc.g,sc.b),s.getStrokeWidth()
    );

    //Chuan bi brush
    Brush* fillBrush = nullptr;
    color fc = this->getColor();

    float r = this->getRadius();
    float cx = this->getCenter().getX();
    float cy = this->getCenter().getY();

    //Tinh bounding box
    RectF bound(cx - r, cy - r, 2.0f * r, 2.0f * r);

    //Neu co gradient
    if (this->getGrad()) {
        fillBrush = this->getGrad()->createBrush(bound);
    }

    //Neu khong co gradient
    if (!fillBrush) {
        fillBrush = new SolidBrush(
            Color(fc.opacity * 255,fc.r,fc.g,fc.b));
    }

    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    //draw
    graphics.FillEllipse(fillBrush, bound);
    graphics.DrawEllipse(&penCircle, bound);

    //delete
    delete fillBrush;
    graphics.Restore(save);
}