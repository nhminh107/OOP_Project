#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

class SVGRectangle : public Shape {
private:
	point root;
	float width, height;

public:
	SVGRectangle();
	~SVGRectangle();

	point getRoot();
	float getWidth();
	float getHeight();

	void setRoot(point);
	void setWidth(float);
	void setHeight(float);
	void updateProperty();
	void draw(Graphics&) override;
};

#endif
