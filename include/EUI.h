
#pragma once

#include "EUIWindow.h"
#include "EUIButton.h"
#include "EUILabel.h"
#include "EUIEditBox.h"
#include "EUIMenu.h"
#include "EUILayout.h"
#include "EUIPanel.h"
#include "EUICheckBox.h"
#include "EUIListBox.h"
#include "EUIComboBox.h"
#include "EUICategories.h"
#include "EUITabPanel.h"

class EUI
{

public:
	static std::vector<EUIWindow*> wnds;
	static void Init(const char* theme);
	static void ReloadTheme(const char* theme);
	static int  Run();
};
