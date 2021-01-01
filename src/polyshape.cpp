#ifndef __RGNDS_GL2D_POLYSHAPE_CPP__
#define __RGNDS_GL2D_POLYSHAPE_CPP__ 1

#include "../gl2d_polyshape.h"

namespace RGNDS {
    namespace GL2D {

        PolyShape* PolyShape::createCircle(float radius, int edges, float thickness) {
            double angSteps = PI2 / (float)edges;
            double ang = 0;
            int a;

            float radiusinner = 0;

            if(thickness > 0)
                radiusinner = std::fmin(radius-0.001, radius-thickness);

            Point<double> p[edges*2+2];

            for(a = 0; a < (edges+1)*2; a+=2) {
                p[a  ].x = cosf(ang);
                p[a  ].y = sinf(ang);
                p[a+1].x = p[a].x * radius;
                p[a+1].y = p[a].y * radius;
                p[a  ].x *= radiusinner;
                p[a  ].y *= radiusinner;

                ang += angSteps;
            }

            return new PolyShape(edges*2+2, p, GL_QUAD_STRIP);
        }

        PolyShape::PolyShape(short c, const Point<double> aPoints[], GL_GLBEGIN_ENUM renderMode) {
            numPoints = c;
            points = (Point<double>*)valloc(sizeof(Point<double>) * numPoints);
            memcpy(points, aPoints, numPoints*sizeof(Point<double>));

            rendertype = renderMode;

        }
        PolyShape::~PolyShape(){
            if(points != nullptr)
                free(points);
        }

        void PolyShape::draw(unsigned short color, Transform* t, int alpha, float zDepth) {
            glShape(rendertype, color, numPoints, points, t, alpha, zDepth);
        }

        void PolyShape::setPoint(short index, float x, float y) {
            if(index < 0 || index > numPoints) return;

            points[index].x = x;
            points[index].y = y;
        }
    }
}

#endif // __RGNDS_GL2D_POLYSHAPE_CPP__
