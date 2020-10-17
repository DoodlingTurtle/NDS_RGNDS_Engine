#ifndef __RGNDS_ENGINE_GL2D_H__
#define __RGNDS_ENGINE_GL2D_H__

namespace RGNDS {
    class EngineGL2D {
    public:

        enum GL_FLIP_MODE {
            GL_FLIP_NONE = 0,
            GL_FLIP_V,
            GL_FLIP_H
        };

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

        static void SetOrtho();

        static void glScreen2D();
        static void glBegin2D();
        static void glEnd2D();

        static void glShape(GL_GLBEGIN_ENUM mode, int color, int numPoints, const Point<int> aPoints[]);

        static void glPutPixel( int x, int y, int color );
        static void glLine( int x1, int y1, int x2, int y2, int color );

        static void glBox( int x1, int y1, int x2, int y2, int color );
        static void glBoxFilled( int x1, int y1, int x2, int y2, int color );
        static void glBoxFilledGradient( int x1, int y1, int x2, int y2,
                                  int color1, int color2, int color3, int color4
                                );

        static void glTriangle( int x1, int y1, int x2, int y2, int x3, int y3, int color );
        static void glTriangleFilled( int x1, int y1, int x2, int y2, int x3, int y3, int color );
        static void glTriangleFilledGradient( int x1, int y1, int x2, int y2, int x3, int y3,
                                       int color1, int color2, int color3
                                     );

        static void glSprite( int x, int y, int flipmode, const EngineGL2D::glImage *spr );
        static void glSpriteScale( int x, int y, s32 scale, int flipmode, const EngineGL2D::glImage *spr );
        static void glSpriteScaleXY( int x, int y, s32 scaleX, s32 scaleY, int flipmode, const EngineGL2D::glImage *spr );
        static void glSpriteRotate( int x, int y, s32 angle, int flipmode, const EngineGL2D::glImage *spr );
        static void glSpriteRotateScale( int x, int y, s32 angle, s32 scale, int flipmode, const EngineGL2D::glImage *spr);
        static void glSpriteRotateScaleXY( int x, int y, s32 angle, s32 scaleX, s32 scaleY, int flipmode, const EngineGL2D::glImage *spr);
        static void glSpriteStretchHorizontal(int x, int y, int length_x, const EngineGL2D::glImage *spr );

        static void glSpriteOnQuad(
             int x1, int y1,
             int x2, int y2,
             int x3, int y3,
             int x4, int y4,
             int uoff, int voff,
             int flipmode, const EngineGL2D::glImage *spr
           );
        static int glLoadSpriteSet(
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
        static int glLoadTileSet(
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

    };
}

#endif // __RGNDS_ENGINE_GL2D_H__
