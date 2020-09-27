#pragma once

#include <GL/glew.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

namespace Shader
{
     GLuint load(const char* vshader_path, 
                 const char* fshader_path);
} 
