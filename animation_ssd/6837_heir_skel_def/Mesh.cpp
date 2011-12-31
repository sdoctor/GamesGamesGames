#include "Mesh.h"

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces

  string str;
  ifstream myfile(filename);

  while(myfile >> str) {
    Vector3f loc;

    if (str == "v") { //location
      myfile >> loc[0] >> loc[1] >> loc[2];
      bindVertices.push_back(loc);
    }
    else if (str == "f") { //face
      int first, second, third;
      myfile >> first >> second >> third;

      Tuple3u face(first, second, third);
      faces.push_back(face);
    }
    else {
      cout << "Invalid file." << endl;
    }
  }
	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

void Mesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".


	//cout << "faces.size = " << faces.size() << endl;

    	glBegin(GL_TRIANGLES);

    for (int i = 0; i < faces.size(); i++) {
      //cout << "faces.size (in loop) = " << faces.size() << endl;
    	Tuple3u tup = faces[i];
    	
    	int a = tup[0];
    	int d = tup[1];
    	int g = tup[2];
    	
    	Vector3f v1 = Vector3f(currentVertices[a-1][0], currentVertices[a-1][1], currentVertices[a-1][2]);
    	Vector3f v2 = Vector3f(currentVertices[d-1][0], currentVertices[d-1][1], currentVertices[d-1][2]);
    	Vector3f v3 = Vector3f(currentVertices[g-1][0], currentVertices[g-1][1], currentVertices[g-1][2]);
    	
    	Vector3f e1 = Vector3f(v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2]);
    	Vector3f e2 = Vector3f(v3[0]-v2[0], v3[1]-v2[1], v3[2]-v2[2]);
    	
    	Vector3f normal = Vector3f::cross(e1, e2);
    	Vector3f norm = -normal.normalized();
    	
    	glNormal3f(norm[0], norm[1], norm[2]);
    	glVertex3f(v1[0], v1[1], v1[2]);
    	glVertex3f(v2[0], v2[1], v2[2]);
    	glVertex3f(v3[0], v3[1], v3[2]);
    }
    glEnd();	
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	
	  string w_0;
	  ifstream myfile(filename);

	  while(myfile >> w_0) {
		  vector<float> weights;
		  weights.push_back(0);
		  weights.push_back(atof(w_0.c_str()));
		  for (int i = 2; i < numJoints; i++) { 
			  string w;
			  myfile >> w;
			  weights.push_back(atof(w.c_str()));
		  }
		  attachments.push_back(weights);
	  }
}

