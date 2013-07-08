#include <iostream>
#include <cmath>

#include "SDLSoftwareRenderer.h"

namespace ZeroEngine{
	SDLSoftwareRenderer::SDLSoftwareRenderer(SDL_Surface* viewport, int viewportWidth, int viewportHeight, int viewportDepth) : Renderer(viewportWidth, viewportHeight, viewportDepth), _viewport(viewport){}
	
	void SDLSoftwareRenderer::DrawIndexedVertexList(const Vertex* vertexList, const int* indexList, int vertexCount){
		for(int i = 0; i < vertexCount; i++) DrawVertex(vertexList[indexList[0]]);
	}
	
	void SDLSoftwareRenderer::DrawVertex(Vertex vertex){
		vertex.Position = _viewMatrix * _userMatrix * vertex.Position;
		vertex.Normal = _viewMatrix * _userMatrix.multiplyBy(vertex.Normal, true);
		
		if(_renderMode & LIGHT_MODE && _materialSet) Shade(vertex);
		
		vertex.Position = _projectionMatrix * vertex.Position;
		
		// Perform clipping
		if(vertex.Position.W < 0 || vertex.Position.X < -vertex.Position.W || vertex.Position.X > vertex.Position.W || vertex.Position.Y < -vertex.Position.W || vertex.Position.Y > vertex.Position.W || vertex.Position.Z < -vertex.Position.W || vertex.Position.Z > vertex.Position.W) return;
		float recW = 1.0f / vertex.Position.W;
		vertex.Position.Homogeneize();
		vertex.Position = _viewportMatrix * vertex.Position;
		
		// Guarantee subpixel - subtexel accuracy
		vertex.Position.X = ceilf(vertex.Position.X);
		vertex.Position.Y = ceilf(vertex.Position.Y);
		
		vertex.Texel *= recW; // Perspective correct texture coordinates and color
		vertex.Color *= recW;
		vertex.Position.W = recW;
		
		rasterizePoint(vertex);
	}
	
	void SDLSoftwareRenderer::DrawIndexedLineList(const Vertex* vertexList, const int* indexList, int lineCount){
		for(int i = 0; i < lineCount; i++) DrawLine(vertexList[indexList[i*2]], vertexList[indexList[i*2+1]]);
	}
	
	void SDLSoftwareRenderer::DrawLine(Vertex v1, Vertex v2){
		v1.Position = _viewMatrix * _userMatrix * v1.Position;
		v1.Normal = _viewMatrix * _userMatrix.multiplyBy(v1.Normal, true);
		v2.Position = _viewMatrix * _userMatrix * v2.Position;
		v2.Normal = _viewMatrix * _userMatrix.multiplyBy(v2.Normal, true);
		
		if(_renderMode & LIGHT_MODE && _materialSet){
			Shade(v1);
			Shade(v2);
		}
		
		v1.Position = _projectionMatrix * v1.Position;
		v2.Position = _projectionMatrix * v2.Position;
		
		if(!ClipLine(v1, v2)) return;
		float recW1 = 1.0f / v1.Position.W, recW2 = 1.0f / v2.Position.W;
		v1.Position.Homogeneize();
		v2.Position.Homogeneize();		
		
		v1.Position = _viewportMatrix * v1.Position;
		v2.Position = _viewportMatrix * v2.Position;
		
		v1.Texel *= recW1;
		v2.Texel *= recW2;
		v1.Color *= recW1;
		v2.Color *= recW2;
		v1.Position.W = recW1;
		v2.Position.W = recW2;
		
		rasterizeLine(v1, v2);
	}
	
	void SDLSoftwareRenderer::DrawIndexedTriangleList(const Vertex* vertexList, const int* indexList, int triangleCount){
		for(int i = 0; i < triangleCount; i++) DrawTriangle(vertexList[indexList[i*3]], vertexList[indexList[i*3+1]], vertexList[indexList[i*3+2]]);
	}
	
	void SDLSoftwareRenderer::DrawTriangle(Vertex v1, Vertex v2, Vertex v3){
		v1.Position = _viewMatrix * _userMatrix * v1.Position;
		v1.Normal = _viewMatrix * _userMatrix.multiplyBy(v1.Normal, true);
		v2.Position = _viewMatrix * _userMatrix * v2.Position;
		v2.Normal = _viewMatrix * _userMatrix.multiplyBy(v2.Normal, true);
		v3.Position = _viewMatrix * _userMatrix * v3.Position;
		v3.Normal = _viewMatrix * _userMatrix.multiplyBy(v3.Normal, true);

		// Backface culling
		if(_renderMode & BACKFACECULLING_MODE && Vector3(v1.Position).Dot(Vector3(v2.Position - v1.Position).Cross(Vector3(v3.Position - v1.Position))) < 0) return;
	
		// Perform light shading
		if(_renderMode & LIGHT_MODE && _materialSet){
			Shade(v1);
			Shade(v2);
			Shade(v3);
		}
		
		v1.Position = _projectionMatrix * v1.Position;
		v2.Position = _projectionMatrix * v2.Position;
		v3.Position = _projectionMatrix * v3.Position;
		
		// Triangle clipping
		Vertex (*ip)[MAXCLIPVERTICES] = &_inputClipVertexArray, (*op)[MAXCLIPVERTICES] = &_outputClipVertexArray;
		int inLength = 3, outLength = 0;
		_inputClipVertexArray[0] = v1;
		_inputClipVertexArray[1] = v2;
		_inputClipVertexArray[2] = v3;
		
		if(v1.Position.W < 0 && v2.Position.W < 0 && v3.Position.W < 0) return;
		for(int i = 0x1; i != 0x80; i <<= 1){
			ClipPolygon(*ip, inLength, *op, &outLength, (ClipEdge)i);
			if((inLength = outLength) < 3) return;
			Vertex (*temp)[MAXCLIPVERTICES] = ip;
			ip = op;
			op = temp;
		}

		for(int i = 1; i < outLength - 1; i++) rasterizeTriangle((*ip)[0], (*ip)[i], (*ip)[i+1]);
	}

	void SDLSoftwareRenderer::rasterizeTriangle(Vertex v1, Vertex v2, Vertex v3){
		//Specialized scan-line algorithm
	
		// Perform perspective correct texturing
		float recW1 = 1.0f / v1.Position.W, recW2 = 1.0f / v2.Position.W, recW3 = 1.0f / v3.Position.W;
	
		v1.Position.Homogeneize();
		v2.Position.Homogeneize();
		v3.Position.Homogeneize();
	
		v1.Position = _viewportMatrix * v1.Position;
		v2.Position = _viewportMatrix * v2.Position;
		v3.Position = _viewportMatrix * v3.Position;
		
		// Perspective correct texture: the interpolation value 1/w is stored as the w coordinate in 'vertex.Position' as w is not used anymore
		v1.Texel *= recW1;
		v2.Texel *= recW2;
		v3.Texel *= recW3;
		v1.Color *= recW1;
		v2.Color *= recW2;
		v3.Color *= recW3;
		v1.Position.W = recW1;
		v2.Position.W = recW2;
		v3.Position.W = recW3;
		
		// Wireframe mode
		if(_renderMode & WIREFRAME_MODE){
			Vertex cp1(v1), cp2(v2), cp3(v3);
			rasterizeLine(v1, v2);
			rasterizeLine(cp1, v3);
			rasterizeLine(cp2, cp3);
			return;
		}
		
		// Order vertices by y value
		if(v1.Position.Y < v2.Position.Y){
			if(v3.Position.Y < v1.Position.Y){
				swap(v1, v2);
				swap(v1, v3);
			}else if(v2.Position.Y > v3.Position.Y) swap(v2, v3);
		}else{
			if(v3.Position.Y < v2.Position.Y) swap(v1, v3);
			else if(v1.Position.Y < v3.Position.Y) swap(v1, v2);
			else{
				swap(v1, v2);
				swap(v2, v3);
			}
		}
		
		float dx[3], dz[3], dw[3];
		RGBAColor dc[3];
		Vector2 dt[3];
		float diff = 0.0f;
		if(v2.Position.Y - v1.Position.Y > 0){
			diff = 1.0f / (v2.Position.Y - v1.Position.Y);
			dx[0] = (v2.Position.X - v1.Position.X) * diff;
			dz[0] = (v2.Position.Z - v1.Position.Z) * diff;
			dc[0] = (v2.Color - v1.Color) * diff;
			dt[0] = (v2.Texel - v1.Texel) * diff;
			dw[0] = (v2.Position.W - v1.Position.W) * diff;
		}
		else{
			dx[0] = v2.Position.X - v1.Position.X;
			dz[0] = v2.Position.Z - v1.Position.Z;
			dc[0] = v2.Color - v1.Color;
			dt[0] = v2.Texel - v1.Texel;
			dw[0] = v2.Position.W - v1.Position.W;
		}
		
		if(v3.Position.Y - v1.Position.Y > 0){
			diff = 1.0f / (v3.Position.Y - v1.Position.Y);
			dx[1] = (v3.Position.X - v1.Position.X) * diff;
			dz[1] = (v3.Position.Z - v1.Position.Z) * diff;
			dc[1] = (v3.Color - v1.Color) * diff;
			dt[1] = (v3.Texel - v1.Texel) * diff;
			dw[1] = (v3.Position.W - v1.Position.W) * diff;
		}
		
		if(v3.Position.Y - v2.Position.Y > 0){
			diff = 1.0f / (v3.Position.Y - v2.Position.Y);
			dx[2] = (v3.Position.X - v2.Position.X) * diff;
			dz[2] = (v3.Position.Z - v2.Position.Z) * diff;
			dc[2] = (v3.Color - v2.Color) * diff;
			dt[2] = (v3.Texel - v2.Texel) * diff;
			dw[2] = (v3.Position.W - v2.Position.W) * diff;
		}
	
		//Triangle traversal
		Vertex left = v1, right = v1;
		left.Position.Y = ceilf(left.Position.Y); // Subpixel accuracy 
		float preStep = left.Position.Y - v1.Position.Y;
				
		if(dx[0] > dx[1]){
			interpolateVertex(left, dx[1] * preStep, dz[1] * preStep, dw[1] * preStep, dc[1] * preStep, dt[1] * preStep);
			interpolateVertex(right, dx[0] * preStep, dz[0] * preStep, dw[0] * preStep, dc[0] * preStep, dt[0] * preStep);
			rasterizeHalfRectangle(left, right, v2, dx[1], dz[1], dw[1], dc[1], dt[1], dx[0], dz[0], dw[0], dc[0], dt[0]);

			right = v2;
			right.Position.Y = ceilf(right.Position.Y);
			preStep = right.Position.Y - v2.Position.Y;
			interpolateVertex(right, dx[2] * preStep, dz[2] * preStep, dw[2] * preStep, dc[2] * preStep, dt[2] * preStep);
			rasterizeHalfRectangle(left, right, v3, dx[1], dz[1], dw[1], dc[1], dt[1], dx[2], dz[2], dw[2], dc[2], dt[2]);
		}else{
			interpolateVertex(left, dx[0] * preStep, dz[0] * preStep, dw[0] * preStep, dc[0] * preStep, dt[0] * preStep);
			interpolateVertex(right, dx[1] * preStep, dz[1] * preStep, dw[1] * preStep, dc[1] * preStep, dt[1] * preStep);
			rasterizeHalfRectangle(left, right, v2, dx[0], dz[0], dw[0], dc[0], dt[0], dx[1], dz[1], dw[1], dc[1], dt[1]);
		
			left = v2;
			left.Position.Y = ceilf(left.Position.Y);
			preStep = left.Position.Y - v2.Position.Y;
			interpolateVertex(left, dx[2] * preStep, dz[2] * preStep, dw[2] * preStep, dc[2] * preStep, dt[2] * preStep);
			rasterizeHalfRectangle(left, right, v3, dx[2], dz[2], dw[2], dc[2], dt[2], dx[1], dz[1], dw[1], dc[1], dt[1]);
		}
	}
	
	inline void SDLSoftwareRenderer::rasterizeHalfRectangle(Vertex& left, Vertex& right, const Vertex& end, float leftDx, float leftDz, float leftDw, const RGBAColor& leftDColor, const Vector2& leftDTex, float rightDx, float rightDz, float rightDw, const RGBAColor& rightDColor, const Vector2& rightDTex){
		float yMax = ceilf(end.Position.Y);
		for(; left.Position.Y < yMax; left.Position.Y++){ // Draw triangle fragment
			rasterizeHorizontalLine(left, right);
			interpolateVertex(left, leftDx, leftDz, leftDw, leftDColor, leftDTex);
			interpolateVertex(right, rightDx, rightDz, rightDw, rightDColor, rightDTex);
		}		
	}
	
	inline void SDLSoftwareRenderer::interpolateVertex(Vertex& vertex, float dx, float dz, float dw, const RGBAColor& dColor, const Vector2& dTexel){
		vertex.Position.X += dx;
		vertex.Position.Z += dz;
		vertex.Color += dColor;
		vertex.Texel += dTexel;
		vertex.Position.W += dw;
	}
	
	void SDLSoftwareRenderer::rasterizeHorizontalLine(const Vertex& v1, const Vertex& v2){	
		int xStart = int(ceilf(v1.Position.X));
		int xEnd = int(ceilf(v2.Position.X));
		float preStep = (float)xStart - v1.Position.X;
		float diff = 1.0f / (v2.Position.X - v1.Position.X);
		
		RGBAColor dColor = (v2.Color - v1.Color) * diff;
		Vector2 dTexel = (v2.Texel - v1.Texel) * diff;
		float dz = (v2.Position.Z - v1.Position.Z) * diff;
		float dw = (v2.Position.W - v1.Position.W) * diff;
		
		Vertex drawVertex(xStart, v1.Position.Y, v1.Position.Z + dz * preStep, v1.Color + dColor * preStep, v1.Texel + dTexel * preStep);
		drawVertex.Position.W = v1.Position.W + dw * preStep;
		while(drawVertex.Position.X < xEnd){
			rasterizePoint(drawVertex);
			interpolateVertex(drawVertex, 1, dz, dw, dColor, dTexel);
		}
	}	
	
	void SDLSoftwareRenderer::swap(Vertex& point1, Vertex& point2){
		Vertex temp = point1;
		point1 = point2;
		point2 = temp;
	}
	
	void SDLSoftwareRenderer::swap(float& x, float& y){
		float t = x;
		x = y;
		y = t;
	}
	
	void SDLSoftwareRenderer::rasterizePoint(const Vertex& vertex){
		int y = (_viewport->h - 1) - int(vertex.Position.Y); // flip y axis
		int x = int(vertex.Position.X);
		
		if(_renderMode & ALPHA_MODE && (x + y) % 2 != 0) return;
		
		if(_zBuffer[_viewportWidth * y + x] < vertex.Position.Z) return;
		_zBuffer[_viewportWidth * y + x] = vertex.Position.Z;
		
		Uint8 *pixmem = (Uint8*) _viewport->pixels + y * _viewport->pitch + x * _viewport->format->BytesPerPixel;
		float w = 1.0f / vertex.Position.W;
		if(!_materialSet || !_material.Texture.isLoaded()){
			switch(_viewport->format->BytesPerPixel){
				case 4:
				case 3:
					pixmem[2] = char(w * vertex.Color.Red * 255);
				case 2:
					pixmem[1] = char(w * vertex.Color.Green * 255);
				case 1:
					pixmem[0] = char(w * vertex.Color.Blue * 255);
			}			
		}else{
			int u = int(w * vertex.Texel.X) & _material.Texture.Width - 1;
			int v = int(w * vertex.Texel.Y) & _material.Texture.Height - 1;
			
            SDL_Color color;
            _material.Texture.LoadPixel(u, v, color);
            
			switch(_viewport->format->BytesPerPixel){
				case 4:
				case 3:
					pixmem[2] = char(w * vertex.Color.Red * color.r);
				case 2:
					pixmem[1] = char(w * vertex.Color.Green * color.g);
				case 1:
					pixmem[0] = char(w * vertex.Color.Blue * color.b);
			}
		}
	}
	
	void SDLSoftwareRenderer::rasterizeLine(Vertex& v0, Vertex& v1){
		bool steep = std::fabs(v1.Position.X - v0.Position.X) > std::fabs(v1.Position.Y - v0.Position.Y);
		if(steep){
			swap(v0.Position.X, v0.Position.Y);
			swap(v1.Position.X, v1.Position.Y);
		}
		
		if(v0.Position.Y > v1.Position.Y) swap(v0, v1);
		
		float dx = 0.0f, dz = 0.0f, dw = 0.0f, yDiff = v1.Position.Y - v0.Position.Y;
		RGBAColor dColor(0.0f, 0.0f, 0.0f);
		Vector2 dTexel;
		if(yDiff != 0){
			yDiff = 1.0f / yDiff;
			dx = (v1.Position.X - v0.Position.X) * yDiff;
			dz = (v1.Position.Z - v0.Position.Z) * yDiff;
			dw = (v1.Position.W - v0.Position.W) * yDiff;
			dColor = (v1.Color - v0.Color) * yDiff;
			dTexel = (v1.Texel - v0.Texel) * yDiff;
		}

		Vertex drawVertex(v0);
		// Guarantee subpixel accuracy
		drawVertex.Position.Y = ceilf(drawVertex.Position.Y);
		v1.Position.Y = ceilf(v1.Position.Y);
		float preStep = drawVertex.Position.Y - v0.Position.Y;
		interpolateVertex(drawVertex, dx * preStep, dz * preStep, dw * preStep, dColor * preStep, dTexel * preStep);
		
		if(!steep){
			for(; drawVertex.Position.Y < v1.Position.Y; drawVertex.Position.Y++){
				rasterizePoint(drawVertex);
				interpolateVertex(drawVertex, dx, dz, dw, dColor, dTexel);
			}
		}else{
			swap(drawVertex.Position.X, drawVertex.Position.Y);
			for(; drawVertex.Position.X < v1.Position.Y; drawVertex.Position.X++){
				rasterizePoint(drawVertex);
				drawVertex.Position.Y += dx;
				drawVertex.Position.Z += dz;
				drawVertex.Position.W += dw;
				drawVertex.Color += dColor;
				drawVertex.Texel += dTexel;
			}
		}
	}
}
