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
unsigned int resolution = 30;
float length = 1.5;
float rotate = 0.0;
float r = 1.0;
float tau;
unsigned int principle_number = 3;
unsigned int angular_number = 1;
int magnetic_number = 0;

unsigned int factorial(unsigned int x)
{
    unsigned int out;
    if (x == 0)
    {
        out = 1;
    } else {
        out = x * factorial(x - 1); // n! = n * (n-1)!
    }
    return out;
}

float P_lm(unsigned int l, int m, float x)
{
    float f = 1.0 - x * x;
    float out;
    if ((l < 0) || (abs(m) > l))
    {
        out = NAN;
    }

    switch (l)
    {
        case 0:
            // *****    l = 0
            out = 1.0;
            break;
        case 1:
            // *****    l = 1
            switch (m)
            {
                case -1:
                    out = 0.5 * sqrt(f);  // m = -1
                    break;
                case 0:
                    out = x;              // m = 0
                    break;
                case 1:
                    out = -sqrt(f);       // m = 1
                    break;
            }
            break;
        case 2:
            // *****    l = 2
            switch (m)
            {
                case -2:
                    out = 1.0 / 8.0 * f;               // m = -2
                    break;
                case -1:
                    out = 0.5 * x * sqrt(f);           // m = -1
                    break;
                case 0:
                    out = 0.5 * sqrt(2.0 - 3.0 * f); // m = 0
                    break;
                case 1:
                    out = -3.0 * x * sqrt(f);         // m = 1
                    break;
                case 2:
                    out = 3.0 * f;                    // m = 2
                    break;
            }
            break;
        case 3:
            // *****    l = 3
            switch (m)
            {
                case -3:
                    out = 1.0 / 48.0 * f * sqrt(f);                 // m = -3
                    break;
                case -2:
                    out = 1.0 / 8.0 * x * f;                        // m = -2
                    break;
                case -1:
                    out = 1.0 / 8.0 * (4.0 - 5.0 * f) * sqrt(f);    // m = -1
                    break;
                case 0:
                    out = 0.5 * x * (2.0 - 5.0 * f);                // m = 0
                    break;
                case 1:
                    out = -3.0 / 2.0 * (4.0 - 5.0 * f) * sqrt(f);   // m = 1
                    break;
                case 2:
                    out = 15.0 * x * f;                             // m = 2
                    break;
                case 3:
                    out = -15.0 * f * sqrt(f);                      // m = 3
                    break;
            }
            break;
        default:
            out = NAN;
    }
    return out;
}

float Spherical_Harmonic (unsigned int l, int m, float theta, float phi)
{
    float out;
/*    if (abs(m) > l)
    {
        out = NAN;      // out of bounds indices
        return out;
    }*/

    float p_plus = cos(m * phi);
    float p_minus = sin(m * phi);
    float n = sqrt(2.0 * (float(factorial(l - m)) / float(factorial(l + m))) * (2.0 * float(l) + 1.0) / (4.0 * M_PI));
    if (m < 0)
    {
        out = n * P_lm(l, m, cos(theta)) * p_minus;
    } else {
        out = n * P_lm(l, m, cos(theta)) * p_plus;
    }
    return out;
}

float Laguerre(unsigned int n, unsigned int a, float x)
{
    float out;
    switch (n)
    {
        case 0:
            out = 1.0;
            break;
        case 1:
            out = -x + float(a) + 1.0;
            break;
        case 2:
            out = 0.5 * (x * x - 2.0 * (float(a) + 2.0) * x + float((a + 1) * (a + 2)));
            break;
        case 3:
            out = 1.0 / 6.0 * (-pow(x, 3) + 3.0 * (float(a) + 3.0) * x * x - 3.0 * float((a + 2) * (a + 3)) * x + float((a + 1) * (a + 2) * (a + 3)));
            break;
        case 4:
            out = 1.0 / 24.0 * (pow(x, 4) - 4.0 * float(a + 4) * pow(x, 3) + 6.0 * float((a + 3) * (a + 4)) * x * x - 4.0 * float((a + 2) * (a + 3) * (a + 4)) * x + float((a + 1) * (a + 2) * (a + 3) * (a + 4)));
    }
    return out;
}

float Radial_Equation(unsigned int n, unsigned int l, float x)
{
    float a0 = 1.0;
//    float norm = sqrt(pow(2.0 / (float(n) * a0), 3) * float(factorial(n - l - 1)) / (float(2 * n) * float(factorial(n + l))));
    float norm = 1.0;
    float out = norm * pow(2.0 * x / (float(n) * a0), l) * pow(M_E, -x / (float(n) * a0)) * Laguerre(n - l - 1, 2 * l + 1, 2.0 * x / (float(n) * a0));
//    float out = 1.0;
    return out;
}

void Color_Scale(float input, float* color_array)
{
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    if ((input > 0.0) && (input <= 1.0 / 3.0))         // Red
    {
        red = 3.0 * input;
    } else if ((input > 1.0 / 3.0) && (input <= 2.0 / 3.0)) {
        red = 2.0 - 3.0 * input;
    } else {
        red = 0.0;
    }

    if ((input > 1.0 / 3.0) && (input <= 2.0 / 3.0))         // Green
    {
        green = 3.0 * input - 1.0;
    } else if (input > 2.0 / 3.0) {
        green = 3.0 - 3.0 * input;
    } else {
        green = 0.0;
    }

    if (input > 2.0 / 3.0)        // Blue
    {
        blue = 3.0 * input - 2.0;
    } else {
        blue = 0.0;
    }

    color_array[0] = red;
    color_array[1] = green;
    color_array[2] = blue;
    color_array[3] = input;

    if (input > 1.00)
    {
        color_array[0] = 0.0;
        color_array[1] = 0.0;
        color_array[2] = 1.0;
        color_array[3] = 1.0;
    } else if (input < 0.00) {
        color_array[0] = 0.0;
        color_array[1] = 0.0;
        color_array[2] = 0.0;
        color_array[3] = 0.0;
    }
}

void Draw_Harmonic(unsigned int n, unsigned int l, int m)
{
    float amp1, amp2, amp3, amp4, color_array[4];
    glPushMatrix ();
        glTranslatef(0.0, 0.0, 0.0);
        glScalef(0.075, 0.075, 0.075);
        glRotatef(rotate, 0.0, 0.0, 1.0);
        glBegin (GL_TRIANGLES);
        float delta = 2.0 * M_PI / float(resolution);
            for (unsigned int d = 0; d < 60; d++)
            {
                float z = float(d) / 2.0;
                for (unsigned int x = 0; x < resolution; x++)
                {
                    float phi = x * delta;
                    for (unsigned int y = 0; y < resolution / 2; y++)
                    {
                        float theta = y * delta;
                        amp1 = Radial_Equation(n, l, z) * Spherical_Harmonic(l, m, theta, phi);
                        amp2 = Radial_Equation(n, l, z) * Spherical_Harmonic(l, m, theta + delta, phi);
                        amp3 = Radial_Equation(n, l, z) * Spherical_Harmonic(l, m, theta + delta, phi + delta);
                        amp4 = Radial_Equation(n, l, z) * Spherical_Harmonic(l, m, theta, phi + delta);
        
                        Color_Scale(amp1 * amp1, color_array);
                        glColor4fv(color_array);
    //                        z = 0.5 * amp1 * amp1;
                            glVertex3f(z * cos(phi) * sin(theta), z * sin(phi) * sin(theta), z * cos(theta));
        
                        Color_Scale(amp3 * amp3, color_array);
                        glColor4fv(color_array);
    //                        z = 0.5 * amp3 * amp3;
                            glVertex3f(z * cos(phi + delta) * sin(theta + delta), z * sin(phi + delta) * sin(theta + delta), z * cos(theta + delta));
    
                        Color_Scale(amp4 * amp4, color_array);
                        glColor4fv(color_array);
    //                        z = 0.5 * amp4 * amp4;
                            glVertex3f(z * cos(phi + delta) * sin(theta), z * sin(phi + delta) * sin(theta), z * cos(theta));    
    
    
                        Color_Scale(amp1 * amp1, color_array);
                        glColor4fv(color_array);
    //                        z = 0.5 * amp1 * amp1;
                            glVertex3f(z * cos(phi) * sin(theta), z * sin(phi) * sin(theta), z * cos(theta));
        
                        Color_Scale(amp2 * amp2, color_array);
                        glColor4fv(color_array);
    //                        z = 0.5 * amp2 * amp2;
                            glVertex3f(z * cos(phi) * sin(theta + delta), z * sin(phi) * sin(theta + delta), z * cos(theta + delta));
        
                        Color_Scale(amp3 * amp3, color_array);
                        glColor4fv(color_array);
    //                        z = 0.5 * amp3 * amp3;
                            glVertex3f(z * cos(phi + delta) * sin(theta + delta), z * sin(phi + delta) * sin(theta + delta), z * cos(theta + delta));
                    }
                }
            }
        glEnd ();
    glPopMatrix ();
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
    float color_array[3];
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

            Draw_Harmonic(principle_number, angular_number, magnetic_number);

            glClear (GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
                unsigned int scale_max = 10;
                glBegin (GL_LINE_LOOP);
                    glColor3f(1.0, 1.0, 1.0);
                        glVertex3f(0.0, 0.85, 0.25);
                        glVertex3f(0.0, 0.90, 0.25);
                        glVertex3f(0.0, 0.90, -0.25);
                        glVertex3f(0.0, 0.85, -0.25);
                glEnd ();
                glBegin (GL_QUADS);
                    for (unsigned int x = 0; x < scale_max; x++)
                    {
                        Color_Scale(1.0 - float(x) / float(scale_max), color_array);
                        glColor3fv(color_array);
                            glVertex3f(0.0, 0.85, 0.25 - float(x) / float(2 * scale_max));

                        glColor3fv(color_array);
                            glVertex3f(0.0, 0.90, 0.25 - float(x) / float(2 * scale_max));

                        Color_Scale(1.0 - float(x + 1) / float(scale_max), color_array);
                        glColor3fv(color_array);
                            glVertex3f(0.0, 0.90, 0.25 - float(x + 1) / float(2 * scale_max));

                        glColor3fv(color_array);
                            glVertex3f(0.0, 0.85, 0.25 - float(x + 1) / float(2 * scale_max));
                    }
                glEnd ();
            glPopMatrix ();

            SwapBuffers (hDC);
            if (!pause)
            {
                tau = (float (clock()) - last_clock) * step_size / CLOCKS_PER_SEC;
                rotate += tau * 25.0;
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
            case VK_TAB:
                if (principle_number == 4)
                {
                    principle_number = 1;
                } else {
                    principle_number++;
                }
                angular_number = 0;
                magnetic_number = 0;
                return 0;
            case VK_PRIOR:
                if ((angular_number == 3) || (angular_number + 1 == principle_number))
                {
                    angular_number = 0;
                } else {
                    angular_number++;
                }
                magnetic_number = 0;
                return 0;
            case VK_NEXT:
                if (magnetic_number == angular_number)
                {
                    magnetic_number = 0;
                } else {
                    magnetic_number++;
                }
                return 0;
            case VK_UP:
                r += 0.025;
                return 0;
            case VK_DOWN:
                r -= 0.025;
                return 0;
            case VK_LEFT:
                rotate -= 5.0;
                return 0;
            case VK_RIGHT:
                rotate += 5.0;
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(-1.0, 1.0, -float(height) / float(width), float(height) / float(width), -1.0, 50.0 );
    gluPerspective(45.0, float(width) / float(height), 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt (5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
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
