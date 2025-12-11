#include "Library.h"

Polyline::Polyline() : Shape() {
	fill.r = fill.g = fill.b = 0;
	fill.opacity = 1;
}

Polyline::~Polyline() {
	Vers = {};
}

void Polyline::updateProperty() {
	stringstream ss(line_str);
	string property, val, temp;

	while (ss >> property) {
		getline(ss, temp, '"');
		getline(ss, val, '"');

		if (property == "points" || property == "point") {
			for (int i = 0; i < val.size(); i++)
				if (val[i] == ',' || val[i] == '/')
					val[i] = ' ';

			stringstream ss(val);
			string x = "", y = "";
			while (ss >> x >> y) {
				point p;
				p.setX(stof(x));
				p.setY(stof(y));
				Vers.push_back(p);
			}
		}
	}
}

vector<point> Polyline::getVers() {
	return this->Vers;
}

void Polyline::setVers(vector<point> Vers) {
	this->Vers = Vers;
}