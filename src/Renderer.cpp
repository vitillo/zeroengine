#include <cfloat>

#include "Renderer.h"

namespace ZeroEngine{
	Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportDepth) : _viewportWidth(viewportWidth), _viewportHeight(viewportHeight), _viewportDepth(viewportDepth), _renderMode(0), _materialSet(false), _zBuffer(NULL), _viewMatrix(), _projectionMatrix(), _viewportMatrix(), _userMatrix(), _lights(), _matrixStack(), _material(){
		_zBuffer = new float[_viewportWidth * _viewportHeight];
		setViewportMatrix();
	}
	
	int Renderer::compOutCodeW(const Vector4& point){
		int code = 0;
		if(point.Y > point.W) code |= TOP;
		if(point.Y < -point.W) code |= BOTTOM;
		if(point.X > point.W) code |= RIGHT;
		if(point.X < -point.W) code |= LEFT;
		if(point.Z < -point.W) code |= FRONT;
		if(point.Z > point.W) code |= BEHIND;
		if(point.W < 0) code |= NOTVISIBLE;
		return code;
	}
	
	bool Renderer::ClipLine(Vertex& v0, Vertex& v1){
		//Cohen-Sutherland algorithm
		if(v0.Position.W < 0 && v1.Position.W < 0) return false; //Trivial rejection test, line is behind the viewer
		
		int outcode0 = compOutCodeW(v0.Position), outcode1 = compOutCodeW(v1.Position), outcodeOut;
		while(true){
			if(!(outcode0 | outcode1)) return true;
			else if(outcode0 & outcode1) return false;
			else{
				float term, w;
				Vertex* point;
				if(outcode0){
					outcodeOut = outcode0;
					point = &v0;
				}else{
					outcodeOut = outcode1;
					point = &v1;
				}
				
				if(outcodeOut & NOTVISIBLE){
					term = (0.1f - v0.Position.W) / (v1.Position.W - v0.Position.W);
					w = v0.Position.W + (v1.Position.W - v0.Position.W) * term;
					*point = v0 + (v1 - v0) * term;
				}else if(outcodeOut & TOP){
					term = (v0.Position.W - v0.Position.Y) / ((v0.Position.W - v0.Position.Y) - (v1.Position.W - v1.Position.Y));
					w = v0.Position.W + (v1.Position.W - v0.Position.W) * term;
					*point = v0 + (v1 - v0) * term;					
					point->Position.Y = w; // Needed for adjusting the floating point error; without the loop it could become infinite
				}else if(outcodeOut & BOTTOM){
					term = (v0.Position.W + v0.Position.Y) / ((v0.Position.W + v0.Position.Y) - (v1.Position.W + v1.Position.Y));
					w = v0.Position.W + (v1.Position.W - v0.Position.W) * term;
					*point = v0 + (v1 - v0) * term;
					point->Position.Y = -w;
				}else if(outcodeOut & RIGHT){
					term = (v0.Position.W - v0.Position.X) / ((v0.Position.W - v0.Position.X) - (v1.Position.W - v1.Position.X));
					w = v0.Position.W + (v1.Position.W - v0.Position.W) * term;
					*point = v0 + (v1 - v0) * term;
					point->Position.X = w;
				}else if(outcodeOut & LEFT){
					term = (v0.Position.W + v0.Position.X) / ((v0.Position.W + v0.Position.X) - (v1.Position.W + v1.Position.X));
					w = v0.Position.W + (v1.Position.W - v0.Position.W) * term;
					*point = v0 + (v1 - v0) * term;
					point->Position.X = -w;
				}else if(outcodeOut & FRONT){
					term = (v0.Position.W + v0.Position.Z) / ((v0.Position.W + v0.Position.Z) - (v1.Position.W + v1.Position.Z));
					w = v0.Position.W + (v1.Position.W - v0.Position.W) * term;
					*point = v0 + (v1 - v0) * term;
					point->Position.Z = -w;
				}else{
					term = (v0.Position.W - v0.Position.Z) / ((v0.Position.W - v0.Position.Z) - (v1.Position.W - v1.Position.Z));
					w = v0.Position.W + (v1.Position.W - v0.Position.W) * term;
					*point = v0 + (v1 - v0) * term;
					point->Position.Z = w;
				}
				
				if(outcodeOut == outcode0) outcode0 = compOutCodeW(v0.Position);
				else outcode1 = compOutCodeW(v1.Position);
			}
		}
	}
		
	void Renderer::SetViewMatrix(const Vector3& eye, const Vector3& lookAt, const Vector3& up){
		_viewMatrix.MakeViewMatrix(eye, lookAt, up);
	}
	
	void Renderer::SetPerspectiveProjection(float fovy, float aspect, float zNear, float zFar){
		_projectionMatrix.MakePerspectiveProjection(fovy, aspect, zNear, zFar);
	}
	
	void Renderer::setViewportMatrix(){
		// I'm assuming the canonical view volume is used during projection
		_viewportMatrix.MakeViewportMatrix(0.0f, _viewportWidth - 1.0f, 0.0f, _viewportHeight - 1.0f, -1.0f, 1.0f);
	}
	
	void Renderer::PushMatrix(const Matrix4& matrix){
		_matrixStack.push_back(matrix);
		_userMatrix *= matrix;
	}
	
	void Renderer::PushMatrix(const Matrix3& matrix){
		Matrix4 pushM = matrix;
		_matrixStack.push_back(pushM);
		_userMatrix *= pushM;
	}
	
	void Renderer::PopMatrix(){
		_matrixStack.pop_back();
		_userMatrix.MakeIdentity();
		
		// Matrix inversion not implemented yet
		for(std::vector<Matrix4>::reverse_iterator it = _matrixStack.rbegin(); it != _matrixStack.rend(); it++){
			_userMatrix *= *it;
		}
	}
	
	void Renderer::ClearZBuffer(){
		for(int i = 0; i < _viewportHeight * _viewportWidth; i ++) _zBuffer[i] = 2.0f;
	}
	
	unsigned Renderer::GetMode(){
		return _renderMode;
	}
	
	void Renderer::SetMode(RenderMode mode){
		_renderMode |= mode;
	}
	
	void Renderer::UnsetMode(RenderMode mode){
		_renderMode &= ~mode;
	}
	
	void Renderer::ClipPolygon(Vertex* inVertexArray, int inLength, Vertex* outVertexArray, int* outLength, ClipEdge edge){
		//Sutherland-Hodgman algorithm
		Vertex* s = &inVertexArray[inLength - 1];
		*outLength = 0;
		for(int j = 0; j < inLength; j++){
			Vertex& p = inVertexArray[j];
			if(inside(p, edge)){
				if(inside(*s, edge)){
					output(p, outLength, outVertexArray);
				}
				else{
					if(*outLength >= MAXCLIPVERTICES - 2) return;
					Vertex i = intersect(*s, p, edge);
					output(i, outLength, outVertexArray);
					output(p, outLength, outVertexArray);
				}
			}else{
				if(inside(*s, edge)){
					if(*outLength >= MAXCLIPVERTICES - 1) return;
					Vertex i = intersect(*s, p, edge);
					output(i, outLength, outVertexArray);
				}
			}
			s = &p;
		}
	}
	
	inline void Renderer::output(const Vertex& vertex, int *outLength, Vertex* outVertexArray){
		outVertexArray[(*outLength)++] = vertex;
	}
	
	bool Renderer::inside(const Vertex& vertex, ClipEdge edge){
		switch(edge){
		case TOP:
			if(vertex.Position.Y > vertex.Position.W) return false;
			break;
		case BOTTOM:
			if(vertex.Position.Y < -vertex.Position.W) return false;
			break;
		case RIGHT:
			if(vertex.Position.X > vertex.Position.W) return false;
			break;
		case LEFT:
			if(vertex.Position.X < -vertex.Position.W) return false;
			break;
		case FRONT:
			if(vertex.Position.Z < -vertex.Position.W) return false;
			break;
		case BEHIND:
			if(vertex.Position.Z > vertex.Position.W) return false;
			break;
		case NOTVISIBLE:
			if(vertex.Position.W < 0) return false; // Vertex behind the viewer
		default:
			break;
		}
		return true;
	}
	
	Vertex Renderer::intersect(const Vertex& v1, const Vertex& v2, ClipEdge edge){
		float ratio = 0.0f;
		switch(edge){
		case TOP:
			ratio = (v1.Position.W - v1.Position.Y) / ((v1.Position.W - v1.Position.Y) - (v2.Position.W - v2.Position.Y));
			break;
		case BOTTOM:
			ratio = (v1.Position.W + v1.Position.Y) / ((v1.Position.W + v1.Position.Y) - (v2.Position.W + v2.Position.Y));
			break;
		case RIGHT:
			ratio = (v1.Position.W - v1.Position.X)/((v1.Position.W - v1.Position.X) - (v2.Position.W - v2.Position.X));
			break;
		case LEFT:
			ratio = (v1.Position.W + v1.Position.X)/((v1.Position.W + v1.Position.X) - (v2.Position.W + v2.Position.X));
			break;
		case FRONT:
			ratio = (v1.Position.W + v1.Position.Z)/((v1.Position.W + v1.Position.Z) - (v2.Position.W + v2.Position.Z));
			break;
		case BEHIND:
			ratio = (v1.Position.W - v1.Position.Z)/((v1.Position.W - v1.Position.Z) - (v2.Position.W - v2.Position.Z));
			break;
		case NOTVISIBLE:
			ratio = (0.1 - v1.Position.W) / (v2.Position.W - v1.Position.W);
			break;
		default:
			break;
		}
		return v1 + (v2 - v1) * ratio;
	}
	
	void Renderer::AddLight(const Vector3& light){
		_lights.push_back((_viewMatrix * _userMatrix.multiplyBy(light, true)).Normalize());
	}
	
	void Renderer::AddLights(const std::vector<Vector3>& lights){
		for(std::vector<Vector3>::const_iterator it = lights.begin(); it != lights.end(); it++){
			_lights.push_back((_viewMatrix * _userMatrix.multiplyBy(*it, true)).Normalize());
		}
	}
	
	void Renderer::RemoveLights(){
		_lights.clear();
	}
	
	void Renderer::Shade(Vertex& vertex){
		vertex.Normal.Normalize();
		Vector3 v = (Vector3(0.0f, 0.0f, 0.0f) - Vector3(vertex.Position)).Normalize();
		RGBAColor lo(0.0f, 0.0f, 0.0f);
		
		for(int i = 0; i < _lights.size(); i++){
			Vector3 h = (v + _lights[i]).Normalize();
			float cosTh = std::max(0.0f, vertex.Normal.Dot(h));
			float cosTi = std::max(0.0f, vertex.Normal.Dot(_lights[i]));
			lo += (_material.Diffuse + _material.Specular * pow(cosTh, 1)) * RGBAColor(1.0f, 1.0f, 1.0f) * cosTi;
		}
		vertex.Color *= lo;
	}
	
	void Renderer::SetMaterial(const Material& material){
		_material = material;
		_materialSet = true;
	}
	
	void Renderer::UnsetMaterial(){
		_materialSet = false;
	}
}
