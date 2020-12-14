# Matrix

## Inverse

 A variant of Gaussian elimination called Gauss--Jordan elimination can be used for finding the inverse of a matrix, if it exists. If $A$ is an $n \times n$ square matrix, then one can use row reduction to compute its inverse matrix, if it exists. First, the $n \times n$ identity matrix is augmented to the right of $A$, forming an $n \times 2n$ block matrix $[A | I]$. Now through application of elementary row operations, find the reduced echelon form of this $n \times 2n$ matrix. The matrix $A$ is invertible if and only if the left block can be reduced to the identity matrix $I$; in this case the right block of the final matrix is $A^{−1}$. If the algorithm is unable to reduce the left block to $I$, then $A$ is not invertible.

## Multiply

``` cpp
unsigned int k; // number of row
struct Vector
{
    inline unsigned long long& operator[](const size_t ind) { return val[ind]; }
    inline const unsigned long long operator[](const size_t ind) const { return val[ind]; }
    unsigned long long val[maxk + 1] = {};
};
struct Matrix
{
    inline Vector& operator[](const size_t ind) { return val[ind]; }
    inline const Vector& operator[](const size_t ind) const { return val[ind]; }
    void unit()
    {
        for (unsigned int i = 0; i < k; ++i)
            val[i][i] = 1;
    }

    Vector val[maxk + 1];
};
Vector operator*(const Matrix& m, const Vector& v)
{
    Vector ret;
    for (unsigned int i = 0; i < k; ++i)
        for (unsigned int j = 0; j < k; ++j)
            ret[j] = ret[j] + m[i][j] * v[i];
    return ret;
}
Matrix operator*(const Matrix& m, const Matrix& r)
{
    Matrix ret;
    for (unsigned int i = 0; i < k; ++i)
        ret[i] = m * r[i];
    return ret;
}
Matrix operator^(Matrix m, unsigned long long e)
{
    Matrix ret;
    ret.unit();
    for (unsigned long long i = 1; e; i <<= 1)
    {
        if (e & i)
        {
            ret = ret * m;
            e ^= i;
        }
        m = m * m;
    }
    return ret;
}
```
