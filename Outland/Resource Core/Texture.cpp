#include "Texture.h"
#include <SOIL.h>

/****************************
* Default Constructor
****************************/
Texture::Texture(const GLenum& texTarget, const std::string& file,const GLuint *texObj):
	m_fileName(file),
	m_TextureTarget(texTarget),
	m_TextureObj(texObj)
{
}

Texture::Texture(const GLenum& texTarget,const GLuint *texObj):
	m_fileName(""),
	m_TextureTarget(texTarget),
	m_TextureObj(texObj)
{
}

Texture::~Texture()
{
	if(m_TextureObj!=NULL)
		delete m_TextureObj;
}

/*bool Texture::Load()
{
	if(m_fileName!="")
	{
		m_TextureObj=new GLuint(SOIL_load_OGL_texture(m_fileName.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS));
		if(*m_TextureObj==0)
		{
			return false;
		}

		glBindTexture(m_TextureTarget,*m_TextureObj);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

		return true;
	}
	else
		return false;
}*/

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	if(m_TextureObj)
		glBindTexture(m_TextureTarget,*m_TextureObj);
}

