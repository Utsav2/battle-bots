#ifndef COORDINATE_H
#define COORDINATE_H

#include <assert.h>

class Coordinate
{
    public:
        int x;
        int y;

        Coordinate(int x,int y)
        {
            this->x = x;
            this->y = y;
        }

        bool operator==(const Coordinate& other) const
        {
            return (this->x == other.x) && (this->y == other.y) ;
        }

        bool operator!=(const Coordinate& other) const
        {
            return !(*this == other); 
        }

        bool operator<(const Coordinate& other) const
        {
            if (this->y == other.y)
            {
                return this->x < other.x;
            }
            else
            {
                return this->y < other.y;
            }
        }

        Coordinate operator-(const Coordinate & rhs)
		{
			return Coordinate(this->x - rhs.x, this->y - rhs.y);
		}

		Coordinate operator+(const Coordinate & rhs)
		{
			return Coordinate(this->x + rhs.x, this->y + rhs.y);
		}

		Coordinate operator/(int factor)
		{
			assert (factor != 0);
			return Coordinate(this->x / factor, this->y / factor);
		}

        Coordinate& operator-=(const Coordinate & rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}

		Coordinate& operator+=(const Coordinate & rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}

		Coordinate& operator/=(const int & denominator)
		{
			assert(denominator != 0);
			this->x /= denominator;
			this->y /= denominator;
			return *this;
		}

};

#endif
