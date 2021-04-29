#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../../tigl.h"
#include "Renderer.h"

namespace renderEngine
{
	namespace renderer
	{

		/*
		 	This function will clear the screen.
		*/
		void prepare()
		{
			glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		/*
			This function will render a RawModel on the screen.
		*/
		void render(RawModel model)
		{
			glBindVertexArray(model.vaoID);
			glEnableVertexAttribArray(0);
			//glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
			glDrawElements(GL_TRIANGLES, model.vertexCount, GL_UNSIGNED_INT, 0);
			glDisableVertexAttribArray(0);
			glBindVertexArray(0);
		}
	}
}