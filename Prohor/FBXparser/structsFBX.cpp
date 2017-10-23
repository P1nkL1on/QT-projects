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
    animRotation = NULL;
    animTrnaslation = NULL;

    lengthFromAttribute = 0;
    BindMatrix = {};
    RotatMatrix = {};

    indexes = {};
    weights = {};
    //correctTransformsCluster = false;

    name = "Unknown";
}

AnimCurve::AnimCurve()
{
    ID = "";
    values = {};
    times = {};
    //typ = -1;
}

AnimNode::AnimNode()
{
    xvalues = {};
    yvalues = {};
    zvalues = {};
    xtimes = {};
    ytimes = {};
    ztimes = {};
    ID = "";
    typ = -1;
}
