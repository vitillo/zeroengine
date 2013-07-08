#ifndef MATRIX4_H
#define MATRIX4_H

#include <climits>
#include <ostream>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"

namespace ZeroEngine{
	class Matrix4{
	public:
		enum MatrixType{
			ZERO = 0,
			IDENTITY
		};
			
		Matrix4(MatrixType type = IDENTITY){
			switch(type){
				case IDENTITY:
					MakeIdentity();
					break;
				default:
					memset(_matrix, 0, num * sizeof(float));
					break;
			}
		}
		Matrix4(const Matrix4& matrix){
			for(int i = 0; i < num; i++)
				_matrix[i] = matrix._matrix[i];
		}
		Matrix4(const Matrix3& matrix){
			_matrix[0] = matrix[0];
			_matrix[1] = matrix[1];
			_matrix[2] = matrix[2];
			_matrix[3] = 0.0f;
			_matrix[4] = matrix[3];
			_matrix[5] = matrix[4];
			_matrix[6] = matrix[5];
			_matrix[7] = 0.0f;
			_matrix[8] = matrix[6];
			_matrix[9] = matrix[7];
			_matrix[10] = matrix[8];
			_matrix[11] = 0.0f;
			_matrix[12] = 0.0f;
			_matrix[13] = 0.0f;
			_matrix[14] = 0.0f;
			_matrix[15] = 1.0f;
		}
		Matrix4(const float buffer[]){
			for(int i = 0; i < num; i++){
				_matrix[i] = buffer[i];
			}
		}
		
		void MakeIdentity(){
			for(int i = 0; i < num; i++){
				if(i % (dim+1) == 0) _matrix[i] = 1.0f;
			else _matrix[i] = 0.0f;}
		}
		void MakeTranslationMatrix(const Vector3& translation){
			_matrix[0] = 1.0f;
			_matrix[1] = 0.0f;
			_matrix[2] = 0.0f;
			_matrix[3] = translation.X;
			_matrix[4] = 0.0f;
			_matrix[5] = 1.0f;
			_matrix[6] = 0.0f;
			_matrix[7] = translation.Y;
			_matrix[8] = 0.0f;
			_matrix[9] = 0.0f;
			_matrix[10] = 1.0f;
			_matrix[11] = translation.Z;
			_matrix[12] = 0.0f;
			_matrix[13] = 0.0f;
			_matrix[14] = 0.0f;
			_matrix[15] = 1.0f;		
		}
		void MakeViewMatrix(const Vector3& eye, const Vector3& lookAt, const Vector3& up){
			Vector3 inverseViewDirection = (eye - lookAt);
			inverseViewDirection /= inverseViewDirection.Length();
			
			Vector3 right = up.Cross(inverseViewDirection);
			right /= right.Length();
			
			Vector3 top =  inverseViewDirection.Cross(right);
			
			_matrix[0] = right.X;
			_matrix[1] = right.Y;
			_matrix[2] = right.Z;
			_matrix[3] = - right.Dot(eye);
			_matrix[4] = top.X;
			_matrix[5] = top.Y;
			_matrix[6] = top.Z;
			_matrix[7] = - top.Dot(eye);
			_matrix[8] = inverseViewDirection.X;
			_matrix[9] = inverseViewDirection.Y;
			_matrix[10] = inverseViewDirection.Z;
			_matrix[11] = - inverseViewDirection.Dot(eye);
			_matrix[12] = 0.0f;
			_matrix[13] = 0.0f;
			_matrix[14] = 0.0f;
			_matrix[15] = 1.0f;
		}
		void MakePerspectiveProjection(float fovy, float aspect, float zNear, float zFar){
			float h = 2.0f * tan(fovy / 2.0f);
			float w = aspect * h;
			float xLeft = - zNear * w / 2.0f;
			float xRight = zNear * w / 2.0f;
			float yBottom = - zNear * h / 2.0f;
			float yTop = zNear * h / 2.0f;
			
			_matrix[0] = (2 * zNear)/(xRight - xLeft);
			_matrix[1] = 0.0f;
			_matrix[2] = (xRight + xLeft)/(xRight - xLeft);
			_matrix[3] = 0.0f;
			_matrix[4] = 0.0f;
			_matrix[5] = (2.0f * zNear)/(yTop - yBottom);
			_matrix[6] = (yTop + yBottom)/(yTop - yBottom);
			_matrix[7] = 0.0f;
			_matrix[8] = 0.0f;
			_matrix[9] = 0.0f;
			_matrix[10] = -(zFar + zNear)/(zFar - zNear);
			_matrix[11] = -(2 * zNear * zFar)/(zFar - zNear);
			_matrix[12] = 0.0f;
			_matrix[13] = 0.0f;
			_matrix[14] = -1.0f;
			_matrix[15] = 0.0f;	
		}
		void MakeViewportMatrix(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax){
			_matrix[0] = (xMax - xMin) / 2.0f;
			_matrix[1] = 0.0f;
			_matrix[2] = 0.0f;
			_matrix[3] = (xMax - xMin) / 2.0f;
			_matrix[4] = 0.0f;
			_matrix[5] = (yMax - yMin) / 2.0f;
			_matrix[6] = 0.0f;
			_matrix[7] = (yMax - yMin) / 2.0f;
			_matrix[8] = 0.0f;
			_matrix[9] = 0.0f;
			_matrix[10] = (zMax - zMin) / 2.0f;
			_matrix[11] = (zMax - zMin) / 2.0f;
			_matrix[12] = 0.0f;
			_matrix[13] = 0.0f;
			_matrix[14] = 0.0f;
			_matrix[15] = 1.0f;
		}
		
		float& operator () (int row, int col){
			return _matrix[row*dim + col];
		}
		float& operator[] (int elem){
			return _matrix[elem];
		}
		float operator[] (int elem) const{
			return _matrix[elem];
		}
		
		Matrix4& operator = (const Matrix4& matrix){
			for(int i = 0; i < num; i++){
				_matrix[i] = matrix._matrix[i];
			}
			return *this;	
		}
		Matrix4& operator += (const Matrix4& matrix){
			for(int i = 0; i < num; i++){
				_matrix[i] += matrix._matrix[i];
			}
			return *this;	
		}
		Matrix4& operator -= (const Matrix4& matrix){
			for(int i = 0; i < num; i++){
				_matrix[i] -= matrix._matrix[i];
			}
			return *this;
		}
		Matrix4& operator *= (const Matrix4& matrix){
			float buffer[num] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

			for(int i = 0; i < num; i++){
				for(int j = 0; j < dim; j++){
					buffer[i] += _matrix[(i / dim) * dim + j] * matrix._matrix[i % dim + j * dim];
				}
			}
			for(int i = 0; i < num; i++){
				_matrix[i] = buffer[i];
			}
			return *this;	
		}
		
		Matrix4 operator + (const Matrix4& matrix) const{
			Matrix4 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = _matrix[i] + matrix._matrix[i];
			
			return outMatrix;
		}
		Matrix4 operator - (const Matrix4& matrix) const{
			Matrix4 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = _matrix[i] - matrix._matrix[i];
			
			return outMatrix;
		}
		Matrix4 operator * (const Matrix4& matrix) const{
			Matrix4 outMatrix(ZERO);
			for(int i = 0; i < num; i++){
				for(int j = 0; j < dim; j++){
					outMatrix[i] += _matrix[(i / dim) * dim + j] * matrix._matrix[i % dim + j * dim];
				}
			}
			
			return outMatrix;
		}
		Matrix4 operator * (float scalar) const{
			Matrix4 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = _matrix[i] * scalar;
			
			return outMatrix;
		}
		Matrix4 operator / (float scalar) const{
			Matrix4 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = _matrix[i] / scalar;
			
			return outMatrix;
		}
		Matrix4 operator - () const{
			Matrix4 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = - _matrix[i];
			
			return outMatrix;
		}
		
		Vector3 operator * (const Vector3& vector) const{
			Vector3 result;
			float w = 0;
			
			for(int i = 0; i < dim - 1; i++){
				for(int j = 0; j < dim - 1; j++){
					result[i] += _matrix[i * dim + j] * vector[j];
				}
				result[i] += _matrix[i * dim + (dim - 1)];
			}
			
			w = _matrix[12] * vector[0] + _matrix[13] * vector[1] + _matrix[14] * vector[2] + _matrix[15];
			if(w != 0)
				for(int i = 0; i < 3; i++)
					result[i] /= w;
			
			return result;
		}
		Vector4 operator * (const Vector4& vector) const{
			Vector4 result;
			
			for(int i = 0; i < dim; i++){
				for(int j = 0; j < dim; j++){
					result[i] += _matrix[i * dim + j] * vector[j];
				}
			}
			
			return result;
		}
		
		Vector3 multiplyBy(const Vector3& vector, bool isDirection = false) const{
			Vector3 result;
			float w = 0.0f;
			
			for(int i = 0; i < dim - 1; i++){
				for(int j = 0; j < dim - 1; j++){
					result[i] += _matrix[i * dim + j] * vector[j];
				}
				if(!isDirection) result[i] += _matrix[i * dim + (dim - 1)];
			}
			if(isDirection) return result;
			
			w = _matrix[12] * vector[0] + _matrix[13] * vector[1] + _matrix[14] * vector[2] + _matrix[15];
			if(w != 0.0f)
				for(int i = 0; i < 3; i++)
					result[i] /= w;
			
			return result;			
		}
		
		friend std::ostream& operator << (std::ostream& s, const Matrix4& matrix){
			for(int i = 0; i < matrix.dim; i++){
				s << "| ";
				for(int j = 0; j < matrix.dim; j++)
					s << matrix._matrix[matrix.dim * i + j] << " ";
				s << "|" << std::endl;
			}
			
			return s << std::endl;
		}
		
		private:
		float _matrix[16];
		static const int dim = 4;
		static const int num = 16;
	};	
}

#endif
