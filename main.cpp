#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

typedef double TValue;
typedef std::vector< TValue > TMatrixRow;
typedef std::vector< TMatrixRow > TMatrix;

class Matrix {
public:
	TMatrix values;
	unsigned int size;

public:
	Matrix(unsigned int size, bool E = false) {
		this->size = size;
		for (int i=0; i<size; i++) {
			TMatrixRow row;
			this->values.push_back(row);
			for (int j=0; j<size; j++) {
				this->values[i].push_back(E && i==j ? 1 : 0);
			}
		}
	}
	Matrix(unsigned int size, TValue value) {
		this->size = size;
		for (int i=0; i<size; i++) {
			TMatrixRow row;
			this->values.push_back(row);
			for (int j=0; j<size; j++) {
				this->values[i].push_back(value);
			}
		}
	}
	Matrix(unsigned int size, TMatrixRow values) {
		this->size = size;
		for (int i=0, k=0; i<size; i++) {
			TMatrixRow row;
			this->values.push_back(row);
			for (int j=0; j<size; j++) {
				this->values[i].push_back(values[k++]);
				// this->values[i].push_back(values[i*size+j]);
			}
		}
	}
	Matrix(unsigned int size, TMatrix values) {
		this->size = size;
		for (int i=0, k=0; i<size; i++) {
			TMatrixRow row;
			this->values.push_back(row);
			for (int j=0; j<size; j++) {
				this->values[i].push_back(values[i][j]);
			}
		}
	}
	Matrix(Matrix & _) {
		this->size = _.size;
		for (int i=0; i<size; i++) {
			TMatrixRow row;
			this->values.push_back(row);
			for (int j=0; j<size; j++) {
				this->values[i].push_back(_.values[i][j]);
			}
		}
	}
	~Matrix() {
		for (int i=0; i<size; i++) {
			this->values[i].clear();
		}
		this->values.clear();
	}

public:
	void print (std::string title = "") {
		if (title.length() > 0) {
			std::cout << title << std::endl;
		}
		for (int i=0; i<this->size; i++) {
			for (int j=0; j<this->size; j++) {
				TValue v = this->values[i][j];
				// v = fabs(v)<0.00000001 ? 0 : v;
				std::cout << std::setw(12);
				std::cout << std::fixed;
				std::cout << std::setprecision(8);
				std::cout << v;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	TMatrixRow valuesOf () {
		TMatrixRow result;
		for (int i=0; i<this->size; i++)
			for (int j=0; j<this->size; j++)
				result.push_back(this->values[i][j]);
		return result;
	}
	
};


typedef struct {
	unsigned int size;
	TMatrixRow a;
	TMatrixRow b;
} Config;

Config readConfig (std::string filePath) {
	Config cfg;
	std::ifstream f(filePath);
	if ( f ) {
		f >> cfg.size;

		for (int i=0; i<cfg.size; i++) {
			for (int j=0; j<cfg.size; j++) {
				TValue value;
				f >> value;
				cfg.a.push_back(value);
			}
		}

		for (int k=0; k<cfg.size; k++) {
			TValue value;
			f >> value;
			cfg.b.push_back(value);
		}

		f.close();
	}
	else std::cout << "Unable to open config file";
	return cfg;
}



/*
L:
		1.000000    0.000000    0.000000    0.000000    0.000000
		0.198083    1.000000    0.000000    0.000000    0.000000
		0.132588   -0.651735    1.000000    0.000000    0.000000
		0.198083   -0.007597    0.314891    1.000000    0.000000
		0.038339    0.172096    0.159178   -1.217739    1.000000
*/
TMatrixRow gaussUp(Matrix a, TMatrixRow b) {
	TMatrixRow y;
	for (unsigned int i=0; i<a.size; i++) {
		TValue value = b[i];
		for (unsigned int j=0; j<i; j++) {
			value -= y[j] * a.values[i][j];
		}
		value /= a.values[i][i];
		y.push_back(value);
	}
	return y;
}

/*
U:
		6.260000    0.960000    1.110000    1.240000    0.240000
		0.000000    3.969840    1.080128   -1.875623   -1.427540
		0.000000    0.000000    5.996785    0.713182   -1.879199
		0.000000    0.000000    0.000000    5.615553   14.533358
		0.000000    0.000000   -0.000000    0.000000   25.233430
*/
TMatrixRow gaussDown(Matrix a, TMatrixRow b) {
	TMatrixRow x(a.size);
	for (unsigned int i=0; i<a.size; i++) {
		unsigned int i_idx = a.size-1 - i;
		TValue value = b[i_idx];
		for (unsigned int j=0; j<i; j++) {
			unsigned int j_idx = a.size-1 - j;
			value -= x[j_idx] * a.values[i_idx][j_idx];
		}
		value /= a.values[i_idx][i_idx];
		x[i_idx] = value;
	}
	return x;
}

void printRow (TMatrixRow _, std::string title = "") {
	if (title.length() > 0) {
		std::cout << title << std::endl;
	}
	for (int i=0; i<_.size(); i++) {
		std::cout << std::setw(12);
		std::cout << std::fixed;
		std::cout << std::setprecision(8);
		std::cout << _[i];
	}
	std::cout << std::endl;
}

void LU ( Matrix A, Matrix &L, Matrix &U, int n ) {
	U=A;

	for(int i=0; i<n; i++)
		for(int j=i; j<n; j++)
			L.values[j][i] = U.values[j][i] / U.values[i][i];
	
	for(int k=1; k<n; k++) {
		for(int i=k-1; i<n; i++)
			for(int j=i; j<n; j++)
				L.values[j][i] = U.values[j][i] / U.values[i][i];

		for(int i=k; i<n; i++)
			for(int j=k-1; j<n; j++)
				U.values[i][j] = U.values[i][j] - L.values[i][k-1] * U.values[k-1][j];
	}
}

void multi ( Matrix A, Matrix B, Matrix &R, int n ) {
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			for(int k=0; k<n; k++)
				R.values[i][j] += A.values[i][k] * B.values[k][j];
}

TValue det ( Matrix U ) {
	TValue result = 1;
	for(int i=0; i<U.size; i++){
		result *= U.values[i][i];
	}
	return result;
}

void printTMatrix (TMatrix _, std::string title = "") {
	std::cout << title << std::endl;
	for(int i=0; i<_.size(); i++) {
		for(int j=0; j<_[i].size(); j++) {
			std::cout << std::setw(12);
			std::cout << std::fixed;
			std::cout << std::setprecision(6);
			std::cout << _[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void invers ( Matrix A, Matrix& result ) {
	// TODO: A^-1 as A|E & Gauss
	unsigned int size = A.size;
	Matrix E(size, true);
	TMatrix AE;
	for(int i=0; i<size; i++) {
		TMatrixRow row;
		AE.push_back(row);
		for(int j=0; j<size; j++) {
			AE[i].push_back(A.values[i][j]);
		}
		for(int j=0; j<size; j++) {
			AE[i].push_back(E.values[i][j]);
		}
	}
	//printTMatrix(AE, "AE1"); // !!!

	// Gauss
	unsigned int rsize = size;
	unsigned int csize = size*2;
	for(int i=0; i<rsize; i++) {
		// Search for maximum in this column
		TValue maxEl = fabs(AE[i][i]);
		unsigned int maxRow = i;
		for(unsigned int k=i+1; k<rsize; k++) {
			if(fabs(AE[k][i]) > maxEl) {
				maxEl = fabs(AE[k][i]);
				maxRow = k;
			}
		}

		// Swap maximum row with current row (column by column)
		if(i!=maxRow){		
			for(unsigned int j=i; j<csize; j++) {
				TValue tmp = AE[maxRow][j];
				AE[maxRow][j] = AE[i][j];
				AE[i][j] = tmp;
			}
		}

		// Make all rows below this one 0 in current column
		for(unsigned int k=i+1; k<rsize; k++) {
			TValue c = -AE[k][i] / AE[i][i];
			for(int j=i; j<csize; j++) {
				if(i==j) {
					AE[k][j] = 0;
					continue;
				}
				AE[k][j] += c * AE[i][j];	
			}
		}

	}
	//printTMatrix(AE, "AE2"); // !!!

	// normalize
	for(unsigned int i=0; i<rsize; i++) {
		if(AE[i][i]==0) continue;
		TValue c = 1/AE[i][i];
		for(unsigned int j=0; j<csize; j++) {
			AE[i][j] *= c;
		}
	}
	//printTMatrix(AE, "AE3"); // !!!

	// A -> E
	for(unsigned int jj=rsize; jj>0; jj--) {
		unsigned int j = jj-1;
		for(unsigned int ii=jj-1; ii>0; ii--) {
			unsigned int i = ii-1;
			TValue c = -AE[i][j]/AE[j][j];
			for(unsigned int k=0; k<csize; k++) {
				AE[i][k] = AE[i][k] + c*AE[j][k];				
			}
		}
	}
	//printTMatrix(AE, "AE4"); // !!!

	// out the result
	for(unsigned int i=0; i<rsize; i++) {
		for(unsigned int j=0; j<rsize; j++) {
			result.values[i][j] = AE[i][rsize+j];
		}
		AE[i].clear();
	}
	AE.clear();
}


int main() {
	const std::string filePath = "input.txt";
	Config cfg = readConfig(filePath);
	Matrix A(cfg.size, cfg.a), L(cfg.size), U(cfg.size), R(cfg.size);
	LU(A, L, U, cfg.size);
	A.print("A:");
	L.print("L:");
	U.print("U:");
	multi(L, U, R, cfg.size);
	R.print("L*U:");
	TMatrixRow y = gaussUp(L, cfg.b);
	printRow(y, "y:");
	TMatrixRow x = gaussDown(U, y);
	printRow(x, "x:");

	TValue detA = det(U);
	printf("det(A): %lf\n", detA);
	Matrix A_1(cfg.size);
	invers(A, A_1);
	A_1.print("A^-1:");
	Matrix AA(cfg.size);
	multi(A, A_1, AA, cfg.size);
	AA.print("A*A^-1:");

	return 0;
}
 