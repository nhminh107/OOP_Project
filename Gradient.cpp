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

void Gradient::addStop(float offset, color c) {
	stopList.push_back({ offset, c }); 
}
