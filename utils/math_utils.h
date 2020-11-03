extern "C" {
    #include <math.h>
    #include <meidogte.h>
    // fixmath tries to include fix16.hpp if defined
    #undef __cplusplus
    #include <fixmath.h>
    #define __cplusplus
}

class Vector{
public:
    VECTOR v_f16;
    Vector()
    {
        v_f16.vx = v_f16.vy = v_f16.vz = 0;
    }

    Vector(double x, double y, double z)
    {
        
        v_f16.vx = fix16_from_dbl(x);
        v_f16.vy = fix16_from_dbl(y);
        v_f16.vz = fix16_from_dbl(z);
    }

    double x() {
        return fix16_to_dbl(v_f16.vx);
    }

    double y() {
        return fix16_to_dbl(v_f16.vy);
    }

    double z() {
        return fix16_to_dbl(v_f16.vz);
    }

    void x(double val) {
        v_f16.vx = fix16_from_dbl(val);
    }

    void y(double val) {
        v_f16.vy = fix16_from_dbl(val);
    }

    void z(double val) {
        v_f16.vz = fix16_from_dbl(val);
    }

    Vector operator+(const Vector& v) const
    {
        Vector result;
        result.v_f16.vx = fix16_add(v_f16.vx, v.v_f16.vx);
        result.v_f16.vy = fix16_add(v_f16.vy, v.v_f16.vy);
        result.v_f16.vz = fix16_add(v_f16.vz, v.v_f16.vz);
        return result;
    }

    Vector operator-(const Vector& v) const
    {
        Vector result;
        result.v_f16.vx = fix16_sub(v_f16.vx, v.v_f16.vx);
        result.v_f16.vy = fix16_sub(v_f16.vy, v.v_f16.vy);
        result.v_f16.vz = fix16_sub(v_f16.vz, v.v_f16.vz);
        return result;
    }

    Vector operator* (double n) const
    {
        Vector result;
        fix16_t n16 = fix16_from_dbl(n);
        result.v_f16.vx = fix16_mul(v_f16.vx, n16);
        result.v_f16.vy = fix16_mul(v_f16.vy, n16);
        result.v_f16.vz = fix16_mul(v_f16.vz, n16);
        return result;
    }

    Vector operator* (fix16_t n) const
    {
        Vector result;
        result.v_f16.vx = fix16_mul(v_f16.vx, n);
        result.v_f16.vy = fix16_mul(v_f16.vy, n);
        result.v_f16.vz = fix16_mul(v_f16.vz, n);
        return result;
    }

    Vector operator* (const Vector& v) const
    {
        Vector result;
        result.v_f16.vx = fix16_mul(v_f16.vx, v.v_f16.vx);
        result.v_f16.vy = fix16_mul(v_f16.vy, v.v_f16.vy);
        result.v_f16.vz = fix16_mul(v_f16.vz, v.v_f16.vz);
        return result;
    }

    Vector operator/ (double n) const
    {
        Vector result;
        fix16_t n16 = fix16_from_dbl(n);
        result.v_f16.vx = fix16_div(v_f16.vx, n16);
        result.v_f16.vy = fix16_div(v_f16.vy, n16);
        result.v_f16.vz = fix16_div(v_f16.vz, n16);
        return result;
    }

    Vector operator/ (fix16_t n) const
    {
        Vector result;
        result.v_f16.vx = fix16_div(v_f16.vx, n);
        result.v_f16.vy = fix16_div(v_f16.vy, n);
        result.v_f16.vz = fix16_div(v_f16.vz, n);
        return result;
    }

    double dot(const Vector& v) const
    {
        fix16_t result = fix16_mul(v_f16.vx,v.v_f16.vx);
        result = fix16_add(result,fix16_mul(v_f16.vy,v.v_f16.vy));
        result = fix16_add(result,fix16_mul(v_f16.vz,v.v_f16.vz));

        return fix16_to_dbl(result);
    }

    Vector cross(const Vector& v) const
    {
        Vector result;

        result.v_f16.vx = fix16_sub(fix16_mul(v_f16.vy,v.v_f16.vz),fix16_mul(v_f16.vz,v.v_f16.vy));
        result.v_f16.vy = fix16_sub(fix16_mul(v_f16.vz,v.v_f16.vx),fix16_mul(v_f16.vx,v.v_f16.vz));
        result.v_f16.vz = fix16_sub(fix16_mul(v_f16.vx,v.v_f16.vy),fix16_mul(v_f16.vy,v.v_f16.vx));

        return result;
    }

    double magnitude() const
    {
        return fix16_to_dbl(magnitude_f16());
    }

    fix16_t magnitude_f16() const
    {
        fix16_t value = fix16_mul(v_f16.vx, v_f16.vx);
        value = fix16_add(value, fix16_mul(v_f16.vy, v_f16.vy));
        value = fix16_add(value, fix16_mul(v_f16.vz, v_f16.vz)); 

        return fix16_sqrt(value);
    }

    Vector normalize() const
    {
        Vector result;
        result = (*this)/magnitude_f16();
        return result;
    }
};