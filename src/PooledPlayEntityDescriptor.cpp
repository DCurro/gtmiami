#include "PooledPlayEntityDescriptor.hpp"

PooledPlayEntityDescriptor::PooledPlayEntityDescriptor(int uniqueReferenceId,
                                                       int floor,
                                                       float x,
                                                       float y,
                                                       float angle)
{
    this->mUniqueReferenceId = uniqueReferenceId;
    this->mFloor = floor;
    this->mX = x;
    this->mY = y;
    this->mAngle = angle;
}

PooledPlayEntityDescriptor::~PooledPlayEntityDescriptor() {
    
}
