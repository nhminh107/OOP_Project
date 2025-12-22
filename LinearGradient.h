#ifndef LINEARGRADIENT_H
#define LINEARGRADIENT_H

class LinearGradient : public Gradient {
private: 
	point start; 
	point end;  

public: 
	LinearGradient(); 
	void setStart(float x, float y); 
	void setEnd(float x, float y); 
	Gdiplus::Brush* createBrush(const Gdiplus::RectF& shapeBound, float opacity) override;
	virtual GradientType getType() const = 0;
};
#endif // !LINEARGRADIENT_H
