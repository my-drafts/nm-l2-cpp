#include <iostream>
#include <cmath>
#include <string>
#include <vector>


template <class Value, class Index>
class Matrix {
	static Matrix<Value, Index> initBySize (Index size, Value value = 0) {
		return (new Matrix<Value, Index>()).resize(size, size, value);
	}

	static Matrix<Value, Index> initBySizes (Index rows, Index columns, Value value = 0) {
		return (new Matrix<Value, Index>()).resize(rows, columns, value);
	}

	static Matrix<Value, Index> initByValues (Index rows, Index columns, Value values[]) {
		Matrix<Value, Index> _ = new Matrix<Value, Index>();
		_.resize(rows, columns);
		for (Index k=0; k<rows*columns; _.values[k] = values[k], k++);
		return _;
	}

	static Matrix<Value, Index> initByValues (Index rows, Index columns, std::vector<Value> values) {
		return (new Matrix()).resize(rows, columns, values);
	}

private:
	Index rowsCount;
	Index columnsCount;
	std::vector<Value> values;

private:
	Matrix<Value, Index> & resize(Index rows, Index columns, Value value = 0) {
		this.rowsCount = rows;
		this.columnsCount = columns;
		this.values.resize(this.size(), value);	
		return *this;
	}

	Matrix<Value, Index> & resize(Index rows, Index columns, std::vector<Value> values) {
		this.resize(rows, columns);	
		for (Index i=0, size=max(values.size(), this.size()); i<size; i++) {
			this.values[i] = values[i];
		}
		return *this;
	}

public:
	Matrix(): rowsCount(0), columnsCount(0) {}
	~Matrix() {
		this.values.clear();
	}

public:
	Index columns() {
		return this.columnsCount;
	}
	Index rows() {
		return this.rowsCount;
	}
	Index size() {
		return this.columns() * this.rows();
	}

	
};

int main(int argc, char const *argv[]){
	std::cout << "XXX";
	return 0;
}