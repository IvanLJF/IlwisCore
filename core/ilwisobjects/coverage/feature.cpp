#include <QStringList>
#include <QSharedPointer>
#include "kernel.h"
#include "geometries.h"
#include "polygon.h"
#include "geometry.h"
#include "ilwisdata.h"
#include "domain.h"
#include "datadefinition.h"
#include "columndefinition.h"
#include "table.h"
#include "attributerecord.h"
#include "feature.h"

using namespace Ilwis;

quint64 Feature::_idbase = 0;

SPFeatureI::SPFeatureI(FeatureInterface *f) : QSharedPointer<FeatureInterface>(f)
{
}

QVariant SPFeatureI::operator ()(const QString &name, int index) {
    return (*this)->value(name, index);
}
//--------------------------------------------
quint32 FeatureNode::itemId() const {
    if ( _feature.isNull())
        return iUNDEF;
    return _feature->itemId();
}

quint64 FeatureNode::featureid() const {
    if ( _feature.isNull())
        return i64UNDEF;
    return _feature->featureid();
}

bool FeatureNode::isValid() const{
    return true;
}

const Geometry& FeatureNode::geometry(quint32 ) const {
    return _geometry;
}

void FeatureNode::set(const Geometry& geom, int ) {
    _geometry = geom;
}

SPFeatureI FeatureNode::clone() const
{
    return QSharedPointer<FeatureNode>(new FeatureNode()) ;
}

IlwisTypes FeatureNode::ilwisType(qint32) const{
    return _geometry.ilwisType();
}

QVariant FeatureNode::cell(const QString& name, int) {

}

//--------------------------------------------
Feature::Feature() : _itemid(iUNDEF), _featureid(i64UNDEF){
}

Feature::~Feature()
{
}
Feature::Feature(const Feature &f) {
}

Feature &Feature::operator =(const Feature &f)
{
    return *this;
}

QVariant Feature::value(const QString &name, int index)
{
    return _record->cellByKey(featureid(), name, index);
}

Feature::Feature(quint32 v): _itemid(v){
    _featureid = _idbase++;
}

quint32 Feature::itemId() const {
    return _itemid;
}

void Feature::itemId(quint32 v) {
    _itemid = v;
}

quint64 Feature::featureid() const{
    return _featureid;
}


bool Feature::isValid() const {

    return _itemid != iUNDEF ;
}

const Geometry &Feature::geometry(quint32 index) const
{
    if ( index < _track.size())
        return _track[index];
    ERROR2(ERR_INVALID_PROPERTY_FOR_2,"index","geometry");

    return _invalidGeom;
}

void Feature::add(const Geometry &geom)
{
    _track.push_back(geom );
}

bool operator==(const Feature& f1, const Feature& f2) {
    return f1.featureid() == f2.featureid();
}

void Feature::attributeRecord(const SPAttributeRecord& record){
    _record = record;
}

SPFeatureI Feature::clone() const
{
    Feature *f = new Feature(itemId());
    f->_track = _track;
    f->_record = _record;

    return SPFeatureI(f);

}

IlwisTypes Feature::ilwisType(qint32 index) const
{
    if ( index != iUNDEF ) {
        if ( index < _track.size())
            return geometry(index).ilwisType();
        return itUNKNOWN;
    }
    IlwisTypes type=itUNKNOWN;
    for(const Geometry& geom : _track)
        type |= geom.ilwisType();
    return type;
}

quint32 Feature::trackSize() const
{
    return _track.size();
}

Ilwis::FeatureInterface *createFeature(quint64 ItemId) {
    return new Feature(ItemId);
}
