#include <iostream>
#include <sstream>

#include "Vertex.h"
#include "Matrix4.h"
#include "SampleApplication.h"

#define PI (3.141592653589793)
#define MOVEMENT 5.0f

using namespace ZeroEngine;

SampleApplication::SampleApplication(int w, int h, int d, bool f) : SDLApplication(w, h, d, f), _directionChanged(false), _cubeAngle(0), _hunter(_renderer, 0.2f), _marine(_renderer, 0.18f), _cube(_renderer, 0.1), _cameraMatrix(), _cameraAngle(0.0f), _cameraTrans(0.0f), _floorMaterial(RGBAColor(0.4f,0.4f,0.4f), RGBAColor(0.5f, 0.5f, 0.5f), 1), _cubeAlpha(true) {};

void SampleApplication::Initialize(){
	_renderer->SetMode(LIGHT_MODE);
	initializeFloorVertices(Vector3(0.0f, -10.0f, -35.0f), 5, 5.0f);

	bool load = _hunter.LoadModel("./models/hunter.md2");
	load &= _hunter.LoadSkin("./textures/hunter.bmp");
	
	load &= _marine.LoadModel("./models/marine.md2");
	load &= _marine.LoadSkin("./textures/marine.pcx");
	
	load &= _cube.LoadModel("./models/cube.md2");
	load &= _cube.LoadSkin("./textures/cube.bmp");
	
	TextureInfo texture;
	if((load &= _textureManager->Load("./textures/floor.bmp", &texture))) _floorMaterial.Texture = texture;
	
	if(!load){
		std::cerr<<"Models and/or textures not found!\n";
		exit(-1);
	}
}

void SampleApplication::Update(){
	updateCameraMatrix();
	_cubeAngle += 0.05f;
}

void SampleApplication::initializeCubeVertices(){
	_cubeVBuffer[0].Set(Vector3(-2.5f, -2.5f, 2.5f), Vector3(-1.0f, -1.0f, 1.0f));
	_cubeVBuffer[1].Set(Vector3(-2.5f + 5.0f, -2.5f, 2.5f), Vector3(1.0f, -1.0f, 1.0f));
	_cubeVBuffer[2].Set(Vector3(-2.5f, -2.5f + 5.0f, 2.5f), Vector3(-1.0f, 1.0f, 1.0f));
	_cubeVBuffer[3].Set(Vector3(-2.5f + 5.0f, -2.5f+5.0f, 2.5f), Vector3(1.0f, 1.0f, 1.0f));
	_cubeVBuffer[4].Set(Vector3(-2.5f + 5.0f, -2.5f, 2.5f - 5.0f), Vector3(1.0f, -1.0f, -1.0f));
	_cubeVBuffer[5].Set(Vector3(-2.5f + 5.0f, -2.5f + 5.0f, 2.5f - 5.0f), Vector3(1.0f, 1.0f, -1.0f));
	_cubeVBuffer[6].Set(Vector3(-2.5f, -2.5f, 2.5f - 5.0f), Vector3(-1.0f, -1.0f, -1.0f));
	_cubeVBuffer[7].Set(Vector3(-2.5f, -2.5f + 5.0f, 2.5f - 5.0f), Vector3(-1.0f, 1.0f, -1.0f));	
	
	_cubeIndex[0] = 0; _cubeIndex[1] = 1; _cubeIndex[2] = 2; _cubeIndex[3] = 1; _cubeIndex[4] = 3; _cubeIndex[5] = 2;
	_cubeIndex[6] = 1; _cubeIndex[7] = 4; _cubeIndex[8] = 3; _cubeIndex[9] = 4; _cubeIndex[10] = 5; _cubeIndex[11] = 3;
	_cubeIndex[12] = 6; _cubeIndex[13] = 4; _cubeIndex[14] = 7; _cubeIndex[15] = 4; _cubeIndex[16] = 5; _cubeIndex[17] = 7;
	_cubeIndex[18] = 0; _cubeIndex[19] = 6; _cubeIndex[20] = 2; _cubeIndex[21] = 6; _cubeIndex[22] = 7; _cubeIndex[23] = 2;
	_cubeIndex[24] = 2; _cubeIndex[25] = 3; _cubeIndex[26] = 7; _cubeIndex[27] = 3; _cubeIndex[28] = 5; _cubeIndex[29] = 7;
	_cubeIndex[30] = 0; _cubeIndex[31] = 1; _cubeIndex[32] = 6; _cubeIndex[33] = 1; _cubeIndex[34] = 4; _cubeIndex[35] = 6;
}

void SampleApplication::initializeFloorVertices(const Vector3& center, int n, float length){
	Vector3 normal(0.0f, 1.0f, 0.0f);
	RGBAColor color;
	float width = n * length;
	float halfWidth = width / 2.0f;
	float texCoord = n * 255.0f;
	_floorVBuffer[0].Set(Vector3(center.X - halfWidth, center.Y, center.Z + halfWidth), normal, color, Vector2(0.0f, 0.0f));
	_floorVBuffer[1].Set(Vector3(center.X + halfWidth, center.Y, center.Z + halfWidth), normal, color, Vector2(texCoord, 0.0f));
	_floorVBuffer[2].Set(Vector3(center.X - halfWidth, center.Y, center.Z - halfWidth), normal, color, Vector2(0.0f, texCoord));
	_floorVBuffer[3].Set(Vector3(center.X + halfWidth, center.Y, center.Z - halfWidth), normal, color, Vector2(texCoord, texCoord));
	
}

void SampleApplication::drawCube(){
	_renderer->DrawIndexedTriangleList(_cubeVBuffer, _cubeIndex, 12);
}

void SampleApplication::drawFloor(){
	_renderer->DrawTriangle(_floorVBuffer[0], _floorVBuffer[1], _floorVBuffer[2]);
	_renderer->DrawTriangle(_floorVBuffer[1], _floorVBuffer[3], _floorVBuffer[2]);
}

void SampleApplication::drawGrid(float x, float y, float z, float length, float distance, const RGBAColor& color){
	int unit = length / distance;
	Vertex start, end;
	
	start.Color = color;
	start.Normal = Vector3(0.0f, 1.0f, 0.0f);
	end.Color = color;
	end.Normal = Vector3(0.0f, 1.0f, 0.0f);
	
	for(int i = 0; i <= unit; i++){
		start.Position.Set(x, y, z - distance * i);
		end.Position.Set(x + length, y, z - distance * i);
		_renderer->DrawLine(start, end);
	}
	for(int i = 0; i <= unit; i++){
		start.Position.Set(x + distance * i, y, z);
		end.Position.Set(x + distance * i, y, z - length);
		_renderer->DrawLine(start, end);
	}
	
}

void SampleApplication::Draw(){
	_renderer->SetViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
	_renderer->SetPerspectiveProjection(PI/4.0f, (float)_viewportWidth / (float)_viewportHeight, 1.0f, 300.0f);
	_renderer->PushMatrix(_cameraMatrix);
	_renderer->AddLight(Vector3(0.0f, 1.0f, 1.0f));

	drawGrid(-250.0f, -11.0f, 250.0f, 500.0f, 5.0f, RGBAColor(0.0f, 0.0f, 0.2f));

	_renderer->SetMaterial(_floorMaterial);
	drawFloor();

	Matrix4 modelTranslationMatrix;
	modelTranslationMatrix.MakeTranslationMatrix(Vector3(-9.0f, -5.0f, -40.0f));
	_renderer->PushMatrix(modelTranslationMatrix);
	_hunter.DrawFrame(0);
	_renderer->PopMatrix();
	
	modelTranslationMatrix.MakeTranslationMatrix(Vector3(9.0f, -5.6f, -40.0f));
	_renderer->PushMatrix(modelTranslationMatrix);
	_marine.DrawFrame(0);
	_renderer->PopMatrix();
	
	if(_cubeAlpha) _renderer->SetMode(ALPHA_MODE);
	_renderer->PushMatrix(generateCubeMatrix(Vector3(0.0f, -5.0f, -30.0f)));
	_cube.DrawFrame(0);
	_renderer->PopMatrix();
	if(_cubeAlpha) _renderer->UnsetMode(ALPHA_MODE);

	_renderer->UnsetMaterial();
	_renderer->RemoveLights();
	_renderer->PopMatrix();
}

void SampleApplication::HandleRepeatingKeys(){
	Vector3 direction;
	Uint8 * keystate = SDL_GetKeyState(NULL);
	
	if(keystate[SDLK_SPACE]) _cameraTrans.Y = _cameraTrans.Y + 1.0f;
	if(keystate[SDLK_c]) _cameraTrans.Y = _cameraTrans.Y - 1.0f;
	if(keystate[SDLK_a]){
		direction = getMoveDirection();
		_cameraTrans.X = _cameraTrans.X + direction.Z;
		_cameraTrans.Z = _cameraTrans.Z - direction.X;
	}
	if(keystate[SDLK_d]){
		direction = getMoveDirection();
		_cameraTrans.X = _cameraTrans.X - direction.Z;
		_cameraTrans.Z = _cameraTrans.Z + direction.X;
	}
	if(keystate[SDLK_w]){
		direction = getMoveDirection();
		_cameraTrans.X = _cameraTrans.X + direction.X;
		_cameraTrans.Z = _cameraTrans.Z + direction.Z;
	}
	if(keystate[SDLK_s]){ 
		direction = getMoveDirection();
		_cameraTrans.X = _cameraTrans.X - direction.X;
		_cameraTrans.Z = _cameraTrans.Z - direction.Z;
	}
}

bool SampleApplication::HandleSDLEvent(SDL_Event event){
	switch(event.type){
		case SDL_MOUSEMOTION:
			if(event.motion.xrel != 0) _cameraAngle.Y = _cameraAngle.Y - event.motion.xrel/1000.0f;
			if(event.motion.yrel != 0) _cameraAngle.X = _cameraAngle.X - event.motion.yrel/1000.0f;
			_directionChanged = true;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					return true;
					break;
				case SDLK_p:
					if(_cubeAlpha) _cubeAlpha = false;
					else _cubeAlpha = true;
					break;
				case SDLK_l:
					if(_renderer->GetMode() & LIGHT_MODE) _renderer->UnsetMode(LIGHT_MODE);
					else _renderer->SetMode(LIGHT_MODE);
					break;
				case SDLK_m:
					if(_renderer->GetMode() & WIREFRAME_MODE) _renderer->UnsetMode(WIREFRAME_MODE);
					else _renderer->SetMode(WIREFRAME_MODE);
					break;
				default:
					break;
			}
		default:
			break;
	}
	return false;
}

Vector3 SampleApplication::getMoveDirection(){
	static Vector3 direction;
	if(_directionChanged){
		_directionChanged = false;
		Matrix3 rotation;
		rotation.MakeEulerMatrix(_cameraAngle.Y, _cameraAngle.X, 0.0f);
		direction = rotation * Vector3(0.0f, 0.0f, -1.0f);
	}
	return direction;
}

void SampleApplication::updateCameraMatrix(){
	Matrix3 rotation;
	rotation.MakeEulerMatrix( - _cameraAngle.Y, - _cameraAngle.X, 0.0f);
	_cameraMatrix[0] = rotation[0];
	_cameraMatrix[1] = rotation[1];
	_cameraMatrix[2] = rotation[2];
	_cameraMatrix[3] =  - (rotation[0]*_cameraTrans.X + rotation[1]*_cameraTrans.Y + rotation[2]*_cameraTrans.Z);
	_cameraMatrix[4] = rotation[3];
	_cameraMatrix[5] = rotation[4];
	_cameraMatrix[6] = rotation[5];
	_cameraMatrix[7] = - (rotation[3]*_cameraTrans.X + rotation[4]*_cameraTrans.Y + rotation[5]*_cameraTrans.Z);
	_cameraMatrix[8] = rotation[6];
	_cameraMatrix[9] = rotation[7];
	_cameraMatrix[10] = rotation[8];
	_cameraMatrix[11] = - (rotation[6]*_cameraTrans.X + rotation[7]*_cameraTrans.Y + rotation[8]*_cameraTrans.Z);
	_cameraMatrix[12] = 0;
	_cameraMatrix[13] = 0;
	_cameraMatrix[14] = 0;
	_cameraMatrix[15] = 1;
}

Matrix4 SampleApplication::generateCubeMatrix(Vector3 position){
	Matrix3 rotation;
	rotation.MakeEulerMatrix(_cubeAngle, _cubeAngle, _cubeAngle);
	Matrix4 fromOrigin;
	fromOrigin.MakeTranslationMatrix(position);
	return fromOrigin * rotation;	
}

void SampleApplication::BeginDraw(){
	std::stringstream ss;
	ss << _frameRate << " fps";
	SDL_WM_SetCaption(ss.str().c_str(),"ZeroEngine SampleApplication");
	SDLApplication::BeginDraw();
}
