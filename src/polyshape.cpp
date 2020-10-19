#ifndef __RGNDS_GL2D_POLYSHAPE_CPP__
#define __RGNDS_GL2D_POLYSHAPE_CPP__ 1

#include "../gl2d_polyshape.h"

namespace RGNDS {
    namespace GL2D {

        PolyShape* PolyShape::createCircle(float radius, int edges, int thickness) {
            float angSteps = PI2 / edges;
            float ang = 0;
            int a;

            float radiusinner = 0;

            if(thickness > 0)
                radiusinner = std::min(radius-1, radius-thickness);

            Point<float> p[edges*2+2];

            for(a = 0; a < (edges+1)*2; a+=2) {
                p[a  ].x = cos(ang);
                p[a  ].y = sin(ang);
                p[a+1].x = p[a].x * radius;
                p[a+1].y = p[a].y * radius;
                p[a  ].x *= radiusinner;
                p[a  ].y *= radiusinner;

                ang += angSteps;
            }

            return new PolyShape(edges*2+2, p, GL_QUAD_STRIP);
        }

        PolyShape::PolyShape(short c, const Point<float> aPoints[], GL_GLBEGIN_ENUM renderMode) {
            numPoints = c;
            points = (Point<float>*)valloc(sizeof(Point<float>) * numPoints);
            memcpy(points, aPoints, numPoints*sizeof(Point<float>));

            rendertype = renderMode;

        }
        PolyShape::~PolyShape(){
            if(points != nullptr)
                free(points);
        }

        void PolyShape::draw(unsigned short color, short screen, Transform* t) {

            Point<int> p[numPoints];
            for(int a = 0 ; a < numPoints; a++) {
                t->translate<float, int>(&points[a], &p[a]);
                p[a].y -= (int)screen;
            }

            glShape(rendertype, color, numPoints, p);
        }

        void PolyShape::setPoint(short index, float x, float y) {
            if(index < 0 || index > numPoints) return;

            points[index].x = x;
            points[index].y = y;
        }
    }
}

#endif // __RGNDS_GL2D_POLYSHAPE_CPP__
