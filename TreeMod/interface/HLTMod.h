//--------------------------------------------------------------------------------------------------
// $Id: BaseMod.h,v 1.7 2008/09/10 03:33:28 loizides Exp $
//
// HLTMod
//
// This module allows to select events according to given HLT trigger bits. The trigger bits 
// are selected via their corresponding HLT trigger name. 
// The trigger name is added to the list of accepted triggers using AddTrigger(). Each member of 
// this list will be logically "ored" to the search mask (see description of AddTrigger()). 
// For every accepted event the list of trigger objects will be published. The name of the 
// published objects can be specified via SetTrigObjsName. 
// The objects can be retrieved in sub-modules with BaseModule::GetHLTObjects(). 
// HLTMod will abort processing the chain of sub-modules if the trigger bits did not 
// match the given trigger mask, unless you call SetAbortIfNotAccepted(kFALSE). 
// For convenience HLTMod defines two virtual functions, OnAccepted() and OnFailed() 
// that can be implemented in derived classes. OnAccepted() will be called if the event passes
// the trigger mask, on Failed() if it did not.
//
// An example of how this is supposed to work can be found in 
// $CMSSW_BASE/src/MitAna/macros/examples/runHLTExample.C
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_TREEMOD_HLTMOD_H
#define MITANA_TREEMOD_HLTMOD_H

#include <string>
#include <TString.h>
#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/Collections.h"

namespace mithep 
{
  class TriggerTable;
  class TriggerObjectsTable;

  class HLTMod : public BaseMod {
    public:
      HLTMod(const char *name="HLTMod", const char *title="High-level trigger module");
      ~HLTMod();

      void                        AddTrigger(const char *expr);
      Int_t                       GetNEvents()   const { return fNEvents; }
      Int_t                       GetNAccepted() const { return fNAcceped; }
      Int_t                       GetNFailed()   const { return fNFailed; }
      void                        SetAbortIfNotAccepted(Bool_t b)   { fAbort = b; }
      void                        SetTrigObjsName(const char *n)    { fMyObjsNamePub = n; }
    protected:
      void                        AddTrigObjs(UInt_t tid);
      void                        BeginRun();
      virtual void                OnAccepted()  {/*could be implemented in derived classes*/}
      virtual void                OnFailed()    {/*could be implemented in derived classes*/}
      void                        Process();
      void                        SlaveBegin();

      Bool_t                      fAbort;         //=true then abort (sub-)modules if not accepted
      TString                     fBitsName;      //trigger bits branch name
      TString                     fMyObjsNamePub; //name of exported trigger object array
      Vector<std::string>         fTrigNames;     //trigger names requested for test mask
      BitMask256                 *fBits;          //!trigger bits branch
      Vector<BitMask256>          fTrigBitsAnd;   //!trigger bits used in mask
      Vector<BitMask256>          fTrigBitsCmp;   //!trigger bits used for comparison
      BitMask256                  fBitsDone;      //!bits for which trigger objects are copied
      TriggerObjectOArr          *fMyTrgObjs;     //!exported published trigger object array
      const TriggerTable         *fTriggers;      //!inported published HLT trigger table
      const TriggerObjectsTable  *fTrigObjs;      //!inported published HLT trigger objects table
      Int_t                       fNEvents;       //!number of processed events
      Int_t                       fNAcceped;      //!number of accepted events
      Int_t                       fNFailed;       //!number of failed events

    ClassDef(HLTMod,1) // HLT TAM module
  };
}
#endif