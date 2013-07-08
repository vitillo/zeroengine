#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include "SDL.h"

#include "Renderer.h"

namespace ZeroEngine{
	class SDLSoftwareRenderer : public Renderer{
		public:
		SDLSoftwareRenderer(SDL_Surface* viewport, int viewportWidth, int viewportHeight, int viewportDepth);
		
		void DrawVertex(Vertex vertex);
		void DrawIndexedVertexList(const Vertex* vertexList, const int* indexList, int vertexCount);
		
		void DrawLine(Vertex start, Vertex end);
		void DrawIndexedLineList(const Vertex* vertexList, const int* indexList, int lineCount);
		
		void DrawTriangle(Vertex v1, Vertex v2, Vertex v3);
		void DrawIndexedTriangleList(const Vertex* vertexList, const int* indexList, int triangleCount); 


		private:
		SDL_Surface* _viewport;
		
		void swap(Vertex& point1, Vertex& point2);
		void swap(float& x, float& y);
		void interpolateVertex(Vertex& vertex, float dx, float dz, float dw, const RGBAColor& dColor, const Vector2& dTexel);
		
		void rasterizePoint(const Vertex& vertex);
		void rasterizeLine(Vertex& v0, Vertex& v1);
		void rasterizeHorizontalLine(const Vertex& v1, const Vertex& v2);
		
		void rasterizeTriangle(Vertex v1, Vertex v2, Vertex v3);
		void rasterizeHalfRectangle(Vertex& left, Vertex& right, const Vertex& end, float leftDx, float leftDz, float leftDw, const RGBAColor& leftDColor, const Vector2& leftDTex, float rightDx, float rightDz, float rightDw, const RGBAColor& rightDColor, const Vector2& rightDTex);
	};
}

#endif
