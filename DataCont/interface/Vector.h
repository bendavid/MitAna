//--------------------------------------------------------------------------------------------------
// $Id: Vector.h,v 1.14 2008/07/14 20:55:19 loizides Exp $
//
// Vector
//
// Implementation of Collection interface using std:vector class. 
//
// Note, in case you one to store pointers to allocated objects on heap, 
// you should use the class PVector.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_VECTOR_H
#define DATATREE_VECTOR_H
 
#include <vector>
#include <Rtypes.h>
#include "MitAna/DataCont/interface/Collection.h"

namespace mithep 
{
  template<class ArrayElement>
  class Vector : public Collection<ArrayElement>
  {
    public:
      Vector(UInt_t rsv=0) : fV(0) { fV.reserve(rsv); }
      Vector(std::vector<ArrayElement> &v) : fV(v) {}
      ~Vector() {}

      void                             AddCopy(const ArrayElement &ae)     { fV.push_back(ae); }
      void                             AddCopy(const ArrayElement *ae)     { fV.push_back(*ae); }
      ArrayElement                    *At(UInt_t idx)                      { return &fV.at(idx); }
      const ArrayElement              *At(UInt_t idx)                const { return &fV.at(idx); }
      void                             Clear(Option_t *opt="");
      UInt_t                           Entries()                     const { return fV.size(); }
      UInt_t                           GetEntries()                  const { return fV.size(); }
      Bool_t                           IsOwner()                     const { return kTRUE; }
      ArrayElement                    &Ref(UInt_t idx)                     { return fV.at(idx); }
      const ArrayElement              &Ref(UInt_t idx)               const { return fV.at(idx); }
      void                             Reset()                             { fV.clear(); }
      void                             Trim();
      ArrayElement                    *UncheckedAt(UInt_t idx)             { return &fV[idx]; }
      const ArrayElement              *UncheckedAt(UInt_t idx)       const { return &fV[idx]; }
      const std::vector<ArrayElement> &Vect()                        const { return fV; }
      std::vector<ArrayElement>       &Vect()                              { return fV; }

      ArrayElement                    *operator[](UInt_t idx)              { return &fV.at(idx); }
      const ArrayElement              *operator[](UInt_t idx)        const { return &fV.at(idx); }

    protected:
      std::vector<ArrayElement>        fV; //std::vector

    private:
      Vector(const Vector &v);

    ClassDefT(Vector, 1) // Wrapper around std::vector class
  };
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline void mithep::Vector<ArrayElement>::Clear(Option_t */*opt*/)
{
  // Destruct container.

  using namespace std;
  fV.~vector<ArrayElement>();
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline void mithep::Vector<ArrayElement>::Trim()
{
  // Trim vector to minimal needed size.

  std::vector<ArrayElement>(fV).swap(fV);
}
#endif