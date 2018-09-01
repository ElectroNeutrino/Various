#include <GL/gl.h>
#include <math.h>
#include "Functions.h"

float res = 60.0;
float angle = 0.0;

void SetView()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0.0, 0.0, zoom);
    glRotatef(turn, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glRotatef(tilt[count], 0.0, 0.0, -1.0);
    glRotatef(angle / rotation[count], 0.0, 1.0, 0.0);
    glScalef(1.0, oblateness[count], 1.0);
    angle += 0.2;
}

void DisplayPlanet()
{
    glBindTexture(GL_TEXTURE_2D, Current_texture);
    for (int x = 0; x < res; x++)
    {
        for (int y = 0; y < (res / 2); y++)
        {
            float y1_scalar = sin(2.0 * M_PI * y / res);
            float y2_scalar = sin(2.0 * M_PI * (y + 1) / res);
            float x1_position = sin(2.0 * M_PI * x / res) * y1_scalar;
            float x2_position = sin(2.0 * M_PI * (x + 1) / res) * y1_scalar;
            float x3_position = sin(2.0 * M_PI * (x + 1) / res) * y2_scalar;
            float x4_position = sin(2.0 * M_PI * x / res) * y2_scalar;
            float z1_position = -cos(2.0 * M_PI * x / res) * y1_scalar;
            float z2_position = -cos(2.0 * M_PI * (x + 1) / res) * y1_scalar;
            float z3_position = -cos(2.0 * M_PI * (x + 1) / res) * y2_scalar;
            float z4_position = -cos(2.0 * M_PI * x / res) * y2_scalar;
            float y1_position = cos(2.0 * M_PI * y / res);
            float y2_position = cos(2.0 * M_PI * (y + 1) / res);
            glBegin(GL_TRIANGLES);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x1_position, y1_position, z1_position);
                    glVertex3f(x1_position, y1_position, z1_position);
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x2_position, y1_position, z2_position);
                    glVertex3f(x2_position, y1_position, z2_position);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x4_position, y2_position, z4_position);
                    glVertex3f(x4_position, y2_position, z4_position);

                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x2_position, y1_position, z2_position);
                    glVertex3f(x2_position, y1_position, z2_position);
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x3_position, y2_position, z3_position);
                    glVertex3f(x3_position, y2_position, z3_position);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x4_position, y2_position, z4_position);
                    glVertex3f(x4_position, y2_position, z4_position);
            glEnd();
        }
    }
}

void DisplayClouds()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, Earth_Cloud_texture);
    for (int x = 0; x < res; x++)
    {
        for (int y = 0; y < (res / 2); y++)
        {
            float y1_scalar = sin(2.0 * M_PI * y / res);
            float y2_scalar = sin(2.0 * M_PI * (y + 1) / res);
            float x1_position = sin(2.0 * M_PI * x / res) * y1_scalar;
            float x2_position = sin(2.0 * M_PI * (x + 1) / res) * y1_scalar;
            float x3_position = sin(2.0 * M_PI * (x + 1) / res) * y2_scalar;
            float x4_position = sin(2.0 * M_PI * x / res) * y2_scalar;
            float z1_position = -cos(2.0 * M_PI * x / res) * y1_scalar;
            float z2_position = -cos(2.0 * M_PI * (x + 1) / res) * y1_scalar;
            float z3_position = -cos(2.0 * M_PI * (x + 1) / res) * y2_scalar;
            float z4_position = -cos(2.0 * M_PI * x / res) * y2_scalar;
            float y1_position = cos(2.0 * M_PI * y / res);
            float y2_position = cos(2.0 * M_PI * (y + 1) / res);
            glBegin(GL_TRIANGLES);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x1_position, y1_position, z1_position);
                    glVertex3f(x1_position, y1_position, z1_position);
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x2_position, y1_position, z2_position);
                    glVertex3f(x2_position, y1_position, z2_position);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x4_position, y2_position, z4_position);
                    glVertex3f(x4_position, y2_position, z4_position);
    
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x2_position, y1_position, z2_position);
                    glVertex3f(x2_position, y1_position, z2_position);
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x3_position, y2_position, z3_position);
                    glVertex3f(x3_position, y2_position, z3_position);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x4_position, y2_position, z4_position);
                    glVertex3f(x4_position, y2_position, z4_position);
            glEnd();
        }
    }
}

void DisplayLights()
{
    glMaterialfv(GL_FRONT, GL_EMISSION, my_grey);
    glBindTexture(GL_TEXTURE_2D, Earth_Lights_texture);
    for (int x = 0; x < res; x++)
    {
        for (int y = 0; y < (res / 2); y++)
        {
            float y1_scalar = sin(2.0 * M_PI * y / res);
            float y2_scalar = sin(2.0 * M_PI * (y + 1) / res);
            float x1_position = sin(2.0 * M_PI * x / res) * y1_scalar;
            float x2_position = sin(2.0 * M_PI * (x + 1) / res) * y1_scalar;
            float x3_position = sin(2.0 * M_PI * (x + 1) / res) * y2_scalar;
            float x4_position = sin(2.0 * M_PI * x / res) * y2_scalar;
            float z1_position = -cos(2.0 * M_PI * x / res) * y1_scalar;
            float z2_position = -cos(2.0 * M_PI * (x + 1) / res) * y1_scalar;
            float z3_position = -cos(2.0 * M_PI * (x + 1) / res) * y2_scalar;
            float z4_position = -cos(2.0 * M_PI * x / res) * y2_scalar;
            float y1_position = cos(2.0 * M_PI * y / res);
            float y2_position = cos(2.0 * M_PI * (y + 1) / res);
            glBegin(GL_TRIANGLES);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y) * 2.0 / res));
                    glVertex3f(x1_position, y1_position, z1_position);
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y) * 2.0 / res));
                    glVertex3f(x2_position, y1_position, z2_position);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y + 1) * 2.0 / res));
                    glVertex3f(x4_position, y2_position, z4_position);
    
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y) * 2.0 / res));
                    glVertex3f(x2_position, y1_position, z2_position);
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y + 1) * 2.0 / res));
                    glVertex3f(x3_position, y2_position, z3_position);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y + 1) * 2.0 / res));
                    glVertex3f(x4_position, y2_position, z4_position);
            glEnd();
        }
    }
    glMaterialfv(GL_FRONT, GL_EMISSION, my_black);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void DisplaySpecular()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, Specular_alpha);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, my_black);
    glMaterialfv(GL_FRONT, GL_SPECULAR, my_blue);
    float temp = res;
    res *= 2.0;
    for (int x = 0; x < res; x++)
    {
        for (int y = 0; y < (res / 2); y++)
        {
            float y1_scalar = sin(2.0 * M_PI * y / res);
            float y2_scalar = sin(2.0 * M_PI * (y + 1) / res);
            float x1_position = sin(2.0 * M_PI * x / res) * y1_scalar;
            float x2_position = sin(2.0 * M_PI * (x + 1) / res) * y1_scalar;
            float x3_position = sin(2.0 * M_PI * (x + 1) / res) * y2_scalar;
            float x4_position = sin(2.0 * M_PI * x / res) * y2_scalar;
            float z1_position = -cos(2.0 * M_PI * x / res) * y1_scalar;
            float z2_position = -cos(2.0 * M_PI * (x + 1) / res) * y1_scalar;
            float z3_position = -cos(2.0 * M_PI * (x + 1) / res) * y2_scalar;
            float z4_position = -cos(2.0 * M_PI * x / res) * y2_scalar;
            float y1_position = cos(2.0 * M_PI * y / res);
            float y2_position = cos(2.0 * M_PI * (y + 1) / res);
            glBegin(GL_TRIANGLES);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x1_position, y1_position, z1_position);
                    glVertex3f(x1_position, y1_position, z1_position);
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x2_position, y1_position, z2_position);
                    glVertex3f(x2_position, y1_position, z2_position);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x4_position, y2_position, z4_position);
                    glVertex3f(x4_position, y2_position, z4_position);
    
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y) * 2.0 / res));
                glNormal3f(x2_position, y1_position, z2_position);
                    glVertex3f(x2_position, y1_position, z2_position);
                glTexCoord2f(1.0 - (float(x + 1) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x3_position, y2_position, z3_position);
                    glVertex3f(x3_position, y2_position, z3_position);
                glTexCoord2f(1.0 - (float(x) / res), 1.0 - (float(y + 1) * 2.0 / res));
                glNormal3f(x4_position, y2_position, z4_position);
                    glVertex3f(x4_position, y2_position, z4_position);
            glEnd();
        }
    }
    res = temp;
    glMaterialfv(GL_FRONT, GL_SPECULAR, my_black);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, my_white);
}

void DisplayRings()
{
    float inside_ring, outside_ring;
    if (Current_texture == Saturn_texture)
    {
        inside_ring = 1.3;
        outside_ring = 2.5;
        glBindTexture(GL_TEXTURE_2D, Saturn_Ring_texture);
    } else if (Current_texture == Uranus_texture) {
        inside_ring = 1.64;
        outside_ring = 2.0;
        glBindTexture(GL_TEXTURE_2D, Uranus_Ring_texture);
    }
    glDisable(GL_LIGHTING);
    for (int x = 0; x < res; x++)
    {
        float x1_position = sin(2.0 * M_PI * x / res);
        float x2_position = sin(2.0 * M_PI * (x + 1) / res);
        float z1_position = cos(2.0 * M_PI * x / res);
        float z2_position = cos(2.0 * M_PI * (x + 1) / res);
        glBegin(GL_TRIANGLES);
            glNormal3f(0.0, 1.0, 0.0);
            glTexCoord2f(0.0, 0.0);
                glVertex3f(inside_ring * x1_position, 0.0, inside_ring * z1_position);
            glTexCoord2f(1.0, 0.0);
                glVertex3f(outside_ring * x1_position, 0.0, outside_ring * z1_position);
            glTexCoord2f(0.0, 0.0);
                glVertex3f(inside_ring * x2_position, 0.0, inside_ring * z2_position);

            glTexCoord2f(1.0, 0.0);
                glVertex3f(outside_ring * x1_position, 0.0, outside_ring * z1_position);
                glVertex3f(outside_ring * x2_position, 0.0, outside_ring * z2_position);
            glTexCoord2f(0.0, 0.0);
                glVertex3f(inside_ring * x2_position, 0.0, inside_ring * z2_position);

            glNormal3f(0.0, -1.0, 0.0);
                glVertex3f(inside_ring * x2_position, 0.0, inside_ring * z2_position);
            glTexCoord2f(1.0, 0.0);
                glVertex3f(outside_ring * x2_position, 0.0, outside_ring * z2_position);
            glTexCoord2f(0.0, 0.0);
                glVertex3f(inside_ring * x1_position, 0.0, inside_ring * z1_position);

            glTexCoord2f(1.0, 0.0);
                glVertex3f(outside_ring * x2_position, 0.0, outside_ring * z2_position);
                glVertex3f(outside_ring * x1_position, 0.0, outside_ring * z1_position);
            glTexCoord2f(0.0, 0.0);
                glVertex3f(inside_ring * x1_position, 0.0, inside_ring * z1_position);
        glEnd();
    }
    glEnable(GL_LIGHTING);
}
