
#include <EUI.h>

class Listener : public EUIWidget::Listener
{
public:
	virtual void OnMouseMove(EUIWidget* sender, int mx, int my)
	{
	}

	virtual void OnRightMouseUp(EUIWidget* sender, int mx, int my)
	{
	}

	virtual void OnMenuItem(EUIWidget* sender, int id)
	{
	}

	virtual void OnUpdate(EUIWidget* sender)
	{
	}

	virtual void OnComboBoxChange(EUIWidget* sender, int index)
	{
	}

	virtual void OnResize(EUIWidget* sender)
	{
	}
};

Listener listiner;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	EUI::Init("EUI/theme.dat");

	EUIWindow* wnd = new EUIWindow("Editor", false, true, 30, 30, 800, 600);

	EUILayout* lt = new EUILayout(wnd, true);

	EUIMenu* menu = new EUIMenu(wnd);
	menu->SetListener(&listiner, 0);

	menu->StartSubMenu("File");

	menu->AddItem(1301, "New...");
	menu->AddItem(1302, "Open...");
	menu->AddItem(1303, "Save...");
	menu->AddSeparator();
	menu->AddItem(1303, "Exit...");

	menu->EndSubMenu();

	menu->AddItem(1301, "About");


	EUITabPanel* tabPanel = new EUITabPanel(lt, 30, 50, 100, 30);
	tabPanel->SetListener(&listiner, 0);

	tabPanel->AddTab("TAb1");
	tabPanel->AddTab("TAb2");
	tabPanel->AddTab("TAb3");

	{
		EUIButton* btn = new EUIButton(tabPanel, "<a>Just</a> <b>Button</b>", 10, 20, 100, 30);
		btn->SetListener(&listiner, 0);

		EUICheckBox* chk = new EUICheckBox(tabPanel, "Selected RadioButton", 10, 60, 100, 30);

		EUIComboBox* cbx = new EUIComboBox(tabPanel, 10, 100, 100, 90);

		cbx->AddItem("itejl ljl");
		cbx->AddItem("jgkjhkkjh kj");

		cbx->SetListener(&listiner, 0);

		tabPanel->AddWidget2Tab(0, btn);
		tabPanel->AddWidget2Tab(0, chk);
		tabPanel->AddWidget2Tab(0, cbx);

		EUIListBox* lbx = new EUIListBox(tabPanel, 10, 10, 200, 100);
		lbx->AddItem("Tset1", NULL);
		lbx->AddItem("Tset2", NULL);

		EUILabel* lbl = new EUILabel(tabPanel, "<a>Just</a> <b>Button</b>", 10, 120, 100, 30);

		EUIEditBox* ebx = new EUIEditBox(tabPanel, "0", 10, 160, 100, 30, EUIEditBox::InputInteger);

		tabPanel->AddWidget2Tab(1, lbx);
		tabPanel->AddWidget2Tab(1, lbl);
		tabPanel->AddWidget2Tab(1, ebx);

		EUIPanel* pn = new EUIPanel(tabPanel, 10, 10, 100, 30);
		pn->SetListener(&listiner, EUIWidget::OnResize | EUIWidget::OnUpdate);

		tabPanel->AddWidget2Tab(2, pn);
	}

	EUIPanel* pn = new EUIPanel(lt, 10, 10, 100, 30);

	EUILayout* lt2 = new EUILayout(pn, false);

	EUICategories* cat = new EUICategories(lt2, 0, 0, 10, 10);

	EUICheckBox* chk1 = new EUICheckBox(cat, "Check Me1", 10, 60, 100, 30);
	EUICheckBox* chk2 = new EUICheckBox(cat, "Check Me2", 10, 60, 100, 30);
	EUICheckBox* chk3 = new EUICheckBox(cat, "Check Me3", 10, 60, 100, 30);
	EUICheckBox* chk4 = new EUICheckBox(cat, "Check Me4", 10, 60, 100, 30);
	EUICheckBox* chk5 = new EUICheckBox(cat, "Check Me5", 10, 60, 100, 30);
	EUICheckBox* chk6 = new EUICheckBox(cat, "Check Me6", 10, 60, 100, 30);
	EUICheckBox* chk7 = new EUICheckBox(cat, "Check Me7", 10, 60, 100, 30);
	EUICheckBox* chk8 = new EUICheckBox(cat, "Check Me8", 10, 60, 100, 30);

	cat->RegisterChildInCategory("cat1", chk1);
	cat->RegisterChildInCategory("cat1", chk2);
	cat->RegisterChildInCategory("cat1", chk3);
	cat->RegisterChildInCategory("cat2", chk4);
	cat->RegisterChildInCategory("cat2", chk5);
	cat->RegisterChildInCategory("cat3", chk6);
	cat->RegisterChildInCategory("cat3", chk7);
	cat->RegisterChildInCategory("cat4", chk8);

	wnd->Show(true);

	return EUI::Run();
}