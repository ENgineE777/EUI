
#pragma once

#ifdef PLATFORM_WIN
#include "native/win/WinWidget.h"
#define WidgetImpl WinWidget
#endif
#ifdef PLATFORM_WIN_DX11
#include "native/win_dx11/WinDX11Widget.h"
#define WidgetImpl WinDX11Widget
#endif