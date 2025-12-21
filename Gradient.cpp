#include "Library.h"

vector<stop> gradient::getStopArr() {
	return this->stopArr;
}
vector<pair<string, vector<float>>> gradient::getGradTrans() {
	return this->gradTrans;
}
string gradient::getLineData() {
	return this->lineData;
}
string gradient::getGradID() {
	return this->gradID;
}

void gradient::setStopArr(vector<stop> s) {
	this->stopArr = s;
}
void gradient::setLineData(string line) {
	this->lineData = line;
}
void gradient::setGradID(string id) {
	this->gradID = id;
}
void gradient::addStop(stop s) {
	stopArr.push_back(s);
}

void gradient::updateElement() { return; }

gradient::gradient() {
	stopArr = {};
}
gradient::gradient(const gradient& grad) {
	for (int i = 0; i < stopArr.size(); i++) {
		stopArr[i] = grad.stopArr[i];
	}
}
gradient& gradient::operator=(const gradient& grad) {
	if (this != &grad) {
		for (int i = 0; i < stopArr.size();i++) {
			stopArr[i] = grad.stopArr[i];
		}
	}
	return *this;
}

void gradient::addGradTrans(string str) {
	string token = "";
	stringstream ss(str);

	while (getline(ss, token, ')')) {
		token += " )";
		while (token[0] == ' ' || token[0] == ',')
			token.erase(0, 1);

		stringstream sss(token);
		string name = "", property;
		getline(sss, name, '(');
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
		else if (name == "rotate") {
			p.first = name;
			string r = "";
			ssss >> r;
			ssss.ignore();
			p.second.push_back(stof(r));
		}
		else if (name == "scale") {
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
		gradTrans.push_back(p);
	}
}


stop::stop()
{
	offset = 0;
	stopColor = { 0, 0, 0, 1 };
}
stop::stop(float off, color c) {
	offset = off;
	stopColor = c;
}
gradient::~gradient() {}