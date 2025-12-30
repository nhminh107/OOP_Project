#ifndef _SHAPE_H_
#define _SHAPE_H_

class Shape {
protected:
    string shape, text_name, line_str;
    vector<pair<string, vector<float>>> transVct;
    bool isSelected;
    stroke strk;
    color fill;

    gradient* grad; 

public:
    Shape();
    virtual ~Shape();
    virtual void updateProperty();

    string getName();
    string getTextName();
    stroke getStroke();
    color getColor();
    gradient* getGrad(); 
    vector<pair<string, vector<float>>> getTransVct();
    void setName(string);
    void setTextName(string);
    void setGrad(gradient* grad);
    void setLine(string);
    //virtual Gdiplus::RectF getBoundingBox();
    void setStroke(stroke);
    void setSelected(bool s) { isSelected = s;  }
    void setColor(color);
    void updateTransformVct(string);
    void convertGradient(gradient* grad); 
    void getTransformMatrix(Gdiplus::Matrix* matrix);
    virtual void draw(Graphics& graphics) = 0;
};

#endif