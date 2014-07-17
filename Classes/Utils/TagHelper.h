#ifndef _TAG_HELPER_
#define _TAG_HELPER_

#include	"cocos2d.h"
#include "Utils/GlobalData.h"

enum ObjectName
{
    ON_ROLE = 1,
    ON_BLOCK,
    ON_EDGE,
    ON_WEAPON,
    ON_ATTACK,
    ON_MONSTOR,
    ON_PILLS
};

class TagHelper
{
public:
	static TagHelper *Instance()
	{
		static TagHelper *instance = new TagHelper();
		return instance;
	}

    int getTag(ObjectName name);
    bool isObject(int tag, ObjectName name);

	inline bool isRoleTag(int tag)
	{
		if(tag == c_tagRole)
			return true;
		else
			return false;
	}
	inline bool isBlockTag(int tag)
	{
		if(tag == c_tagBlock)
			return true;
		else
			return false;
	}
	inline bool isWallTag(int tag)
	{
		if(tag == c_tagWall)
			return true;
		else
			return false;
	}
	
	inline int getRoleTag()
	{
		return c_tagRole;
	}

	inline int getBlockTag()
	{
		return c_tagBlock;
	}

	inline int getWallTag()
	{
		return c_tagWall;
	}


private:
	TagHelper()
	{
		c_tagRole = 1;
		c_tagBlock = 2;
		c_tagWall = 3;
	}

protected:
private:
	int c_tagRole;
    int c_tagWall;
    int c_tagBlock;
};



#endif