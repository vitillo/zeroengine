#ifndef VECTOR4_H
#define VECTOR4_H

#include <cmath>
#include <ostream>

namespace ZeroEngine{	
	class Vector4{
	public:
		float X, Y, Z, W;
		
		Vector4() : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f){};
		Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w){};
		Vector4(float d) : X(d), Y(d), Z(d), W(d){};
		Vector4(const Vector4& vector) : X(vector.X), Y(vector.Y), Z(vector.Z), W(vector.W){};
		
		void Set(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f){ X = x; Y = y; Z = z; W = w; }
		float& operator [] (int i){
			if(!i) return X;
			else if(i == 1) return Y;
			else if(i == 2) return Z;
			else return W;		
		}
		float operator [] (int i) const{
			if(!i) return X;
			else if(i == 1) return Y;
			else if(i == 2) return Z;
			else return W;		
		}
		
		Vector4& operator = (const Vector4& vector){
			X = vector.X;
			Y = vector.Y;
			Z = vector.Z;
			W = vector.W;
			return *this;	
		}
		Vector4& operator += (const Vector4& vector){
			X += vector.X;
			Y += vector.Y;
			Z += vector.Z;
			W += vector.W;
			return *this;
		}		
		Vector4& operator *= (float scalar){
			X = scalar * X;
			Y = scalar * Y;
			Z = scalar * Z;
			W = scalar * W;
			return *this;
		}
		
		Vector4 operator + (const Vector4& vector) const{
			return Vector4(X + vector.X, Y + vector.Y, Z + vector.Z, W + vector.W);
		}
		Vector4 operator - (const Vector4& vector) const{
			return Vector4(X - vector.X, Y - vector.Y, Z - vector.Z, W - vector.W);
		}
		Vector4 operator * (float scalar) const{
			return Vector4(X * scalar, Y * scalar, Z * scalar, W * scalar);
		}
		Vector4 operator / (float scalar) const{
			return Vector4(X / scalar, Y / scalar, Z / scalar, W / scalar);
		}
		
		void Homogeneize(){
			if(W != 0.0f){
				X /= W;
				Y /= W;
				Z /= W;
				W /= W;
			}
		}
		
		friend std::ostream& operator << (std::ostream& s, const Vector4& v){
			return s << "| " << v.X << " " << v.Y << " " << v.Z << " " << v.W << " |" << std::endl;
		}
	};
}

#endif
