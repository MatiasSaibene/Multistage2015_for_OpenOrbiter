#pragma once
#define STRICT
//#include "Multistage2015.h"
#include <iostream>
#include <fstream>
#include <string>
//#include "OrbiterSoundSDK40.h"
#include <time.h>
#include "..//..//Orbitersdk//include//Orbitersdk.h"
#include "WriteIni.h"

/*#include "windows.h"
#include "commctrl.h"
#include "DlgCtrl.h"
#include "resource.h"
*/
#define CD_PLD 0
#define CD_STAGE 1
#define CD_BOOSTER 2
#define CD_FAIRING 3
#define CD_PARTICLE 4
#define CD_TEX 5
#define CD_MISC 6
#define CD_VIEW 7
#define CD_CURVE 8
#define CD_ULLAGE 9
#define CD_INTERSTAGE 10
#define CD_LES 11
#define CD_ADAPTER 12
#define CD_FX 13
#define CD_SCENARIO 14

class Graph;

class DevModeDlg {
	friend class Multistage2015;
	friend class WriteIni;
public:
	DevModeDlg(Multistage2015* _Ms15);
	~DevModeDlg();

	enum ItemType { Stage, Booster, Payload, Fairing, Misc, Texture, Interstage, Particle, Empty };

	void InitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool InitPrms, ItemType Expand);
	void Notify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL PldProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ViewTxtProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL FairingProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ParticleProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL MiscProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL TexProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL BoosterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL CurveProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL StageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL UllageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL InterstageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL LesProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL AdapterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL FXProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ScenarioProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Open(HINSTANCE hInstance);
	int Closed();

	HWND hChild[15];
	HWND hDlg;

private:
	WriteIni WriteIni;

	ItemType Viewing;


	HINSTANCE hInst;
	Multistage2015* Ms15;
	STAGE DMD_stage[10];
	BOOSTER DMD_booster[10];
	PAYLOAD DMD_payload[10];
	FAIRING DMD_fairing;
	MISC DMD_Misc;
	TEX DMD_tex;
	ADAPTER DMD_Adapter;
	LES DMD_Les;
	PARTICLE DMD_Particle[16];
	FX_VENT DMD_FX_Vent;
	FX_MACH DMD_FX_Mach;
	FX_LAUNCH DMD_FX_Launch;
	UINT DMD_nStages;
	UINT DMD_nBoosters;
	UINT DMD_nInterstages;
	UINT DMD_nParticles;
	UINT DMD_nPayloads;
	UINT DMD_nTextures;
	PSTREAM_HANDLE test_part_h;
	PARTICLESTREAMSPEC test_particle;
	double sliderlvl;
	bool TestingParticle;
	int DMD_wFairing;
	bool DMD_wAdapter;
	bool DMD_wLes;
	bool DMD_wMach;
	bool DMD_wVent;
	bool DMD_wFXLaunch;
	void InitParams();
	void PopulateChildDialog(UINT iddialog, UINT id);
	void PopulateBoosters(UINT iddialog, UINT id);
	void PopulateStages(UINT iddialog, UINT id);
	char logbuff[MAXLEN];
	char* PickFileName(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool NoExtension);
	//	void ArrangePayloadMeshes(char data[MAXLEN], int pnl);
	//	void ArrangePayloadOffsets(char data[MAXLEN], int pnl);
	void AddItem(ItemType IT);
	void DelItem(ItemType IT, UINT idx);
	void SaveItems(ItemType IT);
	void ReverseOffsets();
	char CompleteFileIniName[MAX_PATH];

	Graph* Curve;
	void RefreshGraph(Graph* graph, int GraphId);
	POINT point[10];
	UINT BoosterCurveNpts(BOOSTER bst);
	void BoosterCurvePointActive(UINT npts);
	void BoosterCurveReadPoints();
	void BoosterDrawGraph(UINT npts);
	void StageActiveEngines(UINT nEngines);
	void SendMultipleDlgItemsMessage(HWND hDlg, int* idc, UINT Msg, WPARAM wParam, LPARAM lParam, UINT nInputs);
	void SetMultipleDlgItemsText(HWND hDlg, int* idc, string* str, UINT nInputs);
	void removeSpaces(char* s);
	void SaveUllage(UINT stageid);
	void SaveInterstage(UINT stageid);
	double GetDlgItemDouble(HWND hDlg, int Idd_ctrl, int cchMax);
	//int GetDlgItemInt(HWND hDlg, int Idd_ctrl ,  int cchMax);
	VECTOR3 GetDlgItemVector3(HWND hDlg, int Idd_ctrlx, int Idd_ctrly, int Idd_ctrlz, int cchMax);
	VECTOR4F GetDlgItemVector4(HWND hDlg, int Idd_ctrlx, int Idd_ctrly, int Idd_ctrlz, int Idd_ctrlt, int cchMax);

	bool hideFairing;
	bool hideHalfFairing;

	bool SaveScenarioFile(bool defname);
};

INT_PTR CALLBACK  DlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcPld(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcViewTxt(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcFairing(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcParticle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcMisc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcTex(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcBooster(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcCurve(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcStage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcUllage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcInterstage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcLes(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcAdapter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcFX(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK  DlgProcScenario(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);