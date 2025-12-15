#include "Library.h"
Shape* factoryfigure::getFigure(string name) {
	int num = figureId[name];
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

factoryfigure::factoryfigure() {
	figureId["rect"] = 1;
	figureId["circle"] = 2;
	figureId["ellipse"] = 3;
	figureId["line"] = 4;
	figureId["polyline"] = 5;
	figureId["polygon"] = 6;
	figureId["text"] = 7;
	figureId["path"] = 8;
	figureId["g"] = 9;
}

unordered_map<string, int> factoryfigure::getFigureId() {
	return this->figureId;
}