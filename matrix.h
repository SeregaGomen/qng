#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <ostream>

using namespace std;


template <typename T> class matrix
{
private:
    vector<T> buffer;
    unsigned rows;
    unsigned cols;
public:
    matrix(void)
    {
        rows = cols = 0;
    }
    matrix(unsigned r, unsigned c)
    {
        rows = r;
        cols = c;
        buffer.resize(rows*cols);
    }
    matrix(const matrix<T>& r)
    {
        rows = r.rows;
        cols = r.cols;
        buffer = r.buffer;
    }
   ~matrix()
    {
        buffer.clear();
    }
    void resize(unsigned r, unsigned c)
    {
        rows = r;
        cols = c;
        buffer.resize(rows*cols);
    }
    matrix operator = (const matrix& r)
    {
        rows = r.rows;
        cols = r.cols;
        buffer = r.buffer;
        return *this;
    }
    T* operator [] (int i)
    {
        return buffer.data() + i*cols;
    }
    unsigned size1(void)
    {
        return rows;
    }
    unsigned size2(void)
    {
        return cols;
    }
    friend ostream& operator << (ostream& out, matrix& r)
    {
        out.setf( ios::fixed,  ios::floatfield );
        for (unsigned i = 0; i < r.size1(); i++)
        {
            for (unsigned j = 0; j < r.size2(); j++)
            {
                out.precision(5);
                out.width(15);
                out << r[i][j] << ' ';
            }
            out << endl;
        }
        return out;
    }
    void fill(T val)
    {
        fill(buffer.begin(),buffer.end(),val);
    }
    void clear(void)
    {
        rows = cols = 0;
        buffer.clear();
    }
    T* data(void)
    {
        return buffer.data();
    }
    vector<T>& asVector(void)
    {
        return buffer;
    }
};


#endif // MATRIX_H
