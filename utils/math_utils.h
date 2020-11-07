extern "C" {
    #include <math.h>
    // fixmath tries to include fix16.hpp if defined
    #undef __cplusplus
    #include <fixmath.h>
    #define __cplusplus
}

class FixedPoint
{
public:
    fix16_t value = 0;

    FixedPoint() {
        value = 0;
    }

    FixedPoint(const int& val) {
        value = fix16_from_int(val);
    }

    FixedPoint(const double& val) {
        value = fix16_from_dbl(val);
    }

    FixedPoint operator=(const int& val) {
        value = fix16_from_int(val);
        return *this;
    }

    FixedPoint operator=(const double& val) {
        value = fix16_from_dbl(val);
        return *this;
    }

    FixedPoint operator=(const FixedPoint& val) {
        value = val.value;
        return *this;
    }

    FixedPoint operator+(const FixedPoint& val) const {
        FixedPoint result;
        result.value = fix16_add(value, val.value);
        return result;
    }

    FixedPoint operator-(const FixedPoint& val) const {
        FixedPoint result;
        result.value = fix16_sub(value, val.value);
        return result;
    }

    FixedPoint operator*(const FixedPoint& val) const {
        FixedPoint result;
        result.value = fix16_mul(value, val.value);
        return result;
    }

    FixedPoint operator/(const FixedPoint& val) const {
        FixedPoint result;
        result.value = fix16_div(value, val.value);
        return result;
    }
};

class Vector{
public:
    FixedPoint xyzw[4];
    Vector()
    {
        xyzw[0] = xyzw[1] = xyzw[2] = xyzw[3] = FixedPoint(0);
    }

    Vector(double x, double y, double z, double w)
    {
        xyzw[0] = FixedPoint(x);
        xyzw[1] = FixedPoint(y);
        xyzw[2] = FixedPoint(z);
        xyzw[3] = FixedPoint(w);
    }

    Vector operator+(const Vector& v) const
    {
        Vector result;
        result.xyzw[0] = xyzw[0] + v.xyzw[0];
        result.xyzw[1] = xyzw[1] + v.xyzw[1];
        result.xyzw[2] = xyzw[2] + v.xyzw[2];
        return result;
    }

    Vector operator-(const Vector& v) const
    {
        Vector result;
        result.xyzw[0] = xyzw[0] - v.xyzw[0];
        result.xyzw[1] = xyzw[1] - v.xyzw[1];
        result.xyzw[2] = xyzw[2] - v.xyzw[2];
        return result;
    }

    Vector operator* (double n) const
    {
        return operator*(FixedPoint(n));
    }

    Vector operator* (FixedPoint n) const
    {
        Vector result;
        result.xyzw[0] = xyzw[0] * n;
        result.xyzw[1] = xyzw[1] * n;
        result.xyzw[2] = xyzw[2] * n;
        return result;
    }

    Vector operator* (const Vector& v) const
    {
        Vector result;
        result.xyzw[0] = xyzw[0] * v.xyzw[0];
        result.xyzw[1] = xyzw[1] * v.xyzw[1];
        result.xyzw[2] = xyzw[2] * v.xyzw[2];
        return result;
    }

    Vector operator/ (double n) const
    {
        return operator/(FixedPoint(n));
    }

    Vector operator/ (FixedPoint n) const
    {
        Vector result;
        result.xyzw[0] = xyzw[0] / n;
        result.xyzw[1] = xyzw[1] / n;
        result.xyzw[2] = xyzw[2] / n;
        return result;
    }

    double dot(const Vector& v) const
    {
        FixedPoint result = xyzw[0] * v.xyzw[0];
        result = result + (xyzw[1] * v.xyzw[1]);
        result = result + (xyzw[2] * v.xyzw[2]);
        result = result + (xyzw[3] * v.xyzw[3]);

        return fix16_to_dbl(result.value);
    }

    Vector cross(const Vector& v) const
    {
        Vector result;

        result.xyzw[0] = (xyzw[1] * v.xyzw[2]) - (xyzw[2] * v.xyzw[1]);
        result.xyzw[1] = (xyzw[2] * v.xyzw[0]) - (xyzw[0] * v.xyzw[2]);
        result.xyzw[2] = (xyzw[0] * v.xyzw[1]) - (xyzw[1] * v.xyzw[0]);

        return result;
    }

    double magnitude() const
    {
        return fix16_to_dbl(magnitude_f16());
    }

    fix16_t magnitude_f16() const
    {
        FixedPoint value = xyzw[0] * xyzw[0];
        value = value + (xyzw[1] * xyzw[1]);
        value = value + (xyzw[2] * xyzw[2]); 

        return fix16_sqrt(value.value);
    }

    Vector normalize() const
    {
        Vector result;
        result = (*this)/magnitude_f16();
        return result;
    }

    int screen_x() const
    {
        FixedPoint x = xyzw[0] + FixedPoint(1);
        x = x * FixedPoint(320);
        x = x / FixedPoint(2);
        
        return fix16_to_int(x.value);
    }

    int screen_y() const
    {
        // flip vertical axis
        FixedPoint y = xyzw[1] + FixedPoint(-1);
        y = y * FixedPoint(240);
        y = y / FixedPoint(-2);
        return fix16_to_int(y.value);
    }

};

class Matrix
{
public:
    FixedPoint m_values[4][4];
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

    double m(int row, int col) {
        return fix16_to_dbl(m_values[row][col].value);
    }

    void m(int row, int col, double val) {
        m_values[row][col] = fix16_from_dbl(val);
    }

    Matrix operator*(const Matrix& mat) {
        Matrix result;
        for (int row = 0 ; row < 4 ; row++) {
            for (int col = 0; col < 4 ; col++) {
                FixedPoint value;
                for (int i = 0 ; i < 4 ; i++) {
                    value = value + m_values[row][i] * mat.m_values[i][col];
                    result.m_values[row][col] = value;
                }
            }
        }
        return result;
    }

    Vector operator*(const Vector& vec) {
        Vector result;
        
        for (int row = 0 ; row < 4 ; row++) {
            for (int col = 0 ; col < 4 ; col++) {
                result.xyzw[col] = vec.xyzw[col] * m_values[row][col];
            }
        }

        return result;
    }

    static Matrix perspective(double fov, double aspect, double near, double far) {
        Matrix result;

        // this is unlikely to be called too often so not using fixed point
        double fov_rad = M_PI / 180;
        double fov_tan = tan(fov * (M_PI/180));
        
        result.m(0,0, aspect*fov_tan);
        result.m(1,1, 1/fov_tan);
        result.m(2,2, (far + near)/(far - near));
        result.m(2,3, -1);
        result.m(3,2, (-2*far*near) / (far - near));
        result.m(3,3, 0);

        return result;
    }

    static Matrix scale(double x, double y, double z) {
        Matrix result;

        result.m_values[0][0] = fix16_from_dbl(x);
        result.m_values[1][1] = fix16_from_dbl(y);
        result.m_values[2][2] = fix16_from_dbl(z);

        return result;
    }
};