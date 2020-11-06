#ifndef __RGNDS_TRANSFORM_CPP__
#define __RGNDS_TRANSFORM_CPP__ 1

#include "../transform.h"
#include <math.h>

namespace RGNDS {
    Transform Transform::_default = Transform();

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

    void Transform::directionFromPositionVector() {
        float dist = sqrt(pos.x * pos.x + pos.y * pos.y);
        dir.x = pos.x / dist;
        dir.y = pos.y / dist;
    }
}
#endif // __RGNDS_TRANSFORM_CPP__
