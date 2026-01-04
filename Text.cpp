#include "Library.h"

SVGText::SVGText() :Shape() {
	textPos.setX(0);
	textPos.setY(0);
	fontSize = 16;
	content = "";
	dx = dy = 0;
	fontStyle = "";
	fontFamily = "Times New Roman";
	textAnchor = "start";
}

SVGText::~SVGText() {
	fontSize = 0;
	textPos.setX(0);
	textPos.setY(0);
	content = "";
}

void SVGText::updateProperty() {
	size_t pos = line_str.find(">");
	string temp = line_str.substr(0, pos);
	stringstream ss(temp);
	string attribute, val, a;

	while (ss >> attribute) {
		getline(ss, a, '"');
		getline(ss, val, '"');

		if (attribute == "x")
			this->textPos.setX(stof(val));
		if (attribute == "y")
			this->textPos.setY(stof(val));
		if (attribute == "text-anchor")
			this->textAnchor = val;
		if (attribute == "font-size")
			this->fontSize = stof(val);
		if (attribute == "font-family") {
			if (val.find("Time") == string::npos)
				this->fontFamily = val;
		}
		if (attribute == "font-style")
			this->fontStyle = val;
		if (attribute == "dx")
			this->dx = stof(val);
		if (attribute == "dy")
			this->dy = stof(val);
	}
	this->content = text_name;
}

void SVGText::draw(Graphics& graphics) {
	GraphicsState save = graphics.Save();

	// 1. Chuẩn bị nội dung và Font
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	wstring wContent = converter.from_bytes(this->getContent());
	wstring wFontFamily = converter.from_bytes(this->getFontFamily());
	FontFamily WFF(wFontFamily.c_str());

	// 2. Lấy các thông số kích thước cơ bản để tính toán tọa độ
	float fSize = this->getFontSize();
	float baseX = this->getTextPos().getX() + this->getDx();
	float baseY = this->getTextPos().getY() + this->getDy();

	PointF textPosition;
	StringFormat stringFormat;
	string anchor = this->getTextAnchor();

	// 3. Xử lý căn lề (Text Anchor) và tính tọa độ vẽ
	if (anchor == "middle") {
		textPosition = PointF(baseX - fSize / 25.0f, baseY - fSize / 4.0f);
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
	}
	else if (anchor == "end") {
		textPosition = PointF(baseX + fSize / 6.5f, baseY + fSize / 2.8f);
		stringFormat.SetAlignment(StringAlignmentFar);
		stringFormat.SetLineAlignment(StringAlignmentFar);
	}
	else { // "start" hoặc mặc định
		textPosition = PointF(baseX - fSize / 7.0f, baseY - fSize / 1.2f);
		stringFormat.SetAlignment(StringAlignmentNear);
		stringFormat.SetLineAlignment(StringAlignmentNear);
	}

	// 4. Xác định kiểu Font (Bold/Italic/Regular)
	INT fontStyle = FontStyleRegular;
	string styleStr = this->getFontStyle();
	if (styleStr == "italic") {
		fontStyle = FontStyleItalic;
	}
	else if (styleStr == "bold") {
		fontStyle = FontStyleBold;
	}

	// 5. Tạo Path chữ
	GraphicsPath path;
	path.AddString(
		wContent.c_str(),
		-1,
		&WFF,
		fontStyle,
		fSize / 1.05f,
		textPosition,
		&stringFormat
	);

	// 6. Tạo Pen và Brush
	color sColor = this->getStroke().getStrokeColor();
	color fColor = this->getColor();

	Pen penText(Color(sColor.opacity * 255, sColor.r, sColor.g, sColor.b), this->getStroke().getStrokeWidth());
	SolidBrush fillText(Color(fColor.opacity * 255, fColor.r, fColor.g, fColor.b));

	// 7. Áp dụng Transform
	vector<pair<string, vector<float>>> transVct = this->getTransVct();
	for (const auto& trans : transVct) {
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
		else
			graphics.ScaleTransform(x, y);
	}

	// 8. Vẽ hình
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.FillPath(&fillText, &path);
	graphics.DrawPath(&penText, &path);

	graphics.Restore(save);
}

point SVGText::getTextPos() {
	return this->textPos;
}

float SVGText::getFontSize() {
	return this->fontSize;
}

string SVGText::getContent() {
	return this->content;
}

void SVGText::setTextPos(float x, float y) {
	this->textPos.setX(x);
	this->textPos.setY(y);
}

void SVGText::setFontSize(float fontSize) {
	this->fontSize = fontSize;
}

void SVGText::setContent(string content) {
	this->content = content;
}

void SVGText::setFontFamily(string fontFamily) {
	this->fontFamily = fontFamily;
}

void SVGText::setTextAnchor(string textAnchor) {
	this->textAnchor = textAnchor;
}

void SVGText::setFontStyle(string fontStyle) {
	this->fontStyle = fontStyle;
}

void SVGText::setDx(float dx) {
	this->dx = dx;
}

void SVGText::setDy(float dy) {
	this->dy = dy;
}

string SVGText::getFontFamily() {
	return this->fontFamily;
}

string SVGText::getTextAnchor() {
	return this->textAnchor;
}

string SVGText::getFontStyle() {
	return this->fontStyle;
}

float SVGText::getDx() {
	return this->dx;
}

float SVGText::getDy() {
	return this->dy;
}