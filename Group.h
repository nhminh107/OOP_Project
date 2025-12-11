#ifndef _GROUP_H_
#define _GROUP_H_

class group :public Shape {
	vector<Shape*> figureArray;
	group* parent;
public:
	SVGGroup();
	SVGGroup(const SVGGroup&);
	SVGGroup& operator = (const SVGGroup&);

	void setParent(group*);
	void setFigureArray(vector<Shape*>);
	group* getParent();
	vector<Shape*> getFigureArray();
	void draw(Graphics& graphics) override;
	void addFigure(Shape*);
};

#endif