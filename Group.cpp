#include "Library.h"

SVGGroup::SVGGroup() :Shape() {
	shapeArray = {};
}

SVGGroup& SVGGroup:: operator = (const SVGGroup& grp) {
	if (this != &grp) {
		this->shapeArray = grp.shapeArray;
		this->parent = grp.parent;
	}
	return *this;
}
SVGGroup::~SVGGroup() {
    for (auto it = this->shapeArray.begin(); it != this->shapeArray.end(); it++) {
        shapeArray.erase(it); 
    }
    delete this->parent; 
}
void SVGGroup::draw(Graphics& graphics) {

    if (this->shapeArray.empty())
        return;

    GraphicsState save = graphics.Save();

    vector<pair<string, vector<float>>> transforms = this->getTransVct();

    for (const auto& trans : transforms) {
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
        else if (trans.first == "scale")
            graphics.ScaleTransform(x, y);
        else if (trans.first == "matrix") {
            if (trans.second.size() >= 6) {
                Matrix matrix(
                    trans.second[0], trans.second[1], trans.second[2],
                    trans.second[3], trans.second[4], trans.second[5]
                );
                graphics.MultiplyTransform(&matrix);
            }
        }
    }

    for (Shape* shape : this->shapeArray) {
        if (shape != nullptr) {
            shape->draw(graphics);
        }
    }


    graphics.Restore(save);
}
SVGGroup::SVGGroup(const SVGGroup& grp) {
	this->shapeArray = grp.shapeArray;
}

void SVGGroup::addShape(Shape* shape) {
	this->shapeArray.push_back(shape);
}

void SVGGroup::setParent(SVGGroup* parent) {
	this->parent = parent;
}

void SVGGroup::setShapeArray(vector<Shape*> figureArrray) {
	this->shapeArray = shapeArray;
}

SVGGroup* SVGGroup::getParent() {
	return this->parent;
}

vector<Shape*> SVGGroup::getShapeArray() {
	return this->shapeArray;
}