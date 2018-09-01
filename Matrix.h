#include <math.h>

class Matrix {
    public:
        Matrix(const unsigned short r, const unsigned short c);
        ~Matrix();
        Matrix(const Matrix &rhs);
        Matrix &operator=(const Matrix &rhs);
        Matrix operator+(const Matrix &rhs);
        Matrix operator+(const float &rhs);
        Matrix operator-(const Matrix &rhs);
        Matrix operator-(const float &rhs);
        Matrix operator*(const Matrix &rhs);
        Matrix operator*(const float &rhs);
        Matrix operator/(const float &rhs);
        float operator&(const Matrix &rhs);
        Matrix operator^(const Matrix &rhs);
        Matrix Transpose();
        float Determinate();
        float Minor(const unsigned int i, const unsigned int j);
        Matrix Cofactor();
        Matrix Adjugate();
        Matrix Inverse();
        unsigned short rows;
        unsigned short columns;
        float *Data;
};

Matrix::Matrix(const unsigned short r, const unsigned short c)
{
    rows = r;
    columns = c;
    Data = new float[rows * columns];
}

Matrix::~Matrix()
{
    delete Data;
    Data = NULL;
}

Matrix::Matrix(const Matrix &rhs)
{
    *this = rhs;
}

Matrix &Matrix::operator=(const Matrix &rhs)
{
    int element = 0;
    if (this != &rhs)
    {
        rows = rhs.rows;
        columns = rhs.columns;
        delete Data;
        Data = new float[rows * columns];
        // Aij = Bij
        while(element < (rows * columns))
        {
            Data[element] = rhs.Data[element];
            element++;
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &rhs) //Matrix Add
{
    Matrix ret(rows, columns);
    int element = 0;
    if ((rows == rhs.rows) && (columns == rhs.columns))
    {
        // (AB)ij = (Aij + Bij)
        while(element < (rows * columns))
        {
            ret.Data[element] = Data[element] + rhs.Data[element];
            element++;
        }
    }
    return ret;
}

Matrix Matrix::operator+(const float &rhs) //Scalar Add
{
    Matrix ret(rows, columns);
    int element = 0;
    while(element < (rows * columns))
    {
        ret.Data[element] = Data[element] + rhs;
        element++;
    }
    return ret;
}

Matrix Matrix::operator-(const Matrix &rhs) //Matrix Subtract
{
    Matrix ret(rows, columns);
    int element = 0;
    if ((rows == rhs.rows) && (columns == rhs.columns))
    {
        // (AB)ij = (Aij + Bij)
        while(element < (rows * columns))
        {
            ret.Data[element] = Data[element] - rhs.Data[element];
            element++;
        }
    }
    return ret;
}

Matrix Matrix::operator-(const float &rhs) //Scalar Subtract
{
    Matrix ret(rows, columns);
    int element = 0;
    while(element < (rows * columns))
    {
        ret.Data[element] = Data[element] - rhs;
        element++;
    }
    return ret;
}

Matrix Matrix::operator*(const Matrix &rhs) //Matrix Multiply
{
    Matrix ret(rows, rhs.columns);
    // r = columns = rhs.rows
    // (AB)ij = (Ai1 * B1j) + (Ai2 * B2j) ... (Air * Brj)
    for (unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < rhs.columns; j++)
        {
            ret.Data[(i * rhs.columns) + j] = 0;
            for (unsigned int r = 0; r < columns; r++)
            {
                ret.Data[(i * rhs.columns) + j] += Data[(i * columns) + r] * rhs.Data[(r * rhs.columns) + j];
            }
        }
    }
    return ret;
}

Matrix Matrix::operator*(const float &rhs) //Scalar Multiply
{
    Matrix ret(rows, columns);
    for (unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < columns; j++)
        {
            ret.Data[(i * columns) + j] = Data[(i * columns) + j] * rhs;
        }
    }
    return ret;
}

Matrix Matrix::operator/(const float &rhs) //Scalar Divide
{
    Matrix ret(rows, columns);
    for (unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < columns; j++)
        {
            ret.Data[(i * columns) + j] = Data[(i * columns) + j] / rhs;
        }
    }
    return ret;
}

float Matrix::operator&(const Matrix &rhs) //Dot Product
{
    float ret = 0;
    //AB = (A1 * B1) + (A2 * B2) ... (Ai * Bi)
    if (((rows == 1) || (columns == 1)) && ((rhs.rows == 1) || (rhs.columns == 1)))
    {
        if ((rows * columns) == (rhs.rows * rhs.columns))
        {
            for (unsigned int element = 0; element < (rows * columns); element++)
            {
                ret += Data[element] * rhs.Data[element];
            }
        }
    }
    return ret;
}
Matrix Matrix::operator^(const Matrix &rhs) //Cross Product
{
    Matrix ret(rows * columns, 1);
    //Ax = By * Cz - Bz * Cy
    //Ay = Bz * Cx - Bx * Cz
    //Az = Bx * Cy - By * Cx
    if (((rows == 1) || (columns == 1)) && ((rhs.rows == 1) || (rhs.columns == 1)))
    {
        if (((rows * columns) == (rhs.rows * rhs.columns)) && ((rows * columns) == 3))
        {
            ret.Data[0] = (Data[1] * rhs.Data[2]) - (Data[2] * rhs.Data[1]);
            ret.Data[1] = (Data[2] * rhs.Data[0]) - (Data[0] * rhs.Data[2]);
            ret.Data[2] = (Data[0] * rhs.Data[1]) - (Data[1] * rhs.Data[0]);
        }
    }
    return ret;
}

Matrix Matrix::Transpose()
{
    Matrix ret(columns, rows);
    // Aij = Aji
    for (unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < columns; j++)
        {
            ret.Data[(i * rows) + j] = Data[(j * columns) + i];
        }
    }
    return ret;
}

float Matrix::Determinate()
{
    float ret = 0.0;
    if (rows == columns)
    {
        if (rows > 2)
        {
            // |A| = Aij * Cij
            Matrix temp = Cofactor();
            for (unsigned int a = 0; a < columns; a++)
            {
                ret += Data[a] * temp.Data[a];
            }
        } else if (rows == 2) {
            // |A| = A11 * A22 - A12 * A21
            ret = (Data[0] * Data[3]) - (Data[1] * Data[2]);
        } else if (rows < 2) {
            ret = Data[0];
        }
    } else if ((rows == 1) || (columns == 1)) {
        float temp = 0.0;
        for (int a = 0; a < (rows * columns); a++)
        {
            temp += (Data[a] * Data[a]);
        }
        ret = sqrt(temp);
    }
    return ret;
}

float Matrix::Minor(const unsigned int i, const unsigned int j)
{
    Matrix temp(rows - 1, columns - 1);
    float ret = 0.0;
    if ((rows == columns) && (i < rows) && (j < columns))
    {
        unsigned int m = 0;
        unsigned int n = 0;
        for (unsigned int k = 0; k < (rows - 1); k++)
        {
            for (unsigned int l = 0; l < (columns - 1); l++)
            {
                if (k < i)
                {
                    m = k;
                } else {
                    m = (k + 1);
                }
                if (l < j)
                {
                    n = l;
                } else {
                    n = (l + 1);
                }
                temp.Data[(k * (columns - 1)) + l] = Data[(m * columns) + n];
            }
        }
        ret = temp.Determinate();
    }
    return ret;
}

Matrix Matrix::Cofactor()
{
    Matrix ret(rows, columns);
    if (rows == columns)
    {
        // Aij = (-1)^(i + j) * Mij
        for (unsigned int i = 0; i < rows; i++)
        {
            for (unsigned int j = 0; j < columns; j++)
            {
                float sign = 1.0;
                for (unsigned int k = 1; k <= (i + j + 2); k++)
                {
                    sign *= -1.0;
                }
                ret.Data[(i * columns) + j] = sign * Minor(i, j);
            }
        }
    }
    return ret;
}

Matrix Matrix::Adjugate()
{
    Matrix ret(rows, columns);
    ret = (Cofactor()).Transpose();
    return ret;
}

Matrix Matrix::Inverse()
{
    Matrix ret(columns, rows);
    float det = Determinate();
    if (det != 0.0)
    {
        ret = (Adjugate() * (1.0 / det));
    }
    return ret;
}
