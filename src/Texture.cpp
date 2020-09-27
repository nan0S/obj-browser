#include "Texture.hpp"

GLuint Texture::loadBMP(const char* path)
{
    printf("Reading texture %s\n", path);

    unsigned char header[54];
    unsigned int data_pos;
    unsigned int image_size;
    unsigned int width, height;
    unsigned char* data;

    FILE* file = fopen(path, "rb");
    if (!file)
    {
        printf("%s could not be opened. Are you in the right directory?\n", path);
        getchar();
        return 0;
    }

    if (fread(header, 1, 54, file) != 54)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }

    if (*(int*)&(header[0x1E]) != 0 || *(int*)&(header[0x1C]) != 24)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }

    data_pos = *(int*)&(header[0x0A]);
    image_size = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if (image_size == 0)
        image_size = width * height * 3;
    if (data_pos == 0)
        data_pos = 54;

    data = new unsigned char[image_size];
    fread(data, 1, image_size, file);
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    delete[] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

GLuint Texture::loadDDS(const char* path)
{
    printf("Reading texture %s\n", path);

    unsigned char header[124];
    FILE* file;

    file = fopen(path, "rb");
    if (file == NULL)
    {
        printf("%s could not be opened. Are you in the right directory?\n");
        return 0;
    }

    char filecode[4];
    fread(filecode, 1, 4, file);
    if (strncmp(filecode, "DDS ", 4) != 0)
    {
        fclose(file);
        return 0;
    }   

    fread(&header, 124, 1, file);

    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linear_size = *(unsigned int*)&(header[16]);
    unsigned int mipmap_count = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    unsigned char* buffer;
    unsigned int bufsize;
    // how big is it going to be including all mipmaps ?
    bufsize = mipmap_count > 1 ? linear_size * 2 : linear_size;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, file);
    fclose(file);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;

    switch (fourCC)
    {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3: 
		    format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
	    	break; 
	    case FOURCC_DXT5: 
		    format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		    break; 
	    default: 
		    free(buffer); 
		    return 0; 
    }

    // openGL buffers ...
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int block_size = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    // load the mipmaps
    for (unsigned int level = 0; level < mipmap_count && (width || height); ++level)
    {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * block_size;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                               0, size, buffer + offset);
                            
        offset += size;
        width >>= 1;
        height >>= 1;

        // deal with non-power-of-2 textures
        if (width < 1) width = 1;
        if (height < 1) height = 1;
    }

    free(buffer);

    return texture;
}