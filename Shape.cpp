#include "Library.h"
Shape::Shape() {
	grad = NULL;
	text_name = "", line_str = "", fig = "";
}

void Shape::updateProperty() {}

string Shape::getName() {
	return this->fig;
}

gradient* Shape::getGrad() {
	return this->grad;
}

void Shape::setGrad(gradient* grad) {
	if (grad->getGradId() == 1)
		this->grad = new lineargradient;
	else if (grad->getGradId() == 2)
		this->grad = new radialgradient;
}

void Shape::setName(string s) {
	this->fig = s;
}

void Shape::setStroke(stroke stroke) {
	this->strk = stroke;
}

void Shape::setColor(color fill) {
	this->fill = fill;
}

Shape:: ~Shape() {
	delete grad;
	grad = NULL;
}

void Shape::setTextName(string textName) {
	this->text_name = textName;
}

void Shape::setLine(string Line) {
	this->line_str = Line;
}

string Shape::getTextName() {
	return this->text_name;
}

stroke Shape::getStroke() {
	return this->strk;
}

color Shape::getColor() {
	return this->fill;
}

void Shape::updateTransformVct(string str) {
	string token = "";
	stringstream ss(str);

	while (getline(ss, token, ')')) {
		token += " )";
		while (token[0] == ' ' || token[0] == ',')
			token.erase(0, 1);
		stringstream sss(token);

		string name = "", property;
		getline(sss, name, '(');						//name = {Translate, rotate, scale};
		getline(sss, property, ')');
		for (int i = 0; i < property.size(); i++) {
			if (property[i] == ',') {
				property[i] = ' ';
				break;
			}
		}
		pair<string, vector<float>> p;

		stringstream ssss(property);
		if (name == "translate") {
			p.first = name;
			string x = "", y = "";
			ssss >> x >> y;
			ssss.ignore();
			p.second.push_back(stof(x));
			p.second.push_back(stof(y));
		}
		if (name == "rotate") {
			p.first = name;
			string r = "";
			ssss >> r;
			ssss.ignore();
			p.second.push_back(stof(r));
		}
		if (name == "scale") {
			p.first = name;
			int cnt = 0;
			for (int i = 0; i < property.size() - 1; i++) {
				if (isdigit(property[i]) && property[i + 1] == ' ') {
					++cnt;
				}
			}
			if (cnt == 1) {
				string s = "";
				ssss >> s;
				ssss.ignore();
				p.second.push_back(stof(s));
			}
			else {
				string x = "", y = "";
				ssss >> x >> y;
				ssss.ignore();
				p.second.push_back(stof(x));
				p.second.push_back(stof(y));
			}
		}
		else if (name == "matrix") {
			p.first = name;
			string x1 = "", y1 = "", x2 = "", y2 = "", x3 = "", y3 = "";
			ssss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
			ssss.ignore();
			p.second.push_back(stof(x1));
			p.second.push_back(stof(y1));
			p.second.push_back(stof(x2));
			p.second.push_back(stof(y2));
			p.second.push_back(stof(x3));
			p.second.push_back(stof(y3));
		}
		transVct.push_back(p);
	}
}
vector<pair<string, vector<float>>> Shape::getTransVct() {
	return this->transVct;
}

void Shape::convertGradient(gradient* grad) {
	if (grad->getGradId() == 1) {
		this->grad = new lineargradient;
	}
	else if (grad->getGradId() == 2) {
		this->grad = new radialgradient;
	}
	else this->grad = NULL;
	this->grad = grad;
}