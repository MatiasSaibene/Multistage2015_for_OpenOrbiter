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
// MS_Crawler.h
// ==============================================================




#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define ORBITER_MODULE
#define NANIM 64
#define NCING 8
#include "..//..//Orbitersdk//include//Orbitersdk.h"
using namespace std;


class MS_Crawler:public VESSEL4{
public:
 MS_Crawler(OBJHANDLE hObj,int fmodel);
 ~MS_Crawler();
 void clbkSetClassCaps(FILEHANDLE cfg) override;
 void clbkLoadStateEx(FILEHANDLE scn,void *vs) override;
 void clbkSaveState(FILEHANDLE scn) override;
 int clbkConsumeBufferedKey(DWORD key,bool down,char *kstate) override;
 int clbkConsumeDirectKey(char *kstate) override;
 void clbkPreStep(double simt, double simdt, double mjd) override;
 void clbkPostCreation() override;
 void MoveAround(bool FWD,bool BWD, bool LFT, bool RGT);
 bool FWD,BWD,LFT,RGT;
 void UpdateAnimations(bool Reset);
  double Speed;
  double LengthCrawl;
  double rt;
  double earth_circ;
  double each_deg;
  double AngSpeed;
  double TangentSpd;
  //void LoadMeshes();
  void DefineAnimations();
  /*MESHHANDLE Cingolo_mh[NANIM][NCING];
  UINT Cingolo[NANIM][NCING];
  UINT AnimCing[NANIM][NCING];
  MGROUP_TRANSLATE *Tr11[NANIM];
  MGROUP_ROTATE *Rot11[NANIM];
  MGROUP_TRANSLATE *Tr21[NANIM];
  MGROUP_ROTATE *Rot21[NANIM];
  MGROUP_TRANSLATE *Tr31[NANIM];
  MGROUP_ROTATE *Rot31[NANIM];
  MGROUP_TRANSLATE *Tr41[NANIM];
  MGROUP_ROTATE *Rot41[NANIM];
  bool firstStep;
  double Initials[NANIM];
  double states[NANIM];*/

  UINT animCing[NANIM][NCING];
  double StateCing[NANIM][NCING];
  double Initials[NANIM][NCING];
 MGROUP_TRANSLATE *Tr1[NANIM][NCING];
  MGROUP_ROTATE *Rot1[NANIM][NCING];
  MGROUP_TRANSLATE *Tr2[NANIM][NCING];
  MGROUP_ROTATE *Rot2[NANIM][NCING];
  MGROUP_TRANSLATE *Tr3[NANIM][NCING];
  MGROUP_ROTATE *Rot3[NANIM][NCING];
  MGROUP_TRANSLATE *Tr4[NANIM][NCING];
  MGROUP_ROTATE *Rot4[NANIM][NCING];
  
  
  
  UINT AnimLifter;
  double StateLifter;
  double SpeedLifter;
  MGROUP_TRANSLATE *Lifter;
  MGROUP_SCALE *Pistons;
  bool GoingUp,GoingDown;
  VECTOR3 CameraOff;
  void UpdateLifter();

  LightEmitter* lights[4];
  
	COLOUR4 col_d;
	COLOUR4 col_s;
	COLOUR4 col_a;
	COLOUR4 col_white;



OBJHANDLE hPad;
VESSEL4 *vPad;
VESSELSTATUS2 vsPad;
ATTACHMENTHANDLE AttPadCrawler,AttPadRocket,AttCrawlerPad;

MESHHANDLE msh;
bool PadDetached;

char PadMesh[256];
char PadModule[256];
bool wCustomPadModule;
bool NoPad;
};



