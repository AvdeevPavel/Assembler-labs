#include "SystemLinearEquations.h"

SystemLinearEquations::SystemLinearEquations(const Matrix& _matr, const LinearVector& _b): matr(_matr), b(_b) { 
} 

void SystemLinearEquations::JacodMethod() {
	Matrix D(matr.getDiag());
	D.backMatrixDiag();

	LinearVector g = D.mulRightVector(b);
	
	std::vector<float> e(D.getCountRows(), 1);
	Matrix B(e);
	B.subMatrix(D.mulMatrix(matr));

	float norm = 0.0; 
	LinearVector tempX(matr.getCountColumns(), 0); 
	LinearVector x(matr.getCountColumns(), 1) ;	
	int count = 0;

	do { 
		++count;
		tempX = B.mulRightVector(x);	
		tempX.addVector(g); 
		LinearVector vectNorm(x);
		vectNorm.subVector(tempX);	
		norm = fabs(vectNorm.getElement(0));
		
		for(int i = 0; i < vectNorm.getSizeVector(); ++i) { 
			if (fabs(vectNorm.getElement(i)) > norm) 
				norm = fabs(vectNorm.getElement(i));
		}  
		x = tempX;
		if (count > 1000000)
			break;
	} while (norm > EPS);
	
	answer = x.getVector();
}

std::vector<float> SystemLinearEquations::getAnswer() const {
	return answer; 
}  
