// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/

#ifndef WAC_Factory
#define WAC_Factory

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// Factory                                                             //
//                                                                     //
// Generic base class for factory services.
// Does not own a store                                  //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TObject.h"

using namespace std;

template < class T >
class Factory : public TObject
{
protected:
  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  long capacity;
  long index;
  T ** objects;

  public:
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////
  Factory()
  :
  TObject(),
  capacity(0),
  index(0),
  objects(0)
  {
   // no ops
  }
  
  virtual ~Factory()
  {
  delete[] objects;
  }

  void initialize(int initialCapacity)
  {
  if (objects) delete[] objects;
    capacity = initialCapacity;
    objects = new T*[capacity];
    for (int k=0; k<capacity; k++)
      {
      objects[k] = new T();
      }
  }

  void reset()
  {
  index = 0;
  }

  // Returns the capacity of this store/factory
  long getCapacity() const
  {
    return capacity;
  }

  // Returns the number of object currently
  // in use.
  long getCurrentSize() const
  {
    return index;
  }

  long getLastIndex() const
  {
  return index-1;
  }

  T * getNextObject()
  {
  if (index<capacity)
    {
    return objects[index++];
    }
  else
    {
    T** keep = objects;
    long newCapacity = 2*capacity;
    objects = new T*[newCapacity];
    if (!objects)
      {
      cout << "<F> Factory::getObject() Unable to allocate new capacity : " << newCapacity << endl;
      cout << "<F> Factory::getObject() Abort. " << newCapacity << endl;
      exit(1);
      }
    for (long k=0; k<capacity; k++) objects[k] = keep[k];
    for (long k=capacity; k<newCapacity; k++) objects[k] = new T();
    delete[] keep;
    capacity = newCapacity;
    cout << "<W> Factory::getObject() Object capacity increased to : " << capacity << endl;
    return objects[index++];
    }
  }

//  T * getObjectAt(unsigned long index)
//  {
//  if (index>=0 && index<capacity)
//     {
//     return objects[index];
//     }
//   else
//     {
//     cout << "<FATAL> Factory::getObjectAt() Object stored is exceeded" << endl;
//     return 0;
//     }
//  }

  ClassDef(Factory,0)
};

#endif /* Factory_hpp */
