#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

namespace ZeroEngine{
	struct RGBAColor{
	public:
		float Red;
		float Green;
		float Blue;
		
		RGBAColor(float red = 1.0f, float green = 1.0f, float blue = 1.0f) : Red(red), Green(green), Blue(blue){}
		
		RGBAColor& operator += (const RGBAColor& color){
			Red += color.Red;
			Green += color.Green;
			Blue += color.Blue;
			return *this;
		}
		RGBAColor& operator *= (const RGBAColor& color){
			Red *= color.Red;
			Green *= color.Green;
			Blue *= color.Blue;
			return *this;
		}
		RGBAColor& operator *= (float scalar){
			Red *= scalar;
			Green *= scalar;
			Blue *= scalar;
			return *this;
		}
		
		RGBAColor operator - (const RGBAColor& color) const{
			return RGBAColor(Red - color.Red, Green - color.Green, Blue - color.Blue);
		}
		RGBAColor operator + (const RGBAColor& color) const{
			return RGBAColor(Red + color.Red, Green + color.Green, Blue + color.Blue);
		}
		RGBAColor operator * (const RGBAColor& color) const{
			return RGBAColor(Red * color.Red, Green * color.Green, Blue * color.Blue);
		}
		RGBAColor operator / (float scalar) const{
			return RGBAColor(Red / scalar, Green / scalar, Blue / scalar);
		}
		RGBAColor operator * (float scalar) const{
			return RGBAColor(Red * scalar, Green * scalar, Blue * scalar);
		}
	};
	
	class Vertex{
	public:
		Vector4 Position;
		Vector3 Normal;
		Vector2 Texel;
		RGBAColor Color;
		
		Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f) : Position(x, y, z, 1.0f), Normal(0.0f, 0.0f, 0.0f), Texel(), Color(r, g, b){}
		Vertex(float x, float y, float z, const RGBAColor& color, const Vector2 texel) : Position(x, y, z, 1.0f), Texel(texel), Normal(0.0f, 0.0f, 0.0f), Color(color) {};
		Vertex(float x, float y, float z, const RGBAColor& color) : Position(x, y, z, 1.0f), Normal(0.0f, 0.0f, 0.0f), Texel(), Color(color){}
		
		void Set(const Vector3& position, const Vector3& normal = Vector3(), const RGBAColor& color = RGBAColor(), const Vector2 texel = Vector2()){
			Position.Set(position.X, position.Y, position.Z);
			Color = color;
			Normal = normal;
			Texel = texel;
		}
		
		Vertex operator - (const Vertex& vertex) const{
			Vertex result;
			result.Position = this->Position - vertex.Position;
			result.Texel = this->Texel - vertex.Texel;
			result.Color = this->Color - vertex.Color;
			return result;
		}
		Vertex operator + (const Vertex& vertex) const{
			Vertex result;
			result.Position = this->Position + vertex.Position;
			result.Texel = this->Texel + vertex.Texel;
			result.Color = this->Color + vertex.Color;
			return result;
		}
		Vertex operator * (float scalar) const{
			Vertex result;
			result.Position = this->Position * scalar;
			result.Texel = this->Texel * scalar;
			result.Color = this->Color * scalar;
			return result;
		}
	};
}

#endif
