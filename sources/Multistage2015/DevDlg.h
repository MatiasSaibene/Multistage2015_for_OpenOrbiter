#pragma once
#ifndef DEVDLG_H
#define DEVDLG_H
#include "../../Orbitersdk/samples/Common/Dialog/TabDlg.h"

class TabDialog: public TabbedDialog{
	public:
		TabDialog(int id, int idtab);
		~TabDialog()override;
		void Update(double simt);
		int OnInitDialog( WPARAM wParam) override;
		int Closed() override;
};

#endif //!_DEVDLG_H