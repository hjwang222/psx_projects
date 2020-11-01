extern "C" {
    #include <math.h>
}

class Vector{
    // public
    double x,y,z;
    Vector()
    {
        x = y = z = 0;
    }

    Vector(double _x, double _y, double _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector operator+(const Vector& v)
    {
        Vector result;
        result.x = x + v.x;
        result.y = y + v.y;
        result.z = z + v.z;
        return result;
    }

    Vector operator-(const Vector& v)
    {
        Vector result;
        result.x = x - v.x;
        result.y = y - v.y;
        result.z = z - v.z;
        return result;
    }

    Vector operator* (double n)
    {
        Vector result;
        result.x = x * n;
        result.y = y * n;
        result.z = z * n;
        return result;
    }

    Vector operator/ (double n)
    {
        Vector result;
        result.x = x / n;
        result.y = y / n;
        result.z = z / n;
        return result;
    }

    double dot(const Vector& v)
    {
        return x*v.x + y*v.y + z*v.z;
    }

    Vector cross(const Vector& v)
    {
        Vector result;

        result.x = y*v.z - z*v.y;
        result.y = z*v.x - x*v.z;
        result.z = x*v.y - y*v.x;

        return result;
    }

    double magnitude()
    {
        return sqrt(x*x + y*y + z*z);
    }

    Vector normalize()
    {
        Vector result;
        result = (*this)/magnitude();
        return result;
    }
};