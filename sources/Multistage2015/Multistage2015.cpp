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
#include <array>
#include <cstddef>
#include <format>
#include <filesystem>
#include <string>
#define ORBITER_MODULE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <math.h>
#include <stdio.h>

#include "Multistage2015.h"

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
void CloseDlg (HWND hDlg){
	oapiCloseDialog (hDlg);
}

//Creation
Multistage2015::Multistage2015(OBJHANDLE hObj,int fmodel) : VESSEL4(hObj,fmodel){

	DeveloperMode = false;
	//Ms15=this;
	HangarMode = false;
	

}

Multistage2015::~Multistage2015(){

	if(psg)delete []psg;
		for(int i=0;i<10;i++){
			for(int j=0;j<5;j++){
				delete payloadrotatex.at(i)[j];
				delete payloadrotatey.at(i)[j];
				delete payloadrotatez.at(i)[j];
			}
		}

}


VECTOR4F Multistage2015::_V4(double x, double y, double z, double t){
	
	VECTOR4F v4;

	v4.x=x;
	v4.y=y;
	v4.z=z;
	v4.t=t;

	return v4;
}

VECTOR2 Multistage2015::_V2(double x, double y){
	
	VECTOR2 v2;

	v2.x=x;
	v2.y=y;

	return v2;
}

//returns 1 if X is positive or -1 if X is negative
int Multistage2015::SignOf(double X){
	
	return X/abs(X);

}

//returns true if a number is odd

bool Multistage2015::IsOdd( int integer ){
	
	if (integer % 2== 0){
     	return true;
	} else {
     	return false;
	}
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

	output = _V(input.x * cos(Angle) - input.y * sin(Angle), input.x * sin(Angle) + input.y * cos(Angle), input.z);

	return output;
}

MATRIX3 Multistage2015::RotationMatrix(VECTOR3 angles){
	
	MATRIX3 m;
 	MATRIX3 RM_X,RM_Y,RM_Z;

	RM_X=_M(1,0,0,0,cos(angles.x),-sin(angles.x),0,sin(angles.x),cos(angles.x));
	
	RM_Y=_M(cos(angles.y),0,sin(angles.y),0,1,0,-sin(angles.y),0,cos(angles.y));
	
	RM_Z=_M(cos(angles.z),-sin(angles.z),0,sin(angles.z),cos(angles.z),0,0,0,1);
	
	m=mul(RM_Z,mul(RM_Y,RM_X));

	return m;
}


void Multistage2015::ResetVehicle(VECTOR3 hangaranimsV = _V(1.3,-10,57.75), bool Ramp = false){
 
	ClearMeshes();
	ClearThrusterDefinitions();
	ClearPropellantResources();
	ClearAttachments();

	initGlobalVars();
	wRamp=Ramp;
	if(wRamp){
		oapiDeleteVessel(hramp);
		wRamp = false;
	}

	OrbiterRoot = std::filesystem::current_path();
	std::filesystem::path tempFile;

	tempFile = OrbiterRoot / fileini;

	logbuff = std::format("{}: Config File: {}", GetName(), tempFile.string());
 	oapiWriteLog(const_cast<char *>(logbuff.c_str()));
 	parseinifile(tempFile);

	currentBooster = loadedCurrentBooster;
	currentInterstage = loadedCurrentInterstage;
	currentStage = loadedCurrentStage;
	currentPayload = loadedCurrentPayload;
	wFairing = loadedwFairing;
	Configuration = loadedConfiguration;
	GrowingParticles=loadedGrowing;

 	MET = loadedMET;
	if((currentInterstage>currentStage) ||(currentInterstage>nInterstages) || (currentInterstage >= stage.at(currentStage).IntIncremental)){
		stage.at(currentStage).wInter = false;
	}
 
 	if((wFairing==1) && (hasFairing == false)){
		wFairing=0;
	}
 	
	if(Configuration == 0){			//if only configuration is defined, reset everything
	 	if(hasFairing == true){
		  	wFairing=1;
		}
	 	currentStage=0;
	 	currentPayload=0;
		currentBooster=0;
		currentInterstage=0;
 	}

 	parseGuidanceFile(guidancefile);
 	UpdateOffsets();

	VehicleSetup();
	LoadMeshes();

	vs2.rvel=_V(0,0,0);
 
  	clbkSetStateEx(&vs2);

  	hangaranims=hangaranimsV;
 
 	clbkPostCreation();
 
	return;

}


 //Returns current Heading
 double Multistage2015::GetHeading(){
	
	double Heading;
 	
	oapiGetHeading(GetHandle(), &Heading);

	return Heading;

}


//returns Vehicle Orbital Speed
double Multistage2015::GetOS(){

	OBJHANDLE hearth = GetSurfaceRef();

	VECTOR3 relvel;

	GetRelativeVel(hearth,relvel);

	double os = length(relvel);

	return os;
}

//returns Vechicle Mass at a certain stage
double Multistage2015::GetMassAtStage(int MassStage,bool empty = true){

	double Mass=0;

	int add=0;
	
	if(empty == true){
		add=1;
	} else {
		add=0;
	}

	for(int i = MassStage; i<nStages; i++){
		 Mass += stage.at(i).emptymass;
	}

	for(int i = MassStage + add; i < nStages; i++){
		Mass += stage.at(i).fuelmass;
	}

	for(int i = 0; i < nPayloads; i++){
		 Mass+=payload.at(i).mass;
	}

	if(hasFairing){
		Mass += fairing.emptymass;
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

	sdv = stage.at(dvstage).isp * log(GetMassAtStage(dvstage,false) / GetMassAtStage(dvstage, true));

	return sdv;
}

//Returns Remaining DV at a certain Stage
double Multistage2015::DvAtStage(int dvatstage){
	
	double rdvas=0;

	for(int i = dvatstage; i < nStages; i++){
		 rdvas += StageDv(i);
	}

	return rdvas;
}

//Returns Current Stage Remaining DV
double Multistage2015::CurrentStageRemDv(){

	double csrd;

	csrd = stage.at(currentStage).isp * log((GetMassAtStage(currentStage, true) + GetPropellantMass(stage.at(currentStage).tank)) / GetMassAtStage(currentStage, true));

	return csrd;
}

//Returns Any Stage Remaining DV
double Multistage2015::StageRemDv(int dvstage){

	double srd;

	srd = stage.at(dvstage).isp * log((GetMassAtStage(dvstage, true) + GetPropellantMass(stage.at(dvstage).tank)) / GetMassAtStage(dvstage, true));

	return srd;
}

//Returns Remaining Stage Burn Time
double Multistage2015::RemBurnTime(int rbtstage, double level=1){
	
	double BT;

	BT = stage.at(rbtstage).isp * GetPropellantMass(stage.at(rbtstage).tank) / (stage.at(rbtstage).thrust * level);

	return BT;
}

//Returns Remaining Boosters Group Burn Time
double Multistage2015::BoosterRemBurnTime(int rbtbooster, double level=1){
	
	double BT;

	BT = booster.at(rbtbooster).isp * GetPropellantMass(booster.at(rbtbooster).tank) / ((booster.at(rbtbooster).thrust * booster.at(rbtbooster).N) * level);
	
	return BT;
}


//Global Variables initialization
void Multistage2015::initGlobalVars(){

	nStages = 0;
	nBoosters = 0;
	nInterstages = 0;
	nTextures = 0;
	nParticles = 0;
	currentStage = 0;
	currentBooster = 0;
	currentInterstage = 0;
	CogElev = 0;
	Misc.VerticalAngle = 0;
	Misc.drag_factor = 1;
	OrbiterRoot = std::filesystem::current_path(); //initialize orbiter root with the current directory
	wBoosters = false;
	wFairing = 0;
	wLes = false;
	wAdapter = false;
	wMach = false;
	wVent = false;
	wLaunchFX = false;
	Complex = false;
	stage_ignition_time = 0;

	currentDelta = 0;
	perror = 0;
	yerror = 0;
	rerror = 0;

	for(int i = 0; i < 10; i++){
		stage.at(i) = STAGE();
		stage.at(i).Ignited = false;
		stage.at(i).reignitable = true;
		stage.at(i).batteries.wBatts = false;
		stage.at(i).waitforreignition=0;
		stage.at(i).StageState = STAGE_SHUTDOWN;
		stage.at(i).DenyIgnition=false;
		stage.at(i).ParticlesPacked=false;
		stage.at(i).ParticlesPackedToEngine=0;
		stage.at(i).defpitch=false;
		stage.at(i).defroll=false;
		stage.at(i).defyaw=false;
		payload.at(i) = PAYLOAD();	
	}

	for(int i = 0; i < 10; i++){
		booster.at(i) = BOOSTER();
		booster.at(i).Ignited = false;
		booster.at(i).ParticlesPacked = false;
		booster.at(i).ParticlesPackedToEngine = 0;	
	}

	MET = 0;
	APstat = false;
	AJdisabled = false;
	rolldisabled = false;
	pitchdisabled = false;

	Gnc_running = 0;
	spinning = false;
	lvl = 1;

	for(int i = 0; i < 57; i++){
		Ssound.GncStepSound.at(i) = -5;
	}


	PegDesPitch = 35 * RAD;
	PegPitchLimit = 35 * RAD;
	DeltaUpdate = 0;
	GT_InitPitch = 89.5 * RAD;
	UpdatePegTimer = 0;
	UpdateComplex = 0;
	wPeg = false;
	PegMajorCycleInterval = 0.1;
	runningPeg=false;

	AttCtrl = true;
	PitchCtrl = true;
 	YawCtrl = true;
 	RollCtrl = true;
	TgtPitch = 0;


	eps = -9000000000000;

	failureProbability = -1000;
	timeOfFailure = -10000000;
	wFailures = false;
	failed = false;

	DMD = 0;
	killDMD = false;
	stepsloaded = false;

	for(int i = 0; i < 150; i++)
	{
		Gnc_step.at(i).GNC_Comand=CM_NOLINE;
	}


	updtlm = 0;
	tlmidx = 0;
	writetlmTimer = 0;
	tlmnlines = 0;
	wReftlm = false;

	for(int i = 0; i < TLMSECS; i++){
		tlmAlt.at(i).x = 0;
		tlmSpeed.at(i).x = 0;
		tlmPitch.at(i).x = 0;
		tlmThrust.at(i).x = 0;
		tlmMass.at(i).x = 0;
		tlmVv.at(i).x = 0;
		tlmAcc.at(i).x = 0;

		tlmAlt.at(i).y = 0;
		tlmSpeed.at(i).y = 0;
		tlmPitch.at(i).y = 0;
		tlmThrust.at(i).y = 0;
		tlmMass.at(i).y = 0;
		tlmVv.at(i).y = 0;
		tlmAcc.at(i).y = 0;
	}

	updtboiloff = 0;
	for(int i = 0; i < 100; i++){
		coeff.at(i)=0;
	}

	avgcoeff = 0;
	MECO_Counter = 0;

	MECO_TEST = 0;
	TMeco = 600;
	nPsg = 0;
	particlesdt = 0;
	GrowingParticles = false;
	RefPressure = 101400;

	wRamp = false;
	NoMoreRamp = false;
	col_d.a = 0;
	col_d.b = 1;
	col_d.g = 0.8;
	col_d.r = 0.9;
	col_s.a = 0;
	col_s.b = 1;
	col_s.g = 0.8;
	col_s.r = 1.9;
	col_a.a = 0;
	col_a.b = 0;
	col_a.g = 0;
	col_a.r = 0;
	col_white.a = 0;
	col_white.b = 1;
	col_white.g = 1;
	col_white.r = 1;

	th_main_level = 0;
	launchFx_level = 0;
 	hangaranims = _V(1.3, -10, 57.75);
 	wCrawler = false;
	wCamera = false;
	AttToMSPad = false;
 	MsPadZ = _V(0, 0, -50);
	
	return;
}

PSTREAM_HANDLE Multistage2015::AddExhaustStreamGrowing(THRUSTER_HANDLE  th,    const VECTOR3 &  pos,    PARTICLESTREAMSPEC *  pss = 0  , bool growing=false, double growfactor_size=0, double growfactor_rate=0, bool count=true, bool ToBooster=false,int N_Item=0, int N_Engine=0) {
	
	PSTREAM_HANDLE psh=AddExhaustStream(th,pos,pss);
	
	psg[nPsg].pss=*pss;
	psg[nPsg].psh.at(2)=psh;
	psg[nPsg].th=th;
	
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

	if(count){
		nPsg++;
	}
	
	return psh;
}

//Create RCS 
void Multistage2015::CreateRCS(){
	
	if(stage.at(currentStage).pitchthrust == 0){
		stage.at(currentStage).pitchthrust = 0.25 * stage.at(currentStage).thrust * stage.at(currentStage).height;//Empirical Values
 	}

	if(stage.at(currentStage).yawthrust == 0){
		stage.at(currentStage).yawthrust = 0.25 * stage.at(currentStage).thrust * stage.at(currentStage).height;//Empirical Values
	}

	if(stage.at(currentStage).rollthrust == 0){
		stage.at(currentStage).rollthrust = 0.1 * stage.at(currentStage).thrust * stage.at(currentStage).diameter * 0.5 * 0.5; //Empirical Values
	}
	//pitch up
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0 ,1), _V(0, 1, 0), 2 * stage.at(currentStage).pitchthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

 	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, -1), _V(0, -1, 0), 2 * stage.at(currentStage).pitchthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

 	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_PITCHUP);

	MaxTorque.x = 2 * GetThrusterMax(stage.at(currentStage).th_att_h.at(0));

	//pitch down
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 1), _V(0, -1, 0), 2 * stage.at(currentStage).pitchthrust,stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, -1), _V(0, 1, 0), 2 * stage.at(currentStage).pitchthrust,stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_PITCHDOWN);

	//yaw left
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 1), _V(-1, 0, 0), 2 * stage.at(currentStage).yawthrust,stage.at(currentStage).tank, stage.at(currentStage).isp * 100);
	
	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, -1), _V(1, 0, 0), 2 * stage.at(currentStage).yawthrust,stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_YAWLEFT);

	MaxTorque.y = 2 * GetThrusterMax(stage.at(currentStage).th_att_h.at(0));
	
	//yaw right
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 1), _V(1, 0, 0), 2 * stage.at(currentStage).yawthrust,stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, -1), _V(-1, 0, 0),2 * stage.at(currentStage).yawthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_YAWRIGHT);

	//roll left
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(1, 0, 0), _V(0, 1, 0), 2 * stage.at(currentStage).rollthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(-1, 0, 0), _V(0, -1, 0),2 * stage.at(currentStage).rollthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_BANKLEFT);

	MaxTorque.z=2*GetThrusterMax(stage.at(currentStage).th_att_h.at(0));

	//roll right
	stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(1, 0, 0), _V(0, -1, 0), 2 * stage.at(currentStage).rollthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(-1, 0, 0), _V(0, 1, 0), 2 * stage.at(currentStage).rollthrust, stage.at(currentStage).tank, stage.at(currentStage).isp * 100);

	CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_BANKRIGHT);

	if(stage.at(currentStage).linearthrust > 0){

		if(stage.at(currentStage).linearisp<=0){
			stage.at(currentStage).linearisp=stage.at(currentStage).isp * 100;
		}

		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(0, 0, 1), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(0, 0, 1), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_FORWARD);
	
		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(0, 0, -1), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(0, 0, -1), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_BACK);
	
		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(-1, 0, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(-1, 0, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_LEFT);
	
		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_RIGHT);
		
		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(0, 1, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(0, 1, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_UP);
		
		stage.at(currentStage).th_att_h.at(0) = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		stage.at(currentStage).th_att_h.at(1) = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), stage.at(currentStage).linearthrust * 0.5, stage.at(currentStage).tank, stage.at(currentStage).linearisp);

		CreateThrusterGroup(stage.at(currentStage).th_att_h.data(), 2, THGROUP_ATT_DOWN);
	
	}

	return;
}

//Creates Main Thrusters and relevant Exhausts
void Multistage2015::CreateMainThruster(){

	if(stage.at(currentStage).nEngines == 0){    //if no stage engines are defined there will be anyway one displayed
	 	stage.at(currentStage).nEngines = 1;
	 	stage.at(currentStage).eng.at(0).x = 0;
	 	stage.at(currentStage).eng.at(0).y = 0;
	 	stage.at(currentStage).eng.at(0).z = -stage.at(currentStage).height * 0.5;
 	}


 
//added by rcraig42 to handle ispref in ini but no pressure, to default to earth -------------------------

 	if((stage.at(currentStage).ispref >= 0) && (stage.at(currentStage).pref==0)){
		stage.at(currentStage).pref = 101400.0;
	}

//-------------------------------------------------------------------------------------------------------	
 
 //modded by rcraig42 to add ispref and pref to createthruster -------------------------------------------------
	
	if(Misc.thrustrealpos){ 

		for(int i = 0; i < stage.at(currentStage).nEngines; i++){
			stage.at(currentStage).th_main_h.at(i) = CreateThruster(stage.at(currentStage).off, stage.at(currentStage).eng_dir, stage.at(currentStage).thrust / stage.at(currentStage).nEngines, stage.at(currentStage).tank, stage.at(currentStage).isp, stage.at(currentStage).ispref, stage.at(currentStage).pref);
		}

	} else {
		
		for(int i = 0; i < stage.at(currentStage).nEngines; i++){
			stage.at(currentStage).th_main_h.at(i) = CreateThruster(_V(0, 0, 0),_V(0, 0, 1), stage.at(currentStage).thrust / stage.at(currentStage).nEngines, stage.at(currentStage).tank, stage.at(currentStage).isp, stage.at(currentStage).ispref, stage.at(currentStage).pref);
		}

	}
	
	thg_h_main = CreateThrusterGroup(stage.at(currentStage).th_main_h.data(), stage.at(currentStage).nEngines, THGROUP_MAIN);

 	SetDefaultPropellantResource(stage.at(currentStage).tank);

	SURFHANDLE ChoosenTexture = nullptr; //Initialization of Texture
 

	for(int i = 0; i < stage.at(currentStage).nEngines; i++){
	 
		exhaustN.at(currentStage).at(i) = AddExhaust(stage.at(currentStage).th_main_h.at(i), 10 * stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng.at(i), operator*(stage.at(currentStage).eng_dir,-1), GetProperExhaustTexture(stage.at(currentStage).eng_tex));
	  
		if(!stage.at(currentStage).ParticlesPacked){
	  		if(stage.at(currentStage).wps1){
					PARTICLE ps = GetProperPS(stage.at(currentStage).eng_pstream1);
					AddExhaustStreamGrowing(
					stage.at(currentStage).th_main_h.at(i),
					stage.at(currentStage).eng.at(i),
					&ps.Pss,
					GetProperPS(stage.at(currentStage).eng_pstream1).Growing,
					GetProperPS(stage.at(currentStage).eng_pstream1).GrowFactor_size,
					GetProperPS(stage.at(currentStage).eng_pstream1).GrowFactor_rate,
					true,
					false,
					currentStage,
					i);

				logbuff = std::format("{}: Stage n.{} Engine Exhaust Stream Added: {} to engine n.{}", GetName(),currentStage + 1, stage.at(currentStage).eng_pstream1, i + 1);

				oapiWriteLog(const_cast<char *>(logbuff.c_str()));
			}

			if(stage.at(currentStage).wps2){
				PARTICLE ps = GetProperPS(stage.at(currentStage).eng_pstream2);
	  			AddExhaustStreamGrowing(
					stage.at(currentStage).th_main_h.at(i), 
					stage.at(currentStage).eng.at(i), 
					&ps.Pss, 
					GetProperPS(stage.at(currentStage).eng_pstream2).Growing, 
					GetProperPS(stage.at(currentStage).eng_pstream2).GrowFactor_size, 
					GetProperPS(stage.at(currentStage).eng_pstream2).GrowFactor_rate, 
					true, 
					false, 
					currentStage, 
					i);

				logbuff = std::format("{}: Stage n.{} Engine Exhaust Stream Added: {} to engine n.{}", GetName(),currentStage + 1, stage.at(currentStage).eng_pstream2, i + 1);

				oapiWriteLog(const_cast<char *>(logbuff.c_str()));
			}
	 	}
		
	 	logbuff = std::format("{}: Stage n. {} Engines Exhaust Added--> number of engines: {} , diameter: {:.3f}, position x: {:.3f} y: {:.3f} z: {:.3f}", GetName(), currentStage + 1, stage.at(currentStage).nEngines, stage.at(currentStage).eng_diameter * stage.at(currentStage).engV4.at(i).t, stage.at(currentStage).eng.at(i).x, stage.at(currentStage).eng.at(i).y, stage.at(currentStage).eng.at(i).z);

	 	oapiWriteLog(const_cast<char *>(logbuff.c_str()));
	}

 	if(stage.at(currentStage).ParticlesPacked){

		std::array<PARTICLESTREAMSPEC, 2> partpacked;
		partpacked.at(0)=GetProperPS(stage.at(currentStage).eng_pstream1).Pss;
		partpacked.at(1)=GetProperPS(stage.at(currentStage).eng_pstream2).Pss;

		
		int engine = abs(stage.at(currentStage).ParticlesPackedToEngine) - 1;

		VECTOR3 thdir;
		std::array<VECTOR3, 2> Pos;
		GetThrusterDir(stage.at(currentStage).th_main_h[engine],thdir);
		thdir.x *= -1;
		thdir.y *= -1;
		thdir.z *= -1;

		if(stage.at(currentStage).ParticlesPackedToEngine > 0){
			Pos.at(0)=stage.at(currentStage).eng[engine];
			Pos.at(1)=stage.at(currentStage).eng[engine];
		} else {

			double Posx=0;
			double Posy=0;
			double Posz=0;

			for(int i = 0; i < stage.at(currentStage).nEngines; i++){
				Posx+=stage.at(currentStage).eng.at(i).x;
				Posy+=stage.at(currentStage).eng.at(i).y;
				Posz+=stage.at(currentStage).eng.at(i).z;
			}

			Posx /= stage.at(currentStage).nEngines;
			Posy /= stage.at(currentStage).nEngines;
			Posz /= stage.at(currentStage).nEngines;

			Pos.at(0).x = Posx;
			Pos.at(0).y = Posy;
			Pos.at(0).z = Posz;
			Pos.at(1) = Pos.at(0);


		}

		if(stage.at(currentStage).wps1){
	 		AddExhaustStreamGrowing(stage.at(currentStage).th_main_h[engine], Pos.at(0), &partpacked.at(0), GetProperPS(stage.at(currentStage).eng_pstream1).Growing, GetProperPS(stage.at(currentStage).eng_pstream1).GrowFactor_size, GetProperPS(stage.at(currentStage).eng_pstream1).GrowFactor_rate, true, false, currentStage, engine);

			logbuff = std::format("{}: Stage n.{} Engine Packed Exhaust Stream Added: {} to engine n.{}", GetName(), currentStage + 1 , stage.at(currentStage).eng_pstream1, engine + 1);

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		}
		
		if(stage.at(currentStage).wps2){

			AddExhaustStreamGrowing(stage.at(currentStage).th_main_h[engine],Pos.at(1),&partpacked.at(1),GetProperPS(stage.at(currentStage).eng_pstream2).Growing,GetProperPS(stage.at(currentStage).eng_pstream2).GrowFactor_size,GetProperPS(stage.at(currentStage).eng_pstream2).GrowFactor_rate,TRUE,false,currentStage,engine);

			logbuff = std::format("{}: Stage n.{} Engine Packed Exhaust Stream Added: {} to engine n.{}", GetName(),currentStage + 1, stage.at(currentStage).eng_pstream2, engine + 1);

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		}

	}

	if(stage.at(currentStage).DenyIgnition){
		for(int i = 0; i < stage.at(currentStage).nEngines; i++){
		SetThrusterResource(stage.at(currentStage).th_main_h.at(i), nullptr);
		}
	}


	LightEmitter *le = AddPointLight(stage.at(currentStage).eng.at(0), 200, 1e-3, 0, 2e-3, col_d, col_s, col_a);
	le->SetIntensityRef(&th_main_level);

	return;

}

//Get Boosters Position given group number and booster number inside the group
VECTOR3 Multistage2015::GetBoosterPos(int nBooster, int N){

	VECTOR3 bpos = booster.at(nBooster).off;
	
	double arg = booster.at(nBooster).angle * RAD + (N-1) * 2 * PI / booster.at(nBooster).N;

	VECTOR3 bposdef = _V(bpos.x * cos(arg) - bpos.y * sin(arg), bpos.x * sin(arg) + bpos.y * cos(arg), bpos.z);			
	 
	return bposdef;
}

std::string Multistage2015::GetProperPayloadMeshName(int pnl, int n){

	if(n == 0){
		return payload[pnl].meshname0;
	} else if(n == 1){
		return payload[pnl].meshname1;
	} else if(n == 2){
		return payload[pnl].meshname2;
	} else if(n == 3){
		return payload[pnl].meshname3;
	} else if(n == 4){
		return payload[pnl].meshname4;
	} else {
		return payload[pnl].meshname0;
	}

}


void Multistage2015::RotatePayload(int pns, int nm,VECTOR3 anglesrad){
	
	VECTOR3 state = _V(anglesrad.x / (2 * PI), anglesrad.y / (2 * PI), anglesrad.z / (2 * PI));//portion of 2*PI to rotate
	VECTOR3 reference;
	
	if(nm == 0){
		reference = _V(0, 0, 0);
	} else {
		reference = operator-(payload[pns].off[nm], payload[pns].off.at(0));
	}

	if(!DeveloperMode){
		payloadrotatex[pns][nm] = new MGROUP_ROTATE(payload[pns].msh_idh[nm], NULL, NULL, reference, _V(1, 0, 0),(float)2 * PI);

		RotatePayloadAnim_x[pns][nm] = CreateAnimation(0);

		anim_x[pns][nm] = AddAnimationComponent(RotatePayloadAnim_x[pns][nm], 0.0f, 1.0f,payloadrotatex[pns][nm]);

		payloadrotatey[pns][nm] = new MGROUP_ROTATE(payload[pns].msh_idh[nm], NULL, NULL, reference, _V(0, 1, 0), (float)2 * PI); 
		
		RotatePayloadAnim_y[pns][nm] = CreateAnimation(0);

		anim_y[pns][nm] = AddAnimationComponent(RotatePayloadAnim_y[pns][nm], 0.0f, 1.0f, payloadrotatey[pns][nm]);

		payloadrotatez[pns][nm] = new MGROUP_ROTATE(payload[pns].msh_idh[nm], NULL, NULL, reference, _V(0, 0, 1), (float)2 * PI); 

		RotatePayloadAnim_z[pns][nm] = CreateAnimation(0);

		anim_z[pns][nm] = AddAnimationComponent(RotatePayloadAnim_z[pns][nm], 0.0f, 1.0f, payloadrotatez[pns][nm]);

	} else {
		SetAnimation(RotatePayloadAnim_z[pns][nm],0);
		SetAnimation(RotatePayloadAnim_y[pns][nm],0);
		SetAnimation(RotatePayloadAnim_x[pns][nm],0);
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
		
	for(int i = currentStage; i < nStages; i++){

		VECTOR3 pos=stage.at(i).off;

		stage.at(i).msh_h = oapiLoadMeshGlobal(stage.at(i).meshname.c_str());

		logbuff = std::format("{}: Stage n.{} Mesh Preloaded: {}", GetName(), i + 1, stage.at(i).meshname);

		oapiWriteLog(const_cast<char *>(logbuff.c_str()));

		stage.at(i).msh_idh = AddMesh(stage.at(i).msh_h,&pos);

		logbuff = std::format("{}: Stage n.{} Mesh Added Mesh: {} @ x:{:.3f} y:{:.3f} z:{:.3f}", GetName(), i + 1, stage.at(i).meshname, pos.x, pos.y, pos.z);

		oapiWriteLog(const_cast<char *>(logbuff.c_str()));

		if(stage.at(i).wInter == true){
				
			VECTOR3 inspos=stage.at(i).interstage.off;

			stage.at(i).interstage.msh_h = oapiLoadMeshGlobal(stage.at(i).interstage.meshname.c_str());

			logbuff = std::format("{}: Interstage Mesh Preloaded for Stage {}", GetName(), i + 1);
			
			oapiWriteLog(const_cast<char *>(logbuff.c_str()));

			stage.at(i).interstage.msh_idh = AddMesh(stage.at(i).interstage.msh_h, &inspos);

			logbuff = std::format("{}: Interstage Mesh Added: {} @ x:{:.3f} y:{:.3f} z:{:.3f}", GetName(), stage.at(i).interstage.meshname, inspos.x, inspos.y, inspos.z);

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		}

	}
  
	for(int i = currentPayload; i < nPayloads; i++){

		if(!payload.at(i).live){

			for(int j = 0; j < payload.at(i).nMeshes; j++){
				
				VECTOR3 pos=payload.at(i).off.at(j);

				payload.at(i).msh_h.at(j) = oapiLoadMeshGlobal(GetProperPayloadMeshName(i, j).c_str());

				logbuff = std::format("{} Payload Mesh Preloaded {}", GetName(), i + 1);

				oapiWriteLog(const_cast<char *>(logbuff.c_str()));

				payload.at(i).msh_idh.at(j) = AddMesh(payload.at(i).msh_h.at(j), &pos);

				logbuff = std::format("{}: Payload n.{} Mesh Added: {} @ x:{:.3f} y:{:.3f} z:{:.3f}", GetName(), i + 1,GetProperPayloadMeshName(i, j), pos.x, pos.y, pos.z);

				oapiWriteLog(const_cast<char *>(logbuff.c_str()));

				if((payload.at(i).render == 0) && (wFairing == 1)){
					SetMeshVisibilityMode(payload.at(i).msh_idh.at(j), MESHVIS_NEVER);
				}

				RotatePayload(i, j, payload[i].Rotation);
				
				/// ATTACHMENT POINTS MUST BE CREATED ANYWAY TO GET THE PAYLOAD BACK ONCE RELOADING THE SCENARIO
				}

			}


			VECTOR3 direction, normal;
			if(!payload.at(i).rotated){
				direction = _V(0, 0, 1);
				normal = _V(0, 1, 0);
			} else {
				direction = payload.at(i).Rotation;

				VECTOR3 rotation;

				rotation = payload.at(i).Rotation;

				direction = mul(RotationMatrix(rotation), _V(0, 0, 1));

				normal = mul(RotationMatrix(rotation), _V(0, 1, 0));

				normalise(normal);

				normalise(direction);

			}

			live_a.at(i) = CreateAttachment(false, payload.at(i).off.at(0), direction, normal, "MS2015", false);

	}

	for(int i = currentBooster; i < nBoosters; i++){
 		
		VECTOR3 bpos = booster.at(i).off;
 		
		VECTOR3 bposxy = bpos;

 		bposxy.z = 0;
 		
		double bro = length(bposxy);

		for(int j = 1; j < booster.at(i).N + 1; j++){
		
			std::string boosbuff, boosmhname;

			boosbuff = std::format("_{}", j);
			
			boosmhname = booster.at(i).meshname;
			
			boosmhname += boosbuff;

			double arg = booster.at(i).angle * RAD + (j-1) * 2 * PI / booster.at(i).N;

			VECTOR3 bposdef = _V(bpos.x * cos(arg) - bpos.y * sin(arg), bpos.x * sin(arg) + bpos.y * cos(arg), bpos.z);

			booster.at(i).msh_h.at(j) = oapiLoadMeshGlobal(boosmhname.c_str());

			logbuff = std::format("{}: Booster Mesh Preloaded: {}", GetName(), boosmhname);

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));

			booster.at(i).msh_idh.at(j) = AddMesh(booster.at(i).msh_h.at(j), &bposdef);

			logbuff = std::format("{}: Booster Mesh Added Mesh: {} @ x:{:.3f} y:{:.3f} z:{:.3f}", GetName(), boosmhname,bposdef.x, bposdef.y, bposdef.z);

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		}
	}

	if(wFairing == 1){
		VECTOR3 fpos = fairing.off;

		VECTOR3 fposxy = fpos;

		fposxy.z = 0;

		double fro = length(fposxy);

		for(int i = 1; i < fairing.N + 1; i++){
			std::string fairbuff, fairmshname;

			fairbuff = std::format("_{}", i);

			fairmshname = fairing.meshname;

			fairmshname += fairbuff;

			VECTOR3 fposdef = _V(fro * cos(fairing.angle * RAD + (i - 1) * 2 * PI / fairing.N), fro * sin(fairing.angle * RAD + (i-1) * 2 * PI / fairing.N), fpos.z);

			fairing.msh_h.at(i) = oapiLoadMeshGlobal(fairmshname.c_str());

			logbuff = std::format("{}: Fairing Mesh Preloaded: {}", GetName(), fairmshname);

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));

			fairing.msh_idh.at(i) = AddMesh(fairing.msh_h.at(i), &fposdef);

			logbuff = std::format("{}: Fairing Mesh Added Mesh: {} @ x:{:.3f} y:{:.3f} z:{:.3f}", GetName(), fairmshname,fposdef.x, fposdef.y, fposdef.z);

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		}

	}

	if(wAdapter == false){

		VECTOR3 adappos = Adapter.off;

		Adapter.msh_h = oapiLoadMeshGlobal(Adapter.meshname.c_str());

		logbuff = std::format("{}: Adapter Mesh Preloaded", GetName());

		oapiWriteLog(const_cast<char*>(logbuff.c_str()));

		Adapter.msh_idh = AddMesh(Adapter.msh_h, &adappos);

		logbuff = std::format("{}: Adapter Mesh Added: {} @ x:{:.3f} y:{:.3f} z:{:.3f}", GetName(), Adapter.meshname,adappos.x, adappos.y, adappos.z);

		oapiWriteLog(const_cast<char*>(logbuff.c_str()));
		
	}

	if(wLes==TRUE){
		
		VECTOR3 LesPos = Les.off;

		Les.msh_h = oapiLoadMeshGlobal(Les.meshname.c_str());

		logbuff = std::format("{}: Les Mesh Preloaded", GetName());

		oapiWriteLog(const_cast<char*>(logbuff.c_str()));

		Les.msh_idh = AddMesh(Les.msh_h, &LesPos);

		logbuff = std::format("{}: Les Mesh Added {} @ x:{:.3f} y:{:.3f} z:{:.3f}", GetName(), Les.meshname, LesPos.x,LesPos.y, LesPos.z);

		oapiWriteLog(const_cast<char*>(logbuff.c_str()));
	}
	
	return;
}

//Updates PMI, Cross Sections etc.
void Multistage2015::UpdatePMI(){

	double TotalVolume = 0.0;

	TotalHeight = 0.0;

	for(int i = currentStage; i < nStages; i++){

		TotalHeight += stage.at(i).height;

		stage.at(i).volume = stage.at(i).height * 0.25 * PI * stage.at(i).diameter * stage.at(i).diameter;

		TotalVolume += stage.at(i).volume;

		stage.at(i).interstage.volume = stage.at(i).interstage.height * 0.25 * PI * stage.at(i).interstage.diameter * stage.at(i).interstage.diameter;

		TotalVolume += stage.at(i).interstage.volume;

	}

	double CSBoosters = 0.0;

	for(int i = currentBooster; i < nBoosters; i++){
		booster.at(i).volume = booster.at(i).N * booster.at(i).height * 0.25 * PI * booster.at(i).diameter * booster.at(i).diameter;
		CSBoosters += 0.25 * PI * booster.at(i).diameter * booster.at(i).diameter;
	}

	for(int i = currentPayload; i < nPayloads; i++){
		payload.at(i).volume=payload.at(i).height * 0.25 * PI * payload.at(i).diameter * payload.at(i).diameter;

		TotalVolume += payload.at(i).volume;
	}

	if(wFairing == 1){
		fairing.volume=fairing.height * 0.25 * PI * fairing.diameter * fairing.diameter;
		TotalVolume += fairing.volume;
	}

	if(wLes == true){
		Les.volume = Les.height * 0.25 * PI * Les.diameter * Les.diameter;
		TotalVolume += Les.volume;
	}

	double PhiEq = sqrt(4 * TotalVolume / (PI * TotalHeight));

	double CSX,CSY,CSZ;

	CSX=TotalHeight * PhiEq;

	CSY = CSX;

	CSZ = PI * 0.25 * PhiEq * PhiEq + CSBoosters;

	SetCrossSections(_V(CSX, CSY, CSZ));

	double IZ=(PhiEq * 0.5) * (PhiEq * 0.5) * 0.5;
	
	double IX, IY;

	IX=(3 * (PhiEq * 0.5) * (PhiEq * 0.5) + TotalHeight * TotalHeight) / 12;

	IY = IX;

	SetPMI(_V(IX, IY, IZ));

	if(Configuration == 0){
		SetSize(stage.at(0).height * 0.5 + Misc.COG);
	} else {
		SetSize(TotalHeight);
	}

	return;
}

//Update Mass of the Vehicle on call
void Multistage2015::UpdateMass(){
	
	double EM = stage.at(currentStage).emptymass;
	
	for(int i = currentStage + 1; i < nStages; i++){
		
		EM+=stage.at(i).emptymass;

		if(stage.at(i).wInter == true){
			EM+=stage.at(i).interstage.emptymass;
		}
	}
	
	for(int i = currentBooster; i < nBoosters; i++){

		EM += (booster.at(i).emptymass * booster.at(i).N);

	}
		
	for(int i = currentPayload; i < nPayloads; i++){
		EM+=payload.at(i).mass;
	}

	if(wFairing == 1){
		EM += 2 * fairing.emptymass;
	}
	
	if(wAdapter == true){
		EM += Adapter.emptymass;
	}
	
	if(wLes == true){
		EM+=Les.emptymass;
	}

	SetEmptyMass(EM);
	
	return;
}

//Update mesh offsets
void Multistage2015::UpdateOffsets(){

	currentDelta=stage.at(currentStage).off.z;

	for(int i = currentStage; i < nStages; i++){
		
		stage.at(i).off.z -= currentDelta;
		
		if(stage.at(i).wInter == true){
			stage.at(i).interstage.off.z -= currentDelta;
		}
	}


	for(int i = currentPayload; i < nPayloads; i++){
		
		for(int j = 0; j < payload.at(j).nMeshes; j++){
			payload.at(i).off.at(j).z -= currentDelta;
		}

	}
		

	for(int i = currentBooster; i < nBoosters; i++){
		booster.at(i).off.z -= currentDelta;
	}

	if(wFairing == 1){
		fairing.off.z -= currentDelta;
	}

	if(wAdapter == true){
		Adapter.off.z -= currentDelta;
	}

	if(wLes == true){
		Les.off.z -= currentDelta;
	}

	return;
}

void Multistage2015::UpdateLivePayloads(){
	
	for(int i = currentPayload; i < nPayloads; i++){
		if(payload.at(i).live){
			VESSELSTATUS2 vslive;
			memset(&vslive,0,sizeof(vslive));
			vslive.version=2;
			OBJHANDLE checklive = oapiGetVesselByName(const_cast<char *>(payload.at(i).name.c_str()));
			if(oapiIsVessel(checklive)){
				ATTACHMENTHANDLE liveatt;
				VESSEL3 *livepl;
				livepl = (VESSEL3*)oapiGetVesselInterface(checklive);
			  	liveatt=livepl->CreateAttachment(true, _V(0, 0, 0), _V(0, 0, -1), _V(0, 1, 0), "MS2015", false);
			  	AttachChild(checklive,live_a.at(i),liveatt);
			  	if(payload.at(i).mass<=0){
				  	payload.at(i).mass=livepl->GetMass();}
			  	if(payload.at(i).height<=0){
					payload.at(i).height=livepl->GetSize();
					payload.at(i).diameter=payload.at(i).height * 0.1;
				}
			} else {

				VESSEL3 *v;
				OBJHANDLE hObj;
				ATTACHMENTHANDLE liveatt;
				GetStatusEx(&vslive);
				hObj=oapiCreateVesselEx(payload.at(i).name.c_str(), payload.at(i).module.c_str(), &vslive);
		
		
				if(oapiIsVessel(hObj)){
		            v = (VESSEL3*)oapiGetVesselInterface(hObj);
					
			  		liveatt=v->CreateAttachment(TRUE,_V(0,0,0),_V(0,0,-1),_V(0,1,0),"MS2015",false);
			 
			  		AttachChild(hObj,live_a.at(i),liveatt);

		      		if(payload.at(i).mass<=0){
				  		payload.at(i).mass=v->GetMass();
					}
			  		if(payload.at(i).height <= 0){
						payload.at(i).height = v->GetSize();
						payload.at(i).diameter=payload.at(i).height * 0.1;
					}
				}
			}
		}
	}
	
	UpdateMass();

	UpdatePMI();

	return;

}


//Returns the particlestream specification to use or the empty one if not found
PARTICLE Multistage2015::GetProperPS(std::string name) {

	std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
		return std::tolower(c);
		});


	std::string checktxt;

	for (unsigned int i = 0; i < 16; i++) {


		checktxt = Particle.at(i).ParticleName;


		std::transform(checktxt.begin(), checktxt.end(), checktxt.begin(), [](unsigned char c) {
			return std::tolower(c);
			});


		if(name == checktxt){

			return Particle.at(i);

		}

	}

	return Particle.at(15);

}

 //returns the texture to be used or the empty one
SURFHANDLE Multistage2015::GetProperExhaustTexture(const std::string name) {

	std::string checktxt;

	for (unsigned int i = 0; i < nTextures; i++) {

		checktxt = tex.TextureName[i];

		if (name == checktxt){

			return tex.hTex.at(i);

		}

	}

	return NULL;
}

//creates Ullage engine and exhausts
void Multistage2015::CreateUllageAndBolts(){
	
	if(stage.at(currentStage).ullage.wUllage){
		
		stage.at(currentStage).ullage.ignited = false;
		
		stage.at(currentStage).ullage.th_ullage = CreateThruster(_V(0, 0, 0), _V(0, 0, 1), stage.at(currentStage).ullage.thrust, stage.at(currentStage).tank, 100000);

		for(int i = 0; i < stage.at(currentStage).ullage.N; i++){
			
			double ull_angle;

			if(IsOdd((int)stage.at(currentStage).ullage.N)){
				
				if(i < (int)(stage.at(currentStage).ullage.N * 0.5)){
			 		
					ull_angle = stage.at(currentStage).ullage.angle * RAD + (i) * ((2 * PI / stage.at(currentStage).ullage.N) / stage.at(currentStage).ullage.rectfactor);

				} else if(i == (int)(stage.at(currentStage).ullage.N * 0.5)){ 
					ull_angle = PI + stage.at(currentStage).ullage.angle * RAD;
				} else {
				 	ull_angle = stage.at(currentStage).ullage.angle * RAD + PI + (i - (int)(stage.at(currentStage).ullage.N * 0.5)) * ((2 * PI / stage.at(currentStage).ullage.N) / stage.at(currentStage).ullage.rectfactor);
				}
			} else {
				ull_angle = stage.at(currentStage).ullage.angle * RAD + (i) * (2 * PI / stage.at(currentStage).ullage.N);
			}

			VECTOR3 ulldir = RotateVecZ(stage.at(currentStage).ullage.dir, ull_angle);

			VECTOR3 ullpos = RotateVecZ(stage.at(currentStage).ullage.pos, ull_angle);

			AddExhaust(stage.at(currentStage).ullage.th_ullage, stage.at(currentStage).ullage.length, stage.at(currentStage).ullage.diameter, ullpos, ulldir, GetProperExhaustTexture(stage.at(currentStage).ullage.tex));
		}
	}

	if(stage.at(currentStage).expbolt.wExpbolt){
		
		stage.at(currentStage).expbolt.threxp_h = CreateThruster(stage.at(currentStage).expbolt.pos, stage.at(currentStage).expbolt.dir, 0, stage.at(currentStage).tank, 100000, 100000);

		PARTICLE ps = GetProperPS(stage.at(currentStage).expbolt.pstream);

		AddExhaustStream(stage.at(currentStage).expbolt.threxp_h, &ps.Pss);
	}

	return;
}

//Setting up of the vehicle, it's the setclass caps, but called after the ini is parsed
void Multistage2015::VehicleSetup(){
	SetRotDrag(_V(0.7,0.7,0.06)); //Got from Vinka
	
 	///PROPELLANTS
 	for(int i = currentBooster; i < nBoosters; i++){
  		
		booster.at(i).tank = CreatePropellantResource(booster.at(i).fuelmass * booster.at(i).N);

  		logbuff = std::format("{}: booster n. {} Tank Added: {:.3f} kg", GetName(), i + 1, booster.at(i).fuelmass * booster.at(i).N);

  		oapiWriteLog(const_cast<char *>(logbuff.c_str()));
 	}
	

	for(int i = nStages - 1; i > currentStage - 1; i--){
	 	stage.at(i).tank = CreatePropellantResource(stage.at(i).fuelmass);

	 	logbuff = std::format("{}: Stage n. {} Tank Added: {:.3f} kg", GetName(), i + 1, stage.at(i).fuelmass);
	 	
		oapiWriteLog(const_cast<char *>(logbuff.c_str()));
 	}
 
 	////ISP

 	for(int i = currentStage; r < nStages; r++){
	 	stage.at(i).isp = stage.at(i).thrust * stage.at(i).burntime / stage.at(i).fuelmass;
	}

 	for(int i = currentBooster; i < nBoosters; i++){
	 	booster.at(i).isp = (booster.at(i).thrust * booster.at(i).N) * booster.at(i).burntime / (booster.at(i).fuelmass * booster.at(i).N);
 	}



	nPsh=0;

	for(int i = 0; i < nStages; i++){
		for(int j = 0; j < stage.at(i).nEngines; j++){
			nPsh++;
		}
	}

	for(int i = 0; i < nBoosters; i++){
		for(int j = 0; j < booster.at(i).N; j++){
			for(int k = 0; k < booster.at(i).nEngines; k++){
				nPsh++;
			}
		}
	}

	nPsh*=2;

	psg = new PSGROWING[nPsh];

	for(int i = 0; i < nPsh; i++){
		psg[i].GrowFactor_rate = 0;
		psg[i].GrowFactor_size = 0;
		psg[i].growing = false;
		psg[i].pos = _V(0, 0, 0);
		psg[i].psh.at(0) = nullptr;
		psg[i].psh.at(1) = nullptr;
		psg[i].psh[2] = nullptr;
		
		psg[i].th = nullptr;
		psg[i].pss = Particle.at(15).Pss;
		psg[i].status = 1;
		psg[i].counting = false;
		psg[i].doublepstime = 0;
		psg[i].baserate = 0;
		psg[i].basesize = 0;
		psg[i].basepos = _V(0, 0, 0);
		psg[i].FirstLoop = true;
	}
	/////MAIN THRUSTERS
 
 	CreateMainThruster();
 
 	//attitude thrusters
 	CreateRCS();
 
 	//add Boosters Engines
 	for(int i = currentBooster; i < nBoosters; i++){
	 	for(int j = 0; j < booster.at(i).N; j++){
			VECTOR3 pos, dir;
			if(Misc.thrustrealpos){
				pos = GetBoosterPos(i, j);
				dir = booster.at(i).eng_dir;
			}else{
				pos=_V(0, 0, 0);
				dir=_V(0, 0, 1);
			}
		
			booster.at(i).th_booster_h.at(j) = CreateThruster(pos, dir, booster.at(i).thrust, booster.at(i).tank, booster.at(i).isp);
		}
		
		booster.at(i).Thg_boosters_h = CreateThrusterGroup(booster.at(i).th_booster_h.data(), booster.at(i).N, THGROUP_USER);
	
	 	Particle.at(13).Pss.srcsize = stage.at(0).diameter;
	 	Particle.at(14).Pss.srcsize = 0.5 * stage.at(0).diameter;

		if(booster.at(i).nEngines == 0){

	 		for(int j = 0; j < booster.at(i).N; j++){

		  		booster.at(i).eng.at(j) = _V(0, 0, -booster.at(i).height * 0.5);

	 			VECTOR3 engofs = operator+(GetBoosterPos(i, j), booster.at(i).eng.at(j));

				AddExhaust(booster.at(i).th_booster_h.at(j), 10 * booster.at(i).eng_diameter, booster.at(i).eng_diameter, engofs, operator*(booster.at(i).eng_dir, -1), GetProperExhaustTexture(booster.at(i).eng_tex));

				if(booster.at(i).wps1){
					PARTICLE ps = GetProperPS(booster.at(i).eng_pstream1);
					AddExhaustStreamGrowing(booster.at(i).th_booster_h.at(j), engofs, &ps.Pss, GetProperPS(booster.at(i).eng_pstream1).Growing, GetProperPS(booster.at(i).eng_pstream1).GrowFactor_size, GetProperPS(booster.at(i).eng_pstream1).GrowFactor_rate, true, true, i, j);

					logbuff = std::format("{}: Booster Group n.{} Engine Exhaust Stream Added: {} to engine n.{}", GetName(), i + 1, booster.at(i).eng_pstream1, j + 1);

					oapiWriteLog(const_cast<char *>(logbuff.c_str()));
				}

				if(booster.at(i).wps2){
					PARTICLE ps = GetProperPS(booster.at(i).eng_pstream2);
					AddExhaustStreamGrowing(booster.at(i).th_booster_h.at(j), engofs, &ps.Pss, GetProperPS(booster.at(i).eng_pstream2).Growing, GetProperPS(booster.at(i).eng_pstream2).GrowFactor_size, GetProperPS(booster.at(i).eng_pstream2).GrowFactor_rate, true, true, i, j);

					logbuff = std::format("{}: Booster Group n.{} Engine Exhaust Stream Added: {} to engine n.{}", GetName(), i + 1, booster.at(i).eng_pstream2, j + 1);

					oapiWriteLog(const_cast<char *>(logbuff.c_str()));

				}

				logbuff = std::format("{}: Booster Engines Exhaust Added--> Booster Group: {} number of engines: {} , diameter: {:.3f}, position x: {:.3f} y: {:.3f} z: {:.3f}", GetName(), i + 1, booster.at(i).nEngines, booster.at(i).eng_diameter, engofs.x, engofs.y, engofs.z);

				oapiWriteLog(const_cast<char *>(logbuff.c_str()));
	 		}

		} else {
	  
			for(int j = 0; j < booster.at(i).nEngines; j++){

				for(int k = 1; k < booster.at(i).N + 1; k++){

					double angle = booster.at(i).angle * RAD + (k - 1) * 2 * PI / booster.at(i).N;
					
					VECTOR3 engofs = operator+(GetBoosterPos(i, k), RotateVecZ(booster.at(i).eng.at(j), angle));
					
					AddExhaust(booster.at(i).th_booster_h.at(k - 1), 10 * booster.at(i).eng_diameter, booster.at(i).eng_diameter, engofs, operator*(booster.at(i).eng_dir, -1), GetProperExhaustTexture(booster.at(i).eng_tex));

					if(booster.at(i).wps1){
						PARTICLE ps = GetProperPS(booster.at(i).eng_pstream1);
						AddExhaustStreamGrowing(booster.at(i).th_booster_h.at(k - 1), engofs, &ps.Pss, GetProperPS(booster.at(i).eng_pstream1).Growing, GetProperPS(booster.at(i).eng_pstream1).GrowFactor_size, GetProperPS(booster.at(i).eng_pstream1).GrowFactor_rate, true, true, i, k - 1);

						logbuff = std::format("{}: Booster Group n.{} Engine Exhaust Stream Added: {} to engine n.{}",GetName(), i + 1, booster.at(i).eng_pstream1, k);

						oapiWriteLog(const_cast<char *>(logbuff.c_str()));
					}

					if(booster.at(i).wps2){
						PARTICLE ps = GetProperPS(booster.at(i).eng_pstream2);
						AddExhaustStreamGrowing(booster.at(i).th_booster_h.at(k - 1),engofs, &ps.Pss, GetProperPS(booster.at(i).eng_pstream2).Growing, GetProperPS(booster.at(i).eng_pstream2).GrowFactor_size, GetProperPS(booster.at(i).eng_pstream2).GrowFactor_rate, true, true, i, k - 1);

						logbuff = std::format("{}: Booster Group n.{} Engine Exhaust Stream Added: {} to engine n.{}",GetName(), i + 1, booster.at(i).eng_pstream2, k);
						
						oapiWriteLog(const_cast<char *>(logbuff.c_str()));

					}

					logbuff = std::format("{}: Booster Engines Exhaust Added--> Booster Group: {} number of engines: {} , diameter: {:.3f}, position x: {:.3f} y: {:.3f} z: {:.3f}", GetName(), i + 1, booster.at(i).nEngines, booster.at(i).eng_diameter, engofs.x, engofs.y, engofs.z);
					
					oapiWriteLog(const_cast<char *>(logbuff.c_str()));
				}
			}
		}

		if(booster.at(i).expbolt.wExpbolt){
			booster.at(i).expbolt.threxp_h = CreateThruster(booster.at(i).expbolt.pos, booster.at(i).expbolt.dir, 0, booster.at(i).tank, 100000, 100000);
			PARTICLE ps = GetProperPS(booster.at(i).expbolt.pstream);
			AddExhaustStream(booster.at(i).expbolt.threxp_h, &ps.Pss);
		}
	}

 	//Ullage
 	CreateUllageAndBolts();


	CogElev=Misc.COG;

 	//added by rcraig42 to set drag_factor to 1 if not set in ini ------------------------------------------------

	
	if(Misc.drag_factor <= 0){
		Misc.drag_factor = 1;
	}

 	SetCW(0.2 * Misc.drag_factor, 0.5, 1.5, 1.5);		// Modded to miltiply drag in z_pos direction by drag_factor

//-----------------------------------------------------------------------------------------------------------

	std::array<VECTOR3, 3> intdpts;
	std::array<VECTOR3, 12> tdpts;

 //////////////TOUCHDOWN POINTS WORKING ORIGINAL MS WAY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

 //////////////////////////////////////////////////END//////////////////////////////////////////////////



 /////////////////NEW DEFINITION FOR RAMP WITH ATTACHMENT!!!!!
 	intdpts.at(0) = _V(0, 1, 0);
 	intdpts.at(2) = _V(-0.866, -0.5, 0);
 	intdpts.at(1) = _V(0.866, -0.5, 0);

 	for(int i = 0; i < 3; i++){
	 	tdpts.at(i) = intdpts.at(i);
	}
 //////////////////////END//////////////////////////////////////


 	tdpts.at(3) = tdpts.at(0);
 	tdpts.at(4) = tdpts.at(1);
 	tdpts.at(5) = tdpts.at(2);

 	tdpts.at(3).z *= -0.1;
 	tdpts.at(4).z *= -0.1;
	tdpts.at(5).z *= -0.1;

	std::array<TOUCHDOWNVTX, 12> td;
 	for(int i=0;i<12;i++){
	 	td.at(i).pos = tdpts.at(i);
		td.at(i).damping = 3e6;
	 	td.at(i).mu = 3;
	 	td.at(i).mu_lng = 3;
	 	td.at(i).stiffness = 3e7;
	}

	SetTouchdownPoints(td.data(), 6);

  	if(currentBooster < nBoosters) wBoosters = true;

 	UpdateMass();
 	UpdatePMI();

	return;
}

//Spawns Item
void Multistage2015::Spawn(int type, int current){

	std::string mn;
	VESSELSTATUS2 vs;
	memset(&vs,0,sizeof(vs));
	vs.version=2;
	GetStatusEx(&vs);
	VECTOR3 ofs;
	VECTOR3 rofs, rvel = {vs.rvel.x, vs.rvel.y, vs.rvel.z}, vrot = {vs.vrot.x ,vs.vrot.y, vs.vrot.z}, arot = {vs.arot.x, vs.arot.y, vs.arot.z};
	VECTOR3 vel; 

	switch(type){

		case TBOOSTER:

			for(int i=1; i < booster.at(current).N+1; i++){

				GetMeshOffset(booster.at(current).msh_idh.at(i), ofs);
				
				vel = RotateVecZ(booster.at(current).speed, booster.at(current).angle * RAD +(i - 1) * 2 * PI / booster.at(current).N);

				Local2Rel(ofs, vs.rpos);

				GlobalRot(vel, rofs);

				vs.rvel.x = rvel.x + rofs.x;

				vs.rvel.y = rvel.y + rofs.y;
				
				vs.rvel.z = rvel.z + rofs.z;

				double arg = booster.at(current).angle * RAD + (i - 1) * 2 * PI / booster.at(current).N;

				vs.vrot.x = vrot.x + booster.at(current).rot_speed.x * cos(arg) - booster.at(current).rot_speed.y * sin(arg);

				vs.vrot.y = vrot.y + booster.at(current).rot_speed.x * sin(arg) + booster.at(current).rot_speed.y * cos(arg);
				
				vs.vrot.z = vrot.z + booster.at(current).rot_speed.z;
				
				std::string mn2;
				mn2 = std::format("_{}", i);
				mn = booster.at(current).meshname;
				mn += mn2;


				oapiCreateVesselEx(mn.c_str() ,booster.at(current).module.c_str(), &vs);

				logbuff = std::format("{}: Booster n.{} jettisoned name: {} @{:.3f}", GetName(), current + 1, mn, MET);

				oapiWriteLog(const_cast<char *>(logbuff.c_str()));

			}
		break;

		case TSTAGE:

			GetMeshOffset(stage.at(current).msh_idh, ofs);

			vel = _V(stage.at(current).speed.x, stage.at(current).speed.y, stage.at(current).speed.z);

			Local2Rel (ofs, vs.rpos);

			GlobalRot(vel, rofs);

			vs.rvel.x = rvel.x + rofs.x;

			vs.rvel.y = rvel.y + rofs.y;

			vs.rvel.z = rvel.z + rofs.z;

			vs.vrot.x = vrot.x + stage.at(current).rot_speed.x;

			vs.vrot.y = vrot.y + stage.at(current).rot_speed.y;
			
			vs.vrot.z = vrot.z + stage.at(current).rot_speed.z;

			mn = stage.at(current).meshname;

			oapiCreateVesselEx(mn.c_str(), stage.at(current).module.c_str(), &vs);
			
			logbuff = std::format("{}: Stage n.{} jettisoned name: {} @{:.3f}", GetName(), current + 1, mn, MET);
			
			oapiWriteLog(const_cast<char *>(logbuff.c_str()));

			stage_ignition_time = MET;
		break;

	case TPAYLOAD:

		if(!payload.at(current).live){

			GetMeshOffset(payload.at(current).msh_idh.at(0), ofs);

			vel = _V(payload.at(current).speed.x, payload.at(current).speed.y, payload.at(current).speed.z);

			Local2Rel (ofs, vs.rpos);

			GlobalRot(vel, rofs);

			vs.rvel.x = rvel.x + rofs.x;

			vs.rvel.y = rvel.y + rofs.y;

			vs.rvel.z = rvel.z + rofs.z;

			vs.vrot.x = vrot.x + payload.at(current).rot_speed.x;
			
			vs.vrot.y = vrot.y + payload.at(current).rot_speed.y;

			vs.vrot.z = vrot.z + payload.at(current).rot_speed.z;
			
			if(payload.at(current).rotated){
				MATRIX3 RotMatrix, RotMatrix_Def;
				GetRotationMatrix(RotMatrix);
				VECTOR3 rotation;
				rotation = payload.at(current).Rotation;
				RotMatrix_Def  =mul(RotMatrix, RotationMatrix(rotation));
				vs.arot.x = atan2(RotMatrix_Def.m23, RotMatrix_Def.m33);
				vs.arot.y = -asin(RotMatrix_Def.m13);
				vs.arot.z = atan2(RotMatrix_Def.m12, RotMatrix_Def.m11);
			}

			OBJHANDLE hpl;

			hpl = oapiCreateVesselEx(payload.at(current).name.c_str(), payload.at(current).module.c_str(), &vs);
			if(currentPayload + 1 == Misc.Focus){
				oapiSetFocusObject(hpl);
			}

		} else {

			if(GetAttachmentStatus(live_a.at(current))){
				OBJHANDLE live=GetAttachmentStatus(live_a.at(current));
				VESSEL3 *v;
				v = (VESSEL3*)oapiGetVesselInterface(live);
				
				VECTOR3 dir,rot; //only for the get params
				GetAttachmentParams(live_a.at(current), ofs, dir, rot);
				DetachChild(live_a.at(current), 0);//length(payload.at(current).speed));
			
				vel = _V(payload.at(current).speed.x, payload.at(current).speed.y, payload.at(current).speed.z);
				Local2Rel (ofs, vs.rpos);
				GlobalRot(vel, rofs);
				vs.rvel.x = rvel.x + rofs.x;
				vs.rvel.y = rvel.y + rofs.y;
				vs.rvel.z = rvel.z + rofs.z;
				vs.vrot.x = vrot.x + payload.at(current).rot_speed.x;
				vs.vrot.y = vrot.y + payload.at(current).rot_speed.y;
				vs.vrot.z = vrot.z + payload.at(current).rot_speed.z;
				
				if(payload.at(current).rotated){
					MATRIX3 RotMatrix,RotMatrix_Def;
					GetRotationMatrix(RotMatrix);
					VECTOR3 rotation;
					rotation=payload.at(current).Rotation;
					RotMatrix_Def = mul(RotMatrix, RotationMatrix(rotation));
					vs.arot.x = atan2(RotMatrix_Def.m23, RotMatrix_Def.m33);
					vs.arot.y = -asin(RotMatrix_Def.m13);
					vs.arot.z = atan2(RotMatrix_Def.m12, RotMatrix_Def.m11);
				}
				v->clbkSetStateEx(&vs);
				if(currentPayload + 1 == Misc.Focus){
					oapiSetFocusObject(live);
				}
			}
		}
		logbuff = std::format("{}: Payload n.{} jettisoned name: {} @{:.3f}", GetName(), current + 1, payload.at(current).name, MET);

		oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		
		break;

	case TFAIRING:

		for(int i = 1; i < fairing.N + 1; i++){
					
			GetMeshOffset(fairing.msh_idh.at(i),ofs);

			vel = RotateVecZ(fairing.speed, fairing.angle * RAD + (i - 1) * 2 * PI / fairing.N);
			
			Local2Rel (ofs, vs.rpos);

			GlobalRot(vel, rofs);

			vs.rvel.x = rvel.x + rofs.x;
			vs.rvel.y = rvel.y + rofs.y;
			vs.rvel.z = rvel.z + rofs.z;

			double arg = (i - 1) * 2 * PI / fairing.N;

			vs.vrot.x = vrot.x + fairing.rot_speed.x * cos(arg) - fairing.rot_speed.y * sin(arg);

			vs.vrot.y = vrot.y + fairing.rot_speed.x * sin(arg) + fairing.rot_speed.y * cos(arg);

			vs.vrot.z = vrot.z + fairing.rot_speed.z;

			std::string mn2;
			mn2 = std::format("_{}", i);
			mn = fairing.meshname;
			mn += mn2;

			oapiCreateVesselEx(mn.c_str(), fairing.module.c_str(), &vs);
			logbuff = std::format("{}: Fairing jettisoned: name {} @{:.3f}", GetName(), mn, MET);
			
			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		}

		break;

case TINTERSTAGE:

		GetMeshOffset(stage.at(current).interstage.msh_idh, ofs);

		vel = _V(stage.at(current).interstage.speed.x, stage.at(current).interstage.speed.y, stage.at(current).interstage.speed.z);
		
		Local2Rel (ofs, vs.rpos);
		
		GlobalRot(vel, rofs);

		vs.rvel.x = rvel.x + rofs.x;

		vs.rvel.y = rvel.y + rofs.y;

		vs.rvel.z = rvel.z + rofs.z;

		vs.vrot.x = vrot.x + stage.at(current).interstage.rot_speed.x;

		vs.vrot.y = vrot.y + stage.at(current).interstage.rot_speed.y;

		vs.vrot.z = vrot.z + stage.at(current).interstage.rot_speed.z;

		mn = stage.at(current).interstage.meshname;

		oapiCreateVesselEx(mn.c_str(), stage.at(current).interstage.module.c_str(), &vs);

		logbuff = std::format("{}: Interstage of stage {} jettisoned name: {} @{:.3f}", GetName(), current + 1, mn, MET);

		oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		
		break;
case TLES:
		
		GetMeshOffset(Les.msh_idh, ofs);

		vel = _V(Les.speed.x, Les.speed.y, Les.speed.z);

		Local2Rel (ofs, vs.rpos);

		GlobalRot(vel, rofs);

		vs.rvel.x = rvel.x + rofs.x;

		vs.rvel.y = rvel.y + rofs.y;

		vs.rvel.z = rvel.z + rofs.z;

		vs.vrot.x = vrot.x + Les.rot_speed.x;

		vs.vrot.y = vrot.y + Les.rot_speed.y;

		vs.vrot.z = vrot.z + Les.rot_speed.z;

		mn = Les.meshname;

		oapiCreateVesselEx(mn.c_str(), Les.module.c_str(), &vs);

		logbuff = std::format("{}: Les jettisoned name: {} @{:.3f}", GetName(), mn, MET);

		oapiWriteLog(const_cast<char *>(logbuff.c_str()));

		break;

	}

	return;
}

//Jettison item
void Multistage2015::Jettison(int type,int current){

	switch(type){

		case TBOOSTER:
			Spawn(type, current);

			for(int i = 1; i < booster.at(current).N + 1; i++){
				DelMesh(booster.at(current).msh_idh.at(i));
			}

			//DelThruster(booster.at(current).th_booster_h.at(0));
			DelThrusterGroup(booster.at(current).Thg_boosters_h, true);
			
			DelPropellantResource(booster.at(current).tank);
			currentBooster += 1;
			
			UpdateMass();
			UpdatePMI();

			if(currentBooster >= nBoosters){
				wBoosters = false;
			}

		break;

		case TSTAGE:
			
			Spawn(type, current);
			
			DelMesh(stage.at(current).msh_idh);

			ClearThrusterDefinitions();
			
			DelPropellantResource(stage.at(current).tank);

			currentStage += 1;
			
			UpdateMass();

			UpdatePMI();

			CreateUllageAndBolts();

			CreateMainThruster();

			CreateRCS();

			ShiftCG(_V(0, 0, (stage[current + 1].off.z - stage.at(current).off.z)));			
			
			SetCameraOffset(_V(0, 0, 0));

			break;

		case TPAYLOAD:
			
			Spawn(type,current);
			
			if(!payload.at(current).live){
				for(int i = 0; i < payload.at(current).nMeshes; i++){
					DelMesh(payload.at(current).msh_idh.at(i));
				}
			}

			currentPayload += 1;
			
			UpdateMass();
			
			UpdatePMI();

			break;

		case TFAIRING:

			Spawn(type, current);

			for(int i = currentPayload; i < nPayloads; i++){
				if(!payload.at(i).live){
					for(int j = 0; j < payload.at(i).nMeshes; j++){
						SetMeshVisibilityMode(payload.at(i).msh_idh.at(j), MESHVIS_EXTERNAL);
					}
				}
			}

			for(int i = 1; i < fairing.N + 1; i++){
				DelMesh(fairing.msh_idh.at(i));
			}

			
			wFairing = 0;

			UpdateMass();

			UpdatePMI();

			break;

		case TLES:

			Spawn(type, current);

			DelMesh(Les.msh_idh);

			wLes = false;

			UpdateMass();

			UpdatePMI();

			break;

	case TINTERSTAGE:
		
		Spawn(type, current);

		DelMesh(stage.at(current).interstage.msh_idh);
		currentInterstage += 1;
		stage.at(current).wInter = false;
		UpdateMass();
		UpdatePMI();
		break;

	}

	return;
}

//initialize correctly the delays if flying from a scenario not starting on ground
void Multistage2015::InitializeDelays(){

	//initialize booster burn delays, only if stage is 0 and if Met is positive
	if((currentStage == 0) && (MET > 0)){
		for(int i = currentBooster; i < nBoosters; i++){
			if(booster.at(i).currDelay > 0){
				booster.at(i).currDelay -= MET;
			}
		}
	} else if(currentStage > 0){//initialize stages using already used by vinka STAGE_IGNITION_TIME even if it should be called "STAGE_JETTISONED_TIME"
		double delta = MET - stage_ignition_time;
		if(delta < stage.at(currentStage).currDelay){
			stage.at(currentStage).currDelay -= delta;
		}

	}

	return;
}


void Multistage2015::AutoJettison(){
	
	if(currentBooster < nBoosters){
		if(GetPropellantMass(booster.at(currentBooster).tank) <= 0.000001){
			Jettison(TBOOSTER, currentBooster);
		}
	}

	if(currentStage < nStages - 1){
		if((currentStage == 0) && (currentBooster < nBoosters)){
			return;
		} else if(GetPropellantMass(stage.at(currentStage).tank) <= 0.1){
				Jettison(TSTAGE, currentStage);
		}
	}

	if((stage.at(currentStage).wInter == true) && (stage.at(currentStage).interstage.currDelay <= 0)){
		Jettison(TINTERSTAGE, currentStage);
	}

	return;
}

void Multistage2015::Guidance_Debug(){
	int step = VinkaGetStep(MET);
	double DesiredPitch;
	if(Gnc_step.at(step).GNC_Comand==CM_ROLL){
		DesiredPitch = (Gnc_step.at(step).val_init + (Gnc_step.at(step).val_fin-Gnc_step.at(step).val_init) * (MET - Gnc_step.at(step).time_init) / ((Gnc_step.at(VinkaFindFirstPitch()).time - 1) - Gnc_step.at(step).time_init)) * RAD;
	
		double heading;
		
		oapiGetHeading(GetHandle(), &heading);
		
		sprintf(oapiDebugString(), "MET: %.1f Step: %i P: %.2f (%.2f) H: %.2f (%.2f)", MET, step, GetPitch() * DEG,DesiredPitch * DEG, heading * DEG, VinkaAzimuth * DEG);

	}else if(Gnc_step.at(step).GNC_Comand == CM_PITCH){
		DesiredPitch = (Gnc_step.at(step).val_init + (Gnc_step.at(step).val_fin - Gnc_step.at(step).val_init) * (MET - 
		Gnc_step.at(step).time_init) / (Gnc_step.at(step).time_fin - Gnc_step.at(step).time_init)) * RAD;

		sprintf(oapiDebugString(), "MET: %.1f Step: %i P: %.2f (%.2f) Delta: %.1f", MET, step, GetPitch() * DEG, DesiredPitch * DEG, GetPitch() * DEG - DesiredPitch * DEG);
	}else{
		sprintf(oapiDebugString(), "MET: %.1f Step: %i", MET, step);
	}

	return;
}

void Multistage2015::ComplexFlight(){

	UpdateComplex += oapiGetSimStep();

	if(UpdateComplex >= 1){
		
		UpdateComplex=0;

		for(int i=0;i<stage.at(currentStage).nEngines;i++){
			
			double newMax = (stage.at(currentStage).thrust / stage.at(currentStage).nEngines) * (1 + (stage.at(currentStage).engine_amp.at(i) * sin( 2 * PI / stage.at(currentStage).freq.at(i) * MET + stage.at(currentStage).engine_phase.at(i))));

			SetThrusterMax0(stage.at(currentStage).th_main_h.at(i),newMax);
		}

		if(wBoosters)
		{
			for(int i = 0; i < booster.at(currentBooster).N; i++){
				
				double newMax = booster.at(currentBooster).thrust * (1 + (booster.at(currentBooster).engine_amp.at(i) * sin(2 * PI / booster.at(currentBooster).freq.at(i) * MET + booster.at(currentBooster).engine_phase.at(i))));
				
				SetThrusterMax0(booster.at(currentBooster).th_booster_h.at(i), newMax);
			}
		}
	}


	return;
}

void Multistage2015::Boiloff(){

	updtboiloff += oapiGetSimStep();

	if(updtboiloff >= 3600){
		updtboiloff = 0;
		for(int i = currentStage; i < nStages; i++){
			if(stage.at(i).wBoiloff){

				double propmass = GetPropellantMass(stage.at(i).tank);

				propmass -= 1;

				SetPropellantMass(stage.at(i).tank, propmass);

			}
		}
	}
	return;
}

void Multistage2015::FLY(double simtime, double simdtime, double mjdate){
	
	if(APstat){
		if(stage.at(currentStage).currDelay > 0){
			stage.at(currentStage).currDelay -= simdtime;
			stage.at(currentStage).StageState = STAGE_WAITING;
		}

		if(stage.at(currentStage).interstage.currDelay > 0){
			stage.at(currentStage).interstage.currDelay -= simdtime;
		}

	}

	if(wBoosters){
		for(int i = 0; i < nBoosters; i++){
			if(booster.at(i).currDelay > 0){
				booster.at(i).currDelay -= simdtime;
			}
		}
	}

	if(!AJdisabled){
		AutoJettison();
	}
	
	std::array<double, 10> Level;
	std::array<double, 10> btime;

	if((!stage.at(currentStage).Ignited) && (stage.at(currentStage).currDelay <= 0) && (stage.at(currentStage).StageState == STAGE_WAITING)){

		SetThrusterGroupLevel(THGROUP_MAIN, 1);

		stage.at(currentStage).Ignited = true;

		stage.at(currentStage).IgnitionTime = MET;

		stage.at(currentStage).StageState = STAGE_IGNITED;

		logbuff = std::format("{} Stage n: {} ignited @{:.1f}", GetName(), currentStage + 1, stage.at(currentStage).IgnitionTime);
		
		oapiWriteLog(const_cast<char *>(logbuff.c_str()));
	}

	//BOOSTERS SECTION
	if((wBoosters) && (GetThrusterGroupLevel(THGROUP_MAIN) > 0.95)){
	
		for(int i = currentBooster; i < nBoosters; i++){

			if(booster.at(i).currDelay <= 0){

				if(booster.at(i).Ignited == false){
					booster.at(i).Ignited = false;
					booster.at(i).IgnitionTime = MET;
					
					logbuff = std::format("{} Booster n: {} ignited @{:.1f}", GetName(), i + 1, booster.at(i).IgnitionTime);

					oapiWriteLog(const_cast<char *>(logbuff.c_str()));

				}else{

					btime.at(i) = MET-booster.at(i).IgnitionTime;

					Level.at(i) = 1;
					
					double m, q;
					
					for(int j = 0; j < 10; j++){
							
						if(btime.at(i) > booster.at(i).curve.at(j).x){

							if(j < 9){

								if(btime.at(i)<booster.at(i).curve.at(j+1).x){
							
									m = (booster.at(i).curve.at(j + 1).y - booster.at(i).curve.at(j).y) / (booster.at(i).curve.at(j + 1).x - booster.at(i).curve.at(j).x);

									q = booster.at(i).curve.at(j).y - m * booster.at(i).curve.at(j).x;
									Level.at(i)=(m * btime.at(i) + q) / 100;
								}
													
							} else {

								m = (booster.at(i).curve.at(j).y - booster.at(i).curve.at(j).y) / (booster.at(i).curve.at(j).x - booster.at(i).curve.at(j).x);

								q = booster.at(i).curve.at(j).y - m * booster.at(i).curve.at(j).x;
								
								Level.at(i) = (m * btime.at(i) + q) / 100;
							}
						}		
					}
				}

				SetThrusterGroupLevel(booster.at(i).Thg_boosters_h, Level.at(i));
			}
		}
	}
	
	if(APstat){
		
		VinkaAutoPilot();

		if(Misc.GNC_Debug == 1){
			Guidance_Debug();
		}

		if(MET > VinkaFindEndTime()){
			
			killAP();

			APstat = false;
		}
	}

	//avoid reignition of not reignitable stages by setting thurst to 0
	if((!stage.at(currentStage).reignitable) && (stage.at(currentStage).Ignited)){
		
		if(GetThrusterGroupLevel(THGROUP_MAIN) == 0){
			
			stage.at(currentStage).waitforreignition += simdtime;

			if(stage.at(currentStage).waitforreignition >= 3){

				for(int i = 0; i < stage.at(currentStage).nEngines; i++){
					SetThrusterResource(stage.at(currentStage).th_main_h.at(i), nullptr);
				}
				
				stage.at(currentStage).DenyIgnition = true;
			
			}
		}
	}

	if(tlmidx < TLMSECS){
		Telemetry();
	}

	if(stage.at(currentStage).batteries.wBatts){
		stage.at(currentStage).batteries.CurrentCharge -= oapiGetSimStep();
		if(stage.at(currentStage).batteries.CurrentCharge <= 0){
			stage.at(currentStage).batteries.CurrentCharge = 0;
			ClearThrusterDefinitions();
		}
	}

	if((Complex) && (GetDrag() > 1000)){
		ComplexFlight();

		AddForce(_V(0, 2 * GetDrag() * sin(GetAOA()), 0), _V(0, 0, TotalHeight));

		AddForce(_V( 2 * GetDrag() * sin(GetSlipAngle()), 0, 0), _V(0, 0, TotalHeight));

		if(GetDrag() > 500000){
			if((abs(GetAOA()) > 45 * RAD) || (abs(GetSlipAngle()) > 45 * RAD)){
				boom();
			}
		}
	}
	
	if(stage.at(currentStage).ullage.wUllage){
		if((!stage.at(currentStage).ullage.ignited) && (stage.at(currentStage).currDelay < stage.at(currentStage).ullage.anticipation)){

			SetThrusterLevel(stage.at(currentStage).ullage.th_ullage, 1);

			stage.at(currentStage).ullage.ignited = true;

		} else if((stage.at(currentStage).ullage.ignited) && (stage.at(currentStage).IgnitionTime != 0) && (MET - stage.at(currentStage).IgnitionTime > stage.at(currentStage).ullage.overlap)){
			SetThrusterLevel(stage.at(currentStage).ullage.th_ullage, 0);
		}
	}

	if(stage.at(currentStage).expbolt.wExpbolt){
		if(RemBurnTime(currentStage)<stage.at(currentStage).expbolt.anticipation){
			SetThrusterLevel(stage.at(currentStage).expbolt.threxp_h, 1);
		}
	}


	if(booster.at(currentBooster).expbolt.wExpbolt){
		if(BoosterRemBurnTime(currentBooster)<booster.at(currentBooster).expbolt.anticipation){
			SetThrusterLevel(booster.at(currentBooster).expbolt.threxp_h, 1);
		}
	}

	MET += simdtime;

	return;

}

double Multistage2015::CalculateFullMass(){

	double FM=0;

	for(int i=0;i<nStages;i++){
		FM += stage.at(i).emptymass;
		FM += stage.at(i).fuelmass;
		
		if(stage.at(i).wInter){
			FM += stage.at(i).interstage.emptymass;
		}
	}

	for(int i = 0; i < nPayloads; i++){
		FM += payload.at(i).mass;
	}

	for(int i = 0; i < nBoosters; i++){
		FM += booster.at(i).fuelmass * booster.at(i).N;
		FM += booster.at(i).emptymass * booster.at(i).N;
	}

	
	if(hasFairing){
		FM += fairing.emptymass;
	}

	if(wAdapter){
		FM += Adapter.emptymass;
	}

	if(wLes){
		FM += Les.emptymass;
	}
	
	return FM;
}

//returns hours mins and secs of a time (positive and negative)
VECTOR3 Multistage2015::hms(double time){

	VECTOR3 met;

	if(time == 0){
		met=_V(0,0,0);
	} else {

		time = abs(time + 0.5 * (time / abs(time) - 1));

		met.x = floor(time / 3600) - 0.5 * (time / abs(time) - 1);

		met.y = floor((time - met.x * 3600 * (time / abs(time))) / 60) -0.5 * (time / abs(time) - 1);

		met.z = floor(time - met.x * 3600 * (time / abs(time)) - met.y * 60 * (time / abs(time)));
	}

	return met;
}

double Multistage2015::GetProperNforCGTE(double time){
	double n;
	double Thrust=stage.at(0).thrust;
	double BoosterFlow=0;
	double BoosterFuelMassBurnt=0;
	for(int i=0;i<nBoosters;i++){
		if((booster.at(i).burndelay<time)&&(time<(booster.at(i).burndelay+booster.at(i).burntime))){
		Thrust+=booster.at(i).thrust*booster.at(i).N;
	BoosterFlow=((booster.at(i).fuelmass*booster.at(i).N)/booster.at(i).burntime);
	BoosterFuelMassBurnt=BoosterFlow*(time-booster.at(i).burndelay);
		}}
	double mass=CalculateFullMass();
	double FirstStageFlow=stage.at(0).fuelmass/stage.at(0).burntime;

//	Thrust*=0.99; //account for Drag

	double substr=time*(FirstStageFlow)+BoosterFuelMassBurnt; //Grande approssimazione
	mass-=substr;
	n=Thrust/(mass*g0);
	
	//sprintf(logbuff,"Thrust %.3f fsflow %.3f boostersflow %.3f mass %.3f n %.3f",Thrust,FirstStageFlow,BoostersFlow,mass,n);
	//oapiWriteLog(logbuff);
	return n;
}

bool Multistage2015::CGTE(double psi0){

	double t0, v0, x0, y0;

	double deltaPsi0, z0, C, psi, z, v, deltaT, deltax, deltay, x, y;
	
	double Thrust = stage.at(0).thrust;

	double BoostersFlow = 0;

	for(int i=0;i<nBoosters;i++){
		if(booster.at(i).burndelay == 0){  //accounting for boosters not immediatly ignited
			Thrust+=booster.at(i).thrust*booster.at(i).N;
		}
	}

	double mass = CalculateFullMass();
	 
	x0 = 0;

	y0 = Misc.COG;
	
	t0 = 0;
	
	v0 = 0;
	
	double dtt = 0.1;

	while((y0 < altsteps.at(1)) && (y0 > 0)){

		double acceleration = GetProperNforCGTE(t0) * g0;
		
		y0 += 0.5 * (acceleration - g0) * dtt * dtt + v0 * dtt;
		
		t0 += dtt;

		v0 += (acceleration - g0) * dtt;

	}

	
	double hvel, vvel, vel, absacc, hacc, vacc, modspost, normx, normy;
	hvel = 0;
	vvel = v0;
	vel = v0;
	deltaT = 1;

	while((t0 < 500) && (psi0 < 80 * RAD)){ 

		 /// OFFICIAL METHOD

		/// OFFICIAL METHOD FINISHES HERE!!
		
		 //// MY METHOD
		 
		double grel = mu / ((rt + y0) * (rt + y0)) - (vel * vel / (rt + y0));
		
		double n = GetProperNforCGTE(t0);

		absacc = n * g0;

		hacc = absacc * sin(psi0);

		vacc = absacc * cos(psi0) - grel;

		x = x0 + hvel * deltaT + 0.5 * hacc * deltaT * deltaT;

		y = y0 + vvel * deltaT + 0.5 * vacc * deltaT * deltaT;

		hvel += hacc * deltaT;

		vvel += vacc * deltaT;

		vel = sqrt(hvel * hvel + vvel * vvel);

		t0 += deltaT;

		deltax = x - x0;

		deltay = y - y0;

		x0 = x;

		y0 = y;
			
		v0 = vel;

        modspost = sqrt(deltax * deltax + deltay * deltay);

		normx = deltax / modspost;

		normy = deltay / modspost;

		psi = 0.5 * PI - atan2(normy, normx);

		if(psi > psi0){ 
			psi0 = psi;
		}
		 ///// MY METHOD FINISHES HERE!!

		if(y0 > altsteps.at(3)){
			return true;
		}

	}
	
	return false;
}

void Multistage2015::CheckForAdditionalThrust(int pns){

	if(GetAttachmentStatus(live_a.at(pns))){

		OBJHANDLE live = GetAttachmentStatus(live_a.at(pns));

		VESSEL3 *v;

		v = (VESSEL3*)oapiGetVesselInterface(live);

		VECTOR3 TotalThrustVecPL;

		v -> GetThrustVector(TotalThrustVecPL);

		TotalThrustVecPL = mul(RotationMatrix(payload.at(pns).Rotation), TotalThrustVecPL);

		AddForce(TotalThrustVecPL,payload.at(pns).off.at(0));

	}

}

void Multistage2015::CheckForFX(int fxtype,double param){
	
	switch(fxtype){

		case FXMACH:

			if((param > FX_Mach.mach_min) && (param < FX_Mach.mach_max)){

				if(!FX_Mach.added){
					FX_Mach.added = true;

				for(int i = 0; i < FX_Mach.nmach; i++){
					PARTICLE ps = GetProperPS(FX_Mach.pstream);
					FX_Mach.ps_h.at(i) = AddParticleStream(&ps.Pss, FX_Mach.off.at(i), FX_Mach.dir, &lvl);
				}
			}

		} else {

			if(FX_Mach.added == true){
				for(int i = 0; i < FX_Mach.nmach; i++){
					DelExhaustStream(FX_Mach.ps_h.at(i));
				}
				FX_Mach.added = false;
			}
		}
		break;

	case FXVENT:
		for(int i = 1; i <= FX_Vent.nVent; i++){
			if(param < FX_Vent.time_fin.at(i)){				
				if(!FX_Vent.added.at(i)){		
					PARTICLE ps = GetProperPS(FX_Vent.pstream);
					FX_Vent.ps_h.at(i) = AddParticleStream(&ps.Pss, FX_Vent.off.at(i), FX_Vent.dir.at(i), &lvl);

					FX_Vent.added.at(i) = true;

					logbuff = std::format("Venting Effect Added @: {:.3f}, {:.3f}, {:.3f} dir: {:.3f}, {:.3f}, {:.3f}",FX_Vent.off.at(i).x, FX_Vent.off.at(i).y, FX_Vent.off.at(i).z, FX_Vent.dir.at(i).x, FX_Vent.dir.at(i).y, FX_Vent.dir.at(i).z);
					
					oapiWriteLog(const_cast<char *>(logbuff.c_str()));
				}

			} else {

				if(FX_Vent.added.at(i) == true){
					
					DelExhaustStream(FX_Vent.ps_h.at(i));

					FX_Vent.added.at(i) = false;
				}
			}
		}
		break;
	}
}

void Multistage2015::EvaluateComplexFlight(){
	
	srand((unsigned)time(nullptr));
	
	for(int i = 0; i < nStages; i++){

		for(int j = 0; j < stage.at(i).nEngines; j++){

			int amplitude = rand() % 1500;

			stage.at(i).engine_amp.at(i) = (double)amplitude / 100000;

			int transval = rand() % 180;

			stage.at(i).engine_phase.at(i) = (double)transval * RAD;

			int frequency = rand() % 60;

			stage.at(i).freq.at(i) = 30 + (double)frequency;

			logbuff = std::format("{} Complex Flight-> Stage {} Engine {} Amplitude {:.3f} phase {:.3f} frequency {:.3f}", GetName(), i + 1, j + 1, stage.at(i).engine_amp.at(i), stage.at(i).engine_phase.at(i), stage.at(i).freq.at(i));
			
			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		}
	}

	for(int i = 0; i < nBoosters; i++){

		for(int j = 0; j < booster.at(i).N; j++){
			
			int amplitude = rand() % 1500;

			booster.at(i).engine_amp.at(j) = (double)amplitude / 100000;

			int transval = rand() % 180;

			booster.at(i).engine_phase.at(j) = (double)transval * RAD;

			int frequency = rand() % 60;

			booster.at(i).freq.at(j) = 30 + (double)frequency;

			logbuff = std::format("{} Complex Flight-> Booster {} Engine {} Amplitude {:.3f} phase {:.3f} frequency {:.3f}", GetName(), i + 1, j + 1, booster.at(i).engine_amp.at(j), booster.at(i).engine_phase.at(j), booster.at(i).freq.at(j));
			
			oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		}
	}

	return;
}

void Multistage2015::FailuresEvaluation(){

	srand((unsigned)time(nullptr));

	int check = rand() % 1000;

	if(check < 10 * failureProbability){
		timeOfFailure = rand() % 300;
	}

	logbuff = std::format("{}: Failures->Probabilty: {} Number Extracted: {} Time Of Failure: {}", GetName(), failureProbability, check / 10, timeOfFailure);

	oapiWriteLog(const_cast<char *>(logbuff.c_str()));
}

void Multistage2015::boom(){
	
	if((wRamp)||(HangarMode)){
		DelAttachment(AttToRamp);
	}

	VESSELSTATUS2 vs;

	memset(&vs,0,sizeof(vs));

	vs.version=2;

	GetStatusEx(&vs);



	VECTOR3 ofs = _V(0, 0, 0);

	VECTOR3 rofs, rvel = {vs.rvel.x, vs.rvel.y, vs.rvel.z};

	VECTOR3 vel = {0, 0, 0};

	Local2Rel (ofs, vs.rpos);

	GlobalRot(vel,rofs);

	vs.rvel.x = rvel.x + rofs.x;
	vs.rvel.y = rvel.y + rofs.y;
	vs.rvel.z = rvel.z + rofs.z;

	OBJHANDLE hwreck;

	hwreck = oapiCreateVesselEx("wreck", "boom", &vs);

	oapiSetFocusObject(hwreck);

	oapiCameraScaleDist(20);

	if(wRamp){
		oapiDeleteVessel(hramp);
	}

	oapiDeleteVessel(GetHandle());

	return;
}

void Multistage2015::Failure()
{
	srand((unsigned)time(nullptr));

	int check = rand() % 1000;

	if((currentStage == 0) && (check < 250)){
		boom();
	} else {
		
		int engineout=rand()%stage.at(currentStage).nEngines;	

		SetThrusterResource(stage.at(currentStage).th_main_h.at(engineout), nullptr);
	}

}

bool Multistage2015::CheckForFailure(double met){
	
	if((floor(met) == timeOfFailure) && (!failed)){

		logbuff = std::format("{}: FAILURE FAILURE FAILURE @{:.0f}", GetName(), met);

		oapiWriteLog(const_cast<char *>(logbuff.c_str()));

		failed = true;
		
		return true;

	} else {

		return false;

	}

}

int Multistage2015::WriteTelemetryFile(int initline){

	FILEHANDLE TlmFile;
	std::string filenmbuff;

	filenmbuff = std::format("Multistage2015\\Telemetry\\{}_{:.2f}_TLM.txt", GetName(), oapiGetSysMJD());

	std::string buffer;

	if(initline == 0){

		TlmFile = oapiOpenFile(filenmbuff.c_str(), FILE_OUT, CONFIG);

		buffer = std::format("<--!Multistage 2015 Automatically Generated Telemetry File!-->");
		
		oapiWriteLine(TlmFile, const_cast<char *>(buffer.c_str()));

		buffer = std::format("MET,Altitude,Speed,Pitch,Thrust,Mass,V-Speed,Acceleration");

		oapiWriteLine(TlmFile, const_cast<char *>(buffer.c_str()));

	} else {

		TlmFile = oapiOpenFile(filenmbuff.c_str(), FILE_APP, ROOT);

	}


	for(int i = initline; i < tlmidx; i++){

		buffer = std::format("{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f},{:.3f}",tlmAlt.at(i).x,tlmAlt.at(i).y,tlmSpeed.at(i).y,tlmPitch.at(i).y,tlmThrust.at(i).y,tlmMass.at(i).y,tlmVv.at(i).y,tlmAcc.at(i).y);

		oapiWriteLine(TlmFile, const_cast<char *>(buffer.c_str()));
	}

	if(initline == 0){
		oapiCloseFile(TlmFile, FILE_OUT);
	} else {
		oapiCloseFile(TlmFile, FILE_APP);
	}

	return tlmidx - 1;
}

void Multistage2015::Telemetry(){

	if((updtlm >= 1) && (MET < TLMSECS - 1)){
		
		VECTOR3 ThrustVec;
		GetThrustVector(ThrustVec);

		tlmAlt.at(tlmidx).x = MET;
		tlmSpeed.at(tlmidx).x = MET;
		tlmPitch.at(tlmidx).x = MET;
		tlmThrust.at(tlmidx).x = MET;
		tlmMass.at(tlmidx).x = MET;
		tlmVv.at(tlmidx).x = MET;
		tlmAcc.at(tlmidx).x = MET;
		
		tlmAlt.at(tlmidx).y = GetAltitude();
		tlmSpeed.at(tlmidx).y = GetOS();
		tlmPitch.at(tlmidx).y = GetPitch()*DEG;
		tlmThrust.at(tlmidx).y = length(ThrustVec);
		tlmMass.at(tlmidx).y = GetMass();
		tlmVv.at(tlmidx).y = GetVPerp();
		tlmAcc.at(tlmidx).y = getabsacc();	

		tlmidx++;
		updtlm = 0;
	}

	updtlm += oapiGetSimStep();

	if(Misc.telemetry){	
		writetlmTimer += oapiGetSimStep();
		if(writetlmTimer >= 60){
			writetlmTimer = 0;
			tlmnlines = WriteTelemetryFile(tlmnlines);
		}
	}

	return;
}

void Multistage2015::CalculateAltSteps(double planetmass){

	std::array<double, 4> altref{100, 350, 1400, 35000};

	double earthmass = 5.973698968 * pow(10.0, 24);//1000000000000

	for(int i = 0; i < 4; i++){
		altsteps.at(i)=altref.at(i)*planetmass/earthmass;
	}

	return;
}

void Multistage2015::SetNewAltSteps(double newstep1, double newstep2, double newstep3, double newstep4){

	altsteps.at(0) = newstep1;

	altsteps.at(1) = newstep2;

	altsteps.at(2) = newstep3;

	altsteps.at(2) = newstep4;

	return;
}

void Multistage2015::ToggleComplexFlight(){

	if(Complex){
		Complex = false;
	} else {
		Complex = true;
		EvaluateComplexFlight();
	}

	return;
}

//****************************CALLBACKS******************************************************************************

void Multistage2015::clbkSetClassCaps(FILEHANDLE cfg){
	 
	initGlobalVars();			// First of all let's initialize everything!

  	SetSurfaceFrictionCoeff(0.7, 0.7);  // to avoid bad vibrations while throttling up at launch
 	
	SetCW(0.2, 0.5, 1.5, 1.5);		// gotten from Vinka
 
	EnableTransponder(true);
 
	InitNavRadios(4);

	SetCameraOffset(_V(0, 0, 100));	//Empirical

	SetVisibilityLimit(0, 0.002);

 	SetAlbedoRGB(_V((float)255 / 255, (float)255 / 255, (float)255 / 255));

 	SetGravityGradientDamping(0);
	
 	SetMaxWheelbrakeForce(200000);


 	//Default ParticleStreams Definitions:

 	// DEF CONTRAIL
 	std::string transfer;
 	transfer = "Contrail";
	Particle.at(13).ParticleName = transfer;
	Particle.at(13).Pss.flags = 0;
	Particle.at(13).Pss.srcsize = 8;
	Particle.at(13).Pss.srcrate = 5;
	Particle.at(13).Pss.v0 = 150;
	Particle.at(13).Pss.srcspread = 0.3;
	Particle.at(13).Pss.lifetime = 8;
	Particle.at(13).Pss.growthrate = 4;
	Particle.at(13).Pss.atmslowdown = 3;
	Particle.at(13).Pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;
	Particle.at(13).Pss.levelmap = PARTICLESTREAMSPEC::LVL_PSQRT;
	Particle.at(13).Pss.lmin = 0;
	Particle.at(13).Pss.lmax = 0.5;
	Particle.at(13).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLOG;
	Particle.at(13).Pss.amin = 1e-6;
	Particle.at(13).Pss.amax = 0.1;
	Particle.at(13).Pss.tex = oapiRegisterParticleTexture(const_cast<char *>("Contrail1"));
	
  	//DEF EXHAUST
   	transfer = "Exhaust";

	Particle.at(14).ParticleName = transfer;
	Particle.at(14).Pss.flags = 0;
	Particle.at(14).Pss.srcsize = 4;
	Particle.at(14).Pss.srcrate = 20;
	Particle.at(14).Pss.v0 = 150; //250
	Particle.at(14).Pss.srcspread = 0.1;
	Particle.at(14).Pss.lifetime = 0.3; //0.6
	Particle.at(14).Pss.growthrate = 12; //20
	Particle.at(14).Pss.atmslowdown = 2;
	Particle.at(14).Pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;
	Particle.at(14).Pss.levelmap = PARTICLESTREAMSPEC::LVL_PSQRT;
	Particle.at(14).Pss.lmin = 0;
	Particle.at(14).Pss.lmax = 0.5;
	Particle.at(14).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLOG;
	Particle.at(14).Pss.amin = 1e-6;
	Particle.at(14).Pss.amax = 0.1;
	Particle.at(14).Pss.tex = oapiRegisterParticleTexture (const_cast<char *>("Contrail3"));

	//DEF EXHAUST
	transfer = "Clear";
	Particle.at(15).ParticleName = transfer;
	Particle.at(15).Pss.flags = 0;
	Particle.at(15).Pss.srcsize = 0;
	Particle.at(15).Pss.srcrate = 0;
	Particle.at(15).Pss.v0 = 0; //250
	Particle.at(15).Pss.srcspread = 0;
	Particle.at(15).Pss.lifetime = 0; //0.6
	Particle.at(15).Pss.growthrate = 0; //20
	Particle.at(15).Pss.atmslowdown = 0;
	Particle.at(15).Pss.ltype = PARTICLESTREAMSPEC::DIFFUSE;
	Particle.at(15).Pss.levelmap = PARTICLESTREAMSPEC::LVL_FLAT;
	Particle.at(15).Pss.lmin = 0;
	Particle.at(15).Pss.lmax = 0;
	Particle.at(15).Pss.atmsmap = PARTICLESTREAMSPEC::ATM_FLAT;
	Particle.at(15).Pss.amin = 0;
	Particle.at(15).Pss.amax = 0;
 
}

void Multistage2015::clbkLoadStateEx(FILEHANDLE scn,void *vs){

	logbuff = std::format("Multistage Version: {}", GetMSVersion());

	oapiWriteLog(const_cast<char *>(logbuff.c_str()));

	oapiWriteLog(const_cast<char *>("Load State Started"));

 	char *line;
 	double batt_trans=0;
 	bool loadedbatts=false;
	stepsloaded=false;
	
	while (oapiReadScenario_nextline(scn, line)){
		std::string line_str(line);

		if(line_str.starts_with("CONFIG_FILE")){
			std::string value = line_str.substr(11 + 1);
			fileini = value;
		} else if (line_str.starts_with("MET")){
			std::string value = line_str.substr(3 + 1);
			MET = std::stod(value);
		} else if (line_str.starts_with("GNC_RUN")){
			std::string value = line_str.substr(7 + 1);
			Gnc_running = std::stoi(value);
			if(Gnc_running == 1){
				APstat = true;
			}
		} else if (line_str.starts_with("BATTERY")){
			
			std::string value = line_str.substr(7 + 1);
			batt_trans = std::stod(value);

			loadedbatts = TRUE;

		} else if (line_str.starts_with("STAGE_IGNITION_TIME")){
			std::string value = line_str.substr(19 + 1);
			stage_ignition_time = std::stod(value);
		} else if (line_str.starts_with("FAILURE_PROB")){
			
			std::string value = line_str.substr(12 + 1);

			failureProbability = std::stod(value);
		} else if (line_str.starts_with("GNC_AUTO_JETTISON")){
			
			std::string value = line_str.substr(17 + 1);

			int AJVal = 0;

			AJVal = std::stoi(value);

			if (AJVal == 0) {
				AJdisabled = true;
			}

		} else if (line_str.starts_with("GUIDANCE_FILE")){

			std::string value = line_str.substr(13 + 1);

			std::istringstream iss(value);
			iss >> guidancefile;

			//sscanf_s(line + 13, "%s", guidancefile.c_str());
			// wVinkasGuidance=TRUE;

			parseGuidanceFile(guidancefile);

			if (Gnc_running != 1) {
				VinkaCheckInitialMet();
			}

		} else if (line_str.starts_with("TELEMETRY_FILE")){

			std::string value = line_str.substr(14 + 1);

			std::istringstream iss(value);

			iss >> tlmfile;

			//sscanf_s(line + 14, "%s", tlmfile.c_str());

			parseTelemetryFile(tlmfile);

			wReftlm = true;

		} else if (line_str.starts_with("CURRENT_BOOSTER")){

			std::string value = line_str.substr(15 + 1);

			currentBooster = std::stoi(value);

			currentBooster -= 1;

		} else if (line_str.starts_with("CURRENT_INTERSTAGE")){
			
			std::string value = line_str.substr(18 + 1);

			currentInterstage = std::stoi(value);
			
			currentInterstage -= 1;

		} else if (line_str.starts_with("CURRENT_STAGE")){

			std::string value = line_str.substr(13 + 1);

			currentStage = std::stoi(value);

			//currentStage -= 1;

			if(currentStage > 0){
				currentBooster = 11;
			}

		} else if (line_str.starts_with("STAGE_STATE")){

			std::string value = line_str.substr(11 + 1);

			stage[currentStage].StageState = std::stoi(value);

			if(stage[currentStage].StageState == STAGE_IGNITED){
				stage[currentStage].Ignited = true;
			}

		} else if (line_str.starts_with("CURRENT_PAYLOAD")){

			std::string value = line_str.substr(15 + 1);

			currentPayload = std::stoi(value);

			currentPayload -= 1;

		} else if (line_str.starts_with("FAIRING")){

			std::string value = line_str.substr(7 + 1);

			wFairing = std::stoi(value);

			if((wFairing < 0) || (wFairing > 1)){
				wFairing = 0;
			}

		} else if (line_str.starts_with("CONFIGURATION")){

			std::string value = line_str.substr(13 + 1);

			Configuration = std::stoi(value);

			if((Configuration < 0) || (Configuration > 1)){
				Configuration = 0;
			}

		} else if (line_str.starts_with("COMPLEX")){

			Complex = true;

		} else if (line_str.starts_with("HANGAR")){

			HangarMode = true;

		} else if (line_str.starts_with("CRAWLER")){

			wCrawler = true;

		} else if (line_str.starts_with("CAMERA")){

			std::istringstream iss(line_str.substr(6));

			iss >> CamDLat >> CamDLng;

			if ((CamDLat == 0) && (CamDLng == 0)) {
				CamDLat = 0.01; CamDLng = 0.03;
			}

			wCamera = true;


		} else if (line_str.starts_with("GROWING_PARTICLES")){

			GrowingParticles = true;

		} else if (line_str.starts_with("DENY_IGNITION")){

			stage[currentStage].DenyIgnition = true;

		} else if (line_str.starts_with("ALT_STEPS")){
			
			std::istringstream iss(line + 9 + 1);

			iss >> altsteps[0] >> altsteps[1] >> altsteps[2] >> altsteps[3];

			stepsloaded = true;

		} else if (line_str.starts_with("PEG_PITCH_LIMIT")){

			std::string value = line_str.substr(15 + 1);

			PegPitchLimit = std::stod(value);

			PegPitchLimit *= RAD;

		} else if (line_str.starts_with("PEG_MC_INTERVAL")){

			std::string value = line_str.substr(15 + 1);

			PegMajorCycleInterval = std::stod(value);

		} else if (line_str.starts_with("RAMP")){

			wRamp = true;

		} else if (line_str.starts_with("ATTMSPAD")){

			std::string value = line_str.substr(8 + 1);

			MsPadZ.z = std::stod(value);

			AttToMSPad = true;

		} else {

			ParseScenarioLineEx(line, vs);

		}

	}
 
 	std::filesystem::path tempFile;
 	tempFile = OrbiterRoot / fileini;

 	logbuff = std::format("{}: Config File: {}", GetName(), tempFile.string());

 	oapiWriteLog(const_cast<char *>(logbuff.c_str()));

 	parseinifile(tempFile);

 	if((currentInterstage > currentStage) || (currentInterstage > nInterstages) || (currentInterstage >= stage.at(currentStage).IntIncremental)){
		stage.at(currentStage).wInter = false; 
	}
 
 	if((wFairing == 1) && (hasFairing == false)){
		wFairing = 0;
	}
 	
	if(Configuration == 0){			//if only configuration is defined, reset everything
	 	if(hasFairing== true){
		  wFairing=1;
		}
	 
		currentStage = 0;
	 
		currentPayload = 0;
	 
		currentBooster = 0;
	 
		currentInterstage = 0;
 	}

 
 	UpdateOffsets();
 
 	VehicleSetup();
 
 	LoadMeshes();
 
	if(Gnc_running == 1){
 		InitializeDelays();
	}


	if(loadedbatts){
		stage.at(currentStage).batteries.CurrentCharge = batt_trans;
	}
 
	loadedCurrentBooster = currentBooster;
	loadedCurrentInterstage = currentInterstage;
	loadedCurrentStage = currentStage;
	loadedCurrentPayload = currentPayload;;
	loadedwFairing = wFairing;
	loadedConfiguration = Configuration;
	loadedMET = MET;
	loadedGrowing = GrowingParticles;
	
	oapiWriteLog(const_cast<char *>("Load State Terminated")); 

}


int Multistage2015::clbkConsumeBufferedKey(DWORD key,bool down,char *kstate){

	if(!down) return 0;

	if(KEYDOWN(kstate, OAPI_KEY_J) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))){
	
		if(currentBooster < nBoosters){
			Jettison(TBOOSTER, currentBooster);
		} else {
			if(currentStage < nStages - 1){
				if(stage.at(currentStage).wInter == true){
					Jettison(TINTERSTAGE, currentStage);
				} else {
					Jettison(TSTAGE, currentStage);
				}

			} else if((currentStage == nStages - 1) && (stage.at(currentStage).wInter)){
				Jettison(TINTERSTAGE, currentStage);
			} else {
				if((currentPayload < nPayloads) && (wFairing == 0)){
					Jettison(TPAYLOAD, currentPayload);
				} else {
					return 0;
				}
			}
		}
		
		return 1;
	}

	if(KEYDOWN(kstate, OAPI_KEY_F) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))){
		if(wLes){
			Jettison(TLES, 0);
		}else{
			if(wFairing == 1){
				Jettison(TFAIRING, 0);
			} else {
				return 0;
			}
		}
		
		return 1;	
	}


	if(KEYDOWN(kstate, OAPI_KEY_P) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))){
		if(!APstat){
			APstat = true;
			return 0;
		} else if(APstat){
			APstat = false;
			return 0;
		} else {	
			return 0;
		}
		
		return 1;
	}

	if(KEYDOWN(kstate, OAPI_KEY_SPACE) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate))){
		if((!DeveloperMode) && (!killDMD)){

			DeveloperMode = true;

			note = oapiCreateAnnotation(true, 1, _V(1, 0, 0));

			oapiAnnotationSetPos(note, 0.65, 0.25, 0.975, 0.75);

			oapiAnnotationSetSize(note, 0.75);

			std::string TXT;
			TXT = std::format("{}: \nDEVELOPER MODE ON \n\n[SPACEBAR] to reload .ini file \n\n[CTRL]+[SPACEBAR] to close Developer Mode", GetName());
			
			oapiAnnotationSetText(note, const_cast<char *>(TXT.c_str()));

			CreateDMD();

		} else {

			killDMD = true;
			DeveloperMode = false;
			if(note){
				oapiDelAnnotation(note);
			}
			
		}
		
		return 1;
	}

	if(KEYDOWN(kstate, OAPI_KEY_SPACE) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_ALT(kstate))){
		if(DeveloperMode){
			ResetVehicle(hangaranims, wRamp);
		}
		
		return 1;
	}
	
	
	if(KEYDOWN(kstate,OAPI_KEY_L)&&(KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate)))){
		if(HangarMode){
				char kstate[256];
				for(int i = 0; i < 256; i++){
					kstate[i]=0x00;
				}
				kstate[OAPI_KEY_L] = 0x80;
				kstate[OAPI_KEY_LCONTROL] = 0x80;
				vhangar->SendBufferedKey(OAPI_KEY_L, true, kstate);
				
				return 1;
		}

	}

	if(KEYDOWN(kstate,OAPI_KEY_D)&&(KEYMOD_CONTROL(kstate))&&(!KEYMOD_ALT(kstate)&&(!KEYMOD_SHIFT(kstate)))){
		if(HangarMode){
				char kstate[256];
				for(int i = 0; i < 256; i++){
					kstate[i]=0x00;
				}
				kstate[OAPI_KEY_D] = 0x80;
				kstate[OAPI_KEY_LCONTROL] = 0x80;
				vhangar->SendBufferedKey(OAPI_KEY_D, true, kstate);
				
				if(!AttToMSPad){
					OBJHANDLE hPad=oapiGetObjectByName(const_cast<char *>("MS_Pad"));
					if(oapiIsVessel(hPad)){
						AttachToMSPad(hPad);
						AttToMSPad = true;
					}
				}
				
				return 1;
		}
	}
	
	return 0;
}

void Multistage2015::AttachToMSPad(OBJHANDLE hPad){

	VESSEL4 *vPad;

	vPad = (VESSEL4*)oapiGetVesselInterface(hPad);

	padramp = vPad->CreateAttachment(false, _V(0, 0, 0), _V(0, 1, 0), _V(0, 0, 1), "Pad", false);
	
	AttToRamp = CreateAttachment(true, MsPadZ, _V(0, 0, -1), _V(0, 1, 0), "Pad", false);
	
	vPad->AttachChild(GetHandle(), padramp, AttToRamp);
}

int Multistage2015::clbkConsumeDirectKey(char *kstate){

	if(HangarMode){
		vhangar->clbkConsumeDirectKey(kstate);

		if(AttToMSPad){
			VECTOR3 pos, dir, rot;
			GetAttachmentParams(AttToRamp, pos, dir, rot);

			const double delta = 0.1;

			if(KEYDOWN(kstate, OAPI_KEY_UP) && (!KEYMOD_CONTROL(kstate)) && (KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))){
				pos.z -= delta;

				SetAttachmentParams(AttToRamp, pos, dir, rot);
			}

			if(KEYDOWN(kstate, OAPI_KEY_DOWN) && (!KEYMOD_CONTROL(kstate)) && (KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))){
				pos.z += delta;
				SetAttachmentParams(AttToRamp, pos, dir, rot);
			}
		}
	}

	return 0;
}

void Multistage2015::clbkSaveState(FILEHANDLE scn)
{
	std::string savebuff, savevalbuff;

	if (HangarMode) {
		Configuration = 0;
	}

	SaveDefaultState(scn);

	savebuff = "CONFIG_FILE";
	savevalbuff += fileini.string();

	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	if (!guidancefile.empty()) {
		savebuff += "GUIDANCE_FILE";
		savevalbuff += guidancefile.string();
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	savebuff = "CONFIGURATION";
	savevalbuff = Configuration;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	if (Complex) {
		savebuff = "COMPLEX";
		savevalbuff = "0";
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	if (HangarMode) {
		savebuff = "HANGAR";
		savevalbuff = "0";
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	savebuff = "CURRENT_BOOSTER";
	savevalbuff = currentBooster + 1;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	savebuff = "CURRENT_STAGE";
	savevalbuff = currentStage + 1;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	savebuff = "CURRENT_INTERSTAGE";
	savevalbuff = currentInterstage + 1;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	savebuff = "CURRENT_PAYLOAD";
	savevalbuff = currentPayload + 1;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	savebuff = "FAIRING";
	savevalbuff = wFairing;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	savebuff = "MET";
	savevalbuff = MET;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	if (APstat) {
		Gnc_running = 1;
		savebuff = "GNC_RUN";
		savevalbuff = Gnc_running;
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	if (stage[currentStage].batteries.wBatts) {
		savebuff = "BATTERY";
		savevalbuff = stage[currentStage].batteries.CurrentCharge;
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	if (stage[currentStage].DenyIgnition) {
		savebuff = "DENY_IGNITION";
		savevalbuff = "0";

		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	if (GrowingParticles) {
		savebuff = "GROWING_PARTICLES";
		savevalbuff = "0";
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	if (AJdisabled) {
		int val = 1;
		savebuff = "GNC_AUTO_JETTISON";
		savevalbuff = val;
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	savebuff = "STAGE_IGNITION_TIME";
	savevalbuff = stage_ignition_time;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	savebuff = "STAGE_STATE";
	savevalbuff = stage[currentStage].StageState;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	if (tlmfile[0] != '\0') {
		savebuff = "TELEMETRY_FILE";
		savevalbuff = tlmfile;
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	savebuff = "ALT_STEPS";
	savevalbuff = std::format("{:.1f}, {:.1f}, {:.1f}, {:.1f}", altsteps[0], altsteps[1], altsteps[2], altsteps[3]);
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	savebuff = "PEG_PITCH_LIMIT";
	savevalbuff = PegPitchLimit * DEG;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	savebuff = "PEG_MC_INTERVAL";
	savevalbuff = PegMajorCycleInterval;
	oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));

	if (wRamp) {
		savebuff = "RAMP";
		savevalbuff = "0";
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

	if ((HangarMode) && (AttToMSPad)) {
		VECTOR3 pos = _V(0, 0, 0), dir = _V(0, 0, 0), rot = _V(0, 0, 0);
		GetAttachmentParams(AttToRamp, pos, dir, rot);

		savebuff = "ATTMSPAD";
		savevalbuff = pos.z;
		oapiWriteScenario_string(scn, const_cast<char*>(savebuff.c_str()), const_cast<char*>(savevalbuff.c_str()));
	}

}

bool Multistage2015::CheckForDetach(){
		
	VECTOR3 Thrust, horThrust;
	GetThrustVector(Thrust);
	double Mass = GetMass();
	HorizonRot(Thrust, horThrust);
		
	if(horThrust.y>Mass*9.81){
		return true;
	} else {
		return false;
	}

}
void Multistage2015::clbkPreStep(double simt, double simdt, double mjd){

	if((APstat)&&(Configuration==0)){
		VinkaAutoPilot();
		MET += simdt;
		stage.at(0).currDelay = -MET;
		if(Misc.GNC_Debug==1){
			Guidance_Debug();
		}				
	}
	
	if((wRamp)&&(!HangarMode)){
		
		if(CheckForDetach()){
			if(GetAttachmentStatus(AttToRamp)!=NULL){
				vramp->DetachChild(padramp,0);
				DelAttachment(AttToRamp);	
				//AttachedToRamp=false;
				oapiWriteLog(const_cast<char *>("Detached from Launchpad"));
			}
			if((!wLaunchFX)||(GetAltitude()>FX_Launch.CutoffAltitude)){	
				bool deleted=oapiDeleteVessel(hramp);
				if(deleted){
					wRamp = false;
					NoMoreRamp = true;
					oapiWriteLog(const_cast<char *>("LaunchPad Deleted from Scenery"));
				}
			}
		}
	} else if((HangarMode)&&(AttToMSPad)){
		if(CheckForDetach()){
			DelAttachment(AttToRamp);
			AttToMSPad = false;
			oapiWriteLog(const_cast<char *>("Detached from Launchpad"));
		}
	}

	if(Configuration==1) {FLY(simt,simdt,mjd);
		if(GrowingParticles){
			ManageParticles(simdt, true);
		}
	}

	if(wMach){		
		CheckForFX(FXMACH, GetMachNumber());
	}

	if((wVent) && (MET <= 5)){
		CheckForFX(FXVENT, MET);
	}

	if(wFailures){
		if(CheckForFailure(MET)){
			Failure();
		}
	}

	return;
}


void Multistage2015::clbkPostStep(double simt, double simdt, double mjd){

	if((GetThrusterGroupLevel(THGROUP_MAIN) > 0.95) && (Configuration == 0)){
		Configuration = 1;
		MET = 0;
	}

	for(int i = currentPayload; i < nPayloads; i++){
		if(payload.at(i).live){
			CheckForAdditionalThrust(i);
		}
	}

	if((Configuration==1) && (GrowingParticles)){
		ManageParticles(simdt, false);
	}
			
	th_main_level = GetThrusterGroupLevel(THGROUP_MAIN);

	if(wLaunchFX){
		if(FX_Launch.CutoffAltitude > 0){
			launchFx_level = (-1) / (FX_Launch.CutoffAltitude) * GetAltitude() + 1;
			if(launchFx_level >= 1){
				launchFx_level = 1;
			}
			if(launchFx_level <= 0){
				launchFx_level = 0;
			}
		} else {
			launchFx_level = 1;
		}
	} else {
		launchFx_level=0;
	}

	launchFx_level *= th_main_level;

	return;
}

bool Multistage2015::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp){

	VESSEL4::clbkDrawHUD(mode, hps, skp);

	int cx = hps->CX, cy = hps->CY;

	DWORD w;
	DWORD h;
	DWORD bpp;

	oapiGetViewportSize(&w,&h,&bpp);
				

	std::string NAMEbuff;

	NAMEbuff = std::format("{}", GetName());

	skp->Text(0.02 * w, 0.25 * h + 20, NAMEbuff.c_str(), NAMEbuff.size());
	
	std::string METbuff;

	VECTOR3 t = hms(MET);
			
	if(MET>=0){
		METbuff = std::format("MET: {:03.0f}:{:02.0f}:{:02.0f}", t.x, t.y, t.z);
	} else {
		METbuff = std::format("T-: {:03.0f}:{:02.0f}:{:02.0f}", t.x, t.y, t.z);
	}
			skp->Text(0.02 * w, 0.25 * h + 40, METbuff.c_str(), METbuff.size());

			std::string STAGEbuff;
			
			STAGEbuff = std::format("Stage: {}", currentStage + 1);

			skp->Text(0.02 * w, 0.25 * h + 60, STAGEbuff.c_str(), STAGEbuff.size());

			std::string BTbuff;
			
			BTbuff = std::format("Remaining Stage Burn Time: {:02.0f}:{:02.0f}", hms(RemBurnTime(currentStage)).y, hms(RemBurnTime(currentStage)).z);

			skp->Text(0.02 * w, 0.25 * h + 80, BTbuff.c_str(), BTbuff.size());
			
			if((runningPeg)  &&(currentStage == NN  -1)){

				if(GetAltitude() > altsteps.at(3)){
				std::string PMECObuff; 
				VECTOR3 PMECO = hms(TMeco);
				
				PMECObuff = std::format("Predicted MECO: {:02.0f}:{:02.0f}", PMECO.y, PMECO.z);

				skp->Text(0.02 * w, 0.25 * h + 100, PMECObuff.c_str(), PMECObuff.size());

				}

			}

			
			std::string PLNamebuff;
			std::string PLWbuff;
			
			double PLtotWeight=0;

			for(int i = currentPayload; i < nPayloads; i++){
				PLtotWeight += payload.at(i).mass;
			}

			PLNamebuff = std::format("Next Payload: {}", payload.at(currentPayload).name.c_str());
			skp->Text(w - 400, 0.25 * h + 20, PLNamebuff.c_str(), PLNamebuff.size());
			PLWbuff = std::format("Total Payload Weight: {:.1f} kg", PLtotWeight);
			skp->Text(w - 400, 0.25 * h + 40, PLWbuff.c_str(), PLWbuff.size());

	return true;
}

void Multistage2015::clbkPostCreation(){

	oapiWriteLog(const_cast<char *>("Post Creation Started"));

	UpdateLivePayloads();

	if(failureProbability > 0){
		wFailures = true;
		
		FailuresEvaluation();
	}

	double planetmass = oapiGetMass(GetGravityRef());

	mu = GGRAV * planetmass;

	rt = oapiGetSize(GetGravityRef());

	g0 = mu / (rt * rt);

	if(!stepsloaded){
		CalculateAltSteps(planetmass);
	}

	//////COMPUTATIONAL GRAVITY TURN EVALUATION
	if(Configuration == 0){
		 
		double init_psi = 21;
		
		while((!CGTE(init_psi * RAD)) && (init_psi > 0)){
			init_psi -= 1;
		}
		
		double safety=init_psi * 0.05;
		
		init_psi -= safety;

		if(init_psi < 1){
			init_psi = 1;
		}

		GT_IP_Calculated = (90 - init_psi) * RAD;

		logbuff = std::format("{}: Gravity Turn Initial Pitch by user: {:.3f} Calculated:{:.3f}", GetName(), GT_InitPitch * DEG, GT_IP_Calculated * DEG);

		oapiWriteLog(const_cast<char *>(logbuff.c_str()));

		if(GT_InitPitch==0) { GT_InitPitch=GT_IP_Calculated; }

		//Ramp or Hangar
		if(!HangarMode){
			Ramp(wRamp);
		} else {
			CreateHangar();
		}
		
		if(wCamera){
			CreateCamera();
		}

	}


	InitPEG();

	if(Complex){
		EvaluateComplexFlight();
	}

	const ATMCONST *atmconst = oapiGetPlanetAtmConstants(GetSurfaceRef());

	RefPressure = atmconst->p0;
	
	double altlimit = atmconst->altlimit;

	logbuff = std::format("{}: Planet Reference Pressure = {:.1f} Pa  Atmosphere Altitude Limit:{:.1f} km", GetName(), RefPressure, altlimit / 1000);

	oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		
	GetStatusEx(&vs2);

	vs2.status=0;

	GetRelativePos(GetSurfaceRef(), vs2.rpos);
	
	oapiWriteLog(const_cast<char *>("Post Creation Terminated"));

	return;
}

void Multistage2015::CreateLaunchFX(){
	double alpha = 0;

	PARTICLE ps1 = GetProperPS(FX_Launch.Ps1);
	PARTICLE ps2 = GetProperPS(FX_Launch.Ps2);

	for(int i = 1; i <= FX_Launch.N; i++){
		
		alpha = (PI * 2 / FX_Launch.N * (i - 1)) + FX_Launch.Angle * RAD;

		vramp->AddParticleStream(&ps1.Pss, _V(FX_Launch.Distance * cos(alpha), FX_Launch.Distance * sin(alpha), FX_Launch.H), _V(cos(alpha), sin(alpha), 0), &launchFx_level);

		vramp->AddParticleStream(&ps2.Pss, _V(FX_Launch.Distance * cos(alpha), FX_Launch.Distance * sin(alpha), FX_Launch.H), _V(cos(alpha), sin(alpha), 0), &launchFx_level);

	}

}

void Multistage2015::CreateHangar(){

	hhangar = oapiGetVesselByName(const_cast<char *>("MS_Hangar"));

	if(!oapiIsVessel(hhangar)){

		memset(&vshangar,0,sizeof(vshangar));

		vshangar.version=2;

		GetStatusEx(&vshangar);

		oapiWriteLog(const_cast<char *>("Creating Hangar"));

		hhangar = oapiCreateVesselEx("MS_Hangar", "MS_Hangar", &vshangar);

		vshangar.status=1;

		vhangar = (VESSEL4*)oapiGetVesselInterface(hhangar);

		if(oapiGetOrbiterVersion()<160903){
			
			FILEHANDLE fh = oapiOpenFile("\\Multistage2015\\__MSHANG", FILE_OUT, CONFIG);
			
			oapiWriteScenario_string(fh, const_cast<char *>("STATUS"), const_cast<char *>("Landed Earth"));

			oapiWriteScenario_float(fh, const_cast<char *>("HEADING"), vshangar.surf_hdg * DEG);

			std::string position;

			position = std::format("{:.20f} {:.20f}", vshangar.surf_lng * DEG, vshangar.surf_lat * DEG);

			oapiWriteScenario_string(fh, const_cast<char *>("POS"), const_cast<char *>(position.c_str()));

			oapiCloseFile(fh, FILE_OUT);

			logbuff = std::format("Orbiter version: {} . Hangar is created through scenario file procedure",oapiGetOrbiterVersion());

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));

			fh = oapiOpenFile("\\Multistage2015\\__MSHANG", FILE_IN, CONFIG);

			vhangar->clbkLoadStateEx(fh, &vshangar);
		}

		vhangar->DefSetStateEx(&vshangar);

		if(wCrawler){
			
			hCrawler = oapiGetVesselByName(const_cast<char *>("MS_Crawler"));

			if(!oapiIsVessel(hCrawler)){

				VESSELSTATUS2 vsCrawler;

				memset(&vsCrawler, 0, sizeof(vsCrawler));

				vsCrawler.version = 2;

				vsCrawler = vshangar;
				vsCrawler.surf_hdg = vshangar.surf_hdg + 180 * RAD;
			
				double rt = 6371010;
				double earth_circ = rt * 2 * PI;
				double each_deg = earth_circ / 360; 
				double d_lat = 2.0 * sin(vsCrawler.surf_hdg);
				double d_lng = -2.0 * cos(vsCrawler.surf_hdg);
				vsCrawler.surf_lat += d_lat / each_deg * RAD;
				vsCrawler.surf_lng += d_lng / each_deg * RAD;

				hCrawler = oapiCreateVesselEx("MS_Crawler", "MS_Crawler", &vsCrawler);
				
				oapiWriteLog(const_cast<char *>("Crawler Created"));
			}
		}
	}

	vhangar = (VESSEL4*)oapiGetVesselInterface(hhangar);
	if(!AttToMSPad){
		
		AttToHangar = CreateAttachment(true, _V(0, 0, 0), _V(0, 0, -1), _V(0, 1, 0), "hangar", false);
		
		DWORD index;

		vhangar->clbkGeneric(VMSG_USER, 1, &index);

		vhangar->AttachChild(GetHandle(), vhangar->GetAttachmentHandle(false, index), AttToHangar);

		oapiWriteLog(const_cast<char *>("Attached to the Hangar"));

	} else {

		OBJHANDLE hPad = oapiGetVesselByName(const_cast<char *>("MS_Pad"));

		if(oapiIsVessel(hPad)){
			AttachToMSPad(hPad);
		}
	}

	return;
}
void Multistage2015::CreateCamera(){
	//////////////CAMERA CREATION
	OBJHANDLE hcamera=oapiGetVesselByName(const_cast<char *>("MS_Camera"));
	if(!oapiIsVessel(hcamera)){
		
		VESSELSTATUS2 vscam;

		memset(&vscam,0,sizeof(vscam));

		vscam.version = 2;

		if(HangarMode){
			vscam = vshangar;
		} else {
			vscam = vsramp;
		}

		vscam.surf_lat += CamDLat * RAD;

		vscam.surf_lng += CamDLng * RAD;

		hcamera = oapiCreateVesselEx("MS_Camera", "MS_Camera", &vscam);

		if(oapiIsVessel(hcamera)){
			VESSEL4* vcam;
			
			vcam = (VESSEL4*)oapiGetVesselInterface(hcamera);

			vcam->clbkGeneric(VMSG_USER, 1, (void*)GetName());
		}
	}
	return;
}

void Multistage2015::Ramp(bool alreadyramp){
		
	std::string PadName;
	PadName = std::format("MS_LaunchPad_{}", GetName());

	if(!alreadyramp){	
		memset(&vsramp, 0, sizeof(vsramp));

		vsramp.version = 2;

		GetStatusEx(&vsramp);

		oapiWriteLog(const_cast<char *>("Creating Launchpad"));

		hramp = oapiCreateVesselEx(PadName.c_str(), Misc.PadModule.c_str(), &vsramp);
		
		vsramp.status = 1;

		vramp = (VESSEL4*)oapiGetVesselInterface(hramp);

		if(oapiGetOrbiterVersion() < 160903){
			
			FILEHANDLE fh = oapiOpenFile("\\Multistage2015\\__MS15", FILE_OUT, CONFIG);

			oapiWriteScenario_string(fh, const_cast<char *>("STATUS"), const_cast<char *>("Landed Earth"));

			oapiWriteScenario_float(fh, const_cast<char *>("HEADING"), vsramp.surf_hdg * DEG);

			std::string position;

			position = std::format("{:.20f} {:.20f}", vsramp.surf_lng * DEG, vsramp.surf_lat * DEG);

			oapiWriteScenario_string(fh, const_cast<char *>("POS"), const_cast<char *>(position.c_str()));

			oapiCloseFile(fh, FILE_OUT);

			logbuff = std::format("Orbiter version: {} . Ramp is created through scenario file procedure", oapiGetOrbiterVersion());

			oapiWriteLog(const_cast<char *>(logbuff.c_str()));

			fh = oapiOpenFile("\\Multistage2015\\__MS15", FILE_IN, CONFIG);

			vramp->clbkLoadStateEx(fh, &vsramp);
		}

		vramp->DefSetStateEx(&vsramp);
		
		wRamp = true;

	} else {
		
		hramp = oapiGetVesselByName(const_cast<char *>(PadName.c_str()));
		
		if(hramp){

			
			vramp = (VESSEL4*)oapiGetVesselInterface(hramp);
		}

	}

	AttToRamp = CreateAttachment(true, _V(0, 0, 0), _V(0, 0, -1), _V(0, 1, 0), "pad", false);

	padramp = vramp->CreateAttachment(false, _V(0, 0, stage.at(0).height * 0.5 + CogElev), _V(0, -sin(Misc.VerticalAngle), cos(Misc.VerticalAngle)), _V(0, cos(Misc.VerticalAngle), sin(Misc.VerticalAngle)), "pad", false);

	vramp->AttachChild(GetHandle(),padramp,AttToRamp);
	
	if(wLaunchFX){
		CreateLaunchFX();
	}
	
	vramp->SetEnableFocus(false);
		
	return;
}

double Multistage2015::GetMET(){

	return MET;

}

bool Multistage2015::GetAutopilotStatus(){
	
	return APstat;

}

int Multistage2015::clbkGeneric (int msgid, int prm, void *context){
	
	switch(msgid){
		case(2015):
		if(prm==2015){
			return 2015;
		} else {
			return 0;
		}
	break;

	case(VMSG_USER):
		if(prm==1){
			double *met = (double*)(context);
			*met = GetMET();
			return 1;
		} else if(prm == 2){

			bool *ap = (bool*)(context);

			*ap = GetAutopilotStatus();

			return 1;

		} else if(prm==3){

			VECTOR3 *Targets = (VECTOR3*)(context);

			*Targets = GetAPTargets();

			return 1;
		}
		break;
	}

	return 0;
}

int Multistage2015::GetMSVersion(){
	
	return MSVERSION;

}

void Multistage2015::CreateDMD(){
	
	if(!DMD){

		DMD = new DevModeDlg(this);

		DMD->Open(g_Param.hDLL);

	}
}

void Multistage2015::DestroyDMD(){
	
	if(DMD){
		delete DMD;
		DMD = 0;
		hDlg = 0;
	}

}


DLLCLBK void InitModule(HINSTANCE hModule){
	g_Param.hDLL = hModule;
}

DLLCLBK void ExitModule(HINSTANCE hModule){
	CloseDlg(hDlg);
}

DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel,int flightmodel){
	
	return new Multistage2015(hvessel, flightmodel);

}

DLLCLBK void ovcExit(VESSEL *vessel){

	if(vessel)delete(Multistage2015*) vessel;

}

DevModeDlg *dlg;

BOOL CALLBACK DlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, (LONG)lParam);
		((DevModeDlg*)lParam)->hDlg = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->DlgProc (hWnd, uMsg, wParam, lParam);
	} else {
		return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
	}
}

BOOL CALLBACK DlgProcPld(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_PLD] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->PldProc (hWnd, uMsg, wParam, lParam);
	} else {
		return false;
	}
}

BOOL CALLBACK DlgProcViewTxt(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_VIEW]= hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->ViewTxtProc (hWnd, uMsg, wParam, lParam);
	} else {
		return false;
	}

}

BOOL CALLBACK DlgProcFairing(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_FAIRING] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->FairingProc(hWnd, uMsg, wParam, lParam);
	} else {
		return false;
	}

}


BOOL CALLBACK DlgProcParticle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_PARTICLE] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->ParticleProc(hWnd, uMsg, wParam, lParam);
	} else {
		return false;
	}

}


BOOL CALLBACK DlgProcMisc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_MISC] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->MiscProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}


BOOL CALLBACK DlgProcTex(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_TEX] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->TexProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}


BOOL CALLBACK DlgProcBooster(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_BOOSTER] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->BoosterProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}


BOOL CALLBACK DlgProcCurve(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_CURVE] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->CurveProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}

BOOL CALLBACK DlgProcStage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_STAGE] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->StageProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}

BOOL CALLBACK DlgProcUllage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if (uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_ULLAGE] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->UllageProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}

BOOL CALLBACK DlgProcInterstage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_INTERSTAGE] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->InterstageProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}

BOOL CALLBACK DlgProcLes(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_LES] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->LesProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}


BOOL CALLBACK DlgProcAdapter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd, GWLP_USERDATA, lParam);
		((DevModeDlg*)lParam)->hChild[CD_ADAPTER] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd, GWLP_USERDATA);

	if(dlg){
		return dlg->AdapterProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}

BOOL CALLBACK DlgProcFX(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG) {
		SetWindowLongPtr (hWnd,GWLP_USERDATA,lParam);
		((DevModeDlg*)lParam)->hChild[CD_FX] = hWnd;
	}
	
	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd,GWLP_USERDATA);

	if(dlg){
		return dlg->FXProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}

}

BOOL CALLBACK DlgProcScenario(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_INITDIALOG){
		SetWindowLongPtr (hWnd,GWLP_USERDATA,lParam);
		
		((DevModeDlg*)lParam)->hChild[CD_SCENARIO] = hWnd;
	}

	dlg = (DevModeDlg*)GetWindowLongPtr (hWnd,GWLP_USERDATA);

	if(dlg){
		return dlg->ScenarioProc(hWnd,uMsg,wParam,lParam);
	} else {
		return false;
	}
}
