/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <time.h>
#include<sstream>
#include "Font.h"


/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

unsigned int width = 768;
unsigned int height = 768;

bool pause = true;
bool step = false;
bool visible = true;
float step_size = 1.5;
float resolution = 75.0;
float length_x = 1.0;
float count_x = 0.0;
float count_y = 0.0;
float theta, rotate_y = 0.0;

GLfloat grey[] = {0.3, 0.3, 0.3, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
GLfloat position[] = {-100.0, 0.0, -50.0, 0.0};

template <class my_temp>

inline std::string stringify(my_temp x)
{
    std::stringstream o;
    o << x;
    return o.str();
}

class Overloaded
{
    public:
        Overloaded(float input);
        float value;
};

Overloaded::Overloaded(float input)
{
    value = input;
}

float operator ^(Overloaded &lhs, int &rhs)
{
    float out;
    if (rhs > 0)
    {
        for (int n = 0; n < rhs; n++)
        {
            out *= lhs.value;
        }
    } else if (rhs < 0) {
        for (int n = 0; n < -rhs; n++)
        {
            out /= lhs.value;
        }
    } else {
        out = lhs.value;
    }
    return out;
}

unsigned int Factorial(int x)
{
    unsigned int out = 1;
    if (x < 0)
    {
        out = 0;
    } else if (x == 0) {
        out = 1;
    } else {
        int n = x;
        while(n > 1)
        {
            out *= n;
            n--;
        }
    }
    return out;
}        

float Bessel(float n, float x)
{
    float out;
    return out;
}

float Amplitude(float n, float m,  float x, float y)
{
    float amp = 0.5 * cos(m * y);
    if ((m < 0.01) && (m > -0.01))
    {
        amp *= cos((2.0 * n + 1.0) * M_PI * x / (2.0 * length_x)) / (exp(2.0 * x) + exp(-2.0 * x));
    } else {
        amp *= sin((n + 1.0) * M_PI * x / length_x) / (exp(2.0 * x) + exp(-2.0 * x));
    }
    if (step)
    {
        amp *= amp;
    }
    return amp;
}

float Normal_x(float n, float m,  float x, float y)
{
}

float Normal_y(float n, float m,  float x, float y)
{
}

void Set_Color_from_Value(float input, float maximum = 1.0)
{
    float red, green, blue;
    if (input / maximum >= 0.0)
    {
        red = 0.0;
        if (input / maximum >= 0.5)
        {
            blue = 1.0;
            green = 2.0 * (1.0 - input / maximum);
        } else {
            green = 1.0;
            blue = 2.0 * input / maximum;
        }
    } else if (input <= 0.0) {
        blue = 0.0;
        if (input / maximum <= -0.5)
        {
            red = 1.0;
            green = -2.0 * (1.0 - input / maximum);
        } else {
            green = 1.0;
            red = -2.0 * input / maximum;
        }
    } else {
        red = 0.0;
        green = 0.0;
        blue = 0.0;
    }
    glColor3f (red, green, blue);
    GLfloat color_value[] = {red, green, blue, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, color_value);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color_value);
}

void Draw_Bounds()
{
    glBegin (GL_LINE_LOOP);
        glColor3f (1.0f, 1.0f, 1.0f);
        for (float y = 0.0; y < 2.0 * M_PI; y += 2.0 * M_PI / resolution)
        {
            glVertex3f (length_x * cos(y), 0.0, length_x * sin(y));
        }
    glEnd();
}
/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, width, height,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);

    /* program main loop */
    float clock_start = float(clock());
    float last_clock = float(clock());
    float energy, e0;
    e0 = 0.01; // e0 = M_PI * M_PI * h_bar * h_bar / (2.0 * m * length_x * length_x);
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            energy = count_x * count_x * e0;

            glPushMatrix ();
                glMatrixMode(GL_PROJECTION);
                glPushMatrix ();
                glLoadIdentity();
                glOrtho(-1.0, 1.0, -float(height) / float(width), float(height) / float(width), 0.0, 20.0 );
                    glColor3fv(white);
                    glMaterialfv(GL_FRONT, GL_AMBIENT, white);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                    std::string text1 = "Radial: " + stringify(count_x);
                    std::string text2 = "Angular: " + stringify(count_y);
                    printString(text1.c_str(), 0.5, 0.9);
                    printString(text2.c_str(), 0.5, 0.85);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glScalef(0.5, 0.5, 0.5);
//                glRotatef(2.0 * theta, 0.0, 1.0, 0.0);
                glRotatef(rotate_y, 1.0, 0.0, 0.0);
                Draw_Bounds();
/*                    glColor3f (0.0f, 0.0f, 1.0f);
                    glVertex2f (-length_x / 2.0, -1.25f + energy);
                    glVertex2f ( length_x / 2.0, -1.25f + energy);*/

                    for (float x = 0.0; x < length_x; x += length_x / resolution)
                    {
                        for (float y = 0.0; y < 2.0 * M_PI; y += 2.0 * M_PI / resolution)
                        {
                            if (visible)
                            {
                                glBegin (GL_QUADS);
                            } else {
                                glBegin (GL_LINE_LOOP);
                            }

                            float amp_1 = Amplitude(count_x, count_y, x, y) * cos(2.0 * theta);
                            float amp_2 = Amplitude(count_x, count_y, x, y + 2.0 * M_PI / resolution) * cos(2.0 * theta);
                            float amp_3 = Amplitude(count_x, count_y, x + length_x / resolution, y + 2.0 * M_PI / resolution) * cos(2.0 * theta);
                            float amp_4 = Amplitude(count_x, count_y, x + length_x / resolution, y) * cos(2.0 * theta);
                            
                            float max_amp = 0.5;

                            if (step)
                            {
                                amp_1 *= 4.0;
                                amp_2 *= 4.0;
                                amp_3 *= 4.0;
                                amp_4 *= 4.0;
                            }

                            Set_Color_from_Value(amp_4, max_amp);
                            glVertex3f ((x + length_x / resolution) * cos(y), amp_4,(x + length_x / resolution) * sin(y));

                            Set_Color_from_Value(amp_3, max_amp);
                            glVertex3f ((x + length_x / resolution) * cos(y + 2.0 * M_PI / resolution), amp_3, (x + length_x / resolution) * sin(y + 2.0 * M_PI / resolution));

                            Set_Color_from_Value(amp_2, max_amp);
                            glVertex3f (x * cos(y + 2.0 * M_PI / resolution), amp_2, x * sin(y + 2.0 * M_PI / resolution));

                            Set_Color_from_Value(amp_1, max_amp);
                            glVertex3f (x * cos(y), amp_1, x * sin(y));

                            glEnd ();
                        }
                    }
            glPopMatrix ();

            SwapBuffers (hDC);
            if (!pause)
            {
                theta += (float (clock()) - last_clock) * step_size / CLOCKS_PER_SEC;
            }
            last_clock = float(clock());
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
            case VK_ESCAPE:
                PostQuitMessage(0);
                return 0;
            case VK_SPACE:
                pause = !pause;
                return 0;
            case VK_PRIOR:
                rotate_y += 0.4;
//                resolution *= 0.75;
                return 0;
            case VK_NEXT:
                rotate_y -= 0.4;
//                resolution *= 4.0/3.0;
                return 0;
            case VK_UP:
                count_y += 1.0;
                return 0;
            case VK_DOWN:
                if (count_y > 0.1)
                {
                    count_y -= 1.0;
                }
                return 0;
            case VK_LEFT:
                if (count_x > 0.1)
                {
                    count_x -= 1.0;
                }
                return 0;
            case VK_RIGHT:
                count_x += 1.0;
                return 0;
            case 'S':
                step = !step;
                return 0;
            case 'A':
                if (visible)
                {
                    glDisable(GL_LIGHTING);
                } else {
                    glEnable(GL_LIGHTING);
                }
                visible = !visible;
                return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, grey);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -float(height) / float(width), float(height) / float(width), 0.0, 20.0 );
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.25, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, black);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
    glMaterialfv(GL_FRONT, GL_EMISSION, black);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}


/*

a[0] = (1.0 / L) * Integral( f(x) , x, -L / 2.0, L / 2.0)
a[n] = (2.0 / L) * Integral( f(x) * cos(2.0 * n * M_PI * x / L) , x, -L / 2.0, L / 2.0)
b[n] = (2.0 / L) * Integral( f(x) * sin(2.0 * n * M_PI * x / L) , x, -L / 2.0, L / 2.0)

( a[n] * cos(2.0 * n * (x / resolution) * M_PI / L) ) + ( b[n] * sin(2.0 * n * (x / resolution) * M_PI / L) )

*/
