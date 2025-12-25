#include "Library.h"

point lineargradient::getA() {
	return A;
}

point lineargradient::getB() {
	return B;
}

void lineargradient::setA(point p) {
	this->A = p;
}

void lineargradient::setB(point p) {
	this->B = p;
}
//Hàm phụ trợ, mực đích chuẩn hóa chuỗi 

float parseSVGValue1(string val) {
	if (val.empty()) return 0;
	// Nếu có ký tự '%', chia giá trị cho 100
	if (val.find('%') != string::npos) {
		return stof(val.substr(0, val.find('%'))) / 100.0f;
	}
	// Nếu không, trả về giá trị số thực bình thường
	return stof(val);
}
lineargradient::lineargradient() {
	A.setX(0);
	A.setY(0);
	B.setX(0);
	B.setY(0);
}

lineargradient::lineargradient(const lineargradient& linear) {
	this->A = linear.A;
	this->B = linear.B;

}

lineargradient& lineargradient::operator=(const lineargradient& linear) {
	if (this != &linear) {
		this->A = linear.A;
		this->B = linear.B;
	}
	return *this;
}
Gdiplus::Brush* lineargradient::createBrush(Gdiplus::RectF bounds) {
	using namespace Gdiplus;
	PointF p1, p2;

	// Sử dụng getUnits() theo đúng file .h bạn cung cấp
	if (this->getUnits() == OBJECT_BOUNDING_BOX) {
		p1 = PointF(bounds.X + this->getA().getX() * bounds.Width,
			bounds.Y + this->getA().getY() * bounds.Height);
		p2 = PointF(bounds.X + this->getB().getX() * bounds.Width,
			bounds.Y + this->getB().getY() * bounds.Height);
	}
	else {
		p1 = PointF(this->getA().getX(), this->getA().getY());
		p2 = PointF(this->getB().getX(), this->getB().getY());
	}

	vector<stop> ColorOffset = this->getStopVct();
	int size = ColorOffset.size();
	if (size > 0 && ColorOffset[0].offset != 0) {
		color first = ColorOffset[0].stopColor;
		float offset = ColorOffset[0].offset;
		color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
		ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
		size++;
	}

	if (size > 0 && ColorOffset[size - 1].offset != 1) {
		color last = ColorOffset[size - 1].stopColor;
		float offset = ColorOffset[size - 1].offset;
		color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
		ColorOffset.push_back(stop(one, 1));
		size++;
	}

	float* points = new float[size];
	Color* colors = new Color[size];
	for (int k = 0; k < size; k++) {
		points[k] = ColorOffset[k].offset;
		colors[k] = Color(ColorOffset[k].stopColor.opacity * 255, ColorOffset[k].stopColor.r, ColorOffset[k].stopColor.g, ColorOffset[k].stopColor.b);
	}

	LinearGradientBrush* fillBrush = new LinearGradientBrush(p1, p2, colors[0], colors[size - 1]);

	vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
	for (int k = 0; k < gradientTrans.size(); k++) {
		if (gradientTrans[k].first == "translate") {
			fillBrush->TranslateTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
		}
		else if (gradientTrans[k].first == "rotate") {
			fillBrush->RotateTransform(gradientTrans[k].second[0]);
		}
		else if (gradientTrans[k].first == "scale") {
			fillBrush->ScaleTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
		}
		else if (gradientTrans[k].first == "matrix") {
			Matrix matrix(
				gradientTrans[k].second[0], gradientTrans[k].second[1], gradientTrans[k].second[2],
				gradientTrans[k].second[3], gradientTrans[k].second[4], gradientTrans[k].second[5]
			);
			fillBrush->SetTransform(&matrix);
		}
	}

	fillBrush->SetWrapMode(WrapModeTileFlipXY);
	fillBrush->SetInterpolationColors(colors, points, size);

	delete[] colors;
	delete[] points;
	return fillBrush;
}
GradientType lineargradient::getType() {
	return LINEAR; 
}
void lineargradient::updateElement() {
	stringstream sss(strLine);
	string temp = "", attribute = "", value = "", transformGradient = "";

	while (sss >> attribute) {
		getline(sss, temp, '"');
		getline(sss, value, '"');

		if (attribute == "x1") {
			A.setX(parseSVGValue1(value)); // Sử dụng hàm parse mới
		}
		if (attribute == "y1") {
			A.setY(parseSVGValue1(value));
		}
		if (attribute == "x2") {
			B.setX(parseSVGValue1(value));
		}
		if (attribute == "y2") {
			B.setY(parseSVGValue1(value));
		}
		if (attribute == "gradientUnits") {
			if (value == "userSpaceOnUse")
				this->setGradId(0); // Giả sử 0 là USER_SPACE
			else
				this->setGradId(1); // Giả sử 1 là OBJECT_BOUNDING_BOX
		}
		if (attribute == "gradientTransform") {
			transformGradient = value;
		}
	}
	this->updateGradientTransform(transformGradient);
}