#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace Texture
{
    #define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
    #define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
    #define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

    GLuint loadBMP(const char* path);
    GLuint loadDDS(const char* path);
}