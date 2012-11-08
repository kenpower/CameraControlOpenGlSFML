
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>

#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class Camera{
	static aiVector3D zero,yaxis,zaxis;
public:
	aiVector3D position;
	aiVector3D forward;
	aiVector3D up;

	float forwardSpeed;
	float roationSpeed;
	
	Camera():forwardSpeed(20.5f),roationSpeed(0.1f){}

	void Init(aiVector3D& p=zero, aiVector3D& f=zaxis, aiVector3D& u=yaxis){
		position=p;
		forward=f;
		up=u;
		
	}

	void MoveLeftRight(int dir){ //Dir=+1=>Right, dir=-1=> Left
		aiVector3D right=forward^up;
		right.Normalize();
		position+=(right*(forwardSpeed*dir));
	}

	void MoveForwardBack(int dir){ //Dir=+1=>Forward, dir=-1=> Back

		position+=(forward*(forwardSpeed*dir));
	}

	void TurnRightLeft(int dir){ //Dir=+1=>Right, dir=-1=> Left
		aiMatrix3x3 rotate;
		aiMatrix3x3::Rotation(dir*roationSpeed,up,rotate);
		forward*=rotate;
	}
		
	void TurnUpDown(int dir){ //Dir=+1=>Up, dir=-1=> Down
		aiMatrix3x3 rotate;
		aiVector3D right=forward^up;
		right.Normalize();
		aiMatrix3x3::Rotation(dir*roationSpeed,right,rotate);
		forward*=rotate;
		up*=rotate;
	}

	void ViewingTransform(){
		aiVector3D centre=position+forward;
		gluLookAt(position.x,position.y,position.z,centre.x,centre.y,centre.z,up.x,up.y,up.z);
	}

	void ProjectionTransform(bool perspective){
		if(perspective){
			gluPerspective(90.f, 1.f, 1.f, 10000.0f);//fov, aspect, zNear, zFar
		}
		else{
			glOrtho(-1000,1000,-1000,1000,1,100000);
		}
	}

};

aiVector3D Camera::zero(0.0f);
aiVector3D Camera::yaxis(0.0f,1.0f,0.0f);
aiVector3D Camera::zaxis(0.0f,0.0f,1.0f);



void DrawCube(float angle){
		glRotatef(angle * 50, 1.f, 0.f, 0.f);
		glRotatef(angle * 30, 0.f, 1.f, 0.f);
		glRotatef(angle * 90, 0.f, 0.f, 1.f);
			
		
		 
		//Draw a cube
        glBegin(GL_QUADS);//draw some squares
			glColor3f
				(0,1,1);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f( 50.f,  50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);

			glColor3f(0,0,1);
            glVertex3f(-50.f, -50.f, 50.f);
            glVertex3f(-50.f,  50.f, 50.f);
            glVertex3f( 50.f,  50.f, 50.f);
            glVertex3f( 50.f, -50.f, 50.f);

			glColor3f(1,0,1);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f(-50.f,  50.f,  50.f);
            glVertex3f(-50.f, -50.f,  50.f);

			glColor3f(0,1,0);
            glVertex3f(50.f, -50.f, -50.f);
            glVertex3f(50.f,  50.f, -50.f);
            glVertex3f(50.f,  50.f,  50.f);
            glVertex3f(50.f, -50.f,  50.f);

			glColor3f(1,1,0);
            glVertex3f(-50.f, -50.f,  50.f);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f,  50.f);

			glColor3f(1,0,0);
            glVertex3f(-50.f, 50.f,  50.f);
            glVertex3f(-50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f,  50.f);

        glEnd();


}
////////////////////////////////////////////////////////////
/// Entry point of application
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL");

    // Create a clock for measuring time elapsed
    sf::Clock Clock;

	Camera camera;
	camera.Init();
	
    //prepare OpenGL surface for HSR
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    

	bool perspective=true;
	
	bool rotate=true;
	float angle=0;
	
	// Start game loop
	while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        /*while (App.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::R)){
				rotate=!rotate;
			}

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::D)){
				camera.MoveLeftRight(1);
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)){
				camera.MoveLeftRight(-1);
			}

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::W)){
				camera.MoveForwardBack(1);
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::S)){
				camera.MoveForwardBack(-1);
			}

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Right)){
				camera.TurnRightLeft(1);
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Left)){
				camera.TurnRightLeft(-1);
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up)){
				camera.TurnUpDown(1);
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down)){
				camera.TurnUpDown(-1);
			}

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::P)){
				perspective=!perspective;
			}
 
 
		}*/

		        // Escape key : exit
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                App.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
				rotate=!rotate;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				camera.MoveLeftRight(1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
				camera.MoveLeftRight(-1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
				camera.MoveForwardBack(1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
				camera.MoveForwardBack(-1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
				camera.TurnRightLeft(1);
			}
			if (sf::Keyboard::isKeyPressed( sf::Keyboard::Left)){
				camera.TurnRightLeft(-1);
			}
			if (sf::Keyboard::isKeyPressed( sf::Keyboard::Up)){
				camera.TurnUpDown(1);
			}
			if (sf::Keyboard::isKeyPressed( sf::Keyboard::Down)){
				camera.TurnUpDown(-1);
			}

			if (sf::Keyboard::isKeyPressed( sf::Keyboard::P)){
				perspective=!perspective;
			}
        
        //Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// Setup a perspective projection & Camera position
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		camera.ProjectionTransform(perspective);
		//gluPerspective(90.f, 1.f, 1.f, 3000.0f);//fov, aspect, zNear, zFar

        // Apply some transformations for the cube
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        camera.ViewingTransform();
		
		if(rotate){
			angle=Clock.getElapsedTime().asSeconds();
		}

		glTranslatef(0,0,10000);
		int spacing=300;
		for(int i=-5;i<10;i++){
			for(int j=-5;j<10;j++){
				for(int k=-5;k<10;k++){
					glPushMatrix();
					glTranslatef(i*spacing,j*spacing,k*spacing);
					DrawCube(angle);
					glPopMatrix();
				}
			}
		}



        // Finally, display rendered frame on screen
        App.display();
    }

    return EXIT_SUCCESS;
}