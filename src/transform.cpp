#ifndef __RGNDS_TRANSFORM_CPP__
#define __RGNDS_TRANSFORM_CPP__ 1

#include "../transform.h"

namespace RGNDS {
    Transform::Transform(){}
    Transform::~Transform(){}

    void Transform::setAngleRel( float radiants ) {
        setAngle(ang + radiants);
    }
    void Transform::setAngle( float radiants ) {
        if(radiants < 0) {
            radiants = PI2 + radiants;
        }

        if(radiants > PI2) {
            radiants -= PI2;
        }

        ang = radiants;
        dir.y = sin(ang);
        dir.x = cos(ang);
    }

    void Transform::moveInDirection( float distance ) {
        pos += dir * distance;
    }
}
#endif // __RGNDS_TRANSFORM_CPP__
