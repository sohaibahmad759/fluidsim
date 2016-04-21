#include "fluid.h"
#define Y_ACC -0.005

double closeDistance = 0.75;
//double closeDistance = 0.4;
double closeSquared = pow(closeDistance, 2);
double restDensity = 100.0;
//double restDensity = 20.0;
double sigma = 3.0;
double beta = 4.0;

Particle::Particle() {
	xPos = 0;
	yPos = 2;
	zPos = 0;

	xVel = yVel = zVel = 0;
	xAcc = 0;
	yAcc = Y_ACC;
	zAcc = 0;

	maxVelocity = 0.6;

	rho = rhonear = 0;
	press = pressnear = xforce = yforce = zforce = 0;

	radius = 0.1;

	color[0] = 0;
	color[1] = 0;
	color[2] = 1;
}

// arr[] must be a 3-element double-type array
Particle::Particle(double x, double y, double z, double r, double arr[]) {
	xPos = x;
	yPos = y;
	zPos = z;

	xVel = yVel = zVel = 0;
	xAcc = 0;
	yAcc = Y_ACC;
	zAcc = 0;

	maxVelocity = 0.6;

	rho = rhonear = 0;
	press = pressnear = xforce = yforce = zforce = 0;

	radius = r;

	color[0] = arr[0];
	color[1] = arr[1];
	color[2] = arr[2];
}

Particle::Particle(double xV, double yV, double zV, int type) {
	if (type == 0) {
		xVel = xV;
		yVel = yV;
		zVel = zV;
		xPos = 0;
		yPos = 2;
		zPos = 0;
	}
	else if (type == 1) {
		xPos = xV;
		yPos = yV;
		zPos = zV;
		xVel = 0;
		yVel = -0.05;
		zVel = 0;
	}


	xAcc = 0;
	yAcc = Y_ACC;
	zAcc = 0;

	maxVelocity = 0.6;
	
	rho = rhonear = 0;
	press = pressnear = xforce = yforce = zforce = 0;

	radius = 0.1;

	color[0] = 0;
	color[1] = 0;
	color[2] = 0.5;
}

double Particle::getRadius() {
	return radius;
}

double* Particle::getRGB() {
	return color;
}

double Particle::getX() {
	return xPos;
}

double Particle::getY() {
	return yPos;
}

double Particle::getZ() {
	return zPos;
}

void Particle::setX(double x) {
	xPos = x;
}

void Particle::setY(double y) {
	yPos = y;
}

void Particle::setZ(double z) {
	zPos = z;
}

particleInfo Particle::getParticleInfo() {
	particleInfo p;

	p.x = xPos;
	p.y = yPos;
	p.z = zPos;

	p.xVel = xVel;
	p.yVel = yVel;
	p.zVel = zVel;

	p.xAcc = xAcc;
	p.yAcc = yAcc;
	p.zAcc = zAcc;

	p.maxVel = maxVelocity;

	p.rho = rho;
	p.rhonear = rhonear;
	p.press = press;
	p.pressnear = pressnear;
	p.xforce = xforce;
	p.yforce = yforce;
	p.zforce = zforce;

	//p.neighbours = neighbours;
	//p.myPointer = this;

	return p;
}

void Particle::setParticleInfo(particleInfo p) {
	xPos = p.x;
	yPos = p.y;
	zPos = p.z;

	xVel = p.xVel;
	yVel = p.yVel;
	zVel = p.zVel;

	xAcc = p.xAcc;
	yAcc = p.yAcc;
	zAcc = p.zAcc;

	maxVelocity = p.maxVel;

	rho = p.rho;
	rhonear = p.rhonear;

	press = p.press;
	pressnear = p.pressnear;
	xforce = p.xforce;
	yforce = p.yforce;
	zforce = p.zforce;

	//neighbours = p.neighbours;
}

pSystem::pSystem() {
	numParticles = 0;
}

pSystem::pSystem(vector<Particle*> inp) {

	// Initialize particle system with given list of particles
	numParticles = inp.size();

	for (int i=0; i<inp.size(); i++) {
		pList.push_back(inp[i]);
	}
}

void pSystem::addParticle(Particle* p) {
	numParticles++;
	pList.push_back(p);
}

void pSystem::clearAll() {
	while (numParticles > 0) {
		Particle* ptr = pList[numParticles-1];
		delete ptr;

		pList.pop_back();
		numParticles--;
	}
}

vector<Particle*>::const_iterator pSystem::getBegin() {
	return pList.begin();
}

vector<Particle*>::const_iterator pSystem::getEnd() {
	return pList.end();
}

void pSystem::updateSystem() {
	int pSize = pList.size();

	// Initializing densities (need to find a more efficient way to do this)
	for (int i=0; i<pSize; i++) {
		Particle* temp = pList[i];
		particleInfo pi = temp->getParticleInfo();
		temp->setParticleInfo(pi);
	}

	for (int i=0; i<pSize; i++) {
		Particle* temp = pList[i];

		// pi stands for particle info
		particleInfo pi = temp->getParticleInfo();

		// Applying force
		pi.x += pi.xforce;
		pi.y += pi.yforce;
		pi.z += pi.zforce;

		// Updating velocities
		pi.xVel += pi.xAcc;
		pi.yVel += pi.yAcc;
		pi.zVel += pi.zAcc;

		// Velocities must not exceed maximum velocity
		double maxVel = pi.maxVel;
		if (pi.xVel > maxVel)
			pi.xVel = maxVel;
		if (pi.yVel > maxVel)
			pi.yVel = maxVel;
		if (pi.zVel > maxVel)
			pi.zVel = maxVel;

		// Updating positions
		pi.x += pi.xVel;
		pi.y += pi.yVel;
		pi.z += pi.zVel;

		// Reset rho
		pi.xforce = pi.yforce = pi.zforce = 0;

		// Handling collisions with the container
		if (pi.x <= -0) {
			pi.xVel -= (pi.x - (-0))/2;
			pi.xforce -= (pi.x - (-0))/2;
		}
		else if (pi.x > 2) {
			pi.xVel -= (pi.x - 2)/10;
			pi.xforce -= (pi.x - 2)/2;
		}
		if (pi.y <= 0) {
			pi.yVel -= (pi.y - 0)/2;
			pi.yforce -= (pi.y - 0)/2;
			//pi.yforce -= pi.yforce;
			//pi.yVel -= pi.y;
			//pi.y -= (pi.y);
		}/*
		else if (pi.y > 2) {
			pi.yVel -= (pi.y - 2)/10;
			pi.yforce -= (pi.y - 2)/2;
		}*/
		if (pi.z <= -0) {
			pi.zVel -= (pi.z - (-0))/10;
			pi.zforce -= (pi.z - (-0))/2;
		}
		else if (pi.z > 2) {
			pi.zVel -= (pi.z - 2)/10;
			pi.zforce -= (pi.z - 2)/2;
		}

		
		
		// Calculate densities
		for (int j=0; j<pSize; j++) {
			Particle* neighbour = pList[j];
			particleInfo neighbourpi = neighbour->getParticleInfo();

			if (neighbour == temp)
				continue;

			double neighbourDistSqr = pow(neighbourpi.x - pi.x, 2) + pow(neighbourpi.y - pi.y, 2) + pow(neighbourpi.z - pi.z, 2);

			if (neighbourDistSqr <= closeSquared) {
				double rhoPair = (1.0 - sqrt(neighbourDistSqr)/closeDistance);
				double rhoNear = rhoPair;

				rhoPair = rhoPair * rhoPair;
				pi.rho += rhoPair;
				neighbourpi.rho += rhoPair;

				rhoNear = rhoNear * rhoNear * rhoNear;
				pi.rhonear += rhoNear;
				neighbourpi.rhonear += rhoNear;

				//neighbourpi.neighbours.push_back(pi.myPointer);
				//pi.neighbours.push_back(neighbourpi.myPointer);

				neighbour->setParticleInfo(neighbourpi);
				// particle info for the particle itself is updated later
			}
		}

		/*
		// Checking for collisions with the ground
		if (pi.y <= 0) {
			pi.yVel = -pi.yVel;
			pi.y = 0 + (-pi.y);		// to add offset below the ground
		}
		//else if (pi.y > 2) {
		//	pi.yVel = -pi.yVel;
		//}

		// Checking for collisions with the container
		if (pi.x > 2) {
			pi.xVel = -pi.xVel;
			// add offset
			// pi.x = 0 + (-pi.x); 
		}
		else if (pi.x <= -2) {
			pi.xVel = -pi.xVel;
			// add offset
		}
		if (pi.z > 2) {
			pi.zVel = -pi.zVel;
			// add offset
			// pi.z = pi.z - 2*(pi.z-2);
		}
		else if (pi.z <= -2) {
			pi.zVel = -pi.zVel;
			// add offset
			// pi.z = pi.z + 2*(-2-pi.x); 
		}*/

		// update the particle info inside the particle object
		temp->setParticleInfo(pi);
	}

	// Calculating pressure force
	for (int i=0; i<pList.size(); i++) {
		Particle* temp = pList[i];
		particleInfo pi = temp->getParticleInfo();

		double k = 1;
		double knear = 1;

		pi.press = k * (pi.rho - restDensity);
		pi.pressnear = knear * pi.rhonear;

		
		double dx, dy, dz;
		dx = dy = dz = 0;

		for (int j=0; j<pList.size(); j++) {
			Particle* neighbour = pList[j];

			if (neighbour == temp)
				continue;

			particleInfo neighbourpi = neighbour->getParticleInfo();

			double neighbourDistSqr = pow(neighbourpi.x-pi.x,2) + pow(neighbourpi.y-pi.y,2) + pow(neighbourpi.z-pi.z,2);

			if (neighbourDistSqr <= closeSquared) {
				double neighbourDist = sqrt(neighbourDistSqr);
				double q = 1.0 - neighbourDist/closeDistance;
				double q2 = q * q;
					
				double dm = q * (pi.press + neighbourpi.press - 2*restDensity)/100 + q2 * (pi.pressnear + neighbourpi.pressnear)/10;
				//double dm = q2 * (pi.press + neighbourpi.press - 2*restDensity)/(100*neighbourDist); // + q2 * (pi.pressnear + neighbourpi.pressnear);
				dm = dm/1000;
				//double dm = q2 * (pi.pressnear + neighbourpi.pressnear)/10000*2;
				//double dm = q * (pi.press + neighbourpi.press - 2*restDensity)/10000;
				
				//double dirX = dm * (neighbourpi.x-pi.x)/neighbourDist;
				//double dirY = dm * (neighbourpi.y-pi.y)/neighbourDist;
				//double dirZ = dm * (neighbourpi.z-pi.z)/neighbourDist;

				double dirX = dm * (neighbourpi.x-pi.x);
				double dirY = dm * (neighbourpi.y-pi.y);
				double dirZ = dm * (neighbourpi.z-pi.z);

				dx += dirX;
				dy += dirY;
				dz += dirZ;
				
				neighbourpi.xforce += dirX;
				neighbourpi.yforce += dirY;
				neighbourpi.zforce += dirZ;

				neighbour->setParticleInfo(neighbourpi);
			}
		}

		pi.xforce -= dx;
		pi.yforce -= dy;
		pi.zforce -= dz;

		pi.rhonear = 0;
		pi.rho = 1;

		temp->setParticleInfo(pi);

	}

}