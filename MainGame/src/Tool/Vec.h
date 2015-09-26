// stolen (sorry... ) from snipplr.com/view/18098/2d-vector-class/

/*
	A quick hint for future revisions:

	Sqrt is a very expensive function in terms of processing time.
	It is more efficient to just divide x^2 and y^2 by mag^2 when
	normalising

    Collision(float x, float y, float ex, float ey, int width, int height)

    Remember to not use square root, when calculating the distance.
    Calculate (x - ex)*(x - ex) + (y - ey)*(y - ey) and check it with the 
    squared critical distance (the sum of the radii of the balls). 
    Squaring is faster than taking square roots.
*/

#pragma once

#include <math.h>

using namespace std;

class Vec2D {
public:
	Vec2D(float X = 0, float Y = 0)
	{
		x = X;
		y = Y;
	}

	~Vec2D() {};

	float x, y;

	Vec2D operator*(float scalar) const
	{
		return Vec2D(x * scalar, y * scalar);
	}

	Vec2D operator+(const Vec2D &vect) const
	{
		return Vec2D(x + vect.x, y + vect.y);
	}

	Vec2D operator-(const Vec2D &vect) const
	{
		return Vec2D(x - vect.x, y - vect.y);
	}

	/*
	Ussage is as follows:

		testVec.rotate( 90 * (3.14159265 / 180));

	We always have to convert deg to rad to do this, or
	provide rad angles rightaway if we know them beforehand
	*/
	void rotate(float angle)
	{
		float xt = (x * cosf(angle)) - (y * sinf(angle));
		float yt = (y * cosf(angle)) + (x * sinf(angle));
		x = xt;
		y = yt;
	}

	float crossproduct(const Vec2D &vect2) const
	{
		return (this->x * vect2.y) - (this->y * vect2.x);
	}

	float magnitude()
	{
		return sqrtf((x * x) + (y * y));
	}

	void normalise()
	{
		float mag = sqrtf(x * x + y * y);
		this->x = x / mag;
		this->y = y / mag;
	}

	float dotproduct(const Vec2D &vect) const
	{
		return (x * vect.x) + (y * vect.y);
	}

};

