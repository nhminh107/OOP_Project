#ifndef _SHAPEFACTORY_H_
#define _SHAPEFACTORY_H_

class factoryfigure {
private:
	unordered_map<string, int> figureId;

public:
	factoryfigure();
	Shape* getFigure(string name);
	unordered_map<string, int> getFigureId();
};

#endif