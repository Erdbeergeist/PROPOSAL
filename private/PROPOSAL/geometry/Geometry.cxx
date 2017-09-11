/*
 * Geometry.cxx
 *
 *  Created on: 05.06.2013
 *      Author: koehne
 */


#include <boost/algorithm/string.hpp> // case insensitive string compare for configuration file
#include <cmath>
// #include <algorithm>
// #include <vector>
#include "PROPOSAL/Constants.h"
#include "PROPOSAL/geometry/Geometry.h"
#include "PROPOSAL/Output.h"

using namespace std;
using namespace PROPOSAL;


/******************************************************************************
*                                  OStream                                    *
******************************************************************************/


namespace PROPOSAL {

ostream& operator<<(ostream& os, Geometry const& geometry)
{
    os << "--------Geometry( " << &geometry << " )--------" << endl;
    os << "\t" << geometry.name_ << endl;
    os << "\tPosition:\t" << geometry.position_ << endl;
    os << "\tHirarchy:\t" << geometry.hirarchy_ << endl;

    geometry.print(os);

    return os;
}

} // namespace PROPOSAL

/******************************************************************************
*                                  Geometry                                   *
******************************************************************************/


Geometry::Geometry(std::string name)
    : position_( Vector3D() )
    , name_(name)
    , hirarchy_(0)
{
}

Geometry::Geometry(std::string name, Vector3D position)
    : position_(100.*position)
    , name_(name)
    , hirarchy_(0)
{
}

Geometry::Geometry(const Geometry& geometry)
    : position_(geometry.position_)
    , name_(geometry.name_)
    , hirarchy_(geometry.hirarchy_)
{
}

// ------------------------------------------------------------------------- //
void Geometry::swap(Geometry& geometry)
{
    using std::swap;

    position_.swap(geometry.position_);
    name_.swap(geometry.name_);
    swap(hirarchy_, geometry.hirarchy_);
}

// ------------------------------------------------------------------------- //
Geometry& Geometry::operator=(const Geometry& geometry)
{
    if (this != &geometry) {
        position_ = geometry.position_;
        name_ = geometry.name_;
        hirarchy_ = geometry.hirarchy_;
    }

    return *this;
}

// ------------------------------------------------------------------------- //
bool Geometry::operator==(const Geometry& geometry) const
{
    if (position_ != geometry.position_)
        return false;
    else if (name_.compare(geometry.name_) != 0)
        return false;
    else if (hirarchy_ != geometry.hirarchy_)
        return false;
    else
        return this->compare(geometry);
}

// ------------------------------------------------------------------------- //
bool Geometry::operator!=(const Geometry& geometry) const
{
    return !(*this == geometry);
}

// ------------------------------------------------------------------------- //
// Member functions
// ------------------------------------------------------------------------- //

bool Geometry::IsInside(Vector3D& position, Vector3D& direction)
{
    bool is_inside = false;

    pair<double, double> dist = DistanceToBorder(position, direction);

    if (dist.first > 0 && dist.second < 0) {
        is_inside = true;
    }
    return is_inside;
}

// ------------------------------------------------------------------------- //
bool Geometry::IsInfront(Vector3D& position, Vector3D& direction)
{
    bool is_infront = false;

    pair<double, double> dist = DistanceToBorder(position, direction);

    if (dist.first > 0 && dist.second > 0) {
        is_infront = true;
    }
    return is_infront;
}

// ------------------------------------------------------------------------- //
bool Geometry::IsBehind(Vector3D& position, Vector3D& direction)
{
    bool is_behind = false;

    pair<double, double> dist = DistanceToBorder(position, direction);

    if (dist.first < 0 && dist.second < 0) {
        is_behind = true;
    }
    return is_behind;
}

// ------------------------------------------------------------------------- //
double Geometry::DistanceToClosestApproach(Vector3D& position, Vector3D& direction)
{
    return scalar_product(position_ - position, direction);
}