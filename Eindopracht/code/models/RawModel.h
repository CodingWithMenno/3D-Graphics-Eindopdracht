#pragma once

#include <GL/glew.h>

/*
*
* Structure for storing a vboID and vertexCount.\
* 
* This structure represents a Mesh.
* The vaoID, points to an ID stored by openGL and the
* vertexCount is how many triangles in the mesh there are.
*/

struct RawModel
{
	GLuint vaoID;
	int vertexCount;

	RawModel(int id, int count) : vaoID(id), vertexCount(count) {};
};

