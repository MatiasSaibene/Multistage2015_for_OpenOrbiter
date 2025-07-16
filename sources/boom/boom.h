#pragma once
//#######################################################################################//
//Vessel helper structures
//#######################################################################################//
//Vessel constants
//#######################################################################################//
//Vessel class
#include "..//..//Orbitersdk//include//Orbitersdk.h"
class boom:public VESSEL4{
public:
 boom(OBJHANDLE hObj,int fmodel);
 ~boom();
 void clbkSetClassCaps(FILEHANDLE cfg) override;
 void clbkLoadStateEx(FILEHANDLE scn,void *vs) override;
 void clbkSaveState(FILEHANDLE scn) override;
 
 void clbkPostStep(double simt, double simdt, double mjd) override;

 void clbkPostCreation() override;


 PROPELLANT_HANDLE ph_h[1];
 THRUSTER_HANDLE th_h[1];
 THGROUP_HANDLE thg_h[1];
 MESHHANDLE msh_h[1];
 int msh_idh[1];
 int MyID2;
 double deltat1;
};
//#######################################################################################//
