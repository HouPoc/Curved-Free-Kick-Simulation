#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include <GL/glew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "bmptotexture.cpp"
#include "lighting.h"
#include "sphere.h"
#include "glut.h"
#include "glslprogram.h"
#include "glslprogram.cpp"

// title of these windows:

const char *WINDOWTITLE = { "CS450 Finial Project -----Wenbo Hou" };
const char *GLUITITLE   = { "Soccer Kick" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char * ColorNames[ ] =
{
	"Red",
	"Yellow",
	"Green",
	"Cyan",
	"Blue",
	"Magenta",
	"White",
	"Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};

const float ball_trajedctory[][3] = {
	{ 0.010734596705587324 ,0.0012268715213661794 ,0.0018398304985237368 },
	{ 0.021467420378267134 ,0.002453660603055268 ,0.003678586193703771 },
	{ 0.04292989051911339 ,0.004907235632321433 ,0.007353239133674185 },
	{ 0.08584211806836552 ,0.009814377140299254 ,0.014691110609364628 },
	{ 0.1716157837301477 ,0.019628563289414563 ,0.029321125402719667 },
	{ 0.34296030983505676 ,0.03925618052553333 ,0.058398290792730734 },
	{ 0.684841180906292 ,0.07850532938425296 ,0.11582166378353767 },
	{ 1.4224443750317004 ,0.16354938732796642 ,0.2369854163360866 },
	{ 2.1066781753459027 ,0.2428363534592951 ,0.3459959317192857 },
	{ 2.7733345159258307 ,0.32039784114337855 ,0.4490098022543108 },
	{ 3.491901413848734 ,0.40428008162552803 ,0.5564438599722779 },
	{ 4.158256124812285 ,0.4822684199004972 ,0.6526687170809089 },
	{ 4.875816728481289 ,0.5663982038964811 ,0.7525543335664091 },
	{ 5.575712618020958 ,0.6485356952379686 ,0.8461785286380632 },
	{ 6.224531723552831 ,0.7246857502803292 ,0.9295516934878252 },
	{ 6.923735217201967 ,0.8066874638500926 ,1.0156469809010502 },
	{ 7.572444928449019 ,0.8826455545740461 ,1.0919792319741584 },
	{ 8.204656910854103 ,0.9564981977403243 ,1.1630262484470255 },
	{ 8.886476510693859 ,1.0358848213390828 ,1.2358811842273025 },
	{ 9.518837764070813 ,1.1092056226136522 ,1.2998942660131307 },
	{ 10.200164908791521 ,1.1877990334723 ,1.364962278075234 },
	{ 10.864904649586268 ,1.2639988222214664 ,1.4244741015712412 },
	{ 11.481204380933859 ,1.3341542826841164 ,1.4760809617199877 },
	{ 12.145719177379297 ,1.40919365039553 ,1.5278092280555795 },
	{ 12.76231680386689 ,1.4781915634759697 ,1.5721093015284202 },
	{ 13.426520488204865 ,1.551758990749196 ,1.6157753907181998 },
	{ 14.07470593700324 ,1.6227144987411712 ,1.6542620626598303 },
	{ 14.675935257774505 ,1.6877180350847731 ,1.6862524911868628 },
	{ 15.324057132918844 ,1.7568374267957272 ,1.7166707281563667 },
	{ 15.925716404554283 ,1.820042416074148 ,1.741066752022893 },
	{ 16.512345263478558 ,1.880709892813323 ,1.7612315670479746 },
	{ 17.145190688396433 ,1.9450179189286236 ,1.7789078889532917 },
	{ 17.732454424503455 ,2.0035631045635904 ,1.7914623376697272 },
	{ 20.174363575946558 ,2.2337241122295226 ,1.8025689396164937 },
	{ 20.183352954885905 ,2.234527758159059 ,1.802483957768223 },
	{ 20.192341590444197 ,2.2353309924576914 ,1.8023980387245981 },
	{ 20.2103166319093 ,2.236936225424184 ,1.8022233890453434 },
	{ 20.246257799673817 ,2.240141743668521 ,1.8018628432763026 },
	{ 20.31810450203619 ,2.2465329480910863 ,1.8010967657424384 },
	{ 20.461655593656577 ,2.2592356983867554 ,1.7993846636451203 },
	{ 20.748190274507465 ,2.2843199258107303 ,1.79524064348521 },
	{ 21.307790860265253 ,2.33223697856692 ,1.7843300597763998 },
	{ 21.91177230702685 ,2.3822946263410976 ,1.768316219873084 },
	{ 22.472286027010416 ,2.4271317238921966 ,1.7494556483789023 },
	{ 23.07666751543675 ,2.4736461380517536 ,1.7247341379319083 },
	{ 23.63800486729021 ,2.5150650114481135 ,1.6976321956110552 },
	{ 24.18553847474265 ,2.5537357822159055 ,1.6672929085591714 },
	{ 24.776345771185404 ,2.593462324986172 ,1.6301627116197477 },
	{ 25.324868246501918 ,2.628406962663267 ,1.5915432240957226 },
	{ 25.916174369104077 ,2.6638965007836215 ,1.5453666819663874 },
	{ 26.493590871216 ,2.696277700963392 ,1.495650756794932 },
	{ 27.029473468040475 ,2.724237069217464 ,1.4453573527571817 },
	{ 27.607561614614987 ,2.7520504402375763 ,1.3865479178375268 },
	{ 28.144501453039823 ,2.7756185034289795 ,1.3276236789853337 },
	{ 28.668323373852704 ,2.7964306230379536 ,1.2660854796723036 },
	{ 29.23380164342121 ,2.816394047191885 ,1.195090561323752 },
	{ 29.758824659029834 ,2.83252037964395 ,1.1248684448740711 },
	{ 29.767959745004568 ,2.832779957227919 ,1.1236095379120912 },
	{ 29.777094063305668 ,2.833038786236742 ,1.1223494619592567 },
	{ 29.795360397402987 ,2.8335541976144905 ,1.1198258029725845 },
	{ 29.83188385918841 ,2.8345760274104776 ,1.1147644559146936 }
};

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
GLuint	FieldList;				// object display list
GLuint	GoalList;				// object display list
GLuint	GoalList_1;				// object display list
GLuint	GoalList_2;				// object display list
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
float	post_x, post_y, post_z;
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
float	x_velocity, y_velocity, z_velocity;
float	spinning_speed;
unsigned char *Texture;			// texture 
unsigned char *Texture_1;			// texture 
int		height;
int		width;
int		camera;
int level, ncomps, border;
int para_1, para_2;
bool Freeze;
GLuint  *textures = new GLuint[2];
// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitTexture();
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );
void	Draw_Net(float para_2);
// main program:

int
main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );


	// setup all the graphics stuff:

	InitGraphics( );

	InitTexture();
	// create the display structures that will not change:

	InitLists( );


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );


	// setup all the user interface stuff:

	InitMenus( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );

	// this is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it
#define MS_PER_CYCLE 60
#define PI 3.14159265
void
Animate( )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:

	// force a call to Display( ) next time it is convenient:
	if (para_2 > 120) {
		para_1 = 0;
		para_2 = 0;
	}
	para_1 += 1 ;
	para_2 = para_1;

	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= MS_PER_CYCLE;
	Time = (float)ms / (float)MS_PER_CYCLE;
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}


	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );


	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );


	// specify shading to be flat:

	glShadeModel( GL_FLAT );


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		gluPerspective( 90., 1.,	0.1, 1000. );


	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	// set the eye position, look-at position, and up-vector:
	if (camera == 0) {		// Kicker View
		gluLookAt(-3., 0.8, 1.2, -4., 0.8, 1.2, 0., 1., 0.);

	}
	else if (camera == 1) {	// Side View
		gluLookAt(-6., 1., 3., -6., 1., 0., 0., 1., 0.);

	}
	else {					// Back View
		if (para_2 < 61) {
			gluLookAt(-7.2, 0.65, 1.2, -4., 0.6, -0.5, 0.2, 1., 0.);
		}
		else {
			gluLookAt(-7.2, 0.65, 1.2, -4., 0.6, -0.5, 0.2, 1., 0.);
		}
	}
	//Set light 
	glEnable(GL_LIGHTING);
	SetPointLight(GL_LIGHT0, 0., 20., 0., 1., 1., 1.);
	glEnable(GL_LIGHT0);
	//glDisable(GL_LIGHT0);
	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0., 1., 0. );
	glRotatef( (GLfloat)Xrot, 1., 0., 0. );


	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );


	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}


	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3fv( &Colors[WhichColor][0] );
		glCallList( AxesList );
	}


	// since we are using glScalef( ), be sure normals get unitized:

	glEnable( GL_NORMALIZE );

	
	// draw the Field:
	SetMaterial(1.0, 1.0, 1.0, 20.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glCallList(FieldList);

	//draw the ball;

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	SetMaterial(1.0, 1.0, 1.0, 100.0);
	//glTranslatef(-3.9, 0.05, 0.74);
	
	if (para_2 < 60) {
		//printf("para_2 %d \n", para_2);
		Distort = TRUE;
		glTranslatef(-4.0 - ball_trajedctory[para_2][0] / 10., 0.05 + ball_trajedctory[para_2][2] / 2.6, 0.50 + ball_trajedctory[para_2][1] / 8.);
	}
	else {
		Distort = FALSE;
		//post_x = -3.9 - ball_trajedctory[81][0] / 10. - 0.01 *(float)(para_2 - 81);
		post_x = -7.1 + 0.03 * (float)(para_2 - 60);
		post_z = 0.74 - ball_trajedctory[81][1] / 5. - 0.01 * (float)(para_2 - 60) ;
		post_y = 0.05 + ball_trajedctory[81][2] / 2.6;
		if ((para_2 - 60) < 30) {
			post_y = (0.8 + cos(PI * (float)(para_2 - 60) / 30.)) * 0.427 / 1.6 +  0.05;
		}
		else if ((para_2 - 60) < 60) {
			post_y = (1. + cos(2*PI * (float)(para_2 - 90) / 30. + PI)) * 0.427 /8. + 0.05;
		}
		else {
			post_y = 0.05;
		}
		glTranslatef(post_x, post_y, post_z);
		glRotatef(-30. * (float)(para_2 - 60), .3, 0., 1.);
	}
	//glRotatef(45. * (float)para_2, 1., 0., 0.);
	MjbSphere(.04, 50, 50);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//draw the soccer goal
	SetMaterial(1.0, 1.0, 1.0, 30.0);
	//front vertical bars
	glPushMatrix();
	glTranslatef(-6.7, -0.2, 1.4);
	glRotatef(270., 1., 0., 0.);
	glCallList(GoalList);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-6.7, -0.2, -1.4);
	glRotatef(270., 1., 0., 0.);
	glCallList(GoalList);
	glPopMatrix();

	//front horizontal bar
	glPushMatrix();
	glTranslatef(-6.7, 0.80, -1.4);
	glCallList(GoalList_1);
	glPopMatrix();
	//back horizontal bar
	glPushMatrix();
	glTranslatef(-7.3, 0.02, -1.4);
	glCallList(GoalList_1);
	glPopMatrix();

	//side bars
	glPushMatrix();
	glTranslatef(-7.3, 0.02, 1.4);
	glRotatef(90., 0., 1., 0.);
	glCallList(GoalList_2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7.3, 0.02, -1.4);
	glRotatef(90., 0., 1., 0.);
	glCallList(GoalList_2);
	glPopMatrix();

	//back net holders
	glPushMatrix();
	glTranslatef(-7.9, 0., -1.4);
	glRotatef(270., 1., 0., 0.);
	glCallList(GoalList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7.9, 0., 1.4);
	glRotatef(270., 1., 0., 0.);
	glCallList(GoalList);
	glPopMatrix();


	//draw the net
	SetMaterial(1.0, 1.0, 1.0, 1.0);
	Draw_Net((float)para_2);
	// draw some gratuitous text that just rotates on top of the scene:
	glDisable( GL_DEPTH_TEST );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the model view matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0., 100.,     0., 100. );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	DoRasterString(1., 97., 0., "F---Start/Pause Animation");
	DoRasterString(1., 94., 0., "B---Soccer Goal Camera");
	DoRasterString(1., 91., 0., "S---Side Field Camera");
	DoRasterString(1., 88., 0., "K---Ball Kicker Camera");
	// swap the double-buffered frame buffers:

	glutSwapBuffers( );


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	WhichColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Colors",        colormenu);
	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( NULL );

	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

}


void InitTexture() {
	width = 512;
	height = 256;
	level = 0;
	ncomps = 3;
	border = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(2, textures);
	Texture = BmpToTexture("Field.bmp", &width, &height);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, Texture);
	width = 1024;
	height = 512;
	Texture_1 = BmpToTexture("soccerball.bmp", &width, &height);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, level, ncomps, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, Texture_1);
	glBindTexture(GL_TEXTURE_2D, 0);
}



// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	float unit = 0.1;
	float width_add;
	float left_lower_x = -9;
	float left_lowcer_z = -6.3;
	GLUquadricObj *obj = gluNewQuadric();
	GLUquadricObj *obj_1 = gluNewQuadric();
	GLUquadricObj *obj_2 = gluNewQuadric();
	glutSetWindow( MainWindow );

	// create the field:
	FieldList = glGenLists(1);
	glNewList(FieldList, GL_COMPILE);
	for (int i = 0; i< 179; i++){
		width_add = float(i) * unit;
		glColor3f(Colors[WHITE][0], Colors[WHITE][1], Colors[WHITE][2]);
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j < 125; j++) {
			glTexCoord2f((width_add) / 18., unit * (float)j / 12.6);
			glVertex3f(left_lower_x + width_add, 0., left_lowcer_z + unit*(float)j);
			glTexCoord2f((width_add + unit) / 18.0, unit * (float)j / 12.6);
			glVertex3f(left_lower_x + width_add + unit, 0., left_lowcer_z + unit*(float)j);
		}
	}
	glEnd();
	glEndList();

	// create the goal:
	//vertical bar
	GoalList = glGenLists(1);
	glNewList(GoalList, GL_COMPILE);
	glColor3f(Colors[WHITE][0], Colors[WHITE][1], Colors[WHITE][2]);
	gluCylinder(obj, 0.02, 0.02, 1., 30, 30);
	glEndList();

	//horizontal bar
	GoalList_1 = glGenLists(1);
	glNewList(GoalList_1, GL_COMPILE);
	glColor3f(Colors[WHITE][0], Colors[WHITE][1], Colors[WHITE][2]);
	gluCylinder(obj_1, 0.02, 0.02, 2.8, 30, 30);
	glEndList();

	//side bar
	GoalList_2 = glGenLists(1);
	glNewList(GoalList_2, GL_COMPILE);
	glColor3f(Colors[WHITE][0], Colors[WHITE][1], Colors[WHITE][2]);
	gluCylinder(obj_2, 0.02, 0.02, 0.6, 30, 30);
	glEndList();
	// create the axes:
	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );

	
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;
		case 'k':
		case 'K':
			camera = 0;
			break;
		
		case 's':
		case 'S':
			camera = 1;
			break;
		
		case 'b':
		case 'B':
			camera = 2;
			break;

		case 'f':
		case 'F':
			Freeze = !Freeze;
			if (Freeze) {
				glutIdleFunc(NULL);
				Distort = FALSE;
			}
			else {
				glutIdleFunc(Animate);
				Distort = TRUE;
			}
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 0;
	DebugOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
	Freeze = FALSE;
	Distort = FALSE;
	para_1 = 0;
	para_2 = 0;
	camera = 0;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};

static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};

static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void	Draw_Net(float para_2) {
	float omt;
	float t;
	float x;
	float y;
	float z;
	float direction;
	float coordinate[1120][3];
	float coordinate_1[1400][3];
	float Cor_left_lower_front[3] = {-6.7, 0., -1.4};
	float Cor_left_lower_back[3] = {-7.3, 0., -1.4};
	float Cor_left_upper_front[3] = { -6.7, 0.8, -1.4 };
	float Cor_left_upper_back[3] = { -7.3, 0.8, -1.4 };
	float Cor_right_lower_front[3] = { -6.7, 0., 1.4 };
	float Cor_light_lower_back[3] = { -7.3, 0., 1.4 };
	float Cor_right_upper_front[3] = { -6.7, 0.8, 1.45 };
	float Cor_right_upper_back[3] = { -7.3, 0.8, 1.43 };
	//unit square 0.2*0.04
	float side_unit = 0.04;
	glColor3f(Colors[WHITE][0], Colors[WHITE][1], Colors[WHITE][2]);
	
	//left net
	for (int i = 0; i <= 15; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= 20; j++) {
			x = Cor_left_lower_front[0] - side_unit * (float)i;
			y = Cor_left_lower_front[1] + side_unit * (float)j;
			z = Cor_left_lower_front[2];
			if (para_2 > 60.0 && para_2 < 90  && j != 0 & j != 20) {
				z += 0.02 * sin(PI * x) * sin(para_2) / (para_2 - 60.);
			}
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	for (int i = 0; i <= 20; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= 15; j++) {
			x = Cor_left_lower_front[0] - side_unit * (float)j;
			y = Cor_left_lower_front[1] + side_unit * (float)i;
			z = Cor_left_lower_front[2];
			if (para_2 > 60.0 && para_2 < 90 && i != 0 & i != 20) {
				z += 0.02 * sin(PI * x) * sin(para_2) / (para_2 - 60.);
			}
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	//right net
	for (int i = 0; i <= 15; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= 20; j++) {
			x = Cor_right_lower_front[0] - side_unit * (float)i;
			y = Cor_right_lower_front[1] + side_unit * (float)j;
			z = Cor_right_lower_front[2];
			if (para_2 > 60.0 && para_2 < 90 && j != 0 & j != 20) {
				z += 0.02 * sin(PI * x) * sin(para_2) / (para_2 - 60.);
			}
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	for (int i = 0; i <= 20; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= 15; j++) {
			x = Cor_right_lower_front[0] - side_unit * (float)j;
			y = Cor_right_lower_front[1] + side_unit * (float)i;
			z = Cor_right_lower_front[2];
			if (para_2 > 60.0 && para_2 < 90 && i != 0 & i != 20) {
				z += 0.02 * sin(PI * x) * sin(para_2) / (para_2 - 60.);
			}
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	
	//top net
	//float Cor_left_upper_front[3] = { -6.7, 0.8, -1.4 };
	//float Cor_right_upper_front[3] = { -6.7, 0.8, 1.4 };
	float reference_1[3] = { -6.7, 0.76, -0.2 };
	float reference_2[3] = { -6.7, 0.76, 0.2 };
	float c_t = 0.15;
	if (para_2 > 60.0 && para_2 < 90) {
		reference_1[1] = 0.76 + abs(sin(PI * (para_2 - 60.) / 5.)) * (c_t - 0.06 * (float) ((int) para_2 % 4));
		reference_2[1] = 0.76 + abs(sin(PI * (para_2 - 60.) / 5.)) * (c_t - 0.06 * (float) ((int) para_2 % 4));
	}
	int index = 0;
	float drop_down_unit = 0.0;
	for (int i = 0; i < 16; i++) {
		if (i > 0) {
			if (i < 8) {
				drop_down_unit += 0.01;
			}
			else {
				drop_down_unit -= 0.01;
			}
		}
		glBegin(GL_LINE_STRIP);
		for (int it = 0; it < 70; it++)
		{
			t = (float)it / 70.;
			omt = 1.f - t;
			x = omt*omt*omt*(Cor_left_upper_front[0] - side_unit * (float)i) + 3.f*t*omt*omt*(reference_1[0] - side_unit * (float)i ) + 3.f*t*t*omt*(reference_2[0] - side_unit * (float)i) + t*t*t* (Cor_right_upper_front[0] - side_unit * (float)i);
			y = omt*omt*omt*(Cor_left_upper_front[1]) + 3.f*t*omt*omt*(reference_1[1] - abs(drop_down_unit)) + 3.f*t*t*omt*(reference_2[1] -abs(drop_down_unit)) + t*t*t* (Cor_right_upper_front[1]);
			z = omt*omt*omt*(Cor_left_upper_front[2]) + 3.f*t*omt*omt*(reference_1[2]) + 3.f*t*t*omt*(reference_2[2]) + t*t*t* (Cor_right_upper_front[2]);
			
			glVertex3f(x, y, z);
			if (i == 0)
			{
				y += 0.02;
			}
			coordinate[index][0] = x;
			coordinate[index][1] = y;
			coordinate[index][2] = z;
			index++;
		}
		glEnd();
	}
	
	for (int i = 0; i < 70; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 16; j++) {
			x = coordinate[i + 70 * j][0];
			y = coordinate[i + 70 * j][1];
			z = coordinate[i + 70 * j][2];
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	//back net
	//float Cor_left_upper_back[3] = { -7.3, 0.8, -1.4 };
	//float Cor_right_upper_back[3] = { -7.3, 0.8, 1.4 };
	//0.04 vertical shift 
	float y_shift =0.04;
	float reference_3[3] = { -7.3, 0.76, -0.7 };
	float reference_4[3] = { -7.3, 0.76, 0.7 };
	index = 0;
	for (int i = 0; i < 20; i++) {
		if (i == 0)
			y_shift = 0;
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 70; j++) {
			t = (float)j / 70.;
			omt = 1.f - t;
			x = omt*omt*omt*(Cor_left_upper_back[0] ) + 3.f*t*omt*omt*(reference_3[0]) + 3.f*t*t*omt*(reference_4[0]) + t*t*t* (Cor_right_upper_back[0] );
			y = omt*omt*omt*(Cor_left_upper_back[1] - side_unit * (float)i) + 3.f*t*omt*omt*(reference_3[1] - (side_unit) * (float)i -y_shift) + 3.f*t*t*omt*(reference_4[1] - (side_unit) * (float)i -y_shift) + t*t*t* (Cor_right_upper_back[1] - side_unit * (float)i);
			z = omt*omt*omt*(Cor_left_upper_back[2]) + 3.f*t*omt*omt*(reference_3[2]) + 3.f*t*t*omt*(reference_4[2]) + t*t*t* (Cor_right_upper_back[2]);
			if (para_2 > 60.0 && para_2 < 90) {
				x -= sin(PI*(z + 1)) * 0.001 / (float) (abs(j - 58 ) + 1) * (1. + sin((i - 5) * PI / 10)) * sin(para_2) / (para_2 - 60.);
				y += 0.03 *(1. + sin( (i - 5) * PI / 10)) / (float)(abs(j - 55) + 1) * sin(para_2) / (para_2 - 60.);
			}
			glVertex3f(x, y, z);
			coordinate_1[index][0] = x;
			coordinate_1[index][1] = y;
			coordinate_1[index][2] = z;
			index++;
		}
		glEnd();
		if (i == 0)
			y_shift = 0.02;
		y_shift -= 0.0012;
	}
	for (int i = 0; i < 70; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 20; j++) {
			x = coordinate_1[i + 70 * j][0];
			y = coordinate_1[i + 70 * j][1];
			z = coordinate_1[i + 70 * j][2];
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}
