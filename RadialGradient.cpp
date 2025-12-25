#include "Library.h"

float radialgradient::getCx() {
	return cx;
}
float radialgradient::getCy() {
	return cy;
}
float radialgradient::getR() {
	return r;
}
float radialgradient::getFx() {
	return fx;
}
float radialgradient::getFy() {
	return fy;
}

radialgradient::radialgradient() {
	isLink = false;
	cx = cy = r = fx = fy = 0;
}

radialgradient::radialgradient(const radialgradient& radial) {
	cx = radial.cx;
	cy = radial.cy;
	r = radial.r;
	fx = radial.fx;
	fy = radial.fy;
}
radialgradient& radialgradient::operator = (const radialgradient& radial) {
	if (this != &radial) {
		cx = radial.cx;
		cy = radial.cy;
		r = radial.r;
		fx = radial.fx;
		fy = radial.fy;
	}
	return *this;
}

GradientType radialgradient::getType() {
	return GradientType::RADIAL; 
}

Gdiplus::Brush* radialgradient::createBrush(RectF bound) {
	using namespace Gdiplus;
	float cx = this->getCx();
	float cy = this->getCy();
	float r = this->getR();
	vector<stop> ColorOffset = this->getStopVct();
	int size = ColorOffset.size();

	GraphicsPath* pathE = new GraphicsPath();
	pathE->AddEllipse(RectF(cx - r, cy - r, r * 2, r * 2));
	PathGradientBrush* fillPath = new PathGradientBrush(pathE);

	if (ColorOffset[0].offset != 0) {
		color first = ColorOffset[0].stopColor;
		float offset = ColorOffset[0].offset;
		color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
		ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
		size++;
	}

	if (ColorOffset[size - 1].offset != 1) {
		color last = ColorOffset[size - 1].stopColor;
		float offset = ColorOffset[size - 1].offset;
		color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
		ColorOffset.push_back(stop(one, 1));
		size++;
	}

	float* points = new float[size];
	Color* colors = new Color[size];
	for (int k = 0; k < size; k++) {
		points[k] = 1 - ColorOffset[size - k - 1].offset;
		colors[k] = Color(ColorOffset[size - k - 1].stopColor.opacity * 255, ColorOffset[size - k - 1].stopColor.r, ColorOffset[size - k - 1].stopColor.g, ColorOffset[size - k - 1].stopColor.b);
	}

	vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
	for (int k = 0; k < gradientTrans.size(); ++k) {
		if (gradientTrans[k].first == "translate") {
			fillPath->TranslateTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
		}
		else if (gradientTrans[k].first == "rotate") {
			fillPath->RotateTransform(gradientTrans[k].second[0]);
		}
		else if (gradientTrans[k].first == "scale") {
			fillPath->ScaleTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
		}
		else if (gradientTrans[k].first == "matrix") {
			Matrix matrix(
				gradientTrans[k].second[0], gradientTrans[k].second[1], gradientTrans[k].second[2],
				gradientTrans[k].second[3], gradientTrans[k].second[4], gradientTrans[k].second[5]
			);
			fillPath->SetTransform(&matrix);
			pathE->Transform(&matrix);
		}
	}

	fillPath->SetInterpolationColors(colors, points, size);

	delete pathE;
	delete[] colors;
	delete[] points;
	return fillPath;
}

float parseSVGValue2(string val) {
	if (val.empty()) return 0;
	// Nếu có ký tự '%', chia giá trị cho 100
	if (val.find('%') != string::npos) {
		return stof(val.substr(0, val.find('%'))) / 100.0f;
	}
	// Nếu không, trả về giá trị số thực bình thường
	return stof(val);
}
void radialgradient::updateElement() {
	stringstream sss(strLine);
	string temp = "", attribute = "", value = "", transformGradient = "";

	while (sss >> attribute) {
		getline(sss, temp, '"');
		getline(sss, value, '"');

		if (attribute == "cx") {
			this->cx = parseSVGValue2(value);
		}
		if (attribute == "cy") {
			this->cy = parseSVGValue2(value);
		}
		if (attribute == "r") {
			this->r = parseSVGValue2(value);
		}
		if (attribute == "fx") {
			this->fx = parseSVGValue2(value);
		}
		if (attribute == "fy") {
			this->fy = parseSVGValue2(value);
		}
		if (attribute == "gradientUnits") {
			if (value == "userSpaceOnUse")
				this->setGradId(0);
			else
				this->setGradId(1);
		}
		if (attribute == "gradientTransform") {
			transformGradient = value;
		}
		if (attribute == "xlink:href") {
			this->setIsLink(true);
		}
	}
	this->updateGradientTransform(transformGradient);
}

bool radialgradient::getIsLink() {
	return this->isLink;
}

void radialgradient::setIsLink(bool link) {
	this->isLink = link;
}