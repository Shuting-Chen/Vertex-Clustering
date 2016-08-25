// cssample5View.cpp : implementation of the CCssample5View class
//

#include "stdafx.h"
#include <fstream> //get something need to be used
#include <string>

#include "cssample5.h"

#include "cssample5Doc.h"
#include "cssample5View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCssample5View

IMPLEMENT_DYNCREATE(CCssample5View, CView)

BEGIN_MESSAGE_MAP(CCssample5View, CView)
	//{{AFX_MSG_MAP(CCssample5View)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_AS4_SELECT_1, OnSelectControlPoint1)
	ON_COMMAND(ID_AS4_SELECT_2, OnSelectControlPoint2)
	ON_COMMAND(ID_AS4_SELECT_3, OnSelectControlPoint3)
	ON_COMMAND(ID_AS4_SELECT_4, OnSelectControlPoint4)
	ON_COMMAND(ID_AS4_X_DEC, OnXDec)
	ON_COMMAND(ID_AS4_X_INC, OnXInc)
	ON_COMMAND(ID_AS4_Y_DEC, OnYDec)
	ON_COMMAND(ID_AS4_Y_INC, OnYInc)
	ON_COMMAND(ID_AS4_Z_DEC, OnZDec)
	ON_COMMAND(ID_AS4_Z_INC, OnZInc)
	ON_COMMAND(ID_VIEW_FILLED, OnViewFilled)
	ON_COMMAND(ID_VIEW_WIREFRAME, OnViewWireframe)
	ON_COMMAND(ID_VIEW_FLAT, OnViewFlat)
	ON_COMMAND(ID_VIEW_SMOOTH, OnViewSmooth)
	ON_COMMAND(ID_VIEW_LIGHT_YDEC, OnViewLightYdec)
	ON_COMMAND(ID_VIEW_LIGHT_YINC, OnViewLightYinc)
	ON_COMMAND(ID_VIEW_TEXTURE_DISABLE, OnViewTextureDisable)
	ON_COMMAND(ID_VIEW_TEXTURE_ENABLE, OnViewTextureEnable)
	ON_COMMAND(ID_VIEW_POS_DECZ, OnViewPosDecz)
	ON_COMMAND(ID_VIEW_POS_INCZ, OnViewPosIncz)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCssample5View construction/destruction

CCssample5View::CCssample5View()
{
	m_hDC=NULL;
	m_hRC=NULL;
	w=h=0; //width and height of view area
	control_point=0;
	x_step=0;
	y_step=0;
	z_step=0;
	lineNum = 0;
	facesNum = 0;
	xMax = 0.f, xMin = 0.f, yMax = 0.f, yMin = 1.f, zMax = 0.f, zMin = 0.f;
	RedSurface[0]= 1.0f; RedSurface[1]=0.0f; RedSurface[2]=0.0f; RedSurface[3]=1.0f;
	GreenSurface[0]= 0.0f; GreenSurface[1]=1.0f; GreenSurface[2]=0.0f; GreenSurface[3]=1.0f;
	BlueSurface[0]= 0.0f; BlueSurface[1]=0.0f; BlueSurface[2]=1.0f; BlueSurface[3]=1.0f;
	BlackSurface[0]= 0.0f; BlackSurface[1]=0.0f; BlackSurface[2]=0.0f; BlackSurface[3]=1.0f;

	u.x=1.0; u.y=0.0; u.z=0.0;
	v.x=0.0; v.y=1.0; v.z=0.0;
	n.x=0.0; n.y=0.0; n.z=-1.0;
	camera.x = 0.0; camera.y=0.0; camera.z=40.0;
    for(int i=0; i< 4; i++)
    for(int j=0; j< 4; j++) {
		   p[i][j].x = 20.f*j;
		   p[i][j].z = 20.f*i;
		   p[i][j].y = 0.f;
	}

	LightPos.x=50;	LightPos.y=50; 	LightPos.z=50;
	Camera.x=45; Camera.y=45; Camera.z=45;
	WireFrame=1;
	rendering_mode=true; //smooth by default, otherwise flat
	CalcBezier();

	//for(int i = 0; i < 80994; i++)	hashStores1.push_back(std::vector<vetexData>());
	//for (int i = 0; i < 10124 ; i++)  hashStores2.push_back(std::vector<vetexData>());
	
	

}

CCssample5View::~CCssample5View()
{
}

BOOL CCssample5View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCssample5View drawing

void CCssample5View::findObj()
{
	std::ifstream file1("teapot.obj");  //basic string, all kinds of strings.

	std::string line;

	vetexData myVetices;
	float v1, v2, v3;

	int line1, line2, line3;
	while (std::getline(file1, line))//just get a line, it if is v, then ..if it is not, then just jump out. 
	{
		if (line.substr(0, 1) == "v")
		{
			sscanf(line.c_str(), "v %f %f %f", &v1, &v2, &v3);
			
			storeVertices.push_back(vetexData(v1*30, v2*30, v3*30));
			lineNum++;

		}

		if (line.substr(0, 1) == "f")
		{
			sscanf(line.c_str(), "f %d %d %d", &line1, &line2, &line3);
		
			storeFaces.push_back(faceData(line1, line2, line3));
			facesNum++;
		}
	}
	lineNum = lineNum - 1;

	facesNum = facesNum - 1;


}

void CCssample5View::findMaxMin()
{
	float x, y, z;

	for (int i = 0; i < lineNum; i++)
	{
		//get all the things done. 
		x = storeVertices[i].xyz.x;
		y = storeVertices[i].xyz.y;
		z = storeVertices[i].xyz.z;
		if (x > xMax)
		{
			xMax = x;
		}
		if (x < xMin)
		{
			xMin = x;
		}
		if (y > yMax)
		{
			yMax = y;
		}
		if (y < yMin)
		{
			yMin = y;
		}
		if (z > zMax)
		{
			zMax = z;
		}
		if (z < zMin)
		{
			zMin = z;
		}
	}


}


void CCssample5View::hashVertices()
{
	//give a for loop,then let the things to be done, then everything will be at right place. Use the right numbers of the cell to be at the 
	//then push back the corresponding vertices, then go over all vertices again to cluster one cell's vertices into one vertex with the most edges. 
	//Then get everything done. 
	int cellNum,sizeNum;
	float x, y, z;
	int indexX, indexY, indexZ;


	for (int i = 0; i < lineNum; i++)
	{
		x = storeVertices[i].xyz.x;
		y = storeVertices[i].xyz.y;
		z = storeVertices[i].xyz.z;


		
		indexX = (x - xMin) / 9;
		indexY = (y - yMin) / 9;
		indexZ = (z - zMin) / 9;
		//hashStores3[cellNum].push_back(vetexData(x, y, z));
		if (storeVertices[i].edgeNum > hashStores3[indexX][indexY][indexZ].edgeNum)
		{
			hashStores3[indexX][indexY][indexZ] = storeVertices[i];
		}
		//The assignment operator has been loaded.
			
		indexX = (x - xMin) / 6;
		indexY = (y - yMin) / 6;
		indexZ = (z - zMin) / 6;
		//hashStores3[cellNum].push_back(vetexData(x, y, z));
		if (storeVertices[i].edgeNum > hashStores2[indexX][indexY][indexZ].edgeNum)
		{
			hashStores2[indexX][indexY][indexZ] = storeVertices[i];
		}

		indexX = (x - xMin) / 3;
		indexY = (y - yMin) / 3;
		indexZ = (z - zMin) / 3;
		//hashStores3[cellNum].push_back(vetexData(x, y, z));
		if (storeVertices[i].edgeNum > hashStores1[indexX][indexY][indexZ].edgeNum)
		{
			hashStores1[indexX][indexY][indexZ] = storeVertices[i];
		}
	}
}


void CCssample5View::loadObj() 
{
	int line1, line2, line3;
	for (int m = 0; m < facesNum; m++)
	{
		line1 = storeFaces[m].v1line;
		line2 = storeFaces[m].v2line;
		line3 = storeFaces[m].v3line;
		float v1x = storeVertices[line1 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v1y = storeVertices[line1 - 1].xyz.y;   //get some corresponding vertices. 
		float v1z = storeVertices[line1 - 1].xyz.z;   //All of the things just come from the coordinates.
		storeVertices[line1 - 1].edgeNum++;

		float v2x = storeVertices[line2 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v2y = storeVertices[line2 - 1].xyz.y;
		float v2z = storeVertices[line2 - 1].xyz.z;
		storeVertices[line2 - 1].edgeNum++;

		float v3x = storeVertices[line3 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v3y = storeVertices[line3 - 1].xyz.y;
		float v3z = storeVertices[line3 - 1].xyz.z;
		storeVertices[line3 - 1].edgeNum++;
		  //get every vertices' edge added.

		glColor4f(1.f, 1.f, 1.f, 1.f);
		glBegin(GL_LINES);
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v2x, v2y, v2z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v3x, v3y, v3z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v2x, v2y, v2z);
		glVertex3f(v3x, v3y, v3z);
		glEnd();
	}
}



void CCssample5View::loadObj2()  //With 3 is the length of the cube.
{
	int cellx, celly, cellz;
	int line1, line2, line3;
	std::cout << std::fixed << std::showpoint;
	std::cout << std::setprecision(2);
	for (int m = 0; m < facesNum; m++)
	{
		line1 = storeFaces[m].v1line;
		line2 = storeFaces[m].v2line;
		line3 = storeFaces[m].v3line;
		float v1x = storeVertices[line1 - 1].xyz.x;   
		float v1y = storeVertices[line1 - 1].xyz.y;    
		float v1z = storeVertices[line1 - 1].xyz.z;   
		//xMax= 102.843750, xMin=-90.0000000, yMax=94.5000000, yMin=0, zMax=60,zMin=-60 
		cellx = ((v1x - xMin) / 3);
		celly = ((v1y - yMin) / 3);
		cellz = ((v1z - zMin) / 3);

		//int a = hashStores3[cellNumber].size();
		v1x = hashStores1[cellx][celly][cellz].xyz.x;
		v1y = hashStores1[cellx][celly][cellz].xyz.y;
		v1z = hashStores1[cellx][celly][cellz].xyz.z;
		//1,	x=192 y=94 z=120      
		/*
		v1x= (int)((v1x - xMin) / 3); //get xiajie.
		v1x = xMin + 3 * v1x;
		v1x += 1.5;
		
		v1y = (int)((v1y - yMin) / 3); 
		v1y = yMin + 3 * v1y;
		v1y += 1.5;

		v1z = (int)((v1z - zMin) / 3); 
		v1z = zMin + 3 * v1z;
		v1z += 1.5;
		*/
		float v2x = storeVertices[line2 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v2y = storeVertices[line2 - 1].xyz.y;
		float v2z = storeVertices[line2 - 1].xyz.z;

		cellx = ((v2x - xMin) / 3);
		celly = ((v2y - yMin) / 3);
		cellz = ((v2z - zMin) / 3);

		//int a = hashStores3[cellNumber].size();
		v2x = hashStores1[cellx][celly][cellz].xyz.x;
		v2y = hashStores1[cellx][celly][cellz].xyz.y;
		v2z = hashStores1[cellx][celly][cellz].xyz.z;
		/*
		v2x = (int)((v2x - xMin) / 3); 
		v2x = xMin + 3 * v2x;
		v2x += 1.5;

		v2y = (int)((v2y - yMin) / 3);
		v2y = yMin + 3 * v2y;
		v2y += 1.5;

		v2z = (int)((v2z - zMin) / 3);
		v2z = zMin + 3 * v2z;
		v2z += 1.5; 
		*/

		float v3x = storeVertices[line3 - 1].xyz.x; 
		float v3y = storeVertices[line3 - 1].xyz.y;
		float v3z = storeVertices[line3 - 1].xyz.z;

		/*
		v3x = (int)((v3x - xMin) / 3);
		v3x = xMin + 3 * v3x;
		v3x += 1.5;

		v3y = (int)((v3y - yMin) / 3);
		v3y = yMin + 3 * v3y;
		v3y += 1.5;

		v3z = (int)((v3z - zMin) / 3);
		v3z = zMin + 3 * v3z;
		v3z += 1.5;
		*/
		cellx = ((v3x - xMin) / 3);
		celly = ((v3y - yMin) / 3);
		cellz = ((v3z - zMin) / 3);

		//int a = hashStores3[cellNumber].size();
		v3x = hashStores1[cellx][celly][cellz].xyz.x;
		v3y = hashStores1[cellx][celly][cellz].xyz.y;
		v3z = hashStores1[cellx][celly][cellz].xyz.z;


		glColor4f(1.f, 1.f, 1.f, 1.f);
		glBegin(GL_LINES);
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v2x, v2y, v2z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v3x, v3y, v3z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v2x, v2y, v2z);
		glVertex3f(v3x, v3y, v3z);
		glEnd();

	}

}

void CCssample5View::loadObj3()   // With 6 is the length of the cube.
{
	int temp;
	int line1, line2, line3;

	int cellx, celly, cellz;
	for (int m = 0; m < facesNum; m++)
	{
		line1 = storeFaces[m].v1line;
		line2 = storeFaces[m].v2line;
		line3 = storeFaces[m].v3line;
		float v1x = storeVertices[line1 - 1].xyz.x;   
		float v1y = storeVertices[line1 - 1].xyz.y;    
		float v1z = storeVertices[line1 - 1].xyz.z;   
		 
		cellx = ((v1x - xMin) / 6);
		celly = ((v1y - yMin) / 6);
		cellz = ((v1z - zMin) / 6);

		
		v1x = hashStores2[cellx][celly][cellz].xyz.x;
		v1y = hashStores2[cellx][celly][cellz].xyz.y;
		v1z = hashStores2[cellx][celly][cellz].xyz.z;

	
		float v2x = storeVertices[line2 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v2y = storeVertices[line2 - 1].xyz.y;
		float v2z = storeVertices[line2 - 1].xyz.z;

		cellx = ((v2x - xMin) / 6);
		celly = ((v2y - yMin) / 6);
		cellz = ((v2z - zMin) / 6);

		//int a = hashStores3[cellNumber].size();
		v2x = hashStores2[cellx][celly][cellz].xyz.x;
		v2y = hashStores2[cellx][celly][cellz].xyz.y;
		v2z = hashStores2[cellx][celly][cellz].xyz.z;

		float v3x = storeVertices[line3 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v3y = storeVertices[line3 - 1].xyz.y;
		float v3z = storeVertices[line3 - 1].xyz.z;


		cellx = ((v3x - xMin) / 6);
		celly = ((v3y - yMin) / 6);
		cellz = ((v3z - zMin) / 6);

		//int a = hashStores3[cellNumber].size();
		v3x = hashStores2[cellx][celly][cellz].xyz.x;
		v3y = hashStores2[cellx][celly][cellz].xyz.y;
		v3z = hashStores2[cellx][celly][cellz].xyz.z;
		



		glColor4f(1.f, 1.f, 1.f, 1.f);
		glBegin(GL_LINES);
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v2x, v2y, v2z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v3x, v3y, v3z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v2x, v2y, v2z);
		glVertex3f(v3x, v3y, v3z);
		glEnd();

	}
}

void CCssample5View::loadObj4()    // With 9 is the length of the cube.
{

	int line1, line2, line3;
	std::cout << std::fixed << std::showpoint;
	std::cout << std::setprecision(2);
	int cellx, celly, cellz;
	for (int m = 0; m < facesNum; m++)
	{
		line1 = storeFaces[m].v1line;
		line2 = storeFaces[m].v2line;
		line3 = storeFaces[m].v3line;
		float v1x = storeVertices[line1 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v1y = storeVertices[line1 - 1].xyz.y;   //get some corresponding vertices. 
		float v1z = storeVertices[line1 - 1].xyz.z;   //All of the things just come from the coordinates.
		//xMax= 102.843750, xMin=-90.0000000, yMax=94.5000000, yMin=0, zMax=60,zMin=-60 
		//1,	x=192 y=94 z=120      
		
		/*
		v1x = (int)((v1x - xMin) / 9); //get xiajie.
		v1x = xMin + 9 * v1x;
		v1x += 4.5;

		v1y = (int)((v1y - yMin) / 9);
		v1y = yMin + 9 * v1y;
		v1y += 4.5;

		v1z = (int)((v1z - zMin) / 9);
		v1z = zMin + 9 * v1z;
		v1z += 4.5;
		*/
		cellx = ((v1x - xMin) / 9);
		celly = ((v1y - yMin) / 9);
		cellz = ((v1z - zMin) / 9);
			
		//int a = hashStores3[cellNumber].size();
		v1x = hashStores3[cellx][celly][cellz].xyz.x;
		v1y = hashStores3[cellx][celly][cellz].xyz.y;
		v1z = hashStores3[cellx][celly][cellz].xyz.z;

		float v2x = storeVertices[line2 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v2y = storeVertices[line2 - 1].xyz.y;
		float v2z = storeVertices[line2 - 1].xyz.z;
		
		/*
		v2x = (int)((v2x - xMin) / 9);
		v2x = xMin + 9 * v2x;
		v2x += 4.5;

		v2y = (int)((v2y - yMin) / 9);
		v2y = yMin + 9 * v2y;
		v2y += 4.5;

		v2z = (int)((v2z - zMin) / 9);
		v2z = zMin + 9 * v2z;
		v2z += 4.5;
		*/
		cellx = ((v2x - xMin) / 9);
		celly = ((v2y - yMin) / 9);
		cellz = ((v2z - zMin) / 9);

		v2x = hashStores3[cellx][celly][cellz].xyz.x;
		v2y = hashStores3[cellx][celly][cellz].xyz.y;
		v2z = hashStores3[cellx][celly][cellz].xyz.z;

		float v3x = storeVertices[line3 - 1].xyz.x;   //All kinds of the symbols dance. 
		float v3y = storeVertices[line3 - 1].xyz.y;
		float v3z = storeVertices[line3 - 1].xyz.z;

		/*
		v3x = (int)((v3x - xMin) / 9);
		v3x = xMin + 9 * v3x;
		v3x += 4.5;

		v3y = (int)((v3y - yMin) / 9);
		v3y = yMin + 9 * v3y;
		v3y += 4.5;

		v3z = (int)((v3z - zMin) / 9);
		v3z = zMin + 9 * v3z;
		v3z += 4.5;
		*/
		cellx = ((v3x - xMin) / 9);
		celly = ((v3y - yMin) / 9);
		cellz = ((v3z - zMin) / 9);

		v3x = hashStores3[cellx][celly][cellz].xyz.x;
		v3y = hashStores3[cellx][celly][cellz].xyz.y;
		v3z = hashStores3[cellx][celly][cellz].xyz.z;

		glColor4f(1.f, 1.f, 1.f, 1.f);
		glBegin(GL_LINES);
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v2x, v2y, v2z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v3x, v3y, v3z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v2x, v2y, v2z);
		glVertex3f(v3x, v3y, v3z);
		glEnd();

	}

}
void CCssample5View::OnDraw(CDC* pDC) //just substitute the old value with new value, then like a hash table, 
{//but a better plan is to directly change the line number, using another vector to store the lines. 
 //
	CCssample5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rcClient;
	GetClientRect(rcClient);
	
	w=rcClient.Width();
	h=rcClient.Height();

	wglMakeCurrent(m_hDC,m_hRC);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);		
	
	glColor4f(1.0, 1.0, 1.0, 0.0);
	//let the first image in the first viewport, then the


	//--------->up-left viewport.<--------//plot the original one
	glViewport(0, (int)(h / 2.0), (int)(w / 2.0), (int)(h / 2.0));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w/(float)h, 1, 256);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Camera.x, Camera.y, Camera.z, 0, 0, 0, 0, 1, 0);
	findObj();  //This is a critical procedure. 
	findMaxMin();
	//copy(storeVertices.begin(),storeVertices.end(),copyVertices.begin());
	//after this ,get the right max and min vertex of the obj.
	//xMax= 102.843750, xMin=-90.0000000, yMax=94.5000000, yMin=0, zMax=60,zMin=-60 
	//1,	x=192 y=94 z=120      
	glPushMatrix();
		glTranslatef(-40,0, -40);
		DrawXYZaxis();
	glPopMatrix();
	//2,
	glPushMatrix();
		glTranslatef(-40, -60, -40);//cause the translation position is different, so draw the objects at different positions. 
		 //First scale, then translate it.
		loadObj();  
	glPopMatrix();

	
	hashVertices();
	
	//------>up-right viewport.<--------//
	glViewport((int)(w / 2.0), (int)(h / 2.0), (int)(w / 2.0), (int)(h / 2.0));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / (float)h, 1, 256);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Camera.x, Camera.y, Camera.z, 0, 0, 0, 0, 1, 0);

	
	//1,	
	glPushMatrix();
		glTranslatef(-40, 0, -40);
		DrawXYZaxis();
	glPopMatrix();
	//2,
	glPushMatrix();
		glTranslatef(-40, -60, -40);
		
		loadObj2();
	glPopMatrix();

	//------>lower-left viewport.<------the length of the cell is 3.
	//xMax= 102.843750, xMin=-90.0000000, yMax=94.5000000, yMin=0, zMax=60,zMin=-60 
	//1,	x=192 y=94 z=120 
	glViewport(0, 0, (int)(w / 2.0), (int)(h / 2.0));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / (float)h, 1, 256);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Camera.x, Camera.y, Camera.z, 0, 0, 0, 0, 1, 0);

	

	//1,	
	glPushMatrix();
	glTranslatef(-40, 0, -40);
	DrawXYZaxis();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-40, -60, -40);
	loadObj3();
	glPopMatrix();

	

	//------>lower-right viewport.<--------
	glViewport((int)(w / 2.0), 0, (int)(w / 2.0), (int)(h / 2.0));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / (float)h, 1, 256);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Camera.x, Camera.y, Camera.z, 0, 0, 0, 0, 1, 0);


	//1,	
	glPushMatrix();
	glTranslatef(-40, 0, -40);
	DrawXYZaxis();
	glPopMatrix();
	//2,
	glPushMatrix();
	glTranslatef(-40, -60, -40);

	loadObj4();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glFlush();

//	wglMakeCurrent(m_hDC,NULL);
	wglMakeCurrent(m_hDC,m_hRC);
    ::SwapBuffers(m_hDC);
}

/////////////////////////////////////////////////////////////////////////////
// CCssample5View printing

BOOL CCssample5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CCssample5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{}

void CCssample5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{}

/////////////////////////////////////////////////////////////////////////////
// CCssample5View diagnostics

#ifdef _DEBUG
void CCssample5View::AssertValid() const
{
	CView::AssertValid();
}

void CCssample5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCssample5Doc* CCssample5View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCssample5Doc)));
	return (CCssample5Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCssample5View message handlers

int CCssample5View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	int nPixelFormat;				
	m_hDC = ::GetDC(m_hWnd);			

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	
		1,									
		PFD_DRAW_TO_WINDOW |			
		PFD_SUPPORT_OPENGL |			
		PFD_DOUBLEBUFFER,				
		PFD_TYPE_RGBA, 24, 0, 0, 0, 0, 0, 0,					
		0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 
                PFD_MAIN_PLANE, 0, 0, 0, 0 
	};

    nPixelFormat = ::ChoosePixelFormat(m_hDC, &pfd);
    ::SetPixelFormat(m_hDC, nPixelFormat, &pfd);
    m_hRC = ::wglCreateContext(m_hDC);
    ::wglMakeCurrent(m_hDC,m_hRC);
    SetupGL();
    ::wglMakeCurrent(m_hDC,NULL);
	
	return 0;
}

void CCssample5View::OnDestroy() 
{
	CView::OnDestroy();
	// Clean up rendering context stuff
    ::wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd,m_hDC);	
}

void CCssample5View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
		
    ::wglMakeCurrent(m_hDC,m_hRC);
    SizeGL1(cx, cy);
    ::wglMakeCurrent(NULL,NULL);

}

// Setup GL view/projection matrices for window size.
void CCssample5View::SizeGL1(int w, int h)
{
        // Correct for zero dimensions.
        h = h ? h : 1;
        w = w ? w : 1;
 
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, w, 0, h);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glFlush();
}

void CCssample5View::SetupGL()
{
        glDepthFunc(GL_LEQUAL);
	    glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
}

void CCssample5View::SetupLightAndMaterial()
{
	//your implementation here
	//....
}

void CCssample5View::DrawXYZaxis()
{
	  //glColor4f(1.0, 0.0, 0.0, 0.0);
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RedSurface);
	  glBegin(GL_LINES);
		glVertex3f(-100, 0, 0);
		glVertex3f(100,  0, 0);
	  glEnd();

      //glColor4f(0.0, 1.0, 0.0, 0.0);
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GreenSurface);
      glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 100,  0);
	  glEnd();

      //glColor4f(0.0, 0.0, 1.0, 0.0);
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, BlueSurface);
      glBegin(GL_LINES);
		glVertex3f(0, 0, -100);
		glVertex3f(0, 0,  100);
	  glEnd();
      //glColor4f(1.0, 1.0, 1.0, 0.0);
}

void CCssample5View::OnSelectControlPoint1() 
{

}

void CCssample5View::OnSelectControlPoint2() 
{

}

void CCssample5View::OnSelectControlPoint3() 
{

}

void CCssample5View::OnSelectControlPoint4() 
{

}

void CCssample5View::OnXDec() 
{

	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnXInc() 
{

	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnYDec() 
{

    InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnYInc() 
{

	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnZDec() 
{

	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnZInc() 
{

	InvalidateRect(NULL, FALSE);
}

float CCssample5View::Bezier3(int j, float k)
{
return 0;
}

void CCssample5View::CalcBezier()
{

}

void CCssample5View::setAllNormals()
{
	//your implementation here
	//....
}

void CCssample5View::bezier_plane()
{

}

void CCssample5View::CalcNormal(Vector p1, Vector p2, Vector p3, float *n)
{
	//your implementation here
	//....
}

double CCssample5View::dotproduct(float *v1, double v2[])
{
	//your implementation here
	//....
	return 0;
}

void CCssample5View::OnViewFilled() 
{
	//your implementation here
	//....
}

void CCssample5View::OnViewWireframe() 
{
	//your implementation here
	//....
}

void CCssample5View::OnViewFlat() 
{
	//your implementation here
	//....	
}

void CCssample5View::OnViewSmooth() 
{
	//your implementation here
	//....
}


void CCssample5View::OnViewLightYdec() 
{
	//your implementation here
	//....
}

void CCssample5View::OnViewLightYinc() 
{
	//your implementation here
	//....	
}

BOOL CCssample5View::LoadImage(LPCTSTR fileName)
{
	HBITMAP hBmp = (HBITMAP) ::LoadImage (NULL,(LPCTSTR) fileName, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE |LR_CREATEDIBSECTION);

	if (hBmp == NULL){
		AfxMessageBox("Error: unable to load bitmap file");
        return FALSE;
    }

    // Get bitmap info.
	::GetObject (hBmp, sizeof(texImage), &texImage);

    // Must be 24 bit
    if (texImage.bmBitsPixel != 24){
		AfxMessageBox("Error: bitmap must be 24 bit");
        return FALSE;
    }
        
    glBindTexture(GL_TEXTURE_2D, ImgName);
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, texImage.bmWidth, texImage.bmHeight, 
			GL_BGR_EXT, GL_UNSIGNED_BYTE, texImage.bmBits );

    return TRUE;
}

void CCssample5View::SetTexture()
{
	glDrawBuffer(GL_BACK);
	glGenTextures(1, &ImgName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	LoadImage("flower.bmp");
}

void CCssample5View::OnViewTextureDisable() 
{
	//your implementation here
	//....	
}

void CCssample5View::OnViewTextureEnable() 
{
	//your implementation here
	//....
}

void CCssample5View::OnViewPosDecz() 
{
	//your implementation here
	//....
}

void CCssample5View::OnViewPosIncz() 
{
	//your implementation here
	//....	
}

