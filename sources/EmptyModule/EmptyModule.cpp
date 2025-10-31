/*********************************************************************************************
This file is part of Multistage2015 project
Copyright belogs to Fred18 for module implementation and its code
Biggest Credit goes to Vinka for his idea of Multistage.dll. None of his code was used here since his addons are all closed source.
Credit goes to Face for having pointed me to the GetPrivateProfileString
Credit goes to Hlynkacg for his OrientForBurn function which was the basis on which I developed the Attitude Function.

Multistage2015 is distributed FREEWARE. Its code is distributed along with the dll. Nobody is authorized to exploit the module or the code or parts of them commercially directly or indirectly.
You CAN distribute the dll together with your addon but in this case you MUST:
-	Include credit to the author in your addon documentation;
-	Add to the addon documentation the official link of Orbit Hangar Mods for download and suggest to download the latest and updated version of the module.
You CAN use parts of the code of Multistage2015, but in this case you MUST:
-	Give credits in your copyright header and in your documentation for the part you used.
-	Let your project be open source and its code available for at least visualization by other users.
You CAN NOT use the entire code for making and distributing the very same module claiming it as your work entirely or partly.
You CAN NOT claim that Multistage2015 is an invention of yourself or a work made up by yourself, or anyhow let intend that is not made and coded by the author.
You install and use Multistage2015 at your own risk, author will not be responsible for any claim or damage subsequent to its use or for the use of part of it or of part of its code.
*********************************************************************************************/

//****************************************************/

// ==============================================================
//						MultiStage2015
//                  
//					       By Fred18
//                  
//
// EmptyModule.cpp
// ==============================================================





//############################################################################//
#define ORBITER_MODULE
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_NONSTDC_NO_DEPRECATE
#include <math.h>
#include <stdio.h>
//#include "orbitersdk.h"


#include "EmptyModule.h"

//Creation
EmptyModule::EmptyModule(OBJHANDLE hObj, int fmodel) :VESSEL3(hObj, fmodel) {}
EmptyModule::~EmptyModule() {}

void EmptyModule::clbkSetClassCaps(FILEHANDLE cfg) {
	SetEmptyMass(1000);
	SetClipRadius(0.01);
	double ro = 10;
	TOUCHDOWNVTX td[4];
	for (int i = 0; i < 4; i++)
	{

		td[i].damping = 3e5;
		td[i].mu = 3;
		td[i].mu_lng = 3;
		td[i].stiffness = 3e5;
	}
	td[0].pos.x = -cos(30 * RAD) * ro;
	td[0].pos.y = -sin(30 * RAD) * ro;
	td[0].pos.z = 0.01;
	td[1].pos.x = 0;
	td[1].pos.y = 1 * ro;
	td[1].pos.z = 0.01;
	td[2].pos.x = cos(30 * RAD) * ro;
	td[2].pos.y = -sin(30 * RAD) * ro;
	td[2].pos.z = 0.01;
	td[3].pos.x = 0;
	td[3].pos.y = 0;
	td[3].pos.z = 5 * ro;


	SetTouchdownPoints(td, 4);
	return;
}

void EmptyModule::clbkLoadStateEx(FILEHANDLE scn, void* vs)
{
	char* line;
	while (oapiReadScenario_nextline(scn, line)) {
		ParseScenarioLineEx(line, vs);
	}

}
//############################################################################//
void EmptyModule::clbkSaveState(FILEHANDLE scn)
{
	SaveDefaultState(scn);

}
/*
void EmptyModule::clbkSetStateEx (const void *status)
{

	return;
}*/
//Creation
DLLCLBK void InitModule(HINSTANCE hModule) {}
DLLCLBK void ExitModule(HINSTANCE hModule) {}
DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel, int flightmodel) { return new EmptyModule(hvessel, flightmodel); }
DLLCLBK void ovcExit(VESSEL* vessel) { if (vessel)delete(EmptyModule*)vessel; }

