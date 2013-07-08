#include <iostream>
#include <fstream>
#include <string>

#include <SDL_image/SDL_image.h>

#include "Vertex.h"
#include "MD2Model.h"

#define PI (3.141592653589793)

float MD2Model::anorms_table[162][3] = {
	{ -0.525731f,  0.000000f,  0.850651f }, 
	{ -0.442863f,  0.238856f,  0.864188f }, 
	{ -0.295242f,  0.000000f,  0.955423f }, 
	{ -0.309017f,  0.500000f,  0.809017f }, 
	{ -0.162460f,  0.262866f,  0.951056f }, 
	{  0.000000f,  0.000000f,  1.000000f }, 
	{  0.000000f,  0.850651f,  0.525731f }, 
	{ -0.147621f,  0.716567f,  0.681718f }, 
	{  0.147621f,  0.716567f,  0.681718f }, 
	{  0.000000f,  0.525731f,  0.850651f }, 
	{  0.309017f,  0.500000f,  0.809017f }, 
	{  0.525731f,  0.000000f,  0.850651f }, 
	{  0.295242f,  0.000000f,  0.955423f }, 
	{  0.442863f,  0.238856f,  0.864188f }, 
	{  0.162460f,  0.262866f,  0.951056f }, 
	{ -0.681718f,  0.147621f,  0.716567f }, 
	{ -0.809017f,  0.309017f,  0.500000f }, 
	{ -0.587785f,  0.425325f,  0.688191f }, 
	{ -0.850651f,  0.525731f,  0.000000f }, 
	{ -0.864188f,  0.442863f,  0.238856f }, 
	{ -0.716567f,  0.681718f,  0.147621f }, 
	{ -0.688191f,  0.587785f,  0.425325f }, 
	{ -0.500000f,  0.809017f,  0.309017f }, 
	{ -0.238856f,  0.864188f,  0.442863f }, 
	{ -0.425325f,  0.688191f,  0.587785f }, 
	{ -0.716567f,  0.681718f, -0.147621f }, 
	{ -0.500000f,  0.809017f, -0.309017f }, 
	{ -0.525731f,  0.850651f,  0.000000f }, 
	{  0.000000f,  0.850651f, -0.525731f }, 
	{ -0.238856f,  0.864188f, -0.442863f }, 
	{  0.000000f,  0.955423f, -0.295242f }, 
	{ -0.262866f,  0.951056f, -0.162460f }, 
	{  0.000000f,  1.000000f,  0.000000f }, 
	{  0.000000f,  0.955423f,  0.295242f }, 
	{ -0.262866f,  0.951056f,  0.162460f }, 
	{  0.238856f,  0.864188f,  0.442863f }, 
	{  0.262866f,  0.951056f,  0.162460f }, 
	{  0.500000f,  0.809017f,  0.309017f }, 
	{  0.238856f,  0.864188f, -0.442863f }, 
	{  0.262866f,  0.951056f, -0.162460f }, 
	{  0.500000f,  0.809017f, -0.309017f }, 
	{  0.850651f,  0.525731f,  0.000000f }, 
	{  0.716567f,  0.681718f,  0.147621f }, 
	{  0.716567f,  0.681718f, -0.147621f }, 
	{  0.525731f,  0.850651f,  0.000000f }, 
	{  0.425325f,  0.688191f,  0.587785f }, 
	{  0.864188f,  0.442863f,  0.238856f }, 
	{  0.688191f,  0.587785f,  0.425325f }, 
	{  0.809017f,  0.309017f,  0.500000f }, 
	{  0.681718f,  0.147621f,  0.716567f }, 
	{  0.587785f,  0.425325f,  0.688191f }, 
	{  0.955423f,  0.295242f,  0.000000f }, 
	{  1.000000f,  0.000000f,  0.000000f }, 
	{  0.951056f,  0.162460f,  0.262866f }, 
	{  0.850651f, -0.525731f,  0.000000f }, 
	{  0.955423f, -0.295242f,  0.000000f }, 
	{  0.864188f, -0.442863f,  0.238856f }, 
	{  0.951056f, -0.162460f,  0.262866f }, 
	{  0.809017f, -0.309017f,  0.500000f }, 
	{  0.681718f, -0.147621f,  0.716567f }, 
	{  0.850651f,  0.000000f,  0.525731f }, 
	{  0.864188f,  0.442863f, -0.238856f }, 
	{  0.809017f,  0.309017f, -0.500000f }, 
	{  0.951056f,  0.162460f, -0.262866f }, 
	{  0.525731f,  0.000000f, -0.850651f }, 
	{  0.681718f,  0.147621f, -0.716567f }, 
	{  0.681718f, -0.147621f, -0.716567f }, 
	{  0.850651f,  0.000000f, -0.525731f }, 
	{  0.809017f, -0.309017f, -0.500000f }, 
	{  0.864188f, -0.442863f, -0.238856f }, 
	{  0.951056f, -0.162460f, -0.262866f }, 
	{  0.147621f,  0.716567f, -0.681718f }, 
	{  0.309017f,  0.500000f, -0.809017f }, 
	{  0.425325f,  0.688191f, -0.587785f }, 
	{  0.442863f,  0.238856f, -0.864188f }, 
	{  0.587785f,  0.425325f, -0.688191f }, 
	{  0.688191f,  0.587785f, -0.425325f }, 
	{ -0.147621f,  0.716567f, -0.681718f }, 
	{ -0.309017f,  0.500000f, -0.809017f }, 
	{  0.000000f,  0.525731f, -0.850651f }, 
	{ -0.525731f,  0.000000f, -0.850651f }, 
	{ -0.442863f,  0.238856f, -0.864188f }, 
	{ -0.295242f,  0.000000f, -0.955423f }, 
	{ -0.162460f,  0.262866f, -0.951056f }, 
	{  0.000000f,  0.000000f, -1.000000f }, 
	{  0.295242f,  0.000000f, -0.955423f }, 
	{  0.162460f,  0.262866f, -0.951056f }, 
	{ -0.442863f, -0.238856f, -0.864188f }, 
	{ -0.309017f, -0.500000f, -0.809017f }, 
	{ -0.162460f, -0.262866f, -0.951056f }, 
	{  0.000000f, -0.850651f, -0.525731f }, 
	{ -0.147621f, -0.716567f, -0.681718f }, 
	{  0.147621f, -0.716567f, -0.681718f }, 
	{  0.000000f, -0.525731f, -0.850651f }, 
	{  0.309017f, -0.500000f, -0.809017f }, 
	{  0.442863f, -0.238856f, -0.864188f }, 
	{  0.162460f, -0.262866f, -0.951056f }, 
	{  0.238856f, -0.864188f, -0.442863f }, 
	{  0.500000f, -0.809017f, -0.309017f }, 
	{  0.425325f, -0.688191f, -0.587785f }, 
	{  0.716567f, -0.681718f, -0.147621f }, 
	{  0.688191f, -0.587785f, -0.425325f }, 
	{  0.587785f, -0.425325f, -0.688191f }, 
	{  0.000000f, -0.955423f, -0.295242f }, 
	{  0.000000f, -1.000000f,  0.000000f }, 
	{  0.262866f, -0.951056f, -0.162460f }, 
	{  0.000000f, -0.850651f,  0.525731f }, 
	{  0.000000f, -0.955423f,  0.295242f }, 
	{  0.238856f, -0.864188f,  0.442863f }, 
	{  0.262866f, -0.951056f,  0.162460f }, 
	{  0.500000f, -0.809017f,  0.309017f }, 
	{  0.716567f, -0.681718f,  0.147621f }, 
	{  0.525731f, -0.850651f,  0.000000f }, 
	{ -0.238856f, -0.864188f, -0.442863f }, 
	{ -0.500000f, -0.809017f, -0.309017f }, 
	{ -0.262866f, -0.951056f, -0.162460f }, 
	{ -0.850651f, -0.525731f,  0.000000f }, 
	{ -0.716567f, -0.681718f, -0.147621f }, 
	{ -0.716567f, -0.681718f,  0.147621f }, 
	{ -0.525731f, -0.850651f,  0.000000f }, 
	{ -0.500000f, -0.809017f,  0.309017f }, 
	{ -0.238856f, -0.864188f,  0.442863f }, 
	{ -0.262866f, -0.951056f,  0.162460f }, 
	{ -0.864188f, -0.442863f,  0.238856f }, 
	{ -0.809017f, -0.309017f,  0.500000f }, 
	{ -0.688191f, -0.587785f,  0.425325f }, 
	{ -0.681718f, -0.147621f,  0.716567f }, 
	{ -0.442863f, -0.238856f,  0.864188f }, 
	{ -0.587785f, -0.425325f,  0.688191f }, 
	{ -0.309017f, -0.500000f,  0.809017f }, 
	{ -0.147621f, -0.716567f,  0.681718f }, 
	{ -0.425325f, -0.688191f,  0.587785f }, 
	{ -0.162460f, -0.262866f,  0.951056f }, 
	{  0.442863f, -0.238856f,  0.864188f }, 
	{  0.162460f, -0.262866f,  0.951056f }, 
	{  0.309017f, -0.500000f,  0.809017f }, 
	{  0.147621f, -0.716567f,  0.681718f }, 
	{  0.000000f, -0.525731f,  0.850651f }, 
	{  0.425325f, -0.688191f,  0.587785f }, 
	{  0.587785f, -0.425325f,  0.688191f }, 
	{  0.688191f, -0.587785f,  0.425325f }, 
	{ -0.955423f,  0.295242f,  0.000000f }, 
	{ -0.951056f,  0.162460f,  0.262866f }, 
	{ -1.000000f,  0.000000f,  0.000000f }, 
	{ -0.850651f,  0.000000f,  0.525731f }, 
	{ -0.955423f, -0.295242f,  0.000000f }, 
	{ -0.951056f, -0.162460f,  0.262866f }, 
	{ -0.864188f,  0.442863f, -0.238856f }, 
	{ -0.951056f,  0.162460f, -0.262866f }, 
	{ -0.809017f,  0.309017f, -0.500000f }, 
	{ -0.864188f, -0.442863f, -0.238856f }, 
	{ -0.951056f, -0.162460f, -0.262866f }, 
	{ -0.809017f, -0.309017f, -0.500000f }, 
	{ -0.681718f,  0.147621f, -0.716567f }, 
	{ -0.681718f, -0.147621f, -0.716567f }, 
	{ -0.850651f,  0.000000f, -0.525731f }, 
	{ -0.688191f,  0.587785f, -0.425325f }, 
	{ -0.587785f,  0.425325f, -0.688191f }, 
	{ -0.425325f,  0.688191f, -0.587785f }, 
	{ -0.425325f, -0.688191f, -0.587785f }, 
	{ -0.587785f, -0.425325f, -0.688191f }, 
	{ -0.688191f, -0.587785f, -0.425325f }};

MD2Model::MD2Model(ZeroEngine::Renderer* renderer, float scale) : _textureSurface(NULL), _skins(NULL), _texcoords(NULL), _triangles(NULL), _frames(NULL), _glcmds(NULL), _renderer(renderer), _scale(scale), _material(), _m2dMatrix(), _header(){	
	_m2dMatrix.MakeEulerMatrix(-PI/2.0f, 0.0f, -PI/2.0f);
}

MD2Model::~MD2Model(){
	delete[] _skins;
	delete[] _texcoords;
	delete[] _triangles;
	delete[] _frames;
	delete[] _glcmds;
}

bool MD2Model::LoadModel(const char* filename){
	std::ifstream file;
	
	file.open(filename, std::ios::in | std::ios::binary);
	if(file.fail())
		return false;
	
	file.read((char *) &_header, sizeof(md2_header));
	if(_header.ident != md2Ident || _header.version != md2Version){
		file.close();
		return false;
	}
	_skins = new md2_skin[_header.num_skins];
	_texcoords = new md2_texCoord[_header.num_st];
	_triangles = new md2_triangle[_header.num_tris];
	_frames = new md2_frame[_header.num_frames];
	_glcmds = new int[_header.num_glcmds];
	
	file.seekg(_header.offset_skins, std::ios::beg);
	file.read((char*)_skins, _header.num_skins * sizeof(md2_skin));
	
	file.seekg(_header.offset_st, std::ios::beg);
	file.read((char*)_texcoords, _header.num_st * sizeof(md2_texCoord));
	
	file.seekg(_header.offset_tris, std::ios::beg);
	file.read((char*)_triangles, _header.num_tris * sizeof(md2_triangle));
	
	file.seekg(_header.offset_glcmds, std::ios::beg);
	file.read((char*)_glcmds, _header.num_glcmds * sizeof(int));
	
	file.seekg(_header.offset_frames, std::ios::beg);
	for(int i = 0; i < _header.num_frames; i++){
		_frames[i].verts = new md2_vertex[_header.num_vertices];
		
		// Read frame data
		file.read((char*)_frames[i].scale, sizeof(vec3_t));
		file.read((char*)_frames[i].translate, sizeof(vec3_t));
		file.read((char*)_frames[i].name, sizeof(char) * 16);
		file.read((char*)_frames[i].verts, sizeof(md2_vertex) * _header.num_vertices);
	}

	file.close();
	return true;
}

bool MD2Model::LoadSkin(const char* filename){
	if((_textureSurface = IMG_Load(filename))){
		_material = ZeroEngine::Material(ZeroEngine::RGBAColor(0.4f,0.4f,0.4f), ZeroEngine::RGBAColor(0.5f, 0.5f, 0.5f), 1, ZeroEngine::TextureInfo(_textureSurface->pixels, _textureSurface->format->palette->colors, _textureSurface->w, _textureSurface->h, _textureSurface->format));
		return true;
	}
	return false;
}

void MD2Model::DrawFrame(int frame){
	if(frame < 0 || frame >= _header.num_frames) return;
	
	if(_textureSurface) _renderer->SetMaterial(_material);
	_renderer->PushMatrix(_m2dMatrix);
	_renderer->SetMode(ZeroEngine::BACKFACECULLING_MODE);
	
	md2_frame pframe;
	md2_vertex pvertex;
	ZeroEngine::Vertex vertices[3];
	// Draw each triangle
	for(int i = 0; i < _header.num_tris; i++){
		for(int j = 0; j < 3; j++){
			pframe = _frames[frame];
			pvertex = pframe.verts[_triangles[i].vertex[j]];
			
			vertices[j].Texel.X = (_texcoords[_triangles[i].st[j]].s / (float)_header.skinwidth) * _header.skinwidth;
			vertices[j].Texel.Y = (_texcoords[_triangles[i].st[j]].t / (float)_header.skinheight) * _header.skinheight;
			
			vertices[j].Normal.X = anorms_table[pvertex.normalIndex][0];
			vertices[j].Normal.Y = anorms_table[pvertex.normalIndex][1];
			vertices[j].Normal.Z = anorms_table[pvertex.normalIndex][2];
			
			vertices[j].Position.X = pframe.scale[0] * _scale * pvertex.v[0] + pframe.translate[0] * _scale;
			vertices[j].Position.Y = pframe.scale[1] * _scale * pvertex.v[1] + pframe.translate[1] * _scale;
			vertices[j].Position.Z = pframe.scale[2] * _scale * pvertex.v[2] + pframe.translate[2] * _scale;
		}
		_renderer->DrawTriangle(vertices[0], vertices[1], vertices[2]);
	}
	
	_renderer->UnsetMode(ZeroEngine::BACKFACECULLING_MODE);
	_renderer->PopMatrix();
	_renderer->UnsetMaterial();
}
