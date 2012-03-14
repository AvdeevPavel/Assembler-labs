#include <fstream>
#include <vector>
#include <cstdlib>
#include "Matrix.h"
#include "LinearVector.h"
#include "SystemLinearEquations.h"

void readLinearEquations(std::vector<float> &inputMatrix, std::vector<float> &inputVector, size_t &sizeRows, size_t &sizeColumns) { 
	std::ifstream input("input.txt");
	float temp = 0.0; 

	input >> sizeRows >> sizeColumns;

	for(size_t i = 0; i < sizeRows * sizeColumns; ++i) { 
		input >> temp;  
		inputMatrix.push_back(temp); 
	} 
	
	for(size_t i = 0; i < sizeRows; ++i) { 
		input >> temp;  
		inputVector.push_back(temp); 
	}
}  

void printAnswer(std::vector<float>& result) { 
	std::ofstream output("output.txt");
	output << "answer" << std::endl; 

	for(std::vector<float>::iterator it = result.begin(); it < result.end(); ++it) { 
		output << *it << std::endl; 
	} 
	
} 

int main() { 
	size_t sizeRows = 0;
	size_t sizeColumns = 0; 
	std::vector<float> inputMatrix; 
	std::vector<float> inputVector; 
	std::vector<float> answer; 

	readLinearEquations(inputMatrix, inputVector, sizeRows, sizeColumns);  	
 
	Matrix a(inputMatrix, sizeRows, sizeColumns);
	LinearVector b(inputVector);
	SystemLinearEquations lin(a, b);
	lin.JacodMethod();
	answer = lin.getAnswer();

	printAnswer(answer);
} 
