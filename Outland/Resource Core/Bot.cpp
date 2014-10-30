#include "Bot.h"


/****************************
* Default Constructor
****************************/
Bot::Bot(const Vector3<real64>& npos,const std::string& filename):
	Object(npos,filename)
{
}

Bot::Bot(const Vector3<real64>& npos,const std::string& filename,const GLuint* diffuse,const GLuint* normals,const GLuint* bump):
	Object(npos,filename,diffuse,normals,bump)
{
}

/****************************
* Deconstructor
****************************/
Bot::~Bot()
{
}
