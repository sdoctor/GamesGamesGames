#include "MatrixStack.h"

using namespace std;

MatrixStack::MatrixStack()
{
	// Initialize the matrix stack with the identity matrix.
  vector< Matrix4f > mat_stack;
  mat_stack.push_back(Matrix4f::identity());
  m_matrices = mat_stack;
}

void MatrixStack::clear()
{
	// Revert to just containing the identity matrix.
  vector< Matrix4f > mat_stack;
  mat_stack.push_back(Matrix4f::identity());
  m_matrices = mat_stack;
 
}

Matrix4f MatrixStack::top()
{
	// Return the top of the stack
  return m_matrices[m_matrices.size()-1];
}

void MatrixStack::push( const Matrix4f& m )
{
	// Push m onto the stack.
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
  Matrix4f old = m_matrices[m_matrices.size()-1];
  Matrix4f pushed = old*m;
  m_matrices.push_back(pushed);
}

void MatrixStack::pop()
{
	// Remove the top element from the stack
  m_matrices.pop_back(); 
}

int MatrixStack::size()
{
  return m_matrices.size();
}

