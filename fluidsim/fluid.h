#include <stdlib.h>
#include "glut.h"
#include <vector>
#include <list>
using namespace std;

#ifndef particle_system
#define particle_system

struct particleInfo {
	double x, y, z;
	double xVel, yVel, zVel;
	double xAcc, yAcc, zAcc;
	double maxVel;
	double rho, rhonear;
	double press, pressnear, xforce, yforce, zforce;
};

class Particle {
private:

	double xPos, yPos, zPos;
	double xVel, yVel, zVel;
	double xAcc, yAcc, zAcc;

	double maxVelocity;

	double rho, rhonear;

	double press, pressnear, xforce, yforce, zforce;
	
	double radius;
	double color[3];

	//vector<Particle*> neighbours;

public:
	Particle();
	Particle(double x, double y, double z, double r, double arr[]);
	Particle(double xV, double yV, double zV, int type, int st);
	double getRadius();
	double* getRGB();

	double getX();
	double getY();
	double getZ();

	void setX(double);
	void setY(double);
	void setZ(double);

	particleInfo getParticleInfo();
	void setParticleInfo(particleInfo);
};
//
//struct particleInfo {
//	double x, y, z;
//	double xVel, yVel, zVel;
//	double xAcc, yAcc, zAcc;
//	double maxVel;
//	double rho, rhonear;
//	double press, pressnear, xforce, yforce, zforce;
//	vector<Particle*> neighbours;
//	Particle* myPointer;
//};

class pSystem {
private:
	int numParticles;
	vector<Particle*> pList;

public:
	pSystem();
	pSystem(vector<Particle*>);

	// Adds a new particle to the system
	void addParticle(Particle*);
	// Deletes all particles in the system
	void clearAll();
	
	vector<Particle*>::const_iterator getBegin();
	vector<Particle*>::const_iterator getEnd();

	// Updates positions and velocities of all particles in the system
	void updateSystem();
};

#endif