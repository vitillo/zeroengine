#ifndef M2D_H
#define M2D_H

#include "SDL.h"

#include "Material.h"
#include "Renderer.h"

typedef struct md2_header_t
{
	int ident;                  /* magic number: "IDP2" */
	int version;                /* version: must be 8 */
	
	int skinwidth;              /* texture width */
	int skinheight;             /* texture height */
	
	int framesize;              /* size in bytes of a frame */
	
	int num_skins;              /* number of skins */
	int num_vertices;           /* number of vertices per frame */
	int num_st;                 /* number of texture coordinates */
	int num_tris;               /* number of triangles */
	int num_glcmds;             /* number of opengl commands */
	int num_frames;             /* number of frames */
	
	int offset_skins;           /* offset skin data */
	int offset_st;              /* offset texture coordinate data */
	int offset_tris;            /* offset triangle data */
	int offset_frames;          /* offset frame data */
	int offset_glcmds;          /* offset OpenGL command data */
	int offset_end;             /* offset end of file */
} md2_header;

/* Vector */
typedef float vec3_t[3];

/* Texture name */
typedef struct md2_skin_t
{
	char name[64];              /* texture file name */
} md2_skin;

/* Texture coords */
typedef struct md2_texCoord_t
{
	short s;
	short t;
} md2_texCoord;

/* Triangle info */
typedef struct md2_triangle_t
{
	unsigned short vertex[3];   /* vertex indices of the triangle */
	unsigned short st[3];       /* tex. coord. indices */
} md2_triangle;

/* Compressed vertex */
typedef struct md2_vertex_t
{
	unsigned char v[3];         /* position */
	unsigned char normalIndex;  /* normal vector index */
} md2_vertex;

/* Model frame */
typedef struct md2_frame_t
{
	vec3_t scale;               /* scale factor */
	vec3_t translate;           /* translation vector */
	char name[16];              /* frame name */
	struct md2_vertex_t *verts; /* list of frame's vertices */
} md2_frame;

class MD2Model{
public:
	MD2Model(ZeroEngine::Renderer* renderer, float scale = 0.2f);
	~MD2Model();
	bool LoadModel(const char* filename);
	bool LoadSkin(const char* filename);
	void DrawFrame(int frame);
private:
	const static int md2Ident = 844121161;
	const static int md2Version = 8;
	static float anorms_table[162][3];
	
	SDL_Surface* _textureSurface;
	
	ZeroEngine::Material _material;
	ZeroEngine::Matrix3 _m2dMatrix;
	ZeroEngine::Renderer* _renderer;
	float _scale;
	
	md2_header _header;
	md2_skin* _skins;
	md2_texCoord* _texcoords;
	md2_triangle* _triangles;
	md2_frame* _frames;
	int* _glcmds;
};

#endif
