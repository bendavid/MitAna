//--------------------------------------------------------------------------------------------------
// $Id: Electron.h,v 1.23 2009/01/22 14:21:32 loizides Exp $
//
// Electron
//
// Details to be worked out... TODO: Needs description ala Muon class
//
// Authors: C.Loizides, J.Bendavid, S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_ELECTRON_H
#define MITANA_DATATREE_ELECTRON_H
 
#include "MitAna/DataTree/interface/SuperCluster.h"
#include "MitAna/DataTree/interface/ChargedParticle.h"
#include "MitAna/DataCont/interface/Ref.h"

namespace mithep 
{
  class Electron : public ChargedParticle
  {
    public:
      Electron() : fESuperClusterOverP(0), fESeedClusterOverPout(0), fDeltaEtaSuperClTrkAtVtx(0),
                   fDeltaEtaSeedClTrkAtCalo(0), fDeltaPhiSuperClTrkAtVtx(0), 
                   fDeltaPhiSeedClTrkAtCalo(0), fHadronicOverEm(0), fIsEnergyScaleCorrected(0),
                   fIsMomentumCorrected(0), fNumberOfClusters(0), fClassification(0), fE33(0),
                   fE55(0), fCovEtaEta(0), fCoviEtaiEta(0), fCovEtaPhi(0), fCovPhiPhi(0),
                   fCaloIsolation(0), fCaloTowerIsolation(0), fTrackIsolation(0), 
                   fEcalJurassicIsolation(0), fHcalJurassicIsolation(0), fPassLooseID(0),
                   fPassTightID(0), fIDLikelihood(0), fPIn(0), fPOut(0) {}
      ~Electron() {}
      
      const Track         *BestTrk()               const;
      const Track         *GsfTrk()                const { return fGsfTrackRef.Obj();       }
      const Track         *TrackerTrk()            const { return fTrackerTrackRef.Obj();   }
      const SuperCluster  *SCluster()              const { return fSuperClusterRef.Obj();   }
      FourVector           Mom()                   const;
      const Track         *Trk()                   const { return BestTrk();                }
      Double_t    CaloIsolation()                  const { return fCaloIsolation;           }
      Double_t    CaloTowerIsolation()             const { return fCaloTowerIsolation;      }
      Double_t    Classification()                 const { return fClassification;          }
      Double_t    CovEtaEta()                      const { return fCovEtaEta;               }
      Double_t    CovEtaPhi()                      const { return fCovEtaPhi;               }
      Double_t    CovPhiPhi()                      const { return fCovPhiPhi;               }
      Double_t    CoviEtaiEta()                    const { return fCoviEtaiEta;             }
      Double_t    DeltaEtaSuperClusterTrackAtVtx() const { return fDeltaEtaSuperClTrkAtVtx; }
      Double_t    DeltaEtaSeedClusterTrackAtCalo() const { return fDeltaEtaSeedClTrkAtCalo; }
      Double_t    DeltaPhiSuperClusterTrackAtVtx() const { return fDeltaPhiSuperClTrkAtVtx; }
      Double_t    DeltaPhiSeedClusterTrackAtCalo() const { return fDeltaPhiSeedClTrkAtCalo; }
      Double_t    E()                              const;
      Double_t    E33()                            const { return fE33;                     }
      Double_t    E55()                            const { return fE55;                     }
      Double_t    EcalJurassicIsolation()          const { return fEcalJurassicIsolation;   }
      Double_t    ESuperClusterOverP()             const { return fESuperClusterOverP;      }
      Double_t    ESeedClusterOverPout()           const { return fESeedClusterOverPout;    }
      Double_t    ESeedClusterOverPIn()            const;
      Double_t    IDLikelihood()                   const { return fIDLikelihood;            }
      Double_t    IsEnergyScaleCorrected()         const { return fIsEnergyScaleCorrected;  }
      Double_t    IsMomentumCorrected()            const { return fIsMomentumCorrected;     }
      Double_t    HadronicOverEm()                 const { return fHadronicOverEm;          }
      Bool_t      HasGsfTrk()                      const { return fGsfTrackRef.IsValid();   }
      Bool_t      HasTrackerTrk()                  const { return fTrackerTrackRef.IsValid(); }
      Bool_t      HasSuperCluster()                const { return fSuperClusterRef.IsValid(); }
      Double_t    HcalIsolation()                  const { return fHcalJurassicIsolation;   }
      Double_t    Mass()                           const { return 0.51099892e-3;            }
      Double_t    NumberOfClusters()               const { return fNumberOfClusters;        }
      EObjType    ObjType()                        const { return kElectron;                }      
      Double_t    PassLooseID()                    const { return fPassLooseID;             }
      Double_t    PassTightID()                    const { return fPassTightID;             }
      Double_t    PIn()                            const { return fPIn;                     }
      Double_t    POut()                           const { return fPOut;                    }
      Double_t    P()                              const;
      Double_t    Pt()                             const;
      Double_t    Px()                             const;
      Double_t    Py()                             const;
      Double_t    Pz()                             const;
      Double_t    TrackIsolation()                 const { return fTrackIsolation;          }
      void	  SetGsfTrk(const Track* t)              { fGsfTrackRef = t;                }
      void	  SetTrackerTrk(const Track* t)          { fTrackerTrackRef = t;            }
      void	  SetSuperCluster(const SuperCluster* sc) { fSuperClusterRef = sc;          }
      void        SetCaloIsolation(Double_t CaloIsolation)      { fCaloIsolation = CaloIsolation;  }
      void        SetCaloTowerIsolation(Double_t TowerIso)      { fCaloTowerIsolation = TowerIso;  }
      void        SetClassification(Double_t x)                 { fClassification = x;             }
      void        SetCovEtaEta(Double_t CovEtaEta)              { fCovEtaEta = CovEtaEta;          }
      void        SetCovEtaPhi(Double_t CovEtaPhi)              { fCovEtaPhi = CovEtaPhi;          }
      void        SetCovPhiPhi(Double_t CovPhiPhi)              { fCovPhiPhi = CovPhiPhi;          }
      void        SetCoviEtaiEta(Double_t CoviEtaiEta)          { fCoviEtaiEta = CoviEtaiEta;      }
      void        SetDeltaEtaSuperClusterTrackAtVtx(Double_t x) { fDeltaEtaSuperClTrkAtVtx = x;    }
      void        SetDeltaEtaSeedClusterTrackAtCalo(Double_t x) { fDeltaEtaSeedClTrkAtCalo = x;    }
      void        SetDeltaPhiSuperClusterTrackAtVtx(Double_t x) { fDeltaPhiSuperClTrkAtVtx = x;    }
      void        SetDeltaPhiSeedClusterTrackAtCalo(Double_t x) { fDeltaPhiSeedClTrkAtCalo = x;    }
      void        SetE33(Double_t E33)                          { fE33 = E33;                      }
      void        SetE55(Double_t E55)                          { fE55 = E55;                      }
      void        SetEcalJurassicIsolation(Double_t iso )       { fEcalJurassicIsolation = iso;    }
      void        SetESuperClusterOverP(Double_t x)             { fESuperClusterOverP = x;         }
      void        SetESeedClusterOverPout(Double_t x)           { fESeedClusterOverPout = x;       }
      void        SetHadronicOverEm(Double_t x)                 { fHadronicOverEm = x;             }
      void        SetIDLikelihood(Double_t likelihood)          { fIDLikelihood = likelihood;      }
      void        SetIsEnergyScaleCorrected(Double_t x)         { fIsEnergyScaleCorrected = x;     }
      void        SetIsMomentumCorrected(Double_t x)            { fIsMomentumCorrected = x;        }
      void        SetHcalIsolation(Double_t iso )               { fHcalJurassicIsolation = iso;    }
      void        SetNumberOfClusters(Double_t x)               { fNumberOfClusters = x;           }
      void        SetPassLooseID(Double_t passLooseID)          { fPassLooseID = passLooseID;      }
      void        SetPassTightID(Double_t passTightID)          { fPassTightID = passTightID;      }
      void        SetPIn(Double_t PIn)                          { fPIn = PIn;                      }
      void        SetPOut(Double_t POut)                        { fPOut = POut;                    }
      void        SetTrackIsolation(Double_t TrackIsolation)    { fTrackIsolation = TrackIsolation;}

    protected:
      Ref<Track>  fGsfTrackRef;               //gsf track reference
      Ref<Track>  fTrackerTrackRef;           //tracker track reference
      Ref<SuperCluster> fSuperClusterRef;     //reference to SuperCluster
      Double_t    fESuperClusterOverP;        //
      Double_t    fESeedClusterOverPout;      //
      Double_t    fDeltaEtaSuperClTrkAtVtx;   //
      Double_t    fDeltaEtaSeedClTrkAtCalo;   //
      Double_t    fDeltaPhiSuperClTrkAtVtx;   //
      Double_t    fDeltaPhiSeedClTrkAtCalo;   //
      Double_t    fHadronicOverEm;            //
      Double_t    fIsEnergyScaleCorrected;    //
      Double_t    fIsMomentumCorrected;       //
      Double_t    fNumberOfClusters;          //
      Double_t    fClassification;            //
      Double_t    fE33;                       //
      Double_t    fE55;                       //
      Double_t    fCovEtaEta;                 //
      Double_t    fCoviEtaiEta;               //
      Double_t    fCovEtaPhi;                 //
      Double_t    fCovPhiPhi;                 //
      Double_t    fCaloIsolation;             //
      Double_t    fCaloTowerIsolation;        //
      Double_t    fTrackIsolation;            //
      Double_t    fEcalJurassicIsolation;     //
      Double_t    fHcalJurassicIsolation;     // 
      Double_t    fPassLooseID;               //
      Double_t    fPassTightID;               //
      Double_t    fIDLikelihood;              //
      Double_t    fPIn;                       //
      Double_t    fPOut;                      //

    ClassDef(Electron, 1) // Electron class
  };
}

//--------------------------------------------------------------------------------------------------
inline const mithep::Track *mithep::Electron::BestTrk() const
{
  // Return "best" track.

  if (HasGsfTrk())
    return GsfTrk();
  else if (HasTrackerTrk())
    return TrackerTrk();

  return 0;
}

//-------------------------------------------------------------------------------------------------
inline mithep::FourVector mithep::Electron::Mom() const
{
  // Return momentum of the electron. We use the direction of the
  // track and the energy of the SuperCluster.

  return FourVector(Px(), Py(), Pz(), E());
}

//-------------------------------------------------------------------------------------------------
inline Double_t mithep::Electron::ESeedClusterOverPIn() const
{
  // Return energy of the SuperCluster seed divided by the magnitude 
  // of the track momentum at the vertex.
  
  return SCluster()->Seed()->Energy() / PIn();
}

//-------------------------------------------------------------------------------------------------
inline Double_t mithep::Electron::E() const
{
  // Return energy of the SuperCluster if present
  // or else return energy derived from the track.
  
  const mithep::SuperCluster *sc = SCluster();
  if (sc)
    return sc->Energy();
  else
    return TMath::Sqrt(Trk()->P()*Trk()->P() + Mass()*Mass());
}

//-------------------------------------------------------------------------------------------------
inline Double_t mithep::Electron::P() const
{
  // Return momentum derived from the SuperCluster if present
  // or else return momentum from the track.
  
  const mithep::SuperCluster *sc = SCluster();
  if (sc)
    return TMath::Sqrt(sc->Energy()*sc->Energy() - Mass()*Mass());
  else
    return Trk()->P();
}

//-------------------------------------------------------------------------------------------------
inline Double_t mithep::Electron::Px() const
{
  // Return px.

  return Pt()*TMath::Cos(Trk()->Phi());
}

//-------------------------------------------------------------------------------------------------
inline Double_t mithep::Electron::Py() const
{
  // Return py.

  return Pt()*TMath::Sin(Trk()->Phi());
}

//-------------------------------------------------------------------------------------------------
inline Double_t mithep::Electron::Pz() const
{
  // Return pz.

  return P()*TMath::Sin(Trk()->Lambda());
}

//-------------------------------------------------------------------------------------------------
inline Double_t mithep::Electron::Pt() const
{
  // Return pt.

  return TMath::Abs(P()*TMath::Cos(Trk()->Lambda()));
}
#endif
