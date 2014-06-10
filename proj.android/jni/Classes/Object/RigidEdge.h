#ifndef __RIGID__EDGE__
#define __RIGID__EDGE__

#include "Object/Edge.h"

class RigidEdge : public Edge
{

public:
    ~RigidEdge();

    static RigidEdge* createRigidEdge(CCPoint position, int height, void *parm);
  

private:

    RigidEdge();

    bool initWithDockDir(CCPoint position, int height, void *parm);

private:
    b2Body* m_polyBody;

};


#endif