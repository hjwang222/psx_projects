extern "C" {
    #include <math.h>
    // fixmath tries to include fix16.hpp if defined
    #undef __cplusplus
    #include <fixmath.h>
    #define __cplusplus
}

class Vector{
public:
    fix16_t xyz[3];
    Vector()
    {
        xyz[0] = xyz[1] = xyz[2] = 0;
    }

    Vector(double x, double y, double z)
    {
        xyz[0] = fix16_from_dbl(x);
        xyz[1] = fix16_from_dbl(y);
        xyz[2] = fix16_from_dbl(z);
    }

    double x() {
        return fix16_to_dbl(xyz[0]);
    }

    double y() {
        return fix16_to_dbl(xyz[1]);
    }

    double z() {
        return fix16_to_dbl(xyz[2]);
    }

    void x(double val) {
        xyz[0] = fix16_from_dbl(val);
    }

    void y(double val) {
        xyz[1] = fix16_from_dbl(val);
    }

    void z(double val) {
        xyz[2] = fix16_from_dbl(val);
    }

    Vector operator+(const Vector& v) const
    {
        Vector result;
        result.xyz[0] = fix16_add(xyz[0], v.xyz[0]);
        result.xyz[1] = fix16_add(xyz[1], v.xyz[1]);
        result.xyz[2] = fix16_add(xyz[2], v.xyz[2]);
        return result;
    }

    Vector operator-(const Vector& v) const
    {
        Vector result;
        result.xyz[0] = fix16_sub(xyz[0], v.xyz[0]);
        result.xyz[1] = fix16_sub(xyz[1], v.xyz[1]);
        result.xyz[2] = fix16_sub(xyz[2], v.xyz[2]);
        return result;
    }

    Vector operator* (double n) const
    {
        Vector result;
        fix16_t n16 = fix16_from_dbl(n);
        result.xyz[0] = fix16_mul(xyz[0], n16);
        result.xyz[1] = fix16_mul(xyz[1], n16);
        result.xyz[2] = fix16_mul(xyz[2], n16);
        return result;
    }

    Vector operator* (fix16_t n) const
    {
        Vector result;
        result.xyz[0] = fix16_mul(xyz[0], n);
        result.xyz[1] = fix16_mul(xyz[1], n);
        result.xyz[2] = fix16_mul(xyz[2], n);
        return result;
    }

    Vector operator* (const Vector& v) const
    {
        Vector result;
        result.xyz[0] = fix16_mul(xyz[0], v.xyz[0]);
        result.xyz[1] = fix16_mul(xyz[1], v.xyz[1]);
        result.xyz[2] = fix16_mul(xyz[2], v.xyz[2]);
        return result;
    }

    Vector operator/ (double n) const
    {
        Vector result;
        fix16_t n16 = fix16_from_dbl(n);
        result.xyz[0] = fix16_div(xyz[0], n16);
        result.xyz[1] = fix16_div(xyz[1], n16);
        result.xyz[2] = fix16_div(xyz[2], n16);
        return result;
    }

    Vector operator/ (fix16_t n) const
    {
        Vector result;
        result.xyz[0] = fix16_div(xyz[0], n);
        result.xyz[1] = fix16_div(xyz[1], n);
        result.xyz[2] = fix16_div(xyz[2], n);
        return result;
    }

    double dot(const Vector& v) const
    {
        fix16_t result = fix16_mul(xyz[0],v.xyz[0]);
        result = fix16_add(result,fix16_mul(xyz[1],v.xyz[1]));
        result = fix16_add(result,fix16_mul(xyz[2],v.xyz[2]));

        return fix16_to_dbl(result);
    }

    Vector cross(const Vector& v) const
    {
        Vector result;

        result.xyz[0] = fix16_sub(fix16_mul(xyz[1],v.xyz[2]),fix16_mul(xyz[2],v.xyz[1]));
        result.xyz[1] = fix16_sub(fix16_mul(xyz[2],v.xyz[0]),fix16_mul(xyz[0],v.xyz[2]));
        result.xyz[2] = fix16_sub(fix16_mul(xyz[0],v.xyz[1]),fix16_mul(xyz[1],v.xyz[0]));

        return result;
    }

    double magnitude() const
    {
        return fix16_to_dbl(magnitude_f16());
    }

    fix16_t magnitude_f16() const
    {
        fix16_t value = fix16_mul(xyz[0], xyz[0]);
        value = fix16_add(value, fix16_mul(xyz[1], xyz[1]));
        value = fix16_add(value, fix16_mul(xyz[2], xyz[2])); 

        return fix16_sqrt(value);
    }

    Vector normalize() const
    {
        Vector result;
        result = (*this)/magnitude_f16();
        return result;
    }
};

class Matrix
{
public:
    fix16_t m_values[4][4];
    Matrix() {
        m_values[0][0] = m_values[1][1] = m_values[2][2] = m_values[3][3] = fix16_from_dbl(1.0);
    }

    Matrix(const fix16_t** mat) {
        memcpy(m_values, mat, 16*sizeof(fix16_t));
    }

    Matrix(const double **values) {
        for (int row = 0; row < 4 ; row++) {
            for (int col = 0; col < 4; col++) {
                m_values[row][col] = fix16_from_dbl(values[row][col]);
            }
        }
    }

    Matrix operator*(const Matrix& mat) {
        Matrix result;
        for (int row = 0 ; row < 4 ; row++) {
            for (int col = 0; col < 4 ; col++) {
                for (int i = 0 ; i < 4 ; i++) {
                    fix16_t value = 0;
                    value = fix16_add(value,fix16_mul(m_values[row][i], mat.m_values[i][col]));
                    result.m_values[row][col] = value;
                }
            }
        }

        return result;
    }
};