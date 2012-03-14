#include "Matrix.h"

/*Constructoes*/
Matrix::Matrix(size_t countRows_, size_t countColumns_) { 
	countRows = countRows_;
	countColumns = countColumns_; 
	matrix.assign(countRows * countColumns, 0);
} 

Matrix::Matrix(const std::vector<float>& input, const size_t countRows_, const size_t countColumns_) { 
	countRows = countRows_;
	countColumns = countColumns_; 
	matrix = input;
}  

Matrix::Matrix(const std::vector<float>& diag) {
	countRows = diag.size(); 
	countColumns = diag.size(); 
	matrix.assign(countRows * countColumns, 0);

	for(int i = 0, j = 0; j < diag.size(); i += countColumns + 1, ++j) { 
		matrix[i] = diag[j];
	}  
} 
 
Matrix::Matrix(const Matrix& input) { 
	countRows = input.countRows;
	countColumns = input.countColumns; 
	matrix.clear();
	matrix = input.matrix; 	
} 

Matrix& Matrix::operator= (const Matrix& input) { 
	countRows = input.countRows;
	countColumns = input.countColumns; 
	matrix.clear();
	matrix = input.matrix; 
	return *this; 	
} 

/*Geters & Seters*/
std::vector<float> Matrix::getColumn(const int column) const {	
	std::vector<float> output(countRows); 
	
	for(int i = 0; i < countRows; ++i) { 
		output[i] = matrix[i * countColumns + column];
	} 

	return output; 	
}

std::vector<float> Matrix::getDiag() const { 
	std::vector<float> output(std::min(countRows, countColumns)); 
	
	for(int i = 0, j = 0; j < std::min(countRows, countColumns); i += countColumns + 1, ++j) { 
		output[j] = matrix[i];
	}  

	return output; 	
	
} 


std::vector<float> Matrix::getRow(const int row) const { 
	std::vector<float> output(countColumns); 	
	output.assign(matrix.begin() + countColumns * row, matrix.begin() + countColumns * (row + 1));
	return output;
} 

double Matrix::getElement(const size_t i, const size_t j) const { 
	if (i < countRows && j < countColumns) { 
		return matrix[i * countColumns + j];
	} else 
		return 0.0; 
} 

void Matrix::setElement(const size_t i, const size_t j, const double value) { 
	if (i < countRows && j < countColumns) { 
		matrix[i * countColumns + j] = value;
	} 
} 

size_t Matrix::getCountColumns() const { 
	return countColumns;
} 

size_t Matrix::getCountRows() const { 
	return countRows;
} 

/*operations*/
void Matrix::addMatrix(const Matrix& b) { 
	if (countRows == b.countRows && countColumns == b.countColumns) { 
		add(&(matrix[0]), &(b.matrix[0]), matrix.size()); 
	} 
} 
 
void Matrix::subMatrix(const Matrix& b) { 
	if (countRows == b.countRows && countColumns == b.countColumns) { 
		sub(&(matrix[0]), &(b.matrix[0]), matrix.size()); 
	} 
} 

Matrix Matrix::mulMatrix(const Matrix& b) {
	Matrix result(countRows, b.countColumns);

	if (countColumns == b.countRows) { 
		std::vector<float> currentColumn;
		std::vector<float> temp;
		for(int i = 0; i < b.countColumns; ++i) { 
			currentColumn = b.getColumn(i);
			currentColumn = mulVector(currentColumn);  
			for(int j = i, k = 0; k < currentColumn.size(); j += countColumns, ++k) { 	
				result.matrix[j] = currentColumn[k];
			}
		}  
	}  

	return result; 
} 		

void Matrix::mulConst(const float b) {
	for(std::vector<float>::iterator it = matrix.begin(); it < matrix.end(); ++it) { 
		*it *= b;
	} 
} 

LinearVector Matrix::mulRightVector(const LinearVector& b) {
	std::vector<float> am = b.getVector(); 
	am  = mulVector(am); 
	LinearVector ret(am);
	return ret;
} 

void Matrix::backMatrixDiag() {
	for(int i = 0; i < std::min(countRows, countColumns); ++i) { 	
		if (fabs(getElement(i, i)- 0.0) > EPS)
			setElement(i, i, 1 / getElement(i, i));
	} 
}			


/*over*/			
void Matrix::print() { 
	for(int i = 0; i < countRows; ++i) { 
		for(int j = 0;  j < countColumns; ++j)
			std::cout << getElement(i, j) << " ";
		std::cout << std::endl; 	
	} 
	std::cout << std::endl;	
} 		

std::vector<float> Matrix::mulVector(std::vector<float>& b) { 
	std::vector<float> result(countRows, 0);
	if (countColumns == b.size()) { 
		size_t ver = whatSse(); 
		std::vector<float> currentRow; 
		for(int i = 0; i < countRows; ++i) {
			currentRow = getRow(i);

			int ravn1 = 4 - currentRow.size() % 4; 
			for(int j = 0; j < ravn1; ++j)
				currentRow.push_back(0);				

			int ravn2 = 4 - b.size() % 4;
			for(int j = 0; j < ravn2; ++j)
				b.push_back(0);				
							
			if (ver == SSE3_SUPPORTED) 
				result[i] = mulSSE3(&(currentRow[0]), &(b[0]), b.size());  
			else if (ver == SSE41_SUPPORTED) 
				result[i] = mulSSE41(&(currentRow[0]), &(b[0]), b.size());  
			else 
				std::cout << "SSE not supported" << std::endl;
			
			for(int j = 0; j < ravn1; ++j)
				currentRow.pop_back();				

			for(int j = 0; j < ravn2; ++j)
				b.pop_back();							
		}		
	}  
	
	return result;
} 
