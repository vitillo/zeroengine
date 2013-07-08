#ifndef SAMPLEAPPLICATION_H
#define SAMPLEAPPLICATION_H

#include "MD2Model.h"
#include "SDLApplication.h"

using namespace ZeroEngine;

class SampleApplication : public SDLApplication{
public:
	SampleApplication(int w, int h, int d, bool f = false);
	
	void Initialize();
	void Update();
	void Draw();
	
protected:
	void BeginDraw();
	bool HandleSDLEvent(SDL_Event);
	void HandleRepeatingKeys();
	
private:
	bool _directionChanged;
	bool _cubeAlpha;
	float _cubeAngle;
	int _cubeIndex[36];
	
	Material _floorMaterial;
	
	MD2Model _marine;
	MD2Model _hunter;
	MD2Model _cube;
	
	Matrix4 _cameraMatrix;
	Vector3 _cameraAngle;
	Vector3 _cameraTrans;

	Vertex _cubeVBuffer[8];
	Vertex _floorVBuffer[4];
	
	void initializeCubeVertices();
	void initializeFloorVertices(const Vector3& center, int n, float length);
	void drawGrid(float x, float y, float z, float length, float distance, const RGBAColor& color);
	void drawFloor();
	void drawCube();
	void updateCameraMatrix();
	Vector3 getMoveDirection();
	Matrix4 generateCubeMatrix(Vector3 position);
};

#endif
