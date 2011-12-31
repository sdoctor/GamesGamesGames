#include "SkeletalModel.h"
#include <iostream>
#include <FL/Fl.H>


using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();

		//Extra Credit ******************************
		//loadTexture();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}


void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
  
  ifstream myfile(filename);
  float t_1;
  while (myfile >> t_1) {

    float t_2, t_3;
    int parent_index;
    myfile >> t_2 >> t_3 >> parent_index;
	 
    Vector3f translate_by = Vector3f(t_1, t_2, t_3);
  
    //cout << "line = " << t_1 << ", " << t_2 << ", " << t_3 << endl;
    //cout << "PARENT INDEX = " <<  parent_index << endl;

    // define the joint's transform
    Joint *joint = new Joint;
    
    joint->transform = Matrix4f().translation(translate_by);
    
    // update the m_joints list
    // *Note, these will be pushed back in the same order as the lines
    // in the file, so the indices will correlate to the line index
    m_joints.push_back(joint);

    // update the parent to have the proper children
    if (parent_index == -1) // There is no parent to get. Set root.
      {
	m_rootJoint = joint; 
      } 
    else 
      { 
	Joint *parent = m_joints[parent_index];
	//vector<Joint*> childs = parent->children;
	//childs.push_back(joint);
	parent->children.push_back(joint);
	//cout << "size of children = " << parent->children.size() << endl;
      }
  }	
}

void SkeletalModel::jointHelper(Joint* joint) {
  // establish root node
  //cout << "NOW IN RECURSIVE JOINT HELPER" << endl;
  
  m_matrixStack.push(joint->transform);
  
  
  cout << "Size of m = " << m_matrixStack.size() << endl;
  
  glLoadMatrixf(m_matrixStack.top());
  glutSolidSphere(0.025f, 12, 12);
  
  for (int i = 0; i < joint->children.size(); i++) {
    Joint *child = joint->children[i];
    //cout << "CHILDREN SIZE =" << joint->children.size() << endl;
    jointHelper(child);
  }
  //cout << "popping last thing" << endl;
  m_matrixStack.pop();

}//end def


void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.
 
  cout << "Now trying to call recursive joint helper" << endl;
  jointHelper(m_rootJoint);
  
}


void SkeletalModel::boneHelper(Joint* joint) {
m_matrixStack.push(joint->transform);

for (int i = 0; i < joint->children.size(); i++) {
    Joint *child = joint->children[i];

     // translate it in z so that the box ranges from [-0.5, -0.5, 0] to [.5, .5, 1]
    Matrix4f transl_from_origin = Matrix4f().translation(0, 0, 0.5);

     // scale the box so that it ranges from [-.5, -.5, 0] to [.5, .5, l], l is dist
    // to the next joint in the recursion

    // because the child's location is relative to the parent, we can use
    // the child's location from the transform
           // get the child location out of its transform matrix
    Matrix4f child_transf = child->transform;
    Vector4f child_col = child_transf.getCol(3);
    Vector3f child_loc = Vector3f(child_col[0], child_col[1], child_col[2]);
    cout << "child loc = " << child_loc[0] << ", " << child_loc[1] << ", " << child_loc[2] << endl;
    float euc_dist =sqrt(child_loc.absSquared());
    cout << "euc_dist = " << euc_dist << endl;

    Matrix4f scale = Matrix4f().scaling(0.05, 0.05, euc_dist);
    
    // rotate to align to parent->child vector
    Vector3f z = child_loc;//child_loc is parentOffSet
    Vector4f z_ = Vector4f(z[0], z[1], z[2], 0).normalized(); //turn into Vector4f

    Vector3f rnd = Vector3f(0, 0, 1);
    Vector3f y = Vector3f().cross(z,rnd); 
    Vector4f y_ = Vector4f(y[0], y[1], y[2], 0).normalized();

    Vector3f x = Vector3f().cross(y,z);
    Vector4f x_ = Vector4f(x[0], x[1], x[2], 0).normalized();

    Matrix4f rotate = Matrix4f(x_, y_, z_, Vector4f(0,0,0,1), true);


    // add the matrices to the stack in reverse order of how mentioned in pset
    m_matrixStack.push(rotate); 
    m_matrixStack.push(scale);   
    m_matrixStack.push(transl_from_origin);

    // draw a box between the joint and the joint's parent (unless root)
    cout << "drawing solid cube" << endl;

    glLoadMatrixf(m_matrixStack.top());
    glutSolidCube( 1.0f );

    //EXTRA CREDIT *************************************************
    /* glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
glEnd();
    */

    //**************************************************************
  
    // pop off all the translations you put on the stack
    m_matrixStack.pop();
    m_matrixStack.pop();
    m_matrixStack.pop();

    boneHelper(child);
  }
 
  cout << "popping last thing" << endl;
  m_matrixStack.pop();





}




void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
	
	boneHelper(m_rootJoint);
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
  Joint* joint = m_joints[jointIndex];
 
  Matrix4f x = Matrix4f().rotateX(rX);
  Matrix4f y = Matrix4f().rotateY(rY);
  Matrix4f z = Matrix4f().rotateZ(rZ);

  Matrix4f prod = x*y*z;

  Matrix3f rot = prod.getSubmatrix3x3(0,0);

  Matrix4f trans = Matrix4f(rot.getCol(0)[0], rot.getCol(1)[0], rot.getCol(2)[0], 
				 joint->transform.getCol(3)[0],
				 rot.getCol(0)[1], rot.getCol(1)[1], rot.getCol(2)[1],
				 joint->transform.getCol(3)[1],
				 rot.getCol(0)[2], rot.getCol(1)[2], rot.getCol(2)[2],
				 joint->transform.getCol(3)[2],
				 0, 0, 0, 
				 joint->transform.getCol(3)[3] );
  
  joint->transform = trans;
}

void SkeletalModel::bindWorldToJointHelper(Joint* joint, Matrix4f m) {

  m = m*joint->transform;
  	  
  joint->bindWorldToJointTransform = m.inverse();

	  for (int i = 0; i < joint->children.size(); i++) {
		  Joint* child = joint->children[i];
		  bindWorldToJointHelper(child, m);
	  }
}

void SkeletalModel::currentJointToWorldHelper(Joint* joint, Matrix4f m) {

	  m = m*joint->transform;

	  // take a joint and a matrix
	  // calls to its children whatever matrix passed in times its transformation matrix
	  
	  joint->currentJointToWorldTransform = m;
	  
	  for (int i = 0; i < joint->children.size(); i++) {
		  Joint* child = joint->children[i];
		  currentJointToWorldHelper(child, m);
	  }
}

void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
  bindWorldToJointHelper(m_rootJoint, Matrix4f::identity()); // pass in identity
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	
  currentJointToWorldHelper(m_rootJoint, Matrix4f::identity());
}



void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.

	for (int i = 0; i < m_mesh.currentVertices.size(); i++) {
		Vector3f bind_verts = m_mesh.bindVertices[i];
		Vector4f verts = Vector4f(bind_verts[0], bind_verts[1], bind_verts[2], 1);
		Vector3f final = Vector3f(0,0,0);
		vector<float> weights = m_mesh.attachments[i];
		for (int j = 0; j < weights.size(); j++) {
			float w = weights[j];
			
			Matrix4f transform = w*m_joints[j]->currentJointToWorldTransform*m_joints[j]->bindWorldToJointTransform;
			
			Vector4f to_sum = transform*verts;
			final += to_sum.xyz();
		}
	       m_mesh.currentVertices[i] = final;
	}
}

// EXTRA CREDIT

/*AUX_RGBImageRec *LoadBMP(char *Filename)
{
  FILE *File=NULL;
  if (!Filename)
    {
      return NULL;
    }
  FILE = fopen(Filename, "r");
  if (File)
    {
      fclose(File);
      return auxDIBImageLoad(Filename);
    }
  return NULL;
}
*/
 /*
void SkeletalModel::loadTexture()
{
  GLuint texture[1];
  AUX_RGBImageRec swirly = auxDIBImageLoad("texture/swirly.bmp");
  AUX_RGBImageRec *TextureImage[1];
  TextureImage[0] = swirly;
  glGenTextures(1, &texture[0]);

  // Load Texture ********************************************************
  int imageWidth; // integer equal to 2n+2(border) for some integer n
  int imageHeight; // integer equal to 2n+2(border)
  

  glBindTexture(GL_TEXTURE_2D, texture[0]); // makes texture associated with ID 13 the active texture

  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  imageWidth = TextureImage[0]->sizeX;
  imageHeight = TextureImage[0]->sizeY;
  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

  // Draw Texture *********************************************************
  glEnable(GL_TEXTURE_2D);

  // Make sure the texture maps to the correct coordinates*****************
  
  

  
  
}

 */
