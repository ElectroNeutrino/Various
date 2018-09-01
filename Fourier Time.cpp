/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <gl/gl.h>
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

bool pause = true;
bool step = false;
float step_size = 1.0;
float resolution = 50.0;
float h = 0.5;
float length = 1.5;
unsigned int count = 0;
float theta;

float a(float n)
{
    float a_n;
    if (n > 0.1)
    {
        a_n = 2.0 * h / (n * n * M_PI * M_PI) * (1.0 - cos(n * M_PI) );
    } else {
        a_n = h / 2.0;
    }
    return a_n;
}

float b(float n)
{
    float b_n;
    if (n > 0.1)
    {
        b_n = 0.0;
    } else {
        b_n = 0.0;
    }
    return b_n;
}

float amplitude (float x)
{
    float amp = 0.0;
    for (float n = 0.0; n <= float(count) + 0.1; n = n + 1.0)
    {
        amp += (a(n) * cos(n * (x / resolution) * M_PI) + b(n) * sin(n * (x / resolution) * M_PI) ) * cos(2.0 * n * M_PI * theta);
    }
    return amp;
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
      0, 0, 512, 512,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);

    /* program main loop */
    float clock_start = float(clock());
    float n, b_n;
    float last_clock = float(clock());
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

            glClear (GL_COLOR_BUFFER_BIT);

            glPushMatrix ();
                glBegin (GL_LINES);
                    glColor3f (1.0f, 1.0f, 1.0f);
                    glVertex2f (-length / 2.0,  1.0f);
                    glVertex2f (-length / 2.0, -0.7f);
    
                    glVertex2f (-length / 2.0, -0.7f);
                    glVertex2f ( length / 2.0, -0.7f);
    
                    glVertex2f ( length / 2.0,  1.0f);
                    glVertex2f ( length / 2.0, -0.7f);
    
                    glColor3f (0.0f, 1.0f, 0.0f);
                    for (float n = 0.0; n <= float(count) + 0.1; n = n + 1.0)
                    {
                        for (float x = 0.0; x < resolution - 0.1; x = x + 1.0)
                        {
                            glVertex2f ( (x / resolution - 0.5) * length, amplitude(x - 0.5 * resolution) );
                            glVertex2f ( ( (x + 1.0) / resolution - 0.5) * length, amplitude(x + 1.0 - 0.5 * resolution) );
                        }
                    }
                glEnd ();
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
                resolution *= 0.75;
                return 0;
            case VK_NEXT:
                resolution *= 4.0/3.0;
                return 0;
            case VK_UP:
                step_size *= 2.0;
                return 0;
            case VK_DOWN:
                step_size *= 0.5;
                return 0;
            case VK_LEFT:
                if (count > 0.1)
                {
                    count -= 1;
                }
                return 0;
            case VK_RIGHT:
                count += 1;
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
