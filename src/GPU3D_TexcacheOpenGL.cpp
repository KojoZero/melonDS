#include "GPU3D_TexcacheOpenGL.h"

namespace melonDS
{

GLuint TexcacheOpenGLLoader::GenerateTexture(u32 width, u32 height, u32 layers)
{
    GLuint texarray;
    glGenTextures(1, &texarray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texarray);    
    if (IsCompute)
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, layers);
    else
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    
    return texarray;
}

void TexcacheOpenGLLoader::UploadTexture(GLuint handle, u32 width, u32 height, u32 layer, void* data)
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
        0, 0, 0, layer,
        width, height, 1,
        GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (GLAD_GL_EXT_texture_filter_anisotropic) {
        float maxAniso;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
    }
}

void TexcacheOpenGLLoader::DeleteTexture(GLuint handle)
{
    glDeleteTextures(1, &handle);
}

}