#include <iostream>
#include <iomanip>
#include <cmath>

#include <vector>
#include <algorithm>

#include <sstream>
#include <string>


template <class Value, class Index>
class Vector {
private:
	std::vector<Value> values;

public:
	Vector() {}

	Vector(Index size, Value value = 0) {
		this->values.resize(size > 0 ? size : 0, value);
	}

	Vector(Index size, Value values[]) {
		this->values.resize(size > 0 ? size : 0, 0);	
		std::copy(values, values + this->values.size(), this->values.begin());
	}

	Vector(std::vector<Value> values) {
		this->values = values;
	}

public:
	~Vector() {
		this->values.clear();
	}

public:
	Value max() const {
		return this->size()>0 ? this->values[this->maxIndex()] : 0;
	}

	Index maxIndex() const {
		Index _ = -1;
		for (Index i=0; i<this->size(); i++){
			_ = _<0 ? 0 : this->values[i]>this->values[_] ? i : _;
		}
		return _;
	}

	Value min() const {
		return this->size()>0 ? this->values[this->minIndex()] : 0;
	}

	Index minIndex() const {
		Index _ = -1;
		for (Index i=0; i<this->size(); i++){
			_ = _<0 ? 0 : this->values[i]<this->values[_] ? i : _;
		}
		return _;
	}

	Value norm() const {
		return this->size()>0 ? (Value)sqrt((double)this->sum(2, false)) : 0;
	}

	Index size() const {
		return (Index)this->values.size();
	}

	Vector slice(Index start = 0, Index length = -1, Value value = 0) const {
		if (length==0){
			return Vector();
		}
		else if (start<0) {
			return this->slice(this->size() + start, length, value);
		}
		else if (length<0){
			return this->slice(start, this->size() - start, value);
		}
		else {
			std::vector<Value> v(length, value);
			Index size = this->size() - start;
			Index end = start + (length < size ? length : size);
			std::copy(this->values.begin() + start, this->values.begin() + end, v.begin());
			return Vector(v);
		}
	}

	Value sum(double p = 1, bool a = false) const {
		Value result = 0;
		for (Index i=0; i<this->size(); i++){
			Value v = this->values[i];
			v = (a) ? (Value)abs((double)v) : v;
			v = (p!=1) ? (Value)pow((double)v, p) : v;
			result += v;
		}
		return result;
	}

public:
	operator std::string() const {
		std::ostringstream stream;
		stream << "(Vector) { ";
		for (Index i=0; i<this->size(); i++) {
			stream << (i==0 ? "" : ", ");
			// stream << std::setw(10);
			// stream << std::setprecision(8);
			stream << this->values[i];
		}
		stream << " }";
		std::string result(stream.str());
		return result;
	}

	Vector & operator =(Vector const & _) {
		values = _.values;
		return *this;
	}

	bool operator ==(Vector const & _) const {
		for (Index i=0; i<this->size(); i++) {
			if (this->values[i]==_.values[i]) continue;
			return false;
		};
		return true;
	}

	bool operator !=(Vector const & _) const {
		return *this==_ ? false : true;
	}

	Value const & operator [](const Index _) const {
		return this->values[_];
	}

	Value & operator [](const Index _) {
		return this->values[_];
	}

	Vector operator +() const {
		return Vector(this->values);
	}
	Vector operator -() const {
		std::vector<Value> _ = this->values;
		for (Index i=0; i<_.size(); _.values[i] = -_.values[i], i++);
		return Vector(_);
	}

	Vector operator +(Vector const & _) const {
		std::vector<Value> v(max(this->size(), _.size()), 0);
		for (Index i=0; i<v.size(); i++) {
			v.values[i] += i<this->size() ? this->values[i] : 0;
			v.values[i] += i<_.size() ? _.values[i] : 0;
		}
		return Vector(v);
	}

	Vector operator -(Vector const & _) const {
		std::vector<Value> v(max(this->size(), _.size()), 0);
		for (Index i=0; i<v.size(); i++) {
			v.values[i] += i<this->size() ? this->values[i] : 0;
			v.values[i] -= i<_.size() ? _.values[i] : 0;
		}
		return Vector(v);
	}

	Vector operator +(Value const & _) const {
		std::vector<Value> v(this->values);
		for (Index i=0; i<v.size(); v[i++] += _);
		return Vector(v);
	}

	Vector operator -(Value const & _) const {
		std::vector<Value> v(this->values);
		for (Index i=0; i<v.size(); v[i++] -= _);
		return Vector(v);
	}

	Vector operator *(Value const & _) const {
		std::vector<Value> v(this->values);
		for (Index i=0; i<v.size(); v[i++] *= _);
		return Vector(v);
	}

};

/** /
int main(int argc, char const *argv[]){
	double x1[] = {4, 5, 23, 6};
	Vector<double, int> v(4, x1);
	Vector<double, int> b(3, 1);
	std::cout << std::string(v) << std::endl;
	std::cout << std::string(v * 2) << std::endl;
	std::cout << std::string(v * 2 - 2) << std::endl;
	std::cout << std::string(v + 1) << std::endl;
	std::cout << std::string(v.slice(1, 2)) << std::endl;
	std::cout << std::string(v.slice(1, 12)) << std::endl;
	std::cout << std::string(v.slice(-1, 3) + 4) << std::endl;
	std::cout << std::string(b + 2.1) << std::endl;
	std::cout << (b + 2.1).sum() << std::endl;
	std::cout << (b + 2.1).sum(1, false) << std::endl;
	std::cout << (b + 2.1).sum(2, false) << std::endl;
	std::cout << (b + 2.1).norm() << std::endl;
	return 0;
}
/**/