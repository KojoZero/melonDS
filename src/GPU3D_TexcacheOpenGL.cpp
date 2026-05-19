#include "GPU3D_TexcacheOpenGL.h"
#include "Platform.h"
#include <cmath>
namespace melonDS
{

bool anisoEnabled = true;
bool mipmapEnabled = false; // Disabled because it tanks performance in certain games (ex: TWEWY)
bool linearFilteringEnabled = false; // Disabled because it's used for sprites in certain games (ex: TWEWY)

GLuint TexcacheOpenGLLoader::GenerateTexture(u32 width, u32 height, u32 layers)
{
    GLuint texarray;
    glGenTextures(1, &texarray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texarray);
    int mipLevels = 1;
    if (mipmapEnabled){
        mipLevels = std::floor(std::log2(std::max(width, height))) + 1;
    }
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevels, GL_RGBA8, width, height, layers);
    
    return texarray;
}

void TexcacheOpenGLLoader::UploadTexture(GLuint handle, u32 width, u32 height, u32 layer, void* data)
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
        0, 0, 0, layer,
        width, height, 1,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (mipmapEnabled){
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    if (linearFilteringEnabled){
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    
    if (anisoEnabled){
        if (GLAD_GL_EXT_texture_filter_anisotropic) {
            float maxAniso;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
            glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
        }
    }
    Platform::Log(Platform::LogLevel::Info, "Uploaded New Texture\n\n");


}

void TexcacheOpenGLLoader::DeleteTexture(GLuint handle)
{
    glDeleteTextures(1, &handle);
}

}