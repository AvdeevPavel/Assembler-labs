#ifndef LINEARVECTOR_H_
#define LINEARVECTOR_H_
	#include <cstdlib>
	#include <iostream>
	#include <vector>
	#include "VectorOperations.h"

	class LinearVector { 
		private: 
			size_t sizeVector;
			std::vector<float> vect; 
		public: 
			/*Constructors*/
			LinearVector(const std::vector<float>& vect_);
			LinearVector(const size_t s, const float a); 
			LinearVector(const LinearVector& vector_);
			LinearVector& operator= (const LinearVector& vector_);  
			
			/*Geters*/
			size_t getSizeVector() const;
			std::vector<float> getVector() const; 
			void setElement(const int i, const float a); 
			float getElement(const int i) const;

			/*operations*/
			void addVector(const LinearVector& a); 
			void subVector(const LinearVector& a);

			/*over*/
			void print(); 
	}; 
#endif
