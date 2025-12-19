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
int gradient::getGradID() {
	return this->gradID;
}

void gradient::setStopArr(vector<stop> s) {
	this->stopArr = s;
}
void gradient::setLineData(string line) {
	this->lineData = line;
}
void gradient::setGradID(int id) {
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
gradient::~gradient() {}