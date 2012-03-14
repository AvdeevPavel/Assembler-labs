#include "LinearVector.h"

/*Constructors*/
LinearVector::LinearVector(const std::vector<float>& vect_): vect(vect_) { 
	sizeVector = vect_.size();	
} 
 
LinearVector::LinearVector(const size_t s, const float a): vect(s, a) {
	sizeVector = s; 
}
 
LinearVector::LinearVector(const LinearVector& vector_) { 
	sizeVector = vector_.sizeVector; 
	vect = vector_.vect; 
} 

LinearVector& LinearVector::operator= (const LinearVector& vector_) { 
	sizeVector = vector_.sizeVector; 
	vect = vector_.vect; 
	return *this; 
}   
			
/*Geters*/
size_t LinearVector::getSizeVector() const { 
	return sizeVector; 	
}
 
std::vector<float> LinearVector::getVector() const { 
	return vect; 
}

void LinearVector::setElement(const int i, const float a) { 
	if (i < sizeVector)
		vect[i] = a; 
} 

float LinearVector::getElement(const int i) const { 
	if (i < sizeVector)
		return vect[i];
	return 0; 
} 

/*operations*/
void LinearVector::addVector(const LinearVector& a) { 
	if (a.sizeVector == sizeVector) { 
		add(&(vect[0]), &(a.vect[0]), sizeVector);
	} 
} 
 
void LinearVector::subVector(const LinearVector& a) {
	if (a.sizeVector == sizeVector)
		sub(&(vect[0]), &(a.vect[0]), sizeVector); 
} 

/*over*/
void LinearVector::print() { 
	for(std::vector<float>::iterator it = vect.begin(); it < vect.end(); ++it) 
		std::cout << *it << " "; 
	std::cout << std::endl; 
} 
