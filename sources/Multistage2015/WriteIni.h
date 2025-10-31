#pragma once
#ifndef __WRITEINI_H_
#define __WRITEINI_H_
#define STRICT
#include "Multistage2015.h"
#include "..//..//Orbitersdk//include//Orbitersdk.h"

class WriteIni {
	friend class DevModeDlg;
public:
	WriteIni();
	~WriteIni();
	bool WritePayload(PAYLOAD pld, UINT idx, bool Erase = FALSE);
	bool WriteFairing(FAIRING frg, bool wFairing);
	bool WriteParticle(PARTICLE prt, UINT idx, bool Erase = FALSE);
	bool WriteMisc(MISC msc);
	bool WriteTex(TEX tex, UINT ntextures);
	bool WriteBooster(BOOSTER bst, UINT idx, bool Erase = FALSE);
	bool WriteStage(STAGE stg, UINT idx, bool Erase = FALSE);
	bool WriteLes(LES les, bool wLes);
	bool WriteAdapter(ADAPTER adp, bool wAdapter);
	bool WriteFX_Mach(FX_MACH fxm, bool wMach);
	bool WriteFX_Vent(FX_VENT fxv, bool wVent);
	bool WriteFX_Launch(FX_LAUNCH fxl, bool wLaunch);
	//BOOL WNull();
	BOOL W(LPCSTR b, LPCSTR c);
	BOOL Wvec(LPCSTR b, VECTOR3 v);
	BOOL Wdouble(LPCSTR b, double c);
	BOOL WLongDouble(LPCSTR b, double c);
	BOOL Wint(LPCSTR b, int c);
	BOOL Wbool_int(LPCSTR b, bool c);
	char* VecToChar(VECTOR3 v);
	void SetFilename(char fn[MAXLEN]);
	void SetCurrentSection(char Section[MAX_PATH]);
	char sectiontxt[MAX_PATH];
private:
	char filename[MAXLEN];

};

#endif