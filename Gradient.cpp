#include "Library.h"
using namespace std;

Gradient::Gradient(): units(OBJECT_BOUNDING_BOX)  {}
Gradient::~Gradient() {
	/*stopList.clear();
	if (!(this->transformMatrix.IsIdentity)) delete this->transformMatrix; */
}

void Gradient::setID(string id) {
	this->id = id; 
}

string Gradient::getID() const {
	return id; 
}

void Gradient::updateTransform(string str) {
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

vector<stop> Gradient::getStopList() {
	return this->stopList; 
}
void Gradient::getTransformMatrix(Matrix* matrix) {
	matrix->Reset(); 

	for (auto& trans : this->transVct) {
		string type = trans.first; 
		vector<float>& args = trans.second; 

		if (type == "translate") {
			float x = args.size() > 0 ? args[0] : 0;
			float y = args.size() > 1 ? args[1] : 0;
			matrix->Translate(x, y);
		}
		else if (type == "rotate") {
			if (args.size() > 0) matrix->Rotate(args[0]);
		}
		else if (type == "scale") {
			float x = args.size() > 0 ? args[0] : 1;
			float y = args.size() > 1 ? args[1] : x;
			matrix->Scale(x, y);
		}
		else if (type == "matrix") {
			if (args.size() >= 6) {
				Gdiplus::Matrix m(args[0], args[1], args[2], args[3], args[4], args[5]);
				matrix->Multiply(&m);
			}
		}
	}
}
void Gradient::addStop(float offset, color c) {
	stopList.push_back({c, offset }); 
}
