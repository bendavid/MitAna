// $Id: StableData.cc,v 1.2 2008/10/13 10:35:11 bendavid Exp $

#include "MitAna/DataTree/interface/StableData.h"

ClassImp(mithep::StableData)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
const BitMask64 StableData::MissedHits() const
{
  // Construct layer mask of missed hits

  const ChargedParticle *cOrig = dynamic_cast<const ChargedParticle*>(Original());
  if (!cOrig)
    return BitMask64(0);

  BitMask64 missedHits = BadLayers() & ~cOrig->Trk()->Hits();

  return missedHits;

}

//--------------------------------------------------------------------------------------------------
const BitMask64 StableData::WrongHits() const
{
  // Construct layer mask of wrong (before decay vertex) hits

  const ChargedParticle *cOrig = dynamic_cast<const ChargedParticle*>(Original());
  if (!cOrig)
    return BitMask64(0);

  BitMask64 wrongHits = BadLayers() & cOrig->Trk()->Hits();

  return wrongHits;

}
