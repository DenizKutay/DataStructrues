#include <cmath>
#include "Sector.h"

using namespace std;
// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    double distance = std::sqrt((x * x) + (y * y) + (z * z));
    distance_from_earth = distance;
    sector_code = createSectorCode(this->x, this->y, this->z);
}

Sector::~Sector()
{
    // TODO: Free any dynamically allocated memory if necessary
}

Sector &Sector::operator=(const Sector &other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->sector_code = other.sector_code;
    this->distance_from_earth = other.distance_from_earth;
    this->left = other.left;
    this->right = other.right;
    this->color = other.color;
    this->parent = other.parent;
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector &other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector &other) const {
    return !(*this == other);
}

std::string Sector::createSectorCode(int x, int y, int z) {
    string name;
    int distance = std::sqrt((x * x) + (y * y) + (z * z));
    name += std::to_string(distance);
    string coordinate;
    if (x == 0) {
        coordinate += "S";
    } else if (x < 0) {
        coordinate += "L";
    } else {
        coordinate += "R";
    }

    if (y == 0) {
        coordinate += "S";
    } else if (y < 0) {
        coordinate += "D";
    } else {
        coordinate += "U";
    }

    if (z == 0) {
        coordinate += "S";
    } else if (z < 0) {
        coordinate += "B";
    } else {
        coordinate += "F";
    }
    name += coordinate;
    // TODO: Calculate the distance to the Earth, and generate the sector code
    return name;
}

bool Sector::isGraterThanSector(int x, int y, int z) {
    if(this-> x > x) {return true;}
    else if(this->x == x) {
        if(this -> y > y) {return true;}
        else if(this->y == y) {
            if(this->z > z) {return true;}
            else{return false;}
        }else {return false;}
    } else {return false;}
}

