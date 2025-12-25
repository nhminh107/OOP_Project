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

RectF SVGRectangle::getBoundingBox() {
	RectF boundingBox; 

	boundingBox.X = this->root.getX(); 
	boundingBox.Y = this->root.getY(); 
	boundingBox.Width = this->width; 
	boundingBox.Height = this->height; 

	return boundingBox; 
}
void SVGRectangle::draw(Graphics& graphics) {
	// 1. Lưu trạng thái Graphics
	GraphicsState save = graphics.Save();

	//Chuẩn bị Brush
	Brush* fillBrush = nullptr;
	RectF bound = this->getBoundingBox();

	if (this->hasGradient) {
		string id = this->getFillGradientID();
		if (gradientMap.count(id)) {
			Gradient* grad = gradientMap[id];

			vector<stop> stops = grad->getStopList();
			if (!stops.empty()) {
				color lastC = stops.back().stopColor;
				Color padColor(lastC.opacity * this->getColor().opacity * 255, lastC.r, lastC.g, lastC.b);

				SolidBrush padBrush(padColor);
				graphics.FillRectangle(&padBrush, bound); 
			}

			
			fillBrush = grad->createBrush(bound, this->getColor().opacity);

			graphics.FillRectangle(fillBrush, bound);
		}
	}

	//(Không có gradient)
	if (fillBrush == nullptr) {
		fillBrush = new SolidBrush(Color(this->getColor().opacity * 255,
			this->getColor().r, this->getColor().g, this->getColor().b));
		graphics.FillRectangle(fillBrush, bound);
	}

	// 2. Tạo bút vẽ (Pen) cho viền
	Pen penRectangle(Color(this->getStroke().getStrokeColor().opacity * 255,
		this->getStroke().getStrokeColor().r,
		this->getStroke().getStrokeColor().g,
		this->getStroke().getStrokeColor().b),
		this->getStroke().getStrokeWidth());

	// 3. Tạo cọ tô màu (Brush) cho phần nền
	/*SolidBrush fillRectangle(Color(this->getColor().opacity * 255,
		this->getColor().r,
		this->getColor().g,
		this->getColor().b));*/

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


	graphics.FillRectangle(fillBrush, this->getRoot().getX(), this->getRoot().getY(), this->getWidth(), this->getHeight());
	graphics.DrawRectangle(&penRectangle, this->getRoot().getX(), this->getRoot().getY(), this->getWidth(), this->getHeight());

	// 7. Khôi phục trạng thái
	// Dọn dẹp
	if (fillBrush) delete fillBrush;
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

