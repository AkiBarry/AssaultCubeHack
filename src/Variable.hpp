#pragma once
#include <string>

template<class T>
class CVariable
{
public:
	CVariable() = default;
	CVariable(T val, std::string text) : data(val) , name(text){}

	operator T() const;
	T & operator=(T&& val);

	T GetVal() const;
	T& GetVal();

	void SetVal(const T& val);
	void SetVal(T&& val);

private:
	T data;
	std::string name;
};

template<class T>
CVariable<T>::operator T() const
{
	return data;
}

template<class T>
T & CVariable<T>::operator=(T&& val)
{
	SetVal(std::forward<T>(val));

	return data;
}

template<class T>
T CVariable<T>::GetVal() const
{
	return data;
}

template<class T>
T & CVariable<T>::GetVal()
{
	return data;
}

template<class T>
void CVariable<T>::SetVal(const T & val)
{
	data = val;
}

template<class T>
void CVariable<T>::SetVal(T&& val)
{
	data = val;
}
