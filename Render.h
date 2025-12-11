#ifndef _RENDERER_H_
#define _RENDERER_H_

class renderer {
public:
	void renderFigure(Graphics&, group*);

	void drawRectangle(Graphics&, rectangle*);
	void drawEllipse(Graphics&, ellipse*);
	void drawLine(Graphics&, line*);
	
	void drawPolyline(Graphics&, polyline*);
	
	void drawPath(Graphics&, path*);
};

#endif
