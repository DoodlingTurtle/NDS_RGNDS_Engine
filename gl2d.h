#ifndef __RGNDS_GL2D_H__
#define __RGNDS_GL2D_H__ 1

#include <nds/arm9/videoGL.h>

#include "transform.h"

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
        void glBegin2D( float screenoffsety = 0 );
        void glEnd2D();

        void glShape(GL_GLBEGIN_ENUM mode, int color, int numPoints, const Point<float> aPoints[], Transform* t = &Transform::_default, int alpha=0, int zDepth=0);
        void glSprite( int x, int y, int flipmode, const glImage *spr, Transform* tra = &Transform::_default, int zDepth=0);

        void glPixel(int x, int y, int color, int alpha=31, int zDepth=0);

        void glText(const char* text, unsigned short color, Transform* trans, int alpha = 31, int zDepth=0, glImage font[64] = defaultFont);

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

    }
}


#endif // __RGNDS_ENGINE_GL2D_H__
