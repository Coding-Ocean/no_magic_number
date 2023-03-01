#pragma once
struct DATA {
	int rows;
	int cols;
	float tileW;
	float tileH;
	float ofstX;
	float ofstY;
	float sizeText;
	float backR, backG, backB;
	float selectR, selectG, selectB;
	float textR, textG, textB;
	float holdTextR, holdTextG, holdTextB;
	int indexMouseOver;
	int indexMouseHolding;
	char* indices;
	short int numIndices;
};
