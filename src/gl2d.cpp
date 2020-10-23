#ifndef __RGNDS_GL2D_CPP__
#define __RGNDS_GL2D_CPP__ 1

#include <climits>
#include <math.h>

#include "../inc/gl2d.h"
#include "../inc/fonts_res.h"

inline void gxVertex3i(v16 x, v16 y, v16 z) {
    GFX_VERTEX16 = (y << 16) | (x & 0xFFFF);
    GFX_VERTEX16 = ((uint32)(uint16)z);
}
inline void gxVertex2i(v16 x, v16 y) {
    GFX_VERTEX_XY = (y << 16) | (x & 0xFFFF);
}
inline void gxTexcoord2i(t16 u, t16 v) {
    GFX_TEX_COORD = (v << 20) | ( (u << 4) & 0xFFFF );
}
inline void gxScalef32(s32 x, s32 y, s32 z) {
    MATRIX_SCALE = x;
    MATRIX_SCALE = y;
    MATRIX_SCALE = z;
}

inline void gxTranslate3f32( int32 x, int32 y, int32 z ) {
    MATRIX_TRANSLATE = x;
    MATRIX_TRANSLATE = y;
    MATRIX_TRANSLATE = z;
}

v16 g_depth = 0;
extern int gCurrentTexture;
namespace RGNDS {
    namespace GL2D {

        static int polyId=0;

        int defaultFont_TextureID;
        glImage defaultFont[64];

        void SetOrtho( void ) {
            glMatrixMode( GL_PROJECTION );     // set matrixmode to projection
            glLoadIdentity();				 // reset
            glOrthof32( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1 << 12, 1 << 12 );  // downscale projection matrix
        }

        void glScreen2D( void ) {

            // initialize gl
            glInit();

            //enable textures
            glEnable( GL_TEXTURE_2D );

            // enable antialiasing
            glEnable( GL_ANTIALIAS );

            glEnable( GL_BLEND );

            // setup the rear plane
            glClearColor( 0, 0, 0, 31 ); // BG must be opaque for AA to work
            glClearPolyID( 63 ); // BG must have a unique polygon ID for AA to work

            glClearDepth( GL_MAX_DEPTH );

            //this should work the same as the normal gl call
            glViewport(0,0,255,191);


            //any floating point gl call is being converted to fixed prior to being implemented
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            gluPerspective( 70, 256.0 / 192.0, 1, 200 );

            gluLookAt(	0.0, 0.0, 1.0,		//camera possition
                        0.0, 0.0, 0.0,		//look at
                        0.0, 1.0, 0.0);		//up

            glMaterialf( GL_AMBIENT, RGB15(31,31,31) );
            glMaterialf( GL_DIFFUSE, RGB15(31,31,31) );
            glMaterialf( GL_SPECULAR, BIT(15) | RGB15(31,31,31) );
            glMaterialf( GL_EMISSION, RGB15(31,31,31) );

            //ds uses a table for shinyness..this generates a half-ass one
            glMaterialShinyness();

            //not a real gl function and will likely change
            glPolyFmt( POLY_ALPHA(31) | POLY_CULL_BACK );


            defaultFont_TextureID = glLoadTileSet(
                defaultFont,
                8, 8,
                8, 512,
                GL_RGB256,
                TEXTURE_SIZE_8,
                TEXTURE_SIZE_512,
                GL_TEXTURE_COLOR0_TRANSPARENT,
                3, RGNDS::_8x8pixelfont_palette,
                RGNDS::_8x8pixelfont_pixels
            );

        }

        void glBegin2D( float screenoffset ) {


            // save 3d perpective projection matrix
            glMatrixMode( GL_PROJECTION );
            glPushMatrix();

            // save 3d modelview matrix for safety
            glMatrixMode( GL_MODELVIEW );
            glPushMatrix();


            //what?!! No glDisable(GL_DEPTH_TEST)?!!!!!!
            glEnable( GL_BLEND );
            glEnable( GL_TEXTURE_2D );
            glDisable( GL_ANTIALIAS );		// disable AA
            glDisable( GL_OUTLINE );			// disable edge-marking

            glColor( 0x7FFF ); 				// max color

            glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE );  // no culling

            SetOrtho();

            glMatrixMode( GL_TEXTURE );      	// reset texture matrix just in case we did some funky stuff with it
            glLoadIdentity();

            glMatrixMode( GL_MODELVIEW );		// reset modelview matrix. No need to scale up by << 12
            glLoadIdentity();

            glTranslatef32(1, -screenoffset, 1);

            gCurrentTexture = 0; // set current texture to 0
            g_depth = 0; 	// set depth to 0. We need this var since we cannot disable depth testing
            polyId = 0;
        }

        void glEnd2D( void ) {

            // restore 3d matrices and set current matrix to modelview
            glMatrixMode( GL_PROJECTION );
            glPopMatrix( 1 );
            glMatrixMode( GL_MODELVIEW );
            glPopMatrix( 1 );

        }

        void glShape(GL_GLBEGIN_ENUM mode, int color, int numPoints, const Point<double> aPoints[], Transform* tra, int alpha) {
            if(numPoints <= 0) return;

            glBindTexture( 0, 0 );
            glColor(color);
            glPushMatrix();
                if(alpha > 0) {
                    polyId = (polyId+1)%64;
                    glPolyFmt(POLY_ALPHA(std::fmax(0, std::fmin(alpha, 31))) | POLY_CULL_NONE | POLY_ID(polyId));
                }
                glBegin(mode);
                    glTranslatef32(tra->pos.x, tra->pos.y, 1);
                    glRotateZ((tra->ang / PI2) * 356); //(tra->ang / PI2) * (0xffff - SHRT_MAX));
                    glScalef32((1<<12) * tra->scale, (1<<12) * tra->scale, (1<<12));

                    gxVertex3i( aPoints[0].x, aPoints[0].y, g_depth );
                    for(int a = 1; a < numPoints; a++) {
                        gxVertex2i(aPoints[a].x, aPoints[a].y);
                    }
                glEnd();
                if(alpha > 0) {
                    polyId = (polyId+1)%64;
                    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(polyId));
                }
            glPopMatrix(1);

            glColor(0x7fff);
            g_depth++;
            gCurrentTexture = 0;
        }

        void glPixel(int x, int y, int color, int alpha) {
            Point<double> p[4] = {
                {(double)x,   (double)y  }
              , {(double)x+1, (double)y  }
              , {(double)x+1, (double)y+1}
              , {(double)x,   (double)y+1}
            };
            glShape(GL_QUADS, color, 4, p, &Transform::_default, alpha);
        }

        void glSprite(int flipmode, const glImage *spr, Transform* tra ) {
            int x1 = 0;
            int y1 = 0;
            int x2 = spr->width;
            int y2 = spr->height;

            int	u1 = spr->u_off + (( flipmode & GL_FLIP_H ) ? spr->width-1  : 0);
            int	u2 = spr->u_off + (( flipmode & GL_FLIP_H ) ? 0			    : spr->width);
            int v1 = spr->v_off + (( flipmode & GL_FLIP_V ) ? spr->height-1 : 0);
            int v2 = spr->v_off + (( flipmode & GL_FLIP_V ) ? 0 		    : spr->height);

            if ( spr->textureID != gCurrentTexture )
            {
                glBindTexture( GL_TEXTURE_2D, spr->textureID );
                gCurrentTexture = spr->textureID;
            }

            s32 scale = (s32)((1 << 12) * tra->scale);
            glPushMatrix();

                gxTranslate3f32( tra->pos.x * tra->scale, tra->pos.y * tra->scale, 0 );
                gxScalef32( scale , scale, 1 << 12 );
                glRotateZi( (tra->ang / PI2 * 0x7fff) );

                glBegin( GL_QUADS );

                    gxTexcoord2i( u1, v1 ); gxVertex3i( x1, y1, g_depth );
                    gxTexcoord2i( u1, v2 ); gxVertex2i( x1, y2 );
                    gxTexcoord2i( u2, v2 ); gxVertex2i( x2, y2 );
                    gxTexcoord2i( u2, v1 ); gxVertex2i( x2, y1 );

                glEnd();
            glPopMatrix(1);

            g_depth++;
        }

        int glLoadTileSet( glImage              *sprite,
                           int                  tile_wid,
                           int                  tile_hei,
                           int                  bmp_wid,
                           int                  bmp_hei,
                           GL_TEXTURE_TYPE_ENUM type,
                           GL_TEXTURE_SIZE_ENUM sizeX,
                           GL_TEXTURE_SIZE_ENUM sizeY,
                           int 	                param,
                           int					pallette_width,
                           const u16			*palette,
                           const uint8          *texture
        ){

            int textureID;
            glGenTextures( 1, &textureID );
            glBindTexture( 0, textureID );
            glTexImage2D( 0, 0, type, sizeX, sizeY, 0, param, texture );
            glColorTableEXT( 0, 0, pallette_width, 0, 0, palette );

            int i=0;
            int x, y;

            // init sprites texture coords and texture ID
            for (y = 0; y < (bmp_hei/tile_hei); y++)
            {
                for (x = 0; x < (bmp_wid/tile_wid); x++)
                {
                    sprite[i].width 			= tile_wid;
                    sprite[i].height 			= tile_hei;
                    sprite[i].u_off				= x*tile_wid;
                    sprite[i].v_off				= y*tile_hei;
                    sprite[i].textureID 		= textureID;
                    i++;
                }
            }

            return textureID;
        }

        void glText(const char* text, unsigned short color, Transform* tra,  glImage font[64]) {
            byte* ptr = (byte*)text;
            byte c = *ptr;

            Transform t;
            t.scale = tra->scale;
            t.pos = tra->pos;
            t.ang = tra->ang;

            glColor( color );

            while(c != 0) {

            // Handle special cases
                if((char)c == '\n') {
                    t.pos.x = tra->pos.x;
                    t.pos.y += font->height;

                    ptr++;
                    c = *ptr;
                    continue;
                }

            // convert lowercase to uppercase letters
                if(c > 96 && c < 173)
                    c -= 32;

            // make sure only the supported characters are displayed
                if(c <32 || c > 95)
                    c = 32;

            // Translate Char to TileIndex
                glSprite(GL_FLIP_NONE, &(font[c-32]), &t);

                t.pos.x += font->width;
                ptr++;
                c = *ptr;
            }

            glColor(0xFFFF);
        }
    }
}

#endif // __RGNDS_GL2D_CPP__
