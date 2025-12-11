#include "Library.h"

text::text() :Shape() {
	textPos.setX(0);
	textPos.setY(0);
	fontSize = 16;
	content = "";
	dx = dy = 0;
	fontStyle = "";
	fontFamily = "Times New Roman";
	textAnchor = "start";
}

text::~text() {
	fontSize = 0;
	textPos.setX(0);
	textPos.setY(0);
	content = "";
}

void text::updateProperty() {
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
		if (attribute == "font-size")
			this->fontSize = stof(val);
		if (attribute == "text-anchor")
			this->textAnchor = val;
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

void text::drawText(Graphics& graphics, text* fig) {
	GraphicsState save = graphics.Save();
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	wstring wContent = converter.from_bytes(fig->getContent());
	wstring wFontFamily = converter.from_bytes(fig->getFontFamily());
	FontFamily WFF = wFontFamily.c_str();

	PointF textPosition;
	StringFormat stringFormat;
	if (fig->getTextAnchor() == "middle") {
		textPosition = PointF(fig->getTextPos().getX() + fig->getDx() - fig->getFontSize() / 25, fig->getTextPos().getY() + fig->getDy() - fig->getFontSize() / 4);
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
	}
	else if (fig->getTextAnchor() == "end") {
		textPosition = PointF(fig->getTextPos().getX() + fig->getDx() + fig->getFontSize() / 6.5, fig->getTextPos().getY() + fig->getDy() + fig->getFontSize() / 2.8);
		stringFormat.SetAlignment(StringAlignmentFar);
		stringFormat.SetLineAlignment(StringAlignmentFar);
	}
	else {
		textPosition = PointF(fig->getTextPos().getX() + fig->getDx() - fig->getFontSize() / 7, fig->getTextPos().getY() + fig->getDy() - fig->getFontSize() / 1.2);
		stringFormat.SetAlignment(StringAlignmentNear);
		stringFormat.SetLineAlignment(StringAlignmentNear);
	}

	GraphicsPath path;
	if (fig->getFontStyle() == "italic")
		path.AddString(wContent.c_str(), -1, &WFF, FontStyleItalic, fig->getFontSize() / 1.05, textPosition, &stringFormat);
	else if (fig->getFontStyle() == "bold")
		path.AddString(wContent.c_str(), -1, &WFF, FontStyleBold, fig->getFontSize() / 1.05, textPosition, &stringFormat);
	else path.AddString(wContent.c_str(), -1, &WFF, FontStyleRegular, fig->getFontSize() / 1.05, textPosition, &stringFormat);

	Pen penText(Color(fig->getStroke().getStrokeColor().opacity * 255, fig->getStroke().getStrokeColor().r, fig->getStroke().getStrokeColor().g, fig->getStroke().getStrokeColor().b), fig->getStroke().getStrokeWidth());
	SolidBrush fillText(Color(fig->getColor().opacity * 255, fig->getColor().r, fig->getColor().g, fig->getColor().b));
	vector<pair<string, vector<float>>> transVct = fig->getTransVct();

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
		else graphics.ScaleTransform(x, y);
	}

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.FillPath(&fillText, &path);
	graphics.DrawPath(&penText, &path);
	graphics.Restore(save);
}

point text::getTextPos() {
	return this->textPos;
}

float text::getFontSize() {
	return this->fontSize;
}

string text::getContent() {
	return this->content;
}

void text::setTextPos(float x, float y) {
	this->textPos.setX(x);
	this->textPos.setY(y);
}

void text::setFontSize(float fontSize) {
	this->fontSize = fontSize;
}

void text::setContent(string content) {
	this->content = content;
}

void text::setFontFamily(string fontFamily) {
	this->fontFamily = fontFamily;
}

void text::setTextAnchor(string textAnchor) {
	this->textAnchor = textAnchor;
}

void text::setFontStyle(string fontStyle) {
	this->fontStyle = fontStyle;
}

void text::setDx(float dx) {
	this->dx = dx;
}

void text::setDy(float dy) {
	this->dy = dy;
}

string text::getFontFamily() {
	return this->fontFamily;
}

string text::getTextAnchor() {
	return this->textAnchor;
}

string text::getFontStyle() {
	return this->fontStyle;
}

float text::getDx() {
	return this->dx;
}

float text::getDy() {
	return this->dy;
}