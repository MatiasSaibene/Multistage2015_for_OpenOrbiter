// ==============================================================
//                 ORBITER MODULE: FlightData
//                  Part of the ORBITER SDK
//            Copyright (C) 2003 Martin Schweiger
//                   All rights reserved
//
// Graph.cpp
// Generic data graph class implementation.
// ==============================================================

#define STRICT
#include "Graph.h"
#include <stdio.h>
#include <math.h>
#include <strstream>
#include "orbitersdk.h"

using namespace std;

static COLORREF plotcol[MAXPLOT] = {0x0000ff, 0xff0000, 0x00ff00};

Graph::Graph ()
{
	
	idpt=0;
	for(UINT i=0;i<10;i++)
	{
		points[i].x=0;
		points[i].y=0;
	}
}

Graph::~Graph()
{
	
}

void Graph::InitGDI ()
{
	if (!gdi.nref) {
	 	gdi.font[0] = CreateFont (-10, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 49, "Arial");
		gdi.font[1] = CreateFont (-10, 0, 900, 900, 400, 0, 0, 0, 0, 3, 2, 1, 49, "Arial");
		gdi.pen[0]  = CreatePen (PS_SOLID, 2, 0x0000FF);
		gdi.pen[1]  = CreatePen (PS_SOLID, 0, 0xD0D0D0);
		for (int i = 0; i < MAXPLOT; i++)
			gdi.pen[2+i]  = CreatePen (PS_SOLID, 1, plotcol[i]);
	}
	gdi.nref++;
}

void Graph::FreeGDI ()
{
	gdi.nref--;
	if (!gdi.nref) {
		DWORD i;
		for (i = 0; i < 2; i++) DeleteObject (gdi.font[i]);
		for (i = 0; i < 2+MAXPLOT; i++) DeleteObject (gdi.pen[i]);
	}
}



void Graph::AppendDataXY(POINT point)
{
	if(idpt<10)
	{
		points[idpt]=point;
		idpt++;
	}
}


void Graph::ClearPoints()
{
	for(UINT i=0;i<10;i++)
	{
		points[i].x=0;
		points[i].y=0;
		original_points[i].x=0;
		original_points[i].y=0;
	}
	idpt=0;
}
void Graph::Refresh (HDC hDC, int w, int h)
{
	int    x0 = 0,   x1 = w;
	int  y0 = h, y1 = 0;
	
	double topmargin=15;
	double bottommargin=5;
	double rightmargin=15;
	HFONT pfont = (HFONT)SelectObject (hDC, gdi.font[0]);

	for(UINT i=0;i<10;i++)
	{
		if(points[i].x<0){points[i].x=0;}
		if(points[i].y<0){points[i].y=0;}
		original_points[i]=points[i];
		//oapiWriteLogV("original_points[%i].x:%i .y:%i",i,original_points[i].x,original_points[i].y);
	}

		for(UINT idrem=idpt;idrem<10;idrem++)
		{
			points[idrem].x=points[idpt].x;
			points[idrem].y=0;
		}
		// draw data
	
			SelectObject (hDC, gdi.pen[0]);
	
			float MaxX=0;
			for(UINT i=0;i<10;i++)
			{
					if(points[i].x>MaxX){MaxX=points[i].x;}
			}
			double cnvx=(w-rightmargin)/(double)MaxX;
			
			float MaxY=0;
			for(UINT i=0;i<10;i++)
			{
					if(points[i].y>MaxY){MaxY=points[i].y;}
			}
			double cnvy=(h-topmargin-bottommargin)/(double)100;//(int)MaxY;
			
			

			for(UINT i=0;i<10;i++)
			{
				points[i].x*=cnvx;
				points[i].y*=cnvy;
			//	oapiWriteLogV("i:%i x:%i y:%i h:%i cnvy:%.3f",i,points[i].x,points[i].y,h,cnvy);
				
			}
			for(UINT i=0;i<10;i++)
			{				
				points[i].y=(h)-points[i].y-bottommargin;
			}
			
			Polyline(hDC,points,idpt);
			for(UINT i=0;i<idpt;i++)
			{
				char buff[28];
				int len=sprintf_s(buff,"t:%i P:%i%%",original_points[i].x,original_points[i].y);
				TextOut(hDC,points[i].x,points[i].y-15,buff,len);
			}

	// Draw axes
	SelectObject (hDC, GetStockObject (BLACK_PEN));
	MoveToEx (hDC, x0, y1, NULL);
	LineTo (hDC, x0, y0); MoveToEx(hDC,x0,y0-bottommargin,NULL);LineTo (hDC, x1, y0-bottommargin);
	


	SelectObject (hDC, pfont);
}



GDIres Graph::gdi = {0,0,0};