#pragma once

namespace water
{
	/*
		A frameBuffer object (FBO) is some sort of screen which can be rendered onto, like your display.
		You can transform this FBO into a 2D texture.
	 */
	namespace frameBuffer
	{
		/*
			Call this when loading the game
		 */
		void Init();

		/*
			Call this when closing the game
		 */
		void CleanUp();

		/*
			Call this function to render to the reflection FBO
		 */
		void BindReflectionFBO();

		/*
			Call this function to render to the refraction FBO
		 */
		void BindRefractionFBO();

		/*
			Call this to switch to the default FBO
		 */
		void UnbindCurrentFBO();

		/*
			Call this to get the 2D texture of the reflection
		 */
		int GetReflectionTexture();

		/*
			Call this to get the 2D texture of the refraction
		 */
		int GetRefractionTexture();

		/*
			Call this to get the 2D texture of the depth from the refraction
		 */
		int GetRefractionDepthTexture();
	}
}