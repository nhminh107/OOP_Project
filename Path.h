#ifndef _PATH_H_
#define _PATH_H_

class SVGPath : public Shape {
private:
	vector<pair<char, vector<float>>> vct;
	string strokeLineJoin, strokeLineCap, fillRule;

public:
	SVGPath();
	~SVGPath();

	vector<pair<char, vector<float>>> getProp();
	string getStrokeLineJoin();
	string getStrokeLineCap();
	string getFillRule();
	RectF getTightBounds();
	void setStrokeLineJoin(string);
	void setStrokeLineCap(string);
	void setFillRule(string);
	Gdiplus::RectF getBoundingBox();
	void setVct(vector<pair<char, vector<float>>>);
	void draw(Graphics& graphics) override;
	void updateProperty();
};

#endif