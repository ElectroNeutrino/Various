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
float resolution = 50.0;
float theta = 0.0;
float h = 0.75;
float r = 0.0;
float q = 0.9;
unsigned int count = 0;
const unsigned int max_count = 150;
float a_n[max_count], b_n[max_count];

float a(unsigned int n)
{
    float a_out;
    if (n == 0)
    {
        a_out = h / 2.0;
    } else {
        a_out = 2.0 * h / ((r * r - 1.0) * float(n * n) * M_PI * M_PI) * (cos(float(n) * M_PI) - cos(r * float(n) * M_PI));
    }
    return a_out;
}

float b(unsigned int n)
{
    float b_out;
    if (n == 0)
    {
        b_out = 0.0;
    } else {
        b_out = 2.0 * h / ((r * r - 1.0) * float(n * n) * M_PI * M_PI) * (r * sin(float(n) * M_PI) - sin(r * float(n) * M_PI));
    }
    return b_out;
}

float amplitude (float x)
{
    float amp = 0.0;
    for (unsigned int n = 0; n <= count; n++)
    {
        amp += (a_n[n] * cos(float(n) * x * M_PI) + b_n[n] * sin(float(n) * x * M_PI)) * cos(2.0 * float(n) * M_PI * theta) * exp((1.0 - 1/q) * n *theta);
    }
    return amp;
}

void Populate_Coefficients()
{
    for (unsigned int n = 0; n < max_count; n++)
    {
        a_n[n] = a(n);
        b_n[n] = b(n);
    }
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
    float length = 1.5;
    Populate_Coefficients();
    float clock_start = float(clock());
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
                    for (float x = 0.0; x < resolution; x = x + 1.0)
                    {
                        glVertex2f ( (x / resolution - 0.5) * length, amplitude(x / resolution - 0.5 + r / 2.0) - 0.25);
                        glVertex2f ( ( (x + 1.0) / resolution - 0.5) * length, amplitude((x + 1.0) / resolution - 0.5 + r / 2.0) - 0.25);
                    }
                glEnd ();
            glPopMatrix ();

            SwapBuffers (hDC);
            if (pause)
            {
                clock_start = float(clock()) - theta * CLOCKS_PER_SEC;
            } else {
                theta = (float(clock()) - clock_start) / CLOCKS_PER_SEC;
                if (step)
                {
                    pause = !pause;
                }
            }
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
                if (count < max_count)
                {
                    count += 1;
                }
                return 0;
            case VK_DOWN:
                if (count > 0)
                {
                    count -= 1;
                }
                return 0;
            case VK_LEFT:
                if (r > -0.90)
                {
                    r -= 0.05;
                }
                Populate_Coefficients();
                return 0;
            case VK_RIGHT:
                if (r < 0.90)
                {
                    r += 0.05;
                }
                Populate_Coefficients();
                return 0;
            case 'S':
                step = !step;
                return 0;
            case 'R':
                pause = true;
                step = false;
                resolution = 50.0;
                theta = 0.0;
                r = 0.0;
                count = 0;
                Populate_Coefficients();
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

a[0] = (1.0 / L) * Integral( f(x) * cos(2.0 * M_PI * x / L) , x, -L / 2.0, L / 2.0)
a[n] = (2.0 / L) * Integral( f(x) * cos(2.0 * n * M_PI * x / L) , x, -L / 2.0, L / 2.0)
b[n] = (2.0 / L) * Integral( f(x) * sin(2.0 * n * M_PI * x / L) , x, -L / 2.0, L / 2.0)

( a[n] * cos(2.0 * n * (x / resolution) * M_PI / L) ) + ( b[n] * sin(2.0 * n * (x / resolution) * M_PI / L) )

*/
