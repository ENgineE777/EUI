
#pragma once

#include "NativeInclude.h"

class NativeCategories : public WidgetImpl
{
public:

	NativeCategories(EUIWidget* owner) : WidgetImpl(owner) {};

	virtual void UpdateChildPos() = 0;
};
