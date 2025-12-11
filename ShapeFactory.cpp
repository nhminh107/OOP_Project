#include "Library.h"
Shape* factoryfigure::getFigure(string name) {
	int num = figureId[name];
	switch (num) {
	case 1: {
		return new rectangle();
		break;
	}
	case 2: {
		return new Circle();
		break;
	}
	case 3: {
		return new Ellipse();
		break;
	}
	case 4: {
		return new Line();
		break;
	}
	case 5: {
		return new Polyline();
		break;
	}
	case 6: {
		return new Polygon();
		break;
	}
	case 7: {
		return new text();
		break;
	}
	case 8: {
		return new path();
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

factoryfigure::factoryfigure() {
	figureId["rect"] = 1;
	figureId["Circle"] = 2;
	figureId["Ellipse"] = 3;
	figureId["Line"] = 4;
	figureId["Polyline"] = 5;
	figureId["Polygon"] = 6;
	figureId["text"] = 7;
	figureId["path"] = 8;
	figureId["g"] = 9;
}

unordered_map<string, int> factoryfigure::getFigureId() {
	return this->figureId;
}