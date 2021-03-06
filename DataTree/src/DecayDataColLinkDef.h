// $Id: DecayDataColLinkDef.h,v 1.1 2009/06/15 15:00:15 loizides Exp $

#ifndef MITANA_DATATREE_DECAYDATACOLLINKDEF_H
#define MITANA_DATATREE_DECAYDATACOLLINKDEF_H

#include "MitAna/DataTree/interface/DecayDataCol.h"
#endif

#ifndef __CINT__
#   define _R__UNIQUEIDENTIFIER_ DecayDataCol
#   define _R__JOIN3_(F,X,Y) _NAME3_(F,X,Y)
#   undef _R__UNIQUE_
#   define _R__UNIQUE_(X) _R__JOIN3_( _R__UNIQUEIDENTIFIER_,X,__LINE__)
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::DecayData+;
#pragma link C++ class mithep::Collection<mithep::DecayData>+;
#pragma link C++ class mithep::Array<mithep::DecayData>+;
#pragma link C++ typedef mithep::DecayDataCol;
#pragma link C++ typedef mithep::DecayDataArr;
#endif
