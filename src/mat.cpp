#ifndef MAT
#define MAT
#include <cstdio>
#include <iostream>
#include <concepts>
#include <source_location>

template <typename T>
concept printable = requires(T value) 
{
   { std::cout << value } -> std::same_as<std::ostream&>;
};

template <typename T>
class mat
{
   private:
      size_t n_rows;
      size_t n_cols;
      T** data;

   public:
      mat(const size_t& n_rows, const size_t& n_cols);
      ~mat();

      void fill(const T& value);
      bool print(const std::source_location& location = std::source_location::current()) const;
      mat<T> operator+(const mat<T>& other);
};

template <typename T>
mat<T>::mat(const size_t& n_rows, const size_t& n_cols) 
{
   this->n_rows = n_rows;
   this->n_cols = n_cols;
   this->data = new T*[this->n_rows];

   for ( size_t i = 0; i < n_rows; ++i )
      this->data[i] = new T[n_cols];
}

template <typename T>
bool mat<T>::print(const std::source_location& location) const
{
   for ( size_t i = 0; i < this->n_rows; ++i )
   {
      for ( size_t j = 0; j < this->n_cols; ++j )
      {
         if constexpr (printable<T>)
            std::cout << this->data[i][j] << " ";
         else
         {
            std::cout << "PRINT IGNORED (" 
                      << location.file_name() << " -> L" 
                      << location.line() << "):"
                      << " Provided template argument does not support the << operator with std::ostream."
                      << std::endl;
            return false;
         }

         if ( j == this->n_cols - 1 )
            std::cout << std::endl;
      }
   }
   return true;
}

template <typename T>
void mat<T>::fill(const T& value)
{
   for ( size_t i = 0; i < n_rows; ++i )
      for ( size_t j = 0; j < n_cols; ++j )
         this->data[i][j] = value;
}

template <typename T>
mat<T>::~mat()
{
   for ( size_t i = 0; i < this->n_rows; ++i )
      delete[] this->data[i];
   delete[] this->data;
}

template <typename T>
mat<T> mat<T>::operator+(const mat<T>& other)
{
   if ( other.n_rows == this->n_rows && other.n_cols == this->n_cols )
   {
      return 1;
   }
}
#endif
