#ifndef _IFS_
#define _IFS_

class C_IFS
{
public:

	unsigned int SierpinskiTriangleIter, TriAnglesIter, PentagonIter;

	C_IFS()
	{
		SierpinskiTriangleIter = 10000;
		TriAnglesIter = 25000;
		PentagonIter = 50000;
		IFSList = NULL;		// Allows resources to re-allocate without problems, spent ~1.5hrs T/Sing this...
	};
	~C_IFS(){};

	struct RGBquad
	{
		int r, g, b, a;
	};

	struct Vertex
	{
		float x, y;	// Position of vertex in 3D space
		RGBquad color;	// Color of vertex
	}*IFSList;

	void SierpinskiTriangleCoords();
	void TriAnglesCoords();
	void PentagonCoords();
	void CoordsGraph(int equationName);
};

#endif