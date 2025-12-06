#include <string>
#include <format>
#include "Multistage2015.h"
#include "..//simpleini//SimpleIni.h"
#define STRICT

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

// ==============================================================
//						MultiStage2015
//                  
//					       By Fred18
//                  
//
// Parser.cpp
// ==============================================================


/* #define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE */
//define ORBITER_MODULE

#include <math.h>
#include <stdio.h>
#include <sstream>
#include "..//..//Orbitersdk//include//Orbitersdk.h"
#include "Multistage2015.h"

void Multistage2015::parseInterstages(char filename[MAXLEN], int parsingstage) {
	
	oapiWriteLogV("%s: parseInterstages() filename=%s", GetName(), filename);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }

	int ins = 0;

	std::string intertxt = std::format("SEPARATION_{}{}", parsingstage, parsingstage + 1);
	//oapiWriteLog(intertxt);

	stage.at(parsingstage).IntIncremental = 0;
	
	if(!ini.GetSection(intertxt.c_str())){
		std::string msg = std::format("No more sections after {}", intertxt);
		oapiWriteLog(const_cast<char *>(msg.c_str()));
		return;
	} else {
		stage.at(parsingstage).interstage.meshname = ini.GetValue(intertxt.c_str(), "meshname", "");
		stage.at(parsingstage).wInter = true;
		stage.at(parsingstage).IntIncremental = ins + 1;
		std::string off_vec = ini.GetValue(intertxt.c_str(), "off", "0,0,0");
		stage.at(parsingstage).interstage.off = CharToVec(off_vec);
		stage.at(parsingstage).interstage.height = std::stof(ini.GetValue(intertxt.c_str(), "height", "0.0"));
		stage.at(parsingstage).interstage.diameter = std::stof(ini.GetValue(intertxt.c_str(), "diameter", "0.0"));
		stage.at(parsingstage).interstage.emptymass = std::stof(ini.GetValue(intertxt.c_str(), "emptymass", "0.0"));
		stage.at(parsingstage).interstage.separation_delay = std::stof(ini.GetValue(intertxt.c_str(), "separation_delay", "0.0"));
		std::string speed_vec = ini.GetValue(intertxt.c_str(), "speed", "0.0");
		stage.at(parsingstage).interstage.speed = CharToVec(speed_vec);
		std::string rot_speed = ini.GetValue(intertxt.c_str(), "rot_speed", "0,0,0");
		stage.at(parsingstage).interstage.rot_speed = CharToVec(rot_speed);
		stage.at(parsingstage).interstage.module = ini.GetValue(intertxt.c_str(), "module", "Stage");

		ins += 1;
		nInterstages = ins;
	}

	/* GetPrivateProfileString(intertxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	if (dataparsed[0] != '0') {
		stage[parsingstage].wInter = TRUE;
		stage[parsingstage].IntIncremental = ins + 1;
		sprintf(stage[parsingstage].interstage.meshname, dataparsed);
		GetPrivateProfileString(intertxt, "off", buffreset, dataparsed, MAXLEN, filename);
		//interstage[ins].off=CharToVec(dataparsed);
		GetPrivateProfileString(intertxt, "height", buffreset, dataparsed, MAXLEN, filename);
		stage[parsingstage].interstage.height = atof(dataparsed);
		GetPrivateProfileString(intertxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
		stage[parsingstage].interstage.diameter = atof(dataparsed);
		GetPrivateProfileString(intertxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
		stage[parsingstage].interstage.emptymass = atof(dataparsed);


		GetPrivateProfileString(intertxt, "separation_delay", buffreset, dataparsed, MAXLEN, filename);
		stage[parsingstage].interstage.separation_delay = atof(dataparsed);
		stage[parsingstage].interstage.currDelay = stage[parsingstage].interstage.separation_delay;
		GetPrivateProfileString(intertxt, "speed", buffreset, dataparsed, MAXLEN, filename);
		//interstage[ins].speed=CharToVec(dataparsed);
		CharToVec(dataparsed, &stage[parsingstage].interstage.speed);
		GetPrivateProfileString(intertxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
		//interstage[ins].rot_speed=CharToVec(dataparsed);
		CharToVec(dataparsed, &stage[parsingstage].interstage.rot_speed);
		GetPrivateProfileString(intertxt, "module", buffreset, dataparsed, MAXLEN, filename);
		if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "Stage"); }
		sprintf(stage[parsingstage].interstage.module, dataparsed);
		ins += 1;
		nInterstages = ins; */
		std::string logbuff = std::format("{}: Number of Interstages in the ini file: {}", GetName(), nInterstages);
		oapiWriteLog(const_cast<char *>(logbuff.c_str()));

	return;
}
void Multistage2015::parseLes(char filename[MAXLEN]) {
	
	oapiWriteLogV("%s: parseLes() filename=%s", GetName(), filename);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }

	std::string lestxt = "LES";

	if(!ini.GetSection(lestxt.c_str())){
		std::string msg = std::format("No more sections after {}", lestxt);
		oapiWriteLog(const_cast<char *>(msg.c_str()));
		return;
	} else {
		Les.meshname = ini.GetValue(lestxt.c_str(), "meshname", "");
		wLes = true;
		std::string off_vec = ini.GetValue(lestxt.c_str(), "off", "0,0,0");
		Les.off = CharToVec(off_vec);
		Les.height = std::stof(ini.GetValue(lestxt.c_str(), "height", "0.0"));
		Les.diameter = std::stof(ini.GetValue(lestxt.c_str(), "diameter", "0.0"));
		Les.emptymass = std::stof(ini.GetValue(lestxt.c_str(), "emptymass", "0.0"));
		std::string speed_vec = ini.GetValue(lestxt.c_str(), "speed", "0,0,0");
		Les.speed = CharToVec(speed_vec);
		std::string rotspeed_vec = ini.GetValue(lestxt.c_str(), "rot_speed", "0,0,0");
		Les.rot_speed = CharToVec(rotspeed_vec);
		Les.module = ini.GetValue(lestxt.c_str(), "module", "Stage");

		std::string logbuff = std::format("{}: LES Found", GetName());
		oapiWriteLog(const_cast<char *>(logbuff.c_str()));
		return;
	}

	/* GetPrivateProfileString(lestxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	if (dataparsed[0] != '0') {
		wLes = TRUE;
		sprintf(Les.meshname, dataparsed);
		GetPrivateProfileString(lestxt, "off", buffreset, dataparsed, MAXLEN, filename);
		CharToVec(dataparsed, &Les.off);
		GetPrivateProfileString(lestxt, "height", buffreset, dataparsed, MAXLEN, filename);
		Les.height = atof(dataparsed);
		GetPrivateProfileString(lestxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
		Les.diameter = atof(dataparsed);
		GetPrivateProfileString(lestxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
		Les.emptymass = atof(dataparsed);
		GetPrivateProfileString(lestxt, "speed", buffreset, dataparsed, MAXLEN, filename);
		CharToVec(dataparsed, &Les.speed);
		GetPrivateProfileString(lestxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
		CharToVec(dataparsed, &Les.rot_speed);
		GetPrivateProfileString(lestxt, "module", buffreset, dataparsed, MAXLEN, filename);
		if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "Stage"); }
		sprintf(Les.module, dataparsed);

		sprintf(logbuff, "%s: LES Found", GetName());
		oapiWriteLog(logbuff);
		return;
	}
	else {
		return;
	} */
	return;
}

void Multistage2015::parseAdapter(char filename[MAXLEN]) {

	oapiWriteLogV("%s: parseAdapter() filename=%s", GetName(), filename);

	std::string intertxt = std::format("SEPARATION_{}{}", nStages, nStages + 1);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }

	std::string meshname = ini.GetValue(intertxt.c_str(), "meshname", "");

	if(meshname.empty()){
		intertxt = "ADAPTER";
		meshname = ini.GetValue(intertxt.c_str(), "meshname", "");
	}

	if(meshname.empty()) return;

	wAdapter = true;
	Adapter.meshname = meshname;

	Adapter.off = CharToVec(ini.GetValue(intertxt.c_str(), "off", "0,0,0"));
    Adapter.height = std::stod(ini.GetValue(intertxt.c_str(), "height", "0"));
    Adapter.diameter = std::stod(ini.GetValue(intertxt.c_str(), "diameter", "0"));
    Adapter.emptymass = std::stod(ini.GetValue(intertxt.c_str(), "emptymass", "0"));

    oapiWriteLogV("%s: Adapter parsed from section [%s]", GetName(), intertxt.c_str());
	
}

void Multistage2015::parseStages(char filename[MAXLEN]) {

	oapiWriteLogV("%s: parseInterstages() filename=%s", GetName(), filename);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }

	std::string stagetxt;
	std::string engtxt;

	int i;
	for (i = 0; i <= 10; i++) {

		//check for interstage!
		parseInterstages(filename, i);

		////////STAGE SECTION//////////////////////////////////////////////////////////////

		stagetxt = std::format("STAGE_{}", i + 1);

		//GetPrivateProfileString(stagetxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(stage[i].meshname, dataparsed);
		stage.at(i).meshname = ini.GetValue(stagetxt.c_str(), "meshname", "");
		//GetPrivateProfileString(stagetxt, "off", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].off=CharToVec(dataparsed);
		//CharToVec(dataparsed, &stage[i].off);
		std::string off_vec = ini.GetValue(stagetxt.c_str(), "off", "0,0,0");
		stage.at(i).off = CharToVec(off_vec);
		//GetPrivateProfileString(stagetxt, "height", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].height = atof(dataparsed);
		stage.at(i).height = std::stof(ini.GetValue(stagetxt.c_str(), "height", "0.0"));
		//GetPrivateProfileString(stagetxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].diameter = atof(dataparsed);
		stage.at(i).diameter = std::stof(ini.GetValue(stagetxt.c_str(), "diameter", "0.0"));
		//GetPrivateProfileString(stagetxt, "thrust", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].thrust = atof(dataparsed);
		stage.at(i).thrust = std::stof(ini.GetValue(stagetxt.c_str(), "thrust", "0.0"));
		//GetPrivateProfileString(stagetxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].emptymass = atof(dataparsed);
		stage.at(i).emptymass = std::stof(ini.GetValue(stagetxt.c_str(), "emptymass", "0.0"));
		//GetPrivateProfileString(stagetxt, "fuelmass", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].fuelmass = atof(dataparsed);
		stage.at(i).fuelmass = std::stof(ini.GetValue(stagetxt.c_str(), "fuelmass", "0.0"));
		//GetPrivateProfileString(stagetxt, "burntime", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].burntime = atof(dataparsed);
		stage.at(i).burntime = std::stof(ini.GetValue(stagetxt.c_str(), "burntime", "0.0"));
		//GetPrivateProfileString(stagetxt, "ignite_delay", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ignite_delay = atof(dataparsed);
		//stage[i].currDelay = stage[i].ignite_delay;
		stage.at(i).ignite_delay = std::stof(ini.GetValue(stagetxt.c_str(), "ignite_delay", "0.0"));
		stage.at(i).currDelay = std::stof(ini.GetValue(stagetxt.c_str(), "ignite_delay", "0.0"));
		//GetPrivateProfileString(stagetxt, "speed", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].speed=CharToVec(dataparsed);
		//CharToVec(dataparsed, &stage[i].speed);
		std::string speed_vec = ini.GetValue(stagetxt.c_str(), "speed", "0,0,0");
		stage.at(i).speed = CharToVec(speed_vec);
		//GetPrivateProfileString(stagetxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].rot_speed=CharToVec(dataparsed);
		//CharToVec(dataparsed, &stage[i].rot_speed);
		std::string rotspeed_vec = ini.GetValue(stagetxt.c_str(), "rot_speed", "0,0,0");
		stage.at(i).rot_speed = CharToVec(rotspeed_vec);
		//GetPrivateProfileString(stagetxt, "module", buffreset, dataparsed, MAXLEN, filename);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "Stage"); }
		//sprintf(stage[i].module, dataparsed);
		stage.at(i).module = ini.GetValue(stagetxt.c_str(), "module", "Stage");

		//added by rcraig42 to get values for ispref and pref----------------------------------------------------------------	

		//GetPrivateProfileString(stagetxt, "isp_sl", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ispref = atof(dataparsed);
		stage.at(i).ispref = std::stof(ini.GetValue(stagetxt.c_str(), "isp_sl", "0.0"));
		//GetPrivateProfileString(stagetxt, "pressure_sl", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].pref = atof(dataparsed);
		stage.at(i).pref = std::stof(ini.GetValue(stagetxt.c_str(), "pressure_sl", "0.0"));

		//--------------------------------------------------------------------------------------------------------------	



		//GetPrivateProfileString(stagetxt, "pitchthrust", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].pitchthrust = 2 * atof(dataparsed);
		stage.at(i).pitchthrust = 2 * std::stof(ini.GetValue(stagetxt.c_str(), "pitchthrust", "0.0"));
		//if (stage[i].pitchthrust == 0) { stage[i].defpitch = TRUE; }
		if(stage.at(i).pitchthrust == 0) {stage.at(i).defpitch = true;}
		//GetPrivateProfileString(stagetxt, "yawthrust", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].yawthrust = 2 * atof(dataparsed);
		//if (stage[i].yawthrust == 0) { stage[i].defyaw = TRUE; }
		stage.at(i).yawthrust = 2 * std::stof(ini.GetValue(stagetxt.c_str(), "yawthrust", "0.0"));
		if(stage.at(i).yawthrust == 0) {stage.at(i).defyaw = true;}
		//GetPrivateProfileString(stagetxt, "rollthrust", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].rollthrust = 2 * atof(dataparsed);
		//if (stage[i].rollthrust == 0) { stage[i].defroll = TRUE; }
		stage.at(i).rollthrust = 2 * std::stof(ini.GetValue(stagetxt.c_str(), "rollthrust", "0.0"));
		if(stage.at(i).rollthrust == 0) {stage.at(i).defroll = true;}
		//GetPrivateProfileString(stagetxt, "linearthrust", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].linearthrust = atof(dataparsed);
		stage.at(i).linearthrust = std::stof(ini.GetValue(stagetxt.c_str(), "linearthrust", "0.0"));
		//GetPrivateProfileString(stagetxt, "linear_isp", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].linearisp = atof(dataparsed);
		stage.at(i).linearisp = std::stof(ini.GetValue(stagetxt.c_str(), "linearisp", "0.0"));

		//engines///
		int neng;
		for (neng = 0; neng < 32; neng++) {
			std::string engtxt = std::format("ENG_{}", neng + 1);

			std::string value = ini.GetValue(stagetxt.c_str(), engtxt.c_str(), "");

			if(value.empty()){
				break;
			}

			//GetPrivateProfileString(stagetxt, engtxt, buffreset, dataparsed, MAXLEN, filename);


			//CharToVec(dataparsed,&stage[i].eng[neng]);
			//CharToVec4(dataparsed, &stage[i].engV4[neng]);
			stage.at(i).engV4.at(neng) = CharToVec4(value);

			stage.at(i).eng.at(neng).x = stage.at(i).engV4.at(neng).x;
			stage.at(i).eng.at(neng).y = stage.at(i).engV4.at(neng).y;
			stage.at(i).eng.at(neng).z = stage.at(i).engV4.at(neng).z;
			if ((stage.at(i).engV4.at(neng).t <= 0) || (stage.at(i).engV4.at(neng).t > 10)) { stage.at(i).engV4.at(neng).t = 1; }

			stage.at(i).nEngines = neng + 1;

		}

		stage.at(i).eng_diameter = std::stof(ini.GetValue(stagetxt.c_str(), "eng_diameter", "0"));
		if(stage.at(i).eng_diameter == 0){
			stage.at(i).eng_diameter = 0.5 * stage.at(i).diameter;
		}

		//GetPrivateProfileString(stagetxt, "eng_diameter", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].eng_diameter = atof(dataparsed);
		//if (stage[i].eng_diameter == 0) { stage[i].eng_diameter = 0.5 * stage[i].diameter; }

		//GetPrivateProfileString(stagetxt, "eng_dir", buffreset, dataparsed, MAXLEN, filename);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { stage[i].eng_dir = _V(0, 0, 1); }
		//else { CharToVec(dataparsed, &stage[i].eng_dir); }

		std::string engdir_vec = ini.GetValue(stagetxt.c_str(), "eng_dir", "0,0,0");
		if(engdir_vec.empty()){
			stage.at(i).eng_dir = _V(0, 0, 1);
		} else {
			stage.at(i).eng_dir = CharToVec(engdir_vec);
		}

		//GetPrivateProfileString(stagetxt, "eng_tex", buffreset, dataparsed, MAXLEN, filename);
		//strcpy(stage[i].eng_tex, dataparsed);

		stage.at(i).eng_tex = ini.GetValue(stagetxt.c_str(), "eng_tex", "");

		//GetPrivateProfileString(stagetxt, "eng_pstream1", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(stage[i].eng_pstream1, dataparsed);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { stage[i].wps1 = FALSE; }
		//else { stage[i].wps1 = TRUE; }

		stage.at(i).eng_pstream1 = ini.GetValue(stagetxt.c_str(), "eng_pstream1", "");
		if(stage.at(i).eng_pstream1.empty()){
			stage.at(i).wps1 = false;
		} else {
			stage.at(i).wps1 = true;
		}

		//GetPrivateProfileString(stagetxt, "eng_pstream2", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(stage[i].eng_pstream2, dataparsed);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { stage[i].wps2 = FALSE; }
		//else { stage[i].wps2 = TRUE; }

		stage.at(i).eng_pstream2 = ini.GetValue(stagetxt.c_str(), "eng_pstream2", "");
		if(stage.at(i).eng_pstream2.empty()){
			stage.at(i).wps2 = false;
		} else {
			stage.at(i).wps2 = true;
		}

		//GetPrivateProfileString(stagetxt, "particles_packed_to_engine", buffreset, dataparsed, MAXLEN, filename);
		stage[i].ParticlesPackedToEngine = atoi(dataparsed);
		//if (stage[i].ParticlesPackedToEngine != 0) {
			//stage[i].ParticlesPacked = TRUE;

			//sprintf(logbuff, "%s: Particles Packed to Engine %i", GetName(), abs(stage[i].ParticlesPackedToEngine));
			//oapiWriteLog(logbuff);
		//}
		//else { stage[i].ParticlesPacked = FALSE; }

		stage.at(i).ParticlesPackedToEngine = std::stoi(ini.GetValue(stagetxt.c_str(), "particles_packed_to_engine", "0"));
		if(stage.at(i).ParticlesPackedToEngine != 0){
			stage.at(1).ParticlesPacked = true;

			oapiWriteLogV("%s: Particles Packed to Engine %i", GetName(), std::abs(stage.at(i).ParticlesPackedToEngine));
		}

		//GetPrivateProfileString(stagetxt, "reignitable", buffreset, dataparsed, MAXLEN, filename);
		int transfer = std::stoi(ini.GetValue(stagetxt.c_str(), "reignitable", "0"));
		if(transfer == 0){
			stage.at(i).reignitable = false;
		} else {
			stage.at(i).reignitable = true;
		}

		/* if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { transfer = 1; }
		else { transfer = atoi(dataparsed); }
		if (transfer == 0) { stage[i].reignitable = FALSE; }
		else { stage[i].reignitable = TRUE; } */

		//GetPrivateProfileString(stagetxt, "boiloff", buffreset, dataparsed, MAXLEN, filename);
		int tboil = std::stoi(ini.GetValue(stagetxt.
		c_str(), "boiloff", "0"));

		if(tboil == 0){
			stage.at(i).wBoiloff = false;
		} else {
			stage.at(i).wBoiloff = true;
		}

		/* if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { tboil = 0; }
		else { tboil = atoi(dataparsed); }
		if (tboil == 0) { stage[i].wBoiloff = FALSE; }
		else { stage[i].wBoiloff = TRUE; } */


		//GetPrivateProfileString(stagetxt, "battery", buffreset, dataparsed, MAXLEN, filename);

		/* if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) {
			stage[i].batteries.MaxCharge = 12 * 3600; stage[i].batteries.wBatts = FALSE; 
			stage[i].batteries.CurrentCharge = stage[i].batteries.MaxCharge;
		}
		else {
			stage[i].batteries.MaxCharge = atof(dataparsed) * 3600;
			stage[i].batteries.wBatts = TRUE;
			stage[i].batteries.CurrentCharge = stage[i].batteries.MaxCharge;
		} */

		int battery = std::stoi(ini.GetValue(stagetxt.c_str(), "battery", "0"));
		if(battery == 0){
			stage.at(i).batteries.MaxCharge = 12 * 3600;
			stage.at(i).batteries.wBatts = false;
			stage.at(i).batteries.CurrentCharge = stage.at(i).batteries.MaxCharge;
		} else {
			stage.at(i).batteries.MaxCharge = 3600 * battery;
			stage.at(i).batteries.wBatts = true;
			stage.at(i).batteries.CurrentCharge = stage.at(i).batteries.MaxCharge;
		}

		//GetPrivateProfileString(stagetxt, "ullage_thrust", buffreset, dataparsed, MAXLEN, filename);
		stage.at(i).ullage.thrust = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_thrust", "0.0"));
		if(stage.at(i).ullage.thrust == 0){
			stage.at(i).ullage.wUllage = false;
		} else {
			stage.at(i).ullage.wUllage = true;
		}
		/* if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { stage[i].ullage.wUllage = FALSE; }
		else { stage[i].ullage.wUllage = TRUE; }
		stage[i].ullage.thrust = atof(dataparsed); */
		
		//GetPrivateProfileString(stagetxt, "ullage_anticipation", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ullage.anticipation = atof(dataparsed);
		stage.at(i).ullage.anticipation = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_anticipation", "0.0"));
		//GetPrivateProfileString(stagetxt, "ullage_overlap", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ullage.overlap = atof(dataparsed);
		stage.at(i).ullage.overlap = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_overlap", "0.0"));
		//GetPrivateProfileString(stagetxt, "ullage_N", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ullage.N = atoi(dataparsed);
		stage.at(i).ullage.N = std::stoi(ini.GetValue(stagetxt.c_str(), "ullage_N", "0.0"));
		//GetPrivateProfileString(stagetxt, "ullage_angle", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ullage.angle = atof(dataparsed);
		stage.at(i).ullage.angle = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_angle", "0.0"));
		//GetPrivateProfileString(stagetxt, "ullage_diameter", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ullage.diameter = atof(dataparsed);
		stage.at(i).ullage.diameter = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_diameter", "0.0"));
		//GetPrivateProfileString(stagetxt, "ullage_length", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ullage.length = atof(dataparsed);
		stage.at(i).ullage.length = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_length", "0.0"));
		//if (stage[i].ullage.length == 0) { stage[i].ullage.length = 10 * stage[i].ullage.diameter; }
		if(stage.at(i).ullage.length == 0){
			stage.at(i).ullage.length = 10 * stage.at(i).ullage.diameter;
		}
		//GetPrivateProfileString(stagetxt, "ullage_dir", buffreset, dataparsed, MAXLEN, filename);
		//CharToVec(dataparsed, &stage[i].ullage.dir);
		std::string ullage_dir = ini.GetValue(stagetxt.c_str(), "ullage_dir", "0.0");
		stage.at(i).ullage.dir = CharToVec(ullage_dir);
		//GetPrivateProfileString(stagetxt, "ullage_pos", buffreset, dataparsed, MAXLEN, filename);
		//CharToVec(dataparsed, &stage[i].ullage.pos);
		std::string ullagepos_dir = ini.GetValue(stagetxt.c_str(), "ullage_pos", "0,0,0");
		stage.at(i).ullage.pos = CharToVec(ullagepos_dir);
		//GetPrivateProfileString(stagetxt, "ullage_tex", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(stage[i].ullage.tex, dataparsed);
		stage.at(i).ullage.tex = ini.GetValue(stagetxt.c_str(), "ullage_tex", "");
		//GetPrivateProfileString(stagetxt, "ullage_rectfactor", buffreset, dataparsed, MAXLEN, filename);
		//stage[i].ullage.rectfactor = atof(dataparsed);
		stage.at(i).ullage.rectfactor = std::stof(ini.GetValue(stagetxt.c_str(), "ullage_rectfactor", "0.0"));
		//if (stage[i].ullage.rectfactor == 0) { stage[i].ullage.rectfactor = 1; }
		if(stage.at(i).ullage.rectfactor == 0){ stage.at(i).ullage.rectfactor = 1;}

		//GetPrivateProfileString(stagetxt, "expbolts_pos", buffreset, dataparsed, MAXLEN, filename);
		std::string expbolts_pos_vec = ini.GetValue(stagetxt.c_str(), "expbolts_pos", "0,0,0");
		if(expbolts_pos_vec.empty()){
			stage.at(i).expbolt.wExpbolt = false;
		} else {
			stage.at(i).expbolt.wExpbolt = true;
			stage.at(i).expbolt.pos = CharToVec(expbolts_pos_vec);
			stage.at(i).expbolt.pstream = ini.GetValue(stagetxt.c_str(), "expbolts_pstream", "");
			stage.at(i).expbolt.dir = _V(0, 0, 1);
			stage.at(i).expbolt.anticipation = std::stof(ini.GetValue(stagetxt.c_str(), "expbolts_anticipation", "0.0"));
			if(stage.at(i).expbolt.anticipation == 0){
				stage.at(i).expbolt.anticipation = 1;
			}
		}
		/* if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { stage[i].expbolt.wExpbolt = FALSE; }
		else {
			stage[i].expbolt.wExpbolt = TRUE;
			CharToVec(dataparsed, &stage[i].expbolt.pos);
			GetPrivateProfileString(stagetxt, "expbolts_pstream", buffreset, dataparsed, MAXLEN, filename);
			sprintf(stage[i].expbolt.pstream, dataparsed);
			stage[i].expbolt.dir = _V(0, 0, 1);
			GetPrivateProfileString(stagetxt, "expbolts_anticipation", buffreset, dataparsed, MAXLEN, filename);
			if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { stage[i].expbolt.anticipation = 1; }
			else { stage[i].expbolt.anticipation = atof(dataparsed); }
		} */
		/* if (stage[i].meshname[0] == '0') {
			nStages = i;
			sprintf(logbuff, "%s: Number of stages in the ini file: %i", GetName(), nStages);
			oapiWriteLog(logbuff);
			break;
		} */
		 if(!ini.GetSection(stagetxt.c_str())){
			nStages = i;
			oapiWriteLogV("%s: Number of stages in the ini file: %i", GetName(), nStages);
			break;
		 }
	}
}


void Multistage2015::parseBoosters(char filename[MAXLEN]) {

	oapiWriteLogV("%s: parseBoosters() filename=%s", GetName(), filename);

	std::string boostertxt;

	std::string engtxt;

	std::string curvetxt;

	nBoosters = 0;

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }

	std::string buff;
	int cc;

	int b;
	for (b = 0; b < 10; b++) {

		////////BOOSTER SECTION//////////////////////////////////////////////////////////////

		boostertxt = std::format("BOOSTER_{}", b + 1);

		if(!ini.GetSection(boostertxt.c_str())){
			std::string msg = std::format("No more booster sections after {}", boostertxt);
			oapiWriteLog(const_cast<char *>(msg.c_str()));
			break;
		}

		//GetPrivateProfileString(boostertxt, "N", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].N = atoi(dataparsed);
		booster.at(b).N = std::stoi(ini.GetValue(boostertxt.c_str(), "N", 0));

		booster.at(b).meshname = ini.GetValue(boostertxt.c_str(), "meshname", "");
		if(booster.at(b).meshname.empty()){
			oapiWriteLogV("Booster %s meshname not defined", boostertxt.c_str());
		}

		//GetPrivateProfileString(boostertxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(booster[b].meshname, dataparsed);

		std::string off_vec = ini.GetValue(boostertxt.c_str(), "off", "(0,0,0.001)");
		
		booster.at(b).off = CharToVec(off_vec);

		//GetPrivateProfileString(boostertxt, "off", buffreset, dataparsed, MAXLEN, filename);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "(0,0,0.001)"); }
		//booster[b].off=CharToVec(dataparsed);
		//CharToVec(dataparsed, &booster[b].off);

		booster.at(b).height = std::stof(ini.GetValue(boostertxt.c_str(), "height", "0.0"));

		//GetPrivateProfileString(boostertxt, "height", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].height = atof(dataparsed);

		booster.at(b).angle = std::stof(ini.GetValue(boostertxt.c_str(), "angle", "0.0"));

		//GetPrivateProfileString(boostertxt, "angle", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].angle = atof(dataparsed);

		booster.at(b).diameter = std::stof(ini.GetValue(boostertxt.c_str(), "diameter", "0.0"));

		//GetPrivateProfileString(boostertxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].diameter = atof(dataparsed);

		booster.at(b).thrust = std::stof(ini.GetValue(boostertxt.c_str(), "thrust", "0.0"));

		//GetPrivateProfileString(boostertxt, "thrust", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].thrust = atof(dataparsed);

		booster.at(b).emptymass = std::stof(ini.GetValue(boostertxt.c_str(), "emptymass", "0.0"));
		
		//GetPrivateProfileString(boostertxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].emptymass = atof(dataparsed);

		booster.at(b).fuelmass = std::stof(ini.GetValue(boostertxt.c_str(), "fuelmass", "0.0"));

		//GetPrivateProfileString(boostertxt, "fuelmass", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].fuelmass = atof(dataparsed);

		booster.at(b).burntime = std::stof(ini.GetValue(boostertxt.c_str(), "burntime", "0.0"));
		
		//GetPrivateProfileString(boostertxt, "burntime", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].burntime = atof(dataparsed);
		
		booster.at(b).burndelay = std::stof(ini.GetValue(boostertxt.c_str(), "burndelay", "0.0"));

		//GetPrivateProfileString(boostertxt, "burndelay", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].burndelay = atof(dataparsed);
		
		booster.at(b).currDelay = booster.at(b).burndelay;

		//booster[b].currDelay = booster[b].burndelay;

		std::string speed_vec = ini.GetValue(boostertxt.c_str(), "speed", "(3,0,0)");

		booster.at(b).speed = CharToVec(speed_vec);
		
		//GetPrivateProfileString(boostertxt, "speed", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].speed=CharToVec(dataparsed);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "(3,0,0)"); }

		//CharToVec(dataparsed, &booster[b].speed);

		std::string rotspeed_vec = ini.GetValue(boostertxt.c_str(), "rot_speed", "(0,-0.1,0)");

		booster.at(b).rot_speed = CharToVec(rotspeed_vec);

		//GetPrivateProfileString(boostertxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
		//booster[b].rot_speed=CharToVec(dataparsed);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "(0,-0.1,0)"); }
		//CharToVec(dataparsed, &booster[b].rot_speed);

		booster.at(b).module = ini.GetValue(boostertxt.c_str(), "module", "Stage");

		//GetPrivateProfileString(boostertxt, "module", buffreset, dataparsed, MAXLEN, filename);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "Stage"); }
		//sprintf(booster[b].module, dataparsed);

		//engines///
		int nbeng;
		for (nbeng = 0; nbeng < 5; nbeng++) {
			std::string key = std::format("ENG_{}", nbeng + 1);

			std::string value = ini.GetValue(boostertxt.c_str(), key.c_str(), "");

			if(value.empty()){
				booster.at(b).nEngines = nbeng;
				break;
			}

			booster[b].eng[nbeng] = CharToVec(value);
    		booster[b].nEngines = nbeng + 1;
		}

		booster.at(b).eng_diameter = std::stof(ini.GetValue(boostertxt.c_str(), "eng_diameter", "0.5"));

		//GetPrivateProfileString(boostertxt, "eng_diameter", buffreset, dataparsed, MAXLEN, filename);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "%.3f", booster[b].diameter * 0.5); }
		//booster[b].eng_diameter = atof(dataparsed);

		booster.at(b).eng_tex = ini.GetValue(boostertxt.c_str(), "eng_tex", "");
		
		if(booster.at(b).eng_tex.empty()){
			oapiWriteLogV("Booster %s eng_tex is not defined", boostertxt.c_str());
		}


		//GetPrivateProfileString(boostertxt, "eng_tex", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(booster[b].eng_tex, dataparsed);

		std::string dir_vec = ini.GetValue(boostertxt.c_str(), "eng_dir", "(0, 0, 1)");

		booster.at(b).eng_dir = CharToVec(dir_vec);

		//GetPrivateProfileString(boostertxt, "eng_dir", buffreset, dataparsed, MAXLEN, filename);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { booster[b].eng_dir = _V(0, 0, 1); }
		//else { CharToVec(dataparsed, &booster[b].eng_dir); }

		booster.at(b).eng_pstream1 = ini.GetValue(boostertxt.c_str(), "eng_pstream1", "");
		if(booster.at(b).eng_pstream1.empty()){
			oapiWriteLogV("Booster %s eng_pstream1 not defined", boostertxt.c_str());
			booster.at(b).wps1 = false;
		} else {
			booster.at(b).wps1 = true;
		}


		//GetPrivateProfileString(boostertxt, "eng_pstream1", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(booster[b].eng_pstream1, dataparsed);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { booster[b].wps1 = FALSE; }
		//else { booster[b].wps1 = TRUE; }

		booster.at(b).eng_pstream2 = ini.GetValue(boostertxt.c_str(), "eng_pstream2", "");
		if(booster.at(b).eng_pstream2.empty()){
			oapiWriteLogV("Booster %s eng_pstream2 not defined", boostertxt.c_str());
			booster.at(b).wps2 = false;
		} else {
			booster.at(b).wps2 = true;
		}

		//GetPrivateProfileString(boostertxt, "eng_pstream2", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(booster[b].eng_pstream2, dataparsed);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { booster[b].wps2 = FALSE; }
		//else { booster[b].wps2 = TRUE; }

		

		for (cc = 0; cc < 10; cc++) {
			//sprintf(curvetxt, "CURVE_%i", cc + 1);
			curvetxt = std::format("CURVE_{}", cc + 1);
			std::string curve_vec = ini.GetValue(boostertxt.c_str(), curvetxt.c_str(), "(9000000,100,0)");
			//GetPrivateProfileString(boostertxt, curvetxt, buffreset, dataparsed, MAXLEN, filename);
			//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "(9000000,100,0)"); }
			booster.at(b).curve.at(cc) = CharToVec(curve_vec);

			booster.at(b).curve.at(cc).z = 0;

			//if(booster[b].curve[c].y==0){
				//booster[b].curve[c].y=100;
			//}
			//sprintf(logbuff,"curve for booster %i loaded, time:%.1f power:%.1f",b,booster[b].curve[cc].x,booster[b].curve[cc].y);
			//oapiWriteLog(logbuff);
		}

		//GetPrivateProfileString(boostertxt, "expbolts_pos", buffreset, dataparsed, MAXLEN, filename);
		std::string expbolts_pos_vec = ini.GetValue(boostertxt.c_str(), "expbolts_pos", "0,0,0");
		if(expbolts_pos_vec.empty()){
			booster.at(b).expbolt.wExpbolt = false;
		} else {
			booster.at(b).expbolt.wExpbolt = true;
			booster.at(b).expbolt.pos = CharToVec(expbolts_pos_vec);
			booster.at(b).expbolt.pstream = ini.GetValue(boostertxt.c_str(), "expbolts_pstream", "");
			booster.at(b).expbolt.dir = _V(0, 0, 1);
			std::string expbolts_anticipation = ini.GetValue(boostertxt.c_str(), "expbolts_anticipation", "0.0");
			if(expbolts_anticipation.empty()){
				booster.at(b).expbolt.anticipation = 1;
			} else {
				booster.at(b).expbolt.anticipation = std::stof(expbolts_anticipation);
			}
		}

		/* if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { booster[b].expbolt.wExpbolt = FALSE; }
		else {
			booster[b].expbolt.wExpbolt = TRUE;
			CharToVec(dataparsed, &booster[b].expbolt.pos);
			GetPrivateProfileString(boostertxt, "expbolts_pstream", buffreset, dataparsed, MAXLEN, filename);
			sprintf(booster[b].expbolt.pstream, dataparsed);
			booster[b].expbolt.dir = _V(0, 0, 1);
			GetPrivateProfileString(boostertxt, "expbolts_anticipation", buffreset, dataparsed, MAXLEN, filename);
			if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { booster[b].expbolt.anticipation = 1; }
			else { booster[b].expbolt.anticipation = atof(dataparsed); }
		} */


		if (booster[b].meshname[0] == '0') {
			nBoosters = b;
			sprintf(logbuff, "%s: Number of boosters group in the ini file: %i", GetName(), nBoosters);
			oapiWriteLog(logbuff);
			break;
		}







	}
}

void Multistage2015::parseFairing(char filename[MAXLEN]) {

	oapiWriteLogV("%s: parseFairing() filename=%s", GetName(), filename);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }

	hasFairing = false;

	std::string fairingtxt;


	//if(wFairing==1){
	fairingtxt = "FAIRING";
	//GetPrivateProfileString(fairingtxt, "N", buffreset, dataparsed, MAXLEN, filename);
	//fairing.N = atoi(dataparsed);
	fairing.N = std::stoi(ini.GetValue(fairingtxt.c_str(), "N", "0"));
	//if (fairing.N != 0) { hasFairing = TRUE; sprintf(logbuff, "%s: This Rocket Has Fairing", GetName()); oapiWriteLog(logbuff); }
	if(fairing.N != 0) {
		hasFairing = true;
		oapiWriteLogV("%s: This Rocket Has Fairing", GetName());
	}
	//GetPrivateProfileString(fairingtxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	//sprintf(fairing.meshname, dataparsed);
	fairing.meshname = ini.GetValue(fairingtxt.c_str(), "meshname");
	//GetPrivateProfileString(fairingtxt, "off", buffreset, dataparsed, MAXLEN, filename);
	//fairing.off=CharToVec(dataparsed);
	//CharToVec(dataparsed, &fairing.off);
	std::string off_vec = ini.GetValue(fairingtxt.c_str(), "off", "0,0,0");
	fairing.off = CharToVec(off_vec);
	//GetPrivateProfileString(fairingtxt, "height", buffreset, dataparsed, MAXLEN, filename);
	//fairing.height = atof(dataparsed);
	fairing.height = std::stof(ini.GetValue(fairingtxt.c_str(), "height", "0.0"));
	//GetPrivateProfileString(fairingtxt, "angle", buffreset, dataparsed, MAXLEN, filename);
	//fairing.angle = atof(dataparsed);
	fairing.angle = std::stof(ini.GetValue(fairingtxt.c_str(), "angle", "0.0"));
	//GetPrivateProfileString(fairingtxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
	//fairing.diameter = atof(dataparsed);
	fairing.diameter = std::stof(ini.GetValue(fairingtxt.c_str(), "diameter", "0.0"));
	//GetPrivateProfileString(fairingtxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
	//fairing.emptymass = atof(dataparsed);
	fairing.emptymass = std::stof(ini.GetValue(fairingtxt.c_str(), "emptymass", "0.0"));
	//GetPrivateProfileString(fairingtxt, "speed", buffreset, dataparsed, MAXLEN, filename);
	//fairing.speed=CharToVec(dataparsed);
	std::string speed_vec = ini.GetValue(fairingtxt.c_str(), "speed", "0,0,0");
	//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "(0,-3,0)"); }
	if(speed_vec.empty()){
		speed_vec = "(0,-3,0)";
	}
	//CharToVec(dataparsed, &fairing.speed);
	fairing.speed = CharToVec(speed_vec);

	//GetPrivateProfileString(fairingtxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
	//fairing.rot_speed=CharToVec(dataparsed);
	//if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"(0,0,0)");}
	std::string rotspeed_vec = ini.GetValue(fairingtxt.c_str(), "rot_speed", "0,0,0");
	//CharToVec(dataparsed, &fairing.rot_speed);
	fairing.rot_speed = CharToVec(rotspeed_vec);

	//GetPrivateProfileString(fairingtxt, "module", buffreset, dataparsed, MAXLEN, filename);
	fairing.module = ini.GetValue(fairingtxt.c_str(), "module", "Stage");
	//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "Stage"); }
	//sprintf(fairing.module, dataparsed);


	//}else{

	//sprintf(logbuff,"%s: This Rocket Does Not Have Fairing (at least in this Scenario...)",GetName());
	//oapiWriteLog(logbuff);


	//	}


}

void Multistage2015::ArrangePayloadMeshes(char data[MAXLEN], int pnl) {
	
	string meshnm(data);

	string meshnm0, meshnm1, meshnm2, meshnm3, meshnm4;
	std::size_t findFirstSC = meshnm.find_first_of(";");
	if (findFirstSC != meshnm.npos) {
		meshnm0 = meshnm.substr(0, findFirstSC);
		payload.at(pnl).meshname0 = meshnm0;
		std::size_t findSecondSC = meshnm.find_first_of(";", findFirstSC + 1);
		if (findSecondSC != meshnm.npos) {
			meshnm1 = meshnm.substr(findFirstSC + 1, findSecondSC - findFirstSC - 1);
			std::size_t findThirdSC = meshnm.find_first_of(";", findSecondSC + 1);
			if (findThirdSC != meshnm.npos) {
				meshnm2 = meshnm.substr(findSecondSC + 1, findThirdSC - findSecondSC - 1);
				std::size_t findFourthSC = meshnm.find_first_of(";", findThirdSC + 1);
				if (findFourthSC != meshnm.npos) {
					meshnm3 = meshnm.substr(findThirdSC + 1, findFourthSC - findThirdSC - 1);
					meshnm4 = meshnm.substr(findFourthSC + 1, meshnm.npos);
					payload.at(pnl).nMeshes = 5;
				}
				else { meshnm3 = meshnm.substr(findThirdSC + 1, meshnm.npos); payload.at(pnl).nMeshes = 4; }
			}
			else { meshnm2 = meshnm.substr(findSecondSC + 1, meshnm.npos); payload.at(pnl).nMeshes = 3; }
		}
		else { meshnm1 = meshnm.substr(findFirstSC + 1, meshnm.npos); payload.at(pnl).nMeshes = 2; }
	}
	else { meshnm0 = meshnm.substr(0, meshnm.npos); payload.at(pnl).nMeshes = 1; }

	payload.at(pnl).meshname0 = meshnm0;
	//	oapiWriteLog(payload[pnl].meshname0);
	if (payload.at(pnl).nMeshes == 5) {
		payload.at(pnl).meshname1 = meshnm1;
		payload.at(pnl).meshname2 = meshnm2;
		payload.at(pnl).meshname3 = meshnm3;
		payload.at(pnl).meshname4 = meshnm4;
	}
	else if (payload.at(pnl).nMeshes == 4) {
		payload.at(pnl).meshname1 = meshnm1;
		payload.at(pnl).meshname2 = meshnm2;
		payload.at(pnl).meshname3 = meshnm3;
	}
	else if (payload.at(pnl).nMeshes == 3) {
		payload.at(pnl).meshname1 = meshnm1;
		payload.at(pnl).meshname2 = meshnm2;
	}
	else if (payload.at(pnl).nMeshes == 2) {
		payload.at(pnl).meshname1 = meshnm1;
	}
	//sprintf(logbuff,"0 %s 1 %s 2 %s 3 %s 4 %s",payload[pnl].meshname0,payload[pnl].meshname1,payload[pnl].meshname2,payload[pnl].meshname3,payload[pnl].meshname4);
	//oapiWriteLog(logbuff);
}

void Multistage2015::ArrangePayloadOffsets(const std::string &data, int pnl) {
    std::stringstream ss(data);
    std::string token;
    int idx = 0;

    while (std::getline(ss, token, ';') && idx < payload[pnl].nMeshes) {
        token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
        payload[pnl].off.at(idx++) = CharToVec(token);
    }
}

void Multistage2015::parsePayload(char filename[MAXLEN]) {

	oapiWriteLogV("%s: parsePayload() filename=%s", GetName(), filename);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }
	
	std::string payloadtxt;

	int pnl;

	for (pnl = 0; pnl <= 10; pnl++) {

		payloadtxt = std::format("PAYLOAD_{}", pnl + 1);

		payload.at(pnl).nMeshes = 0;

		if(!ini.GetSection(payloadtxt.c_str())){
			nPayloads = pnl;
			oapiWriteLogV("%s: Number of Payloads in the ini file: %i", GetName(), nPayloads);
			break;
		}

		//GetPrivateProfileString(payloadtxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
		//strcpy(payload[pnl].meshname, dataparsed);
		payload.at(pnl).meshname = ini.GetValue(payloadtxt.c_str(), "meshname", "");

		/* if (payload[pnl].meshname[0] == '0') {
			nPayloads = pnl;
			sprintf(logbuff, "%s: Number of Payloads in the ini file: %i", GetName(), nPayloads);
			oapiWriteLog(logbuff);
			break;
		} */

		std::string meshlist = ini.GetValue(payloadtxt.c_str(), "meshname", "");

		if(!meshlist.empty()){
			ArrangePayloadMeshes(meshlist.data(), pnl);
		} else {
			payload.at(pnl).nMeshes = 0;
		}

		//ArrangePayloadMeshes(dataparsed, pnl);

		//GetPrivateProfileString(payloadtxt, "off", buffreset, dataparsed, MAXLEN, filename);

		ArrangePayloadOffsets(meshlist, pnl);
		//CharToVec(dataparsed,&payload[pnl].off[0]);
		//GetPrivateProfileString(payloadtxt, "height", buffreset, dataparsed, MAXLEN, filename);
		//payload[pnl].height = atof(dataparsed);
		payload.at(pnl).height = std::stof(ini.GetValue(payloadtxt.c_str(), "height", "0.0"));
		//GetPrivateProfileString(payloadtxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
		//payload[pnl].diameter = atof(dataparsed);
		payload.at(pnl).diameter = std::stof(ini.GetValue(payloadtxt.c_str(), "diameter", "0.0"));
		//GetPrivateProfileString(payloadtxt, "mass", buffreset, dataparsed, MAXLEN, filename);
		//payload[pnl].mass = atof(dataparsed);
		payload.at(pnl).mass = std::stof(ini.GetValue(payloadtxt.c_str(), "mass", "0.0"));
		//GetPrivateProfileString(payloadtxt, "module", buffreset, dataparsed, MAXLEN, filename);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "Stage"); }
		//sprintf(payload[pnl].module,dataparsed);
		//strcpy(payload[pnl].module, dataparsed);
		payload.at(pnl).module = ini.GetValue(payloadtxt.c_str(), "module", "Stage");
		//GetPrivateProfileString(payloadtxt, "name", buffreset, dataparsed, MAXLEN, filename);
		//sprintf(payload[pnl].name,dataparsed);
		//strcpy(payload[pnl].name, dataparsed);
		payload.at(pnl).name = ini.GetValue(payloadtxt.c_str(), "name", "");
		//GetPrivateProfileString(payloadtxt, "speed", buffreset, dataparsed, MAXLEN, filename);
		//payload[pnl].speed=CharToVec(dataparsed);
		//CharToVec(dataparsed, &payload[pnl].speed);
		std::string speed_vec = ini.GetValue(payloadtxt.c_str(), "speed", "0,0,0");
		payload.at(pnl).speed = CharToVec(speed_vec);
		//GetPrivateProfileString(payloadtxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
		//payload[pnl].rot_speed=CharToVec(dataparsed);
		//CharToVec(dataparsed, &payload[pnl].rot_speed);
		std::string rotspeed_vec = ini.GetValue(payloadtxt.c_str(), "rot_speed", "0,0,0");
		payload.at(pnl).rot_speed = CharToVec(rotspeed_vec);
		//GetPrivateProfileString(payloadtxt, "rotation", buffreset, dataparsed, MAXLEN, filename);
		std::string rotation_vec = ini.GetValue(payloadtxt.c_str(), "rotation", "0,0,0");
		payload.at(pnl).Rotation = _V(0, 0, 0);
		payload.at(pnl).rotated = false;
		//CharToVec(dataparsed, &payload[pnl].Rotation);
		payload.at(pnl).Rotation = operator*(CharToVec(rotation_vec), RAD);
		//payload[pnl].Rotation = operator*(payload[pnl].Rotation, RAD);
		//if (length(payload[pnl].Rotation) > 0) { payload[pnl].rotated = TRUE; }
		if(length(payload.at(pnl).Rotation) > 0) {payload.at(pnl).rotated = true;}

		//GetPrivateProfileString(payloadtxt, "render", buffreset, dataparsed, MAXLEN, filename);
		//payload[pnl].render = atoi(dataparsed);
		payload.at(pnl).render = std::stoi(ini.GetValue(payloadtxt.c_str(), "render", "0"));
		if (payload.at(pnl).render != 1) {
			payload.at(pnl).render = 0;
		}
		
		//GetPrivateProfileString(payloadtxt, "live", buffreset, dataparsed, MAXLEN, filename);
		int check = std::stoi(ini.GetValue(payloadtxt.c_str(), "live", "0"));
		if (check == 1) {
			payload.at(pnl).live = true;
		} else {
			payload.at(pnl).live = false;
		}

	}


}

void Multistage2015::parseParticle(char filename[MAXLEN]) {
	char partxt[128];
	char bufftxt[128];

	int npart;
	for (npart = 0; npart < 13; npart++) {
		sprintf(partxt, "PARTICLESTREAM");
		sprintf(bufftxt, "_%i", npart + 1);
		strcat(partxt, bufftxt);

		GetPrivateProfileString(partxt, "name", buffreset, dataparsed, MAXLEN, filename);
		if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { nParticles = npart; break; }
		char checktxt[MAXLEN];
		int k;
		for (k = 0; k < MAXLEN; k++) {
			Particle[npart].ParticleName[k] = dataparsed[k];
			checktxt[k] = dataparsed[k];
		}
		GetPrivateProfileString(partxt, "srcsize", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.srcsize = atof(dataparsed);
		GetPrivateProfileString(partxt, "srcrate", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.srcrate = atof(dataparsed);
		GetPrivateProfileString(partxt, "V0", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.v0 = atof(dataparsed);
		GetPrivateProfileString(partxt, "srcspread", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.srcspread = atof(dataparsed);
		GetPrivateProfileString(partxt, "lifetime", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.lifetime = atof(dataparsed);
		GetPrivateProfileString(partxt, "growthrate", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.growthrate = atof(dataparsed);
		GetPrivateProfileString(partxt, "atmslowdown", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.atmslowdown = atof(dataparsed);
		GetPrivateProfileString(partxt, "ltype", buffreset, dataparsed, MAXLEN, filename);
		if (strncmp(dataparsed, "EMISSIVE", 8) == 0) {
			Particle[npart].Pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;
		}
		else if (strncmp(dataparsed, "DIFFUSE", 7) == 0) {
			Particle[npart].Pss.ltype = PARTICLESTREAMSPEC::DIFFUSE;
		}
		else {
			sprintf(logbuff, "%s: WARNING! PARTICLE STREAM LTYPE NOT DEFINED! USING EMISSIVE", GetName()); oapiWriteLog(logbuff);
			Particle[npart].Pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;

		}
		GetPrivateProfileString(partxt, "levelmap", buffreset, dataparsed, MAXLEN, filename);
		if (strncmp(dataparsed, "LVL_LIN", 7) == 0) {
			Particle[npart].Pss.levelmap = PARTICLESTREAMSPEC::LVL_LIN;
		}
		else if (strncmp(dataparsed, "LVL_FLAT", 8) == 0) {
			Particle[npart].Pss.levelmap = PARTICLESTREAMSPEC::LVL_FLAT;
		}
		else if (strncmp(dataparsed, "LVL_SQRT", 8) == 0) {
			Particle[npart].Pss.levelmap = PARTICLESTREAMSPEC::LVL_SQRT;
		}
		else if (strncmp(dataparsed, "LVL_PLIN", 8) == 0) {
			Particle[npart].Pss.levelmap = PARTICLESTREAMSPEC::LVL_PLIN;
		}
		else if (strncmp(dataparsed, "LVL_PSQRT", 9) == 0) {
			Particle[npart].Pss.levelmap = PARTICLESTREAMSPEC::LVL_PSQRT;
		}
		else {
			sprintf(logbuff, "%s: WARNING! PARTICLE STREAM LEVEL MAP NOT DEFINED! USING LVL_LIN", GetName()); oapiWriteLog(logbuff);
			Particle[npart].Pss.levelmap = PARTICLESTREAMSPEC::LVL_LIN;
		}


		GetPrivateProfileString(partxt, "lmin", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.lmin = atof(dataparsed);
		GetPrivateProfileString(partxt, "lmax", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.lmax = atof(dataparsed);
		GetPrivateProfileString(partxt, "atmsmap", buffreset, dataparsed, MAXLEN, filename);
		if (strncmp(dataparsed, "ATM_FLAT", 8) == 0) {
			Particle[npart].Pss.atmsmap = PARTICLESTREAMSPEC::ATM_FLAT;
		}
		else if (strncmp(dataparsed, "ATM_PLIN", 8) == 0) {
			Particle[npart].Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLIN;
		}
		else if (strncmp(dataparsed, "ATM_PLOG", 8) == 0) {
			Particle[npart].Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLOG;
		}
		else {
			sprintf(logbuff, "%s: WARNING PARTICLE ATMOSFERIC MAP NOT DEFINED, USED PLIN", GetName()); oapiWriteLog(logbuff);
			Particle[npart].Pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLIN;
		}
		GetPrivateProfileString(partxt, "amin", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.amin = atof(dataparsed);
		GetPrivateProfileString(partxt, "amax", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.amax = atof(dataparsed);
		GetPrivateProfileString(partxt, "tex", buffreset, dataparsed, MAXLEN, filename);
		if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(dataparsed, "Contrail3"); sprintf(logbuff, "%s: WARINING PARTICLE TEXTURE NOT DEFINED, USED CONTRAIL3", GetName()); oapiWriteLog(logbuff); }
		Particle[npart].Pss.tex = oapiRegisterExhaustTexture(dataparsed);
		sprintf(Particle[npart].TexName, dataparsed);


		GetPrivateProfileString(partxt, "GrowFactor_size", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].GrowFactor_size = atof(dataparsed);
		GetPrivateProfileString(partxt, "GrowFactor_rate", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].GrowFactor_rate = atof(dataparsed);
		//	sprintf(logbuff,"%s growfactor_rate:%.3f",GetName(),Particle[npart].GrowFactor_rate);
		//	oapiWriteLog(logbuff);

		if ((Particle[npart].GrowFactor_rate == 0) && (Particle[npart].GrowFactor_size == 0)) { Particle[npart].Growing = FALSE; }
		else { Particle[npart].Growing = TRUE; }
	}
}

void Multistage2015::parseFXMach(char filename[MAXLEN]){

	oapiWriteLogV("%s: parseFXMach() filename=%s", GetName(), filename);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }

	std::string fxmtxt = "FX_MACH";

	//GetPrivateProfileString(fxmtxt, "pstream", buffreset, dataparsed, MAXLEN, filename);
	//sprintf(FX_Mach.pstream, dataparsed);
	//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { wMach = FALSE; }
	//else { wMach = TRUE; }
	FX_Mach.pstream = ini.GetValue(fxmtxt.c_str(), "pstream", "");
	if(FX_Mach.pstream.empty()){
		wMach = false;
	} else {
		wMach = true;
	}

	
	//GetPrivateProfileString(fxmtxt, "mach_min", buffreset, dataparsed, MAXLEN, filename);
	//FX_Mach.mach_min = atof(dataparsed);
	FX_Mach.mach_min = std::stof(ini.GetValue(fxmtxt.c_str(), "mach_min", "0.0"));

	//GetPrivateProfileString(fxmtxt, "mach_max", buffreset, dataparsed, MAXLEN, filename);
	//FX_Mach.mach_max = atof(dataparsed);
	FX_Mach.mach_max = std::stof(ini.GetValue(fxmtxt.c_str(), "mach_max", "0.0"));

	for (int nmach = 0; nmach < 10; nmach++){
		std::string txtbuff = std::format("off_{}", nmach + 1);
		
		//GetPrivateProfileString(fxmtxt, txtbuff, buffreset, dataparsed, MAXLEN, filename);
		std::string fxmachoff_vec = ini.GetValue(fxmtxt.c_str(), txtbuff.c_str(), "0,0,0");
		//CharToVec(dataparsed, &FX_Mach.off[nmach]);
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { FX_Mach.nmach = nmach; break; }
		FX_Mach.off.at(nmach) = CharToVec(fxmachoff_vec);
		if(fxmachoff_vec.empty()){
			FX_Mach.nmach = nmach;
			break;
		}
	}
	
	//GetPrivateProfileString(fxmtxt, "dir", buffreset, dataparsed, MAXLEN, filename);
	//CharToVec(dataparsed, &FX_Mach.dir);
	std::string fxmachdir_vec = ini.GetValue(fxmtxt.c_str(), "dir", "0,0,0");
	FX_Mach.dir = CharToVec(fxmachdir_vec);
	FX_Mach.added = false;

}
void Multistage2015::parseFXVent(char filename[MAXLEN])
{
	oapiWriteLogV("%s: parseFXVent() filename=%s", GetName(), filename);

	CSimpleIniA ini(true, false, false);

	if (ini.LoadFile(filename) < 0) {
        oapiWriteLogV("%s: Failed to load INI configuration file: %s", GetName(), filename);
        return;
    }

	std::string fxvtxt = "FX_VENT";
	std::string itemtxt;
	std::string numtxt;

	//GetPrivateProfileString(fxvtxt, "pstream", buffreset, dataparsed, MAXLEN, filename);
	//sprintf(FX_Vent.pstream, dataparsed);
	FX_Vent.pstream = ini.GetValue(fxvtxt.c_str(), "pstream", "");
	if(FX_Vent.pstream.empty()){
		wVent = false;
	} else {
		wVent = true;
	}

	//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { wVent = FALSE; }
	//else { wVent = TRUE; }
	FX_Vent.nVent = 0;
	for (int fv = 1; fv <= 10; fv++)
	{
		FX_Vent.added.at(fv) = false;
		//sprintf(numtxt, "_%i", fv);
		//sprintf(itemtxt, "off");
		//strcat(itemtxt, numtxt);
		itemtxt = std::format("off_{}", fv);
		//GetPrivateProfileString(fxvtxt, itemtxt, buffreset, dataparsed, MAXLEN, filename);
		//CharToVec(dataparsed, &FX_Vent.off[fv]);
		std::string off_vec = ini.GetValue(fxvtxt.c_str(), itemtxt.c_str(), "0,0,0");
		FX_Vent.off.at(fv) = CharToVec(off_vec);
		//sprintf(itemtxt, "dir");
		//strcat(itemtxt, numtxt);
		//GetPrivateProfileString(fxvtxt, itemtxt, buffreset, dataparsed, MAXLEN, filename);
		itemtxt = std::format("dir_{}", fv);
		std::string dir_vec = ini.GetValue(fxvtxt.c_str(), itemtxt.c_str(), "0,0,0");
		//if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { FX_Vent.nVent = fv - 1; break; }
		if(dir_vec.empty()){
			FX_Vent.nVent = fv - 1;
			break;
		}
		FX_Vent.dir.at(fv) = CharToVec(dir_vec);
		//CharToVec(dataparsed, &FX_Vent.dir[fv]);
		//sprintf(itemtxt, "time_fin");
		//strcat(itemtxt, numtxt);
		//FX_Vent.time_fin[fv] = 0;
		itemtxt = std::format("time_fin_{}", fv);
		FX_Vent.time_fin.at(fv) = 0;
		//GetPrivateProfileString(fxvtxt, itemtxt, buffreset, dataparsed, MAXLEN, filename);
		//FX_Vent.time_fin[fv] = atof(dataparsed);
		FX_Vent.time_fin.at(fv) = std::stof(ini.GetValue(fxvtxt.c_str(), itemtxt.c_str(), "0.0"));
	}
}

void Multistage2015::parseFXLaunch(char filename[MAXLEN])
{
	char fxLtxt[128];
	sprintf(fxLtxt, "FX_LAUNCH");

	GetPrivateProfileString(fxLtxt, "N", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.N = atoi(dataparsed);
	GetPrivateProfileString(fxLtxt, "Height", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.H = atof(dataparsed);
	GetPrivateProfileString(fxLtxt, "Angle", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.Angle = atof(dataparsed);
	GetPrivateProfileString(fxLtxt, "Distance", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.Distance = atof(dataparsed);
	GetPrivateProfileString(fxLtxt, "CutoffAltitude", buffreset, dataparsed, MAXLEN, filename);
	FX_Launch.CutoffAltitude = atof(dataparsed);
	GetPrivateProfileString(fxLtxt, "pstream1", buffreset, dataparsed, MAXLEN, filename);
	sprintf(FX_Launch.Ps1, dataparsed);
	GetPrivateProfileString(fxLtxt, "pstream2", buffreset, dataparsed, MAXLEN, filename);
	sprintf(FX_Launch.Ps2, dataparsed);

	if (FX_Launch.N >= 1) { wLaunchFX = TRUE; }
}
void Multistage2015::parseTexture(char filename[MAXLEN]) {
	char textxt[128];
	char bufftxt[128];
	sprintf(textxt, "TEXTURE_LIST");

	int texn;
	for (texn = 0; texn < 16; texn++) {
		sprintf(bufftxt, "TEX_%i", texn + 1);

		GetPrivateProfileString(textxt, bufftxt, buffreset, dataparsed, MAXLEN, filename);


		if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { nTextures = texn; break; }
		char checktxt[MAXLEN];
		int k;
		for (k = 0; k < MAXLEN; k++) {
			tex.TextureName[k][texn] = dataparsed[k];
			checktxt[k] = dataparsed[k];
		}
		//sprintf(tex.TextureName[texn],dataparsed);
		sprintf(logbuff, "%s: Texture n.%i Loaded %s", GetName(), texn + 1, checktxt);
		oapiWriteLog(logbuff);
		tex.hTex[texn] = oapiRegisterExhaustTexture(checktxt);
	}
	return;
}
void Multistage2015::parseMisc(char filename[MAXLEN]) {
	char Misctxt[64];
	sprintf(Misctxt, "MISC");

	GetPrivateProfileString(Misctxt, "COG", buffreset, dataparsed, MAXLEN, filename);
	Misc.COG = atof(dataparsed);
	GetPrivateProfileString(Misctxt, "GNC_DEBUG", buffreset, dataparsed, MAXLEN, filename);
	Misc.GNC_Debug = atoi(dataparsed);
	int tval;
	GetPrivateProfileString(Misctxt, "TELEMETRY", buffreset, dataparsed, MAXLEN, filename);
	tval = atoi(dataparsed);
	if (tval == 1) { Misc.telemetry = TRUE; }
	else { Misc.telemetry = FALSE; }
	GetPrivateProfileString(Misctxt, "FOCUS", buffreset, dataparsed, MAXLEN, filename);
	Misc.Focus = atoi(dataparsed);
	int trp;
	GetPrivateProfileString(Misctxt, "THRUST_REAL_POS", buffreset, dataparsed, MAXLEN, filename);
	trp = atoi(dataparsed);
	if (trp == 1) { Misc.thrustrealpos = TRUE; sprintf(logbuff, "%s: Thrust in Real Position", GetName()); oapiWriteLog(logbuff); }
	else { Misc.thrustrealpos = FALSE; }
	Misc.VerticalAngle = 0;
	GetPrivateProfileString(Misctxt, "VERTICAL_ANGLE", buffreset, dataparsed, MAXLEN, filename);
	Misc.VerticalAngle = atof(dataparsed) * RAD;

	//added by rcraig42 to retrieve drag_factor from ini --------------------------------------------------------

	GetPrivateProfileString(Misctxt, "drag_factor", buffreset, dataparsed, MAXLEN, filename);
	Misc.drag_factor = atof(dataparsed);

	//------------------------------------------------------------------------------------------------------------
	GetPrivateProfileString(Misctxt, "PAD_MODULE", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { sprintf(Misc.PadModule, "EmptyModule"); }
	else { sprintf(Misc.PadModule, dataparsed); }


	return;
}

void Multistage2015::parseSound(char filename[MAXLEN]) {
	char Soundtxt[64];
	sprintf(Soundtxt, "SOUND");

	GetPrivateProfileString(Soundtxt, "MAIN_THRUST", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Main = FALSE; }
	else {
		Ssound.Main = TRUE; sprintf(Ssound.MAIN_THRUST, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "HOVER_THRUST", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Hover = FALSE; }
	else {
		Ssound.Hover = TRUE; sprintf(Ssound.HOVER_THRUST, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "RCS_THRUST_ATTACK", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.RCS_ta = FALSE; }
	else {
		Ssound.RCS_ta = TRUE; sprintf(Ssound.RCS_THRUST_ATTACK, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "RCS_THRUST_SUSTAIN", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.RCS_ts = FALSE; }
	else {
		Ssound.RCS_ts = TRUE; sprintf(Ssound.RCS_THRUST_SUSTAIN, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "AIR_CONDITIONING", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ac = FALSE; }
	else {
		Ssound.Ac = TRUE; sprintf(Ssound.AIR_CONDITIONNING, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_1", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca1 = FALSE; }
	else {
		Ssound.Ca1 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_1, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_2", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca2 = FALSE; }
	else {
		Ssound.Ca2 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_2, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_3", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca3 = FALSE; }
	else {
		Ssound.Ca3 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_3, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_4", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca4 = FALSE; }
	else {
		Ssound.Ca4 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_4, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_5", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca5 = FALSE; }
	else {
		Ssound.Ca5 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_5, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_6", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca6 = FALSE; }
	else {
		Ssound.Ca6 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_6, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_7", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca7 = FALSE; }
	else {
		Ssound.Ca7 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_7, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_8", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca8 = FALSE; }
	else {
		Ssound.Ca8 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_8, dataparsed);
	}
	GetPrivateProfileString(Soundtxt, "COCKPIT_AMBIENCE_9", buffreset, dataparsed, MAXLEN, filename);
	if (strncmp(dataparsed, buffreset, MAXLEN - 5) == 0) { Ssound.Ca9 = FALSE; }
	else {
		Ssound.Ca9 = TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_9, dataparsed);
	}


	return;
}
bool Multistage2015::parseinifile(char filename[MAXLEN]) {
	int r;
	for (r = 0; r < MAXLEN; r++) {
		buffreset[r] = '0';
	}
	parseStages(filename);
	parseBoosters(filename);
	//parseInterstages(filename);
	parsePayload(filename);
	parseLes(filename);
	parseFairing(filename);
	parseMisc(filename);
	parseTexture(filename);
	parseParticle(filename);
	parseFXMach(filename);
	parseFXVent(filename);
	parseFXLaunch(filename);
	parseAdapter(filename);
	parseSound(filename);
	return true;
}

void Multistage2015::parseTelemetryFile(char filename[MAXLEN])
{
	char filebuff[MAXLEN];
	sprintf(filebuff, OrbiterRoot);
	strcat(filebuff, "\\");
	strcat(filebuff, filename);
	string line;
	ifstream tlm_file(filebuff);
	//int index=0;
	loadedtlmlines = 0;
	if (tlm_file.is_open())
	{

		while (getline(tlm_file, line))
		{
			if (!line.empty()) {
				const char* cstr = line.c_str();

				//sprintf(buffer,"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",tlmAlt[i].x,tlmAlt[i].y,tlmSpeed[i].y,tlmPitch[i].y,tlmThrust[i].y,tlmMass[i].y,tlmVv[i].y,tlmAcc[i].y);
				sscanf(cstr, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &ReftlmAlt[loadedtlmlines].x, &ReftlmAlt[loadedtlmlines].y, &ReftlmSpeed[loadedtlmlines].y, &ReftlmPitch[loadedtlmlines].y, &ReftlmThrust[loadedtlmlines].y, &ReftlmMass[loadedtlmlines].y, &ReftlmVv[loadedtlmlines].y, &ReftlmAcc[loadedtlmlines].y);
				if (ReftlmAlt[loadedtlmlines].x == 0) { continue; }
				ReftlmSpeed[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmPitch[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmThrust[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmMass[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmVv[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				ReftlmAcc[loadedtlmlines].x = ReftlmAlt[loadedtlmlines].x;
				//sprintf(logbuff,"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",ReftlmAlt[index].x,ReftlmAlt[index].y,ReftlmSpeed[index].y,ReftlmPitch[index].y,ReftlmThrust[index].y,ReftlmMass[index].y,ReftlmVv[index].y,ReftlmAcc[index].y);
				//oapiWriteLog(logbuff);
				loadedtlmlines++;
			}
		}
	}
}

void Multistage2015::parseGuidanceFile(char filename[MAXLEN]) {
	nsteps = 1;


	char filebuff[MAXLEN];
	string line;
	sprintf(filebuff, OrbiterRoot);
	strcat(filebuff, "\\");
	strcat(filebuff, guidancefile);//Config\\multistage\\guidance_shuttle.txt");

	sprintf(logbuff, "%s: Guidance File present: %s", GetName(), guidancefile);
	oapiWriteLog(logbuff);
	ifstream gnc_file(filebuff);
	if (gnc_file.is_open())
	{

		while (getline(gnc_file, line))

		{
			if (!line.empty()) {

				for (int i = 0; i < 6; i++) { Gnc_step[nsteps].wValue[i] = FALSE; }

				std::size_t findEqual = line.find_first_of("=");
				if (findEqual != line.npos) {
					string mettime = line.substr(0, findEqual);

					Gnc_step[nsteps].time = atof(&mettime[0]);

					std::size_t findLineEnd = line.find_first_of(")");
					if (findLineEnd != line.npos) {

						std::size_t findOpenP = line.find_first_of("(");
						if (findOpenP != line.npos) {
							string comand = line.substr(findEqual + 1, findOpenP - findEqual - 1);
							sprintf(Gnc_step[nsteps].Comand, &comand[0]);

							string values = line.substr(findOpenP + 1, findLineEnd - findOpenP - 1);
							Gnc_step[nsteps].wValue[0] = TRUE;
							std::size_t findFirstComma = values.find_first_of(",");
							if (findFirstComma != values.npos) {
								value1 = values.substr(0, findFirstComma);
								Gnc_step[nsteps].wValue[1] = TRUE;


								std::size_t findSecondComma = values.find_first_of(",", findFirstComma + 1);

								if (findSecondComma != values.npos) {

									value2 = values.substr(findFirstComma + 1, findSecondComma - findFirstComma - 1);
									Gnc_step[nsteps].wValue[2] = TRUE;

									std::size_t findThirdComma = values.find_first_of(",", findSecondComma + 1);

									if (findThirdComma != values.npos) {

										value3 = values.substr(findSecondComma + 1, findThirdComma - findSecondComma - 1);
										Gnc_step[nsteps].wValue[3] = TRUE;

										std::size_t findFourthComma = values.find_first_of(",", findThirdComma + 1);
										if (findFourthComma != values.npos) {
											value4 = values.substr(findThirdComma + 1, findFourthComma - findThirdComma - 1);
											Gnc_step[nsteps].wValue[4] = TRUE;
											std::size_t findFifthComma = values.find_first_of(",", findFourthComma + 1);
											if (findFifthComma != values.npos) {
												value5 = values.substr(findFourthComma + 1, findFifthComma - findFourthComma - 1);
												value6 = values.substr(findFifthComma + 1, string::npos);
												Gnc_step[nsteps].wValue[5] = TRUE;
											}
											else { value5 = values.substr(findFourthComma + 1, string::npos); }
										}
										else { value4 = values.substr(findThirdComma + 1, values.npos); }
									}
									else { value3 = values.substr(findSecondComma + 1, values.npos); }
								}
								else { value2 = values.substr(findFirstComma + 1, values.npos); }
							}
							else { value1 = values.substr(0, values.npos); }
						}
					}
					if (Gnc_step[nsteps].wValue[0]) { Gnc_step[nsteps].trval1 = atof(&value1[0]); }
					else { Gnc_step[nsteps].trval1 = 0; }
					if (Gnc_step[nsteps].wValue[1]) { Gnc_step[nsteps].trval2 = atof(&value2[0]); }
					else { Gnc_step[nsteps].trval2 = 0; }
					if (Gnc_step[nsteps].wValue[2]) { Gnc_step[nsteps].trval3 = atof(&value3[0]); }
					else { Gnc_step[nsteps].trval3 = 0; }
					if (Gnc_step[nsteps].wValue[3]) { Gnc_step[nsteps].trval4 = atof(&value4[0]); }
					else { Gnc_step[nsteps].trval4 = 0; }
					if (Gnc_step[nsteps].wValue[4]) { Gnc_step[nsteps].trval5 = atof(&value5[0]); }
					else { Gnc_step[nsteps].trval5 = 0; }
					if (Gnc_step[nsteps].wValue[5]) { Gnc_step[nsteps].trval6 = atof(&value6[0]); }
					else { Gnc_step[nsteps].trval6 = 0; }

				}
				else {
					sprintf(Gnc_step[nsteps].Comand, "noline");
					Gnc_step[nsteps].GNC_Comand = CM_NOLINE;
					Gnc_step[nsteps].time_fin = -10000;
					Gnc_step[nsteps].trval1 = 0;
					Gnc_step[nsteps].trval2 = 0;
					Gnc_step[nsteps].trval3 = 0;
					Gnc_step[nsteps].trval4 = 0;
					Gnc_step[nsteps].trval5 = 0;
					Gnc_step[nsteps].trval6 = 0;
				}

				//	sprintf(logbuff,"step: %i, trval1 %.3f trval2 %.3f trval3 %.3f trval4 %.3f trval5 %.3f",nsteps,Gnc_step[nsteps].trval1,Gnc_step[nsteps].trval2,Gnc_step[nsteps].trval3,Gnc_step[nsteps].trval4,Gnc_step[nsteps].trval5);
					//oapiWriteLog(logbuff);

					//sprintf(logbuff,"%.3f , %.3f , %.3f, %.3f, %.3f",Gnc_step[nsteps].trval1,Gnc_step[nsteps].trval2,Gnc_step[nsteps].trval3,Gnc_step[nsteps].trval4,Gnc_step[nsteps].trval5);
					//oapiWriteLog(logbuff);

				/*	line.copy(transition0,findLineEnd,0);
					sscanf(transition0,"%lf=%[^(](%[^,],%[^,],%[^,],%[^,],%[^,])",&Gnc_step[nsteps].time,&Gnc_step[nsteps].Comand,&transition1,&transition2,&transition3,&transition4,&transition5);

					Gnc_step[nsteps].trval1=atof(transition1);
					Gnc_step[nsteps].trval2=atof(transition2);
					Gnc_step[nsteps].trval3=atof(transition3);
					Gnc_step[nsteps].trval4=atof(transition4);
					Gnc_step[nsteps].trval5=atof(transition5);

					sprintf(logbuff,"step: %i 1 %.3f 2 %.3f 3 %.3f 4 %.3f 5 %.3f",nsteps,Gnc_step[nsteps].trval1,Gnc_step[nsteps].trval2,Gnc_step[nsteps].trval3,Gnc_step[nsteps].trval4,Gnc_step[nsteps].trval5);
					oapiWriteLog(logbuff);*/

				for (int l = 0; l < line.size(); l++) {
					line[l] = tolower(line[l]);
				}

				std::size_t foundDisable = line.find("disable");
				std::size_t foundPlay = line.find("playsound");

				if (foundDisable != std::string::npos) {
					std::size_t foundDisPitch = line.find("pitch");
					std::size_t foundDisRoll = line.find("roll");
					std::size_t foundDisJett = line.find("jettison");

					if (foundDisPitch != std::string::npos) {
						sprintf(Gnc_step[nsteps].Comand, "disablepitch");
					}
					else if (foundDisRoll != std::string::npos) {
						sprintf(Gnc_step[nsteps].Comand, "disableroll");
					}
					else if (foundDisJett != std::string::npos) {
						sprintf(Gnc_step[nsteps].Comand, "disablejettison");
					}

				}
				else if (foundPlay != std::string::npos) {
					std::size_t findopen = line.find_first_of("(");
					std::size_t findclose = line.find_first_of(")");
					string filename = line.substr(findopen + 1, findclose - findopen - 1);
					sprintf(Gnc_step[nsteps].Comand, "playsound");
					filename.copy(Gnc_step[nsteps].trchar, MAXLEN, 0);
				}




				++nsteps;
			}
			else { continue; }


		}

		nsteps -= 1;

		gnc_file.close();
	}

	VinkaComposeGNCSteps();
	VinkaRearrangeSteps();
	nsteps = VinkaCountSteps();
	return;
}
