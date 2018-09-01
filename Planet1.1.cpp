#include<stdio.h>
#include<stdlib.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
#include<time.h>
#include "Functions.h"
#include "Display.h"

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
unsigned int MAXFRAMES = 500;

int main ()
{
    unsigned int frames = 0;
    float current_time = clock();
    bool lmouse = false;
    bool rmouse = false;
    unsigned int mouse_x, mouse_y, old_mouse_x, old_mouse_y;
    short win_width = 600;
    short win_height = 600;
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
    } else {
    	printf("\n\tvisual %p selected\n", vi->visualid);/* %p creates hexadecimal output like in glxinfo */
    }
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    win = XCreateWindow(dpy, root, 0, 0, win_width, win_height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Planets");
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
    SetupGL();
    if (LoadTextures() == 0)
    {
        printf("\tAll textures loaded.\n");
    } else {
        printf("\tError loading textures.\n");
        prog_quit = true;
    }
    glBindTexture(GL_TEXTURE_2D, Sun_texture);
    Current_texture = Sun_texture;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, my_white);
    glMaterialfv(GL_FRONT, GL_AMBIENT, my_black);
    glMaterialfv(GL_FRONT, GL_EMISSION, my_white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, my_black);
    glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
    while(!prog_quit)
    {
        while (XPending(dpy)!=0)
        {
            XNextEvent(dpy, &xev);
            if (xev.type == Expose)
            {
                XGetWindowAttributes(dpy, win, &gwa);
                glViewport(0, 0, gwa.width, gwa.height);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity ();
                gluPerspective(35.0, float(gwa.width)/float(gwa.height), 0.1, 100.0);
                glMatrixMode(GL_MODELVIEW);
//                printf("\tExpose event.\n");
            } else if (xev.type == KeyPress) {
                KeyFunction(XLookupKeysym(&xev.xkey, 0));
//                printf("\tKeyPress event.\n");
            } else if (xev.type == ButtonPress) {
//                printf("\tButtonPress event.\n");
                if (xev.xbutton.button == Button1)
                {
                    lmouse = true;
                } else if (xev.xbutton.button == Button2) {
                    rmouse = true;
                }
            } else if (xev.type == ButtonRelease) {
//                printf("\tButtonRelease event.\n");
                if (xev.xbutton.button == Button1)
                {
                    lmouse = false;
                } else if (xev.xbutton.button == Button2) {
                    rmouse = false;
                }
            } else if (xev.type == MotionNotify) {
                old_mouse_x = mouse_x;
                old_mouse_y = mouse_y;
                mouse_x = xev.xmotion.x;
                mouse_y = xev.xmotion.y;
                if (lmouse)
                {
                    if (mouse_x > old_mouse_x)
                    {
                        KeyFunction(XK_Left);
                    } else if (mouse_x < old_mouse_x) {
                        KeyFunction(XK_Right);
                    }
                    if (mouse_y > old_mouse_y)
                    {
                        KeyFunction(XK_Down);
                    } else if (mouse_y < old_mouse_y) {
                        KeyFunction(XK_Up);
                    }
                } else if (rmouse) {
                }
            }
        }
//        printf("\tglPushMatrix().\n");
        glPushMatrix();
            SetView();
            if (Current_texture == Earth_texture)
            {
                DisplayLights();
                DisplayPlanet();
                DisplaySpecular();
                DisplayClouds();
            } else if ((Current_texture == Saturn_texture) || (Current_texture == Uranus_texture)) {
                DisplayPlanet();
                DisplayRings();
            } else {
                DisplayPlanet();
            }
//        printf("\tglPopMatrix().\n");
        glPopMatrix ();
//        printf("\t    Display Output.\n");
        glXSwapBuffers(dpy, win);
//        printf("\t    Buffers Swapped.\n");
        if (frames == MAXFRAMES)
        {
            printf("\tFPS = %.2f\n", MAXFRAMES / ((clock() - current_time) / CLOCKS_PER_SEC));
            current_time = clock();
            frames = 0;
        }
        frames++;
    }
    printf("\tClosing program.\n");
    glXMakeCurrent(dpy, None, NULL);
    glXDestroyContext(dpy, glc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}
