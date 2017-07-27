
#include "EUITabSheet.h"
#include "native/win/WinTabSheet.h"

EUITabSheet::EUITabSheet(EUITabPanel* owner, const char* txt) : EUIWidget(owner, txt)
{
	nativeWidget = new WinTabSheet(this);
}

EUITabSheet::~EUITabSheet()
{
}

NativeTabSheet* EUITabSheet::Native()
{
	return NULL;
}

void EUITabSheet::SetText(const char* text)
{
	EUIWidget::SetText(text);
	Native()->SetText(text);
}

void EUITabSheet::MakeAsCurrent()
{
	Native()->MakeAsCurrent();
}