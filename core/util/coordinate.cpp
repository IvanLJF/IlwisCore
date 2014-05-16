#include <QStringList>
#include "kernel_global.h"
#include "ilwis.h"
#include "geos/geom/Coordinate.h"
#ifdef Q_OS_WIN
#include "geos/geom/Coordinate.inl"
#endif
#include "coordinate.h"
#include <cmath>

using namespace Ilwis;

const double crdDeltaLim = 1e-8;

Ilwis::Coordinate::Coordinate() : geos::geom::Coordinate(rUNDEF, rUNDEF, rUNDEF)
{
}

Ilwis::Coordinate::Coordinate(const geos::geom::Coordinate& crd) : geos::geom::Coordinate(crd){
    if (std::isnan(this->z))
        this->z = rUNDEF;
}

Ilwis::Coordinate::Coordinate(const Ilwis::Coordinate& crd) : geos::geom::Coordinate(crd.x,crd.y,crd.z){

}

Ilwis::Coordinate::Coordinate(double px, double py, double pz) : geos::geom::Coordinate(px,py,pz){
}

Ilwis::Coordinate::Coordinate(const std::vector<double>& v)  {
    if ( v.size() < 2) {
        *this = Ilwis::Coordinate();
        return;
    }
    this->x =v[0];
    this->y =v[1];
    if ( v.size() >= 3)
        this->z = v[2];

 }

Ilwis::Coordinate::~Coordinate() {
}

/*!
 a point is valid if both its x and y have a valid value;
 * \return
 */
bool Ilwis::Coordinate::isValid() const{
    return x != undefined() && y != undefined() ;
}

bool Ilwis::Coordinate::is0() const{
    return x == 0 && y == 0 ;
}

bool Ilwis::Coordinate::is3D() const {
    if (!isValid())
        return false;
    return z != undefined();
}

Ilwis::Coordinate::operator std::vector<double> () {

    std::vector<double> v;
    if ( !isValid())
        return v;

    if ( is3D()) {
         v = {this->x, this->y, this->z};
        return v;
    }
    v = {this->x, this->y};
    return v;
}

Ilwis::Coordinate& Ilwis::Coordinate::operator=(const Ilwis::Coordinate& p2) {
    this->x = p2.x;
    this->y = p2.y;
    this->z = p2.z;
    return *this;
}

Ilwis::Coordinate& Ilwis::Coordinate::operator=(const Coordinate&& p2) {
    this->x = p2.x;
    this->y = p2.y;
    this->z = p2.z;
    return *this;
}

/*!
 operator += addes a vector of 2 values to a point shifting it in 2D space. Using undefined() values in the vector may lead to unpredictable results
 * \param a vector with a pair of values
 * \return a reference to a shifted 2D point
 */
Coordinate& Ilwis::Coordinate::operator+= (const std::vector<double> &vec){
    if (!this->isValid() || vec.size() < 2){
        *this = Ilwis::Coordinate();
        return *this;
    }

    this->x =this->x + vec[0];
    this->y =this->y + vec[1];
    if ( vec.size() >= 3 && z != undefined())
        this->z =this->z + vec[2];

    return *this;
}

Coordinate& Ilwis::Coordinate::operator-= (const std::vector<double>& vec){
    if (!this->isValid() || vec.size() < 2){
        *this = Ilwis::Coordinate();
        return *this;
    }
    this->x =this->x - vec[0];
    this->y =this->y - vec[1];
    if ( vec.size() >= 3 && z != undefined())
        this->z =this->z - vec[2];

    return *this;
}

double Ilwis::Coordinate::distance(const Coordinate& crd) {
    if ( !crd.isValid() || !this->isValid())
        return this->undefined();
    if ( z == undefined() || crd.z == undefined())
        return std::sqrt(std::pow(abs(this->x - crd.x),2) + std::pow(abs(this->y - crd.y),2));
    return std::sqrt(std::pow(abs(this->x - crd.x),2) + std::pow(abs(this->y - crd.y),2) + std::pow(abs(this->z - crd.z),2));
}

/*!
 operator *= multiplies a point with a factor contained in the vector, shifting it in 2D space. Using undefined() values in the vector may lead to unpredictable results
 * \param  a vector with a pair of values
 * \return a reference to a shifted 2D point
 */
Ilwis::Coordinate& Ilwis::Coordinate::operator*=(const std::vector<double>& vec){
    if (!this->isValid() || vec.size() < 2){
        *this = Ilwis::Coordinate();
        return *this;
    }
    this->x =this->x * vec[0];
    this->y =this->y * vec[1];
    if ( vec.size() >= 3 && z != undefined())
        this->z =this->z * vec[2];

    return *this;
}

/*!
 operator *= multipliesthe x,y values with a single factor , shifting it in 2D space. Using undefined() values may lead to unpredictable results
 * \param  multiplier
 * \return a reference to a shifted 2D point
 */
Ilwis::Coordinate& Ilwis::Coordinate::operator*=(double v){
    if (!this->isValid()){
        *this = Ilwis::Coordinate();
        return *this;
    }
    this->x =this->x * v;
    this->y =this->y * v;
    if ( z != undefined())
        this->z =this->z * v;

    return *this;
}

/*!
 operator /= divides the coordinates of a point with a fixed value. The value must not be zero. If it is zero nothing happens
 Using undefined() values may lead to unpredictable results
 * \param v
 * \return
 */
Ilwis::Coordinate& Ilwis::Coordinate::operator/=(double v){
    if (!this->isValid() || v == 0){
        *this = Ilwis::Coordinate();
        return *this;
    }
    this->x =this->x / v;
    this->y =this->y / v;
    if ( z != undefined())
        this->z =this->z / v;

    return *this;
}

/*!
 compares the coordinates of 2 points to see if they are equal. comparision with non valid points always leads to false
 * \param pnt, point to be compared
 * \return true if the points are at the same Coordinate.
 */
bool Ilwis::Coordinate::operator==(const Ilwis::Coordinate& pnt) const {
    if ( !pnt.isValid() && !this->isValid())
        return true;

    if (!this->isValid() || !pnt.isValid())
        return false;

    return (std::abs(pnt.x -  this->x) < crdDeltaLim) && (std::abs(pnt.y - this->y) < crdDeltaLim) && (std::abs(pnt.z - this->z)<crdDeltaLim);
}

bool Coordinate::operator ==(const geos::geom::Coordinate &pnt) const{
    if (!this->isValid() && (pnt.x == rUNDEF || pnt.y == rUNDEF || std::isnan(pnt.x) || std::isnan(pnt.y)))
        return true;

    return (std::abs(pnt.x -  this->x) < crdDeltaLim) && (std::abs(pnt.y - this->y) < crdDeltaLim) && ((std::abs(pnt.z - this->z)<crdDeltaLim) || (std::isnan(pnt.z) && this->z == rUNDEF));
}
/*!
 compares the coordinates of 2 points to see if they are not equal. comparision with non valid points always leads to true
 * \param pnt, point to be compared
 * \return true if the points are not at the same Coordinate.
 */
bool Ilwis::Coordinate::operator!=(const Ilwis::Coordinate& pnt){
    return !(operator==(pnt));
}

Coordinate& Coordinate::operator =(const geos::geom::Coordinate& crd){
    this->x = crd.x;
    this->y = crd.y;
    if (std::isnan(crd.z))
        this->z = rUNDEF;
    else
        this->z = crd.z;
    return *this;
}

//-----------------------------------------------
std::vector<double> Ilwis::operator-(const Ilwis::Coordinate& p1, const Ilwis::Coordinate& p2) {
    if (!(p1.isValid() && p2.isValid()))
        return std::vector<double>();
    std::vector<double> v(3,0);
    v[0] = p1.x - p2.x;
    v[1] = p1.y - p2.y ;
    if ( p1.z != p1.undefined() && p2.z != p2.undefined())
        v[2] = p1.z - p2.z ;
    return v;
}

Ilwis::Coordinate Ilwis::operator+(const Ilwis::Coordinate& p1, const std::vector<double>& vec) {
    if (p1.isValid() == false ||  vec.size() < 2 )
        return Ilwis::Coordinate();
    Ilwis::Coordinate p3 = p1;
    p3.x =p1.x + vec[0];
    p3.y = p1.y + vec[1] ;
    if ( vec.size() >= 3 && p1.z != p1.undefined())
        p3.z = p1.z + vec[2];

    return p3;
}

Ilwis::Coordinate Ilwis::operator-(const Ilwis::Coordinate& p1, const std::vector<double>& vec) {
    if (p1.isValid() == false ||  vec.size() < 2 )
        return Ilwis::Coordinate();
    Ilwis::Coordinate p3 = p1;
    p3.x =p1.x - vec[0];
    p3.y = p1.y - vec[1] ;

    if ( vec.size() >= 3 && p1.z != p1.undefined())
        p3.z = p1.z - vec[2];
    return p3;
}

Ilwis::Coordinate Ilwis::operator*(const Ilwis::Coordinate& p1, double v) {
    if (p1.isValid() == false || v == rUNDEF )
        return Ilwis::Coordinate();
    Ilwis::Coordinate p3 = p1;
    p3.x =p1.x * v;
    p3.y = p1.y * v ;
    if ( p1.z != p1.undefined())
        p3.z = p3.z * v;
    return p3;
}

Ilwis::Coordinate Ilwis::operator/(const Ilwis::Coordinate& p1, double v) {
    if (!p1.isValid() || v == 0)
        return Ilwis::Coordinate();
    Ilwis::Coordinate p3 = p1;
    p3.x =p1.x / v;
    p3.y = p1.y / v ;
    if ( p1.z != p1.undefined())
        p3.z = p3.z / v;
    return p3;
}

//---------------------------------------------------------------------------
LatLon::LatLon(){

}

LatLon::LatLon(const Angle &latd, const Angle &lond, double h){
    lat(latd);
    lon(lond);
    z= h;
}

Angle LatLon::lat() const{
    return y;
}
Angle LatLon::lon() const{
    return x;
}

void LatLon::lat(const Angle& val){
    y = val.degrees();
}

void LatLon::lon(const Angle& val){
    x = val.degrees();
}
