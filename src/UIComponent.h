#pragma once
#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "Entity.h"
#include "UIManager.h"

class UIComponent : public Entity{

public:

	std::string texture;
	Vector4 color;
	Vector3 size;

	virtual void render(void);
	virtual void render(float);
	virtual void update(double);

	UIManager* ui;
	UIElement* uiElement = NULL;
};
#endif