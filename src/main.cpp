#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>

#include "../include/sdlglutils.h"
#include "../include/trackballcamera.h"

#define FPS 50
#define LARGEUR_FENETRE 1366
#define HAUTEUR_FENETRE 700

void DrawGL();

GLuint earth; //earth texture
TrackBallCamera * camera;

void stop()
{
    delete camera;
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    SDL_Event event;
    const Uint32 time_per_frame = 1000/FPS;
    unsigned int width = LARGEUR_FENETRE;
    unsigned int height = HAUTEUR_FENETRE;

    Uint32 last_time,current_time,elapsed_time; //for time animation
    Uint32 start_time,stop_time; //for frame limit

    SDL_Init(SDL_INIT_VIDEO);
    atexit(stop);

    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(width, height, 32, SDL_OPENGL);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective(70,(double)width/height,0.001,1000);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    earth = loadTexture("image/DroneText.jpg");
    camera = new TrackBallCamera();
    camera->setScrollSensivity(0.1);

    last_time = SDL_GetTicks();
    for (;;)
    {

        start_time = SDL_GetTicks();

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                exit(0);
                break;
                case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_p:
                    takeScreenshot("test.bmp");
                    break;
                    case SDLK_ESCAPE:
                    exit(0);
                    break;
                    default :
                    camera->OnKeyboard(event.key);
                }
                break;
                case SDL_MOUSEMOTION:
                camera->OnMouseMotion(event.motion);
                break;
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                camera->OnMouseButton(event.button);
                break;
            }
        }

        current_time = SDL_GetTicks();
        elapsed_time = current_time - last_time;
        last_time = current_time;

        DrawGL();

        stop_time = SDL_GetTicks();
        if ((stop_time - last_time) < time_per_frame)
        {
            //SDL_Delay(time_per_frame - (stop_time - last_time));
        }
    }

    return 0;
}


void DrawGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    camera->look();
    glClearColor(0.1,0.1,0.1,0.1);
    drawAxis();
    drawBox();

    GLUquadric* params = gluNewQuadric();
    gluQuadricTexture(params,GL_TRUE);
    glBindTexture(GL_TEXTURE_2D,earth);

    glTranslated(0.5,0,0);
    gluSphere(params,0.25,10,10);
    glTranslated(-1,0,0);
    gluSphere(params,0.25,10,10);
    glTranslated(0.5,0,0);

    gluDeleteQuadric(params);

    glFlush();
    SDL_GL_SwapBuffers();
}
