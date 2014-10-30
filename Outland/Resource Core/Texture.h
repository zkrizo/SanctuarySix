#ifndef Texture_H
#define Texture_H

#include <Outland\UnitAbstraction.h>
#include <gl\glew.h>
#include <gl\GLU.h>
#include <string>

class Texture
{
public:
	Texture(const GLenum& TextureTarget, const std::string& file,const GLuint *textObj);
	Texture(const GLenum& TextureTarget, const GLuint *textObj);
	~Texture();
	//bool Load();
	void Bind(GLenum TextureUnit);
private:
	std::string m_fileName;
	GLenum m_TextureTarget;
	const GLuint *m_TextureObj;
};

#endif