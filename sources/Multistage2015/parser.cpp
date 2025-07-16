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


#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
//#define ORBITER_MODULE

#include <math.h>
#include <stdio.h>
#include "..//..//Orbitersdk//include//Orbitersdk.h"
#include "Multistage2015.h"



 void Multistage2015::parseInterstages(char filename[MAXLEN], int parsingstage){
	 char intertxt[64];
	 char bufftxt[128];
	int ins=0;

	
	

sprintf(intertxt,"SEPARATION");
sprintf(bufftxt,"_%i%i",parsingstage,parsingstage+1);
	strcat(intertxt,bufftxt);
	//oapiWriteLog(intertxt);
	
	stage[parsingstage].IntIncremental=0;
	GetPrivateProfileString(intertxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	if(dataparsed[0]!='0'){
	stage[parsingstage].wInter=TRUE;
	stage[parsingstage].IntIncremental=ins+1;
	sprintf(stage[parsingstage].interstage.meshname,"%s", dataparsed);
	GetPrivateProfileString(intertxt, "off", buffreset, dataparsed, MAXLEN, filename);
	//interstage[ins].off=CharToVec(dataparsed);
	CharToVec(dataparsed,&stage[parsingstage].interstage.off);
	GetPrivateProfileString(intertxt, "height", buffreset, dataparsed, MAXLEN, filename);
	stage[parsingstage].interstage.height=atof(dataparsed);
	GetPrivateProfileString(intertxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
	stage[parsingstage].interstage.diameter=atof(dataparsed);
	GetPrivateProfileString(intertxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
	stage[parsingstage].interstage.emptymass=atof(dataparsed);
	
	
	GetPrivateProfileString(intertxt, "separation_delay", buffreset, dataparsed, MAXLEN, filename);
	stage[parsingstage].interstage.separation_delay=atof(dataparsed);
	stage[parsingstage].interstage.currDelay=stage[parsingstage].interstage.separation_delay;
	GetPrivateProfileString(intertxt, "speed", buffreset, dataparsed, MAXLEN, filename);
	//interstage[ins].speed=CharToVec(dataparsed);
	CharToVec(dataparsed,&stage[parsingstage].interstage.speed);
	GetPrivateProfileString(intertxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
	//interstage[ins].rot_speed=CharToVec(dataparsed);
	CharToVec(dataparsed,&stage[parsingstage].interstage.rot_speed);
	GetPrivateProfileString(intertxt, "module", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"Stage");}
	sprintf(stage[parsingstage].interstage.module,"%s", dataparsed);
	ins+=1;
	nInterstages=ins;
	sprintf(logbuff,"%s: Number of Interstages in the ini file: %i",GetName(),nInterstages);
	oapiWriteLog(logbuff);
	return;
	}else{
	return;
	}
	return;
}
 void Multistage2015::parseLes(char filename[MAXLEN]){
	 char lestxt[64];
	 sprintf(lestxt,"LES");

	GetPrivateProfileString(lestxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	if(dataparsed[0]!='0'){
		wLes=TRUE;
	sprintf(Les.meshname,"%s", dataparsed);
	GetPrivateProfileString(lestxt, "off", buffreset, dataparsed, MAXLEN, filename);
	CharToVec(dataparsed,&Les.off);
	GetPrivateProfileString(lestxt, "height", buffreset, dataparsed, MAXLEN, filename);
	Les.height=atof(dataparsed);
	GetPrivateProfileString(lestxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
	Les.diameter=atof(dataparsed);
	GetPrivateProfileString(lestxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
	Les.emptymass=atof(dataparsed);
	GetPrivateProfileString(lestxt, "speed", buffreset, dataparsed, MAXLEN, filename);
	CharToVec(dataparsed,&Les.speed);
	GetPrivateProfileString(lestxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
	CharToVec(dataparsed,&Les.rot_speed);
	GetPrivateProfileString(lestxt, "module", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"Stage");}
	sprintf(Les.module,"%s", dataparsed);
	
	sprintf(logbuff,"%s: LES Found",GetName());
	oapiWriteLog(logbuff);
	return;
	}else{
	return;
	}
	return;
}
 void Multistage2015::parseAdapter(char filename[MAXLEN]){
	 char intertxt[64];
	 char bufftxt[128];
	
sprintf(intertxt,"SEPARATION");
sprintf(bufftxt,"_%i%i",nStages,nStages+1);
	strcat(intertxt,bufftxt);
	
	

	GetPrivateProfileString(intertxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(intertxt,"ADAPTER");}
	GetPrivateProfileString(intertxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	if(dataparsed[0]!='0'){
	wAdapter=TRUE;
	sprintf(Adapter.meshname,"%s", dataparsed);
	GetPrivateProfileString(intertxt, "off", buffreset, dataparsed, MAXLEN, filename);
	//interstage[ins].off=CharToVec(dataparsed);
	CharToVec(dataparsed,&Adapter.off);
	GetPrivateProfileString(intertxt, "height", buffreset, dataparsed, MAXLEN, filename);
	Adapter.height=atof(dataparsed);
	GetPrivateProfileString(intertxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
	Adapter.diameter=atof(dataparsed);
	GetPrivateProfileString(intertxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
	Adapter.emptymass=atof(dataparsed);
	

	return;
	}else{
	return;
	}
	return;
}
void Multistage2015::parseStages(char filename[MAXLEN]){
	
	char stagetxt[64];
	char engtxt[64];

	char buff[128];
	
	int i;
	for(i=0;i<=10;i++){
	
		//check for interstage!
	parseInterstages(filename,i);
	
	////////STAGE SECTION//////////////////////////////////////////////////////////////

	sprintf(stagetxt,"STAGE");
	sprintf(buff,"_%i",i+1);
	strcat(stagetxt,buff);
	
	GetPrivateProfileString(stagetxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	sprintf(stage[i].meshname,"%s", dataparsed);
	GetPrivateProfileString(stagetxt, "off", buffreset, dataparsed, MAXLEN, filename);
	//stage[i].off=CharToVec(dataparsed);
	CharToVec(dataparsed,&stage[i].off);
	GetPrivateProfileString(stagetxt, "height", buffreset, dataparsed, MAXLEN, filename);
	stage[i].height=atof(dataparsed);
	GetPrivateProfileString(stagetxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
	stage[i].diameter=atof(dataparsed);
	GetPrivateProfileString(stagetxt, "thrust", buffreset, dataparsed, MAXLEN, filename);
	stage[i].thrust=atof(dataparsed);
	GetPrivateProfileString(stagetxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
	stage[i].emptymass=atof(dataparsed);
	GetPrivateProfileString(stagetxt, "fuelmass", buffreset, dataparsed, MAXLEN, filename);
	stage[i].fuelmass=atof(dataparsed);
	GetPrivateProfileString(stagetxt, "burntime", buffreset, dataparsed, MAXLEN, filename);
	stage[i].burntime=atof(dataparsed);
	GetPrivateProfileString(stagetxt, "ignite_delay", buffreset, dataparsed, MAXLEN, filename);
	stage[i].ignite_delay=atof(dataparsed);
	stage[i].currDelay=stage[i].ignite_delay;
	GetPrivateProfileString(stagetxt, "speed", buffreset, dataparsed, MAXLEN, filename);
	//stage[i].speed=CharToVec(dataparsed);
	CharToVec(dataparsed,&stage[i].speed);
	GetPrivateProfileString(stagetxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
	//stage[i].rot_speed=CharToVec(dataparsed);
	CharToVec(dataparsed,&stage[i].rot_speed);
	GetPrivateProfileString(stagetxt, "module", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"Stage");}
	sprintf(stage[i].module,"%s", dataparsed);

	//added by rcraig42 to get values for ispref and pref----------------------------------------------------------------	
	
	GetPrivateProfileString(stagetxt, "isp_sl", buffreset, dataparsed, MAXLEN, filename);
	stage[i].ispref=atof(dataparsed);
	GetPrivateProfileString(stagetxt, "pressure_sl", buffreset, dataparsed, MAXLEN, filename);
	stage[i].pref=atof(dataparsed);
	
//--------------------------------------------------------------------------------------------------------------	
		


	GetPrivateProfileString(stagetxt, "pitchthrust", buffreset, dataparsed, MAXLEN, filename);
	stage[i].pitchthrust=2*atof(dataparsed);
	if(stage[i].pitchthrust==0){stage[i].defpitch=TRUE;}
	GetPrivateProfileString(stagetxt, "yawthrust", buffreset, dataparsed, MAXLEN, filename);
	stage[i].yawthrust=2*atof(dataparsed);
	if(stage[i].yawthrust==0){stage[i].defyaw=TRUE;}
	GetPrivateProfileString(stagetxt, "rollthrust", buffreset, dataparsed, MAXLEN, filename);
	stage[i].rollthrust=2*atof(dataparsed);
	if(stage[i].rollthrust==0){stage[i].defroll=TRUE;}
	GetPrivateProfileString(stagetxt, "linearthrust",buffreset,dataparsed,MAXLEN,filename);
	stage[i].linearthrust=atof(dataparsed);
	GetPrivateProfileString(stagetxt, "linear_isp",buffreset,dataparsed,MAXLEN,filename);
	stage[i].linearisp=atof(dataparsed);
	
	//engines///
	int neng;
	for(neng=0;neng<32;neng++){
	sprintf(engtxt,"ENG");
	sprintf(buff,"_%i",neng+1);
	strcat(engtxt,buff);
	GetPrivateProfileString(stagetxt, engtxt, buffreset, dataparsed, MAXLEN, filename);
			
	
	//CharToVec(dataparsed,&stage[i].eng[neng]);
	CharToVec4(dataparsed,&stage[i].engV4[neng]);
	stage[i].eng[neng].x=stage[i].engV4[neng].x;
	stage[i].eng[neng].y=stage[i].engV4[neng].y;
	stage[i].eng[neng].z=stage[i].engV4[neng].z;
	if((stage[i].engV4[neng].t<=0)||(stage[i].engV4[neng].t>10)){stage[i].engV4[neng].t=1;}
	
	
	if(dataparsed[0]=='0'){ 
			//if(neng==0){stage[i].NoEngDef=TRUE;}
		
		break;
			}
	stage[i].nEngines=neng+1;
	
	

	}
	GetPrivateProfileString(stagetxt, "eng_diameter", buffreset, dataparsed, MAXLEN, filename);
	stage[i].eng_diameter=atof(dataparsed);
	if(stage[i].eng_diameter==0){stage[i].eng_diameter=0.5*stage[i].diameter;}
	GetPrivateProfileString(stagetxt, "eng_dir", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){stage[i].eng_dir=_V(0,0,1);}else{	CharToVec(dataparsed,&stage[i].eng_dir);}
	GetPrivateProfileString(stagetxt, "eng_tex", buffreset, dataparsed, MAXLEN, filename);
	strcpy(stage[i].eng_tex,dataparsed);
	GetPrivateProfileString(stagetxt, "eng_pstream1", buffreset, dataparsed, MAXLEN, filename);
	sprintf(stage[i].eng_pstream1,"%s", dataparsed);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){stage[i].wps1=FALSE;}else{stage[i].wps1=TRUE;}
	GetPrivateProfileString(stagetxt, "eng_pstream2", buffreset, dataparsed, MAXLEN, filename);
	sprintf(stage[i].eng_pstream2,"%s", dataparsed);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){stage[i].wps2=FALSE;}else{stage[i].wps2=TRUE;}
	GetPrivateProfileString(stagetxt,"particles_packed_to_engine",buffreset,dataparsed,MAXLEN,filename);
	stage[i].ParticlesPackedToEngine=atoi(dataparsed);
	if(stage[i].ParticlesPackedToEngine!=0){
		stage[i].ParticlesPacked=TRUE;
		
	sprintf(logbuff,"%s: Particles Packed to Engine %i",GetName(),abs(stage[i].ParticlesPackedToEngine));
	oapiWriteLog(logbuff);
	}else{stage[i].ParticlesPacked=FALSE;}
	GetPrivateProfileString(stagetxt, "reignitable", buffreset, dataparsed, MAXLEN, filename);
	int transfer;
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){transfer=1;}else{transfer=atoi(dataparsed);}
	if(transfer==0){stage[i].reignitable=FALSE;}else{stage[i].reignitable=TRUE;}

	GetPrivateProfileString(stagetxt, "boiloff", buffreset, dataparsed, MAXLEN, filename);
	int tboil;
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){tboil=0;}else{tboil=atoi(dataparsed);}
	if(tboil==0){stage[i].wBoiloff=FALSE;}else{stage[i].wBoiloff=TRUE;}


	GetPrivateProfileString(stagetxt, "battery", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){
		stage[i].batteries.MaxCharge=12*3600;stage[i].batteries.wBatts=FALSE;stage[i].batteries.CurrentCharge=stage[i].batteries.MaxCharge;
	}else{
		stage[i].batteries.MaxCharge=atof(dataparsed)*3600;stage[i].batteries.wBatts=TRUE;stage[i].batteries.CurrentCharge=stage[i].batteries.MaxCharge;
	}

	GetPrivateProfileString(stagetxt,"ullage_thrust",buffreset,dataparsed,MAXLEN,filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){stage[i].ullage.wUllage=FALSE;}else{stage[i].ullage.wUllage=TRUE;}
	stage[i].ullage.thrust=atof(dataparsed);
	GetPrivateProfileString(stagetxt,"ullage_anticipation",buffreset,dataparsed,MAXLEN,filename);
	stage[i].ullage.anticipation=atof(dataparsed);
	GetPrivateProfileString(stagetxt,"ullage_overlap",buffreset,dataparsed,MAXLEN,filename);
	stage[i].ullage.overlap=atof(dataparsed);
	GetPrivateProfileString(stagetxt,"ullage_N",buffreset,dataparsed,MAXLEN,filename);
	stage[i].ullage.N=atoi(dataparsed);
	GetPrivateProfileString(stagetxt,"ullage_angle",buffreset,dataparsed,MAXLEN,filename);
	stage[i].ullage.angle=atof(dataparsed);
	GetPrivateProfileString(stagetxt,"ullage_diameter",buffreset,dataparsed,MAXLEN,filename);
	stage[i].ullage.diameter=atof(dataparsed);
	GetPrivateProfileString(stagetxt,"ullage_length",buffreset,dataparsed,MAXLEN,filename);
	stage[i].ullage.length=atof(dataparsed);
	if(stage[i].ullage.length==0){stage[i].ullage.length=10*stage[i].ullage.diameter;}
	GetPrivateProfileString(stagetxt,"ullage_dir",buffreset,dataparsed,MAXLEN,filename);
	CharToVec(dataparsed,&stage[i].ullage.dir);
	GetPrivateProfileString(stagetxt,"ullage_pos",buffreset,dataparsed,MAXLEN,filename);
	CharToVec(dataparsed,&stage[i].ullage.pos);
	GetPrivateProfileString(stagetxt,"ullage_tex",buffreset,dataparsed,MAXLEN,filename);
	sprintf(stage[i].ullage.tex,"%s", dataparsed);
	GetPrivateProfileString(stagetxt,"ullage_rectfactor",buffreset,dataparsed,MAXLEN,filename);
	stage[i].ullage.rectfactor=atof(dataparsed);
	if(stage[i].ullage.rectfactor==0){stage[i].ullage.rectfactor=1;}

	GetPrivateProfileString(stagetxt,"expbolts_pos",buffreset,dataparsed,MAXLEN,filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){stage[i].expbolt.wExpbolt=FALSE;}else{stage[i].expbolt.wExpbolt=TRUE;
	CharToVec(dataparsed,&stage[i].expbolt.pos);
	GetPrivateProfileString(stagetxt,"expbolts_pstream",buffreset,dataparsed,MAXLEN,filename);
	sprintf(stage[i].expbolt.pstream,"%s", dataparsed);
	stage[i].expbolt.dir=_V(0,0,1);
	GetPrivateProfileString(stagetxt,"expbolts_anticipation",buffreset,dataparsed,MAXLEN,filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){stage[i].expbolt.anticipation=1;}else{stage[i].expbolt.anticipation=atof(dataparsed);}
	}
	if(stage[i].meshname[0]=='0'){
	nStages=i;
	sprintf(logbuff,"%s: Number of stages in the ini file: %i",GetName(),nStages);
	oapiWriteLog(logbuff);
		break;
	}
	

	}


}
 void Multistage2015::parseBoosters(char filename[MAXLEN]){
	char boostertxt[64];
	char buff[128];
	char engtxt[64];
	char curvetxt[128];
	int cc;

	int b;
	for(b=0;b<10;b++){
	
	////////BOOSTER SECTION//////////////////////////////////////////////////////////////

	sprintf(boostertxt,"BOOSTER");
	sprintf(buff,"_%i",b+1);
	strcat(boostertxt,buff);

	GetPrivateProfileString(boostertxt, "N", buffreset, dataparsed, MAXLEN, filename);
	booster[b].N=atoi(dataparsed);

	GetPrivateProfileString(boostertxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	sprintf(booster[b].meshname,"%s", dataparsed);
	GetPrivateProfileString(boostertxt, "off", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"(0,0,0.001)");}
	//booster[b].off=CharToVec(dataparsed);
	CharToVec(dataparsed,&booster[b].off);
	GetPrivateProfileString(boostertxt, "height", buffreset, dataparsed, MAXLEN, filename);
	booster[b].height=atof(dataparsed);
	GetPrivateProfileString(boostertxt, "angle", buffreset, dataparsed, MAXLEN, filename);
	booster[b].angle=atof(dataparsed);
	GetPrivateProfileString(boostertxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
	booster[b].diameter=atof(dataparsed);
	GetPrivateProfileString(boostertxt, "thrust", buffreset, dataparsed, MAXLEN, filename);
	booster[b].thrust=atof(dataparsed);
	GetPrivateProfileString(boostertxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
	booster[b].emptymass=atof(dataparsed);
	GetPrivateProfileString(boostertxt, "fuelmass", buffreset, dataparsed, MAXLEN, filename);
	booster[b].fuelmass=atof(dataparsed);
	GetPrivateProfileString(boostertxt, "burntime", buffreset, dataparsed, MAXLEN, filename);
	booster[b].burntime=atof(dataparsed);
	GetPrivateProfileString(boostertxt, "burndelay", buffreset, dataparsed, MAXLEN, filename);
	booster[b].burndelay=atof(dataparsed);
	booster[b].currDelay=booster[b].burndelay;
	GetPrivateProfileString(boostertxt, "speed", buffreset, dataparsed, MAXLEN, filename);
	//booster[b].speed=CharToVec(dataparsed);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"(3,0,0)");}

	CharToVec(dataparsed,&booster[b].speed);
	GetPrivateProfileString(boostertxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
	//booster[b].rot_speed=CharToVec(dataparsed);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"(0,-0.1,0)");}
	CharToVec(dataparsed,&booster[b].rot_speed);
	GetPrivateProfileString(boostertxt, "module", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"Stage");}
	sprintf(booster[b].module,"%s", dataparsed);
	
	//engines///
	int nbeng;
	for(nbeng=0;nbeng<=4;nbeng++){
	sprintf(engtxt,"ENG");
	sprintf(buff,"_%i",nbeng+1);
	strcat(engtxt,buff);
	GetPrivateProfileString(boostertxt, engtxt, buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){
	booster[b].nEngines=nbeng;		
	break;
		}else{
	CharToVec(dataparsed,&booster[b].eng[nbeng]);
		}
	}
	GetPrivateProfileString(boostertxt, "eng_diameter", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"%.3f",booster[b].diameter*0.5);}
	booster[b].eng_diameter=atof(dataparsed);
	GetPrivateProfileString(boostertxt, "eng_tex", buffreset, dataparsed, MAXLEN, filename);
	sprintf(booster[b].eng_tex,"%s", dataparsed);
	GetPrivateProfileString(boostertxt, "eng_dir", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){booster[b].eng_dir=_V(0,0,1);}else{	CharToVec(dataparsed,&booster[b].eng_dir);}
	GetPrivateProfileString(boostertxt, "eng_pstream1", buffreset, dataparsed, MAXLEN, filename);
	sprintf(booster[b].eng_pstream1,"%s", dataparsed);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){booster[b].wps1=FALSE;}else{booster[b].wps1=TRUE;}
	GetPrivateProfileString(boostertxt, "eng_pstream2", buffreset, dataparsed, MAXLEN, filename);
	sprintf(booster[b].eng_pstream2,"%s", dataparsed);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){booster[b].wps2=FALSE;}else{booster[b].wps2=TRUE;}

	for(cc=0;cc<10;cc++){
		sprintf(curvetxt,"CURVE_%i",cc+1);
		GetPrivateProfileString(boostertxt,curvetxt,buffreset,dataparsed,MAXLEN,filename);
		if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"(9000000,100,0)");}
		CharToVec(dataparsed,&booster[b].curve[cc]);
		
		booster[b].curve[cc].z=0;
	
		//if(booster[b].curve[c].y==0){
			//booster[b].curve[c].y=100;
		//}
		//sprintf(logbuff,"curve for booster %i loaded, time:%.1f power:%.1f",b,booster[b].curve[cc].x,booster[b].curve[cc].y);
		//oapiWriteLog(logbuff);
	}


	GetPrivateProfileString(boostertxt,"expbolts_pos",buffreset,dataparsed,MAXLEN,filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){booster[b].expbolt.wExpbolt=FALSE;}else{booster[b].expbolt.wExpbolt=TRUE;
	CharToVec(dataparsed,&booster[b].expbolt.pos);
	GetPrivateProfileString(boostertxt,"expbolts_pstream",buffreset,dataparsed,MAXLEN,filename);
	sprintf(booster[b].expbolt.pstream,"%s", dataparsed);
	booster[b].expbolt.dir=_V(0,0,1);
	GetPrivateProfileString(boostertxt,"expbolts_anticipation",buffreset,dataparsed,MAXLEN,filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){booster[b].expbolt.anticipation=1;}else{booster[b].expbolt.anticipation=atof(dataparsed);}
	}

	
	if(booster[b].meshname[0]=='0'){
	nBoosters=b;
	sprintf(logbuff,"%s: Number of boosters group in the ini file: %i",GetName(),nBoosters);
	oapiWriteLog(logbuff);
		break;
	}
	

	

	
	

	}
}

 void Multistage2015::parseFairing(char filename[MAXLEN]){
	 hasFairing=FALSE;
	 
	 char fairingtxt[64];
	 
	 
	 		//if(wFairing==1){
sprintf(fairingtxt,"FAIRING");
GetPrivateProfileString(fairingtxt,"N",buffreset,dataparsed,MAXLEN,filename);
fairing.N=atoi(dataparsed);
if(fairing.N!=0){hasFairing=TRUE;sprintf(logbuff,"%s: This Rocket Has Fairing",GetName()); oapiWriteLog(logbuff);}
GetPrivateProfileString(fairingtxt,"meshname",buffreset,dataparsed,MAXLEN,filename);
sprintf(fairing.meshname,"%s", dataparsed);
GetPrivateProfileString(fairingtxt, "off", buffreset, dataparsed, MAXLEN, filename);
//fairing.off=CharToVec(dataparsed);
CharToVec(dataparsed,&fairing.off);
GetPrivateProfileString(fairingtxt, "height", buffreset, dataparsed, MAXLEN, filename);
fairing.height=atof(dataparsed);
GetPrivateProfileString(fairingtxt, "angle", buffreset, dataparsed, MAXLEN, filename);
fairing.angle=atof(dataparsed);
GetPrivateProfileString(fairingtxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
fairing.diameter=atof(dataparsed);
GetPrivateProfileString(fairingtxt, "emptymass", buffreset, dataparsed, MAXLEN, filename);
fairing.emptymass=atof(dataparsed);
GetPrivateProfileString(fairingtxt, "speed", buffreset, dataparsed, MAXLEN, filename);
//fairing.speed=CharToVec(dataparsed);
if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"(0,-3,0)");}
CharToVec(dataparsed,&fairing.speed);

GetPrivateProfileString(fairingtxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
//fairing.rot_speed=CharToVec(dataparsed);
//if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"(0,0,0)");}

CharToVec(dataparsed,&fairing.rot_speed);

GetPrivateProfileString(fairingtxt, "module", buffreset, dataparsed, MAXLEN, filename);
if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"Stage");}
sprintf(fairing.module,"%s", dataparsed);


//}else{
	
//sprintf(logbuff,"%s: This Rocket Does Not Have Fairing (at least in this Scenario...)",GetName());
//oapiWriteLog(logbuff);


//	}


}

 void Multistage2015::ArrangePayloadMeshes(char data[MAXLEN], int pnl){
	string meshnm(data);
	
	string meshnm0,meshnm1,meshnm2,meshnm3,meshnm4;
	std::size_t findFirstSC=meshnm.find_first_of(";");
		if(findFirstSC!=meshnm.npos){
		meshnm0=meshnm.substr(0,findFirstSC);
		meshnm0.copy(payload[pnl].meshname0,MAXLEN,0);
		std::size_t findSecondSC=meshnm.find_first_of(";",findFirstSC+1);
			if(findSecondSC!=meshnm.npos){
				meshnm1=meshnm.substr(findFirstSC+1,findSecondSC-findFirstSC-1);
				std::size_t findThirdSC=meshnm.find_first_of(";",findSecondSC+1);
				if(findThirdSC!=meshnm.npos){
					meshnm2=meshnm.substr(findSecondSC+1,findThirdSC-findSecondSC-1);
						std::size_t findFourthSC=meshnm.find_first_of(";",findThirdSC+1);
						if(findFourthSC!=meshnm.npos){
							meshnm3=meshnm.substr(findThirdSC+1,findFourthSC-findThirdSC-1);
							meshnm4=meshnm.substr(findFourthSC+1,meshnm.npos);
							payload[pnl].nMeshes=5;
						}else{  meshnm3=meshnm.substr(findThirdSC+1,meshnm.npos);payload[pnl].nMeshes=4;}
				}else{meshnm2=meshnm.substr(findSecondSC+1,meshnm.npos);payload[pnl].nMeshes=3;}
			}else{meshnm1=meshnm.substr(findFirstSC+1,meshnm.npos);payload[pnl].nMeshes=2;}
		}else{meshnm0=meshnm;payload[pnl].nMeshes=1;}
		
		meshnm0.copy(payload[pnl].meshname0,MAXLEN,0);
	//	oapiWriteLog(payload[pnl].meshname0);
		if(payload[pnl].nMeshes==5){
		meshnm1.copy(payload[pnl].meshname1,MAXLEN,0);
		meshnm2.copy(payload[pnl].meshname2,MAXLEN,0);
		meshnm3.copy(payload[pnl].meshname3,MAXLEN,0);
		meshnm4.copy(payload[pnl].meshname4,MAXLEN,0);
		}else if(payload[pnl].nMeshes==4){
		meshnm1.copy(payload[pnl].meshname1,MAXLEN,0);
		meshnm2.copy(payload[pnl].meshname2,MAXLEN,0);
		meshnm3.copy(payload[pnl].meshname3,MAXLEN,0);
		}else if(payload[pnl].nMeshes==3){
		meshnm1.copy(payload[pnl].meshname1,MAXLEN,0);
		meshnm2.copy(payload[pnl].meshname2,MAXLEN,0);
		}else if(payload[pnl].nMeshes==2){
		meshnm1.copy(payload[pnl].meshname1,MAXLEN,0);
		}
		//sprintf(logbuff,"0 %s 1 %s 2 %s 3 %s 4 %s",payload[pnl].meshname0,payload[pnl].meshname1,payload[pnl].meshname2,payload[pnl].meshname3,payload[pnl].meshname4);
		//oapiWriteLog(logbuff);
 }

 void Multistage2015::ArrangePayloadOffsets(char data[MAXLEN], int pnl){
	 string offg(data);
	 string off0,off1,off2,off3,off4;
	 char coff0[MAXLEN],coff1[MAXLEN],coff2[MAXLEN],coff3[MAXLEN],coff4[MAXLEN];
	 if(payload[pnl].nMeshes==1){
		std::size_t FindClosingParenthesis=offg.find_first_of(")");
		 //off0=offg.substr(0,offg.npos);
		 off0=offg.substr(0,FindClosingParenthesis);
		 off0.copy(coff0,MAXLEN,0);
		 CharToVec(coff0,&payload[pnl].off[0]);
	 //}else if(payload[pnl].nMeshes==2){
	 }else if(payload[pnl].nMeshes>1){
		 std::size_t findFirstSC=offg.find_first_of(";");
		 off0=offg.substr(0,findFirstSC);
		 off0.copy(coff0,MAXLEN,0);
		 CharToVec(coff0,&payload[pnl].off[0]);
			
		 if(payload[pnl].nMeshes==2){
		 off1=offg.substr(findFirstSC+1,offg.npos);
		 off1.copy(coff1,MAXLEN,0);
		 CharToVec(coff1,&payload[pnl].off[1]);
			}else{
				std::size_t findSecondSC=offg.find_first_of(";",findFirstSC+1);
				off1=offg.substr(findFirstSC+1,findSecondSC-findFirstSC-1);
				off1.copy(coff1,MAXLEN,0);
				CharToVec(coff1,&payload[pnl].off[1]);
				if(payload[pnl].nMeshes==3){
					off2=offg.substr(findSecondSC+1,offg.npos);
					off2.copy(coff2,MAXLEN,0);
					CharToVec(coff2,&payload[pnl].off[2]);
				}else{
					std::size_t findThirdSC=offg.find_first_of(";",findSecondSC+1);
					off2=offg.substr(findSecondSC+1,findThirdSC-findSecondSC-1);
					off2.copy(coff2,MAXLEN,0);
					CharToVec(coff2,&payload[pnl].off[2]);
					if(payload[pnl].nMeshes==4){
						off3=offg.substr(findThirdSC+1,offg.npos);
						off3.copy(coff3,MAXLEN,0);
						CharToVec(coff3,&payload[pnl].off[3]);
					}else{
						std::size_t findFourthSC=offg.find_first_of(";",findThirdSC+1);
						off3=offg.substr(findThirdSC+1,findFourthSC-findThirdSC-1);
						off3.copy(coff3,MAXLEN,0);
						CharToVec(coff3,&payload[pnl].off[3]);
						off4=offg.substr(findFourthSC+1,offg.npos);
						off4.copy(coff4,MAXLEN,0);
						CharToVec(coff4,&payload[pnl].off[4]);
					}
				}
		 }
	 }
	 
 }
 void Multistage2015::parsePayload(char filename[MAXLEN]){
	 char payloadtxt[64];
	char bufftxt[128];

	 int pnl;
	for(pnl=0;pnl<=10;pnl++){
		
sprintf(payloadtxt,"PAYLOAD");
	sprintf(bufftxt,"_%i",pnl+1);
	strcat(payloadtxt,bufftxt);

	payload[pnl].nMeshes=0;
	GetPrivateProfileString(payloadtxt, "meshname", buffreset, dataparsed, MAXLEN, filename);
	
	strcpy(payload[pnl].meshname,dataparsed);
	
	if(payload[pnl].meshname[0]=='0'){
	nPayloads=pnl;
	sprintf(logbuff,"%s: Number of Payloads in the ini file: %i",GetName(),nPayloads);
	oapiWriteLog(logbuff);
		break;	}
	ArrangePayloadMeshes(dataparsed,pnl);
	GetPrivateProfileString(payloadtxt, "off", buffreset, dataparsed, MAXLEN, filename);
	ArrangePayloadOffsets(dataparsed,pnl);
	//CharToVec(dataparsed,&payload[pnl].off[0]);
	GetPrivateProfileString(payloadtxt, "height", buffreset, dataparsed, MAXLEN, filename);
	payload[pnl].height=atof(dataparsed);
	GetPrivateProfileString(payloadtxt, "diameter", buffreset, dataparsed, MAXLEN, filename);
	payload[pnl].diameter=atof(dataparsed);
	GetPrivateProfileString(payloadtxt, "mass", buffreset, dataparsed, MAXLEN, filename);
	payload[pnl].mass=atof(dataparsed);
	GetPrivateProfileString(payloadtxt, "module", buffreset, dataparsed, MAXLEN, filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"Stage");}
	//sprintf(payload[pnl].module,dataparsed);
	strcpy(payload[pnl].module,dataparsed);
	GetPrivateProfileString(payloadtxt, "name", buffreset, dataparsed, MAXLEN, filename);
	//sprintf(payload[pnl].name,dataparsed);
	strcpy(payload[pnl].name,dataparsed);
	GetPrivateProfileString(payloadtxt, "speed", buffreset, dataparsed, MAXLEN, filename);
	//payload[pnl].speed=CharToVec(dataparsed);
	CharToVec(dataparsed,&payload[pnl].speed);
	GetPrivateProfileString(payloadtxt, "rot_speed", buffreset, dataparsed, MAXLEN, filename);
	//payload[pnl].rot_speed=CharToVec(dataparsed);
	CharToVec(dataparsed,&payload[pnl].rot_speed);
	GetPrivateProfileString(payloadtxt,"rotation",buffreset,dataparsed,MAXLEN,filename);
	payload[pnl].Rotation=_V(0,0,0);
	payload[pnl].rotated=FALSE;
	CharToVec(dataparsed,&payload[pnl].Rotation);
	payload[pnl].Rotation=operator*(payload[pnl].Rotation,RAD);
	if(length(payload[pnl].Rotation)>0){payload[pnl].rotated=TRUE;}

	GetPrivateProfileString(payloadtxt,"render",buffreset,dataparsed,MAXLEN,filename);
	payload[pnl].render=atoi(dataparsed);
	if(payload[pnl].render!=1){
		payload[pnl].render=0;
	}
	GetPrivateProfileString(payloadtxt,"live",buffreset,dataparsed,MAXLEN,filename);
	int check=atoi(dataparsed);
	if(check==1){payload[pnl].live=TRUE;}else{payload[pnl].live=FALSE;}

	}
	

}
 void Multistage2015::parseParticle(char filename[MAXLEN]){
	 char partxt[128];
	 char bufftxt[128];
	
	 int npart;
	 for(npart=0;npart<13;npart++){
		  sprintf(partxt,"PARTICLESTREAM");
		sprintf(bufftxt,"_%i",npart+1);
		strcat(partxt,bufftxt);
	
		GetPrivateProfileString(partxt, "name", buffreset, dataparsed, MAXLEN, filename);
			if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){nParticles=npart;break;}
				char checktxt[MAXLEN];
				int k;
				for(k=0;k<MAXLEN;k++){
				Particle[npart].ParticleName[k]=dataparsed[k];
				checktxt[k]=dataparsed[k];
				}
		GetPrivateProfileString(partxt, "srcsize", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.srcsize=atof(dataparsed);
		GetPrivateProfileString(partxt, "srcrate", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.srcrate=atof(dataparsed);
		GetPrivateProfileString(partxt, "V0", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.v0=atof(dataparsed);
		GetPrivateProfileString(partxt, "srcspread", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.srcspread=atof(dataparsed);
		GetPrivateProfileString(partxt, "lifetime", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.lifetime=atof(dataparsed);
		GetPrivateProfileString(partxt, "growthrate", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.growthrate=atof(dataparsed);
		GetPrivateProfileString(partxt, "atmslowdown", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.atmslowdown=atof(dataparsed);
		GetPrivateProfileString(partxt, "ltype", buffreset, dataparsed, MAXLEN, filename);
		if(strncmp(dataparsed,"EMISSIVE",8)==0){
		Particle[npart].Pss.ltype=PARTICLESTREAMSPEC::EMISSIVE;
		}else if(strncmp(dataparsed,"DIFFUSE",7)==0){
		Particle[npart].Pss.ltype=PARTICLESTREAMSPEC::DIFFUSE;
		}else{sprintf(logbuff,"%s: WARNING! PARTICLE STREAM LTYPE NOT DEFINED! USING EMISSIVE",GetName());oapiWriteLog(logbuff);
		Particle[npart].Pss.ltype=PARTICLESTREAMSPEC::EMISSIVE;

		}
		GetPrivateProfileString(partxt, "levelmap", buffreset, dataparsed, MAXLEN, filename);
		if(strncmp(dataparsed,"LVL_LIN",7)==0){
		Particle[npart].Pss.levelmap=PARTICLESTREAMSPEC::LVL_LIN;
		}else if(strncmp(dataparsed,"LVL_FLAT",8)==0){
		Particle[npart].Pss.levelmap=PARTICLESTREAMSPEC::LVL_FLAT;
		}else if(strncmp(dataparsed,"LVL_SQRT",8)==0){
		Particle[npart].Pss.levelmap=PARTICLESTREAMSPEC::LVL_SQRT;
		}else if(strncmp(dataparsed,"LVL_PLIN",8)==0){
		Particle[npart].Pss.levelmap=PARTICLESTREAMSPEC::LVL_PLIN;
		}else if(strncmp(dataparsed,"LVL_PSQRT",9)==0){
		Particle[npart].Pss.levelmap=PARTICLESTREAMSPEC::LVL_PSQRT;
		}else{sprintf(logbuff,"%s: WARNING! PARTICLE STREAM LEVEL MAP NOT DEFINED! USING LVL_LIN",GetName());oapiWriteLog(logbuff);
		Particle[npart].Pss.levelmap=PARTICLESTREAMSPEC::LVL_LIN;
		}
		
		
		GetPrivateProfileString(partxt, "lmin", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.lmin=atof(dataparsed);
		GetPrivateProfileString(partxt, "lmax", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.lmax=atof(dataparsed);
		GetPrivateProfileString(partxt, "atmsmap", buffreset, dataparsed, MAXLEN, filename);
		if(strncmp(dataparsed,"ATM_FLAT",8)==0){
		Particle[npart].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_FLAT;
		}else if(strncmp(dataparsed,"ATM_PLIN",8)==0){
		Particle[npart].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_PLIN;
		}else if(strncmp(dataparsed,"ATM_PLOG",8)==0){
		Particle[npart].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_PLOG;
		}else{sprintf(logbuff,"%s: WARNING PARTICLE ATMOSFERIC MAP NOT DEFINED, USED PLIN",GetName());oapiWriteLog(logbuff);
		Particle[npart].Pss.atmsmap=PARTICLESTREAMSPEC::ATM_PLIN;
		}
		GetPrivateProfileString(partxt, "amin", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.amin=atof(dataparsed);
		GetPrivateProfileString(partxt, "amax", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].Pss.amax=atof(dataparsed);	 
		GetPrivateProfileString(partxt, "tex", buffreset, dataparsed, MAXLEN, filename);
		if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(dataparsed,"Contrail3");sprintf(logbuff,"%s: WARINING PARTICLE TEXTURE NOT DEFINED, USED CONTRAIL3",GetName());oapiWriteLog(logbuff);}
		Particle[npart].Pss.tex=oapiRegisterExhaustTexture(dataparsed);
		sprintf(Particle[npart].TexName,"%s", dataparsed);


		GetPrivateProfileString(partxt, "GrowFactor_size", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].GrowFactor_size=atof(dataparsed);
		GetPrivateProfileString(partxt, "GrowFactor_rate", buffreset, dataparsed, MAXLEN, filename);
		Particle[npart].GrowFactor_rate=atof(dataparsed);
	//	sprintf(logbuff,"%s growfactor_rate:%.3f",GetName(),Particle[npart].GrowFactor_rate);
	//	oapiWriteLog(logbuff);

		if((Particle[npart].GrowFactor_rate==0)&&(Particle[npart].GrowFactor_size==0)){Particle[npart].Growing=FALSE;}else{Particle[npart].Growing=TRUE;}
	 }
 }
 void Multistage2015::parseFXMach(char filename[MAXLEN])
 {
	 char fxmtxt[128];
	 sprintf(fxmtxt,"FX_MACH");
	 
	 GetPrivateProfileString(fxmtxt, "pstream", buffreset, dataparsed, MAXLEN, filename);
	 sprintf(FX_Mach.pstream,"%s", dataparsed);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){wMach=FALSE;}else{wMach=TRUE;}

	 GetPrivateProfileString(fxmtxt,"mach_min",buffreset,dataparsed,MAXLEN,filename);
	 FX_Mach.mach_min=atof(dataparsed);
	 GetPrivateProfileString(fxmtxt,"mach_max",buffreset,dataparsed,MAXLEN,filename);
	 FX_Mach.mach_max=atof(dataparsed);
	 for(int nmach=0;nmach<10;nmach++)
	 {
		 char txtbuff[MAXLEN],nbuff[15];
		  sprintf(nbuff,"_%i",nmach+1);
		  sprintf(txtbuff,"off");
		  strcat(txtbuff,nbuff);
	 GetPrivateProfileString(fxmtxt,txtbuff,buffreset,dataparsed,MAXLEN,filename);
	 CharToVec(dataparsed,&FX_Mach.off[nmach]);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){FX_Mach.nmach=nmach;break;}
	 }
	 GetPrivateProfileString(fxmtxt,"dir",buffreset,dataparsed,MAXLEN,filename);
	 CharToVec(dataparsed,&FX_Mach.dir);
	 FX_Mach.added=FALSE;

 }
 void Multistage2015::parseFXVent(char filename[MAXLEN])
 {
	 char fxvtxt[128];
	 char itemtxt[128];
	 char numtxt[128];
	 sprintf(fxvtxt,"FX_VENT");
	 
	 GetPrivateProfileString(fxvtxt, "pstream", buffreset, dataparsed, MAXLEN, filename);
	 sprintf(FX_Vent.pstream,"%s", dataparsed);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){wVent=FALSE;}else{wVent=TRUE;}
	 FX_Vent.nVent=0;
	 for(int fv=1;fv<=10;fv++)
	 {
		FX_Vent.added[fv]=FALSE; 
		 sprintf(numtxt,"_%i",fv);
		 sprintf(itemtxt,"off");
		 strcat(itemtxt,numtxt);
		 GetPrivateProfileString(fxvtxt,itemtxt,buffreset,dataparsed,MAXLEN,filename);
		 CharToVec(dataparsed,&FX_Vent.off[fv]);
		 sprintf(itemtxt,"dir");
		 strcat(itemtxt,numtxt);
		 GetPrivateProfileString(fxvtxt,itemtxt,buffreset,dataparsed,MAXLEN,filename);
		 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){FX_Vent.nVent=fv-1;break;}
		 CharToVec(dataparsed,&FX_Vent.dir[fv]);
		 sprintf(itemtxt,"time_fin");
		 strcat(itemtxt,numtxt);
		 FX_Vent.time_fin[fv]=0;
		 GetPrivateProfileString(fxvtxt,itemtxt,buffreset,dataparsed,MAXLEN,filename);
		 FX_Vent.time_fin[fv]=atof(dataparsed);
		
	 }
 }

 void Multistage2015::parseFXLaunch(char filename[MAXLEN])
 {
	 char fxLtxt[128];
	 sprintf(fxLtxt,"FX_LAUNCH");

	 GetPrivateProfileString(fxLtxt,"N",buffreset,dataparsed,MAXLEN,filename);
	 FX_Launch.N=atoi(dataparsed);
	 GetPrivateProfileString(fxLtxt,"Height",buffreset,dataparsed,MAXLEN,filename);
	 FX_Launch.H=atof(dataparsed);
	 GetPrivateProfileString(fxLtxt,"Angle",buffreset,dataparsed,MAXLEN,filename);
	 FX_Launch.Angle=atof(dataparsed);
	 GetPrivateProfileString(fxLtxt,"Distance",buffreset,dataparsed,MAXLEN,filename);
	 FX_Launch.Distance=atof(dataparsed);
	 GetPrivateProfileString(fxLtxt,"CutoffAltitude",buffreset,dataparsed,MAXLEN,filename);
	 FX_Launch.CutoffAltitude=atof(dataparsed);
	 GetPrivateProfileString(fxLtxt,"pstream1",buffreset,dataparsed,MAXLEN,filename);
	 sprintf(FX_Launch.Ps1,"%s", dataparsed);
	 GetPrivateProfileString(fxLtxt,"pstream2",buffreset,dataparsed,MAXLEN,filename);
	 sprintf(FX_Launch.Ps2,"%s", dataparsed);

	 if(FX_Launch.N>=1){wLaunchFX=TRUE;}
 }
 void Multistage2015::parseTexture(char filename[MAXLEN]){
	 char textxt[128];
	 char bufftxt[128];
	 sprintf(textxt,"TEXTURE_LIST");
	  
	 int texn;
	for(texn=0;texn<16;texn++){
	sprintf(bufftxt,"TEX_%i",texn+1);

GetPrivateProfileString(textxt,bufftxt,buffreset,dataparsed,MAXLEN,filename);


if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){nTextures=texn;break;}
	char checktxt[MAXLEN];
	int k;
	for(k=0;k<MAXLEN;k++){
		tex.TextureName[k][texn]=dataparsed[k];
		checktxt[k]=dataparsed[k];
	}
//sprintf(tex.TextureName[texn],dataparsed);
sprintf(logbuff,"%s: Texture n.%i Loaded %s",GetName(),texn+1,checktxt);
oapiWriteLog(logbuff);
	tex.hTex[texn] = oapiRegisterExhaustTexture (checktxt); 
	}
 return;
 }
 void Multistage2015::parseMisc(char filename[MAXLEN]){
	 char Misctxt[64];
	 sprintf(Misctxt,"MISC");

	GetPrivateProfileString(Misctxt, "COG", buffreset, dataparsed, MAXLEN, filename);
	Misc.COG=atof(dataparsed);
	GetPrivateProfileString(Misctxt, "GNC_DEBUG", buffreset, dataparsed, MAXLEN, filename);
	Misc.GNC_Debug=atoi(dataparsed);
	int tval;
	GetPrivateProfileString(Misctxt, "TELEMETRY", buffreset, dataparsed, MAXLEN, filename);
	tval=atoi(dataparsed);
	if(tval==1){Misc.telemetry=TRUE;}else{Misc.telemetry=FALSE;}
	GetPrivateProfileString(Misctxt, "FOCUS", buffreset, dataparsed, MAXLEN, filename);
	Misc.Focus=atoi(dataparsed);
	int trp;
	GetPrivateProfileString(Misctxt, "THRUST_REAL_POS", buffreset, dataparsed, MAXLEN, filename);
	trp=atoi(dataparsed);
	if(trp==1){Misc.thrustrealpos=TRUE;sprintf(logbuff,"%s: Thrust in Real Position",GetName());oapiWriteLog(logbuff);}else{Misc.thrustrealpos=FALSE;}
	Misc.VerticalAngle=0;
	GetPrivateProfileString(Misctxt,"VERTICAL_ANGLE",buffreset,dataparsed,MAXLEN,filename);
	Misc.VerticalAngle=atof(dataparsed)*RAD;

	//added by rcraig42 to retrieve drag_factor from ini --------------------------------------------------------
	
	GetPrivateProfileString(Misctxt, "drag_factor", buffreset, dataparsed, MAXLEN, filename);
	Misc.drag_factor=atof(dataparsed);
	
	//------------------------------------------------------------------------------------------------------------
	GetPrivateProfileString(Misctxt,"PAD_MODULE",buffreset,dataparsed,MAXLEN,filename);
	if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){sprintf(Misc.PadModule,"EmptyModule");}else{sprintf(Misc.PadModule,"%s", dataparsed);}
	

	return;
 }

 void Multistage2015::parseSound(char filename[MAXLEN]){
	 char Soundtxt[64];
	 sprintf(Soundtxt,"SOUND");

	 GetPrivateProfileString(Soundtxt,"MAIN_THRUST",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Main=FALSE;}else{
		 Ssound.Main=TRUE; sprintf(Ssound.MAIN_THRUST,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"HOVER_THRUST",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Hover=FALSE;}else{
		 Ssound.Hover=TRUE; sprintf(Ssound.HOVER_THRUST,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"RCS_THRUST_ATTACK",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.RCS_ta=FALSE;}else{
		 Ssound.RCS_ta=TRUE; sprintf(Ssound.RCS_THRUST_ATTACK,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"RCS_THRUST_SUSTAIN",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.RCS_ts=FALSE;}else{
		 Ssound.RCS_ts=TRUE; sprintf(Ssound.RCS_THRUST_SUSTAIN,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"AIR_CONDITIONING",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ac=FALSE;}else{
		 Ssound.Ac=TRUE; sprintf(Ssound.AIR_CONDITIONNING,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_1",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca1=FALSE;}else{
		 Ssound.Ca1=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_1,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_2",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca2=FALSE;}else{
		 Ssound.Ca2=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_2,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_3",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca3=FALSE;}else{
		 Ssound.Ca3=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_3,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_4",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca4=FALSE;}else{
		 Ssound.Ca4=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_4,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_5",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca5=FALSE;}else{
		 Ssound.Ca5=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_5,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_6",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca6=FALSE;}else{
		 Ssound.Ca6=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_6,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_7",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca7=FALSE;}else{
		 Ssound.Ca7=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_7,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_8",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca8=FALSE;}else{
		 Ssound.Ca8=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_8,"%s", dataparsed);}
	 GetPrivateProfileString(Soundtxt,"COCKPIT_AMBIENCE_9",buffreset,dataparsed,MAXLEN,filename);
	 if(strncmp(dataparsed,buffreset,MAXLEN-5)==0){Ssound.Ca9=FALSE;}else{
		 Ssound.Ca9=TRUE; sprintf(Ssound.COCKPIT_AMBIENCE_9,"%s", dataparsed);}


	 return;
 }
bool Multistage2015::parseinifile(char filename[MAXLEN]){
	int r;
	for(r=0;r<MAXLEN;r++){
		buffreset[r]='0';
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
	sprintf(filebuff,"%s", OrbiterRoot);
	strcat(filebuff,"\\");
	strcat(filebuff,filename);
	string line;
	ifstream tlm_file (filebuff);
	//int index=0;
	loadedtlmlines=0;
	if (tlm_file.is_open())
    {
	
		  while (getline(tlm_file,line))
	      {
				if(!line.empty()){
					const char *cstr = line.c_str();

					//sprintf(buffer,"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",tlmAlt[i].x,tlmAlt[i].y,tlmSpeed[i].y,tlmPitch[i].y,tlmThrust[i].y,tlmMass[i].y,tlmVv[i].y,tlmAcc[i].y);
	            sscanf(cstr,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",&ReftlmAlt[loadedtlmlines].x,&ReftlmAlt[loadedtlmlines].y,&ReftlmSpeed[loadedtlmlines].y,&ReftlmPitch[loadedtlmlines].y,&ReftlmThrust[loadedtlmlines].y,&ReftlmMass[loadedtlmlines].y,&ReftlmVv[loadedtlmlines].y,&ReftlmAcc[loadedtlmlines].y);
				if(ReftlmAlt[loadedtlmlines].x==0){continue;}
				ReftlmSpeed[loadedtlmlines].x=ReftlmAlt[loadedtlmlines].x;
				ReftlmPitch[loadedtlmlines].x=ReftlmAlt[loadedtlmlines].x;
				ReftlmThrust[loadedtlmlines].x=ReftlmAlt[loadedtlmlines].x;
				ReftlmMass[loadedtlmlines].x=ReftlmAlt[loadedtlmlines].x;
				ReftlmVv[loadedtlmlines].x=ReftlmAlt[loadedtlmlines].x;
				ReftlmAcc[loadedtlmlines].x=ReftlmAlt[loadedtlmlines].x;
				//sprintf(logbuff,"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",ReftlmAlt[index].x,ReftlmAlt[index].y,ReftlmSpeed[index].y,ReftlmPitch[index].y,ReftlmThrust[index].y,ReftlmMass[index].y,ReftlmVv[index].y,ReftlmAcc[index].y);
				//oapiWriteLog(logbuff);
				loadedtlmlines++;
				}
		  }
	}
}

void Multistage2015::parseGuidanceFile(char filename[MAXLEN]){
	nsteps=1;
	

		char filebuff[MAXLEN];
		string line;
	sprintf(filebuff,"%s", OrbiterRoot);
	strcat(filebuff,"\\");
	strcat(filebuff,guidancefile);//Config\\multistage\\guidance_shuttle.txt");

	sprintf(logbuff,"%s: Guidance File present: %s",GetName(),guidancefile);
	oapiWriteLog(logbuff);
  ifstream gnc_file (filebuff);
  if (gnc_file.is_open())
  {
	
  while (getline(gnc_file,line))
	
    {
	if(!line.empty()){
		
		for(int i=0;i<6;i++){Gnc_step[nsteps].wValue[i]=FALSE;}
		
		std::size_t findEqual=line.find_first_of("=");
		if(findEqual!=line.npos){
		string mettime=line.substr(0,findEqual);
		
		Gnc_step[nsteps].time=atof(&mettime[0]);

		std::size_t findLineEnd=line.find_first_of(")");
		if(findLineEnd!=line.npos){
	
		std::size_t findOpenP=line.find_first_of("(");
		if(findOpenP!=line.npos){
			string comand=line.substr(findEqual+1,findOpenP-findEqual-1);
			sprintf(Gnc_step[nsteps].Comand,"%s", &comand[0]);
			
			string values=line.substr(findOpenP+1,findLineEnd-findOpenP-1);
				Gnc_step[nsteps].wValue[0]=TRUE;			
			std::size_t findFirstComma=values.find_first_of(",");
			if(findFirstComma!=values.npos){
				value1=values.substr(0,findFirstComma);
			Gnc_step[nsteps].wValue[1]=TRUE;
			

			std::size_t findSecondComma=values.find_first_of(",",findFirstComma+1);
						
			if(findSecondComma!=values.npos){
				
				value2=values.substr(findFirstComma+1,findSecondComma-findFirstComma-1);
				Gnc_step[nsteps].wValue[2]=TRUE;

			std::size_t findThirdComma=values.find_first_of(",",findSecondComma+1);
				
			if(findThirdComma!=values.npos){
				
				value3=values.substr(findSecondComma+1,findThirdComma-findSecondComma-1);
				Gnc_step[nsteps].wValue[3]=TRUE;

			std::size_t findFourthComma=values.find_first_of(",",findThirdComma+1);
			if(findFourthComma!=values.npos){	
				value4=values.substr(findThirdComma+1,findFourthComma-findThirdComma-1);
				Gnc_step[nsteps].wValue[4]=TRUE;
			std::size_t findFifthComma=values.find_first_of(",",findFourthComma+1);
			if(findFifthComma!=values.npos){
				value5=values.substr(findFourthComma+1,findFifthComma-findFourthComma-1);
				value6=values.substr(findFifthComma+1,string::npos);
				Gnc_step[nsteps].wValue[5]=TRUE;
			}else{ value5=values.substr(findFourthComma+1,string::npos);}
			}else{ value4=values.substr(findThirdComma+1,values.npos);}
			}else{ value3=values.substr(findSecondComma+1,values.npos);}
			}else{ value2=values.substr(findFirstComma+1,values.npos);}
			}else{ value1=values;}
		}}
		if(Gnc_step[nsteps].wValue[0]){	Gnc_step[nsteps].trval1=atof(&value1[0]);}else{Gnc_step[nsteps].trval1=0;}
		if(Gnc_step[nsteps].wValue[1]){	Gnc_step[nsteps].trval2=atof(&value2[0]);}else{Gnc_step[nsteps].trval2=0;}
		if(Gnc_step[nsteps].wValue[2]){	Gnc_step[nsteps].trval3=atof(&value3[0]);}else{Gnc_step[nsteps].trval3=0;}
		if(Gnc_step[nsteps].wValue[3]){	Gnc_step[nsteps].trval4=atof(&value4[0]);}else{Gnc_step[nsteps].trval4=0;}
		if(Gnc_step[nsteps].wValue[4]){	Gnc_step[nsteps].trval5=atof(&value5[0]);}else{Gnc_step[nsteps].trval5=0;}
		if(Gnc_step[nsteps].wValue[5]){	Gnc_step[nsteps].trval6=atof(&value6[0]);}else{Gnc_step[nsteps].trval6=0;}

		}else{
		sprintf(Gnc_step[nsteps].Comand,"noline");	
		Gnc_step[nsteps].GNC_Comand=CM_NOLINE;
		Gnc_step[nsteps].time_fin=-10000;
		Gnc_step[nsteps].trval1=0;
		Gnc_step[nsteps].trval2=0;
		Gnc_step[nsteps].trval3=0;
		Gnc_step[nsteps].trval4=0;
		Gnc_step[nsteps].trval5=0;
		Gnc_step[nsteps].trval6=0;
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
		
		for(int l=0;l<line.size();l++){
			line[l]=tolower(line[l]);
		}
		
		std::size_t foundDisable=line.find("disable");
		std::size_t foundPlay=line.find("playsound");

		if(foundDisable!=std::string::npos){
		std::size_t foundDisPitch=line.find("pitch");
		std::size_t foundDisRoll=line.find("roll");
		std::size_t foundDisJett=line.find("jettison");

			if(foundDisPitch!=std::string::npos){
				sprintf(Gnc_step[nsteps].Comand,"disablepitch");
			}else if(foundDisRoll!=std::string::npos){
				sprintf(Gnc_step[nsteps].Comand,"disableroll");
			}else if(foundDisJett!=std::string::npos){
				sprintf(Gnc_step[nsteps].Comand,"disablejettison");
			}

		}else if(foundPlay!=std::string::npos){
		std::size_t findopen=line.find_first_of("(");
		std::size_t findclose=line.find_first_of(")");
		string filename=line.substr(findopen+1,findclose-findopen-1);
			sprintf(Gnc_step[nsteps].Comand,"playsound");
			size_t len = min(filename.length(), size_t(255));
			filename.copy(Gnc_step[nsteps].trchar, len);
			Gnc_step[nsteps].trchar[len] = '\0';
		}

		
		
			
		++nsteps;
	 }else{continue;}
		
	 
    }
	  
  nsteps-=1;
 
    gnc_file.close();
  }

  VinkaComposeGNCSteps();
  VinkaRearrangeSteps();
  nsteps=VinkaCountSteps();
	return;
}
