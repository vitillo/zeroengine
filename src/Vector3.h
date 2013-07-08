#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <ostream>

#include "Vector4.h"

namespace ZeroEngine{
	class Vector3{
	public:
		float X, Y, Z;
		
		Vector3() : X(0.0f), Y(0.0f), Z(0.0f){};
		Vector3(float x, float y, float z) : X(x), Y(y), Z(z){};
		Vector3(float d) : X(d), Y(d), Z(d){};
		Vector3(const Vector3& vector) : X(vector.X), Y(vector.Y), Z(vector.Z){};
		Vector3(const Vector4& vector) : X(vector.X), Y(vector.Y), Z(vector.Z){};
		
		void Set(float x, float y, float z){ X = x; Y = y; Z = z; }
		float& operator [] (int i){
			if(!i) return X;
			else if(i == 1) return Y;
			else return Z;		
		}
		float operator [] (int i) const{
			if(!i) return X;
			else if(i == 1) return Y;
			else return Z;	
		}
		
		Vector3& operator = (const Vector3& vector){
			X = vector.X;
			Y = vector.Y;
			Z = vector.Z;
			return *this;	
		}
		Vector3& operator += (const Vector3& vector){
			X += vector.X;
			Y += vector.Y;
			Z += vector.Z;
			return *this;		
		}
		Vector3& operator -= (const Vector3& vector){
			X -= vector.X;
			Y -= vector.Y;
			Z -= vector.Z;
			return *this;
			
		}
		Vector3& operator *= (float d){
			X *= d;
			Y *= d;
			Z *= d;
			return *this;		
		}
		Vector3& operator /= (float d){
			X /= d;
			Y /= d;
			Z /= d;
			return *this;	
		}
		
		Vector3 operator + (const Vector3& vector) const{
			return Vector3(X + vector.X, Y + vector.Y, Z + vector.Z);
		}
		Vector3 operator - (const Vector3& vector) const{
			return Vector3(X - vector.X, Y - vector.Y, Z - vector.Z);
		}
		Vector3 operator * (float scalar) const{
			return Vector3(X * scalar, Y * scalar, Z * scalar);
		}
		Vector3 operator / (float scalar) const{
			return Vector3(X / scalar, Y / scalar, Z / scalar);
		}
		Vector3 operator - () const{
			return Vector3(- X, - Y, - Z);
		}
		
		float Length() const{
			return sqrt(X * X + Y * Y + Z * Z);
		}
		Vector3& Normalize(){
			*this /= Length();
			return *this;
		}
		float Dot(const Vector3& vector) const{
			return X * vector.X + Y * vector.Y + Z * vector.Z;
		}
		Vector3 Cross(const Vector3& vector) const{
			return Vector3(Y * vector.Z - Z * vector.Y, Z * vector.X - X * vector.Z, X * vector.Y - Y * vector.X);
		}
		
		friend std::ostream& operator << (std::ostream& s, const Vector3& v){
			return s << "| " << v.X << " " << v.Y << " " << v.Z << " |" << std::endl;
		}
	};
}

#endif
