#ifndef _SHAPE_H_
#define _SHAPE_H_

class Shape {
protected:
    string fig, text_name, line_str;
    vector<pair<string, vector<float>>> transVct;
    bool isSelected;
    stroke strk;
    color fill;

public:
    Shape();
    virtual ~Shape();
    virtual void updateProperty();

    string getName();
    string getTextName();
    stroke getStroke();
    color getColor();
    vector<pair<string, vector<float>>> getTransVct();
    void setName(string);
    void setTextName(string);
    void setLine(string);
    void setStroke(stroke);
    void setSelected(bool s) { isSelected = s;  }
    void setColor(color);
    void updateTransformVct(string);
    void getTransformMatrix(Gdiplus::Matrix* matrix);
    virtual void draw(Graphics& graphics) = 0;
};

#endif