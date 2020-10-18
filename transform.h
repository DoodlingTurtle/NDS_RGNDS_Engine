#ifndef __RGNDS_TRANSFORM_H__
#define __RGNDS_TRANSFORM_H__ 1

#include "inc/point.h"

namespace RGNDS {

    class Transform {

    public:
        Point<float>   pos;
        float scale = 1.0f;

        static Transform _default;

        Transform();
        virtual ~Transform();

        void moveInDirection( float distance );

        template <typename TI, typename TO>
        void translate(Point<TI>* in, Point<TO>* out, Transform* parent = nullptr);

        void setAngle(float radiants);
        void setAngleRel( float radiants );

    protected:
        float ang = 0.0;
        Point<float> dir = {1.0f, 0.0f};

    };

    template <typename TI, typename TO>
    void Transform::translate(Point<TI>* in, Point<TO>* out, Transform* parent) {
        if(parent == nullptr)
            parent = this;

        Point<double> tr;

        /*
        +-  -+   +--                  --+   +-  -+
        |tr.x| = | cos(ang), - sin(ang) | * |in.x|
        |tr.y|   | sin(ang),   cos(ang) |   |in.y|
        +-  -+   +--                  --+   +-  -+
        */

        tr.x = parent->dir.x * in->x + -parent->dir.y * in->y;
        tr.y = parent->dir.y * in->x +  parent->dir.x * in->y;

        tr *= parent->scale;
        tr += parent->pos;

        *out = tr.to<TO>();
    }
}


#endif // __RGNDS_TRANSFORM_H__
