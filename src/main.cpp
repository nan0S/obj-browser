#include "App.hpp"
#include <filesystem>

namespace fs = std::filesystem;

void formatError()
{
    std::cerr << "Format error!\n\n"
	    		  "Valid format: ./objbrowser PATH_TO_OBJ_FILE PATH_TO_TEXTURE_FILE\n"
			  "eg.           ./objbrowser ../resources/tank.obj ../resources/tank.DDS\n\n"
			  "Where PATH_TO_OBJ_FILE is path to the file with 'obj' extension and\n"
			  "PATH_TO_TEXTURE_FILE is path to the file with either 'dds' or 'bmp' extension.\n";
}

int main(int argc, char* argv[])
{
    if (argc < 3)
        return formatError(), 0;
    
    std::string obj_path = std::string(argv[1]);
    std::string tex_path = std::string(argv[2]);

    if (!fs::exists(obj_path))
    {
	   std::cerr << "Given obj file does not exists!\n";
        return EXIT_FAILURE;
    }
    if (!fs::exists(tex_path))
    {
	   std::cerr << "Given texture file does not exists!\n";
	   return EXIT_FAILURE;
    }

    App app;
    app.start(obj_path, tex_path);

    return EXIT_SUCCESS;
}
