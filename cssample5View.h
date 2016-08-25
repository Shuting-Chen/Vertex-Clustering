// cssample5View.h : interface of the CCssample5View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSsample5VIEW_H__72070A70_2B2E_4625_A38B_3C3967F61AFB__INCLUDED_)
#define AFX_CSsample5VIEW_H__72070A70_2B2E_4625_A38B_3C3967F61AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include <iomanip>
#include <iostream>

class CCssample5View : public CView
{
protected: // create from serialization only
	CCssample5View();
	DECLARE_DYNCREATE(CCssample5View)

	// Attributes
public:
	CCssample5Doc* GetDocument();

	// Operations
public:

	int m_GLPixelIndex;
	HDC m_hDC;
	HGLRC m_hRC;

	GLdouble m_ytheta;
	GLdouble m_xtheta;
	CPoint m_LDownPos;
	BOOL m_LButtonDown;

	int w, h;
	int angle, rangle;
	int control_point;
	int lineNum;
	int facesNum;
	int cellNumber1, cellNumber2, cellNumber3;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	typedef struct Vec{
		float x;
		float y;
		float z;

		Vec(){ x = .0f; y = .0f; z = .0f; }
		//Vec(float x_, float y_, float z_) : x(x_), y(y_), z(z_){}
		Vec(float x_, float y_, float z_){

			x = x_;
			y = y_;
			z = z_;
		}

	}Vector;



	typedef struct newStruct2{
		int v1line;
		int v2line;
		int v3line;

		newStruct2(){}
		newStruct2(int v1line_, int v2line_, int v3line_){
			v1line = v1line_;
			v2line = v2line_;
			v3line = v3line_;

		}
	}faceData;


	typedef struct newStruct{ //When I have this, I need to let the data structure to have a guidline.
		Vector xyz;  //a struct contains another struct. If I don't know something, just get some things from others. 
		int edgeNum;

		newStruct(){}

		newStruct(float x_, float y_, float z_)
		{
			xyz.x = x_;
			xyz.y = y_;
			xyz.z = z_;
			edgeNum = 0;
		}

	}vetexData;


	std::vector<vetexData> storeVertices;
	std::vector<vetexData> copyVertices;
	std::vector<vetexData> newstoreVertices; //store the second kind of vertices. if find the vertices, just change it into
	//the new positions. First just fulfill the functions.
	std::vector<faceData> storeFaces;

	//std::vector<std::vector<vetexData>> hashStores3;
	//vetexData[(xMax-xMin)/3][(y][];
	//std::vector<vetexData> hashStores3[22][11][14];
	vetexData hashStores1[65][32][40];
	vetexData hashStores2[33][16][20];
	vetexData hashStores3[22][11][14];
	//software has its own namespace. 
	void hashVertices();
	
	
	
	Vector p[4][4];
	Vector m[21][21];
	Vector t[21][21];
	Vector step;
	Vector u, v, n;
	Vector camera;
	int x_step;
	int y_step;
	int z_step;
	float normal[40][40][3];
	float vnormal[21][21][3];

	GLfloat RedSurface[4];
	GLfloat GreenSurface[4];
	GLfloat YellowSurface[4];
	GLfloat WhiteSurface[4];
	GLfloat BlueSurface[4];
	GLfloat BlackSurface[4];
	
	// Setup GL view/projection matrices for window size.
    void SizeGL1(int w, int h);
    // Setup default params (lighting, etc...) for GL.
    void SetupGL();
    // Render scene using OpenGL.
    void DrawCylinder(int base_radius, int top_radius, int height);
	void findMaxMin();
	void DrawXYZaxis();

	void loadObj();
	void loadObj2();
	void loadObj3();
	void loadObj4();
	void findObj();


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCssample5View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void setAllNormals();
	void SetTexture();
	GLuint ImgName;
	BITMAP texImage;
	BOOL LoadImage(LPCTSTR fileName);
	Vector Camera;
	Vector LightPos;
	void CalcBezier();
	BOOL rendering_mode;
	double dotproduct(float* v1, double v2[]);
	int WireFrame;
	void SetupLightAndMaterial(void);
	void CalcNormal(Vector p1, Vector p2, Vector p3, float *n);
	void bezier_plane(void);
	float Bezier3(int j, float k);
	virtual ~CCssample5View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCssample5View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelectControlPoint1();
	afx_msg void OnSelectControlPoint2();
	afx_msg void OnSelectControlPoint3();
	afx_msg void OnSelectControlPoint4();
	afx_msg void OnXDec();
	afx_msg void OnXInc();
	afx_msg void OnYDec();
	afx_msg void OnYInc();
	afx_msg void OnZDec();
	afx_msg void OnZInc();
	afx_msg void OnViewFilled();
	afx_msg void OnViewWireframe();
	afx_msg void OnViewFlat();
	afx_msg void OnViewSmooth();
	afx_msg void OnViewLightYdec();
	afx_msg void OnViewLightYinc();
	afx_msg void OnViewTextureDisable();
	afx_msg void OnViewTextureEnable();
	afx_msg void OnViewPosDecz();
	afx_msg void OnViewPosIncz();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in cssample5View.cpp
inline CCssample5Doc* CCssample5View::GetDocument()
   { return (CCssample5Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSsample5VIEW_H__72070A70_2B2E_4625_A38B_3C3967F61AFB__INCLUDED_)
