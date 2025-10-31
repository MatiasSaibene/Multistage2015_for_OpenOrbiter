#include "..//..//Orbitersdk//include//Orbitersdk.h"
#include "Multistage2015.h"
#include "DevModeCtrl.h"	
#include "WriteIni.h"

WriteIni::WriteIni() {

	filename[0] = '\0';

	sectiontxt[0] = '\0';
}
WriteIni::~WriteIni() {}

void WriteIni::SetFilename(char fn[MAXLEN])
{
	strcpy(filename, fn);
}
char* WriteIni::VecToChar(VECTOR3 v)
{
	char out[MAX_PATH];
	sprintf(out, "(%.3f,%.3f,%.3f)", v.x, v.y, v.z);

	return out;
}

/*BOOL WriteIni::WNull()
{
	return W(NULL,NULL);
}*/
BOOL WriteIni::W(LPCSTR b, LPCSTR c)
{
	return WritePrivateProfileString(sectiontxt, b, c, filename);
}

BOOL WriteIni::Wvec(LPCSTR b, VECTOR3 v)
{
	char temp[MAX_PATH];
	sprintf(temp, VecToChar(v));
	return W(b, temp);

}
BOOL WriteIni::Wdouble(LPCSTR b, double c)
{
	char temp[MAX_PATH];
	sprintf(temp, "%.3f", c);
	return W(b, temp);
}
BOOL WriteIni::WLongDouble(LPCSTR b, double c)
{
	char temp[MAX_PATH];
	sprintf(temp, "%lg", c);
	return W(b, temp);
}
BOOL WriteIni::Wint(LPCSTR b, int c)
{
	char temp[MAX_PATH];
	sprintf(temp, "%i", c);
	return W(b, temp);
}
BOOL WriteIni::Wbool_int(LPCSTR b, bool c)
{
	char temp[MAX_PATH];
	int q = 0;
	if (c) { q = 1; }
	sprintf(temp, "%i", q);
	return W(b, temp);
}
void WriteIni::SetCurrentSection(char Section[MAX_PATH])
{
	strcpy(sectiontxt, Section);
}
bool WriteIni::WritePayload(PAYLOAD pld, UINT idx, bool Erase)
{

	char sectionpld[MAX_PATH], idxtxt[56];
	sprintf(sectionpld, "PAYLOAD");
	sprintf(idxtxt, "_%i", idx + 1);
	strcat(sectionpld, idxtxt);

	SetCurrentSection(sectionpld);
	if (!Erase) {
		W("Meshname", pld.meshname);
		//Wvec("Off",pld.off[0],filename);
		W("Off", pld.MultiOffset);
		Wvec("Rotation", pld.Rotation);
		Wdouble("Height", pld.height);
		Wdouble("Diameter", pld.diameter);
		Wdouble("Mass", pld.mass);
		W("Module", pld.module);
		W("Name", pld.name);
		Wvec("Speed", pld.speed);
		Wvec("Rot_speed", pld.rot_speed);
		Wint("Render", pld.render);
		Wbool_int("Live", pld.live);
	}
	else {
		W(NULL, NULL);
	}

	return TRUE;
}

bool WriteIni::WriteFairing(FAIRING frg, bool wFairing)
{
	char sectionFair[MAX_PATH];
	sprintf(sectionFair, "FAIRING");
	SetCurrentSection(sectionFair);
	if (wFairing)
	{
		W("Meshname", frg.meshname);
		Wvec("Off", frg.off);
		Wint("N", frg.N);
		Wdouble("Angle", frg.angle);
		Wdouble("Height", frg.height);
		Wdouble("Diameter", frg.diameter);
		Wdouble("EmptyMass", frg.emptymass);
		W("Module", frg.module);
		Wvec("Speed", frg.speed);
		Wvec("Rot_Speed", frg.rot_speed);
	}
	else {
		W("Meshname", NULL);
		W("Off", NULL);
		W("N", NULL);
		W("Angle", NULL);
		W("Height", NULL);
		W("Diameter", NULL);
		W("EmptyMass", NULL);
		W("Module", NULL);
		W("Speed", NULL);
		W("Rot_Speed", NULL);
	}
	return TRUE;
}

bool WriteIni::WriteParticle(PARTICLE prt, UINT idx, bool Erase)
{
	char sectionPart[MAX_PATH], idxtxt[56];
	sprintf(sectionPart, "PARTICLESTREAM");
	sprintf(idxtxt, "_%i", idx + 1);
	strcat(sectionPart, idxtxt);

	SetCurrentSection(sectionPart);
	if (!Erase)
	{
		W("Name", prt.ParticleName);
		Wdouble("Srcsize", prt.Pss.srcsize);
		Wdouble("Srcrate", prt.Pss.srcrate);
		Wdouble("V0", prt.Pss.v0);
		Wdouble("Srcspread", prt.Pss.srcspread);
		Wdouble("Lifetime", prt.Pss.lifetime);
		Wdouble("Growthrate", prt.Pss.growthrate);
		Wdouble("Atmslowdown", prt.Pss.atmslowdown);
		switch (prt.Pss.ltype)
		{
		case PARTICLESTREAMSPEC::EMISSIVE:
		{
			W("Ltype", TEXT("EMISSIVE"));
			break;
		}
		case PARTICLESTREAMSPEC::DIFFUSE:
		{
			W("Ltype", TEXT("DIFFUSE"));
			break;
		}
		}
		switch (prt.Pss.levelmap)
		{
		case PARTICLESTREAMSPEC::LVL_FLAT:
		{
			W("Levelmap", TEXT("LVL_FLAT"));
			break;
		}
		case PARTICLESTREAMSPEC::LVL_LIN:
		{
			W("Levelmap", TEXT("LVL_LIN"));
			break;
		}
		case PARTICLESTREAMSPEC::LVL_SQRT:
		{
			W("Levelmap", TEXT("LVL_SQRT"));
			break;
		}
		case PARTICLESTREAMSPEC::LVL_PLIN:
		{
			W("Levelmap", TEXT("LVL_PLIN"));
			break;
		}
		case PARTICLESTREAMSPEC::LVL_PSQRT:
		{
			W("Levelmap", TEXT("LVL_PSQRT"));
			break;
		}
		}

		WLongDouble("Lmin", prt.Pss.lmin);
		Wdouble("Lmax", prt.Pss.lmax);

		switch (prt.Pss.atmsmap)
		{
		case PARTICLESTREAMSPEC::ATM_FLAT:
		{
			W("Atmsmap", TEXT("ATM_FLAT"));
			break;
		}
		case PARTICLESTREAMSPEC::ATM_PLIN:
		{
			W("Atmsmap", TEXT("ATM_PLIN"));
			break;
		}
		case PARTICLESTREAMSPEC::ATM_PLOG:
		{
			W("Atmsmap", TEXT("ATM_PLOG"));
			break;
		}
		}

		WLongDouble("Amin", prt.Pss.amin);
		Wdouble("Amax", prt.Pss.amax);
		W("Tex", prt.TexName);

		if (prt.Growing)
		{
			Wdouble("GrowFactor_size", prt.GrowFactor_size);
			Wdouble("GrowFactor_rate", prt.GrowFactor_rate);
		}
		else {
			W("GrowFactor_size", NULL);
			W("GrowFactor_rate", NULL);
		}

	}
	else {
		W(NULL, NULL);
	}
	return TRUE;
}

bool WriteIni::WriteMisc(MISC msc)
{
	char sectionMisc[MAX_PATH];
	sprintf(sectionMisc, "MISC");
	SetCurrentSection(sectionMisc);

	Wdouble("COG", msc.COG);
	Wint("GNC_Debug", msc.GNC_Debug);
	Wbool_int("Telemetry", msc.telemetry);
	Wint("Focus", msc.Focus);
	Wbool_int("Thrust_real_pos", msc.thrustrealpos);
	Wdouble("Vertical_Angle", msc.VerticalAngle * DEG);
	if (strncmp(msc.PadModule, "EmptyModule", 11) == 0)
	{
		W("Pad_Module", NULL);
	}
	else {
		W("Pad_Module", msc.PadModule);
	}
	return TRUE;
}
bool WriteIni::WriteTex(TEX tex, UINT ntextures)
{
	char sectionTex[MAX_PATH];
	sprintf(sectionTex, "TEXTURE_LIST");
	SetCurrentSection(sectionTex);

	char nx[28], trans[MAXLEN];
	for (UINT n = 0; n < ntextures; n++)
	{
		for (UINT j = 0; j < MAXLEN; j++)
		{
			trans[j] = tex.TextureName[j][n];
		}
		sprintf(nx, "TEX_%i", n + 1);
		W(nx, trans);
	}
	for (UINT nrm = ntextures; nrm < 16; nrm++)
	{
		sprintf(nx, "TEX_%i", nrm + 1);
		W(nx, NULL);
	}
	return TRUE;
}
bool WriteIni::WriteBooster(BOOSTER bst, UINT idx, bool Erase)
{
	char sectionbst[MAX_PATH], idxtxt[56];
	sprintf(sectionbst, "BOOSTER");
	sprintf(idxtxt, "_%i", idx + 1);
	strcat(sectionbst, idxtxt);

	SetCurrentSection(sectionbst);

	if (!Erase)
	{
		Wint("N", bst.N);
		Wdouble("Angle", bst.angle * DEG);
		W("Meshname", bst.meshname);
		Wvec("Off", bst.off);
		Wdouble("Height", bst.height);
		Wdouble("Diameter", bst.diameter);
		Wdouble("Thrust", bst.thrust);
		Wdouble("EmptyMass", bst.emptymass);
		Wdouble("FuelMass", bst.fuelmass);
		Wdouble("Burntime", bst.burntime);
		//if(bst.burndelay>0)
		//{
		Wdouble("BurnDelay", bst.burndelay);
		//}
		Wdouble("Eng_diameter", bst.eng_diameter);
		W("Eng_tex", bst.eng_tex);
		W("Eng_pstream1", bst.eng_pstream1);
		W("Eng_pstream2", bst.eng_pstream2);

		if ((bst.eng_dir.x != 0) && (bst.eng_dir.y != 0) && (bst.eng_dir.z != 1))
		{
			Wvec("Eng_dir", bst.eng_dir);
		}
		switch (bst.nEngines)
		{
		case 1:
		{
			Wvec("Eng_1", bst.eng[0]);
			break;
		}
		case 2:
		{
			Wvec("Eng_1", bst.eng[0]);
			Wvec("Eng_2", bst.eng[1]);
			break;
		}
		case 3:
		{
			Wvec("Eng_1", bst.eng[0]);
			Wvec("Eng_2", bst.eng[1]);
			Wvec("Eng_3", bst.eng[2]);
			break;
		}
		case 4:
		{
			Wvec("Eng_1", bst.eng[0]);
			Wvec("Eng_2", bst.eng[1]);
			Wvec("Eng_3", bst.eng[2]);
			Wvec("Eng_4", bst.eng[3]);
			break;
		}
		}

		if (bst.expbolt.wExpbolt)
		{
			Wvec("Expbolts_Pos", bst.expbolt.pos);
			W("Expbolts_Pstream", bst.expbolt.pstream);
			Wdouble("Expbolts_Anticipation", bst.expbolt.anticipation);
		}

		for (UINT i = 0; i < 10; i++)
		{
			if (bst.curve[i].x != 9000000)
			{
				char curvec[256], curvet[128];
				sprintf(curvec, "(%.1f,%.1f)", bst.curve[i].x, bst.curve[i].y);
				sprintf(curvet, "Curve_%i", i + 1);
				W(curvet, curvec);
			}
		}
		/*Wvec("Speed",stg.speed);
		Wvec("Rot_speed",stg.rot_speed);*/
		Wvec("Speed", bst.speed);
		Wvec("Rot_speed", bst.rot_speed);
	}
	else {
		W(NULL, NULL);
	}

	return TRUE;
}
bool WriteIni::WriteStage(STAGE stg, UINT idx, bool Erase)
{
	char sectionstage[MAX_PATH], idxtxt[56];
	sprintf(sectionstage, "STAGE");
	sprintf(idxtxt, "_%i", idx + 1);
	strcat(sectionstage, idxtxt);

	SetCurrentSection(sectionstage);
	if (!Erase)
	{
		W("Meshname", stg.meshname);
		Wvec("Off", stg.off);
		Wdouble("Height", stg.height);
		Wdouble("Diameter", stg.diameter);
		Wdouble("Thrust", stg.thrust);
		Wdouble("EmptyMass", stg.emptymass);
		Wdouble("FuelMass", stg.fuelmass);
		Wdouble("Burntime", stg.burntime);
		for (UINT i = 0; i < stg.nEngines; i++)
		{
			char engbuff[64];
			sprintf(engbuff, "Eng_%i", i + 1);
			Wvec(engbuff, stg.eng[i]);
		}
		for (UINT j = stg.nEngines; j < 32; j++)
		{
			char engbuff[64];
			sprintf(engbuff, "Eng_%i", j + 1);
			W(engbuff, NULL);
		}

		if (stg.ignite_delay > 0)
		{
			Wdouble("Ignite_delay", stg.ignite_delay);
		}
		else {
			W("Ignite_delay", NULL);
		}

		if (stg.batteries.wBatts)
		{
			Wdouble("Battery", stg.batteries.MaxCharge / 3600);
		}
		else {
			W("Battery", NULL);
		}

		if (stg.reignitable)
		{
			int val = 1;
			Wint("Reignitable", val);
		}
		else {
			int val = 0;
			Wint("Reignitable", val);
		}

		if (stg.defpitch)
		{
			W("Pitchthrust", NULL);
		}
		else {
			Wdouble("Pitchthrust", stg.pitchthrust);
		}
		if (stg.defroll)
		{
			W("Rollthrust", NULL);
		}
		else {
			Wdouble("Rollthrust", stg.rollthrust);
		}
		if (stg.defyaw)
		{
			W("Yawthrust", NULL);
		}
		else {
			Wdouble("Yawthrust", stg.yawthrust);
		}
		if (stg.linearthrust > 0)
		{
			Wdouble("Linearthrust", stg.linearthrust);
		}
		else {
			W("Linearthrust", NULL);
		}
		if ((stg.linearisp != 0) && (stg.linearisp != stg.isp * 100))
		{
			Wdouble("Linear_isp", stg.linearisp);
		}
		else {
			W("Linear_isp", NULL);
		}

		Wdouble("Eng_diameter", stg.eng_diameter);
		char buffreset[MAXLEN];
		for (UINT i = 0; i < MAXLEN; i++)
		{
			buffreset[i] = '0';
		}
		if (strncmp(stg.eng_tex, buffreset, MAXLEN - 5) == 0) {
			W("Eng_tex", NULL);
		}
		else {
			W("Eng_tex", stg.eng_tex);
		}
		if (strncmp(stg.eng_pstream1, buffreset, MAXLEN - 5) == 0) {
			W("Eng_pstream1", NULL);
		}
		else {
			W("Eng_pstream1", stg.eng_pstream1);
		}

		if (strncmp(stg.eng_pstream2, buffreset, MAXLEN - 5) == 0) {
			W("Eng_pstream2", NULL);
		}
		else {
			W("Eng_pstream2", stg.eng_pstream2);
		}

		if (stg.ParticlesPacked)
		{
			Wint("Particles_packed_to_engine", stg.ParticlesPackedToEngine);
		}
		Wvec("Eng_dir", stg.eng_dir);

		W("Module", stg.module);
		Wvec("Speed", stg.speed);
		Wvec("Rot_speed", stg.rot_speed);

		if (stg.ullage.wUllage)
		{
			Wdouble("Ullage_Thrust", stg.ullage.thrust);
			Wdouble("Ullage_Anticipation", stg.ullage.anticipation);
			Wdouble("Ullage_Overlap", stg.ullage.overlap);
			Wint("Ullage_N", stg.ullage.N);
			Wdouble("Ullage_Angle", stg.ullage.angle);
			Wdouble("Ullage_Diameter", stg.ullage.diameter);
			Wdouble("Ullage_Length", stg.ullage.length);
			Wvec("Ullage_dir", stg.ullage.dir);
			Wvec("Ullage_pos", stg.ullage.pos);
			Wdouble("Ullage_Rectfactor", stg.ullage.rectfactor);
		}
		else {
			W("Ullage_Thrust", NULL);
			W("Ullage_Anticipation", NULL);
			W("Ullage_Overlap", NULL);
			W("Ullage_N", NULL);
			W("Ullage_Angle", NULL);
			W("Ullage_Diameter", NULL);
			W("Ullage_Length", NULL);
			W("Ullage_dir", NULL);
			W("Ullage_pos", NULL);
			W("Ullage_Rectfactor", NULL);
		}

		if (stg.expbolt.wExpbolt)
		{
			Wvec("Expbolt_pos", stg.expbolt.pos);
			W("Expbolt_pstream", stg.expbolt.pstream);
			Wdouble("Expbolt_anticipation", stg.expbolt.anticipation);
		}
		else {
			W("Expbolt_pos", NULL);
			W("Expbolt_pstream", NULL);
			W("Expbolt_anticipation", NULL);
		}

		//// INTERSTAGE
		char sectioninter[MAX_PATH], idxintertxt[56];
		sprintf(sectioninter, "SEPARATION");
		sprintf(idxintertxt, "_%i%i", idx, idx + 1);
		strcat(sectioninter, idxintertxt);

		SetCurrentSection(sectioninter);

		if (idx > 0) {
			if (stg.wInter)
			{
				W("Meshname", stg.interstage.meshname);
				Wvec("Off", stg.interstage.off);
				Wdouble("Height", stg.interstage.height);
				Wdouble("Diameter", stg.interstage.diameter);
				Wdouble("EmptyMass", stg.interstage.emptymass);
				if (stg.interstage.separation_delay > 0)
				{
					Wdouble("Separation_delay", stg.interstage.separation_delay);
				}
				else {
					W("Separation_delay", NULL);
				}

				W("Module", stg.interstage.module);
				Wvec("Speed", stg.interstage.speed);
				Wvec("Rot_speed", stg.interstage.rot_speed);
			}
			else {

				W("Meshname", NULL);
				W("Off", NULL);
				W("Height", NULL);
				W("Diameter", NULL);
				W("EmptyMass", NULL);
				W("Separation_delay", NULL);
				W("Module", NULL);
				W("Speed", NULL);
				W("Rot_speed", NULL);
			}
		}
	}
	else {
		W(NULL, NULL);
	}

	return TRUE;
}
bool WriteIni::WriteLes(LES les, bool wLes)
{
	char sectionles[MAX_PATH];
	sprintf(sectionles, "LES");

	SetCurrentSection(sectionles);

	if (wLes)
	{
		W("Meshname", les.meshname);
		Wvec("Off", les.off);
		Wdouble("Height", les.height);
		Wdouble("Diameter", les.diameter);
		Wdouble("EmptyMass", les.emptymass);
		W("Module", les.module);
		Wvec("Speed", les.speed);
		Wvec("Rot_speed", les.rot_speed);
	}
	else {
		W("Meshname", NULL);
		W("Off", NULL);
		W("Height", NULL);
		W("Diameter", NULL);
		W("EmptyMass", NULL);
		W("Module", NULL);
		W("Speed", NULL);
		W("Rot_speed", NULL);
	}
	return TRUE;
}
bool WriteIni::WriteAdapter(ADAPTER adp, bool wAdapter)
{
	char sectionadap[MAX_PATH];
	sprintf(sectionadap, "ADAPTER");

	SetCurrentSection(sectionadap);

	if (wAdapter)
	{
		W("Meshname", adp.meshname);
		Wvec("Off", adp.off);
		Wdouble("Height", adp.height);
		Wdouble("Diameter", adp.diameter);
		Wdouble("EmptyMass", adp.emptymass);
	}
	else {
		W("Meshname", NULL);
		W("Off", NULL);
		W("Height", NULL);
		W("Diameter", NULL);
		W("EmptyMass", NULL);
	}
	return TRUE;
}
bool WriteIni::WriteFX_Mach(FX_MACH fxm, bool wMach)
{
	char sectionfxm[MAX_PATH];
	sprintf(sectionfxm, "FX_MACH");

	SetCurrentSection(sectionfxm);
	char buff[28];
	if (wMach)
	{
		W("pstream", fxm.pstream);
		Wdouble("Mach_min", fxm.mach_min);
		Wdouble("Mach_max", fxm.mach_max);
		Wvec("Dir", fxm.dir);
		for (UINT i = 0; i < fxm.nmach; i++)
		{
			sprintf(buff, "Off_%i", i + 1);
			Wvec(buff, fxm.off[i]);
		}
	}
	else {
		W("pstream", NULL);
		W("Mach_min", NULL);
		W("Mach_max", NULL);
		W("Dir", NULL);
		for (UINT i = 0; i < 10; i++)
		{
			sprintf(buff, "Off_%i", i + 1);
			W(buff, NULL);
		}
	}

	return TRUE;
}
bool WriteIni::WriteFX_Vent(FX_VENT fxv, bool wVent)
{
	char sectionvent[MAX_PATH];
	sprintf(sectionvent, "FX_VENT");
	SetCurrentSection(sectionvent);


	char buff[28];
	if (wVent)
	{
		W("pstream", fxv.pstream);
		for (UINT i = 1; i <= fxv.nVent; i++)
		{
			sprintf(buff, "Off_%i", i);
			Wvec(buff, fxv.off[i]);
			sprintf(buff, "Time_fin_%i", i);
			Wdouble(buff, fxv.time_fin[i]);
			sprintf(buff, "Dir_%i", i);
			Wvec(buff, fxv.dir[i]);
		}

	}
	else {
		W("pstream", NULL);
		for (UINT i = 1; i <= 10; i++)
		{
			sprintf(buff, "Off_%i", i);
			W(buff, NULL);
			sprintf(buff, "Time_fin_%i", i);
			W(buff, NULL);
			sprintf(buff, "Dir_%i", i);
			W(buff, NULL);
		}
	}
	return TRUE;
}
bool WriteIni::WriteFX_Launch(FX_LAUNCH fxl, bool wLaunch)
{
	char sectionlaunch[MAX_PATH];
	sprintf(sectionlaunch, "FX_LAUNCH");
	SetCurrentSection(sectionlaunch);

	char buff[128];
	if (wLaunch)
	{
		Wint("N", fxl.N);
		Wdouble("Height", fxl.H);
		Wdouble("Angle", fxl.Angle);
		Wdouble("Distance", fxl.Distance);
		Wdouble("CutoffAltitude", fxl.CutoffAltitude);
		W("Pstream1", fxl.Ps1);
		W("Pstream2", fxl.Ps2);
	}
	else {
		W("N", NULL);
		W("Height", NULL);
		W("Angle", NULL);
		W("Distance", NULL);
		W("CutoffAltitude", NULL);
		W("Pstream1", NULL);
		W("Pstream2", NULL);
	}
	return TRUE;
}