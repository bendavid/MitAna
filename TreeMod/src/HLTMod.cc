// $Id: BaseMod.cc,v 1.1 2008/06/12 08:47:57 loizides Exp $

#include "MitAna/TreeMod/interface/HLTMod.h"
#include <TFile.h>
#include <TTree.h>
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/TriggerName.h"
#include "MitAna/DataTree/interface/TriggerObject.h"

using namespace mithep;

ClassImp(mithep::HLTMod)

//--------------------------------------------------------------------------------------------------
HLTMod::HLTMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fAbort(kTRUE),
  fBitsName(Names::gkHltBitBrn),
  fMyObjsNamePub(Form("%sObjsadafafa", name)),
  fBits(0),
  fMyTrgObjs(0),
  fTriggers(0),
  fTrigObjs(0),
  fNEvents(0),
  fNAcceped(0),
  fNFailed(0)
{
  // Constructor. 
}

//--------------------------------------------------------------------------------------------------
HLTMod::~HLTMod() 
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void HLTMod::AddTrigger(const char *expr)
{
  // Add trigger search pattern to the list of patters. Each element of the list is logically 
  // "ored". The given expression can contain several trigger names logically "anded" (using "&"). 
  // A "!" infront of a trigger name negates the bit. For example, valid expressions are:
  // "A", "!A", "A&B", "A&!B" or "A&B&C"  

  string tname(expr);
  fTrigNames.AddCopy(tname);
}

//--------------------------------------------------------------------------------------------------
void HLTMod::AddTrigObjs(UInt_t tid)
{
  // Add trigger objects corresponding to trigger id.

  const BitMask256 &ba = fTrigBitsAnd.Ref(tid);
  const BitMask256 &bm = fTrigBitsCmp.Ref(tid);
  for (UInt_t i=0; i<bm.Size(); ++i) {
    if (ba.TestBit(i)==0)
      continue; // not an active trigger bit
    if (fBitsDone.TestBit(i))
      continue; // objects for this bit are already obtained
    if (bm.TestBit(i)==0) 
      continue; // excluded trigger bit (ie a !trgname)

    const TList *list = fTrigObjs->GetList(i);
    if (list) {
      TIterator *iter = list->MakeIterator();
      if (iter) {
        TriggerObject *to = dynamic_cast<TriggerObject *>(iter->Next());
        while (to) {
          fMyTrgObjs->Add(to);
          to = dynamic_cast<TriggerObject *>(iter->Next());
        }
      }
    }
    fBitsDone.SetBit(i);
  }
}
 
//--------------------------------------------------------------------------------------------------
void HLTMod::BeginRun()
{
  // Get HLT tree and set branches. Compute bitmasks to be used when comparing to the HLT bits.

  fTrigBitsAnd.Reset();
  fTrigBitsCmp.Reset();

  for (UInt_t i=0; i<fTrigNames.Entries(); ++i) {
    BitMask256 tmask; //trigger mask
    BitMask256 amask; //bitand mask
    TString names(fTrigNames.At(i)->c_str());
    TObjArray *arr = names.Tokenize("&");
    if (arr) {
      for(Int_t j=0; j<arr->GetEntries(); j++){
        TObjString *s = dynamic_cast<TObjString*>(arr->At(j));
        if (!s) 
          continue;
        const char *sptr = s->GetString().Data();
        Bool_t invert = kFALSE;
        if (sptr[0] == '!') { 
          invert = kTRUE; //inverted bit set
          ++sptr;
        }
        const TriggerName *tn = fTriggers->Get(sptr);
        if (!tn) {
          Warning("BeginRun", "Trigger %s not found.", tn->Name());
          continue;
        }
        UShort_t bit = tn->Id();
        amask.SetBit(bit); //always set and-mask bit 
        if (!invert) 
          tmask.SetBit(bit); //set trigger bit
      }
      delete arr;
    }
    fTrigBitsAnd.AddCopy(amask);
    fTrigBitsCmp.AddCopy(tmask);
  }
}

//--------------------------------------------------------------------------------------------------
void HLTMod::Process()
{
  // Process trigger bits for this event. If trigger bits pass the given bit mask, then obtain
  // and publish the corresponding trigger objects. If OnAccepted or OnFailed is implemented
  // in a derived class, call it. Do not stop processing this event, if fAbort is kFALSE.

  ++fNEvents; 
  LoadBranch(fBitsName.Data());

  // match trigger bits to trigger mask and obtain trigger objects
  fMyTrgObjs = new TriggerObjectOArr(0,fMyObjsNamePub);
  fMyTrgObjs->SetOwner(kFALSE); // the objects are owned by the object table
  fBitsDone.Clear();
  Bool_t accept = kFALSE;
  for (UInt_t i = 0; i<fTrigBitsAnd.Entries(); ++i) {
    BitMask256 bitmask(*fBits);
    bitmask &= fTrigBitsAnd.Ref(i);
    if (bitmask==fTrigBitsCmp.Ref(i)) {
      accept = kTRUE;
      AddTrigObjs(i);
    }
  }

  // take action if failed
  if (!accept) {
    ++fNFailed;
    OnFailed();
    delete fMyTrgObjs;
    if (fAbort) {
      SkipEvent(); // abort processing of this event by sub-modules
    }
    return;
  } 

  // take action if accepted
  ++fNAcceped;
  OnAccepted();
  if (!AddObjThisEvt(fMyTrgObjs)) {
    SendError(kAbortAnalysis, "Process", 
              "Could not add my trigger objects with name %s to event.",
              fMyTrgObjs->GetName());
    return;
  }
  fMyTrgObjs = 0;
}

//--------------------------------------------------------------------------------------------------
void HLTMod::SlaveBegin()
{
  // Request trigger bit branch and obtain trigger table and objects.

  ReqBranch(fBitsName.Data(), fBits);
  
  fTriggers = GetHLTTable();
  if (!fTriggers) {
    SendError(kAbortAnalysis, "SlaveBegin", "Could not get HLT trigger table.");
    return;
  }
  fTrigObjs = GetHLTObjectsTable();
  if (!fTrigObjs) {
    SendError(kAbortAnalysis, "SlaveBegin", "Could not get HLT trigger objects table.");
    return;
  }
}