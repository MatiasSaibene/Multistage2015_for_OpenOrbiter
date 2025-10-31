// ==============================================================
//                 ORBITER MODULE: FlightData
//                  Part of the ORBITER SDK
//            Copyright (C) 2003 Martin Schweiger
//                   All rights reserved
//
// Graph.h
// Generic data graph class interface.
// ==============================================================

#ifndef __GRAPH_H
#define __GRAPH_H

#include "windows.h"

const int MAXPLOT = 3;
//const int NDATA = 200;

struct GDIres {
	HFONT font[2];
	HPEN  pen[2 + MAXPLOT];
	int nref;
};



class Graph {
public:
	Graph();
	~Graph();
	static void InitGDI();
	static void FreeGDI();

	void AppendDataXY(POINT point);
	void Refresh(HDC hDC, int w, int h);
	POINT points[10];
	POINT original_points[10];
	void ClearPoints();


private:
	static GDIres gdi;

	int idpt;

};


#endif // !__GRAPH_H