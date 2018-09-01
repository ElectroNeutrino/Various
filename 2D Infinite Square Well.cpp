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
bool visible = true;
float step_size = 1.5;
float resolution = 75.0;
float length_x = 2.0;
float length_y = 2.0;
float count_x = 1.0;
float count_y = 1.0;
float theta;

GLfloat grey[] = {0.3, 0.3, 0.3, 1.0};
GLfloat white[] = {0.5, 0.5, 0.5, 1.0};
GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
GLfloat position[] = {-100.0, 0.0, -50.0, 0.0};

float Amplitude(float n, float m,  float x, float y)
{
    float amp = 2.0 / sqrt(length_x * length_y) * sin(n * M_PI * x / length_x) * sin(m * M_PI * y / length_y);
    if (step)
    {
        amp *= amp;
    }
    return amp;
}

float Normal_x(float n, float m,  float x, float y)
{
    float amp_x = 2.0 * n * M_PI / (sqrt(length_x * length_y) * length_x) * cos(n * M_PI * x / length_x) * sin(m * M_PI * y / length_y);
    if (step)
    {
        amp_x *= 4.0 / sqrt(length_x * length_y) * sin(n * M_PI * x / length_x) * sin(m * M_PI * y / length_y);
    }
    return amp_x;
}

float Normal_y(float n, float m,  float x, float y)
{
    float amp_y = 2.0 * m * M_PI / (sqrt(length_x * length_y) * length_y) * sin(n * M_PI * x / length_x) * cos(m * M_PI * y / length_y);
    if (step)
    {
        amp_y *= 4.0 / sqrt(length_x * length_y) * sin(n * M_PI * x / length_x) * sin(m * M_PI * y / length_y);
    }
    return amp_y;
}

void Set_Color_from_Value(float input, float maximum)
{
    float red, green, blue;
    if (input / maximum >= 0.0)
    {
        red = input / maximum;
        green = 1.0 - input / maximum;
        blue = 0.0;
    } else if (input <= 0.0) {
        red = 0.0;
        green = 1.0 + input / maximum;
        blue = (1.0 - input) / (2.0 * maximum);
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
    glBegin (GL_LINES);
        glColor3f (1.0f, 1.0f, 1.0f);
        glVertex3f (-length_x / 2.0, 0.0,-length_y / 2.0);
        glVertex3f ( length_x / 2.0, 0.0,-length_y / 2.0);
        
        glVertex3f ( length_x / 2.0, 0.0, length_y / 2.0);
        glVertex3f (-length_x / 2.0, 0.0, length_y / 2.0);
        
        glVertex3f (-length_x / 2.0, 0.0,-length_y / 2.0);
        glVertex3f (-length_x / 2.0, 0.0, length_y / 2.0);
        
        glVertex3f ( length_x / 2.0, 0.0,-length_y / 2.0);
        glVertex3f ( length_x / 2.0, 0.0, length_y / 2.0);
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
                glScalef(0.5, 0.5, 0.5);
                glRotatef(theta, 0.0, 1.0, 0.0);
                Draw_Bounds();
                if (visible)
                {
                    glBegin (GL_TRIANGLES);
                } else {
                    glBegin (GL_LINE_LOOP);
                }
/*                    glColor3f (0.0f, 0.0f, 1.0f);
                    glVertex2f (-length_x / 2.0, -1.25f + energy);
                    glVertex2f ( length_x / 2.0, -1.25f + energy);*/

                    for (float x = 0.0; x + length_x / resolution <= length_x; x += length_x / resolution)
                    {
                        for (float y = 0.0; y + length_y / resolution <= length_y; y += length_y / resolution)
                        {
                            float amp_1 = Amplitude(count_x, count_y, x, y) / sqrt(count_x * count_y);
                            float amp_2 = Amplitude(count_x, count_y, x, y + length_y / resolution) / sqrt(count_x * count_y);
                            float amp_3 = Amplitude(count_x, count_y, x + length_x / resolution, y + length_y / resolution) / sqrt(count_x * count_y);
                            float amp_4 = Amplitude(count_x, count_y, x + length_x / resolution, y) / sqrt(count_x * count_y);

                            float x1 = Normal_x(count_x, count_y, x, y) / sqrt(count_x * count_y);
                            float x2 = Normal_x(count_x, count_y, x, y + length_y / resolution) / sqrt(count_x * count_y);
                            float x3 = Normal_x(count_x, count_y, x + length_x / resolution, y + length_y / resolution) / sqrt(count_x * count_y);
                            float x4 = Normal_x(count_x, count_y, x + length_x / resolution, y) / sqrt(count_x * count_y);

                            float y1 = Normal_y(count_x, count_y, x, y) / sqrt(count_x * count_y);
                            float y2 = Normal_y(count_x, count_y, x, y + length_y / resolution) / sqrt(count_x * count_y);
                            float y3 = Normal_y(count_x, count_y, x + length_x / resolution, y + length_y / resolution) / sqrt(count_x * count_y);
                            float y4 = Normal_y(count_x, count_y, x + length_x / resolution, y) / sqrt(count_x * count_y);

                            float max_amp = 1.0 / sqrt(count_x * count_y);
                            float max_norm1 = sqrt(x1 * x1 + y1 * y1 + 1.0);
                            float max_norm2 = sqrt(x2 * x2 + y2 * y2 + 1.0);
                            float max_norm3 = sqrt(x3 * x3 + y3 * y3 + 1.0);
                            float max_norm4 = sqrt(x4 * x4 + y4 * y4 + 1.0);

                            Set_Color_from_Value(amp_1, max_amp);
                            glVertex3f (x - length_x / 2.0, amp_1, y - length_y / 2.0);
                            glNormal3f(x1 / max_norm1, 1.0 / max_norm1, y1 / max_norm1);

                            Set_Color_from_Value(amp_2, max_amp);
                            glVertex3f (x - length_x / 2.0, amp_2, y - length_y / 2.0 + length_y / resolution);
                            glNormal3f(x2 / max_norm2, 1.0 / max_norm2, y2 / max_norm2);

                            Set_Color_from_Value(amp_3, max_amp);
                            glVertex3f (x - length_x / 2.0 + length_x / resolution, amp_3, y - length_y / 2.0 + length_y / resolution );
                            glNormal3f(x3 / max_norm3, 1.0 / max_norm3, y3 / max_norm3);


                            Set_Color_from_Value(amp_1, max_amp);
                            glVertex3f (x - length_x / 2.0, amp_1, y - length_y / 2.0);
                            glNormal3f(x1 / max_norm1, 1.0 / max_norm1, y1 / max_norm1);

                            Set_Color_from_Value(amp_3, max_amp);
                            glVertex3f (x - length_x / 2.0 + length_x / resolution, amp_3, y - length_y / 2.0 + length_y / resolution );
                            glNormal3f(x3 / max_norm3, 1.0 / max_norm3, y3 / max_norm3);

                            Set_Color_from_Value(amp_4, max_amp);
                            glVertex3f (x - length_x / 2.0 + length_x / resolution, amp_4, y - length_y / 2.0);
                            glNormal3f(x4 / max_norm4, 1.0 / max_norm4, y4 / max_norm4);
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
                glRotatef(0.2, 1.0, 0.0, 0.0);
//                resolution *= 0.75;
                return 0;
            case VK_NEXT:
                glRotatef(-0.2, 1.0, 0.0, 0.0);
//                resolution *= 4.0/3.0;
                return 0;
            case VK_UP:
                count_y += 1.0;
                return 0;
            case VK_DOWN:
                if (count_y > 1.1)
                {
                    count_y -= 1.0;
                }
                return 0;
            case VK_LEFT:
                if (count_x > 1.1)
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
