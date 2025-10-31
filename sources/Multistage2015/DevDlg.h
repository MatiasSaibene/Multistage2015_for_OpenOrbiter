#pragma once

#include "../../../orbitersdk/samples/common/dialog/TabDlg.h"

class TabDialog: public TabbedDialog{
public:
	TabDialog(int id, int idtab);
	~TabDialog();
	void Update(double simt);
	int OnInitDialog( WPARAM wParam);
	int Closed();
};