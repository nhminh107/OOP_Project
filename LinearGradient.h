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

	lineargradient();
	lineargradient(const lineargradient&);
	lineargradient& operator = (const lineargradient&);
};

#endif
