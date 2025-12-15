#ifndef _GROUP_H_
#define _GROUP_H_

class SVGGroup : public Shape {
	vector<Shape*> figureArray;
	SVGGroup* parent;
public:
	SVGGroup();
	~SVGGroup();
	SVGGroup(const SVGGroup&);
	SVGGroup& operator = (const SVGGroup&);

	void setParent(SVGGroup*);
	void setFigureArray(vector<Shape*>);
	SVGGroup* getParent();
	vector<Shape*> getFigureArray();
	void draw(Graphics& graphics) override;
	void addFigure(Shape*);
};

#endif