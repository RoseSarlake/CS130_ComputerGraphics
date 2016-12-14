/**
 * minigl.cpp
 * -------------------------------
 * Implement miniGL here.
 *
 * You may include minigl.h and any of the standard C++ libraries.
 * No other includes are permitted.  Other preprocessing directives
 * are also not permitted.  These requirements are strictly
 * enforced.  Be sure to run a test grading to make sure your file
 * passes the sanity tests.
 */

#include "minigl.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <cstdio>
#include <stack>
#include <iostream>

using namespace std;


/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char* description) {
    printf("%s\n", description);
    exit(1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//VECTOR of Vector of Verticies
vector< vector < vector<MGLfloat> >> GLOBAL_VERTICIES;	

//Global temporary "list" of verticies to be put into the vector above this one
vector<vector<MGLfloat>> TEMP_LIST_VERTICIES;

//GLOBAL MATRIX STACK
stack<vector<MGLfloat>> MatrixStack; 
vector<MGLfloat> CurrentMatrix;

MGLint MatrixMode;	//if 1 --> proj if 2 -->VM

//Varible for TYPE OF vertex
MGLfloat GLOBAL_VTYPE;

// GLOBAL COLOR for verticies
MGLfloat GLOBAL_COLOR[3] = {1,1,1};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Current matrix * VERTEX
vector<MGLfloat> MultMatrixCUR(vector<MGLfloat> i);
//dot product of 2 matricies
MGLfloat dot(vector<MGLfloat> X, vector<MGLfloat> Y);
//4x4 matrix * 4x4 matrix saved in current
void MatrixMult4x4(vector<MGLfloat> i, vector<MGLfloat> j);
/**
 * Read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  The boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * Rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */
void mglReadPixels(MGLsize width,
                   MGLsize height,
                   MGLpixel *data)
{
	//STEP 1	Find The Origin in Data Value
	MGLpixel NewOrigin = width *( height +1);
	//because if you divide an odd number it will round down so we added one for height
	//dividing by 2
	NewOrigin >>= 1;
	if(NewOrigin % width != 0)
	{
		NewOrigin = NewOrigin + (NewOrigin%width);
	}
	//sutracting we want to round down so its good 
	NewOrigin = NewOrigin - (width >> 1); 
	
	
	//Do everything in this for loop b/c one shape at a time
	for(unsigned H = 0; H < GLOBAL_VERTICIES.size(); H++)
	{
	
		int Xmax = -(width >> 1);
		int Xmin = width >> 1;
		int Ymin = height >> 1;
		int Ymax =  -(height >> 1);
		int Zmin = 1000;
		int Zmax = -1000;
		
		for(unsigned l = 0; l < 3; l++)
		{
			cout << "x: " << GLOBAL_VERTICIES[H][l][0] << " y: " << GLOBAL_VERTICIES[H][l][1] <<  endl;
		}
	//STEP 2 	Convert XY percent to X,Y on the Origin store it back into global
	// 			convertion ... X% * .5 width = X ... Y% * .5(height) = Y	
		for(unsigned j = 0; j < GLOBAL_VERTICIES[H].size(); j++)
		{
	
			GLOBAL_VERTICIES[H][j][0] = GLOBAL_VERTICIES[H][j][0]*(width >> 1);
			GLOBAL_VERTICIES[H][j][1] = GLOBAL_VERTICIES[H][j][1]*(height >> 1);

	
		//GET MIN AND MAX POINTS HERE TOO
			if(GLOBAL_VERTICIES[H][j][0] > Xmax)
			{
				Xmax = GLOBAL_VERTICIES[H][j][0];
			}
			if(GLOBAL_VERTICIES[H][j][0] < Xmin)
			{
				Xmin = GLOBAL_VERTICIES[H][j][0];
			}
			if(GLOBAL_VERTICIES[H][j][1] > Ymax)
			{
				Ymax = GLOBAL_VERTICIES[H][j][1];
			}
			if(GLOBAL_VERTICIES[H][j][1] < Ymin)
			{
				Ymin = GLOBAL_VERTICIES[H][j][1];
			}			
			if(GLOBAL_VERTICIES[H][j][2] < Zmin)
			{
				Zmin = GLOBAL_VERTICIES[H][j][2];
			}
			if(GLOBAL_VERTICIES[H][j][2] > Zmax)
			{
				Zmax = GLOBAL_VERTICIES[H][j][2];
			}
		}
	
	//STEP 4	Do Barametric Cordinates on these values ... and change values into data and store
		//1st get the vectors V0 V1  
		vector<MGLfloat> V0, V1,V2;
		V0.resize(3);
		V1.resize(3);
		V2.resize(3);
		
		//dot product results
		MGLfloat dot00, dot01, dot02, dot11, dot12;  
		
		if(GLOBAL_VERTICIES[H].size() >= 3)
		{	
			V0[0] = ( GLOBAL_VERTICIES[H][2][0] - GLOBAL_VERTICIES[H][0][0] ); 
			V0[1] = ( GLOBAL_VERTICIES[H][2][1] - GLOBAL_VERTICIES[H][0][1] );
			V0[2] = ( GLOBAL_VERTICIES[H][2][2] - GLOBAL_VERTICIES[H][0][2] );
			
			V1[0] = ( GLOBAL_VERTICIES[H][1][0] - GLOBAL_VERTICIES[H][0][0] ); 
			V1[1] = ( GLOBAL_VERTICIES[H][1][1] - GLOBAL_VERTICIES[H][0][1] );
			V1[2] = ( GLOBAL_VERTICIES[H][1][2] - GLOBAL_VERTICIES[H][0][2] );
			
		}
		
		dot00 = dot(V0, V0);
		dot01 = dot(V0, V1);
		
		dot11 = dot(V1,V1);
		if(Zmin == Zmax)
		{			
			Zmax = Zmax + 1;
		}
		
		/*
		//CODE USED FOR CHECKING
		for(unsigned l = 0; l < 3; l++)
		{
			cout << "x: " << GLOBAL_VERTICIES[H][l][0] << " y: " << GLOBAL_VERTICIES[H][l][1] << " z: " << GLOBAL_VERTICIES[H][l][2] << " w: " << GLOBAL_VERTICIES[0][l][3] <<  endl;
		}*/
		
		//cerr << Xmin << ' ' << Xmax << ' ' << Ymin << ' ' << Ymax << endl;
		/*
		MGLpixel a; 
		a =  GLOBAL_VERTICIES[0][2][0] + NewOrigin;
		a = a + (GLOBAL_VERTICIES[0][2][1] * width);
		//make it white not teal
		data[a] = 0xFFFFFFFF;
		
		a =  GLOBAL_VERTICIES[0][1][0] + NewOrigin;
		a = a + (GLOBAL_VERTICIES[0][1][1] * width);
		//make it white not teal
		data[a] = 0xFFFFFFFF;
		
		a =  GLOBAL_VERTICIES[0][0][0] + NewOrigin;
		a = a + (GLOBAL_VERTICIES[0][0][1] * width);
		//make it white not teal
		data[a] = 0xFFFFFFFF;
		*/
		
		
		//call function for dot product
		for(int j = Ymin; j < Ymax; j++)
		{
			
			for(int i = Xmin; i < Xmax; i++)
			{
				
				for(int z = Zmin; z < Zmax; z ++)
				{	
					
					
					V2[0] = i - GLOBAL_VERTICIES[H][0][0];
					V2[1] = j - GLOBAL_VERTICIES[H][0][1];
					V2[2] = z - GLOBAL_VERTICIES[H][0][2];		
					
					
					
					dot02 = dot(V0,V2);
					dot12 = dot(V1,V2);

					MGLfloat divisor = 1/(dot00 * dot11 - dot01 * dot01);
					MGLfloat u = (dot11 * dot02 - dot01 * dot12) * divisor;
					MGLfloat v = (dot00 * dot12 - dot01 * dot02) * divisor;
					
					if((u >= 0) && (v >= 0) && (u+v < 1))
					{						
						MGLpixel temp; 
						temp = i + NewOrigin;
						temp = temp + (j * width);
						//make it white not teal
						data[temp] = 0xFFFFFFFF;
					}
					

				}
			}
		}
		
		
	} 
	
	
	
	
}

/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode)
{
	if(MatrixStack.empty())
	{
		//build identity matrix
		CurrentMatrix.resize(16);
		CurrentMatrix.at(0) = 1;
		CurrentMatrix.at(5) = 1;
		CurrentMatrix.at(10) = 1;
		CurrentMatrix.at(15) = 1;
		MatrixStack.push(CurrentMatrix);
	}
	switch(mode)
	{
		case MGL_TRIANGLES:
			GLOBAL_VTYPE = 1;
			break;
				
		case MGL_QUADS:
			GLOBAL_VTYPE = 2;
			break;
				
		default:
			GLOBAL_VTYPE = 0;
			break;
	}
}


/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd()
{
	
	for(unsigned l = 0; l < TEMP_LIST_VERTICIES.size(); l++)
	{
		cout << "x: " << TEMP_LIST_VERTICIES[l][0] << " y: " << TEMP_LIST_VERTICIES[l][1] << " z: " << TEMP_LIST_VERTICIES[l][2] << " w: " << TEMP_LIST_VERTICIES[l][3] <<  endl;
	}
	cout << endl;
	
	if(TEMP_LIST_VERTICIES[0][7] == 2)
	{
		vector <vector<MGLfloat>> T;
		vector<MGLfloat> TEMP_Vertex;
		TEMP_Vertex = TEMP_LIST_VERTICIES[0];
		T.push_back(TEMP_Vertex);
		TEMP_Vertex = TEMP_LIST_VERTICIES[1];
		T.push_back(TEMP_Vertex);
		TEMP_Vertex = TEMP_LIST_VERTICIES[2];
		T.push_back(TEMP_Vertex);
		GLOBAL_VERTICIES.push_back(T);
		T.clear();
		
		TEMP_Vertex = TEMP_LIST_VERTICIES[0];
		T.push_back(TEMP_Vertex);
		TEMP_Vertex = TEMP_LIST_VERTICIES[2];
		T.push_back(TEMP_Vertex);
		TEMP_Vertex = TEMP_LIST_VERTICIES[3];
		T.push_back(TEMP_Vertex);
		GLOBAL_VERTICIES.push_back(T);
		TEMP_LIST_VERTICIES.clear();
		GLOBAL_VTYPE = 0;
	}
	else
	{
		GLOBAL_VERTICIES.push_back(TEMP_LIST_VERTICIES);
		TEMP_LIST_VERTICIES.clear();
		GLOBAL_VTYPE = 0;
	}
}

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
                MGLfloat y)
{
	vector<MGLfloat> TEMP_Vertex;
	TEMP_Vertex.resize(8);
	TEMP_Vertex[0] = x;
	TEMP_Vertex[1] = y;
	TEMP_Vertex[2] = 0;
	TEMP_Vertex[3] = 1;
	TEMP_Vertex[4] = GLOBAL_COLOR[0];
	TEMP_Vertex[5] = GLOBAL_COLOR[1];
	TEMP_Vertex[6] = GLOBAL_COLOR[2];
	TEMP_Vertex[7] = GLOBAL_VTYPE;
	
	TEMP_Vertex = MultMatrixCUR(TEMP_Vertex);
	//push into temporary list of verticies
	TEMP_LIST_VERTICIES.push_back(TEMP_Vertex);
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z)
{
	vector<MGLfloat> TEMP_Vertex;
	TEMP_Vertex.resize(8);
	TEMP_Vertex[0] = x;
	TEMP_Vertex[1] = y;
	TEMP_Vertex[2] = z;
	TEMP_Vertex[3] = 1;
	TEMP_Vertex[4] = GLOBAL_COLOR[0];
	TEMP_Vertex[5] = GLOBAL_COLOR[1];
	TEMP_Vertex[6] = GLOBAL_COLOR[2];
	TEMP_Vertex[7] = GLOBAL_VTYPE;
	

	TEMP_Vertex = MultMatrixCUR(TEMP_Vertex);
	//push into temporary list of verticies
	TEMP_LIST_VERTICIES.push_back(TEMP_Vertex);
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode)
{
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix()
{
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix()
{
}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity()
{
}

/**
 * Replace the current matrix with an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglLoadMatrix(const MGLfloat *matrix)
{
}

/**
 * Multiply the current matrix by an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglMultMatrix(const MGLfloat *matrix)
{
}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z)
{
}

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle,
               MGLfloat x,
               MGLfloat y,
               MGLfloat z)
{
}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
              MGLfloat y,
              MGLfloat z)
{
}

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left,
                MGLfloat right,
                MGLfloat bottom,
                MGLfloat top,
                MGLfloat near,
                MGLfloat far)
{
	if(MatrixStack.empty())
	{
		//build identity matrix
		CurrentMatrix.resize(16);
		CurrentMatrix.at(0) = 1;
		CurrentMatrix.at(5) = 1;
		CurrentMatrix.at(10) = 1;
		CurrentMatrix.at(15) = 1;
		MatrixStack.push(CurrentMatrix);
	}
	
	vector<MGLfloat> temp;
	temp.resize(16);
	MGLfloat A = (right + left) / (right - left); 
	MGLfloat B = (top + bottom) / (top - bottom);
	MGLfloat C = (far + near) / (far - near);
	MGLfloat D =  (2 * far * near) / (far - near);
	
	temp[0] = (2 * near) / (right - left);
	temp[5] = (2 * near) / (top - bottom);
	temp[8] = A;
	temp[9] = B;
	temp[10] = -C;
	temp[11] = -1;
	temp[14] = -D;
	
	for(int i = 0 ; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			cerr <<  temp[4*i + j] << ' ';
		}
		cerr << endl;
	}
	cerr << endl;
	for(int i = 0 ; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			cerr <<  CurrentMatrix[4*i + j] << ' ';
		}
		cerr << endl;
	}
	cerr << endl;
	
	MatrixMult4x4(CurrentMatrix, temp);
	
	for(int i = 0 ; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			cerr <<  CurrentMatrix[4*i + j] << ' ';
		}
		cerr << endl;
	}
	cerr << endl;
}

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left,
              MGLfloat right,
              MGLfloat bottom,
              MGLfloat top,
              MGLfloat near,
              MGLfloat far)
{
	if(MatrixStack.empty())
	{
		//build identity matrix
		CurrentMatrix.resize(16);
		CurrentMatrix.at(0) = 1;
		CurrentMatrix.at(5) = 1;
		CurrentMatrix.at(10) = 1;
		CurrentMatrix.at(15) = 1;
		MatrixStack.push(CurrentMatrix);
	}
	vector<MGLfloat> originalMatrix;
	originalMatrix.resize(16);
		
	originalMatrix[0] = 2/(right - left);
	originalMatrix[1] = 0;
	originalMatrix[2] = 0;
	originalMatrix[3] = 0;
	
	originalMatrix[4] = 0;
	originalMatrix[5] = 2/(top -bottom);
	originalMatrix[6] = 0;
	originalMatrix[7] = 0;
	
	originalMatrix[8] = 0;
	originalMatrix[9] = 0;
	originalMatrix[10] = -2/(far - near);
	originalMatrix[11] = 0;
	
	originalMatrix[12] = -(right + left) / (right - left);
	originalMatrix[13] = -(top + bottom) / (top - bottom);
	if(-near != far)
	{	
		originalMatrix[14] = (near + far) / (near - far);
	}
	else
	{
		originalMatrix[14] = 0;
	}
	originalMatrix[15] = 1;
	
	
	
	MatrixMult4x4(CurrentMatrix, originalMatrix);

	for(unsigned i = 0; i < 4; i++)
	{
		for(unsigned j = 0; j < 4; j++)
		{
			cout << CurrentMatrix.at(i+(j*4)) << ' ';
		}
		cout << endl;
	}
}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLfloat red,
              MGLfloat green,
              MGLfloat blue)
{
}

//4x4 matrix * 4x4 matrix saved in current
void MatrixMult4x4(vector<MGLfloat> i, vector<MGLfloat> j)
{
	vector<MGLfloat> POP;
	POP.resize(16);
	
	POP[0] = i[0]* j[0] + i[4]* j[1] + i[8] * j[2] + i[12] * j[3] ;
	POP[1] = i[1]* j[0] + i[5]* j[1] + i[9] * j[2] + i[13] * j[3];
	POP[2] = i[2]* j[0] + i[6]* j[1] + i[10] * j[2] + i[14] * j[3];
	POP[3] = i[3]* j[0] + i[7]* j[1] + i[11] * j[2] + i[15] * j[3];
	
	POP[4] = i[0]* j[4] + i[4]* j[5] + i[8] * j[6] + i[12] * j[7];
	POP[5] = i[1]* j[4] + i[5]* j[5] + i[9] * j[6] + i[13] * j[7];
	POP[6] = i[2]* j[4] + i[6]* j[5] + i[10] * j[6] + i[14] * j[7];
	POP[7] = i[3]* j[4] + i[7]* j[5] + i[11] * j[6] + i[15] * j[7];
	
	POP[8] = i[0]* j[8] + i[4]* j[9] + i[8] * j[10] + i[12] * j[11];
	POP[9] = i[1]* j[8] + i[5]* j[9] + i[9] * j[10] + i[13] * j[11];
	POP[10] = i[2]* j[8] + i[6]* j[9] + i[10] * j[10] + i[14] * j[11];
	POP[11] = i[3]* j[8] + i[7]* j[9] + i[11] * j[10] + i[15] * j[11];
	
	POP[12] = i[0]* j[12] + i[4]* j[13] + i[8] * j[14] + i[12] * j[15];
	POP[13] = i[1]* j[12] + i[5]* j[13] + i[9] * j[14] + i[13] * j[15];
	POP[14] = i[2]* j[12] + i[6]* j[13] + i[10] * j[14] + i[14] * j[15];
	POP[15] = i[3]* j[12] + i[7]* j[13] + i[11] * j[14] + i[15] * j[15];
	
	CurrentMatrix = POP;
	
}


//dot product
MGLfloat dot(vector<MGLfloat> X, vector<MGLfloat> Y)
{
	MGLfloat T = 0;
	for(unsigned int i = 0; i < X.size(); i++)
	{
		T = T + (X[i] * Y[i]);
	}
	return T;
	
}


//Current matrix * VERTEX
vector<MGLfloat> MultMatrixCUR(vector<MGLfloat> i)
{
	//cerr << CurrentMatrix[1] << ' ' <<CurrentMatrix[5] << ' ' <<CurrentMatrix[9] << ' ' <<CurrentMatrix[13] << endl;
	//cerr << i[0] << ' ' << i[1] << ' ' << i[2] << ' ' << i[3] << endl;
	MGLfloat temp0, temp1, temp2, temp3;
	temp0 = 0;
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	//cerr << i[0] << ' '<< i[1] << ' '<< i[2] << ' ' << i[3] << endl;
	/*for(int i = 0 ; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			cerr <<  CurrentMatrix[4*i + j] << ' ';
		}
		cerr << endl;
	}
	cerr << endl;*/
	temp0 = CurrentMatrix.at(0) * i[0];
	temp0 = temp0 + CurrentMatrix.at(4)*i[1];
	temp0 = temp0 + CurrentMatrix.at(8)*i[2];
	temp0 = temp0 + CurrentMatrix.at(12)*i[3];
	
	temp1 = CurrentMatrix.at(1) * i[0];
	temp1 = temp1 + CurrentMatrix.at(5)*i[1];
	temp1 = temp1 + CurrentMatrix.at(9)*i[2];
	temp1 = temp1 + CurrentMatrix.at(13)*i[3];
	
	temp2 = CurrentMatrix.at(2) * i[0];
	temp2 = temp2 + CurrentMatrix.at(6)*i[1];
	temp2 = temp2 + CurrentMatrix.at(10)*i[2];
	temp2 = temp2 + CurrentMatrix.at(14)*i[3];
	
	temp3 = CurrentMatrix.at(3) * i[0];
	temp3 = temp3 + CurrentMatrix.at(7)*i[1];
	temp3 = temp3 + CurrentMatrix.at(11)*i[2];
	temp3 = temp3 + CurrentMatrix.at(15)*i[3];
	
	i[0] = temp0;
	i[1] = temp1;
	i[2] = temp2;
	i[3] = temp3;
	return i;
	//cerr << temp0 << ' '<< temp1 << ' '<< temp2 << ' '<< temp3 << endl << endl;
	
}
