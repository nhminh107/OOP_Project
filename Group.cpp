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
    for (auto fig : this->shapeArray) {
        if (fig != nullptr) {
            delete fig;
        }
    }
    this->shapeArray.clear();
    this->parent = nullptr;
}

RectF SVGGroup::getBoundingBox() {
	if (this->shapeArray.empty()) {
		return RectF(0, 0, 0, 0);
	}

	float min_X = FLT_MAX;
	float min_Y = FLT_MAX;
	float max_X = -FLT_MAX;
	float max_Y = -FLT_MAX;

	bool hasValidChild = false;

	for (Shape* shape : this->shapeArray) {
		if (shape != nullptr) {
			//Nếu con là Group, nó sẽ tự tính box của nó
			RectF childBox = shape->getBoundingBox();
			if (childBox.Width <= 0 && childBox.Height <= 0) continue;

			hasValidChild = true;
			if (childBox.X < min_X)
				min_X = childBox.X;

			if (childBox.Y < min_Y)
				min_Y = childBox.Y;

			float childRight = childBox.X + childBox.Width;
			if (childRight > max_X)
				max_X = childRight;
			float childBottom = childBox.Y + childBox.Height;
			if (childBottom > max_Y)
				max_Y = childBottom;
		}
	}

	if (!hasValidChild) {
		return RectF(0, 0, 0, 0);
	}

	return RectF(min_X, min_Y, max_X - min_X, max_Y - min_Y);
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