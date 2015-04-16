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

        bool operator==(const Coordinate& other)
        {
            return (this->x == other.x) && (this->y == other.y) ;
        }
};

#endif
