#ifndef SYSTEMLINEAREQUATIONS_H_
#define SYSTEMLINEAREQUATIONS_H_
	#include <vector>
	#include <cstdlib>	
	#include <cmath>
	#include "Matrix.h"
	#include "LinearVector.h"

	class SystemLinearEquations { 
		private: 
			Matrix matr; 
			LinearVector b; 
			std::vector<float> answer; 
		public: 
			SystemLinearEquations (const Matrix& _matr, const LinearVector& _b); 
			void JacodMethod();
			std::vector<float> getAnswer() const; 
	}; 
#endif
