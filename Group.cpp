#include "Library.h"

group::group() :Shape() {
	figureArray = {};
}

group& group:: operator = (const group& grp) {
	if (this != &grp) {
		this->figureArray = grp.figureArray;
		this->parent = grp.parent;
	}
	return *this;
}

void group::draw(Graphics& graphics) {

    if (this->figureArray.empty())
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

    for (Shape* fig : this->figureArray) {
        if (fig != nullptr) {
            fig->draw(graphics);
        }
    }


    graphics.Restore(save);
}
group::group(const group& grp) {
	this->figureArray = grp.figureArray;
}

void group::addFigure(Shape* fig) {
	this->figureArray.push_back(fig);
}

void group::setParent(group* parent) {
	this->parent = parent;
}

void group::setFigureArray(vector<Shape*> figureArrray) {
	this->figureArray = figureArray;
}

group* group::getParent() {
	return this->parent;
}

vector<Shape*> group::getFigureArray() {
	return this->figureArray;
}