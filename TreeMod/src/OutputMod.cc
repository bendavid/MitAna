// $Id: OutputMod.cc,v 1.1 2008/12/01 17:42:23 loizides Exp $

#include "MitAna/TreeMod/interface/OutputMod.h"
#include "MitAna/TreeMod/interface/HLTFwkMod.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataUtil/interface/TreeWriter.h"
#include "MitAna/TreeMod/interface/TreeBranchLoader.h"

using namespace mithep;
using namespace std;

ClassImp(mithep::OutputMod)

//--------------------------------------------------------------------------------------------------
OutputMod::OutputMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fTreeName(Names::gkEvtTreeName),
  fPrefix("skimtest"),
  fPathName("."),
  fMaxSize(1024),
  fCompLevel(9),
  fSplitLevel(99),
  fBranchSize(32*1024),
  fDoReset(kFALSE),
  fCheckTamBr(kTRUE),
  fKeepTamBr(kTRUE),
  fHltFwkModName("HLTFwkMod"),
  fTreeWriter(0),
  fMyEventHeader(0),
  fMyRunInfo(0),
  fMyLaHeader(0),
  fNBranchesMax(1024),
  fHltFwkMod(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void OutputMod::BeginRun()
{
  // Todo.

}

//--------------------------------------------------------------------------------------------------
void OutputMod::CheckAndAddBranch(const char *bname, const char *cname)
{
  // Check if the given branch should be kept or dropped. 
  
  if (IsAcceptedBranch(bname)) 
    return;

  // populate regular expression list if this was not yet done
  if (fCmdReList.size() != fCmdList.Entries()) {
    for (UInt_t i=0; i<fCmdList.Entries(); ++i) {
      const char *ptr = fCmdList.At(i)->c_str();
      fCmdReList.push_back(TRegexp(ptr+5,kTRUE));
      if (ptr[0]=='k') 
        fCmdDeList.push_back(kTRUE);
      else 
        fCmdDeList.push_back(kFALSE);
    }
  }

  // decide whether given branch name should be kept or dropped
  TString brname(bname);
  Bool_t decision       = kFALSE;
  Bool_t decision_found = kFALSE;

  for (UInt_t i=0; i<fCmdList.Entries(); ++i) {
    TRegexp &re(fCmdReList.at(i));
    if (brname.Index(re) == kNPOS)
      continue;
    decision = fCmdDeList.at(i);
    decision_found = kTRUE;
  }

  if (!decision_found) { // no decision found: still drop branch
    Warning("CheckAndAddBranch", 
            "No decision found for branch with name %s and class %s. Branch therefore dropped!",
            bname, cname);
    return;
  }

  if (!decision) { // drop branch according to request
    Info("CheckAndAddBranch", "Dropped branch with name %s and class %s.", bname, cname);
    return;
  }

  // add branch to accepted branch list
  Info("CheckAndAddBranch", "Kept branch with name %s and class %s.", bname, cname);

  fBrNameList.AddCopy(string(bname));
  fBrClassList.AddCopy(string(cname));

  // request branch
  RequestBranch(bname);
}

//--------------------------------------------------------------------------------------------------
void OutputMod::CheckAndResolveDep(Bool_t solve) 
{
  // Check if TAM has loaded additional branches. If requested try to solve the the dependency
  // by adding the branch to the list of branches.

  const THashTable &ht = GetSel()->GetBranchTable();

  TIterator *iter = ht.MakeIterator();
  const TAMBranchInfo *next = dynamic_cast<const TAMBranchInfo*>(iter->Next());

  while (next) {
    const TAMBranchInfo *cur = next;
    next = dynamic_cast<const TAMBranchInfo*>(iter->Next());
    Bool_t isloaded = cur->IsLoaded();
    if (!isloaded)
      continue;

    const char *bname = cur->GetName();
    if (IsAcceptedBranch(bname))
      continue;

    TreeBranchLoader *loader = dynamic_cast<TreeBranchLoader*>(cur->GetLoader());
    if (!loader) 
      continue;

    TBranch *br = loader->GetBranch();
    if (!br) 
      continue;

    const char *cname = br->GetClassName();

    if (solve) {
      Info("CheckAndResolveDep", "Resolving dependency for loaded branch %s and class %s",
           bname,cname);

      fBrNameList.AddCopy(string(bname));
      fBrClassList.AddCopy(string(cname));
      fBranches[GetNBranches()-1] = reinterpret_cast<TObject*>(loader->GetAddress());

    } else {
      Warning("CheckAndResolveDep", "Unresolved dependency for loaded branch %s and class %s",
              bname,cname);
    }
  }
}

//--------------------------------------------------------------------------------------------------
void OutputMod::RequestBranch(const char *bname) 
{
  // Request given branch from TAM.

  if (GetNBranches()>=fNBranchesMax) {
    Error("RequestBranch", "Can not request branch for %bname"
          "since maximum number of branches [%d] is reached", bname, fNBranchesMax);
    return;
  }
  
  fBranches[GetNBranches()-1] = 0;
  ReqBranch(bname, fBranches[GetNBranches()-1]);
}

//--------------------------------------------------------------------------------------------------
void OutputMod::EndRun()
{
  // Todo.

}

//--------------------------------------------------------------------------------------------------
Bool_t OutputMod::IsAcceptedBranch(const char *bname)
{
  // Return true if given branch is already in branch list. Also return true if a special
  // branch like the "EventHeader" branch is reqested. 

  // search in branch list
  for (UInt_t i=0; i<GetNBranches(); ++i) {
    if (fBrNameList.At(i)->compare(bname) == 0) 
      return kTRUE;
  }

  // check if special branch that we take care of ourselves
  string name(bname);
  if (name.compare("EventHeader") == 0) {
    return kTRUE;
  }

  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
Bool_t OutputMod::Notify()
{
  // On first notify, loop over list of branches to determine the list of kept branches.

  if (GetNEventsProcessed() != 0) 
    return kTRUE;

  TTree *tree=const_cast<TTree*>(GetSel()->GetTree());
  if (!tree) 
    return kFALSE;

  TObjArray *arr = tree->GetTree()->GetListOfBranches();
  if (!arr)
    return kFALSE;

  for (Int_t i=0; i<arr->GetEntries(); ++i) {
    TBranch *br = dynamic_cast<TBranch*>(arr->At(i));
    if (!br && !br->GetMother())
      continue;
    br = br->GetMother();
    TClass *cls = TClass::GetClass(br->GetClassName());
    if (!cls)
      continue;

    if (!cls->InheritsFrom("TObject")) {
      Warning("Notify", "Found branch %s where class %s does not derive from TObject.", 
              br->GetName(), br->GetClassName());
      continue;
    } 

    CheckAndAddBranch(br->GetName(), br->GetClassName());
  }

  return kTRUE;
}

//--------------------------------------------------------------------------------------------------
void OutputMod::LoadBranches()
{
  // Loop over requested branches and load them.

  for (UInt_t i=0; i<GetNBranches(); ++i) {
    LoadBranch(fBrNameList.At(i)->c_str());
  }
}

//--------------------------------------------------------------------------------------------------
void OutputMod::Process()
{
  // Pre and post event processing at once?!

  fTreeWriter->BeginEvent(fDoReset);

  if (GetNEventsProcessed() == 0 && fCheckTamBr) {
    CheckAndResolveDep(fKeepTamBr);    
  }

  LoadBranches();

  if (GetNEventsProcessed() == 0) {
    SetupBranches();
  }

  LoadBranch("EventHeader");
//  fMyEventHeader 

  IncNEventsProcessed();
  fTreeWriter->EndEvent(fDoReset);
}

//--------------------------------------------------------------------------------------------------
void OutputMod::SetupBranches()
{
  // Setup branches in tree writer.

  for (UInt_t i=0; i<GetNBranches(); ++i) {
    const char *bname = fBrNameList.At(i)->c_str();
    const char *cname = fBrClassList.At(i)->c_str();
    if (!fBranches[i]) {
      Error("SetupBranches", "Pointer for branch with name %s and class %s is NULL.",
            bname, cname);
      continue;
    }
    fTreeWriter->AddBranch(bname, cname, &fBranches[i]);
  }
}

//--------------------------------------------------------------------------------------------------
void OutputMod::SlaveBegin()
{
  // Todo

  // request here branches we want
  

  // setup tree writer
  fTreeWriter = new TreeWriter(fTreeName, kFALSE);
  fTreeWriter->SetBaseURL(fPathName);
  fTreeWriter->SetPrefix(fPrefix);
  fTreeWriter->SetMaxSize(fMaxSize*1024*1024);
  fTreeWriter->SetCompressLevel(fCompLevel);
  fTreeWriter->SetDefaultSL(fSplitLevel);
  fTreeWriter->SetDefaultBrSize(fBranchSize);
  fTreeWriter->AddTree(fTreeName);
  fTreeWriter->DoBranchRef(fTreeName);

  // deal with my own tree objects
  fMyEventHeader = new EventHeader;
  fTreeWriter->AddBranch(Names::gkEvtHeaderBrn, &fMyEventHeader);
  fMyRunInfo = new RunInfo;
  fTreeWriter->AddBranchToTree(Names::gkRunTreeName, Names::gkRunInfoBrn, &fMyRunInfo);
  fTreeWriter->SetAutoFill(Names::gkRunTreeName,0);
  fMyLaHeader = new LAHeader;
  fTreeWriter->AddBranchToTree(Names::gkLATreeName, Names::gkLAHeaderBrn, &fMyLaHeader);
  fTreeWriter->SetAutoFill(Names::gkLATreeName,0);


  // add branches to HLT trigger info tree
  //tws.AddBranchToTree(Names::gkHltTreeName,hltTableName_.c_str(),&hltTable_,32000,0);
  //tws.SetAutoFill(Names::gkHltTreeName,0);
  //tws.AddBranchToTree(Names::gkHltTreeName,hltLabelName_.c_str(),&hltLabels_,32000,0);
  //tws.SetAutoFill(Names::gkHltTreeName,0);
  //hltTree_=tws.GetTree(Names::gkHltTreeName);


  if (HasHLTInfo(fHltFwkModName)) {
    const TList *tasks = GetSelector()->GetTopModule()->GetSubModules();
    fHltFwkMod = static_cast<HLTFwkMod*>(tasks->FindObject(fHltFwkModName));
  }

  // deal here with published objects

  // create TObject space for TAM
  fBranches = new TObject*[fNBranchesMax];       

  // adjust checks for TAM branches
  if (fKeepTamBr)
    fCheckTamBr = kTRUE;
}

//--------------------------------------------------------------------------------------------------
void OutputMod::SlaveTerminate()
{
  // Todo

  delete fTreeWriter;
  fTreeWriter = 0;

  delete fMyEventHeader;

  delete[] fBranches; 
}

//--------------------------------------------------------------------------------------------------