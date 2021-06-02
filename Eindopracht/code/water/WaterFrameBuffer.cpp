#include <GL/glew.h>
#include "../toolbox/Toolbox.h"
#include "WaterFrameBuffer.h"

namespace water
{
	namespace frameBuffer
	{
		static void initReflectionFBO();
		static void initRefractionFBO();

		static GLuint createFBO();
		static void bindFBO(GLuint fbo, int width, int height);
		static GLuint createTextureAttachment(int width, const int height);
		static GLuint createDepthTextureAttachment(int width, int height);
		static GLuint createDepthBufferAttachment(int width, int height);
		
		// The resolution of the water reflection texture
		static const int REFLECTION_WIDTH = 1920;
		static const int REFLECTION_HEIGHT = 1080;

		// The resolution of the water refraction texture
		static const int REFRACTION_WIDTH = 1920;
		static const int REFRACTION_HEIGHT = 1080;

		static GLuint reflectionFrameBuffer;
		static GLuint reflectionTexture;
		static GLuint reflectionDepthBuffer;

		static GLuint refractionFrameBuffer;
		static GLuint refractionTexture;
		static GLuint refractionDepthTexture;


		void Init()
		{
			initReflectionFBO();
			initRefractionFBO();
		}

		void CleanUp()
		{
			glDeleteFramebuffers(1, &reflectionFrameBuffer);
			glDeleteTextures(1, &reflectionTexture);
			glDeleteRenderbuffers(1, &reflectionDepthBuffer);
			glDeleteFramebuffers(1, &refractionFrameBuffer);
			glDeleteTextures(1, &refractionTexture);
			glDeleteTextures(1, &refractionDepthTexture);
		}

		void BindReflectionFBO()
		{
			bindFBO(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
		}

		void BindRefractionFBO()
		{
			bindFBO(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
		}

		void UnbindCurrentFBO()
		{
			// Bind the default fbo (everything will be rendered to the screen again)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// Set the viewport of the fbo to the width and height of the window
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGT);
		}

		int GetReflectionTexture()
		{
			return reflectionTexture;
		}

		int GetRefractionTexture()
		{
			return refractionTexture;
		}

		int GetRefractionDepthTexture()
		{
			return refractionDepthTexture;
		}

		static void initReflectionFBO()
		{
			reflectionFrameBuffer = createFBO();
			reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
			reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
			UnbindCurrentFBO();
		}

		static void initRefractionFBO()
		{
			refractionFrameBuffer = createFBO();
			refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
			refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
			UnbindCurrentFBO();
		}

		static GLuint createFBO()
		{
			// Create a new fbo
			GLuint fbo;
			glGenFramebuffers(1, &fbo);

			// Activate the fbo
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			// Select the color buffer attachment for the fbo
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			
			return fbo;
		}

		static void bindFBO(GLuint fbo, int width, int height)
		{
			// Disable the current bounded texture
			glBindTexture(GL_TEXTURE_2D, 0);
			// Bind the fbo (so everything gets rendered to this fbo)
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			// Set the resolution of the viewport (screen) of the fbo
			glViewport(0, 0, width, height);
		}

		static GLuint createTextureAttachment(int width, const int height)
		{
			// Create a new texture
			GLuint texture;
			glGenTextures(1, &texture);

			// Activate the texture
			glBindTexture(GL_TEXTURE_2D, texture);

			// Set some settings for the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Attach the texture to the currently bounded fbo
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

			return texture;
		}

		static GLuint createDepthTextureAttachment(int width, int height)
		{
			// Create a new texture
			GLuint texture;
			glGenTextures(1, &texture);

			// Activate the texture
			glBindTexture(GL_TEXTURE_2D, texture);

			// Set some settings for the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Attach the texture to the currently bounded fbo
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

			return texture;
		}

		static GLuint createDepthBufferAttachment(int width, int height)
		{
			// Create a new depth buffer
			GLuint depthBuffer;
			glGenRenderbuffers(1, &depthBuffer);

			// Activate the buffer
			glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);

			// Tell what to store into the depth buffer
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			// Attach the depth buffer to the currently bounded fbo
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

			return depthBuffer;
		}
	}
}