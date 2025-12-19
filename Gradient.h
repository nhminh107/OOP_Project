#ifndef _GRADIENT_H_
#define _GRADIENT_H_
#include "Library.h"

class stop {
private:

public:
	float offset = 0;
	color stopColor;
	stop();
	stop(float, color);

};

class gradient {
protected:
	vector<stop> stopArr;
	vector<pair<string, vector<float>>> gradTrans;
	string lineData;
	int gradID;
public:
	vector<stop> getStopArr();
	vector<pair<string, vector<float>>> getGradTrans();
	string getLineData();
	int getGradID();

	void setStopArr(vector<stop>);
	void setLineData(string);
	void setGradID(int);

	void addStop(stop s);
	void addGradTrans(string);
	virtual void updateElement();

	gradient();
	gradient(const gradient&);
	gradient& operator=(const gradient&);
	~gradient();

};
#endif