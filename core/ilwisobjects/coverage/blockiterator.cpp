#include "kernel.h"
#include "raster.h"
#include "pixeliterator.h"
#include "blockiterator.h"

using namespace Ilwis;

CellIterator::CellIterator(GridBlock *bl, bool end) :
    _block(bl),
    _positionx(end ? _block->size().xsize() : 0),
    _positiony(end ? _block->size().ysize() : 0),
    _positionz(end ? _block->size().zsize() : 0)
{

}

CellIterator &CellIterator::operator=(CellIterator &iter)
{
    _block = iter._block;
    _positionx = iter._positionx;
    _positiony = iter._positiony;
    _positionz = iter._positionz;

    return *this;
}

CellIterator CellIterator::operator++(int)
{
    CellIterator iter = *this;
    move(1);
    return iter;
}

CellIterator CellIterator::operator--(int)
{
    CellIterator iter = *this;
    move(-1);
    return iter;
}

void CellIterator::move(int n){
    const Size& sz = _block->size();
    if ( _positionx < sz.xsize() - 1)
        ++_positionx;
    else if ( _positiony < sz.ysize() - 1) {
        ++_positiony;
        _positionx = 0;
    } else if ( _positionz < sz.zsize() - 1) {
        ++_positionz;
        _positionx = _positiony = 0;
    } else {
        _positionx = sz.xsize();
        _positiony = sz.ysize();
        _positionz = sz.zsize();
    }
}

double &CellIterator::operator*()
{
    double &v =  (*_block)(_positionx,_positiony,_positionz);
    if ( v == 156) {
        qDebug() << "s2";
    }
    return v;
}

//qint32 CellIterator::position() const
//{
//    return _position;
//}

Size CellIterator::blocksize() const
{
    return _block->size();
}

//--------------------------------------------------------
GridBlock::GridBlock(BlockIterator& iter) :
    _iterator(iter)
{
    // for efficiency the blocks and offsets are precalculated at the cost of some memory
    // when calculating the linear postions only very basic operations are needed then
    int ysize = iter._raster->size().ysize();
    _blockYSize = iter._raster->_grid->maxLines();
    _blockXSize = iter._raster->_grid->size().xsize();
    _XYSize = iter._raster->_grid->size().xsize() * iter._raster->_grid->size().ysize();
    _internalBlockNumber.resize(ysize);
    _offsets.resize(ysize);
    qint32 base = 0;
    for(int i=0; i < ysize; ++i ) {
        if ( i % _blockYSize == 0)
            base = 0;
        _internalBlockNumber[i] =   i / _blockYSize;
        _offsets[i] = base;
        base += _blockXSize;
    }
    _bandOffset = iter._raster->_grid->blocksPerBand();
}

double& GridBlock::operator ()(quint32 x, quint32 y, quint32 z)
{
    if ( _iterator._outside != rILLEGAL) {
        _iterator._outside = rILLEGAL;
    }

    quint32 zbase = _iterator._z + z;
    qint32 ypos = _iterator._y + y;
    double &v = _iterator._raster->_grid->value(_internalBlockNumber[ypos ] + _bandOffset * zbase, _offsets[ypos] +  _iterator._x +  x);
    return v;

}

Size GridBlock::size() const
{
    return _iterator.blockSize();
}

CellIterator GridBlock::begin()
{
    return CellIterator(this, false);
}

CellIterator GridBlock::end()
{
    return CellIterator(this, true);
}

const BlockIterator &GridBlock::iterator() const
{
    return _iterator;
}

//----------------------------------------------------------------------------------------------
BlockIterator::BlockIterator(IGridCoverage raster, const Size &sz, const Box3D<> &box) :
    PixelIterator(raster,box),
    _block(*this),
    _blocksize(sz)

{
}

BlockIterator::BlockIterator(quint64 endpos) : PixelIterator(endpos), _block(*this)
{

}

BlockIterator& BlockIterator::operator ++()
{
    quint32 dist = _blocksize.xsize();
    if ( _y + dist - 1 > _endy) {
        dist = linearPosition() + dist + 1;
    } else {
        if ( _x + dist >= _endx) {
            dist = 1 + _endx - _x + ( _box.xlength()) * ( _block.size().ysize() - 1);
        }

    }
    move(dist);
    if ( zchanged()) {
        dist = _box.xlength() * _box.ylength() * (_blocksize.zsize() - 1);
        move(dist);
    }

    return *this;

}

BlockIterator &BlockIterator::operator --()
{
    move(-_blocksize.xsize());
    return *this;
}

BlockIterator BlockIterator::end() const
{
    return BlockIterator(_endpositionid);
}

bool BlockIterator::operator==(const BlockIterator &iter) const
{
    return PixelIterator::operator ==(iter);
}

bool BlockIterator::operator!=(const BlockIterator& iter) const{
    return ! operator ==(iter);
}

Size BlockIterator::blockSize() const
{
    return _blocksize;
}



