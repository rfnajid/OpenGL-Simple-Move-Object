#include <stdio.h>
#include <math.h>
#include <glut.h>

typedef struct {
	float m[3][3];
} matrix2D_t;

typedef struct {
	float v[3];
} vector2D_t;

typedef struct {
	float x;
	float y;
} point2D_t;

typedef struct {
	float r;
	float g;
	float b;
} color_t;

////////////////// matrices and vectors 2D ver 1 /////////////////
matrix2D_t createIdentity(void)
{
	matrix2D_t u;
	int i,j;
	for (i=0;i<3;i++) {
		for(j=0;j<3;j++) u.m[i][j]=0.;
		u.m[i][i]=1.;
	}
	return u;
}

matrix2D_t multiply(matrix2D_t a,matrix2D_t b)
{
	matrix2D_t c;//c=a*b
	int i,j,k;
	for (i=0;i<3;i++) for (j=0;j<3;j++) {
		c.m[i][j]=0;
		for (k=0;k<3;k++) c.m[i][j]+=a.m[i][k]*b.m[k][j];
	}
	return c;
}

vector2D_t multiply(matrix2D_t a, vector2D_t b)
{
	vector2D_t c;//c=a*b
	int i,j;
	for (i=0;i<3;i++) {
		c.v[i]=0;
		for (j=0;j<3;j++) c.v[i]+=a.m[i][j]*b.v[j];
	}
	return c;
}

matrix2D_t translationMTX(float dx,float dy)
{
	matrix2D_t trans=createIdentity();
	trans.m[0][2]=dx;
	trans.m[1][2]=dy;
	return trans;
}

matrix2D_t rotationMTX(float theta)
{
	matrix2D_t rotate=createIdentity();
	float cs=cos(theta);
	float sn=sin(theta);
	rotate.m[0][0]=cs; rotate.m[0][1]=-sn;
	rotate.m[1][0]=sn; rotate.m[1][1]=cs;
	return rotate;
}

matrix2D_t scalingMTX(float factorx,float factory)
{
	matrix2D_t scale=createIdentity();
	scale.m[0][0]=factorx;
	scale.m[1][1]=factory;
	return scale;
}

point2D_t Vector2Point(vector2D_t vec)
{
	point2D_t pnt;
	pnt.x=vec.v[0];
	pnt.y=vec.v[1];
	return pnt;
}

vector2D_t Point2Vector(point2D_t pnt)
{
	vector2D_t vec;
	vec.v[0]=pnt.x;
	vec.v[1]=pnt.y;
	vec.v[2]=1.;
	return vec;
}

////////////// End of matrices and vectors 2D ver 1 //////////////

////////////// OpenGL drawShape Functions ver 1 /////////////////
void setColor(float red,float green,float blue)
{
	glColor3f(red, green, blue);	
}

void setColor(color_t col)
{
	glColor3f(col.r, col.g, col.b);	
}

void drawDot(float x,float y)
{
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

void drawLine(point2D_t p1,point2D_t p2)
{
	drawLine(p1.x,p1.y,p2.x,p2.y);
}

//n: number of points
void drawPolyline(point2D_t pnt[],int n)
{
	int i;
	glBegin(GL_LINE_STRIP);
		for (i=0;i<n;i++) {
			glVertex2f(pnt[i].x, pnt[i].y);
		}
	glEnd();
}

//n: number of vertices
void drawPolygon(point2D_t pnt[],int n)
{
	int i;
	glBegin(GL_LINE_LOOP);
		for (i=0;i<n;i++) {
			glVertex2f(pnt[i].x, pnt[i].y);
		}
	glEnd();
}

void fillPolygon(point2D_t pnt[],int n,color_t color)
{
	int i;
	setColor(color);
	glBegin(GL_POLYGON);
		for (i=0;i<n;i++) {
			glVertex2f(pnt[i].x, pnt[i].y);
		}
	glEnd();
}

void gradatePolygon(point2D_t pnt[],color_t col[],int num)
{
	int i;
	glBegin(GL_POLYGON);
		for (i=0;i<num;i++) {
			setColor(col[i]);
			glVertex2f(pnt[i].x, pnt[i].y);
		}
	glEnd();
}

//////////// End of OpenGL drawShape Functions ver 1 ////////////

void userdraw(void);

void display(void)
{
	glClear( GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////
void drawcharX(float x,float y)
{
	drawLine(x,y,x+10,y+12);drawLine(x,y+12,x+10,y);
}

void drawcharY(float x,float y)
{
	drawLine(x+5,y,x+5,y+7);drawLine(x,y+12,x+5,y+7);drawLine(x+10,y+12,x+5,y+7);
}

void drawAxes(void)
{
	drawLine(-310,0,310,0);drawLine(310,0,300,5);drawLine(310,0,300,-5);
	drawcharX(300,-20);
	drawLine(0,-230,0,230);drawLine(0,230,5,220);drawLine(0,230,-5,220);
	drawcharY(-20,220);
}

//////////////////////////////////////////////////////////////////

void userdraw(void)
{
	static float tick=0; //tick, variabel yang akan kita tambahkan terus tiap frame
	
	float inc=0.01; //variabel pengatur kecepatan, ubah sesuai porsi;
	
	point2D_t ef[4]={{-10,-10},{-10,10},{10,10},{10,-10}}; //bentuk kotak
	
	matrix2D_t mat; //variabel temporary yang digunakan untuk menggerakkan
	vector2D_t vec[4];
	setColor(1,1,1);
	
	//arahx, arahy ditambah tick supaya gerak, kalau gak tambah bakal diam
	float arahx=0.01+tick; //0.01 itu kecepatannya
	float arahy=0.01+tick; 

	mat=translationMTX(arahx,arahy); //membentuk matrixnya, 

	//mengalikan matrix dengan titik asal untuk mengubah posisi
	for (int i=0;i<4;i++) { 
			vec[i]=Point2Vector(ef[i]);
			vec[i]=multiply(mat,vec[i]);
			ef[i]=Vector2Point(vec[i]);
	}
	//digambar deh
	drawPolygon(ef,4);

	//increment tick, biarkan tambah terus
	tick+=inc; 
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow ("kotakgerak");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(-320., 320., -240.0, 240.0);
	  // Define the dimensions of the Orthographic Viewing Volume
	glutIdleFunc(display); // idle event call back
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}