//############################################################################//
#define ORBITER_MODULE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <math.h>
//#include "OrbiterSoundSDK35.h"
//############################################################################//
#include "boom.h"
//############################################################################//
//Class creation
boom::boom(OBJHANDLE hObj,int fmodel):VESSEL4(hObj,fmodel){

	ph_h[0] = nullptr;

	th_h[0] = nullptr;

	thg_h[0] = nullptr;

	msh_h[0] = nullptr;

	msh_idh[0] = NULL;

	MyID2 = NULL;

	deltat1 = NULL;
	
}
boom::~boom(){}
//############################################################################//
//All the parameters
void boom::clbkSetClassCaps(FILEHANDLE cfg){
	deltat1=0;

// int i;
 //THRUSTER_HANDLE **grps;
 //int *grpc;

	 //Base stuff
	 SetSize(5);
	 SetEmptyMass(100);
	 SetPMI(_V(1,1,1));
	 SetCrossSections(_V(0.1,0.1,0.1));
	 SetRotDrag(_V(0,0,0));
	 SetTouchdownPoints(_V(0,0,1),_V(-1,0,-1),_V(1,0,-1));
	 SetSurfaceFrictionCoeff(0.07,0.3);
	 SetCW(0.09,0.09,2,1.4);
	 EnableTransponder(true);
	 InitNavRadios(4);
	 SetCameraOffset(_V(0,0,0));

	 //SetVisibilityLimit(0,0.002);
	 SetAlbedoRGB(_V((float)255/255,(float)255/255,(float)255/255));
	 //SetGravityGradientDamping(0);
	 SetWingAspect(0);
	 SetWingEffectiveness(0);
	 SetMaxWheelbrakeForce(200000);
	 //SetBankMomentScale(0);
	 //SetPitchMomentScale(0);

	char contrail_tex[] = "Multistage2015\\Contrail2";
	SURFHANDLE texfire = oapiRegisterExhaustTexture (contrail_tex); 
	static PARTICLESTREAMSPEC fire= {
		0, //flag
		30, //size
		20, //rate
		0, //velocity
		0, //velocity distribution
		10, //lifetime
		45, //growthrate
		0, //atmslowdown
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_LIN, 0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1,
		texfire
	  };

/*static PARTICLESTREAMSPEC vent = {
	0,		// flag
	0.1,	// size
	15,		// rate
	0.55,	    // velocity
	5,    // velocity distribution
	1.03,		// lifetime
	0.5,	// growthrate
	6,    // atmslowdown 
	PARTICLESTREAMSPEC::EMISSIVE,
	PARTICLESTREAMSPEC::LVL_PLIN, 0.75, 1.0,
	PARTICLESTREAMSPEC::ATM_FLAT, 1.0, 1.0
};*/
	char explosionbolt_tex[] = "Multistage2015\\Explosivebolt";
	SURFHANDLE JSexplosivebolt = oapiRegisterExhaustTexture (explosionbolt_tex); 
	static PARTICLESTREAMSPEC stagingfireps = {

		0,		// flag
		3,	// size
		50,		// rate
		50,	    // velocity
		20.,    // velocity distribution
		10,		// lifetime
		4,	// growthrate
		0,    // atmslowdown 
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_LIN, 0, 1.0,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1,
		JSexplosivebolt
	};
	char explosion_tex[] = "Multistage2015\\explosion";
	SURFHANDLE explosion = oapiRegisterExhaustTexture (explosion_tex); 
	static PARTICLESTREAMSPEC def = {

		0,		// flag
		2,	// size
		0.5,		// rate
		5,	    // velocity
		0.5,    // velocity distribution
		10,		// lifetime
		30,	// growthrate
		0,    // atmslowdown 
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_LIN, 0, 1.0,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1,
		explosion
	};

	 //Fuel
	 ph_h[0]=CreatePropellantResource(1);

	 //Thrusters and groups
	 //grps=(THRUSTER_HANDLE**)malloc(1*sizeof(THRUSTER_HANDLE));
	 //grpc=(int*)malloc(1*sizeof(int));
	 //for(i=0;i<1;i++){grps[i]=(THRUSTER_HANDLE*)malloc(1*sizeof(THRUSTER_HANDLE));grpc[i]=0;}
 
	 th_h[0]=CreateThruster(_V(0,0,0),_V(0,0,1),1,ph_h[0],3,0,101400);
	 //grps[0][grpc[0]]=th_h[0];
	 //grpc[0]++;
	 //for(i=0;i<1;i++){
	  //if(i<15)thg_h[i]=CreateThrusterGroup(grps[i],grpc[i],(THGROUP_TYPE)(THGROUP_MAIN+i));
		// else thg_h[i]=CreateThrusterGroup(grps[i],grpc[i],THGROUP_USER);
	 //}


	 AddExhaustStream(th_h[0],&stagingfireps);
	 AddExhaustStream(th_h[0],&fire);
	 AddExhaustStream(th_h[0],&def);

	 //Meshes
	 //SetMeshVisibilityMode(msh_idh[0]=AddMesh(msh_h[0]=oapiLoadMeshGlobal("jarvis\\bolt1_1")),0);
}


void boom::clbkPostStep(double simt, double simdt, double mjd){
	//sprintf(oapiDebugString(),"deltat1 %.3f",deltat1);
	//oapiWriteLog("first frame");
	deltat1+=oapiGetSimStep();
	//oapiWriteLog("first frame before play sound");
	if((deltat1>0)&&(deltat1<2)){
//		PlayVesselRadioExclusiveWave3(MyID2,1);
	}
	//oapiWriteLog("first frame before th0");
	SetThrusterLevel(th_h[0],1);
	//oapiWriteLog("first frame after th0");
	if(deltat1>20){
	oapiDeleteVessel(GetHandle());
	}
	//oapiWriteLog("first frame after if-delete");
}


//############################################################################//
void boom::clbkLoadStateEx(FILEHANDLE scn,void *vs){
	 char *line;

	 while(oapiReadScenario_nextline(scn,line)){
	   ParseScenarioLineEx(line,vs);
	 }
}
//############################################################################//
void boom::clbkSaveState(FILEHANDLE scn){
	SaveDefaultState(scn);
}


void boom::clbkPostCreation(){
//	MyID2=ConnectToOrbiterSoundDLL3(GetHandle());
	
	//RequestLoadVesselWave3(MyID2,1,"Sound\\boom\\explosion.wav",DEFAULT);
	char msg1[] = "About To Explode";
	oapiWriteLog(msg1);
	

}

//#######################################################################################//
//Creation
DLLCLBK void InitModule(HINSTANCE hModule){}
DLLCLBK void ExitModule(HINSTANCE hModule){}
DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel,int flightmodel){return new boom(hvessel,flightmodel);}
DLLCLBK void ovcExit(VESSEL *vessel){if(vessel)delete(boom*)vessel;}
//#######################################################################################//
