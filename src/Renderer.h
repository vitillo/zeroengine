#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Vertex.h"
#include "Matrix4.h"
#include "Material.h"

#define MAXCLIPVERTICES 20

namespace ZeroEngine {
	typedef enum{
		WIREFRAME_MODE = 0x01,
		BACKFACECULLING_MODE = 0x02,
		ALPHA_MODE = 0x04,
		LIGHT_MODE = 0x08
	} RenderMode;
	
	enum ClipEdge{
		NOTVISIBLE = 0x1,
		TOP = 0x2,
		BOTTOM = 0x4,
		RIGHT = 0x8,
		LEFT = 0x10,
		FRONT = 0x20,
		BEHIND = 0x40
	};

	class Renderer{
	public:
		Renderer(int viewportWidth, int viewportHeight, int viewportDepth);
        virtual ~Renderer(){}
		
		virtual void DrawVertex(Vertex vertex) = 0;
		virtual void DrawIndexedVertexList(const Vertex* vertexList, const int* indexList, int vertexCount) = 0;
		virtual void DrawLine(Vertex start, Vertex end) = 0;
		virtual void DrawIndexedLineList(const Vertex* vertexList, const int* indexList, int lineCount) = 0;
		virtual void DrawTriangle(Vertex v1, Vertex v2, Vertex v3) = 0;
		virtual void DrawIndexedTriangleList(const Vertex* vertexList, const int* indexList, int triangleCount) = 0;
		
		void SetViewMatrix(const Vector3& eye, const Vector3& lookAt, const Vector3& up);
		void SetPerspectiveProjection(float fovy, float aspect, float zNear, float zFar);

		void SetMaterial(const Material& material);
		void UnsetMaterial();
		
		unsigned GetMode();
		void SetMode(RenderMode mode);
		void UnsetMode(RenderMode mode);
		
		void AddLight(const Vector3& light);
		void AddLights(const std::vector<Vector3>& lights);
		void RemoveLights();
		
		void PushMatrix(const Matrix4& matrix);
		void PushMatrix(const Matrix3& matrix);
		void PopMatrix();
		void ClearZBuffer();
		
		
	protected:
		bool _materialSet;
		unsigned _renderMode;
		int _viewportWidth;
		int _viewportHeight;
		int _viewportDepth;	
		float* _zBuffer;
		
		Matrix4 _viewMatrix;
		Matrix4 _projectionMatrix;
		Matrix4 _viewportMatrix;
		Matrix4 _userMatrix;
		
		std::vector<Vector3> _lights;
		std::vector<Matrix4> _matrixStack;
		Material _material;
		
		Vertex _inputClipVertexArray[MAXCLIPVERTICES], _outputClipVertexArray[MAXCLIPVERTICES];
		
		void Shade(Vertex& vertex);
		bool ClipLine(Vertex& v0, Vertex& v1);
		void ClipPolygon(Vertex* inVertexArray, int inLength, Vertex* outVertexArray, int* outLength, ClipEdge edge);
		
		
	private:
		int compOutCodeW(const Vector4& point);
		bool inside(const Vertex& vertex, ClipEdge edge);
		void output(const Vertex& vertex, int *outLength, Vertex* outVertexArray);
		Vertex intersect(const Vertex& v1, const Vertex& v2, ClipEdge edge);
		void setViewportMatrix();
	};
}

#endif
