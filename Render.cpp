#include "Library.h"

void renderer::renderFigure(Graphics& graphics, group* root) {
	if (root == nullptr) return;
	root->draw(graphics);
}