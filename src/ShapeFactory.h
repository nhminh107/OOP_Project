#ifndef _SHAPEFACTORY_H_
#define _SHAPEFACTORY_H_

class ShapeFactory {
private:
	unordered_map<string, int> shapeID;

public:
	ShapeFactory();
	Shape* getShape(string name);
	unordered_map<string, int> getShapeID();
};

#endif