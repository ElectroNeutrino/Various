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


unsigned int MAXFRAMES = 500;
double MIN_MASS = 0.5;
double MAX_MASS = 1.0;
double DELTA_MASS = MAX_MASS - MIN_MASS;
double gravity = -1.0;
GLuint disk;
bool Move = false;
bool Speed = true;
bool Step = false;

class Particle
{
    public:
        Particle();
        Particle(double, double, double, double, double, double);
        void Draw();
        void DrawVelocity();
        void UpdatePosition(double);
        double x, y;
        double vx, vy;
        double radius;
        double mass;
        double collide_time;

};

Particle::Particle()
{

}

Particle::Particle(double p1, double p2, double v1, double v2, double r, double m)
{
    radius = r;
    mass = m;
    x = p1;
    y = p2;
    vx = v1;
    vy = v2;
}

void Particle::Draw()
{
    double mass_color = (mass - MIN_MASS) / DELTA_MASS;
    double draw_color = 0.5 * (1.0 + mass_color);
    glColor3f(draw_color, draw_color, draw_color);
    glPushMatrix();
        glTranslated(x, y, 0.0);
        glScaled(radius, radius, 1.0);
        glCallList(disk);
    glPopMatrix();
}

void Particle::DrawVelocity()
{
    glBegin(GL_LINES);
            glVertex2d(x, y);
            glVertex2d(x + vx * 0.25, y + vy * 0.25);
    glEnd();
}

void Particle::UpdatePosition(double dt)
{
    x  += vx * dt;
    y  += vy * dt;
    vy += gravity * dt;

}

void CollideUpdate(Particle* particle, unsigned int count, unsigned int collide_n, unsigned int collide_m)
{
    if (collide_n == collide_m) // top or bottom
    {
//        printf("\tCollision between particle %i and top/bottom wall.\n", collide_n);
        particle[collide_n].vy *= -1.0;
    } else if (collide_m == count) { // left or right
//        printf("\tCollision between particle %i and left/right wall.\n", collide_n);
        particle[collide_n].vx *= -1.0;
    } else {
//        printf("\tCollision between particle %i and particle %i.\n", collide_n, collide_m);

        double vx_avg, vy_avg, v1_x_prime, v1_y_prime, v2_x_prime, v2_y_prime;
        double dx, dy, distance, normal_x, normal_y;

        double v_norm_1, v_norm_2, v_norm_1_x, v_norm_1_y, v_norm_2_x, v_norm_2_y;
        double v_parallel_1_x, v_parallel_1_y, v_parallel_2_x, v_parallel_2_y;

        double vx_n_prime, vy_n_prime, vx_m_prime, vy_m_prime, vx_n, vy_n, vx_m, vy_m;

        vx_avg = (particle[collide_n].mass * particle[collide_n].vx + particle[collide_m].mass * particle[collide_m].vx) / (particle[collide_n].mass + particle[collide_m].mass); // v_avg = (p1 + p2) / (m1 + m2) = ((m1 * v1) + (m2 * v2)) / (m1 + m2)
        vy_avg = (particle[collide_n].mass * particle[collide_n].vy + particle[collide_m].mass * particle[collide_m].vy) / (particle[collide_n].mass + particle[collide_m].mass);

        v1_x_prime = particle[collide_n].vx - vx_avg; // difference from average velocity, v' = v - v_avg
        v1_y_prime = particle[collide_n].vy - vy_avg;
        v2_x_prime = particle[collide_m].vx - vx_avg;
        v2_y_prime = particle[collide_m].vy - vy_avg;

        dx = particle[collide_m].x - particle[collide_n].x;
        dy = particle[collide_m].y - particle[collide_n].y;

        distance = sqrt(dx * dx + dy * dy);
        normal_x = dx / distance; // unit normal to the contact surface
        normal_y = dy / distance;

        v_norm_1 = v1_x_prime * normal_x + v1_y_prime * normal_y; // velocity component magnitude normal to contact surface
        v_norm_2 = v2_x_prime * normal_x + v2_y_prime * normal_y;

        v_norm_1_x = v_norm_1 * normal_x; // velocity normal x and y components
        v_norm_1_y = v_norm_1 * normal_y;
        v_norm_2_x = v_norm_2 * normal_x;
        v_norm_2_y = v_norm_2 * normal_y;

        v_parallel_1_x = v1_x_prime - v_norm_1_x; // v' = v_norm + v_parallel
        v_parallel_1_y = v1_y_prime - v_norm_1_y;
        v_parallel_2_x = v2_x_prime - v_norm_2_x;
        v_parallel_2_y = v2_y_prime - v_norm_2_y;

        vx_n_prime = v_parallel_1_x - v_norm_1_x; // new v' = -v_norm + v_parallel
        vy_n_prime = v_parallel_1_y - v_norm_1_y;
        vx_m_prime = v_parallel_2_x - v_norm_2_x;
        vy_m_prime = v_parallel_2_y - v_norm_2_y;

        vx_n = vx_n_prime + vx_avg; // new v' = new v - va_vg
        vy_n = vy_n_prime + vy_avg;
        vx_m = vx_m_prime + vx_avg;
        vy_m = vy_m_prime + vy_avg;

        particle[collide_n].vx = vx_n;
        particle[collide_n].vy = vy_n;
        particle[collide_m].vx = vx_m;
        particle[collide_m].vy = vy_m;
    }
}

double Update(Particle* particle, unsigned int count, double dt)
{
    double min_distance, delta_x, delta_y, delta_2, delta_vx, delta_vy, delta_v2;
    double a, b, c, radix_2, radix, tc1, tc2, tc, min_tc;
    double ret_val = -1.0;

    min_tc = 2.0 * dt;

    unsigned int collide_n, collide_m;
    bool top_wall = false;
    bool collide = false;

    for (unsigned int n = 0; n < count - 1; n++) // Check for two particles colliding
    {
        for (unsigned int m = n + 1; m < count; m++)
        {
            min_distance = particle[n].radius + particle[m].radius;

            delta_x = particle[m].x - particle[n].x; // delta p = p2 - p1
            delta_y = particle[m].y - particle[n].y;

            delta_2 = delta_x * delta_x + delta_y * delta_y;

            delta_vx = particle[m].vx - particle[n].vx; // delta v = v2 - v1
            delta_vy = particle[m].vy - particle[n].vy;

            delta_v2 = delta_vx * delta_vx + delta_vy * delta_vy;

            if ((delta_2 - min_distance) <= (delta_v2 * dt)) // check if separation <= distance travelled
            {
                double p_dot_v = delta_x * delta_vx + delta_y * delta_vy;
                if (p_dot_v < 0.0) // p dot v < 0 if v is bringing particles closer
                {
                    a = delta_vx * delta_vx + delta_vy * delta_vy;

                    b = 2.0 * (delta_x * delta_vx + delta_y * delta_vy);

                    c = delta_x * delta_x + delta_y * delta_y - min_distance * min_distance;

                    radix_2 = (b * b) - (4.0 * a * c);
                    radix = sqrt(radix_2);

                    if ((a > 0.0) && (radix_2 >= 0.0))
                    {
                        tc1 = (-b + radix) / (2.0 * a);
                        tc2 = (-b - radix) / (2.0 * a);
                    } else {
                        tc1 = -1.0;
                    }

                    // tc1 >= tc2

                    if (tc1 < 0.0)           // Both negative
                    {
                        tc = 10.0 * min_tc;
                    } else if (tc2 < 0.0) { // Only tc2 negative
                        tc = tc1;
                    } else {                // Both positive
                        tc = tc2;
                    }

                    if (tc < min_tc) // Collision occurs within time frame.
                    {
                        min_tc = tc;
                        collide_n = n;
                        collide_m = m;
                        collide = true;
                    }
                }
            }
        }
    }

    for (unsigned int n = 0; n < count; n++) // Check if collision occurs against wall.
    {

        double t_wall_0, t_wall_1, t_wall_2, t_wall_3;

        t_wall_0 = -1.0;
        t_wall_2 = -1.0;
        t_wall_1 = -1.0;
        t_wall_3 = -1.0;
        if (particle[n].y > 0.0)
        {
            if (particle[n].vy > 0.0) {
                t_wall_0 = ( 1.0 - particle[n].radius - particle[n].y) / particle[n].vy; // Wall 0 (top) collide time
            }
        } else if (particle[n].y < 0.0) {
            if (particle[n].vy < 0.0) {
                t_wall_2 = (-1.0 + particle[n].radius - particle[n].y) / particle[n].vy; // Wall 2 (bottom) collide time
            }
        }

        if (particle[n].x > 0.0)
        {
            if (particle[n].vx > 0.0) {
                t_wall_1 = ( 1.0 - particle[n].radius - particle[n].x) / particle[n].vx; // Wall 1 (right) collide time
            }
        } else if (particle[n].x < 0.0) {
            if (particle[n].vx < 0.0) {
                t_wall_3 = (-1.0 + particle[n].radius - particle[n].x) / particle[n].vx; // Wall 3 (left) collide time
            }
        }

        if ((t_wall_0 < min_tc) && (t_wall_0 >= 0.0)) // top
        {
            collide_n = n;
            collide_m = n;
            min_tc = t_wall_0;
            collide = true;
        } else if ((t_wall_1 < min_tc) && (t_wall_1 >= 0.0)) { // right
            collide_n = n;
            collide_m = count;
            min_tc = t_wall_1;
            collide = true;
        } if ((t_wall_2 < min_tc) && (t_wall_2 >= 0.0)) { // bottom
            collide_n = n;
            collide_m = n;
            min_tc = t_wall_2;
            collide = true;
        } if ((t_wall_3 < min_tc) && (t_wall_3 >= 0.0)) { // left
            collide_n = n;
            collide_m = count;
            min_tc = t_wall_3;
            collide = true;
        }
    }

    if (collide)
    {
        for (unsigned int n = 0; n < count; n++) // Update all particles to the first collision time.
        {
            particle[n].UpdatePosition(min_tc);
        }
        CollideUpdate(particle, count, collide_n, collide_m); // Change the velocities of colliding particles
        ret_val = min_tc;
//        Update(particle, count, dt - min_tc); // Continue after collision for the remainder of the time
    } else {
        ret_val = -1.0;
        for (unsigned int n = 0; n < count; n++) // Update all particles to the first collision time.
        {
            particle[n].UpdatePosition(dt);
        }
    }
    return ret_val;
}

double QuickUpdate(Particle* particle, unsigned int count, double dt)
{
    double min_distance, delta_x, delta_y, delta_2, delta_vx, delta_vy, delta_v2;
    double a, b, c, radix_2, radix, tc1, tc2, tc;
    double ret_val = -1.0;

    for (unsigned int n = 0; n < count - 1; n++) // Check for two particles colliding
    {
        for (unsigned int m = n + 1; m < count; m++)
        {
            min_distance = particle[n].radius + particle[m].radius;

            delta_x = particle[m].x - particle[n].x; // delta p = p2 - p1
            delta_y = particle[m].y - particle[n].y;

            delta_2 = delta_x * delta_x + delta_y * delta_y;

            delta_vx = particle[m].vx - particle[n].vx; // delta v = v2 - v1
            delta_vy = particle[m].vy - particle[n].vy;

            delta_v2 = delta_vx * delta_vx + delta_vy * delta_vy;

            if ((delta_2 - min_distance) <= (delta_v2 * dt)) // check if separation <= distance travelled
            {
                double p_dot_v = delta_x * delta_vx + delta_y * delta_vy;
                if (p_dot_v < 0.0) // p dot v < 0 if v is bringing particles closer
                {
                    a = delta_vx * delta_vx + delta_vy * delta_vy;

                    b = 2.0 * (delta_x * delta_vx + delta_y * delta_vy);

                    c = delta_x * delta_x + delta_y * delta_y - min_distance * min_distance;

                    radix_2 = (b * b) - (4.0 * a * c);
                    radix = sqrt(radix_2);

                    if ((a > 0.0) && (radix_2 >= 0.0))
                    {
                        tc1 = (-b + radix) / (2.0 * a);
                        tc2 = (-b - radix) / (2.0 * a);
                    } else {
                        tc1 = -1.0;
                    }

                    // tc1 >= tc2

                    if (tc1 < 0.0)           // Both negative
                    {
                        tc = -1.0 * dt;
                    } else if (tc2 < 0.0) { // Only tc2 negative
                        tc = tc1;
                    } else {                // Both positive
                        tc = tc2;
                    }

                    if ((tc < dt) && (tc >= 0.0))
                    {
                        particle[n].UpdatePosition(tc);
                        particle[m].UpdatePosition(tc);

                        CollideUpdate(particle, count, n, m);

                        particle[n].x = particle[n].x - particle[n].vx * tc;
                        particle[n].y = particle[n].y - particle[n].vy * tc;

                        particle[m].x = particle[m].x - particle[m].vx * tc;
                        particle[m].y = particle[m].y - particle[m].vy * tc;
                    }
                }
            }
        }
    }

    for (unsigned int n = 0; n < count; n++) // Check if collision occurs against wall.
    {
        double r = 1.0 - particle[n].radius;
        if (particle[n].x > r)
        {
            particle[n].x = 2.0 * r - particle[n].x;
            particle[n].vx *= -1.0;
        } else if (particle[n].x < -r){
            particle[n].x = -2.0 * r - particle[n].x;
            particle[n].vx *= -1.0;
        }
        if (particle[n].y > r)
        {
            particle[n].y = 2.0 * r - particle[n].y;
            particle[n].vy *= -1.0;
        } else if (particle[n].y < -r){
            particle[n].y = -2.0 * r - particle[n].y;
            particle[n].vy *= -1.0;
        }
        particle[n].UpdatePosition(dt);
    }
    return ret_val;
}

double GetRandom(double min = -1.0, double max = 1.0)
{
    double rand_number = double(rand()) / double(RAND_MAX); // random number from 0.0 to 1.0
    double range = max - min;
    double ret_val = min + range * rand_number;
    return ret_val;
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
    unsigned int frames = 0;
    double current_time = clock();
    int random_seed = 546254;
    srand(random_seed);

    unsigned int particle_count = 1000;

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
      0, 0, 600, 600,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);

    /* program main loop */
	disk = glGenLists(1);
	glNewList(disk, GL_COMPILE);
	    glBegin(GL_POLYGON);
            for(int theta = 0; theta < 60; theta++)
            {
                glVertex2d(cos(double(theta) * M_PI / 30.0), sin(double(theta) * M_PI / 30.0));
            }
        glEnd();
    glEndList();
//        particle_count = 2;
    	Particle *particle_list = new Particle[particle_count];
    	for(unsigned int n = 0; n < particle_count; n++)
    	{
            particle_list[n] = Particle(GetRandom(), GetRandom(), GetRandom(), GetRandom(), GetRandom(0.05 / sqrt(double(particle_count)), 0.075 / sqrt(double(particle_count))), GetRandom(MIN_MASS, MAX_MASS)); // (x, y, vx, vy, r, m)
    	}
//        particle_list[0] = Particle(-0.5, 0.0,  0.2, 0.0, 0.1, 5.0); // (x, y, vx, vy, r, m)
//        particle_list[1] = Particle( 0.5, 0.0, -1.0, 0.0, 0.1, 1.0); // (x, y, vx, vy, r, m)

    	unsigned int clock_start = clock();
    	double t = 0.0;
    	double t_old, dt, dt_sum;
    	unsigned int FRAME_COUNT = 0;
    	double t_start = 0.0;
    	dt_sum = 0.0;
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

    		glPushMatrix();
                for(unsigned int n = 0; n < particle_count; n++)
                {
                    particle_list[n].Draw();
                    if (Step)
                    {
                        particle_list[n].DrawVelocity();
                    }
                }
    		glPopMatrix();

            SwapBuffers (hDC);

    		t_old = t;
    		t = double(clock() - clock_start) / double(CLOCKS_PER_SEC);
    		dt = t - t_old;

            if (!Move)
            {
                current_time = clock();
                frames = 0;
            }
    		if(Move)
    		{
    	        if (Speed)
    	        {
                    QuickUpdate(particle_list, particle_count, dt);
    	        } else {
                    double time_value = dt;
                    while (time_value >=0.0)
                    {
                        time_value = Update(particle_list, particle_count, time_value);
                    }
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
        case VK_RETURN:
            Move = !Move;
            return 0;
        case 'S':
            Speed = !Speed;
            return 0;
        case VK_SPACE:
            Step = !Step;
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
