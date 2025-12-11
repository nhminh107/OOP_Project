#ifndef _FIGURE_H_
#define _FIGURE_H_

class Shape {
protected:
    string fig, text_name, line_str;
    vector<pair<string, vector<float>>> transVct;

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
    void setColor(color);
    void updateTransformVct(string);

    virtual void draw(Graphics& graphics) = 0;
};

#endif