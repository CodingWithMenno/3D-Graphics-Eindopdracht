#include <GL/glew.h>
#include "../stb_image.h"
#include "Loader.h"

namespace renderEngine
{
	namespace loader
	{
		static GLuint createVAO();
		static void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float>& data);
		static void bindIndicesBuffer(std::vector<unsigned int>& indices);

		static std::vector<GLuint> vaos;
		static std::vector<GLuint> vbos;
		static std::vector<GLuint> textures;

		/*
			This function will generate a Model from vertex positions, textureCoordinates and indices.
		*/
		struct models::RawModel LoadToVAO(std::vector<float>& positions, std::vector<float>& textureCoords, std::vector<float>& normals, std::vector<unsigned int>& indices)
		{
			GLuint vaoID = createVAO();
			bindIndicesBuffer(indices);
			storeDataInAttributeList(0, 3, positions);
			storeDataInAttributeList(1, 2, textureCoords);
			storeDataInAttributeList(2, 3, normals);
			glBindVertexArray(0);
			return { vaoID, static_cast<int>(indices.size()) };
		}

		/*
			Loads an image as texture into openGL
		 */
		GLuint LoadTexture(std::string fileName)
		{
			int width, height, bpp;
			unsigned char* imgData = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

			GLuint textureID;
			glGenTextures(1, &textureID);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

			// Set mipmapping with a LOD of -0.4f 
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
			
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			stbi_image_free(imgData);
			textures.push_back(textureID);
			return textureID;
		}

		/*
			This function will delete all the vectors declared at the top of this file from openGL.
		*/
		void CleanUp()
		{
			glDeleteVertexArrays(static_cast<GLsizei>(vaos.size()), &vaos[0]);
			glDeleteBuffers(static_cast<GLsizei>(vbos.size()), &vbos[0]);
			glDeleteTextures(static_cast<GLsizei>(textures.size()), &textures[0]);
		}

		/*
			This function will create a new VAO for a new mesh.
		*/
		static GLuint createVAO()
		{
			// Create a new VAO
			GLuint vaoID;
			glGenVertexArrays(1, &vaoID);
			vaos.push_back(vaoID);
			// Activate the VAO
			glBindVertexArray(vaoID);
			return vaoID;
		}

		/*
			This function can store data (vbo) in a vao.
		*/
		static void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float>& data)
		{
			// Create a new VBO
			GLuint vboID;
			glGenBuffers(1, &vboID);
			vbos.push_back(vboID);
			// Activate the VBO
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			// Put the data into the VBO
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);
			glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, 0);
			// Disable the VBO
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		/*
			This functions loads a indices buffer and binds it to a vao.
			(Using this method of rendering is way more effici�nt with large/complex meshes.
			This way you won't have to specify double or more occuring vertices. You just use sort of a lookup table
			to choose which vertex to get)

			Example:
				std::vector<float> vertices =
				{
				  -0.5f, 0.5f, 0,
				  -0.5f, -0.5f, 0,
				  0.5f, -0.5f, 0,
				  0.5f, 0.5f, 0
				};

				std::vector<int> indices =
				{
					0,1,3,
					3,1,2
				};
		*/
		static void bindIndicesBuffer(std::vector<unsigned int>& indices)
		{
			// Create a new VBO
			GLuint vboID;
			glGenBuffers(1, &vboID);
			vbos.push_back(vboID);
			// Put data into the VBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		}
	}
}
