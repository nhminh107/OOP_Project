#include "Library.h"


vector<stop> gradient::getStopVct() {
	return this->stopVct;
}

void gradient::setStopVct(vector<stop> vct) {
	this->stopVct = vct;
}

int gradient::getGradId() {
	return this->gradId;
}

void gradient::setGradId(int gradId) {
	this->gradId = gradId;
}

void gradient::addStop(stop stp) {
	stopVct.push_back(stp);
}

gradient::gradient() {
	stopVct = {};
}

gradient::gradient(const gradient& grad) {
	this->stopVct = grad.stopVct;
	this->gradId = grad.gradId;
	this->gradientTrans = grad.gradientTrans;
	this->strLine = grad.strLine;
}

gradient& gradient::operator=(const gradient& grad) {
	if (this != &grad) {
		this->stopVct = grad.stopVct;
		this->gradId = grad.gradId;
		this->gradientTrans = grad.gradientTrans;
		this->strLine = grad.strLine;
	}
	return *this;
}

vector<pair<string, vector<float>>> gradient::getGradientTrans() {
	return this->gradientTrans;
}


void gradient::getMatrixFromVector(const vector<pair<string, vector<float>>>& transVct, Gdiplus::Matrix* matrix) {
	using namespace Gdiplus;

	// Matrix khởi tạo là Identity.
	// SVG Transform list: "A B C" -> Thực hiện A, rồi B, rồi C.
	// GDI+ Append: [New] * [Old] (sai) hay [Old] * [New] (đúng)?
	// GDI+ Append: Result = OldMatrix * NewMatrix.
	// Ví dụ: Đang có A. Append B -> A * B. Đúng theo chuẩn SVG.

	for (const auto& trans : transVct) {
		string type = trans.first;
		const vector<float>& args = trans.second;

		if (type == "translate") {
			float x = args.size() > 0 ? args[0] : 0;
			float y = args.size() > 1 ? args[1] : 0;
			matrix->Translate(x, y, MatrixOrderAppend); // SỬA THÀNH APPEND
		}
		else if (type == "rotate") {
			if (args.size() > 0) {
				float angle = args[0];
				if (args.size() == 3) {
					// rotate(angle, cx, cy) = translate(cx, cy) * rotate(angle) * translate(-cx, -cy)
					matrix->RotateAt(angle, PointF(args[1], args[2]), MatrixOrderAppend);
				}
				else {
					matrix->Rotate(angle, MatrixOrderAppend);
				}
			}
		}
		else if (type == "scale") {
			float sx = args.size() > 0 ? args[0] : 1;
			float sy = args.size() > 1 ? args[1] : sx;
			matrix->Scale(sx, sy, MatrixOrderAppend);
		}
		else if (type == "matrix") {
			if (args.size() >= 6) {
				Matrix m(args[0], args[1], args[2], args[3], args[4], args[5]);
				matrix->Multiply(&m, MatrixOrderAppend);
			}
		}
	}
}

void gradient::updateGradientTransform(string str) {
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
		gradientTrans.push_back(p);
	}
}

string gradient::getStrLine() {
	return this->strLine;
}

void gradient::setStrLine(string line) {
	this->strLine = line;
}

void gradient::updateElement() {
	return;
}

gradient::~gradient() {}

stop::stop() {
	offset = 0;
}

stop::stop(color clr, float off) :stopColor(clr), offset(off) {}