#include "GPU3D_TexcacheOpenGL.h"
#include "Platform.h"
namespace melonDS
{

GLuint TexcacheOpenGLLoader::GenerateTexture(u32 width, u32 height, u32 layers)
{
    GLuint texarray;
    glGenTextures(1, &texarray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texarray);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    
    return texarray;
}

void TexcacheOpenGLLoader::PremultiplyAlpha(int width, int height, u8* data){
    for (int i = 0; i < width * height * 4; i += 4) {
        float a = data[i + 3] / 31.0f;
        data[i + 0] = (u8)(data[i + 0] * a);
        data[i + 1] = (u8)(data[i + 1] * a);
        data[i + 2] = (u8)(data[i + 2] * a);
    }
}

void TexcacheOpenGLLoader::UploadTexture(GLuint handle, u32 width, u32 height, u32 layer, void* data)
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
    PremultiplyAlpha(width, height, (u8*) data);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
        0, 0, 0, layer,
        width, height, 1,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    Platform::Log(Platform::LogLevel::Info, "Uploaded New Texture\n");

}

void TexcacheOpenGLLoader::DeleteTexture(GLuint handle)
{
    glDeleteTextures(1, &handle);
}

}