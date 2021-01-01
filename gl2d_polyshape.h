#ifndef __RGNDS_GL2D_POLYSHAPE_H__
#define __RGNDS_GL2D_POLYSHAPE_H__ 1

#include "transform.h"
#include "gl2d.h"

namespace RGNDS {
    namespace GL2D {
        class PolyShape {
        public:

            static PolyShape* createCircle(float radius, int edges, float thickness=0);


            /* rendermode:

            GL_TRIANGLES      GL_TRIANGLE_STRIP
            v0                 v2___v4____v6
            |\      v3         /|\  |\    /\
            | \     /\      v0( | \ | \  /  \
            |__\   /__\        \|__\|__\/____\
            v1 v2 v4  v5       v1   v3  v5   v7

            GL_QUADS                GL_QUAD_STRIP
             v0__v3                 v0__v2____v4     v10__
              /  \   v4____v7        /  \     |\ _____ / /v11
             /    \   |    \        /    \    | |v6 v8| /
            /______\  |_____\      /______\___|_|_____|/
            v1    v2  v5    v6     v1    v3  v5 v7    v9

            */

            PolyShape(short numTris, const Point<double> aTris[], GL_GLBEGIN_ENUM rendermode);
            virtual ~PolyShape();

            virtual void draw(unsigned short color, Transform* t = &Transform::_default, int alpha=0, float zDepth=0.00);

            unsigned short color;

            void setPoint(short index, float x, float y);


        protected:
            short numPoints = 0;
            Point<double> *points;
            GL_GLBEGIN_ENUM rendertype;

        };
    }
}
#endif // __RGNDS_GL2D_POLYSHAPE_H__
