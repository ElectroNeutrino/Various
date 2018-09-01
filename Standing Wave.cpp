/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <gl/gl.h>
#include <math.h>


/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

bool show_yellow = true;
bool show_red = false;
bool show_green = false;
bool pause = true;
float step = 0.001;
float frequency = 2.0;
float resolution = 50.0;

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
    float theta = 0.0f;

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
    float amplitude = 0.5;
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
    
                    if (show_yellow)
                    {
                        glColor3f (1.0f, 1.0f, 0.0f);
                        for (float x = 0.0; x < resolution; x = x + 1.0)
                        {
                            glVertex2f ( (x / resolution - 0.5) * length, amplitude * sin(frequency * (x / resolution) * M_PI) * sin(frequency * theta) );
                            glVertex2f ( ( (x + 1.0) / resolution - 0.5) * length, amplitude * sin(frequency * ( (x + 1.0) / resolution) * M_PI) * sin(frequency * theta) );
                        }
                    }
                    if (show_red)
                    {
                        glColor3f (1.0f, 0.0f, 0.0f);
                        for (float x = 0.0; x < resolution; x = x + 1.0)
                        {
                            glVertex2f ( (x / resolution - 0.5) * length, -amplitude * sin( (frequency * x / resolution) * M_PI - frequency * theta - M_PI / 2.0) );
                            glVertex2f ( ( (x + 1.0) / resolution - 0.5) * length, -amplitude * sin( (frequency * (x + 1.0) / resolution) * M_PI - frequency * theta - M_PI / 2.0) );
                        }
                    }
                    if (show_green)
                    {
                        glColor3f (0.0f, 1.0f, 0.0f);
                        for (float x = 0.0; x < resolution; x = x + 1.0)
                        {
                            glVertex2f ( (x / resolution - 0.5) * length, amplitude * sin( (frequency * x / resolution) * M_PI + frequency * theta - M_PI / 2.0) );
                            glVertex2f ( ( (x + 1.0) / resolution - 0.5) * length, amplitude * sin( (frequency * (x + 1.0) / resolution) * M_PI + frequency * theta - M_PI / 2.0) );
                        }
                    }
                glEnd ();
            glPopMatrix ();

            SwapBuffers (hDC);
            if (!pause)
            {
                theta += step;
//                pause = !pause;
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
                step *= 2.0;
                return 0;
            case VK_DOWN:
                step *= 0.5;
                return 0;
            case VK_LEFT:
                if (frequency - 1.0 > 0.0)
                {
                    frequency -= 1.0;
                }
                return 0;
            case VK_RIGHT:
                frequency += 1.0;
                return 0;
            case '1':
                show_yellow = !show_yellow;
                return 0;
            case '2':
                show_red = !show_red;
                return 0;
            case '3':
                show_green = !show_green;
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
