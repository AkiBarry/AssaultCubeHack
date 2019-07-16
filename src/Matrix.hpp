#pragma once

#include "Math.hpp"
#include <initializer_list>
#include <algorithm>

template <class T, size_t rows, size_t columns>
class NMath::CMatrix
{
public:
	T data[rows][columns];

	CMatrix() {}
	CMatrix(std::initializer_list<std::initializer_list<T>> init_list);

	constexpr void									Zero();

	T *												operator[](size_t i);
	const T *										operator[](size_t i) const;

	CMatrix &										operator=(const CMatrix& m);

	CMatrix &										operator+=(const CMatrix& m);
	CMatrix &										operator-=(const CMatrix& m);

	template<class U>
	CMatrix &										operator*=(const CMatrix<U, columns, columns>& m);

	CMatrix &										operator*=(const T t);
	CMatrix &										operator/=(const T t);

	CMatrix											operator+(const CMatrix& m) const;
	CMatrix											operator-(const CMatrix& m) const;

	template<class U, size_t columns1>
	CMatrix<decltype(T() * U()), rows, columns1> &	operator*(const CMatrix<U, columns, columns1>& m);

	CMatrix											operator*(T t) const;
	CMatrix											operator/(T t) const;

	bool											operator==(const CMatrix& m) const;
	bool											operator!=(const CMatrix& m) const;

	T												Det() const;

	CMatrix<T, columns, rows>						Transpose() const;
	void											TransposeInPlace();

	void											Negate();
	bool											IsZero() const;

	template<class U, size_t rows1, size_t columns1>
	friend CMatrix<U, rows1, columns1>;
};

template<typename T, size_t rows, size_t columns>
constexpr void NMath::CMatrix<T, rows, columns>::Zero()
{
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < rows; ++j)
			data[i][j] = T();
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns>::CMatrix(std::initializer_list<std::initializer_list<T>> init_list)
{
	static_assert(init_list.size() == rows);

	auto it = init_list.begin();
	for (size_t i = 0; it != init_list.end(); ++i, ++it)
	{
		static_assert((*it).size() == columns);

		auto inner_it = (*it).begin();
		auto inner_list = (*it);

		for (size_t j = 0; inner_it != inner_list.end(); ++j, ++inner_it)
		{
			data[i][j] = *inner_it;
		}
	}
}

template <typename T, size_t rows, size_t columns>
T* NMath::CMatrix<T, rows, columns>::operator[](size_t i)
{
	return data[i];
}

template <typename T, size_t rows, size_t columns>
const T* NMath::CMatrix<T, rows, columns>::operator[](size_t i) const
{
	return data[i];
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns>& NMath::CMatrix<T, rows, columns>::operator=(const CMatrix& m)
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			data[i][j] = m.data[i][j];

	return *this;
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns>& NMath::CMatrix<T, rows, columns>::operator+=(const CMatrix& m)
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			data[i][j] += m.data[i][j];

	return *this;
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns>& NMath::CMatrix<T, rows, columns>::operator-=(const CMatrix& m)
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			data[i][j] -= m.data[i][j];

	return *this;
}

template<typename T, size_t rows, size_t columns>
template<class U>
NMath::CMatrix<T, rows, columns> & NMath::CMatrix<T, rows, columns>::operator*=(const CMatrix<U, columns, columns> & m)
{
	CMatrix<T, rows, columns> temp;

	temp.Zero();

	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < columns; ++j)
			for (size_t k = 0; k < columns; ++k)
				temp.data[i][j] += data[i][k] * m.data[k][j];

	*this = std::move(temp);

	return *this;
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns>& NMath::CMatrix<T, rows, columns>::operator*=(const T t)
{
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < columns; ++j)
			data[i][j] *= t;

	return *this;
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns>& NMath::CMatrix<T, rows, columns>::operator/=(const T t)
{
	if constexpr(std::is_floating_point<T>::value)
	{
		constexpr T invt = static_cast<T>(1) / t;

		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < columns; ++j)
				data[i][j] *= invt;
	}
	else
	{
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < columns; ++j)
				data[i][j] /= t;
	}

	return *this;
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns> NMath::CMatrix<T, rows, columns>::operator+(const CMatrix& m) const
{
	CMatrix temp;

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			temp.data[i][j] = data[i][j] + m.data[i][j];

	return temp;
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns> NMath::CMatrix<T, rows, columns>::operator-(const CMatrix& m) const
{
	CMatrix temp;

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			temp.data[i][j] = data[i][j] - m.data[i][j];

	return temp;
}

template<class T, size_t rows, size_t columns>
template<class U, size_t columns1>
NMath::CMatrix<decltype(T() * U()), rows, columns1> & NMath::CMatrix<T, rows, columns>::operator*(const CMatrix<U, columns, columns1> & m)
{
	CMatrix<decltype(T() * U()), rows, columns1> temp;

	temp.Zero();

	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < columns1; ++j)
			for (size_t k = 0; k < columns; ++k)
				temp.data[i][j] += data[i][k] * m.data[k][j];

	return temp;
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns> NMath::CMatrix<T, rows, columns>::operator*(T t) const
{
	CMatrix temp;

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			temp.data[i][j] = data[i][j] * t;

	return temp;
}

template <typename T, size_t rows, size_t columns>
NMath::CMatrix<T, rows, columns> NMath::CMatrix<T, rows, columns>::operator/(T t) const
{
	CMatrix temp;

	if constexpr(std::is_floating_point<T>::value)
	{
		constexpr T invt = static_cast<T>(1) / t;

		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < columns; ++j)
				temp.data[i][j] = data[i][j] * invt;
	}
	else
	{
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < columns; ++j)
				temp.data[i][j] = data[i][j] / t;
	}

	return *this;
}

template <typename T, size_t rows, size_t columns>
bool NMath::CMatrix<T, rows, columns>::operator==(const CMatrix& m) const
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
		{
			if (data[i][j] != m.data[i][j])
				return false;
		}
		
	return true;
}

template <typename T, size_t rows, size_t columns>
bool NMath::CMatrix<T, rows, columns>::operator!=(const CMatrix& m) const
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
		{
			if (data[i][j] != m.data[i][j])
				return true;
		}

	return false;
}

template<class T, size_t rows, size_t columns>
T NMath::CMatrix<T, rows, columns>::Det() const
{
	static_assert(rows == columns);

	constexpr size_t N = rows;

	if constexpr (N == 2)
	{
		return data[0][0] * data[1][1] - data[0][1] * data[1][0];
	}

	return T();
	/*
	for (int i = 0; i < rows; ++i)
		for (int j = i+1; j < columns; ++j)
			data[i][j] = -data[i][j];
	*/
}

template<class T, size_t rows, size_t columns>
NMath::CMatrix<T, columns, rows> NMath::CMatrix<T, rows, columns>::Transpose() const
{
	CMatrix<T, columns, rows> temp;

	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < columns; ++j)
			temp.data[j][i] = data[i][j];

	return std::move(temp);
}

template<class T, size_t rows, size_t columns>
void NMath::CMatrix<T, rows, columns>::TransposeInPlace()
{
	static_assert(rows == columns);

	for (int i = 0; i < rows; ++i)
		for (int j = i + 1; j < columns; ++j)
			data[i][j] = data[j][i];
}

template <typename T, size_t rows, size_t columns>
void NMath::CMatrix<T, rows, columns>::Negate()
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
			data[i][j] = -data[i][j];
}

template <typename T, size_t rows, size_t columns>
bool NMath::CMatrix<T, rows, columns>::IsZero() const
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < columns; ++j)
		{
			if (data[i][j] != T())
				return false;
		}

	return true;
}
