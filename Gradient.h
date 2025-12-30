#ifndef _GRADIENT_H_
#define _GRADIENT_H_


class stop {
private:

public:
	color stopColor;
	float offset = 0;
	stop();
	stop(color, float);
};

enum GradientUnits {
	USER_SPACE_ON_USE,
	OBJECT_BOUNDING_BOX // Mặc định
};

enum GradientType {
	LINEAR,
	RADIAL
};
class gradient {
protected:
	string strLine;
	int gradId;
	GradientUnits units = OBJECT_BOUNDING_BOX;
	vector<stop> stopVct;
	vector<pair<string, vector<float>>> gradientTrans;

public:
	vector<stop> getStopVct();
	int getGradId();
	string getStrLine();
	vector<pair<string, vector<float>>> getGradientTrans();

	void setStopVct(vector<stop>);
	void setStrLine(string);
	void setGradId(int);
	virtual GradientType getType() = 0;
	void addStop(stop);
	void updateGradientTransform(string);
	virtual void updateElement();
	gradient();
	gradient(const gradient&);
	gradient& operator =(const gradient&);
	virtual Gdiplus::Brush* createBrush(Gdiplus::RectF bounds) = 0;
	void setUnits(GradientUnits u) { units = u; }
	GradientUnits getUnits() { return units; }

	void getMatrixFromVector(const vector<pair<string, vector<float>>>& transVct, Gdiplus::Matrix* matrix);
	~gradient();
};

#endif