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
// MS_Crawler.cpp
// ==============================================================





//############################################################################//
#define ORBITER_MODULE
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_NONSTDC_NO_DEPRECATE
#include <math.h>
#include <stdio.h>
//#include "orbitersdk.h"


#include "MS_Crawler.h"

//Creation
MS_Crawler::MS_Crawler(OBJHANDLE hObj, int fmodel) :VESSEL3(hObj, fmodel) {
	FWD = false;

	BWD = false;

	LFT = false;

	RGT = false;

	Speed = 0.0;

	LengthCrawl = 0.0;

	rt = 0.0;

	earth_circ = 0.0;

	each_deg = 0.0;

	AngSpeed = 0.0;

	TangentSpd = 0.0;

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			StateCing[i][j] = 0.0;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Initials[i][j] = 0.0;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Tr1[i][j] = nullptr;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Rot1[i][j] = nullptr;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Tr2[i][j] = nullptr;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Rot2[i][j] = nullptr;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Tr3[i][j] = nullptr;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Rot3[i][j] = nullptr;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Tr4[i][j] = nullptr;
		}
	}

	for (int i = 0; i < NANIM; i++) {
		for (int j = 0; j < NCING; j++) {
			Rot4[i][j] = nullptr;
		}
	}

	StateLifter = 0.0;

	SpeedLifter = 0.0;

	Lifter = nullptr;

	Pistons = nullptr;

	GoingUp = false;

	GoingDown = false;

	for (int i = 0; i < 4; i++) {
		lights[i] = nullptr;
	}

	col_d = { 0 };
	col_s = { 0 };
	col_a = { 0 };
	col_white = { 0 };

	hPad = nullptr;

	vPad = nullptr;

	vsPad = { 0 };

	AttPadCrawler = nullptr;

	AttPadRocket = nullptr;

	AttCrawlerPad = nullptr;

	msh = nullptr;

	PadDetached = false;

	PadMesh[0] = '\0';

	PadModule[0] = '\0';

	wCustomPadModule = false;

	NoPad = false;

	AnimLifter = 0;

	CameraOff = _V(0, 0, 0);

	for (unsigned int i = 0; i < 64; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			animCing[i][j] = 0;
		}
	}
}
MS_Crawler::~MS_Crawler() {

}


void MS_Crawler::DefineAnimations()
{
	VECTOR3 Translate1 = _V(0, 0, 4);
	VECTOR3 Rotate1 = _V(0, 3.48, 22.71);
	double Angle1 = 22.4 * RAD;
	//VECTOR3 Translate2=_V(0,-1.9,4.63);
	VECTOR3 Translate2 = _V(0, -sin(Angle1) * 4.5, 4.5 * cos(Angle1));
	VECTOR3 Rotate2 = _V(0, 1.66, 26.73);
	double Angle2 = 154.3 * RAD;
	VECTOR3 Translate3 = _V(0, 0, -12.05);
	VECTOR3 Rotate3 = _V(0, 1.66, 14.68);
	double Angle3 = 160.9 * RAD;
	//double Angle3=154.3*RAD;

	//VECTOR3 Translate4=_V(0,1.82,4.63);
	VECTOR3 Translate4 = _V(0, sin(Angle1) * 4.25, cos(Angle1) * 4.25);
	VECTOR3 Rotate4 = _V(0, 3.48, 18.71);
	double Angle4 = 22.4 * RAD;
	//	double Angle4=29*RAD;



		//static UINT Elements[NANIM]={481,479,436,435,456,423,421,459,466,468,467,420,422,428,425,424,426,427,469,463,462,465,458,464,461,457,460,455,453,454,440,441,451,452,450,449,444,446,443,438,447,445,442,448,439,473,474,472,470,476,478,477,475,471,480,432,429,437,431,430,434,433,482,483};
	static UINT ElementsCing1[NANIM] = {
481	,
479	,
436	,
435	,
456	,
423	,
421	,
459	,
466	,
468	,
467	,
420	,
422	,
428	,
425	,
424	,
426	,
427	,
469	,
463	,
462	,
465	,
458	,
464	,
461	,
457	,
460	,
455	,
453	,
454	,
440	,
441	,
451	,
452	,
450	,
449	,
444	,
446	,
443	,
438	,
447	,
445	,
442	,
448	,
439	,
473	,
474	,
472	,
470	,
476	,
478	,
477	,
475	,
471	,
480	,
432	,
429	,
437	,
431	,
430	,
434	,
433	,
482	,
483
	};

	static UINT ElementsCing2[NANIM] = {
		417	,
415	,
372	,
371	,
392	,
359	,
357	,
395	,
402	,
404	,
403	,
356	,
358	,
364	,
361	,
360	,
362	,
363	,
405	,
399	,
398	,
401	,
394	,
400	,
397	,
393	,
396	,
391	,
389	,
390	,
376	,
377	,
387	,
388	,
386	,
385	,
380	,
382	,
379	,
374	,
383	,
381	,
378	,
384	,
375	,
409	,
410	,
408	,
406	,
412	,
414	,
413	,
411	,
407	,
416	,
368	,
365	,
373	,
367	,
366	,
370	,
369	,
418	,
419
	};

	static UINT ElementsCing3[NANIM] = {
		353	,
351	,
308	,
307	,
328	,
295	,
293	,
331	,
338	,
340	,
339	,
292	,
294	,
300	,
297	,
296	,
298	,
299	,
341	,
335	,
334	,
337	,
330	,
336	,
333	,
329	,
332	,
327	,
325	,
326	,
312	,
313	,
323	,
324	,
322	,
321	,
316	,
318	,
315	,
310	,
319	,
317	,
314	,
320	,
311	,
345	,
346	,
344	,
342	,
348	,
350	,
349	,
347	,
343	,
352	,
304	,
301	,
309	,
303	,
302	,
306	,
305	,
354	,
355
	};

	static UINT ElementsCing4[NANIM] = {
	289	,
	287	,
	244	,
	243	,
	264	,
	231	,
	229	,
	267	,
	274	,
	276	,
	275	,
	228	,
	230	,
	236	,
	233	,
	232	,
	234	,
	235	,
	277	,
	271	,
	270	,
	273	,
	266	,
	272	,
	269	,
	265	,
	268	,
	263	,
	261	,
	262	,
	248	,
	249	,
	259	,
	260	,
	258	,
	257	,
	252	,
	254	,
	251	,
	246	,
	255	,
	253	,
	250	,
	256	,
	247	,
	281	,
	282	,
	280	,
	278	,
	284	,
	286	,
	285	,
	283	,
	279	,
	288	,
	240	,
	237	,
	245	,
	239	,
	238	,
	242	,
	241	,
	290	,
	291

	};

	static UINT ElementsCing5[NANIM] = {
		225	,
	223	,
	180	,
	179	,
	200	,
	167	,
	165	,
	203	,
	210	,
	212	,
	211	,
	164	,
	166	,
	172	,
	169	,
	168	,
	170	,
	171	,
	213	,
	207	,
	206	,
	209	,
	202	,
	208	,
	205	,
	201	,
	204	,
	199	,
	197	,
	198	,
	184	,
	185	,
	195	,
	196	,
	194	,
	193	,
	188	,
	190	,
	187	,
	182	,
	191	,
	189	,
	186	,
	192	,
	183	,
	217	,
	218	,
	216	,
	214	,
	220	,
	222	,
	221	,
	219	,
	215	,
	224	,
	176	,
	173	,
	181	,
	175	,
	174	,
	178	,
	177	,
	226	,
	227

	};

	static UINT ElementsCing6[NANIM] = {
		161	,
	159	,
	116	,
	115	,
	136	,
	103	,
	101	,
	139	,
	146	,
	148	,
	147	,
	100	,
	102	,
	108	,
	105	,
	104	,
	106	,
	107	,
	149	,
	143	,
	142	,
	145	,
	138	,
	144	,
	141	,
	137	,
	140	,
	135	,
	133	,
	134	,
	120	,
	121	,
	131	,
	132	,
	130	,
	129	,
	124	,
	126	,
	123	,
	118	,
	127	,
	125	,
	122	,
	128	,
	119	,
	153	,
	154	,
	152	,
	150	,
	156	,
	158	,
	157	,
	155	,
	151	,
	160	,
	112	,
	109	,
	117	,
	111	,
	110	,
	114	,
	113	,
	162	,
	163
	};

	static UINT ElementsCing7[NANIM] = {
		97	,
	95	,
	563	,
	562	,
	72	,
	550	,
	548	,
	75	,
	82	,
	84	,
	83	,
	547	,
	549	,
	555	,
	552	,
	551	,
	553	,
	554	,
	85	,
	79	,
	78	,
	81	,
	74	,
	80	,
	77	,
	73	,
	76	,
	582	,
	580	,
	581	,
	567	,
	568	,
	578	,
	579	,
	577	,
	576	,
	571	,
	573	,
	570	,
	565	,
	574	,
	572	,
	569	,
	575	,
	566	,
	89	,
	90	,
	88	,
	86	,
	92	,
	94	,
	93	,
	91	,
	87	,
	96	,
	559	,
	556	,
	564	,
	558	,
	557	,
	561	,
	560	,
	98	,
	99
	};

	static UINT ElementsCing8[NANIM] = {
		544	,
	542	,
	499	,
	498	,
	519	,
	486	,
	484	,
	522	,
	529	,
	531	,
	530	,
	583	,
	485	,
	491	,
	488	,
	487	,
	489	,
	490	,
	532	,
	526	,
	525	,
	528	,
	521	,
	527	,
	524	,
	520	,
	523	,
	518	,
	516	,
	517	,
	503	,
	504	,
	514	,
	515	,
	513	,
	512	,
	507	,
	509	,
	506	,
	501	,
	510	,
	508	,
	505	,
	511	,
	502	,
	536	,
	537	,
	535	,
	533	,
	539	,
	541	,
	540	,
	538	,
	534	,
	543	,
	495	,
	492	,
	500	,
	494	,
	493	,
	497	,
	496	,
	545	,
	546

	};
	static UINT ElementsCing[NANIM][NCING];
	for (int i = 0; i < NANIM; i++)
	{
		ElementsCing[i][0] = ElementsCing1[i];
		ElementsCing[i][1] = ElementsCing2[i];
		ElementsCing[i][2] = ElementsCing3[i];
		ElementsCing[i][3] = ElementsCing4[i];
		ElementsCing[i][4] = ElementsCing5[i];
		ElementsCing[i][5] = ElementsCing6[i];
		ElementsCing[i][6] = ElementsCing7[i];
		ElementsCing[i][7] = ElementsCing8[i];
	}


	static double Beginnings[NANIM] = {
0	,
0.014320309	,
0.028640619	,
0.042960928	,
0.057281237	,
0.071601546	,
0.085921856	,
0.100242165	,
0.114562474	,
0.130126531	,
0.14444684	,
0.15876715	,
0.173087459	,
0.187407768	,
0.201728078	,
0.216048387	,
0.230368696	,
0.244689005	,
0.259009315	,
0.273664093	,
0.288318871	,
0.302973649	,
0.317628427	,
0.332283205	,
0.346937983	,
0.361592761	,
0.376247539	,
0.393503511	,
0.410759484	,
0.428015457	,
0.44527143	,
0.462527402	,
0.479783375	,
0.497039348	,
0.51429532	,
0.531551293	,
0.548807266	,
0.566063238	,
0.583319211	,
0.600575184	,
0.617831157	,
0.635087129	,
0.652343102	,
0.669599075	,
0.686855047	,
0.70411102	,
0.721366993	,
0.738140264	,
0.754913534	,
0.771686805	,
0.788460076	,
0.805233347	,
0.822006618	,
0.838779889	,
0.85555316	,
0.869873469	,
0.884193778	,
0.898514087	,
0.912834397	,
0.927154706	,
0.941475015	,
0.955795325	,
0.970115634	,
0.984435943


	};




	//static UINT BloccoRuota1[21]={62,658,657,40,39,54,53,595,9,596,71,597,70,41,38,656,659,55,598,56,8};
	//Rotate[8]=new MGROUP_ROTATE(0,MegaBlocco1,149,_V(17.248863,2.77175,20.169667),_V(0,1,0),(float)-45*RAD);


	for (int j = 0; j < NCING; j++)
	{
		for (int i = 0; i < NANIM; i++)
		{
			StateCing[i][j] = Beginnings[i];
			Initials[i][j] = Beginnings[i];
		}
	}

	for (int j = 0; j < NCING; j++)
	{
		for (int i = 0; i < NANIM; i++)
		{
			if (j < 4)
			{
				Rotate1 = _V(0, 3.48, 22.71);
				Rotate2 = _V(0, 1.66, 26.73);
				Rotate3 = _V(0, 1.66, 14.68);
				Rotate4 = _V(0, 3.48, 18.71);
			}
			else {
				double deltaZ = 41.919094;
				Rotate1 = _V(0, 3.48, 22.71 - deltaZ);
				Rotate2 = _V(0, 1.66, 26.73 - deltaZ);
				Rotate3 = _V(0, 1.66, 14.68 - deltaZ);
				Rotate4 = _V(0, 3.48, 18.71 - deltaZ);
			}

			Tr1[i][j] = new MGROUP_TRANSLATE(0, &ElementsCing[i][j], 1, Translate1);
			Rot1[i][j] = new MGROUP_ROTATE(0, &ElementsCing[i][j], 1, Rotate1, _V(1, 0, 0), Angle1);
			Tr2[i][j] = new MGROUP_TRANSLATE(0, &ElementsCing[i][j], 1, Translate2);
			Rot2[i][j] = new MGROUP_ROTATE(0, &ElementsCing[i][j], 1, Rotate2, _V(1, 0, 0), Angle2);
			Tr3[i][j] = new MGROUP_TRANSLATE(0, &ElementsCing[i][j], 1, Translate3);
			Rot3[i][j] = new MGROUP_ROTATE(0, &ElementsCing[i][j], 1, Rotate3, _V(1, 0, 0), Angle3);
			Tr4[i][j] = new MGROUP_TRANSLATE(0, &ElementsCing[i][j], 1, Translate4);
			Rot4[i][j] = new MGROUP_ROTATE(0, &ElementsCing[i][j], 1, Rotate4, _V(1, 0, 0), Angle4);

			animCing[i][j] = CreateAnimation(Beginnings[i]);
			AddAnimationComponent(animCing[i][j], 0, 0.113683, Tr1[i][j]);
			AddAnimationComponent(animCing[i][j], 0.113683, 0.129128, Rot1[i][j]);
			AddAnimationComponent(animCing[i][j], 0.129128, 0.257022, Tr2[i][j]);
			AddAnimationComponent(animCing[i][j], 0.257022, 0.37336, Rot2[i][j]);
			AddAnimationComponent(animCing[i][j], 0.37336, 0.723505, Tr3[i][j]);
			AddAnimationComponent(animCing[i][j], 0.723505, 0.856662, Rot3[i][j]);
			AddAnimationComponent(animCing[i][j], 0.856662, 0.984555, Tr4[i][j]);
			AddAnimationComponent(animCing[i][j], 0.984555, 1, Rot4[i][j]);



		}
	}




	static UINT LifterGrp[64] = { 668,669,666,663,664,661,670,673,675,0,667,665,662,660,674,672,671,1,601,604,600,607,58,628,642,630,629,643,632,633,631,627,611,610,609,608,613,614,615,616,612,636,640,635,634,638,637,622,617,621,626,620,625,619,624,618,623,641,639,59,605,603,602,606 };
	double HeightLifter = 2;
	Lifter = new MGROUP_TRANSLATE(0, LifterGrp, 64, _V(0, HeightLifter, 0));
	AnimLifter = CreateAnimation(0);
	AddAnimationComponent(AnimLifter, 0, 1, Lifter);

	static UINT Pistoni[16] = { 14,15,16,17,11,12,13,25,21,22,23,24,10,18,19,20 };
	Pistons = new MGROUP_SCALE(0, Pistoni, 16, _V(0, 2.77444, 0), _V(1, (2.63016 + HeightLifter) / 2.63016, 1));





	AddAnimationComponent(AnimLifter, 0, 1, Pistons);



	//MGROUP_SCALE (UINT _mesh, UINT *_grp, UINT _ngrp, const VECTOR3 &_ref, const VECTOR3 &_scale)
	return;
}
/*
void MS_Crawler::LoadMeshes()
{
MESHHANDLE ms2=oapiLoadMeshGlobal("SLS\\crawlerBody");
VECTOR3 ofs=_V(0,0,0);
SetMeshVisibilityMode(AddMesh(ms2,&ofs),MESHVIS_ALWAYS);

	VECTOR3 position[8];
	position[0]=_V(21.15,4.7428,18.505656);
for(int i=0;i<NANIM;i++)
{
	Cingolo_mh[i][0]=oapiLoadMeshGlobal("SLS\\Cingolo");
	Cingolo[i][0]=AddMesh(Cingolo_mh[i][0],&position[0]);
}
}*/

void MS_Crawler::clbkSetClassCaps(FILEHANDLE cfg) {
	SetEmptyMass(30000);
	SetClipRadius(0.01);
	double ro = 20;

	SetTouchdownPoints(_V(cos(30 * RAD) * ro, 0.01, -sin(30 * RAD) * ro), _V(0, 0.01, 1 * ro), _V(-cos(30 * RAD) * ro, 0.01, -sin(30 * RAD) * ro));
	msh = oapiLoadMeshGlobal("Multistage2015\\MS_Crawler\\MS_Crawler");
	VECTOR3 ofs = _V(0, 0, 0);
	SetMeshVisibilityMode(AddMesh(msh, &ofs), MESHVIS_ALWAYS);

	CameraOff = _V(-20.56, 8, 25.13);
	SetCameraOffset(CameraOff);
	SetCameraRotationRange(PI, PI, PI, PI);
	SetSurfaceFrictionCoeff(0.1, 0.99);


	FWD = FALSE;
	BWD = FALSE;
	LFT = FALSE;
	RGT = FALSE;

	GoingUp = FALSE;
	GoingDown = FALSE;
	PadDetached = FALSE;

	//Speed=0.833;
	oapiReadItem_float(cfg, const_cast<char*>("Speed"), Speed);
	LengthCrawl = 34.915447;
	rt = 6371010;
	earth_circ = rt * 2 * PI;
	each_deg = earth_circ / 360;
	//AngSpeed=1.5*RAD; //rotation speed, 1.5 deg/s
	oapiReadItem_float(cfg, const_cast<char*>("AngSpeed"), AngSpeed);
	AngSpeed *= RAD;
	//TangentSpd=0.544363;
	double radius = 20.7931;
	TangentSpd = AngSpeed * radius;
	StateLifter = 1;
	SpeedLifter = 0.1;
	//LoadMeshes();

	DefineAnimations();

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


	VECTOR3 dir;
	dir = _V(0, sin(60 * RAD), -cos(60 * RAD));
	const double att = 0.0001;
	lights[0] = AddSpotLight(_V(0, 10, 20), dir, 200, att, att, att, 45 * RAD, 60 * RAD, col_d, col_s, col_a);

	dir = _V(0, sin(60 * RAD), cos(60 * RAD));
	lights[1] = AddSpotLight(_V(0, 10, -20), dir, 200, att, att, att, 45 * RAD, 60 * RAD, col_d, col_s, col_a);

	dir = _V(0, -sin(15 * RAD), cos(15 * RAD));
	lights[2] = AddSpotLight(_V(0, 10, 40), dir, 200, att, att, att, 60 * RAD, 80 * RAD, col_d, col_s, col_a);

	dir = _V(0, -sin(15 * RAD), -cos(15 * RAD));
	lights[3] = AddSpotLight(_V(0, 10, -40), dir, 200, att, att, att, 60 * RAD, 80 * RAD, col_d, col_s, col_a);

	for (int i = 0; i < 4; i++)
	{
		lights[i]->Activate(FALSE);
	}


	oapiReadItem_string(cfg, const_cast<char*>("PAD_MESH"), PadMesh);
	for (UINT i = 0; i < 256; i++)
	{
		PadModule[i] = '\0';
	}
	wCustomPadModule = FALSE;
	oapiReadItem_string(cfg, const_cast<char*>("PAD_MODULE"), PadModule);
	if (PadModule[0] != '\0')
	{
		wCustomPadModule = TRUE;
	}
	if (!strnicmp(PadModule, "NOPAD", 5))
	{
		NoPad = TRUE;
	}
	else { NoPad = FALSE; }
	//RotState=0;
	//firstStep=FALSE;
	return;
}

void MS_Crawler::clbkLoadStateEx(FILEHANDLE scn, void* vs)
{

	char* line;
	while (oapiReadScenario_nextline(scn, line)) {

		if (!strnicmp(line, "LIFTER", 6)) {
			sscanf(line + 6, "%lf", &StateLifter);
		}
		else if (!strnicmp(line, "NOATTPAD", 8)) {
			PadDetached = TRUE;
		}
		else {
			ParseScenarioLineEx(line, vs);
		}
	}

}
//############################################################################//
void MS_Crawler::clbkSaveState(FILEHANDLE scn)
{
	char savebuff[256], savevalbuff[256];
	SaveDefaultState(scn);

	sprintf(savebuff, "LIFTER");
	sprintf(savevalbuff, "%.3f", StateLifter);
	oapiWriteScenario_string(scn, savebuff, savevalbuff);

	if (PadDetached) {
		sprintf(savebuff, "NOATTPAD");
		sprintf(savevalbuff, "\0");
		oapiWriteScenario_string(scn, savebuff, savevalbuff);
	}
}
int MS_Crawler::clbkConsumeBufferedKey(DWORD key, bool down, char* kstate)
{
	if (KEYDOWN(kstate, OAPI_KEY_UP) && (KEYMOD_CONTROL(kstate)) && (KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		if (GoingDown)
		{
			GoingDown = FALSE;
		}
		else {
			GoingUp = TRUE;
		}
		return 1;
	}
	if (KEYDOWN(kstate, OAPI_KEY_DOWN) && (KEYMOD_CONTROL(kstate)) && (KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		if (GoingUp)
		{
			GoingUp = FALSE;
		}
		else {
			GoingDown = TRUE;
		}
		return 1;
	}
	if (KEYDOWN(kstate, OAPI_KEY_G) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		if (oapiIsVessel(hPad))
		{
			VECTOR3 rpos;
			GetRelativePos(hPad, rpos);
			if (length(rpos) < 5) { AttachChild(hPad, AttCrawlerPad, AttPadCrawler); }
		}
		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_D) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		if (StateLifter < 0.1)
		{
			if (GetAttachmentStatus(AttCrawlerPad) != NULL)
			{
				DetachChild(AttCrawlerPad);
				PadDetached = TRUE;
			}
		}
		return 1;
	}


	if (KEYDOWN(kstate, OAPI_KEY_UP) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if ((BWD) || (RGT) || (LFT)) {
			BWD = FALSE;
			FWD = FALSE;
			LFT = FALSE;
			RGT = FALSE;
		}
		else {
			FWD = TRUE;
			BWD = FALSE;
			LFT = FALSE;
			RGT = FALSE;
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_DOWN) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if ((FWD) || (RGT) || (LFT)) {
			BWD = FALSE;
			FWD = FALSE;
			LFT = FALSE;
			RGT = FALSE;
		}
		else {
			FWD = FALSE;
			BWD = TRUE;
			LFT = FALSE;
			RGT = FALSE;
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_LEFT) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if ((RGT) || (FWD) || (BWD)) {
			BWD = FALSE;
			FWD = FALSE;
			LFT = FALSE;
			RGT = FALSE;
		}
		else {
			FWD = FALSE;
			BWD = FALSE;
			LFT = TRUE;
			RGT = FALSE;
		}

		return 1;
	}
	if (KEYDOWN(kstate, OAPI_KEY_RIGHT) && (!KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {

		if ((LFT) || (FWD) || (BWD)) {
			BWD = FALSE;
			FWD = FALSE;
			LFT = FALSE;
			RGT = FALSE;
		}
		else {
			FWD = FALSE;
			BWD = FALSE;
			LFT = FALSE;
			RGT = TRUE;
		}

		return 1;
	}

	if (KEYDOWN(kstate, OAPI_KEY_L) && (KEYMOD_CONTROL(kstate)) && (!KEYMOD_SHIFT(kstate) && (!KEYMOD_ALT(kstate)))) {
		for (int i = 0; i < 4; i++)
		{
			lights[i]->Activate(!lights[i]->IsActive());
		}
		return 1;
	}
	return 0;
}

void MS_Crawler::MoveAround(bool FWD, bool BWD, bool LFT, bool RGT)
{
	if (FWD)
	{

		VESSELSTATUS2 vs2;
		memset(&vs2, 0, sizeof(vs2));
		vs2.version = 2;
		GetStatusEx(&vs2);

		double d_lat = (Speed * oapiGetSimStep() * cos(vs2.surf_hdg)) / each_deg;
		double d_lng = (Speed * oapiGetSimStep() * sin(vs2.surf_hdg)) / each_deg;
		vs2.surf_lat += d_lat * RAD;
		vs2.surf_lng += d_lng * RAD;

		if (oapiGetOrbiterVersion() < 160903)
		{
			FILEHANDLE fh = oapiOpenFile("\\Multistage2015\\__Crawl", FILE_OUT, CONFIG);
			oapiWriteScenario_string(fh, const_cast<char*>("STATUS"), const_cast<char*>("Landed Earth"));
			oapiWriteScenario_float(fh, const_cast<char*>("HEADING"), vs2.surf_hdg * DEG);
			char position[256];
			sprintf(position, "%.20f %.20f", vs2.surf_lng * DEG, vs2.surf_lat * DEG);
			oapiWriteScenario_string(fh, const_cast<char*>("POS"), position);
			oapiCloseFile(fh, FILE_OUT);
			fh = oapiOpenFile("\\Multistage2015\\__Crawl", FILE_IN, CONFIG);
			clbkLoadStateEx(fh, &vs2);
			oapiCloseFile(fh, FILE_IN);
		}
		DefSetStateEx(&vs2);
		return;
	}

	if (BWD) {
		VESSELSTATUS2 vs2;
		memset(&vs2, 0, sizeof(vs2));
		vs2.version = 2;
		GetStatusEx(&vs2);

		double d_lat = (Speed * oapiGetSimStep() * cos(vs2.surf_hdg)) / each_deg;
		double d_lng = (Speed * oapiGetSimStep() * sin(vs2.surf_hdg)) / each_deg;
		vs2.surf_lat -= d_lat * RAD;
		vs2.surf_lng -= d_lng * RAD;

		if (oapiGetOrbiterVersion() < 160903)
		{
			FILEHANDLE fh = oapiOpenFile("\\Multistage2015\\__Crawl", FILE_OUT, CONFIG);
			oapiWriteScenario_string(fh, const_cast<char*>("STATUS"), const_cast<char*>("Landed Earth"));
			oapiWriteScenario_float(fh, const_cast<char*>("HEADING"), vs2.surf_hdg * DEG);
			char position[256];
			sprintf(position, "%.20f %.20f", vs2.surf_lng * DEG, vs2.surf_lat * DEG);
			oapiWriteScenario_string(fh, const_cast<char*>("POS"), position);
			oapiCloseFile(fh, FILE_OUT);
			fh = oapiOpenFile("\\Multistage2015\\__Crawl", FILE_IN, CONFIG);
			clbkLoadStateEx(fh, &vs2);
			oapiCloseFile(fh, FILE_IN);
		}
		DefSetStateEx(&vs2);
		return;
	}

	if (LFT)
	{
		VESSELSTATUS2 vs2;
		memset(&vs2, 0, sizeof(vs2));
		vs2.version = 2;
		GetStatusEx(&vs2);
		double d_hdg = AngSpeed * oapiGetSimStep();
		vs2.surf_hdg -= d_hdg;

		if (vs2.surf_hdg < 0) { vs2.surf_hdg += 2 * PI; }



		if (oapiGetOrbiterVersion() < 160903)
		{
			FILEHANDLE fh = oapiOpenFile("\\Multistage2015\\__Crawl", FILE_OUT, CONFIG);
			oapiWriteScenario_string(fh, const_cast<char*>("STATUS"), const_cast<char*>("Landed Earth"));
			oapiWriteScenario_float(fh, const_cast<char*>("HEADING"), vs2.surf_hdg * DEG);
			char position[256];
			sprintf(position, "%.20f %.20f", vs2.surf_lng * DEG, vs2.surf_lat * DEG);
			oapiWriteScenario_string(fh, const_cast<char*>("POS"), position);
			oapiCloseFile(fh, FILE_OUT);
			fh = oapiOpenFile("\\Multistage2015\\__Crawl", FILE_IN, CONFIG);
			clbkLoadStateEx(fh, &vs2);
			oapiCloseFile(fh, FILE_IN);
		}
		DefSetStateEx(&vs2);
		return;
	}


	if (RGT)
	{
		VESSELSTATUS2 vs2;
		memset(&vs2, 0, sizeof(vs2));
		vs2.version = 2;
		GetStatusEx(&vs2);
		double d_hdg = AngSpeed * oapiGetSimStep();
		vs2.surf_hdg += d_hdg;

		if (vs2.surf_hdg > 2 * PI) { vs2.surf_hdg -= 2 * PI; }



		if (oapiGetOrbiterVersion() < 160903)
		{
			FILEHANDLE fh = oapiOpenFile("\\Multistage2015\\__Crawl", FILE_OUT, CONFIG);
			oapiWriteScenario_string(fh, const_cast<char*>("STATUS"), const_cast<char*>("Landed Earth"));
			oapiWriteScenario_float(fh, const_cast<char*>("HEADING"), vs2.surf_hdg * DEG);
			char position[256];
			sprintf(position, "%.20f %.20f", vs2.surf_lng * DEG, vs2.surf_lat * DEG);
			oapiWriteScenario_string(fh, const_cast<char*>("POS"), position);
			oapiCloseFile(fh, FILE_OUT);
			fh = oapiOpenFile("\\Multistage2015\\__Crawl", FILE_IN, CONFIG);
			clbkLoadStateEx(fh, &vs2);
			oapiCloseFile(fh, FILE_IN);
		}
		DefSetStateEx(&vs2);
		return;
	}
	return;
}
void MS_Crawler::UpdateAnimations(bool Reset = FALSE)
{
	if (!Reset)
	{
		double d_anim = 0;
		if ((FWD) || (BWD))
		{
			d_anim = (Speed / LengthCrawl) * oapiGetSimStep();
		}
		else if ((LFT) || (RGT)) {
			d_anim = (TangentSpd / LengthCrawl) * oapiGetSimStep();
		}
		d_anim = d_anim / oapiGetTimeAcceleration();
		//d_anim=0.0001;
		if (FWD)
		{
			for (int j = 0; j < NCING; j++)
			{
				//StateCing[0][j]+=d_anim;
				for (int i = 0; i < NANIM; i++)
				{
					StateCing[i][j] += d_anim;
					//StateCing[i][j]+=StateCing[0][j]+(Initials[i][j]);
				}
			}
		}
		if (BWD)
		{
			for (int j = 0; j < NCING; j++)
			{
				for (int i = 0; i < NANIM; i++)
				{
					StateCing[i][j] -= d_anim;
				}
			}
		}

		if (LFT) {
			for (int j = 0; j < 2; j++)
			{
				for (int i = 0; i < NANIM; i++)
				{
					StateCing[i][j] += d_anim;
					StateCing[i][j + 4] += d_anim;
				}
			}


			for (int j = 2; j < 4; j++)
			{
				for (int i = 0; i < NANIM; i++)
				{
					StateCing[i][j] -= d_anim;
					StateCing[i][j + 4] -= d_anim;
				}
			}
		}

		if (RGT) {
			for (int j = 0; j < 2; j++)
			{
				for (int i = 0; i < NANIM; i++)
				{
					StateCing[i][j] -= d_anim;
					StateCing[i][j + 4] -= d_anim;
				}
			}

			for (int j = 2; j < 4; j++)
			{
				for (int i = 0; i < NANIM; i++)
				{
					StateCing[i][j] += d_anim;
					StateCing[i][j + 4] += d_anim;
				}
			}
		}


		/*
		if((StateCing[0][0]>1)&&(FWD))
		{
			for(int i=0;i<NANIM;i++)
			{
				StateCing[i][0]=Initials[i];
			}
		}else if((StateCing[0][0]<0)&&(BWD))
		{
			for(int i=0;i<NANIM;i++)
			{
				StateCing[i][0]=1-Initials[i];
			}
		}
		*/


		for (int j = 0; j < NCING; j++)
		{
			for (int i = 0; i < NANIM; i++)
			{
				if (StateCing[i][j] > 1) { StateCing[i][j] = 0; }
				if (StateCing[i][j] < 0) { StateCing[i][j] = 1; }

			}
		}
	}
	else {
		for (int j = 0; j < NCING; j++)
		{
			for (int i = 0; i < NANIM; i++)
			{
				StateCing[i][j] = Initials[i][j];
			}
		}
	}


	for (int j = 0; j < NCING; j++)
	{
		for (int i = 0; i < NANIM; i++)
		{
			SetAnimation(animCing[i][j], StateCing[i][j]);
		}
	}


	//sprintf(oapiDebugString(),"STATE CING[0][0]=%.3f STATE CING[32][0]=%.3f, Delta=%.6f d_Anim:%.6f",StateCing[0][0],StateCing[32][0],StateCing[32][0]-StateCing[0][0],d_anim);
	return;
}

void MS_Crawler::UpdateLifter()
{
	if (GoingUp)
	{
		StateLifter += SpeedLifter * oapiGetSimStep();


		if (StateLifter > 1) { StateLifter = 1; GoingUp = FALSE; }
	}
	else if (GoingDown) {
		StateLifter -= SpeedLifter * oapiGetSimStep();

		if (StateLifter < 0) { StateLifter = 0; GoingDown = FALSE; }
	}
	CameraOff.y = 8 + 2 * StateLifter;
	SetCameraOffset(CameraOff);
	SetAnimation(AnimLifter, StateLifter);
	VECTOR3 attpos = _V(0, 9.405 + 2 * StateLifter, 0);
	VECTOR3 dir, rot, pos;
	if (AttCrawlerPad) {
		GetAttachmentParams(AttCrawlerPad, pos, dir, rot);
		SetAttachmentParams(AttCrawlerPad, attpos, dir, rot);
	}
	return;
}

void MS_Crawler::clbkPreStep(double simt, double simdt, double mjd)
{
	//	RotState+=0.01*oapiGetSimStep();
	//	SetAnimation(AnimRotat,RotState);

	/*if(!firstStep){
		for(int i=0;i<NANIM;i++)
		{
			states[i]=Initials[i];
			SetAnimation(AnimCing[i][0],Initials[i]);
		}
		firstStep=TRUE;
	}

	for(int i=0;i<NANIM;i++)
		{
			states[i]+=0.1*oapiGetSimStep();
			if(states[i]>1)states[i]=0;
			SetAnimation(AnimCing[i][0],states[i]);
		}
		*/
	MoveAround(FWD, BWD, LFT, RGT);
	if ((FWD) || (BWD) || (LFT) || (RGT))
	{
		if (oapiGetTimeAcceleration() < 10) {
			UpdateAnimations(FALSE);
		}
		else {
			UpdateAnimations(TRUE);
		}
	}
	if ((GoingUp) || (GoingDown))
	{
		UpdateLifter();
	}




	/*
	OBJHANDLE Hvessel;
	Hvessel=oapiGetVesselByName("SLS");

	if(Hvessel){
	VESSEL *v;
	v = oapiGetVesselInterface(Hvessel);


	int testversion=v->Version();

	if(testversion>=2){
		int test=((VESSEL3*)v)->clbkGeneric(2015,1,0);
		int test2=((VESSEL3*)v)->clbkGeneric(2015,2,0);
		sprintf(oapiDebugString(),"MET: %i AutopilotStatus:%i",test,test2);
	}
	}
	*/





}


int MS_Crawler::clbkConsumeDirectKey(char* kstate)
{



	return 0;
}



void MS_Crawler::clbkPostCreation() {
	hPad = oapiGetVesselByName(const_cast<char*>("MS_Pad"));
	if (!NoPad)
	{
		memset(&vsPad, 0, sizeof(vsPad));
		vsPad.version = 2;
		GetStatusEx(&vsPad);


		if (!oapiIsVessel(hPad))
		{
			if (!wCustomPadModule)
			{
				hPad = oapiCreateVesselEx("MS_Pad", "EmptyModule", &vsPad);
			}
			else {
				hPad = oapiCreateVesselEx("MS_Pad", PadModule, &vsPad);
			}
		}


		vPad = (VESSEL3*)oapiGetVesselInterface(hPad);
		VECTOR3 ofs = _V(0, 0, 0);

		if (!wCustomPadModule)
		{
			MESHHANDLE msPad = oapiLoadMeshGlobal(PadMesh);
			UINT msh_idx = vPad->AddMesh(msPad, &ofs);
			vPad->SetMeshVisibilityMode(msh_idx, MESHVIS_ALWAYS);
			vPad->SetCameraOffset(_V(0, 120, 20));
			vPad->SetEmptyMass(30000);
			vPad->SetClipRadius(0.01);
			double ro = 20;
			vPad->SetTouchdownPoints(_V(cos(30 * RAD) * ro, 0, -sin(30 * RAD) * ro), _V(0, 0, 1 * ro), _V(-cos(30 * RAD) * ro, 0, -sin(30 * RAD) * ro));
		}

		AttPadCrawler = vPad->CreateAttachment(TRUE, _V(0, 9.405, 0), _V(0, -1, 0), _V(0, 0, 1), "PadCrawl", FALSE);
		AttCrawlerPad = CreateAttachment(FALSE, _V(0, 9.4050, 0), _V(0, 1, 0), _V(0, 0, 1), "PadCrawl", FALSE);
		if (!PadDetached) { AttachChild(hPad, AttCrawlerPad, AttPadCrawler); }

	}

	UpdateLifter();

	return;
}





/*
void Crawler::clbkSetStateEx (const void *status)
{

	return;
}*/
//Creation
DLLCLBK void InitModule(HINSTANCE hModule) {}
DLLCLBK void ExitModule(HINSTANCE hModule) {}
DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel, int flightmodel) { return new MS_Crawler(hvessel, flightmodel); }
DLLCLBK void ovcExit(VESSEL* vessel) { if (vessel)delete(MS_Crawler*)vessel; }

