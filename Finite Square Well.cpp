/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <time.h>


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
float step_size = 1.0;
float resolution = 200.0;
float length = 1.5;
float count = 1.0;
float theta;
float energy = 0.5;
float potential = 1.0;

// **** FIX THIS FUNCTION!! ****

float amplitude (float n, float x)
{
    float alpha = sqrt(energy);
    float beta = sqrt(potential - energy);
    if (potential <= energy)
    {
        beta = sqrt(energy - potential);
    }
    float gamma = exp(beta * length / 2.0) * cos(alpha * length / 2.0);
    float mu = -exp(beta * length / 2.0) * sin(alpha * length / 2.0);
    float amp;
    if (x <= -length / 2.0)
    {
        if (energy < potential)
        {
            amp = (gamma * (1.0 - cos(n * M_PI)) - mu * (1.0 + cos(n * M_PI))) / 2.0 * exp(beta * x);
        } else {
            amp = sin(beta * x);
        }
    } else if ((x >= -length / 2.0) && (x <= length / 2.0)) {
        amp = (1.0 + cos((n - 1.0) * M_PI)) / 2.0 * cos(alpha * x) - (1.0 - cos((n - 1.0) * M_PI)) / 2.0 * sin(alpha * x);
    } else if (x >= length / 2.0) {
        if (energy < potential)
        {
            amp = (gamma * (1.0 - cos(n * M_PI)) + mu * (1.0 + cos(n * M_PI))) / 2.0 * exp(-beta * x);
        } else {
            amp = sin(beta * x);
        }
    } else {
        amp = 0.0;
    }
    if (!pause)
    {
        amp *= amp;
    }
    return amp;
}

float Test (float n)
{
    bool out = false;
    float alpha = sqrt(energy);
    float beta = sqrt(potential - energy);
    if (potential <= energy)
    {
        out = false;
    }
    float plus = (alpha / beta) * tan(alpha * length / 2.0);
    float minus = -(beta / alpha) * tan(alpha * length / 2.0);
    if (cos(n * M_PI) < 0.0)
    {
        if ((0.99 < plus) && (plus < 1.01))
        {
            out = true;
        } else {
            out = false;
        }
    } else if (cos(n * M_PI) > 0.0) {
        if ((0.99 < minus) && (minus < 1.01))
        {
            out = true;
        } else {
            out = false;
        }
    } else {
        out - false;
    }
    return out;
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
    float e0;
    e0 = 0.01; // e0 = M_PI * M_PI * h_bar * h_bar / (2.0 * m * length * length);
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            } else {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        } else {
            /* OpenGL animation code goes here */

            glClear (GL_COLOR_BUFFER_BIT);

            glPushMatrix ();
                glScalef(0.5, 0.5, 0.5);
                glBegin (GL_LINES);
                    glColor3f (1.0f, 1.0f, 1.0f);
                    glVertex2f (-length / 2.0,  1.25f);
                    glVertex2f (-length / 2.0, -1.25f);
    
                    glVertex2f (-2.0, -1.25f);
                    glVertex2f ( 2.0, -1.25f);
    
                    glVertex2f ( length / 2.0,  1.25f);
                    glVertex2f ( length / 2.0, -1.25f);
    
                    glColor3f (0.0f, 0.0f, 1.0f);
                    glVertex2f (-length / 2.0, -1.25f + energy);
                    glVertex2f ( length / 2.0, -1.25f + energy);

                    glColor3f (1.0f, 0.0f, 0.0f);
                    glVertex2f (-2.0, -1.25f + potential);
                    glVertex2f (-length / 2.0, -1.25f + potential);

                    glVertex2f ( length / 2.0, -1.25f + potential);
                    glVertex2f ( 2.0, -1.25f + potential);

                    glColor3f (1.0f, 1.0f, 0.0f);
                    if (Test(count))
                    {
                        glColor3f (0.0f, 1.0f, 0.0f);
                    }
                    for (float x = -2.0; x < 2.0 - length / resolution; x = x + length / resolution)
                    {
                        glVertex2f (x, amplitude(count, x));
                        glVertex2f (x + length / resolution, amplitude(count, x + length / resolution));
                    }
                glEnd ();
            glPopMatrix ();

            SwapBuffers (hDC);
/*            if (!pause)
            {
                theta += (float (clock()) - last_clock) * step_size / CLOCKS_PER_SEC;
            }
            last_clock = float(clock());*/
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
                energy += 0.01;
                return 0;
            case VK_NEXT:
                if (energy >= 0.001)
                {
                    energy -= 0.01;
                }
                return 0;
            case VK_UP:
                potential += 0.01;
                return 0;
            case VK_DOWN:
                if (potential >= 0.011)
                {
                    potential -= 0.01;
                }
                return 0;
            case VK_LEFT:
                if (count > 1.1)
                {
                    count -= 1.0;
                }
                return 0;
            case VK_RIGHT:
                count += 1.0;
                return 0;
            case 'S':
                step = !step;
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -float(height) / float(width), float(height) / float(width) );
    glMatrixMode(GL_MODELVIEW);
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
