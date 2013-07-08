#ifndef MATRIX3_H
#define MATRIX3_H

#include <climits>
#include <ostream>

#include "Vector4.h"
#include "Vector3.h"

namespace ZeroEngine{
	class Matrix3{
	public:
		enum MatrixType{
			ZERO = 0,
			IDENTITY
		};
	
		Matrix3(MatrixType type = IDENTITY){
			switch(type){
				case IDENTITY:
					MakeIdentity();
					break;
				default:
					memset(_matrix, 0, num * sizeof(float));
					break;
		
			}
		}
		Matrix3(const Vector3& cv1, const Vector3& cv2, const Vector3& cv3){
			_matrix[0] = cv1.X;
			_matrix[3] = cv1.Y;
			_matrix[6] = cv1.Z;
			_matrix[1] = cv2.X;
			_matrix[4] = cv2.Y;
			_matrix[7] = cv2.Z;
			_matrix[2] = cv3.X;
			_matrix[5] = cv3.Y;
			_matrix[8] = cv3.Z;				
		}
		Matrix3(const Matrix3& matrix){
			for(int i = 0; i < num; i++)
				_matrix[i] = matrix._matrix[i];		
		}
		Matrix3(const float buffer[]){
			for(int i = 0; i < num; i++){
				_matrix[i] = buffer[i];
			}		
		}
		
		void MakeIdentity(){
			for(int i = 0; i < num; i++)
				if(i % (dim+1) == 0) _matrix[i] = 1.0f;
				else _matrix[i] = 0.0f;
		}
		void MakeEulerMatrix(float h, float p, float r){
			_matrix[0] = cos(r) * cos(h) - sin(r) * sin(p) * sin(h);
			_matrix[1] = -sin(r) * cos(p);
			_matrix[2] = cos(r) * sin(h) + sin(r) * sin(p) * cos(h);
			_matrix[3] = sin(r) * cos(h) + cos(r) * sin(p) * sin(h);
			_matrix[4] = cos(r) * cos(p);
			_matrix[5] = sin(r) * sin(h) - cos(r) * sin(p) * cos(h);
			_matrix[6] = -cos(p) * sin(h);
			_matrix[7] = sin(p);
			_matrix[8] = cos(p) * cos(h);		
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
		
		Matrix3& operator = (const Matrix3& matrix){
			for(int i = 0; i < num; i++){
				_matrix[i] = matrix._matrix[i];
			}
			return *this;
		}
		Matrix3& operator += (const Matrix3& matrix){
			for(int i = 0; i < num; i++){
				_matrix[i] += matrix._matrix[i];
			}
			return *this;		
		}
		Matrix3& operator -= (const Matrix3& matrix){
			for(int i = 0; i < num; i++){
				_matrix[i] -= matrix._matrix[i];
			}
			return *this;		
		}
		Matrix3& operator *= (const Matrix3& matrix){
			float buffer[num] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0};
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
		
		Matrix3 operator + (const Matrix3& matrix) const{
			Matrix3 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = _matrix[i] + matrix._matrix[i];
			
			return outMatrix;
		}
		Matrix3 operator - (const Matrix3& matrix) const{
			Matrix3 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = _matrix[i] - matrix._matrix[i];
			
			return outMatrix;
		}
		Matrix3 operator * (const Matrix3& matrix) const{
			Matrix3 outMatrix(ZERO);
			for(int i = 0; i < num; i++){
				for(int j = 0; j < dim; j++){
					outMatrix[i] += _matrix[(i / dim) * dim + j] * matrix._matrix[i % dim + j * dim];
				}
			}
			
			return outMatrix;
		}
		Matrix3 operator * (float scalar) const{
			Matrix3 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = _matrix[i] * scalar;
			
			return outMatrix;
		}
		Matrix3 operator / (float scalar) const{
			Matrix3 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = _matrix[i] / scalar;
			
			return outMatrix;
		}
		Matrix3 operator - () const{
			Matrix3 outMatrix;
			for(int i = 0; i < num; i++)
				outMatrix[i] = - _matrix[i];
			
			return outMatrix;
		}
		Vector3 operator * (const Vector3& vector) const{
			Vector3 result;
			
			for(int i = 0; i < dim; i++){
				for(int j = 0; j < dim; j++){
					result[i] += _matrix[i * dim + j] * vector[j];
				}
			}
			
			return result;	
		}
		
		friend std::ostream& operator << (std::ostream& s, const Matrix3& matrix){
			for(int i = 0; i < matrix.dim; i++){
				s << "| ";
				for(int j = 0; j < matrix.dim; j++)
					s << matrix._matrix[matrix.dim * i + j] << " ";
				s << "|" << std::endl;
			}
			
			return s << std::endl;	
		}
		
		private:
		float _matrix[9];
		static const int dim = 3;
		static const int num = 9;
	};
}

#endif
