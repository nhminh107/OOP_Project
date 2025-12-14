#include "Library.h"

rectangle::rectangle() : Shape() {
	width = height = 0;
	root.setX(0);
	root.setY(0);
}

rectangle::~rectangle() {
	width = height = 0;
	root.setX(0);
	root.setY(0);
}

void rectangle::updateProperty() {
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
void rectangle::draw(Graphics& graphics) {
	// 1. Lưu trạng thái Graphics
	GraphicsState save = graphics.Save();

	// 2. Tạo bút vẽ (Pen) cho viền
	Pen penRectangle(Color(this->getStroke().getStrokeColor().opacity * 255,
		this->getStroke().getStrokeColor().r,
		this->getStroke().getStrokeColor().g,
		this->getStroke().getStrokeColor().b),
		this->getStroke().getStrokeWidth());

	// 3. Tạo cọ tô màu (Brush) cho phần nền
	SolidBrush fillRectangle(Color(this->getColor().opacity * 255,
		this->getColor().r,
		this->getColor().g,
		this->getColor().b));

	// 4. Xử lý các phép biến đổi (Translate, Rotate, Scale, Matrix)
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
		else if (trans.first == "scale")
			graphics.ScaleTransform(x, y);
		else if (trans.first == "matrix") {
			if (trans.second.size() >= 6) {
				Matrix matrix(trans.second[0], trans.second[1], trans.second[2],
					trans.second[3], trans.second[4], trans.second[5]);
				graphics.MultiplyTransform(&matrix);
			}
		}
	}

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);


	graphics.FillRectangle(&fillRectangle, this->getRoot().getX(), this->getRoot().getY(), this->getWidth(), this->getHeight());
	graphics.DrawRectangle(&penRectangle, this->getRoot().getX(), this->getRoot().getY(), this->getWidth(), this->getHeight());

	// 7. Khôi phục trạng thái
	graphics.Restore(save);
}
point rectangle::getRoot() {
	return this->root;
}

float rectangle::getWidth() {
	return this->width;
}

float rectangle::getHeight() {
	return this->height;
}

void rectangle::setRoot(point root) {
	this->root = root;
}

void rectangle::setWidth(float w) {
	this->width = w;
}

void rectangle::setHeight(float h) {
	this->height = h;
}

