#include "structsFBX.h"

Cluster::Cluster()
{
    ID = "";
    indexes = {};
    weights = {};
}

LimbNodeAttribute::LimbNodeAttribute()
{
    ID = "";
    length = 0;
}

LimbNode::LimbNode()
{
    ID = "";
    translation = {};
    rotation = {};
    pater = NULL;
    lengthFromAttribute = 0;
    BindMatrix = {};
}
