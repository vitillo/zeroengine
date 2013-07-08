#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <ostream>

namespace ZeroEngine{
	class Vector2{
	public:
		float X, Y;
		
		Vector2() : X(0.0f), Y(0.0f){};
		Vector2(float x, float y) : X(x), Y(y){};
		Vector2(float d) : X(d), Y(d){};
		Vector2(const Vector2& vector) : X(vector.X), Y(vector.Y){};
		
		void Set(float x, float y){ X = x; Y = y; }
		float& operator [] (int i){
			if(i) return Y;
			else return X;	
		}
		float operator [] (int i) const{
			if(i) return Y;
			else return X;		
		}
		
		Vector2& operator = (const Vector2& vector){
			X = vector.X;
			Y = vector.Y;
			return *this;	
		}
		Vector2& operator += (const Vector2& vector){
			X += vector.X;
			Y += vector.Y;
			return *this;
			
		}
		Vector2& operator -= (const Vector2& vector){
			X -= vector.X;
			Y -= vector.Y;
			return *this;	
		}
		Vector2& operator *= (float d){
			X *= d;
			Y *= d;
			return *this;	
		}
		Vector2& operator /= (float d){
			X /= d;
			Y /= d;
			return *this;	
		}
		
		Vector2 operator + (const Vector2& vector) const{
			return Vector2(X + vector.X, Y + vector.Y);
		}
		Vector2 operator - (const Vector2& vector) const{
			return Vector2(X - vector.X, Y - vector.Y);
		}
		Vector2 operator * (float scalar) const{
			return Vector2(X * scalar, Y * scalar);
		}
		Vector2 operator / (float scalar) const{
			return Vector2(X / scalar, Y / scalar);
		}
		
		float Length() const{
			return std::sqrt(X * X + Y * Y);
		}
		Vector2& Normalize(){
			*this /= Length();
			return *this;	
		}
		float Dot(const Vector2& vector) const{
			return X * vector.X + Y * vector.Y;
		}
		
		friend std::ostream& operator << (std::ostream& s, const Vector2& v){
			return s << "| " << v.X << " " << v.Y << " |" << std::endl;
		}
	};
}

#endif
