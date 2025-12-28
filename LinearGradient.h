#ifndef _LINEARGRADIENT_H_
#define _LINEARGRADIENT_H_

class lineargradient : public gradient {
private:
	point A;
	point B; 
public:
	point getA();
	point getB();
	void setA(point);
	void setB(point);
	GradientType getType() override; 
	void updateElement();
	Gdiplus::Brush* createBrush(Gdiplus::RectF bounds) override;
	lineargradient();
	lineargradient(const lineargradient&);
	lineargradient& operator = (const lineargradient&);
};

#endif
