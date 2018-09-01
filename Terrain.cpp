#include<stdio.h>
#include<stdlib.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
#include<math.h>
#include "Bitmap.h"
#include "Font.h"

Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;
double scale = 2048.0;

void SetupGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 1.0, 1.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

int main(int argc, char *argv[])
{
	dpy = XOpenDisplay(NULL);
	if(dpy == NULL)
	{
		printf("\n\tcannot connect to X server\n\n");
		exit(0);
	}
	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);
	if(vi == NULL)
	{
		printf("\n\tno appropriate visual found\n\n");
	        exit(0);
	}
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
	win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "VERY SIMPLE APPLICATION");
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	SetupGL();
	Bmp_File my_terrain("Terrain.bmp");
	GLuint color_data;
	glGenTextures(1, &color_data);
	glBindTexture(GL_TEXTURE_2D, color_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	Bmp_File terrain_colors("Terrain Colors.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, terrain_colors.Width, terrain_colors.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, terrain_colors.Data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, color_data);
	double angle = 0.0;
	double angle2 = 0.0;
	double zoom = 1.5;
	double aspect_ratio = double(my_terrain.Height) / double(my_terrain.Width);
	unsigned int polygons = 0;
	glColor3f(1.0, 1.0, 1.0);
	GLuint terrain;
	terrain = glGenLists (1);
	glNewList(terrain, GL_COMPILE);
		for (int x = 0; x < my_terrain.Height - 1; x++)
		{
			for (int z = 0; z < my_terrain.Width - 1; z++)
			{
				double x1 = (double(x) / double(my_terrain.Height)) - 0.5;
				double x2 = (double(x + 1) / double(my_terrain.Height)) - 0.5;
				double z1 = (double(z) / double(my_terrain.Width)) - 0.5;
				double z2 = (double(z + 1) / double(my_terrain.Width)) - 0.5;
				double y1 = double(my_terrain.Data[((x * my_terrain.Width) + z) * 3]) / scale;
				double y2 = double(my_terrain.Data[(((x + 1) * my_terrain.Width) + z) * 3]) / scale;
				double y3 = double(my_terrain.Data[(((x + 1) * my_terrain.Width) + (z + 1)) * 3]) / scale;
				double y4 = double(my_terrain.Data[((x * my_terrain.Width) + (z + 1)) * 3]) / scale;
				glBegin (GL_TRIANGLES);
					polygons++;

					glTexCoord2d(z1 + 0.5, x1 + 0.5);
					glVertex3d(x1 * aspect_ratio, y1, z1);

					glTexCoord2d(z1 + 0.5, x2 + 0.5);
					glVertex3d(x2 * aspect_ratio, y2, z1);

					glTexCoord2d(z2 + 0.5, x2 + 0.5);
					glVertex3d(x2 * aspect_ratio, y3, z2);

					polygons++;

					glTexCoord2d(z1 + 0.5, x1 + 0.5);
					glVertex3d(x1 * aspect_ratio, y1, z1);

					glTexCoord2d(z2 + 0.5, x2 + 0.5);
					glVertex3d(x2 * aspect_ratio, y3, z2);

					glTexCoord2d(z2 + 0.5, x1 + 0.5);
					glVertex3d(x1 * aspect_ratio, y4, z2);
				glEnd ();
			}
		}
	glEndList();
	while(1)
	{
		while (XPending(dpy)!=0)
		{
			XNextEvent(dpy, &xev);
			if(xev.type == Expose)
			{
				XGetWindowAttributes(dpy, win, &gwa);
				glViewport(0, 0, gwa.width, gwa.height);
			} else if(xev.type == KeyPress) {
				switch (XLookupKeysym(&xev.xkey, 0))
				{
					case XK_Escape:
						glXMakeCurrent(dpy, None, NULL);
						glXDestroyContext(dpy, glc);
						XDestroyWindow(dpy, win);
						XCloseDisplay(dpy);
						exit(0);
						break;
					case XK_s:
						scale *= 2.0;
						break;
					case XK_a:
						scale /= 2.0;
						break;
					case XK_Prior:
						zoom *= 1.1;
						break;
					case XK_Next:
						zoom /= 1.1;
						break;
					case XK_Up:
						if (angle2 >= 360.0)
						{
							angle2 -= (355.0);
						} else {
							angle2 += 5.0;
						}
						break;
						break;
					case XK_Down:
						if (angle2 <= 0.0)
						{
							angle2 += (355.0);
						} else {
							angle2 -= 5.0;
						}
						break;
					case XK_Right:
						if (angle >= 360.0)
						{
							angle -= (355.0);
						} else {
							angle += 5.0;
						}
						break;
					case XK_Left:
						if (angle <= 0.0)
						{
							angle += (355.0);
						} else {
							angle -= 5.0;
						}
						break;
					case XK_space:
						printf("\tNumber of polygons: %d\n", polygons);
						break;
					default:
						break;
				}
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix ();
			glRotatef(angle2, 1.0, 0.0, 0.0);
			glRotatef(angle, 0.0, 1.0, 0.0);
			glScalef(zoom, zoom, zoom);
			glCallList(terrain);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();
				glColor3f(1.0, 1.0, 1.0);
				printString("Terrain Test", -0.85, 0.95);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glXSwapBuffers(dpy, win);
	} /* this closes while(1) { */
} /* this is the } which closes int main(int argc, char *argv[]) { */
