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
// peg.cpp
// ==============================================================





//############################################################################//

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <math.h>
#include <stdio.h>
#include "..//..//Orbitersdk//include//Orbitersdk.h"


#include "Multistage2015.h"


VECTOR3 Multistage2015::GetAPTargets()
{
	return _V(tgtapo, tgtperi, tgtinc);
}
//Get Vehicle Acceleration along Velocity Vector
double Multistage2015::getacc() {

	double acc;
	VECTOR3 force_vec, acc_vec, spd_vec;
	VECTOR3 weight_vec, thrust_vec, drag_vec;
	// Get the vectors 
	//GetShipAirspeedVector(spd_vec);
	GetGroundspeedVector(FRAME_HORIZON, spd_vec);
	GetThrustVector(thrust_vec);
	GetWeightVector(weight_vec);
	GetDragVector(drag_vec);
	force_vec.x = thrust_vec.x + weight_vec.x + drag_vec.x;
	force_vec.y = thrust_vec.y + weight_vec.y + drag_vec.y;
	force_vec.z = thrust_vec.z + weight_vec.z + drag_vec.z;
	// Normalize the speed vector
	normalise(spd_vec);
	// Calculate the acceleration vector
	acc_vec = force_vec / GetMass();

	// Take the dot product
	acc = dotp(acc_vec, spd_vec);

	return acc;
}


double Multistage2015::getabsacc()
{
	VECTOR3 ThrustVec, DragVec;
	GetThrustVector(ThrustVec);
	GetDragVector(DragVec);
	return (length(ThrustVec) - length(DragVec)) / GetMass();
}
//Initialize PEG
void Multistage2015::InitPEG() {
	oapiWriteLog(const_cast<char*>("RESET PEG"));
	for (int i = 1; i <= nStages; i++) {
		v_e[i] = stage[i - 1].isp;
		a_[i] = stage[i - 1].thrust / GetMassAtStage(i - 1, FALSE);
		a_fin[i] = stage[i - 1].thrust / GetMassAtStage(i - 1, TRUE);
		tau_[i] = v_e[i] / a_[i];
		A[i] = 0;
		B[i] = 0;
	}
	NN = nStages;
	for (int i = 0; i < 10; i++)
	{
		T[i] = 0;
		OmegaS[i] = 0;
		delta_rdot[i] = 0;
		rdot_in[i] = 0;
		delta_r[i] = 0;
		r_in[i] = 0;
		r_T[i] = 0;
		rdot_T[i] = 0;
		VthetaAtStaging[i] = 0;
	}


	return;
}

void Multistage2015::CalculateTargets()
{
	if ((tgtabside > tgtapo) || (tgtabside < tgtperi))
	{
		if (tgtperi >= 80000)
		{
			tgtabside = tgtperi;
		}
		else {
			tgtabside = tgtapo;
		}
	}
	double etarget = (tgtapo + rt - tgtperi - rt) / (tgtapo + 2 * rt + tgtperi);
	double atarget = (tgtapo + rt) / (1 + etarget);
	epsfin = -mu / (2 * atarget);
	double vr_target = sqrt(mu * (2 / (tgtabside + rt) - 1 / (0.5 * (tgtapo + tgtperi + 2 * rt))));
	if (etarget == 0)
	{
		tgtvv = 0;
	}
	else {
		if ((tgtabside == tgtperi) || (tgtabside == tgtapo)) {
			tgtvv = 0;
		}
		else {
			double cosTA = (atarget * (1 - (etarget * etarget)) - (tgtabside + rt)) / ((tgtabside + rt) * etarget);
			double tgtTrueAnomaly = acos(cosTA);
			tgtvv = vr_target * (etarget * sin(tgtTrueAnomaly)) / (1 + etarget * cos(tgtTrueAnomaly));
		}
	}
	return;
}

bool Multistage2015::CutoffCheck() {
	OBJHANDLE hearth = GetSurfaceRef();
	GetElements(hearth, el, &op, 0, 1);
	eps = -mu / (2 * el.a);
	if (eps >= epsfin) {
		killAP();
		runningPeg = FALSE;
		return TRUE;
	}
	return FALSE;
}

void Multistage2015::CalcMECO()
{
	double MECO = 0;
	for (int i = 1; i <= NN; i++)
	{
		MECO += T[i];
	}



	MECO += MET;
	if (MECO_Counter >= 100) { MECO_Counter = 0; }
	double delta = MECO - MECO_TEST;
	coeff[MECO_Counter] = delta / UpdatePegTimer;
	double transcoeff = 0;
	for (int h = 0; h < 100; h++)
	{
		transcoeff += coeff[h];
	}
	avgcoeff = transcoeff / 100;
	MECO_TEST = MECO;
	MECO_Counter++;

	if (avgcoeff < 1) { TMeco = MET + (MECO - MET) / (1 - avgcoeff); }
	else { TMeco = MECO; }
	return;
}
void Multistage2015::Navigate() {
	J = currentStage + 1;
	//GetHorizonAirspeedVector(ShipSpeed);
	GetGroundspeedVector(FRAME_HORIZON, ShipSpeed);
	VertVel = ShipSpeed.y;
	GetRelativePos(GetGravityRef(), rvec);
	GetRelativeVel(GetGravityRef(), vvec);
	r = length(rvec);
	v = length(vvec);
	hvec = crossp(rvec, vvec);
	h = length(hvec);
	thrust = stage[currentStage].thrust;
	mass = GetMass();

	omega = h / (r * r);
	if (wBoosters) { a_[J] = thrust / mass; }
	else { a_[J] = getabsacc(); }
	v_e[J] = stage[J - 1].isp;
	tau_[J] = v_e[J] / a_[J];
	NN = coc.stage + 1;

	for (int i = 1; i <= NN; i++)
	{
		T[i] = coc.remBT[i - 1];
	}

	g = -mu / (r * r);
	cent = omega * omega * r;
	g_term = -g / a_[J];
	cent_term = -cent / a_[J];

	A[J - 1] = 0;
	B[J - 1] = 0;
	DeltaA[J - 1] = 0;
	DeltaB[J - 1] = 0;
	DeltaA[NN] = 0;
	DeltaB[NN] = 0;

}

double Multistage2015::b0(int j, double t_j) {
	return -v_e[j] * log(1 - t_j / tau_[j]);
}

double Multistage2015::b_(int n, int j, double t_j) {
	if (n == 0) return b0(j, t_j);
	return b_(n - 1, j, t_j) * tau_[j] - v_e[j] * pow(t_j, n) / n;
}

double Multistage2015::c0(int j, double t_j) {
	return b0(j, t_j) * t_j - b_(1, j, t_j);
}

double Multistage2015::c_(int n, int j, double t_j) {
	if (n == 0) return c0(j, t_j);
	return c_(n - 1, j, t_j) * tau_[j] - v_e[j] * pow(t_j, n + 1) / (n * (n + 1));
}


double Multistage2015::a(int j, double t_j) {
	return a_[j] / (1 - t_j / tau_[j]);
}

void Multistage2015::FEstimate()
{

	for (int i = 0; i < 10; i++)
	{
		coc.remBT[i] = 0;
	}
	double T_MECO = 0;
	double RemDvToDo = finalv(tgtabside, tgtapo, tgtperi) - GetOS();
	double RemDv = RemDvToDo;
	int q = 0;

	while ((RemDv > StageRemDv(currentStage + q)) && (currentStage + q < nStages - 1))
	{
		RemDv -= StageRemDv(currentStage + q);
		coc.remBT[currentStage + q] = RemBurnTime(currentStage + q, 1);
		T_MECO += RemBurnTime(currentStage + q, 1);
		q++;
	}

	T_MECO += RemDv * GetPropellantMass(stage[currentStage + q].tank) / (stage[currentStage + q].thrust * log((GetMassAtStage(currentStage + q, TRUE) + GetPropellantMass(stage[currentStage + q].tank)) / GetMassAtStage(currentStage + q, TRUE)));
	coc.remBT[currentStage + q] = RemDv * GetPropellantMass(stage[currentStage + q].tank) / (stage[currentStage + q].thrust * log((GetMassAtStage(currentStage + q, TRUE) + GetPropellantMass(stage[currentStage + q].tank)) / GetMassAtStage(currentStage + q, TRUE)));

	coc.T = T_MECO;
	coc.stage = currentStage + q;
	coc.rem_stages = q;

	return;
}

void Multistage2015::FStaging()
{
	r_T[J] = r + VertVel * T[J] + c0(1, T[J]) * A[J] + c_(1, 1, T[J]) * B[J];
	rdot_T[J] = VertVel + b0(1, T[J]) * A[J] + b_(1, 1, T[J]) * B[J];

	r_in[J] = r;
	rdot_in[J] = VertVel;

	for (int i = J + 1; i < NN; i++)
	{
		delta_rdot[i] = b0(i, T[i]) * A[i] + b_(1, i, T[i]) * B[i];
		delta_r[i] = rdot_in[i] * T[i] + c0(i, T[i]) * A[i] + c_(1, i, T[i]) * B[i];
	}

	for (int i = J + 1; i < NN; i++)
	{
		rdot_in[i] = rdot_in[J];
		r_in[i] = r_in[J];
		for (int j = J + 1; j <= i - 1; j++)
		{
			rdot_in[i] += delta_rdot[j];
			r_in[i] += delta_r[j];
		}
	}

	for (int j = J + 1; j < NN; j++)
	{
		rdot_T[j] = rdot_in[j];
		r_T[j] = r_in[j];
	}

	r_T[NN] = tgtabside + rt;//tgtapo+rt;
	rdot_T[NN] = tgtvv;//0;

	VthetaAtStaging[J] = GetOS() + StageRemDv(J - 1);
	for (int i = J + 1; i <= NN; i++)
	{
		VthetaAtStaging[i] = VthetaAtStaging[i - 1] + StageRemDv(i - 1);
	}
	for (int i = J; i <= NN; i++)
	{
		OmegaS[i] = VthetaAtStaging[i] / r_T[i];//FredEstimateStaging().r_T[i];
	}

	return;
}

//void Multistage2015::PEG(){
void Multistage2015::MajorCycle() {
	CalculateTargets();
	Navigate();
	FStaging();
	FEstimate();
	CalcMECO();
	for (int i = currentStage + 1; i < NN; i++)
	{
		DeltaA[i] = (mu / (r_T[i] * r_T[i]) - OmegaS[i] * OmegaS[i] * r_T[i]) * ((1 / (a_fin[i]) - (1 / (a_[i + 1]))));
		DeltaB[i] = (mu / (r_T[i] * r_T[i]) - OmegaS[i] * OmegaS[i] * r_T[i]) * ((1 / v_e[i]) - (1 / v_e[i + 1])) + ((3 * OmegaS[i] * OmegaS[i] - 2 * mu / (r_T[i] * r_T[i] * r_T[i])) * rdot_T[i]) * ((1 / (a_fin[i]) - (1 / (a_[i + 1]))));
	}



	double alpha = 0;
	for (int l = J; l <= NN; l++)
	{
		alpha += b0(l, T[l]);
	}

	double beta = 0;
	for (int l = J; l <= NN; l++)
	{
		double term = 0;
		for (int k = J; k <= l - 1; k++)
		{
			term += T[k];
		}
		beta += b_(1, l, T[l]) + b0(l, T[l]) * term;
	}

	double gamma = 0;
	for (int l = J; l <= NN; l++)
	{
		double term1 = 0;
		for (int k = J; k <= l - 1; k++)
		{
			double term2 = 0;
			for (int i = J; i <= k - 1; i++)
			{
				term2 += DeltaB[i];
			}
			term1 += b0(l, T[l]) * DeltaA[k] + b0(l, T[l]) * T[k] * term2 + b_(1, l, T[l]) * DeltaB[k];
		}
		gamma += term1;
	}

	gamma = gamma * (-1);
	gamma += tgtvv - VertVel;

	double epsilon = 0;
	for (int l = J; l <= NN; l++)
	{
		double term = 0;
		for (int k = J; k <= l - 1; k++)
		{
			term += b0(k, T[k]);
		}
		epsilon += c0(l, T[l]) + T[l] * term;
	}


	double zeta = 0;

	for (int l = J; l <= NN; l++)
	{
		double term2 = 0;
		for (int k = J; k <= l - 1; k++)
		{
			double term1 = 0;
			for (int i = J; i <= k - 1; i++)
			{
				term1 += T[i];
			}
			term2 += c0(l, T[l]) * T[k] + b_(1, k, T[k]) * T[l] + b0(k, T[k]) * T[l] * term1;
		}
		zeta += c_(1, l, T[l]) + term2;
	}

	double iota = 0;
	for (int l = J; l <= NN; l++)
	{
		double term3 = 0;
		for (int k = J; k <= l - 1; k++)
		{
			double term2 = 0;
			for (int i = J; i <= k - 1; i++)
			{
				double term1 = 0;
				for (int m = J; m <= i - 1; m++)
				{
					term1 += DeltaB[m];
				}
				term2 += b0(k, T[k]) * T[l] * DeltaA[i] + b0(k, T[k]) * T[i] * T[l] * term1 + b_(1, k, T[k]) * T[l] * DeltaB[i] + c0(l, T[l]) * T[k] * DeltaB[i];
			}
			term3 += c0(l, T[l]) * DeltaA[k] + c_(1, l, T[l]) * DeltaB[k] + term2;
		}
		iota += term3;
	}

	double k23 = 0;
	for (int l = J; l <= NN; l++)
	{
		k23 += T[l];
	}

	k23 = k23 * VertVel;
	double lambda = (tgtabside + rt) - k23 - (GetAltitude() + rt) - iota;

	A[J] = (gamma * zeta - beta * lambda) / (alpha * zeta - beta * epsilon);
	B[J] = (alpha * lambda - gamma * epsilon) / (alpha * zeta - beta * epsilon);




	double transvarA = 0;
	for (int l = J; l <= NN; l++)
	{
		double term1 = 0;
		for (int k = J; k < l - 1; k++)
		{
			term1 += DeltaB[k];
		}
		transvarA += DeltaA[l] + B[1] * T[l] + T[l] * term1;
	}


	double transvarB = 0;
	for (int l = J; l <= NN; l++)
	{
		transvarB += DeltaB[l];
	}



	for (int l = J + 1; l <= NN; l++)
	{
		A[l] = A[l - 1] + transvarA;
		B[l] = B[l - 1] + transvarB;
	}


}

void Multistage2015::PEG() {

	UpdatePegTimer += oapiGetSimStep();

	if (UpdatePegTimer >= PegMajorCycleInterval) //0.1 was here
	{
		MajorCycle();
		UpdatePegTimer = 0;
	}



	double arg = 0;

	if ((A[J] < 10) && (A[J] > -10))
	{
		//arg=A[J]+B[J]*oapiGetSimStep()+g_term+cent_term;
		arg = A[J] + B[J] * UpdatePegTimer + g_term + cent_term;
	}
	else {
		InitPEG();
	}


	//	if(J==1){pitchlimit=35*RAD;}else{pitchlimit=35*RAD;} //For future refinery of peg program start

	if (abs(arg) < sin(PegPitchLimit))
	{
		PegDesPitch = asin(arg);
	}
	else {
		PegDesPitch = PegPitchLimit * SignOf(arg);//*(abs(arg)/arg);
	}
	if (abs(arg) < 1)
	{
		TgtPitch = asin(arg);
	}
	else {
		TgtPitch = 90 * RAD * SignOf(arg);
	};

	// ACCOUNT FOR THRUST NOT IN INLINE WITH VEHICLE
	VECTOR3 thrustVector;
	GetThrustVector(thrustVector);
	if (length(thrustVector) > 0)
	{
		normalise(thrustVector);
		PegDesPitch -= asin(thrustVector.y);
		TgtPitch -= asin(thrustVector.y);
	}



	Attitude(PegDesPitch, (0.5 * (1 - VinkaMode) * PI), GetProperHeading(), 8, 5, 8);
	//sprintf(oapiDebugString(),"DesPitch: %.1f arg: %.3f T1:%.1f T2 %.1f T3:%.1f NN:%i r_T[1]:%.3f rdot_T[1]:%.3f",PegDesPitch*DEG,arg,T[1],T[2],T[3],NN,(r_T[currentStage+1]-rt)/1000,rdot_T[currentStage+1]);
	//oapiWriteLog(oapiDebugString());
}

void Multistage2015::SetPegMajorCycleInterval(double newinterval)
{
	PegMajorCycleInterval = newinterval;
}

void Multistage2015::SetPegPitchLimit(double newlimit)
{
	PegPitchLimit = newlimit;
}