#ifndef _GROUP_H_
#define _GROUP_H_

class SVGGroup : public Shape {
	vector<Shape*> shapeArray;
	SVGGroup* parent;
public:
	SVGGroup();
	~SVGGroup();
	SVGGroup(const SVGGroup&);
	SVGGroup& operator = (const SVGGroup&);

	void setParent(SVGGroup*);
	void setShapeArray(vector<Shape*>);
	SVGGroup* getParent();
	vector<Shape*> getShapeArray();
	void draw(Graphics& graphics) override;
	void addShape(Shape*);
};

#endif