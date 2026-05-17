#include "scene.h"
#include <math.h>
#include <GL/gl.h>

void init_scene(Scene* scene)
{
}

void update_scene(Scene* scene)
{
}

void render_scene(const Scene* scene)
{
    draw_origin();
    draw_triangle();
    draw_sphere();


}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();

}

void draw_triangle()
{
    glBegin(GL_TRIANGLES);

    glColor3f(1, 1, 0);
    glVertex3f(1, 0, 0);
    glColor3f(0, 1, 1);
    glVertex3f(0, 1, 0);
    glColor3f(1, 0, 1);
    glVertex3f(0, 0, 1);

    glEnd();
}

void draw_sphere()
{
    double angle1;
    double angle2;
    double angle;
    double x;
    double y;
    double z;
    /*glPushMatrix();
    
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.9, 0.8);
    glVertex2f(0, 0);
    angle = 0;
    while (angle < 2.0 * M_PI + 1) {
        x = cos(angle) ;
        y = sin(angle) ;
        glVertex2f(x, y);
        angle += 0.3;
    }
    glEnd();
    glPopMatrix();*/

    
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.9, 0.8);
    angle1=0;
    angle2=-M_PI ;
    for(;angle1 < 2.0 * M_PI +1 ; angle1 += 0.3)
    {
        for(;angle2 < M_PI + 1 ; angle2+= 0.3)
        {
            x=sin(angle1)*cos(angle2);
            y=sin(angle1)*sin(angle2);
            z=cos(angle1);
            glVertex3f(x, y,z);
        }
    }

    glEnd();
}