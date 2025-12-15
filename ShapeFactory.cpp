#include "Library.h"
Shape* ShapeFactory::getShape(string name) {
	int num = shapeID[name];
	switch (num) {
	case 1: {
		return new SVGRectangle();
		break;
	}
	case 2: {
		return new SVGCircle();
		break;
	}
	case 3: {
		return new SVGEllipse();
		break;
	}
	case 4: {
		return new SVGLine();
		break;
	}
	case 5: {
		return new SVGPolyline();
		break;
	}
	case 6: {
		return new SVGPolygon();
		break;
	}
	case 7: {
		return new SVGText();
		break;
	}
	case 8: {
		return new SVGPath();
		break;
	}
	case 9: {
		return new SVGGroup();
		break;
	}
	default: return NULL;
	}
	return NULL;
}

ShapeFactory::ShapeFactory() {
	shapeID["rect"] = 1;
	shapeID["circle"] = 2;
	shapeID["ellipse"] = 3;
	shapeID["line"] = 4;
	shapeID["polyline"] = 5;
	shapeID["polygon"] = 6;
	shapeID["text"] = 7;
	shapeID["path"] = 8;
	shapeID["g"] = 9;
}

unordered_map<string, int> ShapeFactory::getShapeID() {
	return this->shapeID;
}