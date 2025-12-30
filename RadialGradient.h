#ifndef _RADIALGRADIENT_H_
#define _RADIALGRADIENT_H_

class radialgradient : public gradient {
private:
	point center; // cx, cy
	point focal;  // fx, fy
	float r;      // radius
	bool isLink;

public:
	point getCenter() const { return center; }
	point getFocal() const { return focal; }
	float getR() const { return r; }

	void updateElement();

	radialgradient();
	radialgradient(const radialgradient&);
	radialgradient& operator = (const radialgradient&);
	GradientType getType() override; 
	Gdiplus::Brush* createBrush(Gdiplus::RectF bounds) override;
	bool getIsLink();
	void setIsLink(bool);
};

#endif
