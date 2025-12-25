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
	point getStart() { return start; }
	point getEnd() { return end;  }
	Gdiplus::Brush* createBrush(const Gdiplus::RectF& shapeBound, float opacity) override;
	GradientType getType() const ;
};
#endif // !LINEARGRADIENT_H
