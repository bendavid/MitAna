//--------------------------------------------------------------------------------------------------
// $Id: EventHeader.h,v 1.7 2008/09/17 04:21:16 loizides Exp $
//
// EventHeader
//
// Class to hold event specific information.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_EVENTHEADER_H
#define DATATREE_EVENTHEADER_H
 
#include "MitAna/DataTree/interface/Types.h"
#include "MitAna/DataTree/interface/DataBase.h"

namespace mithep 
{
  class EventHeader : public DataBase
  {
    public:
      EventHeader() : fRunNum(0), fEvtNum(0), fLumiSec(0), fRunEntry(-1) {}
      EventHeader(UInt_t run, UInt_t evt, UInt_t lumi) : 
        fRunNum(run), fEvtNum(evt), fLumiSec(lumi), fRunEntry(-1)        {}
      virtual ~EventHeader() {}

      UInt_t              EvtNum()       const { return fEvtNum; }
      UInt_t              LumiSec()      const { return fLumiSec; }
      Int_t               RunEntry()     const { return fRunEntry; }
      UInt_t              RunNum()       const { return fRunNum; }
      void                SetEvtNum(UInt_t i)  { fEvtNum=i; }
      void                SetLumiSec(UInt_t i) { fLumiSec=i; }
      void                SetRunEntry(Int_t i) { fRunEntry=i; }
      void                SetRunNum(UInt_t i)  { fRunNum=i; }

    protected:
      UInt_t              fRunNum;          //run number
      UInt_t              fEvtNum;          //event number
      UInt_t              fLumiSec;         //luminosity block number
      Int_t               fRunEntry;        //entry for run block
      
    ClassDef(EventHeader, 2) // Event header class
  };
}

#endif
