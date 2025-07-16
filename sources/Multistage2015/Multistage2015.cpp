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
// Multistage2015.cpp
// ==============================================================





//############################################################################//
#include <cstddef>
#include <windows.h>
#define ORBITER_MODULE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <math.h>
#include <stdio.h>
#include "..//..//Orbitersdk/include/Orbitersdk.h"
//#include "windows.h"
//#include "DlgCtrl.h"
#include "Multistage2015.h"
//#include "resource.h"
#include "DevModeCtrl.h"



HWND hDlg;
//Multistage2015 *Ms15;


typedef struct {
	HINSTANCE hDLL;
} GDIParams;
GDIParams g_Param;


//BOOL CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM);


void OpenDlgClbk (void *context);
//BOOL CALLBACK MsgProc (HWND, UINT, WPARAM, LPARAM);
void CloseDlg (HWND hDlg)
{
	oapiCloseDialog (hDlg);
}






//Creation
Multistage2015::Multistage2015(OBJHANDLE hObj,int fmodel):VESSEL4(hObj,fmodel){

	DeveloperMode=FALSE;
	//Ms15=this;
	HangarMode=FALSE;

	DMD = nullptr;

	MyID = 0;

	thg_h_main = nullptr;

	particlesdt = 0.0;

	GrowingParticles = false;

	RefPressure = 0.0;

	for(int i = 0; i < 150; i++){
		Gnc_step[i] = {0};
	}

	TotalHeight = 0.0;

	dataparsed[0] = '\0';

	Configuration = 0;

	CogElev = 0.0;

	OrbiterRoot[0] = '\0';

	fileini[0] = '\0';

	guidancefile[0] = '\0';

	nStages = 0;

	nBoosters = 0;

	nInterstages = 0;

	nPayloads = 0;

	nTextures = 0;

	nParticles = 0;

	currentStage = 0;

	currentBooster = 0;

	currentPayload = 0;

	hasFairing = false;

	wLes = false;

	wMach = false;

	wVent = false;

	wLaunchFX = false;

	wFairing = false;

	tex = {0};

	currentDelta = 0.0;

	nPsh = 0;

	nPsg = 0;

	psg = {0};

	Ssound = {0};
	
	currentInterstage = 0;

	wBoosters = 0;

	wAdapter = 0;

	logbuff[0] = '\0';

	buffreset[0] = '\0';

	MET = 0.0;

	stage_ignition_time = 0.0;

	for(int i = 0; i < 10; i++){
		live_a[i] = nullptr;
	}

	lvl = 0.0;

	nsteps = 0;

	perror = 0.0;

	yerror = 0.0;

	rerror = 0.0;

	pintegral = 0.0;

	kp = 0.0;

	kd = 0.0;

	ki = 0.0;

	Tu = 0.0;

	Ku = 0.0;

	VinkaAzimuth = 0.0;

	VinkaMode = 0.0;

	initlong = 0.0;

	initlat = 0.0;

	APstat = false;

	AJdisabled = false;

	rolldisabled = false;

	pitchdisabled = false;

	Gnc_running = false;

	spinning = false;

	AttCtrl = false;

	PitchCtrl = false;

	YawCtrl = false;

	RollCtrl = false;

	DeltaUpdate = 0.0;

	wPeg = false;

	TgtPitch = 0.0;

	for(int i = 0; i < 4; i++){
		altsteps[i] = 0.0;
	}

	stepsloaded = 0.0;

	Complex = false;

	UpdateComplex = 0.0;

	GT_InitPitch = 0.0;

	GT_IP_Calculated = 0.0;

	runningPeg = false;

	op = {0};

	el = {0};

	epsfin = 0.0;

	eps = 0.0;

	coc = {0};

	MECO_TEST = 0.0;

	for(int i = 0; i < 100; i++){
		coeff[i] = 0.0;
	}

	avgcoeff = 0.0;

	MECO_Counter = 0.0;

	TMeco = 0.0;

	PegPitchLimit = 0.0;

	for(int i = 0; i < 10; i++){
		r_T[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		rdot_T[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		T[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		DeltaA[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		DeltaB[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		A[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		B[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		delta_rdot[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		delta_r[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		r_in[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		rdot_in[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		VthetaAtStaging[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		OmegaS[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		a_fin[i] = 0.0;
	}

	tgtapo = 0.0;

	tgtperi = 0.0;

	tgtinc = 0.0;

	tgtabside = 0.0;

	tgtvv = 0.0;

	Azimuth = 0.0;

	mu = 0.0;

	rt = 0.0;

	g0 = 0.0;

	r = 0.0;

	v = 0.0;

	z = 0.0;

	h = 0.0;

	thrust = 0.0;

	mass = 0.0;

	for(int i = 0; i < 10; i++){
		a_[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		v_e[i] = 0.0;
	}

	for(int i = 0; i < 10; i++){
		tau_[i] = 0.0;
	}

	omega = 0.0;

	rdot = 0.0;

	vtheta = 0.0;

	VertVel = 0.0;

	NN = 0;

	PegMajorCycleInterval = 0.0;

	g = 0.0;

	cent = 0.0;

	g_term = 0.0;

	cent_term = 0.0;

	J = 0;

	UpdatePegTimer = 0.0;

	PegDesPitch = 0.0;

	failureProbability = 0;

	timeOfFailure = 0;

	wFailures = false;

	failed = false;

	tlmfile[0] = '\0';

	for(int i = 0; i < TLMSECS; i++){
		tlmAlt[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		tlmSpeed[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		tlmPitch[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		tlmThrust[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		tlmMass[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		tlmVv[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		tlmAcc[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		ReftlmAlt[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		ReftlmSpeed[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		ReftlmPitch[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		ReftlmThrust[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		ReftlmMass[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		ReftlmVv[i] = {0};
	}

	for(int i = 0; i < TLMSECS; i++){
		ReftlmAcc[i] = {0};
	}

	updtlm = 0.0;

	writetlmTimer = 0.0;

	tlmidx = 0;

	tlmnlines = 0;

	loadedtlmlines = 0;

	updtboiloff = 0.0;

	wReftlm = false;

	scenario = nullptr;

	config = nullptr;

	loadedCurrentBooster = 0;

	loadedCurrentInterstage = 0;

	loadedCurrentStage = 0;

	loadedCurrentPayload = 0;

	loadedwFairing = 0;

	loadedConfiguration = 0;

	loadedGrowing = false;

	loadedMET = 0.0;

	vs2 = {0};

	killDMD = false;

	note = nullptr;

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 5; j++){
			payloadrotatex[i][j] = nullptr;
		}
	}

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 5; j++){
			payloadrotatey[i][j] = nullptr;
		}
	}

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 5; j++){
			payloadrotatez[i][j] = nullptr;
		}
	}

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 5; j++){
			anim_x[i][j] = nullptr;
		}
	}

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 5; j++){
			anim_y[i][j] = nullptr;
		}
	}

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 5; j++){
			anim_z[i][j] = nullptr;
		}
	}

	hramp = nullptr;

	hhangar = nullptr;

	hCrawler = nullptr;

	vCrawler = nullptr;

	vramp = nullptr;

	vhangar = nullptr;

	vshangar = {0};

	AttToRamp = nullptr;

	padramp = nullptr;

	AttToHangar = nullptr;

	PadHangar = nullptr;

	AttToCrawler = nullptr;

	wRamp = false;

	NoMoreRamp = false;

	col_d = {0};

	col_s = {0};

	col_a = {0};

	col_white = {0};

	th_main_level = 0.0;

	launchFx_level = 0.0;

	wCrawler = false;

	wCamera = false;

	CamDLat = 0.0;

	CamDLng = 0.0;

	AttToMSPad = false;

	vsramp = {0};

	MaxTorque = _V(0, 0, 0);

	MsPadZ = _V(0, 0, 0);

	RotMatrix = { 0 };

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			RotatePayloadAnim_x[i][j] = 0;
			RotatePayloadAnim_y[i][j] = 0;
			RotatePayloadAnim_z[i][j] = 0;
		}
	}

	ShipSpeed = _V(0, 0, 0);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 32; j++) {
			exhaustN[i][j] = 0;
		}
	}

	fhat = _V(0, 0, 0);

	hangaranims = _V(0, 0, 0);

	hhat = _V(0, 0, 0);

	hvec = _V(0, 0, 0);

	rhat = _V(0, 0, 0);

	rvec = _V(0, 0, 0);

	thetahat = _V(0, 0, 0);

	vvec = _V(0, 0, 0);

}

Multistage2015::~Multistage2015(){
	if(psg)delete []psg;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<5;j++)
		{
			delete payloadrotatex[i][j];
			delete payloadrotatey[i][j];
			delete payloadrotatez[i][j];
		}
	}


	//delete Ms15;
}


VECTOR4F Multistage2015::_V4(double x, double y, double z, double t){
VECTOR4F v4;
v4.x=x;
v4.y=y;
v4.z=z;
v4.t=t;
return v4;
}

VECTOR2 Multistage2015::_V2(double x, double y)
{
	VECTOR2 v2;
	v2.x=x;
	v2.y=y;
	return v2;
}
//returns 1 if X is positive or -1 if X is negative
int Multistage2015::SignOf(double X)
{
	return X/abs(X);
}

//returns true if a number is odd

bool Multistage2015::IsOdd( int integer )
{
	if ( integer % 2== 0 )
    	return true;
	else
    	return false;
}
//transforms char variable in v3
VECTOR3 Multistage2015::CharToVec(char charvar[MAXLEN],VECTOR3* outvec){

	double dbuff[3];
	char *cbuff;
	cbuff=NULL;
	cbuff=strtok(charvar,",");
	int k=0;
	 while (cbuff != NULL)
		{
			int cbs;
			for(cbs=0;cbs<sizeof(cbuff);cbs++){
				if(cbuff[cbs]=='(') cbuff[cbs]=' ';
				else if(cbuff[cbs]==')') cbuff[cbs]=' ';
							}
			
			
		dbuff[k]=atof(cbuff);
				
		switch(k){
			case 0:
			outvec->x=dbuff[k];
			break;
			case 1:
			outvec->y=dbuff[k];
			break;
			case 2:
			outvec->z=dbuff[k];
			break;
		}
		k+=1;
		if(k>2)k=0;

		cbuff = strtok (NULL, ",");
		
		}
	return *outvec;
}

//transforms a char variable in V4
VECTOR4F Multistage2015::CharToVec4(char charvar[MAXLEN],VECTOR4F* outvec){

	double dbuff[4];
	char *cbuff;
	cbuff=NULL;
	cbuff=strtok(charvar,",");
	int k=0;
	 while (cbuff != NULL)
		{
			int cbs;
			for(cbs=0;cbs<sizeof(cbuff);cbs++){
				if(cbuff[cbs]=='(') cbuff[cbs]=' ';
				else if(cbuff[cbs]==')') cbuff[cbs]=' ';
							}
			
			
		dbuff[k]=atof(cbuff);
				
		switch(k){
			case 0:
			outvec->x=dbuff[k];
			break;
			case 1:
			outvec->y=dbuff[k];
			break;
			case 2:
			outvec->z=dbuff[k];
			break;
			case 3:
			outvec->t=dbuff[k];
			break;
		}
		k+=1;
		if(k>3)k=0;

		cbuff = strtok (NULL, ",");
		
		}
	return *outvec;
}
//Function to Rotate a Vector3 around Z axis of a given Angle
 VECTOR3 Multistage2015::RotateVecZ(VECTOR3 input, double Angle){
	 VECTOR3 output;
	  	output=_V(input.x*cos(Angle)-input.y*sin(Angle),input.x*sin(Angle)+input.y*cos(Angle),input.z);
	 return output;
 }

 MATRIX3 Multistage2015::RotationMatrix(VECTOR3 angles)
 {MATRIX3 m;
 	MATRIX3 RM_X,RM_Y,RM_Z;
	RM_X=_M(1,0,0,0,cos(angles.x),-sin(angles.x),0,sin(angles.x),cos(angles.x));
	RM_Y=_M(cos(angles.y),0,sin(angles.y),0,1,0,-sin(angles.y),0,cos(angles.y));
	RM_Z=_M(cos(angles.z),-sin(angles.z),0,sin(angles.z),cos(angles.z),0,0,0,1);
	m=mul(RM_Z,mul(RM_Y,RM_X));
 return m;
 }


void Multistage2015::ResetVehicle( VECTOR3 hangaranimsV = _V(1.3,-10,57.75),bool Ramp=FALSE)
 {

	// 	 if(HangarMode)
	 //{
		// SetAttachmentParams(PadHangar,_V(0,0,100),_V(0,0,1),_V(0,1,0));
	 //}
 
	 ClearMeshes();
	 ClearThrusterDefinitions();
	 ClearPropellantResources();
	 ClearAttachments();

//	 	if((RampCreated)&&(!RampDeleted))
	

	 initGlobalVars();
	 wRamp=Ramp;
	 if(wRamp)
			{
				oapiDeleteVessel(hramp);
				wRamp=FALSE;
			}

 
char tempFile[MAXLEN];
 strcpy_s(tempFile,OrbiterRoot);
 strcat_s(tempFile,"\\");
 strcat_s(tempFile,fileini);
 sprintf(logbuff,"%s: Config File: %s",GetName(),tempFile);
 oapiWriteLog(logbuff);
 parseinifile(tempFile);

 currentBooster = loadedCurrentBooster;
currentInterstage = loadedCurrentInterstage;
currentStage = loadedCurrentStage;
currentPayload = loadedCurrentPayload;
wFairing = loadedwFairing;
Configuration = loadedConfiguration;
GrowingParticles=loadedGrowing;

 MET = loadedMET;
if((currentInterstage>currentStage)||(currentInterstage>nInterstages)||(currentInterstage>=stage[currentStage].IntIncremental)){stage[currentStage].wInter=FALSE; }
 
 if((wFairing==1)&&(hasFairing==FALSE)){wFairing=0;}
 if(Configuration==0){			//if only configuration is defined, reset everything
	 if(hasFairing==TRUE){
		  wFairing=1;}
	 currentStage=0;
	 currentPayload=0;
	 currentBooster=0;
	 currentInterstage=0;
 }

 parseGuidanceFile(guidancefile);
 UpdateOffsets();

 VehicleSetup();
 LoadMeshes();

 //vs2.arot=_V(0,0,0);
 vs2.rvel=_V(0,0,0);
 
  clbkSetStateEx(&vs2);

  hangaranims=hangaranimsV;
 
 clbkPostCreation();
 
 
	 return;
 }


 //Returns current Heading
 double Multistage2015::GetHeading()
 {double Heading;
 oapiGetHeading(GetHandle(),&Heading);
 return Heading;
 }
 //returns Vehicle Orbital Speed
 double Multistage2015::GetOS(){
	OBJHANDLE hearth=GetSurfaceRef();
	VECTOR3 relvel;
	GetRelativeVel(hearth,relvel);
	double os=length(relvel);
	return os;
}
 //returns Vechicle Mass at a certain stage
 double Multistage2015::GetMassAtStage(int MassStage,bool empty=TRUE){
	 double Mass=0;

	 int add=0;
		 if(empty==TRUE){add=1;}else{add=0;}

	 for(int i=MassStage;i<nStages;i++){
		 Mass+=stage[i].emptymass;
	 }
	 for(int j=MassStage+add;j<nStages;j++){
	 Mass+=stage[j].fuelmass;
	 }
	 for(int k=0;k<nPayloads;k++){
		 Mass+=payload[k].mass;
	 }
	 if(hasFairing){
	 Mass+=fairing.emptymass;
	 }
	 if(wAdapter){
	 Mass+=Adapter.emptymass;
	 }
	 if(wLes){
	 Mass+=Les.emptymass;
	 }

	 return Mass;
 }
 
 //Returns Remainign Stage DV
 double Multistage2015::StageDv(int dvstage){
	 double sdv;
	  sdv=stage[dvstage].isp*log(GetMassAtStage(dvstage,FALSE)/GetMassAtStage(dvstage,TRUE));
	 return sdv;
 }

  //Returns Remaining DV at a certain Stage
 double Multistage2015::DvAtStage(int dvatstage){
	 double rdvas=0;
	 for(int i=dvatstage;i<nStages;i++){
		 rdvas+=StageDv(i);
	 }
	 return rdvas;
 }

 //Returns Current Stage Remaining DV
 double Multistage2015::CurrentStageRemDv(){
	 double csrd;
	 csrd=stage[currentStage].isp*log((GetMassAtStage(currentStage,TRUE)+GetPropellantMass(stage[currentStage].tank))/GetMassAtStage(currentStage,TRUE));
	 return csrd;
 }

 //Returns Any Stage Rmeaining DV
 double Multistage2015::StageRemDv(int dvstage)
 {
	 double srd;
	 srd=stage[dvstage].isp*log((GetMassAtStage(dvstage,TRUE)+GetPropellantMass(stage[dvstage].tank))/GetMassAtStage(dvstage,TRUE));
	 return srd;
 }

 //Returns Remaining Stage Burn Time
 double Multistage2015::RemBurnTime(int rbtstage, double level=1){
	 double BT;
	 	 BT=stage[rbtstage].isp*GetPropellantMass(stage[rbtstage].tank)/(stage[rbtstage].thrust*level);
	 return BT;
 }
 //Returns Remaining Boosters Group Burn Time
 double Multistage2015::BoosterRemBurnTime(int rbtbooster, double level=1){
	 double BT;
	 	 BT=booster[rbtbooster].isp*GetPropellantMass(booster[rbtbooster].tank)/((booster[rbtbooster].thrust*booster[rbtbooster].N)*level);
	 return BT;
 }


 //Global Variables initialization
void Multistage2015::initGlobalVars(){
	nStages=0;
	nBoosters=0;
	nInterstages=0;
	nTextures=0;
	nParticles=0;
	currentStage=0;
	currentBooster=0;
	currentInterstage=0;
	CogElev=0;
	Misc.VerticalAngle=0;
	Misc.drag_factor=1;
	GetCurrentDirectory(MAXLEN,OrbiterRoot); //initialize orbiter root with the current directory
	wBoosters=FALSE;
	wFairing=0;
	wLes=FALSE;
	wAdapter=FALSE;
	wMach=FALSE;
	wVent=FALSE;
	wLaunchFX=FALSE;
	Complex=FALSE;
	stage_ignition_time=0;

	currentDelta=0;
	perror=0;
	yerror=0;
	rerror=0;



	int i;
	for(i=0;i<10;i++){
		stage[i]=STAGE();
	stage[i].Ignited=FALSE;
	stage[i].reignitable=TRUE;
	stage[i].batteries.wBatts=FALSE;
	stage[i].waitforreignition=0;
	stage[i].StageState=STAGE_SHUTDOWN;
	stage[i].DenyIgnition=FALSE;
	stage[i].ParticlesPacked=FALSE;
	stage[i].ParticlesPackedToEngine=0;
	stage[i].defpitch=FALSE;
	stage[i].defroll=FALSE;
	stage[i].defyaw=FALSE;
	payload[i]=PAYLOAD();
	
	}

	int ii;
	for(ii=0;ii<10;ii++){
	booster[ii]=BOOSTER();
	booster[ii].Ignited=FALSE;
	booster[ii].ParticlesPacked=FALSE;
	booster[ii].ParticlesPackedToEngine=0;
	
	}
	MET=0;
	APstat=FALSE;
	AJdisabled=FALSE;
	rolldisabled=FALSE;
	pitchdisabled=FALSE;

	Gnc_running=0;
	spinning=FALSE;
	lvl=1;
	for(int s=0;s<57;s++){
		Ssound.GncStepSound[s]=-5;
	}


PegDesPitch=35*RAD;
PegPitchLimit=35*RAD;
DeltaUpdate=0;
GT_InitPitch=89.5*RAD;
UpdatePegTimer=0;
UpdateComplex=0;
wPeg=FALSE;
PegMajorCycleInterval=0.1;
runningPeg=FALSE;
//g0=9.80655;
AttCtrl=TRUE;
 PitchCtrl=TRUE;
 YawCtrl=TRUE;
 RollCtrl=TRUE;
TgtPitch=0;
//tgtapo=200000;
//tgtperi=200000;
eps=-9000000000000;

failureProbability=-1000;
timeOfFailure=-10000000;
wFailures=FALSE;
failed=FALSE;

DMD=0;
killDMD=FALSE;
stepsloaded=FALSE;

for(int q=0;q<150;q++)
{
	Gnc_step[q].GNC_Comand=CM_NOLINE;
}


updtlm=0;
tlmidx=0;
writetlmTimer=0;
tlmnlines=0;
wReftlm=FALSE;
for(int q=0;q<TLMSECS;q++)
{
	tlmAlt[q].x=0;
	tlmSpeed[q].x=0;
	tlmPitch[q].x=0;
	tlmThrust[q].x=0;
	tlmMass[q].x=0;
	tlmVv[q].x=0;
	tlmAcc[q].x=0;

	tlmAlt[q].y=0;
	tlmSpeed[q].y=0;
	tlmPitch[q].y=0;
	tlmThrust[q].y=0;
	tlmMass[q].y=0;
	tlmVv[q].y=0;
	tlmAcc[q].y=0;
}

updtboiloff=0;
for(int h=0;h<100;h++)
{coeff[h]=0;}
avgcoeff=0;
MECO_Counter=0;

MECO_TEST=0;
TMeco=600;
nPsg=0;
particlesdt=0;
GrowingParticles=FALSE;
//ParticleFirstLoop=TRUE;
RefPressure=101400;

//RampCreated=FALSE;
//AttachedToRamp=FALSE;
//RampDeleted=FALSE;
wRamp=FALSE;
NoMoreRamp=FALSE;
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

 th_main_level=0;
 launchFx_level=0;
 hangaranims=_V(1.3,-10,57.75);
 wCrawler=FALSE;
 wCamera=FALSE;
 AttToMSPad=FALSE;
 MsPadZ=_V(0,0,-50);
return;
}

PSTREAM_HANDLE Multistage2015::AddExhaustStreamGrowing(THRUSTER_HANDLE  th,    const VECTOR3 &  pos,    PARTICLESTREAMSPEC *  pss = 0  , bool growing=FALSE, double growfactor_size=0, double growfactor_rate=0, bool count=TRUE, bool ToBooster=FALSE,int N_Item=0, int N_Engine=0) 
{
	
	PSTREAM_HANDLE psh=AddExhaustStream(th,pos,pss);
	
		psg[nPsg].pss=*pss;
		psg[nPsg].psh[2]=psh;
		psg[nPsg].th=th;
		//psg[nPsg].psh[0]=psh;
		psg[nPsg].pos=pos;
		psg[nPsg].GrowFactor_rate=growfactor_rate;
		psg[nPsg].GrowFactor_size=growfactor_size;
		psg[nPsg].growing=growing;
		psg[nPsg].baserate=psg[nPsg].pss.growthrate;
		psg[nPsg].basesize=psg[nPsg].pss.srcsize;
		psg[nPsg].basepos=psg[nPsg].pos;
		psg[nPsg].ToBooster=ToBooster;
		psg[nPsg].nItem=N_Item;
		psg[nPsg].nEngine=N_Engine;
	/*	sprintf(logbuff,"nPsg:%i nItem:%i nEngine:%i",nPsg,psg[nPsg].nItem,psg[nPsg].nEngine);
		oapiWriteLog(logbuff);*/
		if(count){	nPsg++;}
		
		return psh;
}
//Create RCS 
void Multistage2015::CreateRCS(){
	if(stage[currentStage].pitchthrust==0){
	 stage[currentStage].pitchthrust=0.25*stage[currentStage].thrust*stage[currentStage].height;//Empirical Values
 }
 if(stage[currentStage].yawthrust==0){
	 stage[currentStage].yawthrust=0.25*stage[currentStage].thrust*stage[currentStage].height;//Empirical Values
 }
 if(stage[currentStage].rollthrust==0){
	 stage[currentStage].rollthrust=0.1*stage[currentStage].thrust*stage[currentStage].diameter*0.5*0.5; //Empirical Values
 }
	//pitch up
 stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,1),_V(0,1,0),2*stage[currentStage].pitchthrust,stage[currentStage].tank,stage[currentStage].isp*100);
 stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,-1),_V(0,-1,0),2*stage[currentStage].pitchthrust,stage[currentStage].tank,stage[currentStage].isp*100);
 CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_PITCHUP);

 MaxTorque.x=2*GetThrusterMax(stage[currentStage].th_att_h[0]);

	//pitch down
	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,1),_V(0,-1,0),2*stage[currentStage].pitchthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,-1),_V(0,1,0),2*stage[currentStage].pitchthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_PITCHDOWN);

	//yaw left
	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,1),_V(-1,0,0),2*stage[currentStage].yawthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,-1),_V(1,0,0),2*stage[currentStage].yawthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_YAWLEFT);

MaxTorque.y=2*GetThrusterMax(stage[currentStage].th_att_h[0]);
	//yaw right
	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,1),_V(1,0,0),2*stage[currentStage].yawthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,-1),_V(-1,0,0),2*stage[currentStage].yawthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_YAWRIGHT);

	//roll left
	stage[currentStage].th_att_h[0]=CreateThruster(_V(1,0,0),_V(0,1,0),2*stage[currentStage].rollthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(-1,0,0),_V(0,-1,0),2*stage[currentStage].rollthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_BANKLEFT);

MaxTorque.z=2*GetThrusterMax(stage[currentStage].th_att_h[0]);
	//roll right
	stage[currentStage].th_att_h[0]=CreateThruster(_V(1,0,0),_V(0,-1,0),2*stage[currentStage].rollthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(-1,0,0),_V(0,1,0),2*stage[currentStage].rollthrust,stage[currentStage].tank,stage[currentStage].isp*100);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_BANKRIGHT);
	if(stage[currentStage].linearthrust>0){

		if(stage[currentStage].linearisp<=0){stage[currentStage].linearisp=stage[currentStage].isp*100;}

	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,0),_V(0,0,1),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,0),_V(0,0,1),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_FORWARD);
	
	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,0),_V(0,0,-1),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,0),_V(0,0,-1),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_BACK);
	
	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,0),_V(-1,0,0),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,0),_V(-1,0,0),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_LEFT);
	
	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,0),_V(1,0,0),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,0),_V(1,0,0),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_RIGHT);
		
	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,0),_V(0,1,0),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,0),_V(0,1,0),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_UP);
		
	stage[currentStage].th_att_h[0]=CreateThruster(_V(0,0,0),_V(0,-1,0),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	stage[currentStage].th_att_h[1]=CreateThruster(_V(0,0,0),_V(0,-1,0),stage[currentStage].linearthrust*0.5,stage[currentStage].tank,stage[currentStage].linearisp);
	CreateThrusterGroup(stage[currentStage].th_att_h,2,THGROUP_ATT_DOWN);
	}
	return;
}

//Creates Main Thrusters and relevant Exhausts
void Multistage2015::CreateMainThruster(){

 if(stage[currentStage].nEngines==0){    //if no stage engines are defined there will be anyway one displayed
	 stage[currentStage].nEngines=1;
	 stage[currentStage].eng[0].x=0;
	 stage[currentStage].eng[0].y=0;
	 stage[currentStage].eng[0].z=-stage[currentStage].height*0.5;

 }


 
//added by rcraig42 to handle ispref in ini but no pressure, to default to earth -------------------------

 if((stage[currentStage].ispref >= 0) && (stage[currentStage].pref==0)){
		stage[currentStage].pref=101400.0;
	}

//-------------------------------------------------------------------------------------------------------	
 
 //modded by rcraig42 to add ispref and pref to createthruster -------------------------------------------------
	
	if(Misc.thrustrealpos){ 
		for(int i=0;i<stage[currentStage].nEngines;i++){
			stage[currentStage].th_main_h[i]=CreateThruster(stage[currentStage].off,stage[currentStage].eng_dir,stage[currentStage].thrust/stage[currentStage].nEngines,stage[currentStage].tank,stage[currentStage].isp,stage[currentStage].ispref,stage[currentStage].pref);
		}
		//stage[currentStage].th_main_h[0]=CreateThruster(stage[currentStage].off,stage[currentStage].eng_dir,stage[currentStage].thrust,stage[currentStage].tank,stage[currentStage].isp);
	}else{
		for(int i=0;i<stage[currentStage].nEngines;i++){
			stage[currentStage].th_main_h[i]=CreateThruster(_V(0,0,0),_V(0,0,1),stage[currentStage].thrust/stage[currentStage].nEngines,stage[currentStage].tank,stage[currentStage].isp,stage[currentStage].ispref,stage[currentStage].pref);
				}
	}
	
//-------------------------------------------------------------------------------------------------------	

	







	/*
	if(Misc.thrustrealpos){ 
		for(int i=0;i<stage[currentStage].nEngines;i++){
			stage[currentStage].th_main_h[i]=CreateThruster(stage[currentStage].off,stage[currentStage].eng_dir,stage[currentStage].thrust/stage[currentStage].nEngines,stage[currentStage].tank,stage[currentStage].isp);
		}
		//stage[currentStage].th_main_h[0]=CreateThruster(stage[currentStage].off,stage[currentStage].eng_dir,stage[currentStage].thrust,stage[currentStage].tank,stage[currentStage].isp);
	}else{
		for(int i=0;i<stage[currentStage].nEngines;i++){
			stage[currentStage].th_main_h[i]=CreateThruster(_V(0,0,0),_V(0,0,1),stage[currentStage].thrust/stage[currentStage].nEngines,stage[currentStage].tank,stage[currentStage].isp);
				}
	}*/
		//stage[currentStage].th_main_h[0]=CreateThruster(_V(0,0,0),_V(0,0,1),stage[currentStage].thrust,stage[currentStage].tank,stage[currentStage].isp);}
 //thg_h_main=CreateThrusterGroup(stage[currentStage].th_main_h,1,THGROUP_MAIN);
	thg_h_main=CreateThrusterGroup(stage[currentStage].th_main_h,stage[currentStage].nEngines,THGROUP_MAIN);
 SetDefaultPropellantResource(stage[currentStage].tank);
 
 SURFHANDLE ChoosenTexture=NULL; //Initialization of Texture
 

 int i;
 for(i=0;i<stage[currentStage].nEngines;i++){
	 
	exhaustN[currentStage][i]=AddExhaust(stage[currentStage].th_main_h[i],10*stage[currentStage].eng_diameter*stage[currentStage].engV4[i].t,stage[currentStage].eng_diameter*stage[currentStage].engV4[i].t,stage[currentStage].eng[i],operator*(stage[currentStage].eng_dir,-1),GetProperExhaustTexture(stage[currentStage].eng_tex));
	  
  if(!stage[currentStage].ParticlesPacked){
	  if(stage[currentStage].wps1){
		PARTICLE ps = GetProperPS(stage[currentStage].eng_pstream1);
		AddExhaustStreamGrowing(
		stage[currentStage].th_main_h[i],
		stage[currentStage].eng[i],
		&ps.Pss,
		ps.Growing,
		ps.GrowFactor_size,
		ps.GrowFactor_rate,
		TRUE,
		FALSE,
		currentStage,
		i);
		sprintf(logbuff,"%s: Stage n.%i Engine Exhaust Stream Added: %s to engine n.%i",GetName(),currentStage+1,stage[currentStage].eng_pstream1,i+1);
		oapiWriteLog(logbuff);}
	if(stage[currentStage].wps2){
		PARTICLE ps = GetProperPS(stage[currentStage].eng_pstream2);
	  	AddExhaustStreamGrowing(
		stage[currentStage].th_main_h[i],
		stage[currentStage].eng[i],
		&ps.Pss,
		ps.Growing,
		ps.GrowFactor_size,
		ps.GrowFactor_rate,
		TRUE,
		FALSE,
		currentStage,
		i);
		sprintf(logbuff,"%s: Stage n.%i Engine Exhaust Stream Added: %s to engine n.%i",GetName(),currentStage+1,stage[currentStage].eng_pstream2,i+1);
		oapiWriteLog(logbuff);}
	 }
	 sprintf(logbuff,"%s: Stage n. %i Engines Exhaust Added--> number of engines: %i , diameter: %.3f, position x: %.3f y: %.3f z: %.3f",GetName(),currentStage+1,stage[currentStage].nEngines,stage[currentStage].eng_diameter*stage[currentStage].engV4[i].t,stage[currentStage].eng[i].x,stage[currentStage].eng[i].y,stage[currentStage].eng[i].z);
	 oapiWriteLog(logbuff);
	 //oapiWriteLogV("Adding 1: %s 2: %s",stage[currentStage].eng_pstream1,stage[currentStage].eng_pstream2);
 }

 if(stage[currentStage].ParticlesPacked)
 {
	 PARTICLESTREAMSPEC partpacked[2];
	 partpacked[0]=GetProperPS(stage[currentStage].eng_pstream1).Pss;
	 partpacked[1]=GetProperPS(stage[currentStage].eng_pstream2).Pss;

	
	 int engine=abs(stage[currentStage].ParticlesPackedToEngine)-1;

	 VECTOR3 thdir,Pos[2];
	GetThrusterDir(stage[currentStage].th_main_h[engine],thdir);
	thdir.x*=-1;
	thdir.y*=-1;
	thdir.z*=-1;
		if(stage[currentStage].ParticlesPackedToEngine>0){
		/*Pos[0].x=stage[currentStage].eng[engine].x+thdir.x*partpacked[0].srcsize*0.5;
		Pos[0].y=stage[currentStage].eng[engine].y+thdir.y*partpacked[0].srcsize*0.5;
		Pos[0].z=stage[currentStage].eng[engine].z+thdir.z*partpacked[0].srcsize*0.5;
	
		Pos[1].x=stage[currentStage].eng[engine].x+thdir.x*partpacked[1].srcsize*0.5;
		Pos[1].y=stage[currentStage].eng[engine].y+thdir.y*partpacked[1].srcsize*0.5;
		Pos[1].z=stage[currentStage].eng[engine].z+thdir.z*partpacked[1].srcsize*0.5;*/
			Pos[0]=stage[currentStage].eng[engine];
			Pos[1]=stage[currentStage].eng[engine];
		}else{
		double Posx=0;
		double Posy=0;
		double Posz=0;
			for(int x=0;x<stage[currentStage].nEngines;x++)
			{
				Posx+=stage[currentStage].eng[x].x;
				Posy+=stage[currentStage].eng[x].y;
				Posz+=stage[currentStage].eng[x].z;
			}

			Posx/=stage[currentStage].nEngines;
			Posy/=stage[currentStage].nEngines;
			Posz/=stage[currentStage].nEngines;

			Pos[0].x=Posx;
			Pos[0].y=Posy;
			Pos[0].z=Posz;
			Pos[1]=Pos[0];


	}
		if(stage[currentStage].wps1){
	 AddExhaustStreamGrowing(stage[currentStage].th_main_h[engine],Pos[0],&partpacked[0],GetProperPS(stage[currentStage].eng_pstream1).Growing,GetProperPS(stage[currentStage].eng_pstream1).GrowFactor_size,GetProperPS(stage[currentStage].eng_pstream1).GrowFactor_rate,TRUE,FALSE,currentStage,engine);
	sprintf(logbuff,"%s: Stage n.%i Engine Packed Exhaust Stream Added: %s to engine n.%i",GetName(),currentStage+1,stage[currentStage].eng_pstream1,engine+1);
	oapiWriteLog(logbuff);}
		if(stage[currentStage].wps2){
	 AddExhaustStreamGrowing(stage[currentStage].th_main_h[engine],Pos[1],&partpacked[1],GetProperPS(stage[currentStage].eng_pstream2).Growing,GetProperPS(stage[currentStage].eng_pstream2).GrowFactor_size,GetProperPS(stage[currentStage].eng_pstream2).GrowFactor_rate,TRUE,FALSE,currentStage,engine);
	sprintf(logbuff,"%s: Stage n.%i Engine Packed Exhaust Stream Added: %s to engine n.%i",GetName(),currentStage+1,stage[currentStage].eng_pstream2,engine+1);
	oapiWriteLog(logbuff);}

 }




 if(stage[currentStage].DenyIgnition){
	for(int i=0;i<stage[currentStage].nEngines;i++)
	{
		SetThrusterResource(stage[currentStage].th_main_h[i],NULL);
	}
 }


  LightEmitter *le = AddPointLight (stage[currentStage].eng[0], 200, 1e-3, 0, 2e-3, col_d, col_s, col_a);
	le->SetIntensityRef (&th_main_level);

 return;
}

//Get Boosters Position given group number and booster number inside the group
VECTOR3 Multistage2015::GetBoosterPos(int nBooster, int N){

	VECTOR3 bpos=booster[nBooster].off;
	double arg=booster[nBooster].angle*RAD+(N-1)*2*PI/booster[nBooster].N;
	VECTOR3 bposdef=_V(bpos.x*cos(arg)-bpos.y*sin(arg),bpos.x*sin(arg)+bpos.y*cos(arg),bpos.z);			
	 
return bposdef;
}

char* Multistage2015::GetProperPayloadMeshName(int pnl, int n){
	if(n==0){
		return payload[pnl].meshname0;
	}else if(n==1){
		return payload[pnl].meshname1;
	}else if(n==2){
		return payload[pnl].meshname2;
	}else if(n==3){
		return payload[pnl].meshname3;
	}else if(n==4){
		return payload[pnl].meshname4;
	}else{	return payload[pnl].meshname0;
	}
}


void Multistage2015::RotatePayload(int pns, int nm,VECTOR3 anglesrad){
	
	VECTOR3 state=_V(anglesrad.x/(2*PI),anglesrad.y/(2*PI),anglesrad.z/(2*PI));//portion of 2*PI to rotate
	VECTOR3 reference;
	
	if(nm==0){reference=_V(0,0,0);}else{reference=operator-(payload[pns].off[nm],payload[pns].off[0]);}

if(!DeveloperMode){
	payloadrotatex[pns][nm]= new MGROUP_ROTATE(payload[pns].msh_idh[nm],NULL,NULL,reference,_V(1,0,0),(float)2*PI); 
	RotatePayloadAnim_x[pns][nm]=CreateAnimation(0);
	anim_x[pns][nm]=AddAnimationComponent(RotatePayloadAnim_x[pns][nm],0.0f,1.0f,payloadrotatex[pns][nm]);
	payloadrotatey[pns][nm]=  new MGROUP_ROTATE(payload[pns].msh_idh[nm],NULL,NULL,reference,_V(0,1,0),(float)2*PI) ; 
	RotatePayloadAnim_y[pns][nm]=CreateAnimation(0);
	anim_y[pns][nm]=AddAnimationComponent(RotatePayloadAnim_y[pns][nm],0.0f,1.0f,payloadrotatey[pns][nm]);

	payloadrotatez[pns][nm]= new MGROUP_ROTATE(payload[pns].msh_idh[nm],NULL,NULL,reference,_V(0,0,1),(float)2*PI); 
	RotatePayloadAnim_z[pns][nm]=CreateAnimation(0);
	anim_z[pns][nm]=AddAnimationComponent(RotatePayloadAnim_z[pns][nm],0.0f,1.0f,payloadrotatez[pns][nm]);


	

}else{
	SetAnimation(RotatePayloadAnim_z[pns][nm],0);
	SetAnimation(RotatePayloadAnim_y[pns][nm],0);
	SetAnimation(RotatePayloadAnim_x[pns][nm],0);
	
	/*DelAnimationComponent(RotatePayloadAnim_z[pns][nm],anim_z[pns][nm]);
	DelAnimationComponent(RotatePayloadAnim_y[pns][nm],anim_y[pns][nm]);
	DelAnimationComponent(RotatePayloadAnim_x[pns][nm],anim_x[pns][nm]);*/

}


SetAnimation(RotatePayloadAnim_x[pns][nm],state.x);
SetAnimation(RotatePayloadAnim_y[pns][nm],state.y);
SetAnimation(RotatePayloadAnim_z[pns][nm],state.z);


return;
}

VECTOR3 Multistage2015::RotateVector(const VECTOR3& input, double angle, const VECTOR3& rotationaxis) 
{
	// To rotate a vector in 3D space we'll need to build a matrix, these are the variables treqired to do so.
	MATRIX3 rMatrix;
	double c = cos(angle);
	double s = sin(angle);
	double t = 1.0 - c;
	double x = rotationaxis.x;
	double y = rotationaxis.y;
	double z = rotationaxis.z;

	// Build rotation matrix
	rMatrix.m11 = (t * x * x + c);
	rMatrix.m12 = (t * x * y - s * z);
	rMatrix.m13 = (t * x * z + s * y);
	rMatrix.m21 = (t * x * y + s * z);
	rMatrix.m22 = (t * y * y + c);
	rMatrix.m23 = (t * y * z - s * x);
	rMatrix.m31 = (t * x * z - s * y);
	rMatrix.m32 = (t * y * z + s * x);
	rMatrix.m33 = (t * z * z + c);

	// Perform Rotation
	VECTOR3 output = mul(rMatrix, input); // multiply the input vector by our rotation matrix to get our output vector
	return output; // Return rotated vector
}

//Load Meshes
void Multistage2015::LoadMeshes(){
		
 int q;
 for(q=currentStage;q<nStages;q++){
	 VECTOR3 pos=stage[q].off;
	 stage[q].msh_h=oapiLoadMeshGlobal(stage[q].meshname);
	 sprintf(logbuff,"%s: Stage n.%i Mesh Preloaded: %s",GetName(),q+1,stage[q].meshname);
		oapiWriteLog(logbuff);
	stage[q].msh_idh=AddMesh(stage[q].msh_h,&pos);
	sprintf(logbuff,"%s: Stage n.%i Mesh Added Mesh: %s @ x:%.3f y:%.3f z:%.3f",GetName(),q+1,stage[q].meshname,pos.x,pos.y,pos.z);
		oapiWriteLog(logbuff);
		if(stage[q].wInter==TRUE){
			
	VECTOR3 inspos=stage[q].interstage.off;
		stage[q].interstage.msh_h=oapiLoadMeshGlobal(stage[q].interstage.meshname);
		sprintf(logbuff,"%s: Interstage Mesh Preloaded for Stage %i",GetName(),q+1);
		oapiWriteLog(logbuff);
		stage[q].interstage.msh_idh=AddMesh(stage[q].interstage.msh_h,&inspos);
		sprintf(logbuff,"%s: Interstage Mesh Added: %s @ x:%.3f y:%.3f z:%.3f",GetName(),stage[q].interstage.meshname,inspos.x,inspos.y,inspos.z);
		oapiWriteLog(logbuff);
		}
	}
 

 
for(int pns=currentPayload;pns<nPayloads;pns++){

	if(!payload[pns].live){
	for(int nm=0;nm<payload[pns].nMeshes;nm++){
		
		VECTOR3 pos=payload[pns].off[nm];
		payload[pns].msh_h[nm]=oapiLoadMeshGlobal(GetProperPayloadMeshName(pns,nm));
		sprintf(logbuff,"%s Payload Mesh Preloaded %i",GetName(),pns+1);
		oapiWriteLog(logbuff);
		payload[pns].msh_idh[nm]=AddMesh(payload[pns].msh_h[nm],&pos);
		sprintf(logbuff,"%s: Payload n.%i Mesh Added: %s @ x:%.3f y:%.3f z:%.3f",GetName(),pns+1,GetProperPayloadMeshName(pns,nm),pos.x,pos.y,pos.z);
		oapiWriteLog(logbuff);
		if((payload[pns].render==0)&&(wFairing==1)){
		SetMeshVisibilityMode(payload[pns].msh_idh[nm],MESHVIS_NEVER);
		}
		//if(payload[pns].rotated){
		RotatePayload(pns,nm,payload[pns].Rotation);
		//	}

		
		/// ATTACHMENT POINTS MUST BE CREATED ANYWAY TO GET THE PAYLOAD BACK ONCE RELOADING THE SCENARIO
		}
	}//else{
	VECTOR3 direction,normal;
		if(!payload[pns].rotated){direction=_V(0,0,1);normal=_V(0,1,0);}else{direction=payload[pns].Rotation;
			VECTOR3 rotation;
			rotation=payload[pns].Rotation;
			direction=mul(RotationMatrix(rotation),_V(0,0,1));
			normal=mul(RotationMatrix(rotation),_V(0,1,0));
			normalise(normal);
			normalise(direction);
				}

	live_a[pns]=CreateAttachment(false,payload[pns].off[0],direction,normal,"MS2015",false);
	
	//}
}
 int qb;
 for(qb=currentBooster;qb<nBoosters;qb++){
 VECTOR3 bpos=booster[qb].off;
 VECTOR3 bposxy=bpos;
 bposxy.z=0;
 double bro=length(bposxy);
 int NN;
for(NN=1;NN<booster[qb].N+1;NN++){
		char boosbuff[32],boosmhname[MAXLEN];
		sprintf(boosbuff,"_%i",NN);
		strcpy_s(boosmhname,booster[qb].meshname);
		strcat_s(boosmhname,boosbuff);

		double arg=booster[qb].angle*RAD+(NN-1)*2*PI/booster[qb].N;
		VECTOR3 bposdef=_V(bpos.x*cos(arg)-bpos.y*sin(arg),bpos.x*sin(arg)+bpos.y*cos(arg),bpos.z);
		booster[qb].msh_h[NN]=oapiLoadMeshGlobal(boosmhname);
		sprintf(logbuff,"%s: Booster Mesh Preloaded: %s",GetName(),boosmhname);
		oapiWriteLog(logbuff);
		booster[qb].msh_idh[NN]=AddMesh(booster[qb].msh_h[NN],&bposdef);
		sprintf(logbuff,"%s: Booster Mesh Added Mesh: %s @ x:%.3f y:%.3f z:%.3f",GetName(),boosmhname,bposdef.x,bposdef.y,bposdef.z);
		oapiWriteLog(logbuff);
		}
	}

if(wFairing==1){
	VECTOR3 fpos=fairing.off;
	VECTOR3 fposxy=fpos;
	fposxy.z=0;
	double fro=length(fposxy);
	int NF;
	for(NF=1;NF<fairing.N+1;NF++){
		char fairbuff[32],fairmshname[MAXLEN];
		sprintf(fairbuff,"_%i",NF);
		strcpy_s(fairmshname,fairing.meshname);
		strcat_s(fairmshname,fairbuff);
		VECTOR3 fposdef=_V(fro*cos(fairing.angle*RAD+(NF-1)*2*PI/fairing.N),fro*sin(fairing.angle*RAD+(NF-1)*2*PI/fairing.N),fpos.z);
		fairing.msh_h[NF]=oapiLoadMeshGlobal(fairmshname);
		sprintf(logbuff,"%s: Fairing Mesh Preloaded: %s", GetName(),fairmshname);
		oapiWriteLog(logbuff);
		fairing.msh_idh[NF]=AddMesh(fairing.msh_h[NF],&fposdef);
		sprintf(logbuff,"%s: Fairing Mesh Added Mesh: %s @ x:%.3f y:%.3f z:%.3f",GetName(),fairmshname,fposdef.x,fposdef.y,fposdef.z);
		oapiWriteLog(logbuff);

		}

	}
if(wAdapter==TRUE){	
		VECTOR3 adappos=Adapter.off;
		Adapter.msh_h=oapiLoadMeshGlobal(Adapter.meshname);
		sprintf(logbuff,"%s: Adapter Mesh Preloaded",GetName());
		oapiWriteLog(logbuff);
		Adapter.msh_idh=AddMesh(Adapter.msh_h,&adappos);
		sprintf(logbuff,"%s: Adapter Mesh Added: %s @ x:%.3f y:%.3f z:%.3f",GetName(),Adapter.meshname,adappos.x,adappos.y,adappos.z);
		oapiWriteLog(logbuff);
		
	}
if(wLes==TRUE)
{
	VECTOR3 LesPos=Les.off;
	Les.msh_h=oapiLoadMeshGlobal(Les.meshname);
	sprintf(logbuff,"%s: Les Mesh Preloaded",GetName());
	oapiWriteLog(logbuff);
	Les.msh_idh=AddMesh(Les.msh_h,&LesPos);
	sprintf(logbuff,"%s: Les Mesh Added %s @ x:%.3f y:%.3f z:%.3f",GetName(),Les.meshname,LesPos.x,LesPos.y,LesPos.z);
	oapiWriteLog(logbuff);
}
	
	return;
}

//Updates PMI, Cross Sections etc.
void Multistage2015::UpdatePMI(){
	double TotalVolume=0;
	TotalHeight=0;
	int i;
	for(i=currentStage;i<nStages;i++){
		TotalHeight+=stage[i].height;
		stage[i].volume=stage[i].height*0.25*PI*stage[i].diameter*stage[i].diameter;
		TotalVolume+=stage[i].volume;
		stage[i].interstage.volume=stage[i].interstage.height*0.25*PI*stage[i].interstage.diameter*stage[i].interstage.diameter;
		TotalVolume+=stage[i].interstage.volume;
	}
	int q;
	double CSBoosters=0;
	for(q=currentBooster;q<nBoosters;q++){
		booster[q].volume=booster[q].N*booster[q].height*0.25*PI*booster[q].diameter*booster[q].diameter;
		CSBoosters+=0.25*PI*booster[q].diameter*booster[q].diameter;
	//	TotalVolume+=booster[q].volume;
	}

	int k;
	for(k=currentPayload;k<nPayloads;k++){
		//TotalHeight+=payload[k].height; //Not Considered in Height
		payload[k].volume=payload[k].height*0.25*PI*payload[k].diameter*payload[k].diameter;
		TotalVolume+=payload[k].volume;
	
	}

		if(wFairing==1){
		//TotalHeight+=fairing.height; //Not Considered in Height
		fairing.volume=fairing.height*0.25*PI*fairing.diameter*fairing.diameter;
	TotalVolume+=fairing.volume;
	}
		if(wLes==TRUE){
			Les.volume=Les.height*0.25*PI*Les.diameter*Les.diameter;
			TotalVolume+=Les.volume;
		}
	double PhiEq=sqrt(4*TotalVolume/(PI*TotalHeight));

	double CSX,CSY,CSZ;
	CSX=TotalHeight*PhiEq;
	CSY=CSX;
	CSZ=PI*0.25*PhiEq*PhiEq+CSBoosters;
	SetCrossSections(_V(CSX,CSY,CSZ));

	double IZ=(PhiEq*0.5)*(PhiEq*0.5)*0.5;
	double IX,IY;
	IX=(3*(PhiEq*0.5)*(PhiEq*0.5)+TotalHeight*TotalHeight)/12;
	IY=IX;
	SetPMI(_V(IX,IY,IZ));
  //  SetSize(10*TotalHeight);
	if(Configuration==0){
	SetSize(stage[0].height*0.5+Misc.COG);
	//if(GetSize()<=0){SetSize(stage[0].height*0.5+Misc.COG);}
	}else{
	SetSize(TotalHeight);
	}
//	oapiWriteLogV("####### SIZE: %.3f",GetSize());
	return;
}

//Update Mass of the Vehicle on call
void Multistage2015::UpdateMass(){
	
	double EM=stage[currentStage].emptymass;
	
	int s;
	for(s=currentStage+1;s<nStages;s++){
		EM+=stage[s].emptymass;
		if(stage[s].wInter==TRUE){
			EM+=stage[s].interstage.emptymass;
		}
			 }
	
	int bs;
	for(bs=currentBooster;bs<nBoosters;bs++){
	EM+=(booster[bs].emptymass*booster[bs].N);
			}
		
	int pns;
	for(pns=currentPayload;pns<nPayloads;pns++){
		EM+=payload[pns].mass;
	}
	if(wFairing==1){	EM+=2*fairing.emptymass;}
	if(wAdapter==TRUE){ EM+=Adapter.emptymass;}
	if(wLes==TRUE){EM+=Les.emptymass;}
	SetEmptyMass(EM);
	
	
	
	return;
}

//Update mesh offsets
void Multistage2015::UpdateOffsets(){
	 currentDelta=stage[currentStage].off.z;
	int i;
	for(i=currentStage;i<nStages;i++){
		stage[i].off.z-=currentDelta;
		if(stage[i].wInter==TRUE){
			stage[i].interstage.off.z-=currentDelta;
		}
	}
	int p;
	for(p=currentPayload;p<nPayloads;p++){
		for(int s=0;s<payload[p].nMeshes;s++){
		payload[p].off[s].z-=currentDelta;
		}
	}
	
	int z;
	for(z=currentBooster;z<nBoosters;z++){
		booster[z].off.z-=currentDelta;
	}

	if(wFairing==1){
		fairing.off.z-=currentDelta;
	}

	if(wAdapter==TRUE){
		Adapter.off.z-=currentDelta;
	}
	if(wLes==TRUE){
		Les.off.z-=currentDelta;
	}
	return;
}
void Multistage2015::UpdateLivePayloads(){
	
	for(int pns=currentPayload;pns<nPayloads;pns++){
		if(payload[pns].live){
		VESSELSTATUS2 vslive;
		memset(&vslive,0,sizeof(vslive));
		vslive.version=2;
		OBJHANDLE checklive=oapiGetVesselByName(payload[pns].name);
		if(oapiIsVessel(checklive)){
		ATTACHMENTHANDLE liveatt;
		VESSEL4 *livepl;
		livepl = static_cast<VESSEL4*>(oapiGetVesselInterface(checklive));
			  liveatt=livepl->CreateAttachment(TRUE,_V(0,0,0),_V(0,0,-1),_V(0,1,0),"MS2015",FALSE);
			  AttachChild(checklive,live_a[pns],liveatt);
			  if(payload[pns].mass<=0){
				  payload[pns].mass=livepl->GetMass();}
			  if(payload[pns].height<=0){
				payload[pns].height=livepl->GetSize();
				payload[pns].diameter=payload[pns].height*0.1;}
				}else{

		VESSEL4 *v;
		OBJHANDLE hObj;
		ATTACHMENTHANDLE liveatt;
		GetStatusEx(&vslive);
		hObj=oapiCreateVesselEx(payload[pns].name,payload[pns].module,&vslive);
		
		
		if(oapiIsVessel(hObj)){
              v = static_cast<VESSEL4*>(oapiGetVesselInterface(hObj));
			 
			  liveatt=v->CreateAttachment(TRUE,_V(0,0,0),_V(0,0,-1),_V(0,1,0),"MS2015",FALSE);
			 
			  AttachChild(hObj,live_a[pns],liveatt);
		      if(payload[pns].mass<=0){
				  payload[pns].mass=v->GetMass();}
			  if(payload[pns].height<=0){
				payload[pns].height=v->GetSize();
				payload[pns].diameter=payload[pns].height*0.1;}
				}
			}
		}
	}
	UpdateMass();
	UpdatePMI();
return;}
//Returns the particlestream specification to use or the empty one if not found
 PARTICLE Multistage2015::GetProperPS(char name[MAXLEN]){
	 for(int z=0;z<MAXLEN;z++){
		  name[z]=tolower(name[z]);
	  }
	 char checktxt[MAXLEN];
	  int nt=0;
	  int k;
	for(nt=0;nt<16;nt++){
	  for(k=0;k<MAXLEN;k++){
		  checktxt[k]=Particle[nt].ParticleName[k];
	  }
	 for(int z=0;z<MAXLEN;z++){
		  checktxt[z]=tolower(checktxt[z]);
	  }
	  
	   if(strncmp(name,checktxt,MAXLEN-5)==0){

	return Particle[nt];
	   }
	
	}
return Particle[15];
 }

 //returns the texture to be used or the empty one
SURFHANDLE Multistage2015::GetProperExhaustTexture(char name[MAXLEN]){
	char checktxt[MAXLEN];
	  int nt=0;
	  int k;
	for(nt=0;nt<nTextures;nt++){
	  for(k=0;k<MAXLEN;k++){
		  checktxt[k]=tex.TextureName[k][nt];
	  }
	  
	   if(strncmp(name,checktxt,MAXLEN-5)==0){
	  
	 return tex.hTex[nt];
	   }
	
	}
	  
return NULL;
}

//creates Ullage engine and exhausts
void Multistage2015::CreateUllageAndBolts(){
	if(stage[currentStage].ullage.wUllage)
	{
		stage[currentStage].ullage.ignited=FALSE;
		stage[currentStage].ullage.th_ullage=CreateThruster(_V(0,0,0),_V(0,0,1),stage[currentStage].ullage.thrust,stage[currentStage].tank,100000);
		/*VECTOR3 *ullpos=new VECTOR3[stage[currentStage].ullage.N];
		VECTOR3 *ulldir=new VECTOR3[stage[currentStage].ullage.N];
*/
		//double angleprog=0;
		for(int i=0;i<stage[currentStage].ullage.N;i++)
		{
			double ull_angle;
			if(IsOdd((int)stage[currentStage].ullage.N))
			{
			if(i<(int)(stage[currentStage].ullage.N*0.5)){
			 ull_angle=stage[currentStage].ullage.angle*RAD+(i)*((2*PI/stage[currentStage].ullage.N)/stage[currentStage].ullage.rectfactor);
			
			//angleprog+=ull_angle;
			}else if(i==(int)(stage[currentStage].ullage.N*0.5)){ 
				ull_angle=PI+stage[currentStage].ullage.angle*RAD;
				/*sprintf(logbuff,"i: %i ull_angle: %.3f",i,ull_angle*DEG);
			 oapiWriteLog(logbuff);*/
			}else{
			 ull_angle=stage[currentStage].ullage.angle*RAD+PI+(i-(int)(stage[currentStage].ullage.N*0.5))*((2*PI/stage[currentStage].ullage.N)/stage[currentStage].ullage.rectfactor);
			 /*sprintf(logbuff,"i: %i ull_angle: %.3f",i,ull_angle*DEG);
			 oapiWriteLog(logbuff);*/
			}
			}else{
				ull_angle=stage[currentStage].ullage.angle*RAD+(i)*(2*PI/stage[currentStage].ullage.N);
			}
			VECTOR3 ulldir=RotateVecZ(stage[currentStage].ullage.dir,ull_angle);
			VECTOR3 ullpos=RotateVecZ(stage[currentStage].ullage.pos,ull_angle);
					    //VECTOR3 ull_ofs=operator+(GetBoosterPos(bi,biii),RotateVecZ(booster[bi].eng[bii],angle));

			AddExhaust(stage[currentStage].ullage.th_ullage,stage[currentStage].ullage.length,stage[currentStage].ullage.diameter,ullpos,ulldir,GetProperExhaustTexture(stage[currentStage].ullage.tex));
		}
	}

	if(stage[currentStage].expbolt.wExpbolt)
	{
		PARTICLE ps = GetProperPS(stage[currentStage].expbolt.pstream);
		stage[currentStage].expbolt.threxp_h=CreateThruster(stage[currentStage].expbolt.pos,
		stage[currentStage].expbolt.dir,
		0,
		stage[currentStage].tank,
		100000,
		100000);
		AddExhaustStream(stage[currentStage].expbolt.threxp_h,
		&ps.Pss);
	}

	return;
}
//Setting up of the vehicle, it's the setclass caps, but called after the ini is parsed
void Multistage2015::VehicleSetup(){
	

SetRotDrag(_V(0.7,0.7,0.06)); //Got from Vinka
	
 ///PROPELLANTS
 int bk;
 for(bk=currentBooster;bk<nBoosters;bk++){
  booster[bk].tank=CreatePropellantResource(booster[bk].fuelmass*booster[bk].N);
  sprintf(logbuff,"%s: booster n. %i Tank Added: %.3f kg",GetName(),bk+1,booster[bk].fuelmass*booster[bk].N);
  oapiWriteLog(logbuff);
 }
	
int k;
for(k=nStages-1;k>currentStage-1;k--){
	 stage[k].tank=CreatePropellantResource(stage[k].fuelmass);
	 sprintf(logbuff,"%s: Stage n. %i Tank Added: %.3f kg",GetName(),k+1,stage[k].fuelmass);
	 oapiWriteLog(logbuff);
 }
 
 ////ISP
 int r;
 for(r=currentStage;r<nStages;r++){
	 stage[r].isp=stage[r].thrust*stage[r].burntime/stage[r].fuelmass;
 }

 int br;
 for(br=currentBooster;br<nBoosters;br++){
	 booster[br].isp=(booster[br].thrust*booster[br].N)*booster[br].burntime/(booster[br].fuelmass*booster[br].N);
 }



nPsh=0;
for(int pp=0;pp<nStages;pp++)
{
	for(int ppp=0;ppp<stage[pp].nEngines;ppp++)
	{
		nPsh++;
	}
}
for(int pb=0;pb<nBoosters;pb++)
{
	for(int ppb=0;ppb<booster[pb].N;ppb++)
	{
		for(int ppbb=0;ppbb<booster[pb].nEngines;ppbb++)
		{
		nPsh++;
		}
	}
	
}
nPsh*=2;
//if(psg){delete[] psg;}

psg=new PSGROWING[nPsh];

for(int ps=0;ps<nPsh;ps++)
{
	psg[ps].GrowFactor_rate=0;
	psg[ps].GrowFactor_size=0;
	psg[ps].growing=FALSE;
	psg[ps].pos=_V(0,0,0);
	psg[ps].psh[0]=NULL;
	psg[ps].psh[1]=NULL;
	psg[ps].psh[2]=NULL;
	
	psg[ps].th=NULL;
	psg[ps].pss=Particle[15].Pss;
	psg[ps].status=1;
	psg[ps].counting=FALSE;
	psg[ps].doublepstime=0;
	psg[ps].baserate=0;
	psg[ps].basesize=0;
	psg[ps].basepos=_V(0,0,0);
	psg[ps].FirstLoop=TRUE;
	
//	psg[ps].Th_idx=ps;
}
 /////MAIN THRUSTERS
 
 CreateMainThruster();
 
 //attitude thrusters
 CreateRCS();
 
 //add Boosters Engines
 unsigned int bi,bii,biii;
 for(bi = currentBooster; bi < min(nBoosters, 10); bi++){
	 for(unsigned int bn=0;bn<booster[bi].N;bn++)
	 {
		 VECTOR3 pos,dir;
		if(Misc.thrustrealpos){
			pos=GetBoosterPos(bi,bn);
			dir=booster[bi].eng_dir;

		}else{
			pos=_V(0,0,0);
			dir=_V(0,0,1);
		}
		//booster[bi].th_booster_h[0]=CreateThruster(pos,dir,booster[bi].N*booster[bi].thrust,booster[bi].tank,booster[bi].isp);
		booster[bi].th_booster_h[bn]=CreateThruster(pos,dir,booster[bi].thrust,booster[bi].tank,booster[bi].isp);
	 }
	booster[bi].Thg_boosters_h = CreateThrusterGroup(booster[bi].th_booster_h,booster[bi].N,THGROUP_USER);
	
	 Particle[13].Pss.srcsize=stage[0].diameter;
	 Particle[14].Pss.srcsize=0.5*stage[0].diameter;

	if(booster[bi].nEngines==0){
	 for(bii=0;bii<booster[bi].N;bii++){
		  booster[bi].eng[bii]=_V(0,0,-booster[bi].height*0.5);
	 VECTOR3 engofs=operator+(GetBoosterPos(bi,bii),booster[bi].eng[bii]);
	 AddExhaust(booster[bi].th_booster_h[bii],10*booster[bi].eng_diameter,booster[bi].eng_diameter,engofs,operator*(booster[bi].eng_dir,-1),GetProperExhaustTexture(booster[bi].eng_tex));

	 if(booster[bi].wps1){
		PARTICLE ps = GetProperPS(booster[bi].eng_pstream1);
		AddExhaustStreamGrowing(booster[bi].th_booster_h[bii],
		engofs,
		&ps.Pss,
		ps.Growing,
		ps.GrowFactor_size,
		ps.GrowFactor_rate,
		TRUE,
		TRUE,
		bi,
		bii);
	sprintf(logbuff,"%s: Booster Group n.%i Engine Exhaust Stream Added: %s to engine n.%i",GetName(),bi+1,booster[bi].eng_pstream1,bii+1);
	oapiWriteLog(logbuff);}
	 if(booster[bi].wps2){
		PARTICLE ps = GetProperPS(booster[bi].eng_pstream2);
	 	AddExhaustStreamGrowing(booster[bi].th_booster_h[bii],
			engofs,
			&ps.Pss,
			ps.Growing,
			ps.GrowFactor_size,
			ps.GrowFactor_rate,
			TRUE,
			TRUE,
			bi,
			bii);
	sprintf(logbuff,"%s: Booster Group n.%i Engine Exhaust Stream Added: %s to engine n.%i",GetName(),bi+1,booster[bi].eng_pstream2,bii+1);
	oapiWriteLog(logbuff); }

	 sprintf(logbuff,"%s: Booster Engines Exhaust Added--> Booster Group: %i number of engines: %i , diameter: %.3f, position x: %.3f y: %.3f z: %.3f",GetName(),bi+1,booster[bi].nEngines,booster[bi].eng_diameter,engofs.x,engofs.y,engofs.z);
	 oapiWriteLog(logbuff);
	 }
 }else{
	  
 for(bii=0;bii<booster[bi].nEngines;bii++){
	for(biii=1;biii<booster[bi].N+1;biii++){
		double angle=booster[bi].angle*RAD+(biii-1)*2*PI/booster[bi].N;
		
	 VECTOR3 engofs=operator+(GetBoosterPos(bi,biii),RotateVecZ(booster[bi].eng[bii],angle));
	 
	 AddExhaust(booster[bi].th_booster_h[biii-1],10*booster[bi].eng_diameter,booster[bi].eng_diameter,engofs,operator*(booster[bi].eng_dir,-1),GetProperExhaustTexture(booster[bi].eng_tex));

	 if(booster[bi].wps1){
		PARTICLE ps = GetProperPS(booster[bi].eng_pstream1);
	 	AddExhaustStreamGrowing(booster[bi].th_booster_h[biii-1],
		engofs,
		&ps.Pss,
		ps.Growing,
		ps.GrowFactor_size,
		ps.GrowFactor_rate,
		TRUE,
		TRUE,
		bi,
		biii-1);
	 sprintf(logbuff,"%s: Booster Group n.%i Engine Exhaust Stream Added: %s to engine n.%i",GetName(),bi+1,booster[bi].eng_pstream1,biii);
	 oapiWriteLog(logbuff);}
	 if(booster[bi].wps2){
		PARTICLE ps = GetProperPS(booster[bi].eng_pstream2);
	 	AddExhaustStreamGrowing(booster[bi].th_booster_h[biii-1],
		engofs,
		&ps.Pss,
		ps.Growing,
		ps.GrowFactor_size,
		ps.GrowFactor_rate,
		TRUE,
		TRUE,
		bi,
		biii-1);
	 sprintf(logbuff,"%s: Booster Group n.%i Engine Exhaust Stream Added: %s to engine n.%i",GetName(),bi+1,booster[bi].eng_pstream2,biii);
	 oapiWriteLog(logbuff);}

	 sprintf(logbuff,"%s: Booster Engines Exhaust Added--> Booster Group: %i number of engines: %i , diameter: %.3f, position x: %.3f y: %.3f z: %.3f",GetName(),bi+1,booster[bi].nEngines,booster[bi].eng_diameter,engofs.x,engofs.y,engofs.z);
	 oapiWriteLog(logbuff);
				}
			}
		}

		if(booster[bi].expbolt.wExpbolt)
		{
			PARTICLE ps = GetProperPS(booster[bi].expbolt.pstream);
			booster[bi].expbolt.threxp_h=CreateThruster(booster[bi].expbolt.pos,
			booster[bi].expbolt.dir,
			0,
			booster[bi].tank,
			100000,
			100000);
			AddExhaustStream(booster[bi].expbolt.threxp_h,
			&ps.Pss);
		}







































	}
 





















 //Ullage
 CreateUllageAndBolts();


 CogElev=Misc.COG;

 	//added by rcraig42 to set drag_factor to 1 if not set in ini ------------------------------------------------





	
	if(Misc.drag_factor<=0){
		Misc.drag_factor=1;

	








	}

 SetCW(0.2 * Misc.drag_factor,0.5,1.5,1.5);		// Modded to miltiply drag in z_pos direction by drag_factor

//-----------------------------------------------------------------------------------------------------------



 VECTOR3 intdpts[3],tdpts[12];
/* for(int ii=0;ii<3;ii++)
 {
	 intdpts[ii]=_V(cos(ii*2/3*PI)*stage[0].diameter*0.5,sin(ii*2/3*PI)*stage[0].diameter*0.5,-stage[0].height*0.5-CogElev);
 }*/



 //////////////TOUCHDOWN POINTS WORKING ORIGINAL MS WAY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /*
 double rad=100;//stage[0].diameter*0.5;
 intdpts[0]=_V(0,rad,-stage[0].height*0.5-CogElev);
 intdpts[2]=_V(-0.866*rad,-0.5*rad,-stage[0].height*0.5-CogElev);
 intdpts[1]=_V(0.866*rad,-0.5*rad,-stage[0].height*0.5-CogElev);
 
 for(int j=0;j<3;j++)
 {		
	 tdpts[j].x=intdpts[j].x;
	 tdpts[j].y=intdpts[j].y*cos(Misc.VerticalAngle)+intdpts[j].z*sin(Misc.VerticalAngle);
	 tdpts[j].z=-intdpts[j].y*sin(Misc.VerticalAngle)+intdpts[j].z*cos(Misc.VerticalAngle);
 }
 */

 //////////////////////////////////////////////////END//////////////////////////////////////////////////



 /////////////////NEW DEFINITION FOR RAMP WITH ATTACHMENT!!!!!
 intdpts[0]=_V(0,1,0);
 intdpts[2]=_V(-0.866,-0.5,0);
 intdpts[1]=_V(0.866,-0.5,0);

 for(int j=0;j<3;j++)
 {
	 tdpts[j]=intdpts[j];
		 
 }
 //////////////////////END//////////////////////////////////////


 /*tdpts[3]=_V(-stage[0].diameter*0.5,0,100);
 tdpts[4]=_V(stage[0].diameter*0.5,0,100);
 tdpts[5]=_V(-stage[0].diameter*0.5,0,10*TotalHeight*0.5);
 tdpts[6]=_V(stage[0].diameter*0.5,0,10*TotalHeight*0.5);
 tdpts[7]=_V(0,-stage[0].diameter*0.5,100);
 tdpts[8]=_V(0,stage[0].diameter*0.5,100);
 tdpts[9]=_V(0,-stage[0].diameter*0.5,10*TotalHeight*0.5);
 tdpts[10]=_V(0,stage[0].diameter*0.5,10*TotalHeight*0.5);
 

 tdpts[11]=_V(0,0,10*TotalHeight);*/

 tdpts[3]=tdpts[0];
 tdpts[4]=tdpts[1];
 tdpts[5]=tdpts[2];

 tdpts[3].z*=-0.1;
 tdpts[4].z*=-0.1;
 tdpts[5].z*=-0.1;


 /*tdpts[4]=_V(-stage[0].diameter*0.5,0,0);
 tdpts[5]=_V(stage[0].diameter*0.5,0,0);
 tdpts[6]=_V(0,-stage[0].diameter*0.5,0);
 tdpts[7]=_V(0,stage[0].diameter*0.5,0);*/
 //SetTouchdownPoints(tdpts[0],tdpts[1],tdpts[2]);
 TOUCHDOWNVTX td[12];
 for(int i=0;i<12;i++)
 {
	 td[i].pos=tdpts[i];
	/* td[i].damping=1e7;
	 td[i].mu=0.9;
	 td[i].mu_lng=0.9;
	 td[i].stiffness=1e7;
	 */
	  td[i].damping=3e6;
	 td[i].mu=3;
	 td[i].mu_lng=3;
	 td[i].stiffness=3e7;
 }
 
 /*={
// for(int q=0;q<3;q++)
 //{
		 
	 {tdpts[0],1e8,1e7,3},
	 {tdpts[1],1e8,1e7,3},
	 {tdpts[2],1e8,1e7,3},
	 {tdpts[3],1e8,1e7,3},
	 {tdpts[4],1e8,1e7,3},
	 {tdpts[5],1e8,1e7,3},
	 {tdpts[6],1e8,1e7,3},
	 {tdpts[7],1e8,1e7,3}
	 */
	 //{tdpts[4]},
	 //{tdpts[5]},
	 //{tdpts[6]},
	 //{tdpts[7]}
	 
	 /*
	 {tdpts[0],3e7,3e2,0.3},
	 {tdpts[1],3e7,3e2,0.3},
	 {tdpts[2],3e7,3e2,0.3}
	 	*/ 
	 	 
// };
 //}

SetTouchdownPoints(td,6);
 //SetTouchdownPoints(tdpts[0],tdpts[1],tdpts[2]);

  if(currentBooster<nBoosters) wBoosters=TRUE;

 UpdateMass();
 UpdatePMI();
 
 




 return;
}

//Spawns Item
void Multistage2015::Spawn(int type, int current){

char mn[MAXLEN];
VESSELSTATUS2 vs;
memset(&vs,0,sizeof(vs));
vs.version=2;
GetStatusEx(&vs);
VECTOR3 ofs;
VECTOR3 rofs, rvel = {vs.rvel.x, vs.rvel.y, vs.rvel.z},vrot={vs.vrot.x,vs.vrot.y,vs.vrot.z},arot={vs.arot.x,vs.arot.y,vs.arot.z};
VECTOR3 vel; 

	switch(type){

	case TBOOSTER:
		int i;
		for(i=1;i<booster[current].N+1;i++){
		GetMeshOffset(booster[current].msh_idh[i],ofs);
		//double vro=length(booster[current].speed);
		//vel =_V(vro*cos(booster[current].angle*RAD+(i-1)*2*PI/booster[current].N),vro*sin(booster[current].angle*RAD+(i-1)*2*PI/booster[current].N),booster[current].speed.z);
		vel=RotateVecZ(booster[current].speed,booster[current].angle*RAD+(i-1)*2*PI/booster[current].N);
		Local2Rel (ofs, vs.rpos);
		GlobalRot(vel,rofs);
		vs.rvel.x=rvel.x+rofs.x;
		vs.rvel.y=rvel.y+rofs.y;
		vs.rvel.z=rvel.z+rofs.z;
		double arg=booster[current].angle*RAD+(i-1)*2*PI/booster[current].N;
		vs.vrot.x=vrot.x+booster[current].rot_speed.x*cos(arg)-booster[current].rot_speed.y*sin(arg);
		vs.vrot.y=vrot.y+booster[current].rot_speed.x*sin(arg)+booster[current].rot_speed.y*cos(arg);
		vs.vrot.z=vrot.z+booster[current].rot_speed.z;
		
		char mn2[32];
		sprintf(mn2, "_%i", i);
		strcpy_s(mn, booster[current].meshname);
		strcat_s(mn, mn2);

		// Verificación de módulo antes de crear
		char path[MAXLEN];
		sprintf(path, "Modules\\%s.dll", booster[current].module);
		FILE *f = nullptr;
		fopen_s(&f, path, "rb");
		if (!f) {
			sprintf(logbuff, "ERROR: No se encontró el módulo '%s' para booster[%d]. Archivo esperado: '%s'", booster[current].module, current, path);
			oapiWriteLog(logbuff);
			return;
		} else {
			fclose(f);
		}

		sprintf(logbuff, "%s: Booster n.%i jettisoned name: %s @%.3f", GetName(), current + 1, mn, MET);
		oapiWriteLog(const_cast<char *>("Before oapiCreateVesselEx - Line 2313"));
		oapiCreateVesselEx(mn, booster[current].module, &vs);
		oapiWriteLog(logbuff);

		}
		oapiWriteLog(const_cast<char *>("Spawn TBOOSTER: fin de bloque TBOOSTER"));
break;
	case TSTAGE:

		GetMeshOffset(stage[current].msh_idh,ofs);
		vel =_V(stage[current].speed.x,stage[current].speed.y,stage[current].speed.z);
		Local2Rel (ofs, vs.rpos);
		GlobalRot(vel,rofs);
		vs.rvel.x=rvel.x+rofs.x;
		vs.rvel.y=rvel.y+rofs.y;
		vs.rvel.z=rvel.z+rofs.z;
		vs.vrot.x=vrot.x+stage[current].rot_speed.x;
		vs.vrot.y=vrot.y+stage[current].rot_speed.y;
		vs.vrot.z=vrot.z+stage[current].rot_speed.z;

		strcpy_s(mn,stage[current].meshname);
		oapiCreateVesselEx(mn,stage[current].module, &vs);
		
		sprintf(logbuff,"%s: Stage n.%i jettisoned name: %s @%.3f",GetName(),current+1,mn,MET);
		oapiWriteLog(logbuff);
		stage_ignition_time=MET;
break;

	case TPAYLOAD:
		if(!payload[current].live){
			GetMeshOffset(payload[current].msh_idh[0],ofs);
			vel =_V(payload[current].speed.x,payload[current].speed.y,payload[current].speed.z);
			Local2Rel (ofs, vs.rpos);
			GlobalRot(vel,rofs);
			vs.rvel.x=rvel.x+rofs.x;
			vs.rvel.y=rvel.y+rofs.y;
			vs.rvel.z=rvel.z+rofs.z;
			vs.vrot.x=vrot.x+payload[current].rot_speed.x;
			vs.vrot.y=vrot.y+payload[current].rot_speed.y;
			vs.vrot.z=vrot.z+payload[current].rot_speed.z;
			
			if(payload[current].rotated){
			MATRIX3 RotMatrix,RotMatrix_Def;
			GetRotationMatrix(RotMatrix);
			VECTOR3 rotation;
			rotation=payload[current].Rotation;
			RotMatrix_Def=mul(RotMatrix,RotationMatrix(rotation));
			vs.arot.x=atan2(RotMatrix_Def.m23,RotMatrix_Def.m33);
			vs.arot.y=-asin(RotMatrix_Def.m13);
			vs.arot.z=atan2(RotMatrix_Def.m12,RotMatrix_Def.m11);
			}
			OBJHANDLE hpl;
			hpl=oapiCreateVesselEx(payload[current].name,payload[current].module, &vs);
			if(currentPayload+1==Misc.Focus){
			oapiSetFocusObject(hpl);
					}
		}else{
			if(GetAttachmentStatus(live_a[current])){
				OBJHANDLE live=GetAttachmentStatus(live_a[current]);
				VESSEL4 *v;
				v=static_cast<VESSEL4*>(oapiGetVesselInterface(live));
				
				VECTOR3 dir,rot; //only for the get params
				GetAttachmentParams(live_a[current],ofs,dir,rot);
				DetachChild(live_a[current],0);//length(payload[current].speed));
			
				vel =_V(payload[current].speed.x,payload[current].speed.y,payload[current].speed.z);
			Local2Rel (ofs, vs.rpos);
			GlobalRot(vel,rofs);
			vs.rvel.x=rvel.x+rofs.x;
			vs.rvel.y=rvel.y+rofs.y;
			vs.rvel.z=rvel.z+rofs.z;
			vs.vrot.x=vrot.x+payload[current].rot_speed.x;
			vs.vrot.y=vrot.y+payload[current].rot_speed.y;
			vs.vrot.z=vrot.z+payload[current].rot_speed.z;
			
			if(payload[current].rotated){
			MATRIX3 RotMatrix,RotMatrix_Def;
			GetRotationMatrix(RotMatrix);
			VECTOR3 rotation;
			rotation=payload[current].Rotation;
			RotMatrix_Def=mul(RotMatrix,RotationMatrix(rotation));
			vs.arot.x=atan2(RotMatrix_Def.m23,RotMatrix_Def.m33);
			vs.arot.y=-asin(RotMatrix_Def.m13);
			vs.arot.z=atan2(RotMatrix_Def.m12,RotMatrix_Def.m11);
			}
				v->clbkSetStateEx(&vs);
				if(currentPayload+1==Misc.Focus){
				oapiSetFocusObject(live);
					}
				}
			}
		sprintf(logbuff,"%s: Payload n.%i jettisoned name: %s @%.3f",GetName(),current+1,payload[current].name,MET);
		oapiWriteLog(logbuff);
		
		break;
	case TFAIRING:

		int ii;
		for(ii=1;ii<fairing.N+1;ii++){
					
		GetMeshOffset(fairing.msh_idh[ii],ofs);

		//double vro=length(fairing.speed);
		//vel =_V(vro*cos(fairing.angle*RAD+(ii-1)*2*PI/fairing.N),vro*sin(fairing.angle*RAD+(ii-1)*2*PI/fairing.N),fairing.speed.z);
		vel=RotateVecZ(fairing.speed,fairing.angle*RAD+(ii-1)*2*PI/fairing.N);
		
		Local2Rel (ofs, vs.rpos);
		GlobalRot(vel,rofs);
		vs.rvel.x=rvel.x+rofs.x;
		vs.rvel.y=rvel.y+rofs.y;
		vs.rvel.z=rvel.z+rofs.z;

		double arg=(ii-1)*2*PI/fairing.N;
		vs.vrot.x=vrot.x+fairing.rot_speed.x*cos(arg)-fairing.rot_speed.y*sin(arg);
		vs.vrot.y=vrot.y+fairing.rot_speed.x*sin(arg)+fairing.rot_speed.y*cos(arg);
		vs.vrot.z=vrot.z+fairing.rot_speed.z;

		char mn2[32];
		sprintf(mn2,"_%i",ii);
		strcpy_s(mn,fairing.meshname);
		strcat_s(mn,mn2);

		oapiCreateVesselEx(mn,fairing.module, &vs);
		sprintf(logbuff,"%s: Fairing jettisoned: name %s @%.3f",GetName(),mn,MET);
		oapiWriteLog(logbuff);
		}

		break;
case TINTERSTAGE:

	GetMeshOffset(stage[current].interstage.msh_idh,ofs);

		vel =_V(stage[current].interstage.speed.x,stage[current].interstage.speed.y,stage[current].interstage.speed.z);
		Local2Rel (ofs, vs.rpos);
		GlobalRot(vel,rofs);
		vs.rvel.x=rvel.x+rofs.x;
		vs.rvel.y=rvel.y+rofs.y;
		vs.rvel.z=rvel.z+rofs.z;
		vs.vrot.x=vrot.x+stage[current].interstage.rot_speed.x;
		vs.vrot.y=vrot.y+stage[current].interstage.rot_speed.y;
		vs.vrot.z=vrot.z+stage[current].interstage.rot_speed.z;

		strcpy_s(mn,stage[current].interstage.meshname);

		oapiCreateVesselEx(mn,stage[current].interstage.module, &vs);
		sprintf(logbuff,"%s: Interstage of stage %i jettisoned name: %s @%.3f",GetName(),current+1,mn,MET);
		oapiWriteLog(logbuff);
		
		break;
case TLES:
		
		GetMeshOffset(Les.msh_idh,ofs);

		vel =_V(Les.speed.x,Les.speed.y,Les.speed.z);
		Local2Rel (ofs, vs.rpos);
		GlobalRot(vel,rofs);
		vs.rvel.x=rvel.x+rofs.x;
		vs.rvel.y=rvel.y+rofs.y;
		vs.rvel.z=rvel.z+rofs.z;
		vs.vrot.x=vrot.x+Les.rot_speed.x;
		vs.vrot.y=vrot.y+Les.rot_speed.y;
		vs.vrot.z=vrot.z+Les.rot_speed.z;

		strcpy_s(mn,Les.meshname);

		oapiCreateVesselEx(mn,Les.module, &vs);
		sprintf(logbuff,"%s: Les jettisoned name: %s @%.3f",GetName(),mn,MET);
		oapiWriteLog(logbuff);
	break;


	}
return;
}

//Jettison item
void Multistage2015::Jettison(int type,int current){
	switch(type){
	case TBOOSTER:
		Spawn(type,current);
		
		int i;
		for(i=1;i<booster[current].N+1;i++){
		DelMesh(booster[current].msh_idh[i]);
		}

		//DelThruster(booster[current].th_booster_h[0]);
		DelThrusterGroup(booster[current].Thg_boosters_h, false);
		
		DelPropellantResource(booster[current].tank);
		currentBooster+=1;
		
		UpdateMass();
		UpdatePMI();

		if(currentBooster>=nBoosters){wBoosters=FALSE;}
	break;

	case TSTAGE:
		
		Spawn(type,current);
		
		
		DelMesh(stage[current].msh_idh);
		ClearThrusterDefinitions();
		//DelThrusterGroup(thg_h_main,TRUE);
		//DelThruster(stage[current].th_main_h[0]);
		DelPropellantResource(stage[current].tank);
		currentStage+=1;
		
		UpdateMass();
		UpdatePMI();
		CreateUllageAndBolts();
		CreateMainThruster();
		CreateRCS();
		ShiftCG(_V(0,0,(stage[current+1].off.z-stage[current].off.z)));
		
		
		SetCameraOffset(_V(0,0,0));

		break;

	case TPAYLOAD:
		
		Spawn(type,current);
		
		if(!payload[current].live){
		for(int ss=0;ss<payload[current].nMeshes;ss++){
		//	sprintf(logbuff,"deleting payload[%i] mesh number %i",current,ss);
			//oapiWriteLog(logbuff);
			DelMesh(payload[current].msh_idh[ss]);}
		}
		currentPayload+=1;
		UpdateMass();
		UpdatePMI();

		break;
	case TFAIRING:
		Spawn(type,current);
		int pns;
		for(pns=currentPayload;pns<nPayloads;pns++){
			if(!payload[pns].live){
			for(int s=0;s<payload[pns].nMeshes;s++){
				SetMeshVisibilityMode(payload[pns].msh_idh[s],MESHVIS_EXTERNAL);
				}
			}
		}
		int ii;
		for(ii=1;ii<fairing.N+1;ii++){
		DelMesh(fairing.msh_idh[ii]);
		}

		
		wFairing=0;
		UpdateMass();
		UpdatePMI();
		break;
	case TLES:
		Spawn(type,current);
		DelMesh(Les.msh_idh);
		wLes=FALSE;
		UpdateMass();
		UpdatePMI();
		break;
case TINTERSTAGE:
	Spawn(type,current);
	DelMesh(stage[current].interstage.msh_idh);
		currentInterstage+=1;
		stage[current].wInter=FALSE;
		UpdateMass();
		UpdatePMI();
	break;

	}
return;
}

//initialize correctly the delays if flying from a scenario not starting on ground
void Multistage2015::InitializeDelays(){

	//initialize booster burn delays, only if stage is 0 and if Met is positive
	if((currentStage==0)&&(MET>0)){
		for(int kb=currentBooster;kb<nBoosters;kb++){
			if(booster[kb].currDelay>0){
				booster[kb].currDelay-=MET;
			}
		}
	}

	//initialize stages using already used by vinka STAGE_IGNITION_TIME even if it should be called "STAGE_JETTISONED_TIME"
	else if(currentStage>0){
		double delta=MET-stage_ignition_time;
		if(delta<stage[currentStage].currDelay){
		stage[currentStage].currDelay-=delta;
		}

	}
return;
}


void Multistage2015::AutoJettison(){
	
	if(currentBooster<nBoosters){
		
		if(GetPropellantMass(booster[currentBooster].tank)<=0.000001){
				Jettison(TBOOSTER,currentBooster);
			
		}
	}

	if(currentStage<nStages-1){
		if((currentStage==0)&&(currentBooster<nBoosters)){return;}
		else if(GetPropellantMass(stage[currentStage].tank)<=0.1){
			Jettison(TSTAGE,currentStage);
			
		}
	
	}
	if((stage[currentStage].wInter==TRUE)&&(stage[currentStage].interstage.currDelay<=0)){
		Jettison(TINTERSTAGE,currentStage);
	}
	return;
}
void Multistage2015::Guidance_Debug(){
	int step=VinkaGetStep(MET);
	double DesiredPitch;
	if(Gnc_step[step].GNC_Comand==CM_ROLL){
	DesiredPitch=(Gnc_step[step].val_init+(Gnc_step[step].val_fin-Gnc_step[step].val_init)*(MET-Gnc_step[step].time_init)/((Gnc_step[VinkaFindFirstPitch()].time-1)-Gnc_step[step].time_init))*RAD;//88*RAD;
	double heading;
	oapiGetHeading(GetHandle(),&heading);
	sprintf(oapiDebugString(),"MET: %.1f Step: %i P: %.2f (%.2f) H: %.2f (%.2f)",MET,step,GetPitch()*DEG,DesiredPitch*DEG,heading*DEG,VinkaAzimuth*DEG);
	}else if(Gnc_step[step].GNC_Comand==CM_PITCH){
	DesiredPitch=(Gnc_step[step].val_init+(Gnc_step[step].val_fin-Gnc_step[step].val_init)*(MET-Gnc_step[step].time_init)/(Gnc_step[step].time_fin-Gnc_step[step].time_init))*RAD;
	sprintf(oapiDebugString(),"MET: %.1f Step: %i P: %.2f (%.2f) Delta: %.1f",MET,step,GetPitch()*DEG,DesiredPitch*DEG,GetPitch()*DEG-DesiredPitch*DEG);	
	}else{
	sprintf(oapiDebugString(),"MET: %.1f Step: %i",MET,step);	
	}
	


		

	
	return;
}

void Multistage2015::ComplexFlight()
{
	UpdateComplex+=oapiGetSimStep();
	if(UpdateComplex>=1)
	{
		UpdateComplex=0;

		for(int i=0;i<stage[currentStage].nEngines;i++)
		{
			
			double newMax=(stage[currentStage].thrust/stage[currentStage].nEngines)*(1+(stage[currentStage].engine_amp[i]*sin(2*PI/stage[currentStage].freq[i]*MET+stage[currentStage].engine_phase[i])));
			SetThrusterMax0(stage[currentStage].th_main_h[i],newMax);
			/*double test=(1+stage[currentStage].engine_amp[i]*sin(2*PI/60*MET+stage[currentStage].engine_phase[i]));
			sprintf(logbuff,"MET:%.1f engine:%i TEST:%.6f",MET,i,test);
			oapiWriteLog(logbuff);*/
		}
		if(wBoosters)
		{
			for(int j=0;j<booster[currentBooster].N;j++)
			{
				double newMax=booster[currentBooster].thrust*(1+(booster[currentBooster].engine_amp[j]*sin(2*PI/booster[currentBooster].freq[j]*MET+booster[currentBooster].engine_phase[j])));
				SetThrusterMax0(booster[currentBooster].th_booster_h[j],newMax);
			}
		}
	}


	return;
}

void Multistage2015::Boiloff()
{
	updtboiloff+=oapiGetSimStep();

	if(updtboiloff>=3600)
	{
		updtboiloff=0;
		for(int i=currentStage;i<nStages;i++)
		{
			if(stage[i].wBoiloff)
			{
				double propmass=GetPropellantMass(stage[i].tank);
				propmass-=1;
				SetPropellantMass(stage[i].tank,propmass);
		
			}
		}
	}
	return;
}

void Multistage2015::FLY(double simtime, double simdtime, double mjdate){
	if(APstat){
	if(stage[currentStage].currDelay>0){ stage[currentStage].currDelay-=simdtime; stage[currentStage].StageState=STAGE_WAITING; }
	if(stage[currentStage].interstage.currDelay>0) {stage[currentStage].interstage.currDelay-=simdtime;}
	}
	if(wBoosters){for(int nb=0;nb<nBoosters;nb++){
	if(booster[nb].currDelay>0){ booster[nb].currDelay-=simdtime;
	}}}
	if(!AJdisabled){AutoJettison();}
	double Level[10]; 
	double btime[10];

	if((!stage[currentStage].Ignited)&&(stage[currentStage].currDelay<=0)&&(stage[currentStage].StageState==STAGE_WAITING)){
		SetThrusterGroupLevel(THGROUP_MAIN,1);
		stage[currentStage].Ignited=TRUE;
		stage[currentStage].IgnitionTime=MET;
		stage[currentStage].StageState=STAGE_IGNITED;
		sprintf(logbuff,"%s Stage n: %i ignited @%.1f",GetName(),currentStage+1,stage[currentStage].IgnitionTime);
		oapiWriteLog(logbuff);
	}
	//MET+=simdtime;


	//BOOSTERS SECTION
	if((wBoosters)&&(GetThrusterGroupLevel(THGROUP_MAIN)>0.95)){
	
	int kb;
	for(kb=currentBooster;kb<nBoosters;kb++){
		if(booster[kb].currDelay<=0){
			if(booster[kb].Ignited==FALSE){
			booster[kb].Ignited=TRUE;
			booster[kb].IgnitionTime=MET;
			
			sprintf(logbuff,"%s Booster n: %i ignited @%.1f",GetName(),kb+1,booster[kb].IgnitionTime);
			oapiWriteLog(logbuff);
			}else{

			btime[kb]=MET-booster[kb].IgnitionTime;
			Level[kb]=1;
			double m,q;
			int qq;
			for(qq=0;qq<10;qq++){
					
			if(btime[kb]>booster[kb].curve[qq].x){
					if(qq<9){
					
				if(btime[kb]<booster[kb].curve[qq+1].x){
					
			m=(booster[kb].curve[qq+1].y-booster[kb].curve[qq].y)/(booster[kb].curve[qq+1].x-booster[kb].curve[qq].x);
			q=booster[kb].curve[qq].y-m*booster[kb].curve[qq].x;
			Level[kb]=(m*btime[kb]+q)/100;
													}
												
				}else{
				m=(booster[kb].curve[qq].y-booster[kb].curve[qq].y)/(booster[kb].curve[qq].x-booster[kb].curve[qq].x);
				q=booster[kb].curve[qq].y-m*booster[kb].curve[qq].x;
				Level[kb]=(m*btime[kb]+q)/100;
				
					}
				}				
				}
				}
			
			
			//SetThrusterLevel(booster[kb].th_booster_h[0],Level[kb]);
			//SetThrusterGroupLevel(booster[kb].Thg_boosters_h,Level[kb]);
			SetThrusterGroupLevel(booster[kb].Thg_boosters_h,Level[kb]);
			
			
		}
			}
	}
	



	//if((APstat)&&(wVinkasGuidance)){
	if(APstat){
		//MET+=simdtime;
	/*	VinkaAutoPilot();
		if(Misc.GNC_Debug==1){Guidance_Debug();}				
			if(MET>VinkaFindEndTime()){
				killAP();
				APstat=100;
				}*/
		
		VinkaAutoPilot();
		if(Misc.GNC_Debug==1){Guidance_Debug();}				
		if(MET>VinkaFindEndTime()){
			killAP();
			APstat=FALSE;
					}

		}

	//avoid reignition of not reignitable stages by setting thurst to 0
	if((!stage[currentStage].reignitable)&&(stage[currentStage].Ignited))
	{
		
		if(GetThrusterGroupLevel(THGROUP_MAIN)==0){
			stage[currentStage].waitforreignition+=simdtime;
			if(stage[currentStage].waitforreignition>=3){	
				for(int i=0;i<stage[currentStage].nEngines;i++)
				{
					SetThrusterResource(stage[currentStage].th_main_h[i],NULL);
				}
				stage[currentStage].DenyIgnition=TRUE;
			
			}
		}
	}
	if(tlmidx<TLMSECS){Telemetry();}

	if(stage[currentStage].batteries.wBatts)
	{
		stage[currentStage].batteries.CurrentCharge-=oapiGetSimStep();
		if(stage[currentStage].batteries.CurrentCharge<=0){
			stage[currentStage].batteries.CurrentCharge=0;
			ClearThrusterDefinitions();
		}
	}

		if((Complex)&&(GetDrag()>1000))
	{
		ComplexFlight();
		AddForce(_V(0,2*GetDrag()*sin(GetAOA()),0),_V(0,0,TotalHeight));
		AddForce(_V(2*GetDrag()*sin(GetSlipAngle()),0,0),_V(0,0,TotalHeight));
		if(GetDrag()>500000){if((abs(GetAOA())>45*RAD)||(abs(GetSlipAngle())>45*RAD)){boom();}}

		
	}
	
	if(stage[currentStage].ullage.wUllage)
	{
		if((!stage[currentStage].ullage.ignited)&&(stage[currentStage].currDelay<stage[currentStage].ullage.anticipation))//&&(MET-stage[currentStage].IgnitionTime<stage[currentStage].ullage.overlap))
		{
			SetThrusterLevel(stage[currentStage].ullage.th_ullage,1);
			stage[currentStage].ullage.ignited=TRUE;
		}else if((stage[currentStage].ullage.ignited)&&(stage[currentStage].IgnitionTime!=0)&&(MET-stage[currentStage].IgnitionTime>stage[currentStage].ullage.overlap)){

			SetThrusterLevel(stage[currentStage].ullage.th_ullage,0);
		}
	}

	if(stage[currentStage].expbolt.wExpbolt)
	{
		if(RemBurnTime(currentStage)<stage[currentStage].expbolt.anticipation)
		{
			SetThrusterLevel(stage[currentStage].expbolt.threxp_h,1);
		}
	}


	if(booster[currentBooster].expbolt.wExpbolt)
	{
		if(BoosterRemBurnTime(currentBooster)<booster[currentBooster].expbolt.anticipation)
		{
			SetThrusterLevel(booster[currentBooster].expbolt.threxp_h,1);
		}
	}

//	Boiloff();

/*	if((Configuration==0)&&(GetThrusterGroupLevel(THGROUP_MAIN)>0))
	{
		SetAngularVel(_V(0,0,0));
	}
*/
	MET+=simdtime;
return;
}
double Multistage2015::CalculateFullMass()
{double FM=0;
for(int i=0;i<nStages;i++)
{
	FM+=stage[i].emptymass;
	FM+=stage[i].fuelmass;
	if(stage[i].wInter){FM+=stage[i].interstage.emptymass;}
}
for(int j=0;j<nPayloads;j++)
{
	FM+=payload[j].mass;
}
for(int q=0;q<nBoosters;q++)
{
	FM+=booster[q].fuelmass*booster[q].N;
	FM+=booster[q].emptymass*booster[q].N;
}
if(hasFairing)
{
	FM+=fairing.emptymass;
}
if(wAdapter)
{
	FM+=Adapter.emptymass;
}
if(wLes)
{
	FM+=Les.emptymass;
}

return FM;
}

//returns hours mins and secs of a time (positive and negative)
VECTOR3 Multistage2015::hms(double time){
	VECTOR3 met;
	if(time==0){met=_V(0,0,0);}else{
	time=abs(time+0.5*(time/abs(time)-1));
	met.x=floor(time/3600)-0.5*(time/abs(time)-1);
	met.y=floor((time-met.x*3600*(time/abs(time)))/60)-0.5*(time/abs(time)-1);
	met.z=floor(time-met.x*3600*(time/abs(time))-met.y*60*(time/abs(time)));}
	return met;
}

double Multistage2015::GetProperNforCGTE(double time){
	double n;
	double Thrust=stage[0].thrust;
	double BoosterFlow=0;
	double BoosterFuelMassBurnt=0;
	for(int i=0;i<nBoosters;i++){
		if((booster[i].burndelay<time)&&(time<(booster[i].burndelay+booster[i].burntime))){
		Thrust+=booster[i].thrust*booster[i].N;
	BoosterFlow=((booster[i].fuelmass*booster[i].N)/booster[i].burntime);
	BoosterFuelMassBurnt=BoosterFlow*(time-booster[i].burndelay);
		}}
	double mass=CalculateFullMass();
	double FirstStageFlow=stage[0].fuelmass/stage[0].burntime;

//	Thrust*=0.99; //account for Drag

	double substr=time*(FirstStageFlow)+BoosterFuelMassBurnt; //Grande approssimazione
	mass-=substr;
	n=Thrust/(mass*g0);
	
	//sprintf(logbuff,"Thrust %.3f fsflow %.3f boostersflow %.3f mass %.3f n %.3f",Thrust,FirstStageFlow,BoostersFlow,mass,n);
	//oapiWriteLog(logbuff);
	return n;
}
bool Multistage2015::CGTE(double psi0){
	double t0,v0,x0,y0;
  double deltaPsi0,z0,C,psi,z,v,deltaT,deltax,deltay,x,y;
	//t0=3;
	
	double Thrust=stage[0].thrust;
	double BoostersFlow=0;
	for(int i=0;i<nBoosters;i++){
		if(booster[i].burndelay==0){  //accounting for boosters not immediatly ignited
			Thrust+=booster[i].thrust*booster[i].N;}
	}
	 double mass=CalculateFullMass();//GetMassAtStage(0,FALSE);
	 
	 x0=0;
	 


	/* y0=altsteps[1];
	 t0=sqrt((2*altsteps[1])/((Thrust/mass)-g0));
	 v0=t0*((Thrust/mass)-g0);*/
	 y0=Misc.COG;
	 t0=0;
	 v0=0;
	 double dtt=0.1;
	 while((y0<altsteps[1])&&(y0>0))
	 {
		 double acceleration=GetProperNforCGTE(t0)*g0;
		 y0+=0.5*(acceleration-g0)*dtt*dtt+v0*dtt;
		 t0+=dtt;
		 v0+=(acceleration-g0)*dtt;
	
	 }

	
//	 gt_pitch_table[gt_step].Gtpitch=psi0;
	
	 double hvel,vvel,vel,absacc,hacc,vacc,modspost,normx,normy;
		hvel=0;
		vvel=v0;
		vel=v0;
		deltaT=1;





	 while((t0<500)&&(psi0<80*RAD)){ 

		 /// OFFICIAL METHOD
	/*	 deltaPsi0=psi0/1000;
		 double n=GetProperNforCGTE(t0);
		 z0=tan(0.5*psi0);
		 C=v0/((pow(z0,n-1))*(1+z0*z0));
		 psi=(psi0+deltaPsi0);
		 z=tan(0.5*psi);
		 v=C*(pow(z,n-1))*(1+z*z);
		 deltaT=C/g0*((pow(z,n-1))*(1/(n-1)+(z*z)/(n+1))-(pow(z0,n-1))*(1/(n-1)+(z0*z0)/(n+1)));
		 deltax=0.5*deltaT*(v0*sin(psi0)+v*sin(psi));
		 deltay=0.5*deltaT*(v0*cos(psi0)+v*cos(psi));
		 x=x0+deltax;
		 y=y0+deltay;
		 x0=x;
		 y0=y;
		 v0=v;
		 psi0=psi;
		 t0+=deltaT;*/
		/// OFFICIAL METHOD FINISHES HERE!!
		
		 //// MY METHOD
		 
			double grel= mu/((rt+y0)*(rt+y0))-(vel*vel/(rt+y0));
		 	double n=GetProperNforCGTE(t0);
			absacc=n*g0;
			hacc=absacc*sin(psi0);
			vacc=absacc*cos(psi0)-grel;
			x=x0+hvel*deltaT+0.5*hacc*deltaT*deltaT;
			y=y0+vvel*deltaT+0.5*vacc*deltaT*deltaT;
			hvel+=hacc*deltaT;
			vvel+=vacc*deltaT;
			//v=v0+absacc*deltaT;
			vel=sqrt(hvel*hvel+vvel*vvel);
			t0+=deltaT;
			deltax=x-x0;
			deltay=y-y0;
			x0=x;
			y0=y;
			
			v0=vel;
            modspost=sqrt(deltax*deltax+deltay*deltay);
			normx=deltax/modspost;
			normy=deltay/modspost;
			psi=0.5*PI-atan2(normy,normx);
			if(psi>psi0){ 
				psi0=psi;
			}

		
		//sprintf(logbuff,"x:%.0f,y:%.0f,v:%.0f,t:%.0f,psi:%.0f",x,y,v,t0,psi0*DEG);
	//	sprintf(logbuff,"%.3f",vel);
		//oapiWriteLog(logbuff);
		 ///// MY METHOD FINISHES HERE!!

			/*
		 gt_pitch_table[gt_step].Pitch=psi0;
		 gt_pitch_table[gt_step].Altitude=y0;
		 gt_pitch_table[gt_step].Downrange=x0;
		 gt_pitch_table[gt_step].Speed=v0;
		 gt_pitch_table[gt_step].Time=t0;
		 */
	
		/* sprintf(logbuff,"t0 %.3f psi0 %.3f v %.3f x %.3f y %.3f deltaT %.3f",t0,psi0*DEG,v,x,y,deltaT);
		 oapiWriteLog(logbuff);*/
		 if(y0>altsteps[3]){return true;}
	 }
		

	 return false;
}

void Multistage2015::CheckForAdditionalThrust(int pns){
	if(GetAttachmentStatus(live_a[pns])){
		OBJHANDLE live=GetAttachmentStatus(live_a[pns]);
		VESSEL4 *v;
		v=static_cast<VESSEL4*>(oapiGetVesselInterface(live));
		VECTOR3 TotalThrustVecPL;;
		v->GetThrustVector(TotalThrustVecPL);
		TotalThrustVecPL=mul(RotationMatrix(payload[pns].Rotation),TotalThrustVecPL);
		AddForce(TotalThrustVecPL,payload[pns].off[0]);
		}
}

void Multistage2015::CheckForFX(int fxtype,double param)
{
	
	switch(fxtype){
	case FXMACH:
	if((param>FX_Mach.mach_min)&&(param<FX_Mach.mach_max))
		{
			PARTICLE ps = GetProperPS(FX_Mach.pstream);
			if(!FX_Mach.added)
			{
			FX_Mach.added=TRUE;
			for(int nmach=0;nmach<FX_Mach.nmach;nmach++)
			FX_Mach.ps_h[nmach]=AddParticleStream(&ps.Pss,FX_Mach.off[nmach],FX_Mach.dir,&lvl);
			}
		}else{
			if(FX_Mach.added==TRUE){
			for(int nmach=0;nmach<FX_Mach.nmach;nmach++)
			{
				DelExhaustStream(FX_Mach.ps_h[nmach]);
			}
			FX_Mach.added=FALSE;
			}
		}
	break;
	case FXVENT:
		for(int fv=1;fv<=FX_Vent.nVent;fv++)
		{
			if(param<FX_Vent.time_fin[fv])
			{				
				if(!FX_Vent.added[fv])
				{
					PARTICLE ps = GetProperPS(FX_Vent.pstream);		
					FX_Vent.ps_h[fv]=AddParticleStream(&ps.Pss,FX_Vent.off[fv],FX_Vent.dir[fv],&lvl);
					FX_Vent.added[fv]=TRUE;
					sprintf(logbuff,"Venting Effect Added @: %.3f,%.3f,%.3f dir: %.3f,%.3f,%.3f",FX_Vent.off[fv].x,FX_Vent.off[fv].y,FX_Vent.off[fv].z,FX_Vent.dir[fv].x,FX_Vent.dir[fv].y,FX_Vent.dir[fv].z);
					oapiWriteLog(logbuff);
				}
			}else{
				if(FX_Vent.added[fv]==TRUE)
				{
					DelExhaustStream(FX_Vent.ps_h[fv]);
					FX_Vent.added[fv]=FALSE;
				}
			}
		}
		break;
	}
}

void Multistage2015::EvaluateComplexFlight()
{
	srand((unsigned)time(NULL));
	
	for(int i=0;i<nStages;i++)
	{
		for(int q=0;q<stage[i].nEngines;q++)
		{
			int amplitude=rand()%1500;
			stage[i].engine_amp[q]=(double)amplitude/100000;
			int transval=rand()%180;
			stage[i].engine_phase[q]=(double)transval*RAD;
			int frequency=rand()%60;
			stage[i].freq[q]=30+(double)frequency;
			sprintf(logbuff,"%s Complex Flight-> Stage %i Engine %i Amplitude %.3f phase %.3f frequency %.3f",GetName(),i+1,q+1,stage[i].engine_amp[q],stage[i].engine_phase[q],stage[i].freq[q]);
			oapiWriteLog(logbuff);
		}
	}

	for(int j=0;j<nBoosters;j++)
	{
		for(int z=0;z<booster[j].N;z++)
		{
			int amplitude=rand()%1500;
			booster[j].engine_amp[z]=(double)amplitude/100000;
			int transval=rand()%180;
			booster[j].engine_phase[z]=(double)transval*RAD;
			int frequency=rand()%60;
			booster[j].freq[z]=30+(double)frequency;
			sprintf(logbuff,"%s Complex Flight-> Booster %i Engine %i Amplitude %.3f phase %.3f frequency %.3f",GetName(),j+1,z+1,booster[j].engine_amp[z],booster[j].engine_phase[z],booster[j].freq[z]);
			oapiWriteLog(logbuff);
		}
	}

	
	return;
}

void Multistage2015::FailuresEvaluation()
{
	srand((unsigned)time(NULL));
	int check=rand()%1000;
	if(check<10*failureProbability)
	{
		timeOfFailure=rand()%300;
	}

	sprintf(logbuff,"%s: Failures->Probabilty: %i Number Extracted: %i Time Of Failure: %i",GetName(),failureProbability,check/10,timeOfFailure);
	oapiWriteLog(logbuff);




























}
/*
void Multistage2015::Decompose()
{
	for(int i=currentBooster;i<nBoosters;i++)
	{
		Spawn(TBOOSTER,i);
	}
	for(int j=currentStage;j<nStages;j++)
	{
		Spawn(TSTAGE,j);
	}
	for(int k=currentPayload;k<nPayloads;k++)
	{
		Spawn(TPAYLOAD,k);
	}
	if(wFairing)
	{
		Spawn(TFAIRING,0);
	}
	if(wLes)
	{
		Spawn(TLES,0);
	}
	oapiDeleteVessel(GetHandle());
	
	return;
}
*/

void Multistage2015::boom()
{
if((wRamp)||(HangarMode))
{
	DelAttachment(AttToRamp);
}
VESSELSTATUS2 vs;
memset(&vs,0,sizeof(vs));
vs.version=2;
GetStatusEx(&vs);

VECTOR3 ofs = _V(0,0,0);
VECTOR3 rofs, rvel = {vs.rvel.x, vs.rvel.y, vs.rvel.z};
VECTOR3 vel = {0,0,0};
Local2Rel (ofs, vs.rpos);
GlobalRot(vel,rofs);
vs.rvel.x=rvel.x+rofs.x;
vs.rvel.y=rvel.y+rofs.y;
vs.rvel.z=rvel.z+rofs.z;

OBJHANDLE hwreck;
hwreck=oapiCreateVesselEx("wreck","boom",&vs);
oapiSetFocusObject(hwreck);
oapiCameraScaleDist(20);
if(wRamp)
{
	oapiDeleteVessel(hramp);
}
oapiDeleteVessel(GetHandle());
return;
}
void Multistage2015::Failure()
{
	srand((unsigned)time(NULL));
	int check=rand()%1000;
	if((currentStage==0)&&(check<250))
	//if((currentStage==0)&&(check>0))
	{
			boom();
	}else{
		//double newlevel=stage[currentStage].thrust*(1-(1/(double)stage[currentStage].nEngines));
		
		int engineout=rand()%stage[currentStage].nEngines;
		//SetThrusterMax0(stage[currentStage].th_main_h[engineout],newlevel);
		
		//SetThrusterLevel(stage[currentStage].th_main_h[engineout],0);
		

		SetThrusterResource(stage[currentStage].th_main_h[engineout],NULL);
		
		//	DelExhaust(exhaustN[currentStage][engineout]);
		//sprintf(logbuff,"engineout:%i exhaustN:%i oldlevel %.1f newlevel %.1f",engineout,exhaustN[currentStage][engineout],stage[currentStage].thrust,newlevel);
		//oapiWriteLog(logbuff);
	}
}
bool Multistage2015::CheckForFailure(double met)
{
	
	if((floor(met)==timeOfFailure)&&(!failed))
	{
		sprintf(logbuff,"%s: FAILURE FAILURE FAILURE @%.0f",GetName(),met);
		oapiWriteLog(logbuff);
		failed=TRUE;
		return TRUE;
	}else{
		return FALSE;
	}
}
int Multistage2015::WriteTelemetryFile(int initline){
FILEHANDLE TlmFile;
char filenmbuff[MAXLEN];
sprintf(filenmbuff,"Multistage2015\\Telemetry\\%s_%.2f_TLM.txt",GetName(),oapiGetSysMJD());
char buffer[MAXLEN];
if(initline==0){
	TlmFile=oapiOpenFile(filenmbuff,FILE_OUT,CONFIG);
sprintf(buffer,"<--!Multistage 2015 Automatically Generated Telemetry File!-->");
oapiWriteLine(TlmFile,buffer);
sprintf(buffer,"MET,Altitude,Speed,Pitch,Thrust,Mass,V-Speed,Acceleration");
oapiWriteLine(TlmFile,buffer);

}else{
	TlmFile=oapiOpenFile(filenmbuff,FILE_APP,ROOT);
}


for(int i=initline;i<tlmidx;i++)
{
	sprintf(buffer,"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",tlmAlt[i].x,tlmAlt[i].y,tlmSpeed[i].y,tlmPitch[i].y,tlmThrust[i].y,tlmMass[i].y,tlmVv[i].y,tlmAcc[i].y);
	oapiWriteLine(TlmFile,buffer);
}
if(initline==0){oapiCloseFile(TlmFile,FILE_OUT);}else{oapiCloseFile(TlmFile,FILE_APP);}
	return tlmidx-1;
}
void Multistage2015::Telemetry()
{
	if((updtlm>=1)&&(MET<TLMSECS-1))
	{
		VECTOR3 ThrustVec;
		GetThrustVector(ThrustVec);

		tlmAlt[tlmidx].x=MET;
		tlmSpeed[tlmidx].x=MET;
		tlmPitch[tlmidx].x=MET;
		tlmThrust[tlmidx].x=MET;
		tlmMass[tlmidx].x=MET;
		tlmVv[tlmidx].x=MET;
		tlmAcc[tlmidx].x=MET;
		
		tlmAlt[tlmidx].y=GetAltitude();
		tlmSpeed[tlmidx].y=GetOS();
		tlmPitch[tlmidx].y=GetPitch()*DEG;
		tlmThrust[tlmidx].y=length(ThrustVec);
		tlmMass[tlmidx].y=GetMass();
		tlmVv[tlmidx].y=GetVPerp();
		tlmAcc[tlmidx].y=getabsacc();	

		tlmidx++;
		updtlm=0;
	}
	updtlm+=oapiGetSimStep();

	if(Misc.telemetry){	
	writetlmTimer+=oapiGetSimStep();
	if(writetlmTimer>=60)
	{
		writetlmTimer=0;
		tlmnlines=WriteTelemetryFile(tlmnlines);
	}
	}
	return;
}

void Multistage2015::CalculateAltSteps(double planetmass)
{
	double altref[4]={100,350,1400,35000};
	double earthmass=5.973698968*pow(10.0,24);//1000000000000
	for(int i=0;i<4;i++)
	{
		altsteps[i]=altref[i]*planetmass/earthmass;
		//sprintf(logbuff,"altstep[%i]=%.1f",i,altsteps[i]);
		//oapiWriteLog(logbuff);
	}
	return;
}

void Multistage2015::SetNewAltSteps(double newstep1, double newstep2, double newstep3, double newstep4)
{
	altsteps[0]=newstep1;
	altsteps[1]=newstep2;
	altsteps[2]=newstep3;
	altsteps[3]=newstep4;
	return;
}
void Multistage2015::ToggleComplexFlight()
{
	if(Complex){
	Complex=FALSE;
	}else{
	Complex=TRUE;
	EvaluateComplexFlight();
	}
	return;
}
//****************************CALLBACKS******************************************************************************
void Multistage2015::clbkSetClassCaps(FILEHANDLE cfg)
{
	 
	initGlobalVars();			// First of all let's initialize everything!
	



  SetSurfaceFrictionCoeff(0.7,0.7);  // to avoid bad vibrations while throttling up at launch
 SetCW(0.2,0.5,1.5,1.5);		// gotten from Vinka
 EnableTransponder(true);
 InitNavRadios(4);
 SetCameraOffset(_V(0,0,100));	//Empirical

 SetVisibilityLimit(0,0.002);
 SetAlbedoRGB(_V((float)255/255,(float)255/255,(float)255/255));
 SetGravityGradientDamping(0);
	
 SetMaxWheelbrakeForce(200000);


 //Default ParticleStreams Definitions:

 // DEF CONTRAIL
 char transfer[MAXLEN];
 sprintf(transfer,"Contrail");
 for(int k=0;k<MAXLEN;k++){ 
Particle[13].ParticleName[k]=transfer[k];}
  Particle[13].Pss.flags=0;
  Particle[13].Pss.srcsize=8;
  Particle[13].Pss.srcrate=5;
  Particle[13].Pss.v0=150;
  Particle[13].Pss.srcspread=0.3;
  Particle[13].Pss.lifetime=8;
  Particle[13].Pss.growthrate=4;
  Particle[13].Pss.atmslowdown=3;
  Particle[13].Pss.ltype=PARTICLESTREAMSPEC::EMISSIVE;
  Particle[13].Pss.levelmap=PARTICLESTREAMSPEC::LVL_PSQRT;
  Particle[13].Pss.lmin=0;
  Particle[13].Pss.lmax=0.5;
  Particle[13].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_PLOG;
  Particle[13].Pss.amin=1e-6;
  Particle[13].Pss.amax=0.1;
  Particle[13].Pss.tex=oapiRegisterParticleTexture (const_cast<char *>("Contrail1"));
	
  //DEF EXHAUST
   sprintf(transfer,"Exhaust");
 for(int q=0;q<MAXLEN;q++){ 
	Particle[14].ParticleName[q]=transfer[q];}
  Particle[14].Pss.flags=0;
  Particle[14].Pss.srcsize=4;
  Particle[14].Pss.srcrate=20;
  Particle[14].Pss.v0=150; //250
  Particle[14].Pss.srcspread=0.1;
  Particle[14].Pss.lifetime=0.3; //0.6
  Particle[14].Pss.growthrate=12; //20
  Particle[14].Pss.atmslowdown=2;
  Particle[14].Pss.ltype=PARTICLESTREAMSPEC::EMISSIVE;
  Particle[14].Pss.levelmap=PARTICLESTREAMSPEC::LVL_PSQRT;
  Particle[14].Pss.lmin=0;
  Particle[14].Pss.lmax=0.5;
  Particle[14].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_PLOG;
  Particle[14].Pss.amin=1e-6;
  Particle[14].Pss.amax=0.1;
  Particle[14].Pss.tex=oapiRegisterParticleTexture (const_cast<char *>("Contrail3"));

  //DEF EXHAUST
   sprintf(transfer,"Clear");
 for(int z=0;z<MAXLEN;z++){ 
	Particle[15].ParticleName[z]=transfer[z];}
  Particle[15].Pss.flags=0;
  Particle[15].Pss.srcsize=0;
  Particle[15].Pss.srcrate=0;
  Particle[15].Pss.v0=0; //250
  Particle[15].Pss.srcspread=0;
  Particle[15].Pss.lifetime=0; //0.6
  Particle[15].Pss.growthrate=0; //20
  Particle[15].Pss.atmslowdown=0;
  Particle[15].Pss.ltype=PARTICLESTREAMSPEC::DIFFUSE;
  Particle[15].Pss.levelmap=PARTICLESTREAMSPEC::LVL_FLAT;
  Particle[15].Pss.lmin=0;
  Particle[15].Pss.lmax=0;
  Particle[15].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_FLAT;
  Particle[15].Pss.amin=0;
  Particle[15].Pss.amax=0;
  
 // AttToRamp=CreateAttachment(TRUE,_V(0,0,0),_V(0,0,-1),_V(0,1,0),"pad",FALSE);
 
 
}
void Multistage2015::clbkLoadStateEx(FILEHANDLE scn,void *vs)
{
	sprintf(logbuff,"Multistage Version: %i",GetMSVersion());
	oapiWriteLog(logbuff);

	oapiWriteLog(const_cast<char *>("Load State Started"));
 char *line;
 double batt_trans=0;
 bool loadedbatts=FALSE;
 stepsloaded=FALSE;
// wVinkasGuidance=FALSE;
 





 while(oapiReadScenario_nextline(scn,line)){
	  if(!_strnicmp(line,"CONFIG_FILE",11)){
		 sscanf_s(line+11,"%s",fileini, (unsigned)sizeof(fileini));
	  
	  }else if(!_strnicmp(line,"MET",3)){
		 sscanf(line+3,"%lf",&MET);
		/* if((MET<0)||(MET>1000000)){
			 MET=0;
			}*/
	  
	  }else if(!_strnicmp(line,"GNC_RUN",7)){
		  sscanf(line+7,"%i",&Gnc_running);
		  if(Gnc_running==1){APstat=TRUE;}
	
	  }else if(!_strnicmp(line,"BATTERY",7)){
		  sscanf(line+7,"%lf",&batt_trans);
		  loadedbatts=TRUE;

	  }else if(!_strnicmp(line,"STAGE_IGNITION_TIME",19)){
		  sscanf(line+19,"%lf",&stage_ignition_time);

	  }else if(!_strnicmp(line,"FAILURE_PROB",12)){
		  sscanf(line+12,"%i",&failureProbability);

	  }else if(!_strnicmp(line,"GNC_AUTO_JETTISON",17)){
		  int AJVal;
		  sscanf(line+17,"%i",&AJVal);
		  if(AJVal==0){AJdisabled=TRUE;}

	  }else if(!_strnicmp(line,"GUIDANCE_FILE",13)){
		 sscanf_s(line+13,"%s", guidancefile, MAXLEN);
		// wVinkasGuidance=TRUE;
		 parseGuidanceFile(guidancefile);
		 if(Gnc_running!=1){
		 VinkaCheckInitialMet();
		 }
	  
	    }else if(!_strnicmp(line,"TELEMETRY_FILE",14)){
		 sscanf_s(line+14,"%s", tlmfile, MAXLEN);
		 parseTelemetryFile(tlmfile);
		 wReftlm=TRUE;

	 }else if(!_strnicmp(line,"CURRENT_BOOSTER",15)){
		 sscanf_s(line+15,"%i",&currentBooster);
		 currentBooster-=1;
		 
	  }else if(!_strnicmp(line,"CURRENT_INTERSTAGE",18)){
		 sscanf_s(line+18,"%i",&currentInterstage);
		//sprintf(logbuff,"current interstage:%i",currentInterstage);
		//oapiWriteLog(logbuff);
		 currentInterstage-=1;
		

	  }else if(!strnicmp(line,"CURRENT_STAGE",13)){
		 sscanf(line+13,"%i",&currentStage);
		 currentStage-=1;
		 if(currentStage>0){currentBooster=11;}
		// sprintf(logbuff,"interstage:%i stage:%i",currentInterstage,currentStage);
		 //oapiWriteLog(logbuff);
	
	}else if(!strnicmp(line,"STAGE_STATE",11)){
		  sscanf(line+11,"%i",&stage[currentStage].StageState);
		  if(stage[currentStage].StageState==STAGE_IGNITED){stage[currentStage].Ignited=TRUE;}
		 
	  }else if(!strnicmp(line,"CURRENT_PAYLOAD",15)){
		 sscanf(line+15,"%i",&currentPayload);
		 currentPayload-=1;

		 	 
	  }else if(!strnicmp(line,"FAIRING",7)){
		 sscanf(line+7,"%i",&wFairing);
		 if((wFairing<0)||(wFairing>1)){wFairing=0;}
		 	 	 
	  }else if(!strnicmp(line,"CONFIGURATION",13)){
		 sscanf(line+13,"%i",&Configuration);
		 if((Configuration<0)||(Configuration>1)){Configuration=0;}

	  }else if(!strnicmp(line,"COMPLEX",7)){
		 Complex=TRUE;

		}else if(!strnicmp(line,"HANGAR",6)){
		 HangarMode=TRUE;
		
	  }else if(!strnicmp(line,"CRAWLER",7)){
		 wCrawler=TRUE;
	
	  }else if(!strnicmp(line,"CAMERA",6)){
		  sscanf(line+6,"%lf %lf",&CamDLat,&CamDLng);
		  if((CamDLat==0)&&(CamDLng==0)){CamDLat=0.01;CamDLng=0.03;}
			wCamera=TRUE;

	//  }else if(!strnicmp(line,"GENERATE_PITCH_TABLE",20)){
		//  GeneratePT=TRUE;

		}else if(!strnicmp(line,"GROWING_PARTICLES",17)){
		 GrowingParticles=TRUE;

	  }else if(!strnicmp(line,"DENY_IGNITION",13)){
		  stage[currentStage].DenyIgnition=TRUE;
				
	  }else if(!strnicmp(line,"ALT_STEPS",9)){
		  sscanf(line+9,"%lf,%lf,%lf,%lf",&altsteps[0],&altsteps[1],&altsteps[2],&altsteps[3]);
		  stepsloaded=TRUE;

	  }else if(!strnicmp(line,"PEG_PITCH_LIMIT",15)){
		  sscanf(line+15,"%lf",&PegPitchLimit);
		  PegPitchLimit*=RAD;

	  }else if(!strnicmp(line,"PEG_MC_INTERVAL",15)){
		  sscanf(line+15,"%lf",&PegMajorCycleInterval);

	  }else if(!strnicmp(line,"RAMP",4)){
		  wRamp=TRUE;
	  }else if(!strnicmp(line,"ATTMSPAD",8)){
		  sscanf(line+8,"%lf",&MsPadZ.z);
		  AttToMSPad=TRUE;

 }else{



   ParseScenarioLineEx(line,vs);

	  }
 
 }
 


 
 char tempFile[MAXLEN];
 strcpy_s(tempFile,OrbiterRoot);
 strcat_s(tempFile,"\\");
 strcat_s(tempFile,fileini);
 sprintf(logbuff,"%s: Config File: %s",GetName(),tempFile);
 oapiWriteLog(logbuff);
 parseinifile(tempFile);
 
 //sprintf(logbuff,"currentInterstage:%i currentStage:%i csIncremental:%i nInterstages:%i",currentInterstage,currentStage,stage[currentStage].IntIncremental,nInterstages);
 //oapiWriteLog(logbuff);
 if((currentInterstage>currentStage)||(currentInterstage>nInterstages)||(currentInterstage>=stage[currentStage].IntIncremental)){stage[currentStage].wInter=FALSE; }
 
 if((wFairing==1)&&(hasFairing==FALSE)){wFairing=0;}
 if(Configuration==0){			//if only configuration is defined, reset everything
	 if(hasFairing==TRUE){
		  wFairing=1;}
	 currentStage=0;
	 currentPayload=0;
	 currentBooster=0;
	 currentInterstage=0;
 }

 
 UpdateOffsets();
 
 VehicleSetup();
 
 LoadMeshes();
 
 if(Gnc_running==1){
 InitializeDelays();
 }
 
 if(loadedbatts)
 {
	 stage[currentStage].batteries.CurrentCharge=batt_trans;
 }
 


 loadedCurrentBooster=currentBooster;
 loadedCurrentInterstage=currentInterstage;
 loadedCurrentStage=currentStage;
 loadedCurrentPayload=currentPayload;;
 loadedwFairing=wFairing;
 loadedConfiguration=Configuration;
 loadedMET=MET;
 loadedGrowing=GrowingParticles;

 

oapiWriteLog(const_cast<char *>("Load State Terminated")); 
}



int Multistage2015::clbkConsumeBufferedKey(DWORD key,bool down,char *kstate){

	if(!down) return 0;

	if(KEYDOWN(kstate,OAPI_KEY_J)&&(!KEYMOD_CONTROL(kstate))&&(!KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
	
		if(currentBooster<nBoosters){
			Jettison(TBOOSTER,currentBooster);
		}else{
		if(currentStage<nStages-1){
			if(stage[currentStage].wInter==TRUE){
				Jettison(TINTERSTAGE,currentStage);
			}else{	Jettison(TSTAGE,currentStage);}

		}else if((currentStage==nStages-1)&&(stage[currentStage].wInter)){
				Jettison(TINTERSTAGE,currentStage);
		}else{

		if((currentPayload<nPayloads)&&(wFairing==0)){
		Jettison(TPAYLOAD,currentPayload);
		}else{return 0;}
		}
		}
	
	return 1;
	}
	if(KEYDOWN(kstate,OAPI_KEY_F)&&(!KEYMOD_CONTROL(kstate))&&(!KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		if(wLes){
			Jettison(TLES,0);
		}else{
			if(wFairing==1){Jettison(TFAIRING,0);}else{return 0;}
			}
	return 1;	
	}

	if(KEYDOWN(kstate,OAPI_KEY_P)&&(!KEYMOD_CONTROL(kstate))&&(!KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		if(!APstat){APstat=TRUE;return 0;}else if(APstat){APstat=FALSE; return 0;}else{
	
			return 0;}
	return 1;
	}

if(KEYDOWN(kstate,OAPI_KEY_SPACE)&&(KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate))){
		if((!DeveloperMode)&&(!killDMD))
		{
			DeveloperMode=TRUE;
			note=oapiCreateAnnotation(TRUE,1,_V(1,0,0));
			oapiAnnotationSetPos(note,0.65,0.25,0.975,0.75);
			oapiAnnotationSetSize(note,0.75);
			char TXT[MAXLEN];
			sprintf(TXT,"%s: \nDEVELOPER MODE ON \n\n[SPACEBAR] to reload .ini file \n\n[CTRL]+[SPACEBAR] to close Developer Mode",GetName());
			oapiAnnotationSetText(note,TXT);

			//oapiOpenDialogEx(g_Param.hDLL, IDD_DIALOG1, DlgProc,0,this);
			CreateDMD();
		}else{
			
			killDMD=TRUE;
			DeveloperMode=FALSE;
			if(note){oapiDelAnnotation(note);}
			
		}
	return 1;
	}

	if(KEYDOWN(kstate,OAPI_KEY_SPACE)&&(!KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate))){
		if(DeveloperMode)
		{
			ResetVehicle(hangaranims,wRamp);
		}
	return 1;
	}
	
	
	if(KEYDOWN(kstate,OAPI_KEY_L)&&(KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate))))
	{
		if(HangarMode)
		{
				char kstate[256];
				for(int i=0;i<256;i++) kstate[i]=0x00;
				kstate[OAPI_KEY_L]=0x80;
				kstate[OAPI_KEY_LCONTROL]=0x80;
				vhangar->SendBufferedKey(OAPI_KEY_L,TRUE,kstate);
		return 1;
		}
	}
	if(KEYDOWN(kstate,OAPI_KEY_D)&&(KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate))))
	{
		if(HangarMode)
		{
				char kstate[256];
				for(int i=0;i<256;i++) kstate[i]=0x00;
				kstate[OAPI_KEY_D]=0x80;
				kstate[OAPI_KEY_LCONTROL]=0x80;
				vhangar->SendBufferedKey(OAPI_KEY_D,TRUE,kstate);
				
				if(!AttToMSPad){
				OBJHANDLE hPad=oapiGetObjectByName(const_cast<char *>("MS_Pad"));
				if(oapiIsVessel(hPad))
				{
					AttachToMSPad(hPad);
					AttToMSPad=TRUE;
				}
				}
		return 1;
		}
	}
	
	return 0;
}

void Multistage2015::AttachToMSPad(OBJHANDLE hPad)
{
	VESSEL4 *vPad;
	vPad = static_cast<VESSEL4*>(oapiGetVesselInterface(hPad));
	padramp = vPad->CreateAttachment(FALSE,_V(0,0,0),_V(0,1,0),_V(0,0,1),"Pad",FALSE);
	//AttToRamp=CreateAttachment(TRUE,_V(0,0,-stage[0].height*0.5-MSPadZ0),_V(0,0,-1),_V(0,1,0),"Pad",FALSE);
	AttToRamp=CreateAttachment(TRUE,MsPadZ,_V(0,0,-1),_V(0,1,0),"Pad",FALSE);
	
	vPad->AttachChild(GetHandle(),padramp,AttToRamp);
}

int Multistage2015::clbkConsumeDirectKey(char *kstate)
{
	if(HangarMode)
	{
		vhangar->clbkConsumeDirectKey(kstate);
	if(AttToMSPad){
		VECTOR3 pos,dir,rot;
		GetAttachmentParams(AttToRamp,pos,dir,rot);
		const double delta=0.1;
		if(KEYDOWN(kstate,OAPI_KEY_UP)&&(!KEYMOD_CONTROL(kstate))&&(KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		pos.z-=delta;
		SetAttachmentParams(AttToRamp,pos,dir,rot);
		}
		if(KEYDOWN(kstate,OAPI_KEY_DOWN)&&(!KEYMOD_CONTROL(kstate))&&(KEYMOD_SHIFT(kstate)&&(!KEYMOD_ALT(kstate)))){
		pos.z+=delta;
		SetAttachmentParams(AttToRamp,pos,dir,rot);
		}
	}
	}
	return 0;
}

void Multistage2015::clbkSaveState(FILEHANDLE scn)
{
	char savebuff[256],savevalbuff[256];
	if(HangarMode)
	{
		Configuration=0;
	}
	SaveDefaultState(scn);

	sprintf(savebuff,"CONFIG_FILE");
	sprintf(savevalbuff,"%s", fileini);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

  if(guidancefile[0]!='\0'){
	sprintf(savebuff,"GUIDANCE_FILE");
	sprintf(savevalbuff,"%s", guidancefile);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }

	sprintf(savebuff,"CONFIGURATION");
	sprintf(savevalbuff,"%i",Configuration);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);






  if(Complex){
  sprintf(savebuff,"COMPLEX");
  sprintf(savevalbuff,"0");
  oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }


   if(HangarMode){
  sprintf(savebuff,"HANGAR");
  sprintf(savevalbuff,"0");
  oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }

	sprintf(savebuff,"CURRENT_BOOSTER");
	sprintf(savevalbuff,"%i",currentBooster+1);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

    sprintf(savebuff,"CURRENT_STAGE");
	sprintf(savevalbuff,"%i",currentStage+1);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

    sprintf(savebuff,"CURRENT_INTERSTAGE");
	sprintf(savevalbuff,"%i",currentInterstage+1);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

    sprintf(savebuff,"CURRENT_PAYLOAD");
	sprintf(savevalbuff,"%i",currentPayload+1);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

    sprintf(savebuff,"FAIRING");
	sprintf(savevalbuff,"%i",wFairing);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

    sprintf(savebuff,"MET");
	sprintf(savevalbuff,"%.3f",MET);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

  if(APstat){
	  Gnc_running=1;
    sprintf(savebuff,"GNC_RUN");
	sprintf(savevalbuff,"%i",Gnc_running);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }

  if(stage[currentStage].batteries.wBatts)
  {
	  sprintf(savebuff,"BATTERY");
	  sprintf(savevalbuff,"%lf",stage[currentStage].batteries.CurrentCharge);
	  oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }

  if(stage[currentStage].DenyIgnition)
  {
	  sprintf(savebuff,"DENY_IGNITION");
	  sprintf(savevalbuff,"0");





	  oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }
if(GrowingParticles){
	sprintf(savebuff,"GROWING_PARTICLES");
	sprintf(savevalbuff,"0");
	oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }
  if(AJdisabled){
	  int val=1;
	sprintf(savebuff,"GNC_AUTO_JETTISON");
	sprintf(savevalbuff,"%i",val);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }

  sprintf(savebuff,"STAGE_IGNITION_TIME");
  sprintf(savevalbuff,"%.6f",stage_ignition_time);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

  sprintf(savebuff,"STAGE_STATE");
  sprintf(savevalbuff,"%i",stage[currentStage].StageState);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

  if(tlmfile[0]!='\0')
  {
	  sprintf(savebuff,"TELEMETRY_FILE");
	  sprintf(savevalbuff,"%s",tlmfile);
	  oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }

  sprintf(savebuff,"ALT_STEPS");
  sprintf(savevalbuff,"%.1f,%.1f,%.1f,%.1f",altsteps[0],altsteps[1],altsteps[2],altsteps[3]);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

  sprintf(savebuff,"PEG_PITCH_LIMIT");
  sprintf(savevalbuff,"%.3f",PegPitchLimit*DEG);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

  sprintf(savebuff,"PEG_MC_INTERVAL");
  sprintf(savevalbuff,"%.3f",PegMajorCycleInterval);
  oapiWriteScenario_string(scn,savebuff,savevalbuff);

  if(wRamp)
  {
	sprintf(savebuff,"RAMP");
	sprintf(savevalbuff,"0");
	oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }
  if((HangarMode)&&(AttToMSPad))
  {
	  VECTOR3 pos,dir,rot;
	  GetAttachmentParams(AttToRamp,pos,dir,rot);

	sprintf(savebuff,"ATTMSPAD");
	sprintf(savevalbuff,"%.3f",pos.z);
	oapiWriteScenario_string(scn,savebuff,savevalbuff);
  }

}

bool Multistage2015::CheckForDetach()
{
		VECTOR3 Thrust,horThrust;
		GetThrustVector(Thrust);
		double Mass=GetMass();
		HorizonRot(Thrust,horThrust);
		
		if(horThrust.y>Mass*9.81)
		{return TRUE;}else{return FALSE;}

}
void Multistage2015::clbkPreStep(double simt, double simdt, double mjd){

	if((APstat)&&(Configuration==0)){
		VinkaAutoPilot();
		MET+=simdt;
		stage[0].currDelay=-MET;
		if(Misc.GNC_Debug==1){Guidance_Debug();}				
	}
	

	
	if((wRamp)&&(!HangarMode))
	{
		
	if(CheckForDetach()){
			if(GetAttachmentStatus(AttToRamp)!=NULL){
			vramp->DetachChild(padramp,0);
			DelAttachment(AttToRamp);	
			//AttachedToRamp=FALSE;
			oapiWriteLog(const_cast<char *>("Detached from Launchpad"));
			}
			if((!wLaunchFX)||(GetAltitude()>FX_Launch.CutoffAltitude)){
				
			bool deleted=oapiDeleteVessel(hramp);
				if(deleted)
				{
					wRamp=FALSE;
					NoMoreRamp=TRUE;
					oapiWriteLog(const_cast<char *>("LaunchPad Deleted from Scenery"));
				}
			}
		}
	}else if((HangarMode)&&(AttToMSPad)){
		if(CheckForDetach()){	DelAttachment(AttToRamp);	
			AttToMSPad=FALSE;
			oapiWriteLog(const_cast<char *>("Detached from Launchpad"));
		}
	}




	if(Configuration==1) {FLY(simt,simdt,mjd);
	if(GrowingParticles){	ManageParticles(simdt,TRUE);}
	}
	if(wMach)
	{		
		CheckForFX(FXMACH,GetMachNumber());
	}
	if((wVent)&&(MET<=5))
	{
		CheckForFX(FXVENT,MET);
	}
	if(wFailures)
	{
		if(CheckForFailure(MET))
		{
			Failure();
		}
	}

	

	return;
}
void Multistage2015::clbkPostStep(double simt, double simdt, double mjd){
	

	/*
	THRUSTER_HANDLE th=CreateThruster(_V(0,0,0),_V(0,0,1),1);
	THRUSTER_HANDLE* th_pointer=&th;
	THRUSTER_HANDLE th_getthevalue=*th_pointer;
	VECTOR3 thdir;
	GetThrusterDir(*th_pointer,thdir);

	sprintf(oapiDebugString(),"X:%.1f Y:%.1f Z:%.1f",thdir.x,thdir.y,thdir.z);
	*/

//	if((APstat)&&(wVinkasGuidance)&&(Configuration==0)){//(MET<0)){




	//////Moved in PRE for pad boom
		/*if((APstat)&&(Configuration==0)){
		VinkaAutoPilot();
		MET+=simdt;
		stage[0].currDelay=-MET;
		if(Misc.GNC_Debug==1){Guidance_Debug();}				
	}*/
		
	
	if((GetThrusterGroupLevel(THGROUP_MAIN)>0.95)&&(Configuration==0)) {
		Configuration=1;
		MET=0;
	}













	//if((Configuration==1)) {FLY(simt,simdt,mjd);}
	
	/*
	VECTOR3 hhvec,vvvec,rrvec,hloc;
	oapiGetRelativePos(GetSurfaceRef(),GetHandle(),&rrvec);
	oapiGetRelativeVel(GetSurfaceRef(),GetHandle(),&vvvec);
	hhvec=crossp(rrvec,vvvec);
	MATRIX3 m3;
	oapiGetRotationMatrix(GetSurfaceRef(),&m3);
	hloc=tmul(m3,hhvec);
	normalise(hloc);
	double INC=acos(hloc.y)*DEG-90-90;
	
	sprintf(oapiDebugString(),"x: %.3f y:%.3f z:%.3f INC: %.2f tgtapo:%.1f",hloc.x,hloc.y,hloc.z,INC,tgtapo);
	*/

	for(int pns=currentPayload;pns<nPayloads;pns++)
	{
		if(payload[pns].live){CheckForAdditionalThrust(pns);}
	}

if((Configuration==1)&&(GrowingParticles)){
	ManageParticles(simdt,FALSE);
	}
		
		
th_main_level=GetThrusterGroupLevel(THGROUP_MAIN);

if(wLaunchFX){
	if(FX_Launch.CutoffAltitude>0){
		launchFx_level=(-1)/(FX_Launch.CutoffAltitude)*GetAltitude()+1;
		
		if(launchFx_level>=1){launchFx_level=1;}
		if(launchFx_level<=0){launchFx_level=0;}
	}else{
		launchFx_level=1;
	}
}else{launchFx_level=0;}
//Try.run();
launchFx_level*=th_main_level;
return;
}
bool Multistage2015::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp){
	VESSEL4::clbkDrawHUD (mode, hps, skp);
	int cx = hps->CX, cy = hps->CY;

				DWORD w;
				DWORD h;
				DWORD bpp;
				oapiGetViewportSize(&w,&h,&bpp);
				

			char NAMEbuff[512];
			memset(NAMEbuff, 0, sizeof(NAMEbuff));
			int NAMElen; 
			NAMElen=sprintf_s(NAMEbuff,"%s",GetName());
			skp->Text(0.02*w,0.25*h+20,NAMEbuff,NAMElen);


			char METbuff[512];
			memset(METbuff, 0, sizeof(METbuff));
			int METlen; 
			
			if(MET>=0){
				METlen=sprintf_s(METbuff,"MET: %03.0f:%02.0f:%02.0f",hms(MET));
			}else{
				METlen=sprintf_s(METbuff,"T-: %03.0f:%02.0f:%02.0f",hms(MET));
				}
			skp->Text(0.02*w,0.25*h+40,METbuff,METlen);

			char STAGEbuff[512];
			memset(STAGEbuff, 0, sizeof(STAGEbuff));
			int STAGElen; 
			
			STAGElen=sprintf_s(STAGEbuff,"Stage: %i",currentStage+1);
			skp->Text(0.02*w,0.25*h+60,STAGEbuff,STAGElen);

			char BTbuff[512];
			memset(BTbuff, 0, sizeof(BTbuff));
			int BTlen; 
			
			BTlen=sprintf_s(BTbuff,"Remaining Stage Burn Time: %02.0f:%02.0f",hms(RemBurnTime(currentStage)).y,hms(RemBurnTime(currentStage)).z);
			skp->Text(0.02*w,0.25*h+80,BTbuff,BTlen);
			
			if((runningPeg)&&(currentStage==NN-1)){

				if(GetAltitude()>altsteps[3])
				{
				char PMECObuff[512];
				memset(PMECObuff, 0, sizeof(PMECObuff));
				int PMECOlen; 
				VECTOR3 PMECO=hms(TMeco);
				PMECOlen=sprintf_s(PMECObuff,"Predicted MECO: %02.0f:%02.0f",PMECO.y,PMECO.z);
				skp->Text(0.02*w,0.25*h+100,PMECObuff,PMECOlen);
				}
			}

			
			char PLNamebuff[512];
			memset(PLNamebuff, 0, sizeof(PLNamebuff));
			int PLNamelen; 
			char PLWbuff[512];
			memset(PLWbuff, 0, sizeof(PLWbuff));
			int PLWlen; 
			
			double PLtotWeight=0;
			for(int i=currentPayload;i<nPayloads;i++)
			{
				PLtotWeight+=payload[i].mass;
			}
			PLNamelen=sprintf_s(PLNamebuff,"Next Payload: %s",payload[currentPayload].name);
			skp->Text(w-400,0.25*h+20,PLNamebuff,PLNamelen);
			PLWlen=sprintf_s(PLWbuff,"Total Payload Weight: %.1f kg",PLtotWeight);
			skp->Text(w-400,0.25*h+40,PLWbuff,PLWlen);


			

	return true;
}



























/*
void Multistage2015::GeneratePitchTable()
{
	gt_step=0;
		  double init_psi=21;
		while((init_psi>0))
		{	CGTE(init_psi*RAD); 
			init_psi-=1;
		gt_step++;}


		char filenmbuff[MAXLEN];
sprintf(filenmbuff,"Multistage2015\\PitchTables\\%s_%.2f_PT.txt",GetName(),oapiGetSysMJD());
FILEHANDLE gtPitchTableFile=oapiOpenFile(filenmbuff,FILE_OUT,CONFIG);
char buffer[MAXLEN];
sprintf(buffer,"Multistage 2015 Automatically Generated Pitch Table File");
oapiWriteLine(gtPitchTableFile,buffer);
sprintf(buffer,"Vehicle name:%s",GetName());
oapiWriteLine(gtPitchTableFile,buffer);
sprintf(buffer,"Initial Pitch[deg],Final Time[s],Final Altitude[m],Final Pitch[deg],Downrange[m],Final Velocity[m/s]");
oapiWriteLine(gtPitchTableFile,buffer);

	
		for(int q=0;q<22;q++)
	 {
		 sprintf(buffer,"%.2f,%.1f,%.1f,%.2f,%.1f,%.1f",90-gt_pitch_table[q].Gtpitch*DEG,gt_pitch_table[q].Time,gt_pitch_table[q].Altitude,90-gt_pitch_table[q].Pitch*DEG,gt_pitch_table[q].Downrange,gt_pitch_table[q].Speed);
		 oapiWriteLine(gtPitchTableFile,buffer);
	 }
oapiCloseFile(gtPitchTableFile,FILE_OUT);

}
*/
void Multistage2015::clbkPostCreation(){
	oapiWriteLog(const_cast<char *>("Post Creation Started"));
	UpdateLivePayloads();
	if(failureProbability>0){	wFailures=TRUE; FailuresEvaluation();}

/*

	MyID=ConnectToOrbiterSoundDLL(GetHandle());

	if(Ssound.Main){
		ReplaceStockSound(MyID,Ssound.MAIN_THRUST,	REPLACE_MAIN_THRUST);}
	if(Ssound.Hover){
		ReplaceStockSound(MyID,Ssound.HOVER_THRUST,	REPLACE_HOVER_THRUST);}
	if(Ssound.RCS_ta){
		ReplaceStockSound(MyID,Ssound.RCS_THRUST_ATTACK, REPLACE_RCS_THRUST_ATTACK);}
	if(Ssound.RCS_ts){
		ReplaceStockSound(MyID,Ssound.RCS_THRUST_SUSTAIN, REPLACE_RCS_THRUST_SUSTAIN);}
	if(Ssound.Ac){
		ReplaceStockSound(MyID,Ssound.AIR_CONDITIONNING, REPLACE_AIR_CONDITIONNING);}
	if(Ssound.Ca1){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_1, REPLACE_COCKPIT_AMBIENCE_1);}
	if(Ssound.Ca2){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_2, REPLACE_COCKPIT_AMBIENCE_2);}
	if(Ssound.Ca3){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_3, REPLACE_COCKPIT_AMBIENCE_3);}
	if(Ssound.Ca4){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_4, REPLACE_COCKPIT_AMBIENCE_4);}
	if(Ssound.Ca5){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_5, REPLACE_COCKPIT_AMBIENCE_5);}
	if(Ssound.Ca6){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_6, REPLACE_COCKPIT_AMBIENCE_6);}
	if(Ssound.Ca7){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_7, REPLACE_COCKPIT_AMBIENCE_7);}
	if(Ssound.Ca8){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_8, REPLACE_COCKPIT_AMBIENCE_8);}
	if(Ssound.Ca9){
		ReplaceStockSound(MyID,Ssound.COCKPIT_AMBIENCE_9, REPLACE_COCKPIT_AMBIENCE_9);}
 
 	 for(int s=0;s<57;s++){
		 if(Ssound.GncStepSound[s]!=-5){
			 RequestLoadVesselWave(MyID,Ssound.GncStepSound[s],Gnc_step[Ssound.GncStepSound[s]].trchar,RADIO_SOUND);
		 }
		}
 
	 */

	




	
//mu=398600441800000;
//rt=6371010;
double planetmass=oapiGetMass(GetGravityRef());
mu=GGRAV*planetmass;//oapiGetMass(GetGravityRef());
rt=oapiGetSize(GetGravityRef());
g0=mu/(rt*rt);
//sprintf(logbuff,"g0=%.3f",g0);
//oapiWriteLog(logbuff);
if(!stepsloaded){CalculateAltSteps(planetmass);}

	 //////COMPUTATIONAL GRAVITY TURN EVALUATION
	 if(Configuration==0)
	 {
		 

	 double init_psi=21;
		while((!CGTE(init_psi*RAD))&&(init_psi>0))
		{	 init_psi-=1; }
	 double safety=init_psi*0.05;//.2;
	 init_psi-=safety;
	 if(init_psi<1){init_psi=1;}
	 //double gtip;
	 //GT_InitPitch=(90-init_psi)*RAD;
	 GT_IP_Calculated=(90-init_psi)*RAD;
	 sprintf(logbuff,"%s: Gravity Turn Initial Pitch by user: %.3f Calculated:%.3f",GetName(),GT_InitPitch*DEG,GT_IP_Calculated*DEG);
	 oapiWriteLog(logbuff);

	 if(GT_InitPitch==0) { GT_InitPitch=GT_IP_Calculated; }

	//Ramp or Hangar
	 if(!HangarMode){	Ramp(wRamp);
	 }else{	
		 CreateHangar();
	 }
	
	 if(wCamera)
	 {
		 CreateCamera();
	 }

	 }

	
	


	 InitPEG();

	 if(Complex)
	 {
		 EvaluateComplexFlight();
	 }

 const ATMCONST *atmconst = oapiGetPlanetAtmConstants(GetSurfaceRef());
	 RefPressure=atmconst->p0;
	 double altlimit=atmconst->altlimit;
	 sprintf(logbuff,"%s: Planet Reference Pressure = %.1f Pa  Atmosphere Altitude Limit:%.1f km",GetName(),RefPressure,altlimit/1000);
	 oapiWriteLog(logbuff);
		
	 GetStatusEx(&vs2);
	vs2.status=0;
	GetRelativePos(GetSurfaceRef(),vs2.rpos);
	

	 oapiWriteLog(const_cast<char *>("Post Creation Terminated"));
	return;
}

void Multistage2015::CreateLaunchFX()
{
	double alpha=0;
	for(int i=1;i<=FX_Launch.N;i++)
	{
		PARTICLE ps = GetProperPS(FX_Launch.Ps1);
		alpha=(PI*2/FX_Launch.N*(i-1))+FX_Launch.Angle*RAD;
		vramp->AddParticleStream(&ps.Pss,_V(FX_Launch.Distance*cos(alpha),FX_Launch.Distance*sin(alpha),FX_Launch.H),_V(cos(alpha),sin(alpha),0),&launchFx_level);
		PARTICLE ps2 = GetProperPS(FX_Launch.Ps2);
		vramp->AddParticleStream(&ps2.Pss,_V(FX_Launch.Distance*cos(alpha),FX_Launch.Distance*sin(alpha),FX_Launch.H),_V(cos(alpha),sin(alpha),0),&launchFx_level);
	}
}

void Multistage2015::CreateHangar()
{
		hhangar=oapiGetVesselByName(const_cast<char *>("MS_Hangar"));
	if(!oapiIsVessel(hhangar)){
	//oapiWriteLog("Creating Hangar");
	//	DelAttachment(AttToRamp);
		//oapiWriteLog("Deleting old attachment");

		
		memset(&vshangar,0,sizeof(vshangar));
		vshangar.version=2;
		GetStatusEx(&vshangar);
		oapiWriteLog(const_cast<char *>("Creating Hangar"));
		hhangar= oapiCreateVesselEx("MS_Hangar","MS_Hangar",&vshangar);
		vshangar.status=1;
		vhangar = static_cast<VESSEL4*>(oapiGetVesselInterface(hhangar));
		if(oapiGetOrbiterVersion()<160903)
		{
		FILEHANDLE fh=oapiOpenFile("\\Multistage2015\\__MSHANG",FILE_OUT,CONFIG);
		oapiWriteScenario_string(fh,const_cast<char *>("STATUS"),const_cast<char *>("Landed Earth"));
		oapiWriteScenario_float(fh,const_cast<char *>("HEADING"),vshangar.surf_hdg*DEG);
		char position[256];
		sprintf(position,"%.20f %.20f",vshangar.surf_lng*DEG,vshangar.surf_lat*DEG);
		oapiWriteScenario_string(fh,const_cast<char *>("POS"),position);
		oapiCloseFile(fh,FILE_OUT);
		sprintf(logbuff,"Orbiter version: %i . Hangar is created through scenario file procedure",oapiGetOrbiterVersion());
		oapiWriteLog(logbuff);
		fh=oapiOpenFile("\\Multistage2015\\__MSHANG",FILE_IN,CONFIG);
		vhangar->clbkLoadStateEx(fh,&vshangar);
		}
		vhangar->DefSetStateEx(&vshangar);


		if(wCrawler){
			hCrawler=oapiGetVesselByName(const_cast<char *>("MS_Crawler"));
if(!oapiIsVessel(hCrawler))
	{
		VESSELSTATUS2 vsCrawler;
		memset(&vsCrawler,0,sizeof(vsCrawler));
		vsCrawler.version=2;
		vsCrawler=vshangar;
		vsCrawler.surf_hdg=vshangar.surf_hdg+180*RAD;

	
double rt=6371010;
double earth_circ=rt*2*PI;
double each_deg=earth_circ/360; 
double d_lat=2.0*sin(vsCrawler.surf_hdg);
	double d_lng=-2.0*cos(vsCrawler.surf_hdg);
	vsCrawler.surf_lat+=d_lat/each_deg*RAD;
	vsCrawler.surf_lng+=d_lng/each_deg*RAD;

			hCrawler=oapiCreateVesselEx("MS_Crawler","MS_Crawler",&vsCrawler);
			oapiWriteLog(const_cast<char *>("Crawler Created"));
	}
}





	}

	vhangar = static_cast<VESSEL4*>(oapiGetVesselInterface(hhangar));
		if(!AttToMSPad){
		AttToHangar=CreateAttachment(TRUE,_V(0,0,0),_V(0,0,-1),_V(0,1,0),"hangar",FALSE); 
		DWORD index;
		vhangar->clbkGeneric(VMSG_USER,1,&index);
		vhangar->AttachChild(GetHandle(),vhangar->GetAttachmentHandle(FALSE,index),AttToHangar);
		oapiWriteLog(const_cast<char *>("Attached to the Hangar"));
		}else{
			OBJHANDLE hPad=oapiGetVesselByName(const_cast<char *>("MS_Pad"));
			if(oapiIsVessel(hPad))
			{
			AttachToMSPad(hPad);
			}
		}

	

	
		

	return;
}
void Multistage2015::CreateCamera()
{
	//////////////CAMERA CREATION
	OBJHANDLE hcamera=oapiGetVesselByName(const_cast<char *>("MS_Camera"));
	if(!oapiIsVessel(hcamera)){
		VESSELSTATUS2 vscam;
		memset(&vscam,0,sizeof(vscam));
		vscam.version=2;
		if(HangarMode){vscam=vshangar;}else{vscam=vsramp;}
		
	//	vscam.surf_lat+=0.01*RAD;
	//	vscam.surf_lng+=0.03*RAD;
		vscam.surf_lat+=CamDLat*RAD;
		vscam.surf_lng+=CamDLng*RAD;
		hcamera=oapiCreateVesselEx("MS_Camera","MS_Camera",&vscam);
		if(oapiIsVessel(hcamera))
		{
			VESSEL4* vcam;
			vcam=static_cast<VESSEL4*>(oapiGetVesselInterface(hcamera));
			vcam->clbkGeneric(VMSG_USER,1,(void*)GetName());
		}
	}
	return;
}
void Multistage2015::Ramp(bool alreadyramp)
{
		char PadName[256];
		sprintf(PadName,"MS_LaunchPad_%s",GetName());
		if(!alreadyramp){	
		memset(&vsramp,0,sizeof(vsramp));
		vsramp.version=2;
		GetStatusEx(&vsramp);
		oapiWriteLog(const_cast<char *>("Creating Launchpad"));
	//	hramp= oapiCreateVesselEx("MS_LaunchPad","EmptyModule",&vsramp);
		
		hramp= oapiCreateVesselEx(PadName,Misc.PadModule,&vsramp);
		vsramp.status=1;
		vramp = static_cast<VESSEL4*>(oapiGetVesselInterface(hramp));
		if(oapiGetOrbiterVersion()<160903)
		{
		FILEHANDLE fh=oapiOpenFile("\\Multistage2015\\__MS15",FILE_OUT,CONFIG);
		oapiWriteScenario_string(fh,const_cast<char *>("STATUS"),const_cast<char *>("Landed Earth"));
		oapiWriteScenario_float(fh,const_cast<char *>("HEADING"),vsramp.surf_hdg*DEG);
		char position[256];
		sprintf(position,"%.20f %.20f",vsramp.surf_lng*DEG,vsramp.surf_lat*DEG);
		oapiWriteScenario_string(fh,const_cast<char *>("POS"),position);
		oapiCloseFile(fh,FILE_OUT);
		sprintf(logbuff,"Orbiter version: %i . Ramp is created through scenario file procedure",oapiGetOrbiterVersion());
		oapiWriteLog(logbuff);
		fh=oapiOpenFile("\\Multistage2015\\__MS15",FILE_IN,CONFIG);
		vramp->clbkLoadStateEx(fh,&vsramp);
		}
		vramp->DefSetStateEx(&vsramp);
		wRamp=TRUE;
		}
		
		
		else{
		//hramp=oapiGetVesselByName("MS_LaunchPad");
			hramp=oapiGetVesselByName(PadName);
		if(hramp){
				vramp = static_cast<VESSEL4*>(oapiGetVesselInterface(hramp));
			}
		}

		AttToRamp=CreateAttachment(TRUE,_V(0,0,0),_V(0,0,-1),_V(0,1,0),"pad",FALSE);
		padramp = vramp->CreateAttachment(FALSE,_V(0,0,stage[0].height*0.5+CogElev),_V(0,-sin(Misc.VerticalAngle),cos(Misc.VerticalAngle)),_V(0,cos(Misc.VerticalAngle),sin(Misc.VerticalAngle)),"pad",FALSE);
		vramp->AttachChild(GetHandle(),padramp,AttToRamp);
	
		if(wLaunchFX)
		{
			CreateLaunchFX();
		}
		vramp->SetEnableFocus(FALSE);
	
		
		
	return;
}

/*
void Multistage2015::clbkVisualCreated (VISHANDLE _vis, int refcount)
{
  if (refcount > 1) return; 
  vis = _vis;

  




}
void Multistage2015::clbkVisualDestroyed (VISHANDLE _vis, int refcount)
{
  if (vis == _vis) vis = NULL;





}
*/

double Multistage2015::GetMET()
{
	return MET;
}
bool Multistage2015::GetAutopilotStatus()
{
	return APstat;
}
int Multistage2015::clbkGeneric (int msgid, int prm, void *context)
{
	
	switch(msgid){
	case(2015):
		if(prm==2015){
			return 2015;}
		else{return 0;}
	break;
	case(VMSG_USER):
		if(prm==1)
		{
			double *met = static_cast<double*>(context);
			*met=GetMET();
			return 1;
		}else if(prm==2)
		{
			bool *ap = static_cast<bool*>(context);
			*ap=GetAutopilotStatus();
			return 1;
		}else if(prm==3)
		{
			VECTOR3* Targets=static_cast<VECTOR3*>(context);
			*Targets=GetAPTargets();
			return 1;
		}
	break;
	}

	return 0;
}

int Multistage2015::GetMSVersion()
{
	return MSVERSION;
}

void Multistage2015::CreateDMD()
{
	//oapiWriteLog("CREATE DMD");
	if(!DMD){
		DMD=new DevModeDlg(this);
		DMD->Open(g_Param.hDLL);
	}
}

void Multistage2015::DestroyDMD()
{
	if(DMD){
		delete DMD;
		DMD = 0;
		hDlg=0;
	}
}


DLLCLBK void InitModule(HINSTANCE hModule){
g_Param.hDLL = hModule;
}
DLLCLBK void ExitModule(HINSTANCE hModule){
CloseDlg(hDlg);
}
DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel,int flightmodel){return new Multistage2015(hvessel,flightmodel);}
DLLCLBK void ovcExit(VESSEL *vessel){if(vessel)delete(Multistage2015*)vessel;}
DevModeDlg *dlg;

INT_PTR CALLBACK DlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, (LONG)lParam);
		((DevModeDlg*)lParam)->hDlg = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if (dlg) return dlg->DlgProc (hWnd, uMsg, wParam, lParam);
	else     return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}
INT_PTR CALLBACK DlgProcPld(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_PLD] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if (dlg) return dlg->PldProc (hWnd, uMsg, wParam, lParam);
	else     return FALSE;
}
INT_PTR CALLBACK DlgProcViewTxt(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_VIEW]= hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if (dlg) return dlg->ViewTxtProc (hWnd, uMsg, wParam, lParam);
	else     return FALSE;
}
INT_PTR CALLBACK DlgProcFairing(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_FAIRING] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if (dlg) return dlg->FairingProc(hWnd, uMsg, wParam, lParam);
	else   return FALSE;
}
INT_PTR CALLBACK DlgProcParticle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_PARTICLE] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if (dlg) return dlg->ParticleProc(hWnd, uMsg, wParam, lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcMisc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_MISC] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->MiscProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcTex(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_TEX] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->TexProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcBooster(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_BOOSTER] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->BoosterProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcCurve(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_CURVE] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->CurveProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcStage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_STAGE] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->StageProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcUllage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_ULLAGE] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->UllageProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcInterstage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_INTERSTAGE] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->InterstageProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcLes(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_LES] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->LesProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcAdapter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	if(uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_ADAPTER] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);
	if(dlg) return dlg->AdapterProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcFX(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd,GWLP_USERDATA,lParam);
		((DevModeDlg*)lParam)->hChild[CD_FX] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd,GWLP_USERDATA);
	if(dlg) return dlg->FXProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
INT_PTR CALLBACK DlgProcScenario(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd,GWLP_USERDATA,lParam);
		((DevModeDlg*)lParam)->hChild[CD_SCENARIO] = hWnd;
	}
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd,GWLP_USERDATA);
	if(dlg) return dlg->ScenarioProc(hWnd,uMsg,wParam,lParam);
	else return FALSE;
}
