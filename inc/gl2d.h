#ifndef __RGNDS_GL2D_H__
#define __RGNDS_GL2D_H__ 1

#include "../transform.h"

typedef struct
        {
            int		width;		/*!< Width of the Sprite */
            int 	height;		/*!< Height of the Sprite */
            int 	u_off;		/*!< S texture offset */
            int 	v_off;		/*!< T texture offset */
            int		textureID;  /*!< Texture handle ( used in glDeleteTextures() ) <Br>
                                     The texture handle in VRAM (returned by glGenTextures()) <Br>
                                     ie. This references the actual texture stored in VRAM */
} glImage;

namespace RGNDS {

    enum GL_FLIP_MODE {
        GL_FLIP_NONE = 0,
        GL_FLIP_V,
        GL_FLIP_H
    };



    namespace GL2D {

        extern int defaultFont_TextureID;
        extern glImage defaultFont[64];

        void SetOrtho();

        void glScreen2D();
        void glBegin2D();
        void glEnd2D();

        void glShape(GL_GLBEGIN_ENUM mode, int color, int numPoints, const Point<float> aPoints[], Transform* t = &Transform::_default, int alpha=0);
        void glSprite( int x, int y, int flipmode, const glImage *spr, Transform* tra = &Transform::_default );

        void glPixel(int x, int y, int color, int alpha);

        void glText(const char* text, unsigned short color, Transform* trans, glImage font[64] = defaultFont);

        int glLoadTileSet(
           glImage              *sprite,
           int                  tile_wid,
           int                  tile_hei,
           int                  bmp_wid,
           int                  bmp_hei,
           GL_TEXTURE_TYPE_ENUM type,
           GL_TEXTURE_SIZE_ENUM sizeX,      // Size of entire texture (not just tile-size)
           GL_TEXTURE_SIZE_ENUM sizeY,      // Size of entire texture (not just tile-size)
           int 	                param,
           int					pallette_width,
           const u16			*palette,
           const uint8          *texture
        );

/*        void glSpriteScale( int x, int y, s32 scale, int flipmode, const EngineGL2D::glImage *spr );
        void glSpriteScaleXY( int x, int y, s32 scaleX, s32 scaleY, int flipmode, const EngineGL2D::glImage *spr );
        void glSpriteRotate( int x, int y, s32 angle, int flipmode, const EngineGL2D::glImage *spr );
        void glSpriteRotateScale( int x, int y, s32 angle, s32 scale, int flipmode, const EngineGL2D::glImage *spr);
        void glSpriteRotateScaleXY( int x, int y, s32 angle, s32 scaleX, s32 scaleY, int flipmode, const EngineGL2D::glImage *spr);
        void glSpriteStretchHorizontal(int x, int y, int length_x, const EngineGL2D::glImage *spr );

        void glSpriteOnQuad(
             int x1, int y1,
             int x2, int y2,
             int x3, int y3,
             int x4, int y4,
             int uoff, int voff,
             int flipmode, const EngineGL2D::glImage *spr
           );
        int glLoadSpriteSet(
             EngineGL2D::glImage              *sprite,
             const unsigned int   numframes,
             const unsigned int   *texcoords,
             GL_TEXTURE_TYPE_ENUM type,
             int 	              sizeX,
             int 	              sizeY,
             int 	              param,
             int				  pallette_width,
             const u16			  *palette,
             const uint8          *texture
           );
        int glLoadTileSet(
           EngineGL2D::glImage              *sprite,
           int                  tile_wid,
           int                  tile_hei,
           int                  bmp_wid,
           int                  bmp_hei,
           GL_TEXTURE_TYPE_ENUM type,
           int 	                sizeX,
           int 	                sizeY,
           int 	                param,
           int					pallette_width,
           const u16			*palette,
           const uint8          *texture
         );
    }

    void glPutPixel( int x, int y, int color );
    void glLine( int x1, int y1, int x2, int y2, int color );

    void glBox( int x1, int y1, int x2, int y2, int color );
    void glBoxFilled( int x1, int y1, int x2, int y2, int color );
    void glBoxFilledGradient( int x1, int y1, int x2, int y2,
                              int color1, int color2, int color3, int color4
                            );

    void glTriangle( int x1, int y1, int x2, int y2, int x3, int y3, int color );
    void glTriangleFilled( int x1, int y1, int x2, int y2, int x3, int y3, int color );
    void glTriangleFilledGradient( int x1, int y1, int x2, int y2, int x3, int y3,
                                   int color1, int color2, int color3
                                 );

*/
    }
}


#endif // __RGNDS_ENGINE_GL2D_H__
