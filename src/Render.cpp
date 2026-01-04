#include "Library.h"

void renderer::renderFigure(Graphics& graphics, SVGGroup* root) {
	if (root == nullptr) return;
	root->draw(graphics);
}