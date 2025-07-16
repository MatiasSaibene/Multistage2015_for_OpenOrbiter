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
// MS_Camera.cpp
// ==============================================================





//############################################################################//
//#define ORBITER_MODULE
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_NONSTDC_NO_DEPRECATE
#include <math.h>
#include <stdio.h>
//#include "orbitersdk.h"


#include "MS_Camera.h"

//Creation
MS_Camera::MS_Camera(OBJHANDLE hObj,int fmodel):VESSEL4(hObj,fmodel){
penwhite=oapiCreatePen(1,3,0xFFFFFF);
	RefVessel[0] = '\0';

	RefHeight = NULL;

	v = nullptr;

	Distance = 0.0;

	Polar = 0.0;

	Azimuth = 0.0;

	Aperture = 0.0;

	hMS = nullptr;

}

MS_Camera::~MS_Camera(){

	oapiReleasePen(penwhite);

}

void MS_Camera::clbkSetClassCaps(FILEHANDLE cfg){
	
	  SetEmptyMass(1000);
	  SetClipRadius(0.01);
	  RefHeight=50;
	  for(int i=0;i<256;i++)
	  {
		  RefVessel[i]='\0';
	  }
	  SetCameraDefaultDirection(_V(0,0,1));
	  SetCameraRotationRange(2*PI,2*PI,2*PI,2*PI);
	
	  double ro=20;
	
	SetTouchdownPoints(_V(cos(30*RAD)*ro,-20,-sin(30*RAD)*ro),_V(0,-20,1*ro),_V(-cos(30*RAD)*ro,-20,-sin(30*RAD)*ro));
	Distance=0;
	Polar=0;
	Azimuth=0;
	Aperture=0;
	//FirstLoop=TRUE;
	//TargetLock=TRUE;
	return;
}
bool MS_Camera::clbkLoadGenericCockpit()
{
/*	if(FirstLoop){
		FirstLoop=FALSE;  
		return TRUE;
	}else{
		oapiSetHUDMode(HUD_SURFACE);
	//	FirstLoop=TRUE;
		return FALSE;
	}
	*/
	
	return FALSE;
}

bool MS_Camera::clbkLoadPanel2D(int id,PANELHANDLE hPanel,DWORD viewW,DWORD viewH)
{
	
	return TRUE;
}
void MS_Camera::clbkLoadStateEx(FILEHANDLE scn,void *vs)
{
	
	char *line;
	while(oapiReadScenario_nextline(scn,line)){
		 if(!_strnicmp(line,"TARGET",6)){
			 sscanf_s(line + 6, "%255s", RefVessel);
			 char logbuff[256];
			 
		 sprintf(logbuff,"MS_Camera: Target Vessel Loaded->%s",RefVessel);
		 oapiWriteLog(logbuff);
		 }
		 else{
		ParseScenarioLineEx(line,vs);}
	}
}
//############################################################################//
void MS_Camera::clbkSaveState(FILEHANDLE scn)
{
	char savebuff[256],savevalbuff[256];
	SaveDefaultState(scn);
	
	sprintf(savebuff,"TARGET");
	sprintf(savevalbuff,"%s", RefVessel);
	oapiWriteScenario_string(scn,savebuff,savevalbuff);
	
}
int MS_Camera::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
		if(!down) return 0;

	if(KEYDOWN(kstate,OAPI_KEY_UP)&&(!KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate))))
	{
		RefHeight-=50;
		if(RefHeight<50){RefHeight=50;}
	return 1;
	}
	if(KEYDOWN(kstate,OAPI_KEY_DOWN)&&(!KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate))))
	{
		RefHeight+=50;
		if(RefHeight>200){RefHeight=200;}
	return 1;
	}
	if(KEYDOWN(kstate,OAPI_KEY_R)&&(KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate))))
	{
		char ch_referenceVsl[] = "Reference Vessel";
		oapiOpenInputBox(ch_referenceVsl,InputNewRefVessel,0,35,(void*)this);
	return 1;
	}
	return 0;
}
void MS_Camera::clbkPreStep(double simt, double simdt, double mjd)
{
	
	return;
}

void MS_Camera::clbkPostStep(double simt, double simdt, double mjd)
{
	
	if(Distance>10e6){oapiDeleteVessel(GetHandle(),oapiGetVesselByName(RefVessel));}
	//if(FirstLoop){FirstLoop=FALSE;}

	if(oapiGetFocusObject()==GetHandle())
	{
	oapiCameraAttach(GetHandle(),0);

	hMS=oapiGetVesselByName(RefVessel);
	if(oapiIsVessel(hMS)){
		v=(VESSEL4*)oapiGetVesselInterface(hMS);
		
	VECTOR3 rpos,gpos;
	GetRelativePos(hMS,rpos);
	GetGlobalPos(gpos);
	Distance=length(rpos);
	Aperture=2*atan2(RefHeight,Distance);
	if(Aperture<=0.1*RAD){Aperture=0.1*RAD;}
	oapiCameraSetAperture(Aperture);

	VECTOR3 rposloc;
	Global2Local((gpos-rpos),rposloc);
	normalise(rposloc);
	Polar=atan2(-rposloc.x,rposloc.z);
	Azimuth=acos(sqrt(rposloc.x*rposloc.x+rposloc.z*rposloc.z));
	oapiCameraSetCockpitDir(Polar,Azimuth,FALSE);
	//oapiSetHUDMode(HUD_DOCKING);
	}else{return;}
	}
	return;
}
int MS_Camera::clbkGeneric(int msgid=0, int prm=0, void *context=NULL)
{
	if(msgid==VMSG_USER)
	{
		if(prm==1)
		{
		sprintf(RefVessel,"%s", (char*)context);
		return 1;
		}
		return 0;
	}
	return 0;
}
bool MS_Camera::InputNewRefVessel(void *id, char *str, void *usrdata)
{
	MS_Camera* Cam=(MS_Camera*)(usrdata);
	sprintf_s(Cam->RefVessel,str);
	if(oapiIsVessel(oapiGetVesselByName(str)))
	{
		return TRUE;
	}else{
		return FALSE;
	}
}
void MS_Camera::clbkRenderHUD(int mode, const HUDPAINTSPEC *hps, SURFHANDLE hDefaultTex)
{
	
	return;
}
bool MS_Camera::clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp)
{
	
	if(mode!=HUD_NONE)
	{
	//VESSEL3::clbkDrawHUD (mode, hps, skp);
	int cx = hps->CX, cy = hps->CY;
	DWORD w;
	DWORD h;
	DWORD bpp;
	oapiGetViewportSize(&w,&h,&bpp);
	int ww=(int)w;
	int hh=(int)h;
	
	skp->SetPen(penwhite);
	skp->Line((0.5*ww),(0.1*hh),(0.5*ww),(0.45*hh));
	skp->Line((0.5*ww),(0.55*hh),(0.5*ww),(0.9*hh));
	skp->Line((0.5*ww),(0.49*hh),(0.5*ww),(0.51*hh));
	skp->Line((0.1*ww),(0.5*hh),(0.45*ww),(0.5*hh));
	skp->Line((0.55*ww),(0.5*hh),(0.9*ww),(0.5*hh));
	skp->Line((0.49*ww),(0.5*hh),(0.51*ww),(0.5*hh));
	
	//skp->Rectangle(0.1*ww,0.1*hh,0.35*ww,0.1*hh+60);
	char hudbuff[256];
	skp->SetTextColor(0xFFFFFF);
	int len=0;
	if(oapiIsVessel(hMS))
	{
	len=sprintf_s(hudbuff,"Altitude	: %03.3f km",v->GetAltitude()/1000);
	skp->Text(0.1*ww,0.1*hh,hudbuff,len);
	len=sprintf_s(hudbuff,"Distance	: %03.3f km",Distance/1000);
	skp->Text(0.1*ww,0.1*hh+20,hudbuff,len);
	len=sprintf_s(hudbuff,"Velocity	: %04.0f m/s",v->GetGroundspeed());
	skp->Text(0.1*ww,0.1*hh+40,hudbuff,len);

	len=sprintf_s(hudbuff,"Polar   : %03.1f °",Polar*DEG);
	skp->Text(0.1*ww,0.1*hh+80,hudbuff,len);
	len=sprintf_s(hudbuff,"Azimuth : %03.1f °",Azimuth*DEG);
	skp->Text(0.1*ww,0.1*hh+100,hudbuff,len);
	len=sprintf_s(hudbuff,"Aperture: %03.2f °",Aperture*DEG);
	skp->Text(0.1*ww,0.1*hh+120,hudbuff,len);
	double Met;
	v->clbkGeneric(VMSG_USER,1,&Met);
	len=sprintf_s(hudbuff,"MET: %03.2f s",Met);
	skp->Text(0.1*ww,0.1*hh+160,hudbuff,len);

/*
	VECTOR3 Targets;
	v->clbkGeneric(VMSG_USER,3,&Targets);
	len=sprintf_s(hudbuff,"Tgtapo: %03.2f Tgtperi %03.2f TgtInc: %03.2f",Targets.x,Targets.y,Targets.z*DEG);
	skp->Text(0.1*ww,0.1*hh+180,hudbuff,len);
	*/
	}

	}
	return TRUE;
}
/*
void MS_Camera::clbkSetStateEx (const void *status)
{
	
	return;
}*/
//Creation
DLLCLBK void InitModule(HINSTANCE hModule){}
DLLCLBK void ExitModule(HINSTANCE hModule){}
DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel,int flightmodel){return new MS_Camera(hvessel,flightmodel);}
DLLCLBK void ovcExit(VESSEL *vessel){if(vessel)delete(MS_Camera*)vessel;}

