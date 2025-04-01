#ifndef MAT
#define MAT
#include <cstdio>
#include <iostream>
#include <concepts>
#include <source_location>
#include <stdexcept>

namespace lawcat
{
   class dimension_mismatch_error : public std::runtime_error 
   {
      public:
         explicit dimension_mismatch_error(const std::string& message, const std::source_location& location = std::source_location::current())
            : std::runtime_error(message + " at " + location.file_name() + ":" + std::to_string(location.line()) + ".") {}
   };

   inline void check_matrix_dimensions(const size_t& n_rows_a, const size_t& n_cols_a, const size_t& n_rows_b, const size_t& n_cols_b) 
   {
      if ( n_rows_a != n_rows_b || n_cols_a != n_cols_b ) 
      {
         const std::string msg = "Cannot perform addition between a " + std::to_string(n_rows_a) + "x" + std::to_string(n_cols_a) + " matrix and a " + std::to_string(n_rows_b) + "x" + std::to_string(n_cols_b) + " matrix.";
         throw dimension_mismatch_error(msg);
      }
   }

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
            void set(const size_t& row, const size_t col, const T& value);

            bool print(const std::source_location& location = std::source_location::current()) const;

            mat<T> operator+(const mat<T>& other) const;
            void operator+=(const mat<T>& other);
            mat<T> operator-(const mat<T>& other) const;
            void operator-=(const mat<T>& other);
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
      void mat<T>::set(const size_t& row, const size_t col, const T& value)
      {
         if ( row >= this->n_rows || row < 0 )
            throw std::invalid_argument("ERROR: Row lies outside the bounds of the matrix.");

         if ( col >= this->n_cols || col < 0 )
            throw std::invalid_argument("ERROR: Column lies outside the bounds of the matrix.");

         this->data[row][col] = value;
      }

   template <typename T>
      mat<T>::~mat()
      {
         for ( size_t i = 0; i < this->n_rows; ++i )
            delete[] this->data[i];
         delete[] this->data;
      }

   template <typename T>
      mat<T> mat<T>::operator+(const mat<T>& other) const
      {
         check_matrix_dimensions(this->n_rows, this->n_cols, other.n_rows, other.n_cols);

         mat<T> out(this->n_rows, this->n_cols);

         for ( size_t i = 0; i < this->n_rows; ++i )
            for ( size_t j = 0; j < this->n_cols; ++j )
               out.set(i, j, this->data[i][j] + other.data[i][j]);

         return out;
      }

   template <typename T>
      void mat<T>::operator+=(const mat<T>& other) 
      {
         check_matrix_dimensions(this->n_rows, this->n_cols, other.n_rows, other.n_cols);

         for ( size_t i = 0; i < this->n_rows; ++i )
            for ( size_t j = 0; j < this->n_cols; ++j )
               this->data[i][j] += other.data[i][j];
      }

   template <typename T>
      mat<T> mat<T>::operator-(const mat<T>& other) const
      {
         check_matrix_dimensions(this->n_rows, this->n_cols, other.n_rows, other.n_cols);

         mat<T> out(this->n_rows, this->n_cols);

         for ( size_t i = 0; i < this->n_rows; ++i )
            for ( size_t j = 0; j < this->n_cols; ++j )
               out.set(i, j, this->data[i][j] - other.data[i][j]);

         return out;
      }

   template <typename T>
      void mat<T>::operator-=(const mat<T>& other) 
      {
         check_matrix_dimensions(this->n_rows, this->n_cols, other.n_rows, other.n_cols);

         for ( size_t i = 0; i < this->n_rows; ++i )
            for ( size_t j = 0; j < this->n_cols; ++j )
               this->data[i][j] -= other.data[i][j];
      }
}
#endif
