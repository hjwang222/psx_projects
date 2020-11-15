#pragma once
extern "C" {
    #include <math.h>
    // fixmath tries to include fix16.hpp if defined
    #include <fixmath.h>
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

    FixedPoint(const float& val) {
        value = fix16_from_float(val);
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

    static FixedPoint tan(const FixedPoint& val) {
        FixedPoint result;

        result.value = fix16_tan(val.value);

        return result;
    }

    static FixedPoint cos(const FixedPoint& val) {
        FixedPoint result;

        result.value = fix16_cos(val.value);

        return result;
    }

    static FixedPoint sin(const FixedPoint& val) {
        FixedPoint result;

        result.value = fix16_sin(val.value);

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

    Vector(float x, float y, float z) {
        xyzw[0] = FixedPoint(x);
        xyzw[1] = FixedPoint(y);
        xyzw[2] = FixedPoint(z);
        xyzw[3] = FixedPoint(1.0);
    }

    Vector(float x, float y, float z, float w)
    {
        xyzw[0] = FixedPoint(x);
        xyzw[1] = FixedPoint(y);
        xyzw[2] = FixedPoint(z);
        xyzw[3] = FixedPoint(w);
    }

    Vector(double x, double y, double z) {
        xyzw[0] = FixedPoint(x);
        xyzw[1] = FixedPoint(y);
        xyzw[2] = FixedPoint(z);
        xyzw[3] = FixedPoint(1.0);
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
        // perspective divide
        FixedPoint x = (xyzw[0] / xyzw[3]) + FixedPoint(1);
        x = x / FixedPoint(2);
        x = x * FixedPoint(320);
        
        return fix16_to_int(x.value);
    }

    int screen_y() const
    {
        // flip vertical axis + perspective divide
        FixedPoint y = (xyzw[1] / xyzw[3]) + FixedPoint(-1);
        y = y / FixedPoint(-2);
        y = y * FixedPoint(240);
        return fix16_to_int(y.value);
    }

};

class Matrix
{
public:
    FixedPoint m_values[4][4];
    Matrix() {
        m_values[0][0] = m_values[1][1] = m_values[2][2] = m_values[3][3] = FixedPoint(1.0);
    }

    Matrix(const double **values) {
        for (int row = 0; row < 4 ; row++) {
            for (int col = 0; col < 4; col++) {
                m_values[row][col] = fix16_from_dbl(values[row][col]);
            }
        }
    }

    Matrix operator*(const Matrix& mat) const {
        Matrix result;

        for (int i = 0 ; i < 4 ; i++) {
            result.m_values[i][0] = m_values[i][0]*mat.m_values[0][0] +
                                    m_values[i][1]*mat.m_values[1][0] +
                                    m_values[i][2]*mat.m_values[2][0] +
                                    m_values[i][3]*mat.m_values[3][0];

            result.m_values[i][1] = m_values[i][0]*mat.m_values[0][1] +
                                    m_values[i][1]*mat.m_values[1][1] +
                                    m_values[i][2]*mat.m_values[2][1] +
                                    m_values[i][3]*mat.m_values[3][1];

            result.m_values[i][2] = m_values[i][0]*mat.m_values[0][2] +
                                    m_values[i][1]*mat.m_values[1][2] +
                                    m_values[i][2]*mat.m_values[2][2] +
                                    m_values[i][3]*mat.m_values[3][2];

            result.m_values[i][3] = m_values[i][0]*mat.m_values[0][3] +
                                    m_values[i][1]*mat.m_values[1][3] +
                                    m_values[i][2]*mat.m_values[2][3] +
                                    m_values[i][3]*mat.m_values[3][3];  
        }

        return result;
    }

    Vector operator*(const Vector& vec) const {
        Vector result;

        result.xyzw[0] = vec.xyzw[0]*m_values[0][0] +
                         vec.xyzw[1]*m_values[1][0] +
                         vec.xyzw[2]*m_values[2][0] +
                         vec.xyzw[3]*m_values[3][0];

        result.xyzw[1] = vec.xyzw[0]*m_values[0][1] +
                         vec.xyzw[1]*m_values[1][1] +
                         vec.xyzw[2]*m_values[2][1] +
                         vec.xyzw[3]*m_values[3][1];

        result.xyzw[2] = vec.xyzw[0]*m_values[0][2] +
                         vec.xyzw[1]*m_values[1][2] +
                         vec.xyzw[2]*m_values[2][2] +
                         vec.xyzw[3]*m_values[3][2];

        result.xyzw[3] = vec.xyzw[0]*m_values[0][3] +
                         vec.xyzw[1]*m_values[1][3] +
                         vec.xyzw[2]*m_values[2][3] +
                         vec.xyzw[3]*m_values[3][3]; 

        return result;
    }

    static Matrix perspective(double fov, double aspect, double near, double far) {
        Matrix result;

        // this is unlikely to be called too often so not using fixed point
        double fov_rad = fov*(M_PI / 180);
        double fov_tan = tan(fov_rad/2);
        
        result.m_values[0][0] = FixedPoint(1/(aspect*fov_tan));
        result.m_values[1][1] = FixedPoint(1/fov_tan);
        result.m_values[2][2] = FixedPoint(-(far + near)/(far - near));
        result.m_values[2][3] = FixedPoint(-1);
        result.m_values[3][2] = FixedPoint(-(2*far*near) / (far - near));
        result.m_values[3][3] = FixedPoint(0);

        return result;
    }

    static Matrix translate(double x, double y, double z) {
        Matrix result;

        result.m_values[0][3] = FixedPoint(x);
        result.m_values[1][3] = FixedPoint(y);
        result.m_values[2][3] = FixedPoint(z);

        return result;
    }

    static Matrix scale(double x, double y, double z) {
        Matrix result;

        result.m_values[0][0] = FixedPoint(x);
        result.m_values[1][1] = FixedPoint(y);
        result.m_values[2][2] = FixedPoint(z);

        return result;
    }

    static Matrix rotate(double x, double y, double z) {

        // x
        FixedPoint pi((double)M_PI);
        FixedPoint r180(180);
        Matrix x_rot;
        {
            FixedPoint angle(x);
            angle = angle * (pi/r180);
            x_rot.m_values[1][1] = FixedPoint::cos(angle);
            x_rot.m_values[1][2] = FixedPoint(0) - FixedPoint::sin(angle);
            x_rot.m_values[2][1] = FixedPoint::sin(angle);
            x_rot.m_values[2][2] = FixedPoint::cos(angle);
        }

        // y
        Matrix y_rot;
        {
            FixedPoint angle(y);
            angle = angle * (pi/r180);
            y_rot.m_values[0][0] = FixedPoint::cos(angle);
            y_rot.m_values[0][2] = FixedPoint::sin(angle);
            y_rot.m_values[2][0] = FixedPoint(0) - FixedPoint::sin(angle);
            y_rot.m_values[2][2] = FixedPoint::cos(angle);
        }

        // z
        Matrix z_rot;
        {
            FixedPoint angle(z);
            angle = angle * (pi/r180);
            z_rot.m_values[0][0] = FixedPoint::cos(angle);
            z_rot.m_values[0][1] = FixedPoint(0) - FixedPoint::sin(angle);
            z_rot.m_values[1][0] = FixedPoint::sin(angle);
            z_rot.m_values[1][1] = FixedPoint::cos(angle);
        }

        return x_rot*y_rot*z_rot;
    }
};