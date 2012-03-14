#ifndef MATRIX_H_
#define MATRIX_H_
	#include <cstdlib>
	#include <cmath>	
	#include <vector>
	#include <algorithm>
	#include <iostream> 
	#include "VectorOperations.h"
	#include "LinearVector.h"

	#define EPS 0.00001

	class Matrix { 
		private: 
			size_t countColumns;
			size_t countRows;		
			std::vector<float> matrix; 
		private: 
			std::vector<float> mulVector(std::vector<float>& b);	
		public: 
			/*Constructors*/
			Matrix(size_t countRows_, size_t countColumns_);
			Matrix(const std::vector<float>& input, const size_t countRows, const size_t countColumns_); 
			Matrix(const std::vector<float>& diag); 
			Matrix(const Matrix& input); 
			Matrix& operator= (const Matrix& input); 
			
			/*Geters*/
			double getElement(const size_t i, const size_t j) const; 
			void setElement(const size_t i, const size_t j, const double value);
			std::vector<float> getColumn(const int column) const; 
			std::vector<float> getRow(const int row) const; 
			std::vector<float> getDiag() const;
			size_t getCountColumns() const; 
			size_t getCountRows() const; 

			/*Operations*/
			void addMatrix(const Matrix& b); 
			void subMatrix(const Matrix& b);
			Matrix mulMatrix(const Matrix& b);		
			void mulConst(const float b); 
			LinearVector mulRightVector(const LinearVector& b);
			void backMatrixDiag(); 			
			
			/*over*/			
			void print(); 
	}; 

#endif
