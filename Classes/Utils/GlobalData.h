#ifndef _GLOBAL_DATE_
#define _GLOBAL_DATE_

#include "Box2D/Box2D.h"
#define PTM_RATIO 32

extern b2World *G_world;

typedef struct {
	GLfloat x;
	GLfloat y;
} Vertex2D;

inline Vertex2D Vertex2DMake(GLfloat inX, GLfloat inY) {
	Vertex2D ret;
	ret.x = inX;
	ret.y = inY;
	return ret;
}

enum GameTags
{
	TAGS_ROLE_MIN = 0,
	TAGS_ROLE = 1,
	TAGS_ROLE_MAX = 10,
	TAGS_WALL_MIN = 50,
	TAGS_WALL = 51,
	TAGS_BLOCK_MIN = 100,
	TAGS_BLOCK = 101,
	TAGS_BLOCK_MAX = 200,

};
#endif