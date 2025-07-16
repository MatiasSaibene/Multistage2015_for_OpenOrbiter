#pragma once
/****************************************************************************
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




 */
// ==============================================================
//						MultiStage2015
//                  
//					       By Fred18
//                  
//
// MS_Camera.h
// ==============================================================


#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define ORBITER_MODULE

#include "..//..//Orbitersdk//include//Orbitersdk.h"
using namespace std;

class MS_Camera:public VESSEL4{
public:
 MS_Camera(OBJHANDLE hObj,int fmodel);
 ~MS_Camera();
 void clbkSetClassCaps(FILEHANDLE cfg) override;
 void clbkLoadStateEx(FILEHANDLE scn,void *vs) override;
 void clbkSaveState(FILEHANDLE scn) override;
 void clbkPreStep(double simt, double simdt, double mjd) override;
 void clbkPostStep(double simt, double simdt, double mjd) override;
 int clbkConsumeBufferedKey(DWORD key, bool down, char *kstate) override;
 bool clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp) override;
 void clbkRenderHUD(int mode, const HUDPAINTSPEC *hps, SURFHANDLE hDefaultTex) override;
 bool clbkLoadGenericCockpit() override;
 //bool clbkLoadVC(int id);
 bool clbkLoadPanel2D(int id,PANELHANDLE hPanel,DWORD viewW,DWORD viewH) override;
 int clbkGeneric (int msgid, int prm, void *context) override;
 static bool InputNewRefVessel(void *id, char *str, void *usrdata);
 //void clbkSetStateEx (const void *status);

 char RefVessel[256];
 double RefHeight;
 oapi::Pen *penwhite;
 VESSEL4 *v;
 double Distance;
 double Polar;
 double Azimuth;
 double Aperture;
 //bool FirstLoop;
 OBJHANDLE hMS;
 //bool TargetLock;
};



