#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Loader.h"

namespace renderEngine
{
	namespace loader
	{
		static int createVAO();
		static void storeDataInAttributeList(int attributeNumber, std::vector<float> data);
		static void bindIndicesBuffer(std::vector<int> indices);

		static std::vector<GLuint> vaos;
		static std::vector<GLuint> vbos;

		/*
			This function will get a list with all the vertex positions of
			the mesh and will return an RawModel.
		*/
		struct RawModel loadToVAO(std::vector<float> positions, std::vector<int> indices)
		{
			int vaoID = createVAO();
			bindIndicesBuffer(indices);
			storeDataInAttributeList(0, positions);
			glBindVertexArray(0);
			return { vaoID, (int) indices.size() };
		}

		/*
			This function will delete all the vao's with the vbo's which are stored in them.
		*/
		void cleanUp()
		{
			for (const auto& vao : vaos)
			{
				glDeleteVertexArrays(1, &vao);
			}

			for (const auto& vbo : vbos)
			{
				glDeleteBuffers(1, &vbo);
			}
		}

		/*
			This function will create a new VAO for a new mesh.
		*/
		static int createVAO()
		{
			GLuint vaoID;
			glGenVertexArrays(1, &vaoID);
			vaos.push_back(vaoID);
			glBindVertexArray(vaoID);
			return vaoID;
		}

		/*
			This function can store data (vbo) in a vao.
		*/
		static void storeDataInAttributeList(int attributeNumber, std::vector<float> data)
		{
			GLuint vboID;
			glGenBuffers(1, &vboID);
			vbos.push_back(vboID);
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);
			glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		/*
			This functions loads a indices buffer and bind it to a vao.
			(Using this method of rendering is way more efficiënt with large/complex meshes.
			Using indices you won't have to specify double or more occuring vertices. You just use sort of a lookup table
			to choose which vertex to get)
		*/
		static void bindIndicesBuffer(std::vector<int> indices)
		{
			GLuint vboID;
			glGenBuffers(1, &vboID);
			vbos.push_back(vboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		}
	}
}