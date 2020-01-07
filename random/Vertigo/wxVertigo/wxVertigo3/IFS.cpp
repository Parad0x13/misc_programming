#include "wx/glcanvas.h"
#include <malloc.h>
#include "rand.h"
#include "IFS.h"
#include "enum.h"

void C_IFS::CoordsGraph(int equationName)
{
	unsigned int iterations;
	switch(equationName)
	{
	case ID_SierpinskiTriangle:
		iterations = this->SierpinskiTriangleIter;
		break;
	case ID_TriAngles:
		iterations = this->TriAnglesIter;
		break;
	case ID_Pentagon:
		iterations = this->PentagonIter;
		break;
	default:
		break;
	}

	glPushMatrix();
	glBegin(GL_POINTS);

	for(unsigned int d=0;d<iterations;d++)
	{
		glColor3ub(IFSList[d].color.r, IFSList[d].color.g, IFSList[d].color.b);	
		glVertex2f(IFSList[d].x, IFSList[d].y);
	}

	glEnd();
	glPopMatrix();
}

void C_IFS::SierpinskiTriangleCoords()
{
	delete this->IFSList;
	this->IFSList = new C_IFS::Vertex[this->SierpinskiTriangleIter * sizeof(C_IFS::Vertex)];
	if(this->IFSList == NULL) exit(1);

	float pax=0, pay=-1, pbx=-1, pby=1, pcx=1, pcy=1, px=0, py=0;
	int pick;
	static int skip=0;

	for(unsigned int d=0;d<this->SierpinskiTriangleIter;d++)
	{
		pick=rnd(3);
		if(pick==0)
		{
			px=(px-pax)/2;
			py=(py-pay)/2;
		}
		if(pick==1)
		{
			px=(px-pbx)/2;
			py=(py-pby)/2;
		}
		if(pick==2)
		{
			px=(px-pcx)/2;
			py=(py-pcy)/2;
		}
		if(skip>10)
		{
			this->IFSList[d].x = px;
			this->IFSList[d].y = py;
			switch(pick)        // This coloring algorithm does something compleatly unexpected!
			{
			case 0:
				this->IFSList[d].color.r = 255;
				this->IFSList[d].color.g = 0;
				this->IFSList[d].color.b = 0;
				this->IFSList[d].color.a = 255;
				break;
			case 1:
				this->IFSList[d].color.r = 0;
				this->IFSList[d].color.g = 255;
				this->IFSList[d].color.b = 0;
				this->IFSList[d].color.a = 255;
				break;
			case 2:
				this->IFSList[d].color.r = 0;
				this->IFSList[d].color.g = 0;
				this->IFSList[d].color.b = 255;
				this->IFSList[d].color.a = 255;
				break;
			default:
				break;
			}
		}
		skip++;
	}
}

void C_IFS::TriAnglesCoords()
{
	delete IFSList;
	IFSList = new C_IFS::Vertex[C_IFS::TriAnglesIter * sizeof(C_IFS::Vertex)];
	if(IFSList == NULL) exit(1);

	float pax=0, pay=1, pbx=-1, pby=-1, pcx=1, pcy=-1, px=0, py=0;
	int pick=0;
	static int skip=0;

	for(unsigned int d=0;d<C_IFS::TriAnglesIter;d++)
	{
		pick=rnd(3);
		if(pick==0)
		{
			px=(pax-px)/2;
			py=(pay-py)/2;
		}
		if(pick==1)
		{
			px=(pbx-px)/2;
			py=(pby-py)/2;
		}
		if(pick==2)
		{
			px=(pcx-px)/2;
			py=(pcy-py)/2;
		}
		if(skip>10)
		{
			IFSList[d].x = px;
			IFSList[d].y = py;
			switch(pick)        // This coloring algorithm does something compleatly unexpected!
			{
				case 0:
					IFSList[d].color.r = 255;
					IFSList[d].color.g = 0;
					IFSList[d].color.b = 0;
					IFSList[d].color.a = 255;
					break;
				case 1:
					IFSList[d].color.r = 0;
					IFSList[d].color.g = 255;
					IFSList[d].color.b = 0;
					IFSList[d].color.a = 255;
					break;
				case 2:
					IFSList[d].color.r = 0;
					IFSList[d].color.g = 0;
					IFSList[d].color.b = 255;
					IFSList[d].color.a = 255;
					break;
				case 3:
					IFSList[d].color.r = 255;
					IFSList[d].color.g = 255;
					IFSList[d].color.b = 0;
					IFSList[d].color.a = 255;
					break;
				default:
					break;
			}
		}
		skip++;
	}
}

void C_IFS::PentagonCoords()
{
	delete IFSList;
	IFSList = new C_IFS::Vertex[C_IFS::PentagonIter * sizeof(C_IFS::Vertex)];
	if(IFSList == NULL) exit(1);

	float pax=0.0f, pay=-1.0f, pbx=1.0f, pby=-0.309f, pcx=0.588f, pcy=1.0f, pdx=-0.588f, pdy=1.0f, pex=-1.0f, pey=-0.309f, px=0, py=0;		// Bleh, had to use Blender to find the vertex coordinates
	int pick;
	static int skip;

	for(unsigned int d=0;d<C_IFS::PentagonIter;d++)
	{
		pick=rnd(5);
		if(pick==0)
		{
			px=(px-pax)/2;
			py=(py-pay)/2;
		}
		if(pick==1)
		{
			px=(px-pbx)/2;
			py=(py-pby)/2;
		}
		if(pick==2)
		{
			px=(px-pcx)/2;
			py=(py-pcy)/2;
		}
		if(pick==3)
		{
			px=(px-pdx)/2;
			py=(py-pdy)/2;
		}
		if(pick==4)
		{
			px=(px-pex)/2;
			py=(py-pey)/2;
		}
		if(skip>10)
		{
			IFSList[d].x = px;
			IFSList[d].y = py;
			switch(pick)        // This coloring algorithm does something compleatly unexpected!
			{
				case 0:
					IFSList[d].color.r = 255;
					IFSList[d].color.g = 0;
					IFSList[d].color.b = 0;
					IFSList[d].color.a = 255;
					break;
				case 1:
					IFSList[d].color.r = 0;
					IFSList[d].color.g = 255;
					IFSList[d].color.b = 0;
					IFSList[d].color.a = 255;
					break;
				case 2:
					IFSList[d].color.r = 0;
					IFSList[d].color.g = 0;
					IFSList[d].color.b = 255;
					IFSList[d].color.a = 255;
					break;
				case 3:
					IFSList[d].color.r = 255;
					IFSList[d].color.g = 255;
					IFSList[d].color.b = 255;
					IFSList[d].color.a = 255;
					break;
				case 4:
					IFSList[d].color.r = 255;
					IFSList[d].color.g = 255;
					IFSList[d].color.b = 0;
					IFSList[d].color.a = 255;
					break;
				default:
					break;
			}
		}
		skip++;
	}
}