#ifndef COORDINATE_H
#define COORDINATE_H

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

};

#endif
