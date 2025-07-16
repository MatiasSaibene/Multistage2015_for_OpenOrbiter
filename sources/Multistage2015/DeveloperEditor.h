#pragma once
#ifndef __DEVELOPER_EDITOR
#define __DEVELOPER_EDITOR


#include "..\..\Orbitersdk\Samples\Common\Dialog\TabDlg.h"

class Multistage2015;


class DeveloperEditorDlgTab;

class DeveloperEditorDlg: public TabbedDialog{

public:
	DeveloperEditorDlg ();
virtual ~DeveloperEditorDlg();
//int OnInitDialog (WPARAM wParam);
//int Closed();

};

class DeveloperEditorDlgTab: public TabPage {

	DeveloperEditorDlgTab(DeveloperEditorDlg *frame, int dlgId);

};

class StageTab: public DeveloperEditorDlgTab {
public:
	StageTab(DeveloperEditorDlg *frame);

};

#endif // !__DEVELOPER_EDITOR