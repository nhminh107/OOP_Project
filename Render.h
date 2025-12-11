#ifndef _RENDERER_H_
#define _RENDERER_H_

class renderer {
public:
	void renderFigure(Graphics&, SVGGroup*);

	void drawRectangle(Graphics&, rectangle*);
	void drawEllipse(Graphics&, Ellipse*);
	void drawLine(Graphics&, Line*);
	void drawPolygon(Graphics&, Polygon*);
	void drawPolyline(Graphics&, Polyline*);
	void drawText(Graphics&, text*);
	void drawPath(Graphics&, path*);
};

#endif
