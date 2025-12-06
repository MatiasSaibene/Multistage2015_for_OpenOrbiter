#pragma once
#ifndef __MULTISTAGE2015_H_
#define  __MULTISTAGE2015_H_
#define STRICT
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
 // Multistage2015.h
 // ==============================================================


/* #define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE */
#define MSVERSION 170506

#define MAXLEN 4096
#define TBOOSTER 0
#define TSTAGE 1
#define TPAYLOAD 2
#define TFAIRING 3
#define TINTERSTAGE 4
#define TLES 5

#define STAGE_SHUTDOWN 1
#define STAGE_IGNITED 2
#define STAGE_WAITING 3

#define FXMACH 0
#define FXVENT 1
//#define ORBITER_MODULE

#define TLMSECS 7200

#include <iostream>
#include <fstream>
#include <string>
//#include "OrbiterSoundSDK40.h"
#include <time.h>
#include "..//..//Orbitersdk//include//Orbitersdk.h"
#include "windows.h"
#include "commctrl.h"
#include "..//..//Orbitersdk//include//DlgCtrl.h"
#include "resource.h"
//#include "DevModeCtrl.h"


/* #include <mmsystem.h>
#pragma comment (lib, "winmm.lib") */
#include <array>


using namespace std;


struct VECTOR4F {
	double x;
	double y;
	double z;
	double t;
	VECTOR4F()
	{
		x = 0;
		y = 0;
		z = 0;
		t = 0;
	}
};
struct VECTOR2 {
	double x;
	double y;
};
struct CUTOFF_CONDITIONS {
	double T;
	int stage;
	int rem_stages;
	double r_T;
	double rdot_T;
	double remBT[10];
};
struct BATTS {
	bool wBatts;
	double MaxCharge;
	double CurrentCharge;
	BATTS()
	{
		wBatts = FALSE;
		MaxCharge = 0;
		CurrentCharge = 0;
	}
};
struct EXPBOLT {
	VECTOR3 pos;
	VECTOR3 dir;
	std::string pstream;
	double anticipation;
	bool wExpbolt;
	THRUSTER_HANDLE threxp_h;
	EXPBOLT()
	{
		threxp_h = nullptr;
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 0);
		anticipation = 0;
		wExpbolt = FALSE;
	}
};
struct MISC {
	double COG;
	int GNC_Debug;
	bool telemetry;
	int Focus;
	bool thrustrealpos;
	double VerticalAngle;

	//added by rcraig42 to store drag_factor -----------------------------------------------

	double drag_factor;
	char PadModule[MAXLEN];
	//---------------------------------------------------------------------------------------
	MISC()
	{
		COG = 0;
		GNC_Debug = 0;
		telemetry = FALSE;
		Focus = 0;
		thrustrealpos = FALSE;
		VerticalAngle = 0;
		drag_factor = 0;
		for (int i = 0; i < MAXLEN; i++)
		{
			PadModule[i] = '\0';
		}
	}
};
struct TEX {
	char TextureName[MAXLEN][16];
	SURFHANDLE hTex[16];
	/*TEX()
	{
		for(UINT i=0;i<16;i++)
		{
			TextureName[0][i]='\0';
			hTex[i]=NULL;
		}
	}*/
};
struct PARTICLE {
	char ParticleName[MAXLEN];
	PARTICLESTREAMSPEC Pss;
	double GrowFactor_size;
	double GrowFactor_rate;
	bool Growing;
	char TexName[256];
	PARTICLE()
	{
		for (UINT i = 0; i < MAXLEN; i++)
		{
			ParticleName[i] = '\0';
		}
		GrowFactor_size = 0;
		GrowFactor_rate = 0;
		Growing = FALSE;
		for (UINT j = 0; j < 256; j++)
		{
			TexName[j] = '\0';
		}
		Pss.amax = 0;
		Pss.amin = 0;
		Pss.atmslowdown = 0;
		Pss.atmsmap = PARTICLESTREAMSPEC::ATM_FLAT;
		Pss.growthrate = 0;
		Pss.levelmap = PARTICLESTREAMSPEC::LVL_FLAT;
		Pss.lifetime = 0;
		Pss.lmax = 0;
		Pss.lmin = 0;
		Pss.ltype = PARTICLESTREAMSPEC::DIFFUSE;
		Pss.srcrate = 0;
		Pss.srcsize = 0;
		Pss.srcspread = 0;
		Pss.tex = NULL;
		Pss.v0 = 0;
	}

};

struct PSGROWING {
	PSTREAM_HANDLE psh[3];
	PARTICLESTREAMSPEC pss;
	double GrowFactor_size;
	double GrowFactor_rate;
	THRUSTER_HANDLE th;
	VECTOR3 pos;
	bool growing;
	int status;
	bool counting;
	double doublepstime;
	double basesize;
	double baserate;
	VECTOR3 basepos;
	bool ToBooster;
	int nItem;
	int nEngine;
	bool FirstLoop;
	//int Th_idx;
	PSGROWING()
	{
		pss = {0};
		psh[0] = 0;
		psh[1] = 0;
		psh[2] = 0;
		//psh[3]=0;
		//pss=0;
		GrowFactor_size = 0;
		GrowFactor_rate = 0;
		th = NULL;
		pos = _V(0, 0, 0);
		growing = FALSE;
		status = 0;
		counting = FALSE;
		doublepstime = 0;
		basesize = 0;
		baserate = 0;
		basepos = _V(0, 0, 0);
		ToBooster = FALSE;
		nItem = 0;
		nEngine = 0;
		FirstLoop = TRUE;
		//	Th_idx=0;
	}
};
struct BOOSTER {
	//mandatory
	int N;
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double thrust;
	double emptymass;
	double fuelmass;
	double burntime;
	double angle;
	//unmandatory
	double burndelay;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	VECTOR3 eng[4];
	double engine_phase[32];
	double engine_amp[32];
	double freq[32];
	double eng_diameter;
	std::string eng_tex;
	std::string eng_pstream1;
	bool wps1;
	std::string eng_pstream2;
	bool wps2;
	VECTOR3 eng_dir;
	THGROUP_HANDLE Thg_boosters_h;
	EXPBOLT expbolt;
	//derived parameters
	double isp;
	double volume;
	PROPELLANT_HANDLE tank;
	std::array<THRUSTER_HANDLE, 10> th_booster_h;
	//THRUSTER_HANDLE th_booster_h[10];
	std::array<MESHHANDLE, 10> msh_h;
	//MESHHANDLE msh_h[10];
	std::array<int, 10> msh_idh;
	//int msh_idh[10];
	int nEngines;
	double currDelay;
	//VECTOR3 curve[10];
	std::array<VECTOR3, 10> curve;
	double IgnitionTime;
	bool Ignited;
	bool PSSdefined;
	bool ParticlesPacked;
	int ParticlesPackedToEngine;
	BOOSTER()
	{

		isp = 0.0;
		currDelay = 0.0;
		Thg_boosters_h = nullptr;
		IgnitionTime = 0.0;
		volume = 0.0;
		th_booster_h.fill(nullptr);
		tank = nullptr;
		msh_idh.fill(0);
		msh_h.fill({});
		isp = 0.0;
		currDelay = 0.0;
		Thg_boosters_h = nullptr;
		IgnitionTime = 0.0;
		N = 0;
		wps1 = FALSE;
		wps2 = FALSE;
		off = _V(0, 0, 0);
		height = 0;
		diameter = 0;
		thrust = 0;
		emptymass = 0;
		fuelmass = 0;
		burntime = 0;
		angle = 0;
		burndelay = 0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);

		for (UINT i = 0; i < 4; i++)
		{
			eng[i] = _V(0, 0, 0);
		}
		for (UINT j = 0; j < 32; j++)
		{
			engine_phase[j] = 0;
			engine_amp[j] = 0;
			freq[j] = 0;
		}
		eng_diameter = 0;
		eng_tex[0] = '\0';

		eng_dir = _V(0, 0, 0);
		Ignited = FALSE;
		PSSdefined = FALSE;
		nEngines = 0;
		for (UINT k = 0; k < 10; k++)
		{
			curve[k] = _V(9000000, 100, 0);
		}
		ParticlesPacked = FALSE;
		ParticlesPackedToEngine = 0;
		expbolt = EXPBOLT();
	}
};
struct INTERSTAGE {
	//mandatory
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double volume;
	double emptymass;
	//unmandatory
	double separation_delay;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	MESHHANDLE msh_h;
	int msh_idh;
	double currDelay;
	INTERSTAGE()
	{
		off = _V(0, 0, 0);
		height = 0;
		diameter = 0;
		volume = 0;
		emptymass = 0;
		separation_delay = 0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		currDelay = 0;
	}
};
struct ADAPTER {
	//mandatory
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double emptymass;

	MESHHANDLE msh_h;
	int msh_idh;
	ADAPTER()
	{
		off = _V(0, 0, 0);
		height = 0;
		diameter = 0;
		emptymass = 0;
	}
};

struct LES {
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double emptymass;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	MESHHANDLE msh_h;
	int msh_idh;
	double volume;
	LES()
	{
		height = 0;
		diameter = 0;
		emptymass = 0;
		off = _V(0, 0, 0);
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		volume = 0;
	}
};
struct ULLAGE {
	bool wUllage;
	bool ignited;
	double thrust;
	double anticipation;
	double overlap;
	int N;
	double angle;
	VECTOR3 pos;
	VECTOR3 dir;
	double length;
	double diameter;
	std::string tex;
	THRUSTER_HANDLE th_ullage;
	double rectfactor;
	ULLAGE()
	{
		wUllage = FALSE;
		ignited = FALSE;
		thrust = 0;
		anticipation = 0;
		overlap = 0;
		N = 0;
		angle = 0;
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 0);
		length = 0;
		diameter = 0;
		rectfactor = 0;
	}
};
struct STAGE {
	//mandatory
	std::string meshname;
	VECTOR3 off;
	double height;
	double diameter;
	double thrust;
	double emptymass;
	double fuelmass;
	double burntime;
	//unmandatory


//added by rcraig42 to store ispref and reference pressure----------------------------------------------

	double ispref;
	double pref;

	//----------------------------------------------------------------------------------------------------	

	double waitforreignition;
	BATTS batteries;
	double ignite_delay;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	double pitchthrust;
	bool defpitch;
	double yawthrust;
	bool defyaw;
	double rollthrust;
	bool defroll;
	//VECTOR3 eng[32];
	std::array<VECTOR3, 32> eng;
	double engine_phase[32];
	double engine_amp[32];
	double freq[32];
	//VECTOR4F engV4[32];
	std::array<VECTOR4F, 32> engV4;
	double eng_diameter;
	std::string eng_tex;
	std::string eng_pstream1;
	bool wps1;
	bool wps2;
	std::string eng_pstream2;
	VECTOR3 eng_dir;
	ULLAGE ullage;
	EXPBOLT expbolt;
	//derived parameters
	double isp;
	double volume;
	PROPELLANT_HANDLE tank;
	std::array<THRUSTER_HANDLE, 32> th_main_h;
	std::array<THRUSTER_HANDLE, 2> th_att_h;
	//THRUSTER_HANDLE th_main_h[32], th_att_h[2];
	MESHHANDLE msh_h;
	int msh_idh;
	int nEngines;
	bool wInter;
	int IntIncremental;
	INTERSTAGE interstage;
	bool Ignited;
	double currDelay;
	double IgnitionTime;
	bool reignitable;
	bool DenyIgnition;
	bool wBoiloff;
	int StageState;
	bool ParticlesPacked;
	int ParticlesPackedToEngine;
	double linearthrust;
	double linearisp;
	//bool NoEngDef;
	STAGE()
	{
		wps1 = FALSE;
		wps2 = FALSE;
		off = _V(0, 0, 0);
		height = 0;
		diameter = 0;
		thrust = 0;
		emptymass = 0;
		fuelmass = 0;
		burntime = 0;
		waitforreignition = 0;
		batteries = BATTS();
		ignite_delay = 0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		pitchthrust = 0;
		rollthrust = 0;
		yawthrust = 0;

		defpitch = FALSE;
		defyaw = FALSE;
		defroll = FALSE;
		for (UINT j = 0; j < 32; j++)
		{
			eng[j] = _V(0, 0, 0);
			engine_phase[j] = 0;
			engine_amp[j] = 0;
			freq[j] = 0;
			engV4[j] = VECTOR4F();
		}
		eng_diameter = 0;
		eng_dir = _V(0, 0, 0);
		ullage = ULLAGE();
		expbolt = EXPBOLT();
		isp = 0;
		volume = 0;
		nEngines = 0;
		wInter = FALSE;
		IntIncremental = 0;
		interstage = INTERSTAGE();
		Ignited = FALSE;
		currDelay = 0;
		IgnitionTime = 0;
		reignitable = TRUE;
		DenyIgnition = FALSE;
		wBoiloff = FALSE;
		StageState = 0;
		ParticlesPacked = FALSE;
		ParticlesPackedToEngine = 0;
		linearthrust = 0;
		linearisp = 0;
		ispref = 0;
		pref = 0;
		//NoEngDef=FALSE;
	}
};
struct PAYLOAD {
	//mandatory
	std::string meshname;
	std::string meshname0;
	std::string meshname1;
	std::string meshname2;
	std::string meshname3;
	std::string meshname4;

	//VECTOR3 off[5];
	std::array<VECTOR3, 5> off;
	double height;
	double diameter;
	double mass;
	std::string module;
	std::string name;
	//unmandatory
	char MultiOffset[128];
	VECTOR3 speed;
	VECTOR3 rot_speed;

	double volume;
	int render;
	int nMeshes;
	MESHHANDLE msh_h[5];
	int msh_idh[5];
	VECTOR3 Rotation;
	bool rotated;
	bool live;

	PAYLOAD()
	{
		for (int i = 0; i < 5; i++) {
			off[i] = _V(0, 0, 0);
			msh_h[i] = NULL;
			msh_idh[i] = 0;
		}
		height = 0;
		diameter = 0;
		mass = 0;
		module[0] = '\0';
		name[0] = '\0';
		MultiOffset[0] = '\0';
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);
		volume = 0;
		render = 0;
		nMeshes = 0;
		Rotation = _V(0, 0, 0);
		rotated = FALSE;
		live = FALSE;
	}
};
struct FAIRING {
	//mandatory
	int N;
	std::string meshname;
	VECTOR3 off;
	double angle;
	double height;
	double diameter;
	double emptymass;
	//unmandatory
	double volume;
	VECTOR3 speed;
	VECTOR3 rot_speed;
	std::string module;
	MESHHANDLE msh_h[10];
	int msh_idh[10];
	FAIRING()
	{
		N = 0;
		off = _V(0, 0, 0);
		angle = 0;
		height = 0;
		diameter = 0;
		emptymass = 0;
		volume = 0;
		speed = _V(0, 0, 0);
		rot_speed = _V(0, 0, 0);

		for (UINT i = 0; i < 10; i++)
		{
			msh_idh[i] = 0;
			msh_h[i] = NULL;
		}
	}
};
enum GNC_Comand { CM_ROLL, CM_PITCH, CM_ENGINE, CM_FAIRING, CM_LES, CM_DISABLE_PITCH, CM_DISABLE_ROLL, CM_DISABLE_JETTISON, CM_PLAY, CM_JETTISON, CM_TARGET, CM_AOA, CM_ATTITUDE, CM_SPIN, CM_INVERSE, CM_ORBIT, CM_ENGINEOUT, CM_DEFAP, CM_GLIMIT, CM_DESTROY, CM_EXPLODE, CM_NOLINE };
struct GNC_STEP {
	double time;
	char Comand[128];
	GNC_Comand GNC_Comand;
	double val_init;
	double val_fin;
	double time_init;
	double time_fin;
	double duration;
	double trval1, trval2, trval3, trval4, trval5, trval6;
	bool executed;
	char trchar[256];
	bool wValue[6];
};
struct SSOUND {
	char MAIN_THRUST[MAXLEN];
	bool Main;
	char HOVER_THRUST[MAXLEN];
	bool Hover;
	char RCS_THRUST_ATTACK[MAXLEN];
	bool RCS_ta;
	char RCS_THRUST_SUSTAIN[MAXLEN];
	bool RCS_ts;
	char AIR_CONDITIONNING[MAXLEN];
	bool Ac;
	char COCKPIT_AMBIENCE_1[MAXLEN];
	bool Ca1;
	char COCKPIT_AMBIENCE_2[MAXLEN];
	bool Ca2;
	char COCKPIT_AMBIENCE_3[MAXLEN];
	bool Ca3;
	char COCKPIT_AMBIENCE_4[MAXLEN];
	bool Ca4;
	char COCKPIT_AMBIENCE_5[MAXLEN];
	bool Ca5;
	char COCKPIT_AMBIENCE_6[MAXLEN];
	bool Ca6;
	char COCKPIT_AMBIENCE_7[MAXLEN];
	bool Ca7;
	char COCKPIT_AMBIENCE_8[MAXLEN];
	bool Ca8;
	char COCKPIT_AMBIENCE_9[MAXLEN];
	bool Ca9;
	int GncStepSound[57];
};
struct FX_LAUNCH {
	double H;
	int N;
	double Angle;
	double Distance;
	double CutoffAltitude;
	char Ps1[MAXLEN];
	char Ps2[MAXLEN];
	FX_LAUNCH()
	{
		for (UINT i = 0; i < MAXLEN; i++)
		{
			Ps1[i] = '\0';
			Ps2[i] = '\0';
		}
		H = 0;
		N = 0;
		Angle = 0;
		Distance = 0;
		CutoffAltitude = 0;
	}
};
struct FX_MACH {
	double mach_min;
	double mach_max;
	std::string pstream;
	bool added;
	std::array<VECTOR3, 10> off;
	//VECTOR3 off[10];
	VECTOR3 dir;
	PSTREAM_HANDLE ps_h[10];
	int nmach;

	FX_MACH()
	{
		mach_min = 0;
		mach_max = 0;
		added = FALSE;
		dir = _V(0, 0, 0);
		nmach = 0;
		off.fill(_V(0, 0, 0));
	}
};
struct FX_VENT {
	//double time_fin[11];
	std::array<double, 11> time_fin;
	std::string pstream;
	std::array<VECTOR3, 11> off;
	//VECTOR3 off[11];
	std::array<VECTOR3, 11> dir;
	//VECTOR3 dir[11];
	PSTREAM_HANDLE ps_h[11];
	//bool added[11];
	std::array<bool, 11> added;
	int nVent;
	FX_VENT()
	{
		nVent = 0;
		for (UINT i = 0; i <= 10; i++)
		{
			time_fin[i] = 0;
			off[i] = _V(0, 0, 0);
			dir[i] = _V(0, 0, 0);
			added[i] = FALSE;
		}
	}
};

struct Fuel_Levels {
	double Stage_Fuel_Lvls[10];
	double Boosters_Fuel_Lvls[10];
};


class Multistage2015 :public VESSEL3 {
	//	friend class test;
	//	test Try;
	friend class DevModeDlg;
	DevModeDlg* DMD;
public:
	Multistage2015(OBJHANDLE hObj, int fmodel);
	~Multistage2015();

	void CreateDMD();
	void DestroyDMD();
	void clbkSetClassCaps(FILEHANDLE cfg) override;
	int clbkConsumeBufferedKey(DWORD key, bool down, char* kstate) override;
	int clbkConsumeDirectKey(char* kstate) override;
	void clbkLoadStateEx(FILEHANDLE scn, void* vs) override;
	void clbkSaveState(FILEHANDLE scn) override;
	void clbkPostStep(double simt, double simdt, double mjd) override;
	void clbkPreStep(double simt, double simdt, double mjd) override;
	void clbkPostCreation() override;
	bool clbkDrawHUD(int mode, const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) override;
	//void clbkVisualCreated (VISHANDLE _vis, int refcount);
	//void clbkVisualDestroyed (VISHANDLE _vis, int refcount);
	int clbkGeneric(int msgid, int prm, void* context) override;
	int MyID;
	//VISHANDLE vis;
	THGROUP_HANDLE thg_h_main;

	//Making Public Some Vars
   // virtual double GetTgtApo();
   // virtual Fuel_Levels GetFuelLevels();
	//double testing;
	PSTREAM_HANDLE AddExhaustStreamGrowing(THRUSTER_HANDLE  th, const VECTOR3& pos, PARTICLESTREAMSPEC* pss, bool growing, double growfactor_size, double growfactor_rate, bool count, bool ToBooster, int N_Item, int N_Engine);
	void ManageParticles(double dt, bool prestep);
	double particlesdt;
	bool GrowingParticles;
	double RefPressure;
	VECTOR4F _V4(double x, double y, double z, double t);
	VECTOR2 _V2(double x, double y);
	bool IsOdd(int integer);
	GNC_STEP Gnc_step[150]; //MAx 150 Steps
	void AutoJettison();
	void initGlobalVars();
	void VehicleSetup();
	void RotatePayload(int pns, int nm, VECTOR3 anglesrad);
	void LoadMeshes();
	void UpdateMass();
	void UpdatePMI();
	double TotalHeight;

	void UpdateOffsets(); ///////*********
	void CreateUllageAndBolts();
	char dataparsed[MAXLEN];
	bool parseinifile(char filename[MAXLEN]);
	void parseStages(char filename[MAXLEN]);
	void parseBoosters(char filename[MAXLEN]);
	void parseInterstages(char filename[MAXLEN], int parsingstage);
	void parseAdapter(char filename[MAXLEN]);
	void parseLes(char filename[MAXLEN]);
	void parseFairing(char filename[MAXLEN]);
	void parsePayload(char filename[MAXLEN]);
	void parseMisc(char filename[MAXLEN]);
	void parseTexture(char filename[MAXLEN]);
	void parseParticle(char filename[MAXLEN]);
	void parseFXMach(char filename[MAXLEN]);
	void parseFXVent(char filename[MAXLEN]);
	void parseFXLaunch(char filename[MAXLEN]);
	void parseSound(char filename[MAXLEN]);
	VECTOR3 GetBoosterPos(int nBooster, int N);
	void ArrangePayloadMeshes(char data[MAXLEN], int pnl);
	char* GetProperPayloadMeshName(int pnl, int n);
	void ArrangePayloadOffsets(const std::string &data, int pnl);
	void Jettison(int type, int current);
	void Spawn(int type, int current);
	VECTOR3 CharToVec(const std::string &str);
	VECTOR4F CharToVec4(const std::string &str);
	int Configuration;
	double CogElev;

	char OrbiterRoot[MAXLEN];
	char fileini[MAXLEN];
	char guidancefile[MAXLEN];
	int nStages;
	int nBoosters;
	int nInterstages;
	int nPayloads;
	int nTextures;
	int nParticles;
	int currentStage;
	int currentBooster;
	int currentInterstage;
	int currentPayload;
	bool hasFairing;
	bool wLes;
	bool wMach;
	bool wVent;
	bool wLaunchFX;
	int wFairing;
	std::array<STAGE, 10> stage;
	//STAGE stage[10];
	std::array<BOOSTER, 10> booster;
	//BOOSTER booster[10];
	//PAYLOAD payload[10];
	std::array<PAYLOAD, 10> payload;
	FAIRING fairing;
	MISC Misc;
	TEX tex;
	PARTICLE Particle[16];
	double currentDelta;
	int nPsh;
	int nPsg;
	PSGROWING* psg;

	//bool *ParticleFirstLoop;
	ADAPTER Adapter;
	LES Les;
	SSOUND Ssound;
	FX_MACH FX_Mach;
	FX_VENT FX_Vent;
	FX_LAUNCH FX_Launch;
	void CreateRCS();
	void CreateMainThruster();
	bool wBoosters;
	bool wAdapter;
	char logbuff[MAXLEN];
	char buffreset[MAXLEN];
	double MET;
	SURFHANDLE GetProperExhaustTexture(char name[MAXLEN]);
	void FLY(double simtime, double simdtime, double mjdate);
	VECTOR3 RotateVecZ(VECTOR3 input, double Angle);
	PARTICLE GetProperPS(char name[MAXLEN]);
	void InitializeDelays();
	double stage_ignition_time;
	virtual void ToggleComplexFlight();
	//bool wVinkasGuidance;
	virtual VECTOR3 hms(double time);
	virtual double RemBurnTime(int rbtstage, double level);
	virtual double BoosterRemBurnTime(int rbtbooster, double level);
	double StageDv(int dvstage);
	double DvAtStage(int dvatstage);
	double StageRemDv(int dvstage);
	double CurrentStageRemDv();
	VECTOR3 RotateVector(const VECTOR3& input, double angle, const VECTOR3& rotationaxis);
	ATTACHMENTHANDLE live_a[10];
	void UpdateLivePayloads();
	MATRIX3 RotationMatrix(VECTOR3 angles);
	void CheckForAdditionalThrust(int pns);
	void CheckForFX(int fxtype, double param);
	// int nVent;
	double lvl;


	UINT exhaustN[10][32];
	virtual void WriteGNCFile();
	//GUIDANCE
	void parseGuidanceFile(char filename[MAXLEN]);
	int nsteps;
	void VinkaComposeGNCSteps();
	GNC_STEP VinkaComposeSpecificGNCSteps(GNC_STEP gnc);
	virtual void VinkaDeleteStep(int q);
	virtual void VinkaRearrangeSteps();
	virtual void VinkaAddStep(char input[MAXLEN]);
	int VinkaCountSteps();
	double perror;
	double yerror;
	double rerror;
	double pintegral;
	double kp;
	double kd;
	double ki;

	double Tu;
	double Ku;

	VECTOR3 MaxTorque;
	void Attitude(double pitch, double roll, double heading, double pitchrate, double rollrate, double yawrate);
	double VinkaAzimuth;
	double VinkaMode;
	virtual int VinkaGetStep(double met);
	void VinkaPitch(int step);
	void VinkaRoll(int step);
	void VinkaEngine(int step);
	void VinkaConsumeStep(int step);
	void VinkaAutoPilot();
	void VinkaCheckInitialMet();
	virtual double GetProperHeading();
	virtual double GetHeading();
	double GetProperRoll(double RequestedRoll);
	double initlong, initlat;
	int VinkaFindFirstPitch();
	int VinkaFindRoll();
	void VinkaUpdateRollTime();
	bool APstat;
	double finalv(double Abside, double Apo, double Peri);
	bool AJdisabled;
	bool rolldisabled;
	bool pitchdisabled;
	void killAP();
	double VinkaFindEndTime();
	int Gnc_running;
	void Guidance_Debug();
	string value1, value2, value3, value4, value5, value6;
	bool spinning;
	bool AttCtrl;
	bool PitchCtrl;
	bool YawCtrl;
	bool RollCtrl;
	virtual void ToggleAttCtrl(bool Pitch, bool Yaw, bool Roll);
	virtual void ToggleAP();
	double DeltaUpdate;
	bool wPeg;
	double TgtPitch;
	double GetVPerp();
	double altsteps[4];
	bool stepsloaded;
	virtual void SetNewAltSteps(double newstep1, double newstep2, double newstep3, double newstep4);
	bool Complex;
	void EvaluateComplexFlight();
	void ComplexFlight();
	double UpdateComplex;
	void CalculateAltSteps(double planetmass);
	////COMPUTATIONAL GRAVITY TURN EVALUATION

	double GetProperNforCGTE(double time);
	bool CGTE(double psi0);
	double GT_InitPitch;
	double GT_IP_Calculated;


	//Some More Utilities
	double GetMassAtStage(int MassStage, bool empty);
	double CalcAzimuth();
	double getabsacc();
	double getacc();
	double GetOS();
	int SignOf(double X);

	void Boiloff();





	void CalcMECO();

	////PEG
	bool runningPeg;
	bool CutoffCheck();
	ORBITPARAM op;
	ELEMENTS el;
	double epsfin;
	double eps;
	CUTOFF_CONDITIONS coc;
	void FEstimate();

	double MECO_TEST;
	double coeff[100];
	double avgcoeff;
	int MECO_Counter;
	double TMeco;
	void FStaging();
	double PegPitchLimit;
	virtual void SetPegPitchLimit(double newlimit);
	double r_T[10];
	double rdot_T[10];
	double T[10];
	double DeltaA[10];
	double DeltaB[10];
	double A[10];
	double B[10];
	double delta_rdot[10];
	double delta_r[10];
	double r_in[10];
	double rdot_in[10];
	double VthetaAtStaging[10];
	double OmegaS[10];
	double a_fin[10];


	double tgtapo;
	double tgtperi;
	double tgtinc;
	double tgtabside;
	double tgtvv;
	double Azimuth;
	double mu;
	double rt;
	double g0;

	//Navigation vars
	double r;
	double v;
	double z;
	VECTOR3 hvec;
	VECTOR3 rvec;
	VECTOR3 vvec;
	double h;
	VECTOR3 rhat;
	VECTOR3 hhat;
	VECTOR3 thetahat;
	VECTOR3 fhat;
	double thrust;
	double mass;
	double a_[10];
	double v_e[10];
	double tau_[10];
	double omega;
	double rdot;
	double vtheta;
	VECTOR3 ShipSpeed;
	double VertVel;
	int NN;
	double PegMajorCycleInterval;
	virtual void SetPegMajorCycleInterval(double newinterval);

	//feedback vars
	double g;
	double cent;
	double g_term;
	double cent_term;
	int J;


	void InitPEG();
	void Navigate();
	double b0(int j, double t_j);
	double b_(int n, int j, double t_j);
	double c0(int j, double t_j);
	double c_(int n, int j, double t_j);
	double a(int j, double t_j);

	void CalculateTargets();
	void MajorCycle();
	double UpdatePegTimer;
	double PegDesPitch;

	void PEG();

	//FAILURES
	int failureProbability;
	int timeOfFailure;
	bool wFailures;
	bool failed;
	void Failure();
	void FailuresEvaluation();
	bool CheckForFailure(double met);
	void boom();



	//TELEMETRY
	char tlmfile[MAXLEN];
	VECTOR2 tlmAlt[TLMSECS];
	VECTOR2 tlmSpeed[TLMSECS];
	VECTOR2 tlmPitch[TLMSECS];
	VECTOR2 tlmThrust[TLMSECS];
	VECTOR2 tlmMass[TLMSECS];
	VECTOR2 tlmVv[TLMSECS];
	VECTOR2 tlmAcc[TLMSECS];
	VECTOR2 ReftlmAlt[TLMSECS];
	VECTOR2 ReftlmSpeed[TLMSECS];
	VECTOR2 ReftlmPitch[TLMSECS];
	VECTOR2 ReftlmThrust[TLMSECS];
	VECTOR2 ReftlmMass[TLMSECS];
	VECTOR2 ReftlmVv[TLMSECS];
	VECTOR2 ReftlmAcc[TLMSECS];

	double updtlm;
	double writetlmTimer;
	int tlmidx;
	void Telemetry();
	int tlmnlines;
	int loadedtlmlines;
	virtual int WriteTelemetryFile(int initline);
	virtual void parseTelemetryFile(char filename[MAXLEN]);
	double updtboiloff;
	bool wReftlm;


	double CalculateFullMass();


	void ResetVehicle(VECTOR3 hangaranimsV, bool Ramp);
	VECTOR3 hangaranims;
	FILEHANDLE scenario;
	FILEHANDLE config;
	int loadedCurrentBooster;
	int loadedCurrentInterstage;
	int loadedCurrentStage;
	int loadedCurrentPayload;
	int loadedwFairing;
	int loadedConfiguration;
	bool loadedGrowing;
	double loadedMET;
	VESSELSTATUS2 vs2;
	MATRIX3 RotMatrix;
	bool DeveloperMode;
	bool killDMD;
	NOTEHANDLE note;
private:
	MGROUP_ROTATE* payloadrotatex[10][5];
	MGROUP_ROTATE* payloadrotatey[10][5];
	MGROUP_ROTATE* payloadrotatez[10][5];
	UINT RotatePayloadAnim_x[10][5];
	UINT RotatePayloadAnim_y[10][5];
	UINT RotatePayloadAnim_z[10][5];
	ANIMATIONCOMPONENT_HANDLE anim_x[10][5];
	ANIMATIONCOMPONENT_HANDLE anim_y[10][5];
	ANIMATIONCOMPONENT_HANDLE anim_z[10][5];


	void Ramp(bool alreadyramp);
	//bool RampCreated;
	//bool AttachedToRamp;
	OBJHANDLE hramp;
	OBJHANDLE hhangar;
	OBJHANDLE hCrawler;
	VESSEL3* vCrawler;
	VESSEL3* vramp;
	VESSEL3* vhangar;
	VESSELSTATUS2 vsramp, vshangar;
	ATTACHMENTHANDLE AttToRamp, padramp, AttToHangar, PadHangar, AttToCrawler;
	//bool RampDeleted;
	bool wRamp;
	bool NoMoreRamp;
	int GetMSVersion();


	COLOUR4 col_d;
	COLOUR4 col_s;
	COLOUR4 col_a;
	COLOUR4 col_white;

	double th_main_level;
	double launchFx_level;

	void CreateLaunchFX();

	bool HangarMode;
	bool wCrawler;
	bool wCamera;
	double CamDLat, CamDLng;
	void CreateHangar();
	void CreateCamera();
	bool CheckForDetach();
	bool AttToMSPad;
	void AttachToMSPad(OBJHANDLE hPad);
	VECTOR3 MsPadZ;
	double GetMET();
	bool GetAutopilotStatus();
	VECTOR3 GetAPTargets();

	//virtual void UpdateMeshVisual(UINT *msh_id, VECTOR3 Rotation);
};

#endif