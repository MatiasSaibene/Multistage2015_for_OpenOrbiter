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
// MS_Hangar.cpp
// ==============================================================





//############################################################################//
#define ORBITER_MODULE
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_NONSTDC_NO_DEPRECATE
#include <math.h>
#include <stdio.h>
//#include "orbitersdk.h"


#include "MS_Hangar.h"

//Creation
MS_Hangar::MS_Hangar(OBJHANDLE hObj,int fmodel):VESSEL3(hObj,fmodel){

	PadHangar = NULL;

	for (int i = 0; i < 5; i++) {
		Hangarlight[i] = NULL;
	}

	col_d = { 0, 0, 0, 0 };

	col_s = { 0, 0, 0, 0 };

	col_a = { 0, 0, 0, 0 };

	col_white = { 0, 0, 0, 0 };

	BridgeTrY = 0;

	CablesTrX = 0;

	CablesTrY = 0;

	CablesTrZ = 0;

	HookTrX = 0;

	HookTrY = 0;

	HookTrZ = 0;

	TrailerTrX = 0;

	hangaranims = _V(0, 0, 0);
}
MS_Hangar::~MS_Hangar(){}
bool MS_Hangar::UpdateHangarAnimations(VECTOR3 pos)
{

	SetAnimation(HookTrZ,(pos.z/164));
	SetAnimation(CablesTrZ,(pos.z/164));
	SetAnimation(HookTrZ,(pos.z/164));
	SetAnimation(CablesTrZ,(pos.z/164));
	SetAnimation(HookTrX,(pos.x+40)/80);
	SetAnimation(CablesTrX,(pos.x+40)/80);
	SetAnimation(TrailerTrX,(pos.x+40)/80);
	SetAnimation(HookTrX,(pos.x+40)/80);
	SetAnimation(CablesTrX,(pos.x+40)/80);
	SetAnimation(TrailerTrX,(pos.x+40)/80);
	SetAnimation(HookTrY,((pos.y+40)/80));
	SetAnimation(CablesTrY,((pos.y+40)/80));
	SetAnimation(BridgeTrY,((pos.y+40)/80));
	SetAnimation(HookTrY,((pos.y+40)/80));
	SetAnimation(CablesTrY,((pos.y+40)/80));
	SetAnimation(BridgeTrY,((pos.y+40)/80));
	return TRUE;
}
int MS_Hangar::ProcessHangar(char *kstate)
{
	VECTOR3 pos,dir,rot;
	GetAttachmentParams(PadHangar,pos,dir,rot);
	const double delta=0.1;
	if(KEYDOWN(kstate,OAPI_KEY_UP)&&(!KEYMOD_CONTROL(kstate))&&(KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		pos.z+=delta;
		if(pos.z>164) pos.z=164;
		SetAttachmentParams(PadHangar,pos,dir,rot);
	}

	if(KEYDOWN(kstate,OAPI_KEY_DOWN)&&(!KEYMOD_CONTROL(kstate))&&(KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		pos.z-=delta;
		if(pos.z<0) pos.z=0;
		SetAttachmentParams(PadHangar,pos,dir,rot);
	}

	if(KEYDOWN(kstate,OAPI_KEY_LEFT)&&(!KEYMOD_CONTROL(kstate))&&(KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		pos.x-=delta;
		if(pos.x<-40) pos.x=-40;
		SetAttachmentParams(PadHangar,pos,dir,rot);
	}

	if(KEYDOWN(kstate,OAPI_KEY_RIGHT)&&(!KEYMOD_CONTROL(kstate))&&(KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		pos.x+=delta;
		if(pos.x>40) pos.x=40;
		SetAttachmentParams(PadHangar,pos,dir,rot);
	}

	if(KEYDOWN(kstate,OAPI_KEY_UP)&&(KEYMOD_CONTROL(kstate))&&(KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		pos.y-=delta;
		if(pos.y<-40) pos.y=-40;
		SetAttachmentParams(PadHangar,pos,dir,rot);
	}
	
	if(KEYDOWN(kstate,OAPI_KEY_DOWN)&&(KEYMOD_CONTROL(kstate))&&(KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		pos.y+=delta;
		if(pos.y>40) pos.y=40;
		SetAttachmentParams(PadHangar,pos,dir,rot);
	}

	UpdateHangarAnimations(pos);
	hangaranims=pos;
	//sprintf(oapiDebugString(),"pos x:%.3f y:%.3f z:%.3f",pos.x,pos.y,pos.z);
	return 1;
}
void MS_Hangar::clbkSetClassCaps(FILEHANDLE cfg){
	  SetEmptyMass(1000);
	  SetClipRadius(0.01);
	double ro=10;
			   TOUCHDOWNVTX td[4];
 for(int i=0;i<4;i++)
 {
	
	 td[i].damping=3e5;
	 td[i].mu=3;
	 td[i].mu_lng=3;
	 td[i].stiffness=3e5;
 }
 td[0].pos.x=-cos(30*RAD)*ro;
 td[0].pos.y=-sin(30*RAD)*ro;
 td[0].pos.z=0.01;
 td[1].pos.x=0;
 td[1].pos.y=1*ro;
 td[1].pos.z=0.01;
 td[2].pos.x=cos(30*RAD)*ro;
 td[2].pos.y=-sin(30*RAD)*ro;
 td[2].pos.z=0.01;
 td[3].pos.x=0;
 td[3].pos.y=0;
 td[3].pos.z=5*ro;

 
SetTouchdownPoints(td,4);







oapiWriteLog(const_cast<char *>("Creating Hangar"));
MESHHANDLE ms1=oapiLoadMeshGlobal("Multistage2015\\MS_Hangar\\MS_Hangar");

oapiWriteLog(const_cast<char *>("Hangar Mesh Loaded"));
VECTOR3 ofs=_V(0,0,0);
AddMesh(ms1,&ofs);
col_d.a=0;
col_d.b=1;
col_d.g=0.8;
col_d.r=0.9;
col_s.a=0;
col_s.b=1;
col_s.g=0.8;
col_s.r=1.9;
col_a.a=0;
col_a.b=0;
col_a.g=0;
col_a.r=0;
col_white.a=0;
col_white.b=1;
col_white.g=1;
col_white.r=1;
		
		
		PadHangar = CreateAttachment(FALSE,_V(1.3,-10,57.75),_V(0,0,1),_V(0,1,0),"hangar",FALSE);
		VECTOR3 dir;
		dir=_V(-29,51,-183);
		normalise(dir);
		const double att=0.0001;
		Hangarlight[0] = AddSpotLight(_V(29,-51,183),dir,200,att,att,att,45*RAD,60*RAD,col_d,col_s,col_a);
		dir=_V(29,51,-183);
		normalise(dir);
		Hangarlight[1] = AddSpotLight(_V(-29,-51,183),dir,200,att,att,att,45*RAD,60*RAD,col_d,col_s,col_a);
		dir=_V(29,-51,-183);
		normalise(dir);
		Hangarlight[2] = AddSpotLight(_V(-29,51,183),dir,200,att,att,att,45*RAD,60*RAD,col_d,col_s,col_a);
		dir=_V(-29,-51,-183);
		normalise(dir);
		Hangarlight[3] = AddSpotLight(_V(29,51,183),dir,200,att,att,att,45*RAD,60*RAD,col_d,col_s,col_a);

		Hangarlight[4] = AddSpotLight(_V(0,200,0),_V(0,-sin(60*RAD),cos(60*RAD)),200,0.5*att,0.5*att,0.5*att,65*RAD,80*RAD,col_d,col_s,col_a);
		for(int i=0;i<5;i++)
		{
			Hangarlight[i]->Activate(FALSE);
		}
		
	oapiWriteLog(const_cast<char *>("Hangar Lights Created"));

	static UINT HookGrp[2]={574,575};
	static MGROUP_TRANSLATE HookTr_compX (0,HookGrp,2,_V(80,0,0));
	HookTrX=CreateAnimation(0.51625);
	AddAnimationComponent(HookTrX,0,1,&HookTr_compX);
	
	static MGROUP_TRANSLATE HookTr_compZ (0,HookGrp,2,_V(0,0,164));
	HookTrZ = CreateAnimation(0.35213414634146341463414634146341);
	AddAnimationComponent(HookTrZ,0,1,&HookTr_compZ);

	static MGROUP_TRANSLATE HookTr_compY (0,HookGrp,2,_V(0,80,0));
	HookTrY = CreateAnimation(0.375);
	AddAnimationComponent(HookTrY,0,1,&HookTr_compY);

	static UINT CablesGrp[6]={478,479,480,481,482,483};
	static MGROUP_TRANSLATE CablesTr_compX (0,CablesGrp,6,_V(80,0,0));
	CablesTrX = CreateAnimation(0.51625);
	AddAnimationComponent(CablesTrX,0,1,&CablesTr_compX);

	static MGROUP_TRANSLATE CablesTr_compY (0,CablesGrp,6,_V(0,80,0));
	CablesTrY = CreateAnimation(0.375);
	AddAnimationComponent(CablesTrY,0,1,&CablesTr_compY);

	static MGROUP_SCALE CablesTr_compZ (0,CablesGrp,6,_V(1.6,-16.957,179.75),_V(1,1,0.07735583684950773558368495077356));
	CablesTrZ = CreateAnimation(0.35213414634146341463414634146341);
	AddAnimationComponent(CablesTrZ,0,1,&CablesTr_compZ);

	static UINT BridgeGrp[7]={476,477,569,570,571,572,573};
	static MGROUP_TRANSLATE Bridge_compY (0,BridgeGrp,7,_V(0,80,0));
	BridgeTrY = CreateAnimation(0.375);
	AddAnimationComponent(BridgeTrY,0,1,&Bridge_compY);

	static UINT TrailerGrp[3]={476,477,571};
	static MGROUP_TRANSLATE TrailerTr_compX (0,TrailerGrp,3,_V(80,0,0));
	TrailerTrX = CreateAnimation(0.51625);
	AddAnimationComponent(TrailerTrX,0,1,&TrailerTr_compX);
	hangaranims=_V(1.3,-10,57.75);
	SetAttachmentParams(PadHangar,hangaranims,_V(0,0,1),_V(0,1,0));
	UpdateHangarAnimations(hangaranims);

	oapiWriteLog(const_cast<char *>("Hangar Animations Created and Set"));







	return;
}
int MS_Hangar::clbkGeneric(int msgid=0, int prm=0, void* context=NULL)
{
	if(msgid==VMSG_USER)
	{
		if(prm==1)
		{
			
			DWORD* idx=(DWORD*)(context);
			*idx=GetAttachmentIndex(PadHangar);
			
			return 1;//(int)GetAttachmentIndex(PadHangar);
		}
	}
	return 0;
}
void MS_Hangar::clbkLoadStateEx(FILEHANDLE scn,void *vs)
{
	char *line;
	while(oapiReadScenario_nextline(scn,line)){
		ParseScenarioLineEx(line,vs);}
 
}
//############################################################################//
void MS_Hangar::clbkSaveState(FILEHANDLE scn)
{
	SaveDefaultState(scn);
	
}
int MS_Hangar::clbkConsumeBufferedKey(DWORD key,bool down,char *kstate)
{
	if(!down) return 0;

	if(KEYDOWN(kstate,OAPI_KEY_L)&&(KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate))))
	{
				for(int i=0;i<5;i++)
				{
					Hangarlight[i]->Activate(!Hangarlight[i]->IsActive());
				}
		return 1;
		}
	if(KEYDOWN(kstate,OAPI_KEY_D)&&(KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate))))
	{
		DetachChild(PadHangar,0);
	return 1;
	}
	return 0;}
int MS_Hangar::clbkConsumeDirectKey(char *kstate)
{
	
	
		ProcessHangar(kstate);
	
	return 0;
}
/*
void MS_Hangar::clbkSetStateEx (const void *status)
{
	
	return;
}*/
//Creation
DLLCLBK void InitModule(HINSTANCE hModule){}
DLLCLBK void ExitModule(HINSTANCE hModule){}
DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel,int flightmodel){return new MS_Hangar(hvessel,flightmodel);}
DLLCLBK void ovcExit(VESSEL *vessel){if(vessel)delete(MS_Hangar*)vessel;}

