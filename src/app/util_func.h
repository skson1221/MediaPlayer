#pragma once

template <class T>
inline void safe_delete(T ptr)
{
	delete ptr;
	ptr = nullptr;
}

template <class T>
inline void safe_delete_array(T arr)
{
	delete[] arr;
	arr = nullptr;
}
