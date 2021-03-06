/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskCorPIDTOFdiprot
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */
//#include <iostream>
#include <fstream>
#include <cmath>
//#include <iomanip>
#include "TChain.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TMath.h"
//#include "TGraphErrors.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskCorPIDTOFdeut.h"
#include "AliPIDResponse.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"
//#include "AliEmcalTrackSelection.h"
//#include "AliEmcalTrackSelectionAOD.h"

//#include "TFile.h"

//#include "AliMultSelection.h"


// particle identifications in ALICE
//
//    enum EParticleType
//
//    kElectron =  0;
//    kMuon     =  1;
//    kPion     =  2;
//    kKaon     =  3;
//    kProton   =  4;
//    kDeuteron =  5;
//    kTriton   =  6;
//    kHe3      =  7;
//    kAlpha    =  8;
//    kPhoton   =  9;
//    kPi0      = 10;
//    kNeutron  = 11;
//    kKaon0    = 12;
//    kEleCon   = 13;
//    kUnknown  = 14;

//  event trigger selection
//  if(datajets)
//  {
//      if(!(fInputHandler->IsEventSelected() & fTriggerSelectionBits)) return false;
//      if(fTriggerSelectionString.Length())
//      {
//	  if(!fInputEvent->GetFiredTriggerClasses().Contains(fTriggerSelectionString)) return false;
//      }
//  }



using namespace std;            // std namespace: so you can do things like 'cout'
//using namespace BSchaefer_devel;

//ofstream file_output("output.txt");


ClassImp(AliAnalysisTaskCorPIDTOFdeut) // classimp: necessary for root

AliAnalysisTaskCorPIDTOFdeut::AliAnalysisTaskCorPIDTOFdeut() : AliAnalysisTaskSE(), 
fAOD(0), fOutputList(0), fPIDResponse(0),

    fHistPt(0),                    //  1
    cent_ntracks(0),               //  2
    
    m2_pt_pos(0),                  //  3
    m2_pt_neg(0),                  //  4
    beta_p_pos(0),                 //  5
    beta_p_neg(0),                 //  6

    deltat_pt_pos(0),              //  7
    deltat_pt_neg(0),              //  8

    m2_pt_pos_cut(0),              //  9
    m2_pt_neg_cut(0),              // 10
    beta_p_pos_cut(0),             // 11
    beta_p_neg_cut(0),             // 12
    deltat_pt_pos_cut(0),          // 13
    deltat_pt_neg_cut(0),          // 14

    deut_per_event(0),             // 15
//  deut_per_event_pos(0),         // 16
//  deut_per_event_neg(0),         // 17
    
    m2_pt_pos_cut_T(0),            // 18
    m2_pt_neg_cut_T(0),            // 19

    deut_phi_pt(0),                // 20
    deut_phi_pt_pos(0),            // 21
    deut_phi_pt_neg(0),            // 22
    

    trig_05_phi_pt(0),             // 23
    trig_05_phi_pt_pos(0),         // 24
    trig_05_phi_pt_neg(0),         // 25
    trig_08_phi_pt(0),             // 26
    trig_08_phi_pt_pos(0),         // 27
    trig_08_phi_pt_neg(0),         // 28
    
    deut_dphi_pt_pos_pos_05(0),    // 29
    deut_dphi_pt_pos_neg_05(0),    // 30
    deut_dphi_pt_neg_neg_05(0),    // 31

    deut_dphi_pt_pos_pos_08(0),    // 32
    deut_dphi_pt_pos_neg_08(0),    // 33
    deut_dphi_pt_neg_neg_08(0),    // 34

    tof_phi_eta_pos(0),            // 35
    tof_phi_eta_neg(0),            // 36

    tof_phi_eta_pos_deut(0),       // 37
    tof_phi_eta_neg_deut(0)        // 38
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskCorPIDTOFdeut::AliAnalysisTaskCorPIDTOFdeut(const char* name) : AliAnalysisTaskSE(name),
fAOD(0), fOutputList(0), fPIDResponse(0),
									   

    fHistPt(0),                    //  1
    cent_ntracks(0),               //  2
    
    m2_pt_pos(0),                  //  3
    m2_pt_neg(0),                  //  4
    beta_p_pos(0),                 //  5
    beta_p_neg(0),                 //  6

    deltat_pt_pos(0),              //  7
    deltat_pt_neg(0),              //  8

    m2_pt_pos_cut(0),              //  9
    m2_pt_neg_cut(0),              // 10
    beta_p_pos_cut(0),             // 11
    beta_p_neg_cut(0),             // 12
    deltat_pt_pos_cut(0),          // 13
    deltat_pt_neg_cut(0),          // 14

    deut_per_event(0),             // 15
//  deut_per_event_pos(0),         // 16
//  deut_per_event_neg(0),         // 17
    
    m2_pt_pos_cut_T(0),            // 18
    m2_pt_neg_cut_T(0),            // 19
							  
    deut_phi_pt(0),                // 20
    deut_phi_pt_pos(0),            // 21
    deut_phi_pt_neg(0),            // 22

    trig_05_phi_pt(0),             // 23
    trig_05_phi_pt_pos(0),         // 24
    trig_05_phi_pt_neg(0),         // 25
    trig_08_phi_pt(0),             // 26
    trig_08_phi_pt_pos(0),         // 27
    trig_08_phi_pt_neg(0),         // 28
									   
    deut_dphi_pt_pos_pos_05(0),    // 29
    deut_dphi_pt_pos_neg_05(0),    // 30
    deut_dphi_pt_neg_neg_05(0),    // 31

    deut_dphi_pt_pos_pos_08(0),    // 32
    deut_dphi_pt_pos_neg_08(0),    // 33
    deut_dphi_pt_neg_neg_08(0),    // 34

    tof_phi_eta_pos(0),            // 35
    tof_phi_eta_neg(0),            // 36
    tof_phi_eta_pos_deut(0),       // 37
    tof_phi_eta_neg_deut(0)        // 38
{
    // constructor
    DefineInput(0, TChain::Class());
    DefineOutput(1, TList::Class());
}
//_____________________________________________________________________________
AliAnalysisTaskCorPIDTOFdeut::~AliAnalysisTaskCorPIDTOFdeut()
{
    // destructor
    if(fOutputList)
    {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskCorPIDTOFdeut::UserCreateOutputObjects()
{
    // 2.88465 0.0761582 0.709281 0.124386 0.017642 -0.0316078 2.65738 0.115151 0.918566 0.0986592 0.0187545 0.00346519    // pp 2016 untriggered
    
    deut_curves[0][0][0] = 2.88465;     // pos deut mean curve
    deut_curves[0][0][1] = 0.0761582;
    deut_curves[0][0][2] = 0.709281;

    deut_curves[0][1][0] = 0.124386;  // pos deut sigma curve
    deut_curves[0][1][1] = 0.017642;
    deut_curves[0][1][2] = -0.0316078;

    deut_curves[1][0][0] = 2.65738;     // neg deut mean curve
    deut_curves[1][0][1] = 0.115151;
    deut_curves[1][0][2] = 0.918566;

    deut_curves[1][1][0] = 0.0986592;  // neg deut sigma curve
    deut_curves[1][1][1] = 0.0187545;
    deut_curves[1][1][2] = 0.00346519;


    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and will delete them
                                        // if requested (dont worry about this now)


    Double_t pt_binning[2001];


    Float_t moving_marker = 0.10;
    for(int i=0; i<1202; i++)
    {
	pt_binning[i] = moving_marker;
	moving_marker = moving_marker + pt_binning[i] * 0.005;
    }
    
    
     
    fHistPt                    = new TH1F("fHistPt",                    "Pt()",                        800,       pt_binning);                              //  1
    cent_ntracks               = new TH2F("cent_ntracks",               "cent_ntracks",                100,        0,    100,     100,       0,     800);   //  2
    
    m2_pt_pos                  = new TH2F("m2_pt_pos",                  "m2_pt_pos",                   800,       pt_binning,    2400,    -1.0,     7.0);   //  3
    m2_pt_neg                  = new TH2F("m2_pt_neg",                  "m2_pt_neg",                   800,       pt_binning,    2400,    -1.0,     7.0);   //  4
    beta_p_pos                 = new TH2F("beta_p_pos",                 "beta_p_pos",                  800,       pt_binning,    3000,     0.1,     1.1);   //  5
    beta_p_neg                 = new TH2F("beta_p_neg",                 "beta_p_neg",                  800,       pt_binning,    3000,     0.1,     1.1);   //  6
    deltat_pt_pos              = new TH2F("deltat_pt_pos",              "deltat_pt_pos",               800,       pt_binning,    7100,    -1.0,    70.0);   //  7
    deltat_pt_neg              = new TH2F("deltat_pt_neg",              "deltat_pt_neg",               800,       pt_binning,    7100,    -1.0,    70.0);   //  8

    m2_pt_pos_cut              = new TH2F("m2_pt_pos_cut",              "m2_pt_pos_cut",               800,       pt_binning,    2400,    -1.0,     7.0);   //  9
    m2_pt_neg_cut              = new TH2F("m2_pt_neg_cut",              "m2_pt_neg_cut",               800,       pt_binning,    2400,    -1.0,     7.0);   // 10
    beta_p_pos_cut             = new TH2F("beta_p_pos_cut",             "beta_p_pos_cut",              800,       pt_binning,    3000,     0.1,     1.1);   // 11
    beta_p_neg_cut             = new TH2F("beta_p_neg_cut",             "beta_p_neg_cut",              800,       pt_binning,    3000,     0.1,     1.1);   // 12
    deltat_pt_pos_cut          = new TH2F("deltat_pt_pos_cut",          "deltat_pt_pos_cut",           800,       pt_binning,    7100,    -1.0,    70.0);   // 13
    deltat_pt_neg_cut          = new TH2F("deltat_pt_neg_cut",          "deltat_pt_neg_cut",           800,       pt_binning,    7100,    -1.0,    70.0);   // 14


    deut_per_event             = new TH1I("deut_per_event",             "deut_per_event",               12,        0,     12);                              // 15
//  deut_per_event_pos         = new TH1I("deut_per_event_pos",         "deut_per_event_pos",           12,        0,     12);                              // 16
//  deut_per_event_neg         = new TH1I("deut_per_event_neg",         "deut_per_event_neg",           12,        0,     12);                              // 17
    m2_pt_pos_cut_T            = new TH2F("m2_pt_pos_cut_T",            "m2_pt_pos_cut_T",             800,       pt_binning,    2400,    -1.0,     7.0);   // 18
    m2_pt_neg_cut_T            = new TH2F("m2_pt_neg_cut_T",            "m2_pt_neg_cut_T",             800,       pt_binning,    2400,    -1.0,     7.0);   // 19

    deut_phi_pt                = new TH2F("deut_phi_pt",                "deut_phi_pt",                 800,       pt_binning,     300, -1.6708,  4.8124);   // 20
    deut_phi_pt_pos            = new TH2F("deut_phi_pt_pos",            "deut_phi_pt_pos",             800,       pt_binning,     300, -1.6708,  4.8124);   // 21
    deut_phi_pt_neg            = new TH2F("deut_phi_pt_neg",            "deut_phi_pt_neg",             800,       pt_binning,     300, -1.6708,  4.8124);   // 22


    trig_05_phi_pt             = new TH2F("trig_05_phi_pt",             "trig_05_phi_pt",             1200,       pt_binning,     300, -1.6708,  4.8124);   // 23
    trig_05_phi_pt_pos         = new TH2F("trig_05_phi_pt_pos",         "trig_05_phi_pt_pos",         1200,       pt_binning,     300, -1.6708,  4.8124);   // 24
    trig_05_phi_pt_neg         = new TH2F("trig_05_phi_pt_neg",         "trig_05_phi_pt_neg",         1200,       pt_binning,     300, -1.6708,  4.8124);   // 25
    trig_08_phi_pt             = new TH2F("trig_08_phi_pt",             "trig_08_phi_pt",             1200,       pt_binning,     300, -1.6708,  4.8124);   // 26
    trig_08_phi_pt_pos         = new TH2F("trig_08_phi_pt_pos",         "trig_08_phi_pt_pos",         1200,       pt_binning,     300, -1.6708,  4.8124);   // 27
    trig_08_phi_pt_neg         = new TH2F("trig_08_phi_pt_neg",         "trig_08_phi_pt_neg",         1200,       pt_binning,     300, -1.6708,  4.8124);   // 28
    
    deut_dphi_pt_pos_pos_05    = new TH2F("deut_dphi_pt_pos_pos_05",    "deut_dphi_pt_pos_pos_05",     800,       pt_binning,     300, -1.6708,  4.8124);   // 29
    deut_dphi_pt_pos_neg_05    = new TH2F("deut_dphi_pt_pos_neg_05",    "deut_dphi_pt_pos_neg_05",     800,       pt_binning,     300, -1.6708,  4.8124);   // 30
    deut_dphi_pt_neg_neg_05    = new TH2F("deut_dphi_pt_neg_neg_05",    "deut_dphi_pt_neg_neg_05",     800,       pt_binning,     300, -1.6708,  4.8124);   // 31
    
    deut_dphi_pt_pos_pos_08    = new TH2F("deut_dphi_pt_pos_pos_08",    "deut_dphi_pt_pos_pos_08",     800,       pt_binning,     300, -1.6708,  4.8124);   // 32
    deut_dphi_pt_pos_neg_08    = new TH2F("deut_dphi_pt_pos_neg_08",    "deut_dphi_pt_pos_neg_08",     800,       pt_binning,     300, -1.6708,  4.8124);   // 33
    deut_dphi_pt_neg_neg_08    = new TH2F("deut_dphi_pt_neg_neg_08",    "deut_dphi_pt_neg_neg_08",     800,       pt_binning,     300, -1.6708,  4.8124);   // 34

    tof_phi_eta_pos            = new TH2F("tof_phi_eta_pos",            "tof_phi_eta_pos",             600,  -1.6708, 4.8124,     300,    0.82,    0.82);   // 35
    tof_phi_eta_neg            = new TH2F("tof_phi_eta_neg",            "tof_phi_eta_neg",             600,  -1.6708, 4.8124,     300,    0.82,    0.82);   // 36

    tof_phi_eta_pos_deut       = new TH2F("tof_phi_eta_pos_deut",       "tof_phi_eta_pos_deut",        600,  -1.6708, 4.8124,     300,    0.82,    0.82);   // 37
    tof_phi_eta_neg_deut       = new TH2F("tof_phi_eta_neg_deut",       "tof_phi_eta_neg_deut",        600,  -1.6708, 4.8124,     300,    0.82,    0.82);   // 38
    
    // objects added to output file



    fOutputList->Add(fHistPt);                     //  1
    fOutputList->Add(cent_ntracks);                //  2
    
    fOutputList->Add(m2_pt_pos);                   //  3
    fOutputList->Add(m2_pt_neg);                   //  4 
    fOutputList->Add(beta_p_pos);                  //  5
    fOutputList->Add(beta_p_neg);                  //  6
    fOutputList->Add(deltat_pt_pos);               //  7
    fOutputList->Add(deltat_pt_neg);               //  8

    fOutputList->Add(m2_pt_pos_cut);               //  9
    fOutputList->Add(m2_pt_neg_cut);               // 10
    fOutputList->Add(beta_p_pos_cut);              // 11
    fOutputList->Add(beta_p_neg_cut);              // 12
    fOutputList->Add(deltat_pt_pos_cut);           // 13
    fOutputList->Add(deltat_pt_neg_cut);           // 14

    fOutputList->Add(deut_per_event);              // 15
//  fOutputList->Add(deut_per_event_pos);          // 16
//  fOutputList->Add(deut_per_event_neg);          // 17
    fOutputList->Add(m2_pt_pos_cut_T);             // 18
    fOutputList->Add(m2_pt_neg_cut_T);             // 19

    fOutputList->Add(deut_phi_pt);                 // 20
    fOutputList->Add(deut_phi_pt_pos);             // 21
    fOutputList->Add(deut_phi_pt_neg);             // 22
    
    fOutputList->Add(trig_05_phi_pt);              // 23
    fOutputList->Add(trig_05_phi_pt_pos);          // 24
    fOutputList->Add(trig_05_phi_pt_neg);          // 25
    fOutputList->Add(trig_08_phi_pt);              // 26
    fOutputList->Add(trig_08_phi_pt_pos);          // 27
    fOutputList->Add(trig_08_phi_pt_neg);          // 28
    
    fOutputList->Add(deut_dphi_pt_pos_pos_05);     // 29
    fOutputList->Add(deut_dphi_pt_pos_neg_05);     // 30
    fOutputList->Add(deut_dphi_pt_neg_neg_05);     // 31
    
    fOutputList->Add(deut_dphi_pt_pos_pos_08);     // 32
    fOutputList->Add(deut_dphi_pt_pos_neg_08);     // 33
    fOutputList->Add(deut_dphi_pt_neg_neg_08);     // 34

    fOutputList->Add(tof_phi_eta_pos);             // 35
    fOutputList->Add(tof_phi_eta_neg);             // 36

    fOutputList->Add(tof_phi_eta_pos_deut);        // 37
    fOutputList->Add(tof_phi_eta_neg_deut);        // 38


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    AliAnalysisManager *man            = AliAnalysisManager::GetAnalysisManager();                  //// added by Brennan
    AliInputEventHandler* inputHandler = (AliInputEventHandler*) (man->GetInputEventHandler());     //// added by Brennan
    fPIDResponse                       = inputHandler->GetPIDResponse();                            //// added by Brennan

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////


    PostData(1, fOutputList);
}







//_____________________________________________________________________________
void AliAnalysisTaskCorPIDTOFdeut::UserExec(Option_t *)
{

    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());
    if(!fAOD) return;

    Int_t iTracks(fAOD->GetNumberOfTracks());


    
    //////////////////////////////////////// MULTIPLICITY PART ////////////////////////////////////////

    cent_ntracks->Fill(fAOD->GetCentrality()->GetCentralityPercentile("V0A"), iTracks);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////



    int deut_track_num[20];    
    int deut_count              = 0;
    
    int trig_05_track_num[20];
    int trig_05_track_count     = 0;

    int trig_08_track_num[20];
    int trig_08_track_count     = 0;

    Float_t pio2 = TMath::PiOver2();
    Float_t twopi  = TMath::TwoPi();
    // markA

    // loop over all these tracks
    //
    // pull out track numbers for high-pt triggers and also deutons

   
    for(Int_t i(0); i < iTracks; i++)
    {
        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));
        if(!track)                                                                      {    continue;    }

	Float_t phi           = track->Phi();
	if(phi <  -pio2){    phi = phi + twopi; }	if(phi <  -pio2){    phi = phi + twopi;   }
	if(phi > 3*pio2){    phi = phi - twopi;	}       if(phi > 3*pio2){    phi = phi - twopi;   }
	
	Float_t pt            = track->Pt();
	if(!(track->IsHybridGlobalConstrainedGlobal()))                                 {    continue;    }
	Float_t eta = track->Eta();	if(TMath::Abs(eta) > 0.8)                       {    continue;    }

	if(!track->IsPrimaryCandidate())                                                {    continue;    }

	Double_t nsigmaTPC = 999.0;	Double_t nsigmaTOF = 999.0;
	AliPIDResponse::EDetPidStatus statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC, track, (AliPID::EParticleType) 0, nsigmaTPC);
	AliPIDResponse::EDetPidStatus statusTOF = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTOF, track, (AliPID::EParticleType) 0, nsigmaTOF);
	Bool_t tpcIsOk = (statusTPC == AliPIDResponse::kDetPidOk);     /* && trk->IsOn(AliESDtrack::kTPCpid)*/;
	Bool_t tofIsOk = (statusTOF == AliPIDResponse::kDetPidOk);
	if(!tpcIsOk)	                                                                {    continue;    }

	fHistPt->Fill(pt);
	
	if(pt >= 5.0)	{    trig_05_track_num[trig_05_track_count] = i;	    trig_05_track_count++;	}
	if(pt >= 8.0)	{    trig_08_track_num[trig_08_track_count] = i;	    trig_08_track_count++;	}

	if(!tofIsOk)	                                                                {    continue;    }


	
	Float_t deltat = tof_minus_tpion(track);

	float   mom           = track->P();
	Short_t charge        = track->Charge();
	Float_t deut_mean     = 0.0;
	Float_t deut_sigma    = 0.0;


	Float_t m2tof  = get_mass_squared(track);
	Float_t dedx   = track->GetTPCsignal();
	Float_t beta      = 0.0;
	beta      = Beta(track);

	    
	if(charge > 0)
	{
	    m2_pt_pos      ->Fill(pt,  m2tof);
	    beta_p_pos     ->Fill(mom, Beta(track));
	    deltat_pt_pos  ->Fill(pt,  deltat);
	    if(pt >= 2.0  &&  pt < 5.0)   tof_phi_eta_pos->Fill(phi, eta);
	    if(dedx > 7.91143*deltat+28.8714  &&  deltat > 0.07216*dedx-5.11340)
	    {
		if(        (1.0 <= deltat  &&  deltat < 6.0  &&  dedx <   9.6774*deltat+46.7742)
		       ||  (0.5 <= deltat  &&  deltat < 1.0  &&  dedx <  56.4516)
		       ||  (0.5 >  deltat  &&                    dedx <  56.4516)
		       ||  (6.0 <= deltat  &&                    dedx <  12.9032*deltat+27.4193)
		    )
		{
//		    AliPIDResponse::EDetPidStatus statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC, track, (AliPID::EParticleType) 4, nsigmaTPC);
//		    if(nsigmaTPC <= 2.0)
//		    {
			m2_pt_pos_cut    ->Fill(pt,  m2tof);
			beta_p_pos_cut   ->Fill(mom, Beta(track));
			deltat_pt_pos_cut->Fill(pt,  deltat);
			
			if(mom >= 1.0  &&  mom < 4.4)
			{
			    for(int w=0; w<3; w++){   fit_deut_curve->SetParameter(w, deut_curves[0][0][w]);   }
			    deut_mean = fit_deut_curve->Eval(mom);
			    for(int w=0; w<3; w++){   fit_deut_curve->SetParameter(w, deut_curves[0][1][w]);   }
			    deut_sigma = fit_deut_curve->Eval(mom);

			    if(m2tof < deut_mean + cut_width * deut_sigma  &&   m2tof > deut_mean - cut_width * deut_sigma)
			    {
				deut_track_num[deut_count] = i;
				deut_count++;
				m2_pt_pos_cut_T->Fill(pt,m2tof);
				if(pt >= 2.0  &&  pt < 5.0)   tof_phi_eta_pos_deut->Fill(phi, eta);
	
				Float_t deut_phi = track->Phi();
				if(deut_phi <  -pio2){    deut_phi = deut_phi + twopi;   }	if(deut_phi <  -pio2){    deut_phi = deut_phi + twopi;   }
				if(deut_phi > 3*pio2){    deut_phi = deut_phi - twopi;   }	if(deut_phi > 3*pio2){    deut_phi = deut_phi - twopi;   }
				deut_phi_pt    ->Fill(pt, deut_phi);
				deut_phi_pt_pos->Fill(pt, deut_phi);
			    }		    
			}			    
//		    }
		}
	    }
	}
	    
	else if(charge < 0)
	{
	    m2_pt_neg      ->Fill(pt,  m2tof);
	    beta_p_neg     ->Fill(mom, Beta(track));
	    deltat_pt_neg  ->Fill(pt,  deltat);
	    if(pt >= 2.0  &&  pt < 5.0)   tof_phi_eta_neg->Fill(phi, eta);
	    if(dedx > 7.91143*deltat+28.8714  &&  deltat > 0.07216*dedx-5.11340)
	    {
		if(        (1.0 <= deltat  &&  deltat < 6.0  &&  dedx <   9.6774*deltat+46.7742)
		       ||  (0.5 <= deltat  &&  deltat < 1.0  &&  dedx <  56.4516)
		       ||  (0.5 >  deltat  &&                    dedx <  56.4516)
		       ||  (6.0 <= deltat  &&                    dedx <  12.9032*deltat+27.4193)
		    )
		{
//		    AliPIDResponse::EDetPidStatus statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC, track, (AliPID::EParticleType) 4, nsigmaTPC);
//		    if(nsigmaTPC <= 2.0)
//		    {	
			m2_pt_neg_cut    ->Fill(pt, m2tof);
			beta_p_neg_cut   ->Fill(mom, Beta(track));
			deltat_pt_neg_cut->Fill(pt, deltat);


			if(mom >= 1.0  &&  mom < 4.4)
			{
			    for(int w=0; w<3; w++){   fit_deut_curve->SetParameter(w, deut_curves[1][0][w]);   }
			    deut_mean = fit_deut_curve->Eval(mom);
			    for(int w=0; w<3; w++){   fit_deut_curve->SetParameter(w, deut_curves[1][1][w]);   }
			    deut_sigma = fit_deut_curve->Eval(mom);

			    
			    
			    if(m2tof < deut_mean + cut_width * deut_sigma  &&   m2tof > deut_mean - cut_width * deut_sigma)
			    {
				deut_track_num[deut_count] = i;
				deut_count++;
				m2_pt_neg_cut_T->Fill(pt,m2tof);
				if(pt >= 2.0  &&  pt < 5.0)   tof_phi_eta_neg_deut->Fill(phi, eta);
				
				Float_t deut_phi = track->Phi();
				if(deut_phi <  -pio2){    deut_phi = deut_phi + twopi;   }	if(deut_phi <  -pio2){    deut_phi = deut_phi + twopi;   }
				if(deut_phi > 3*pio2){    deut_phi = deut_phi - twopi;   }	if(deut_phi > 3*pio2){    deut_phi = deut_phi - twopi;   }
				deut_phi_pt    ->Fill(pt, deut_phi);
				deut_phi_pt_neg->Fill(pt, deut_phi);

			    }		    
			}
//		    }
		}
	    }
	}    //   end of neg charge if statement
    }        //   end of track loop


    deut_per_event->Fill(deut_count);
//    deut_per_event_pos->Fill(NDeut_pos);
//    deut_per_event_neg->Fill(NDeut_neg);


//    cout<<endl<<endl;
    // markA
    
    if(deut_count > 0  &&  trig_05_track_count > 0)
    {
//	cout<<"trig count: "<<trig_05_track_count<<" associate deuton count: "<<deut_count<<" ";
//	cout<<trig_05_track_count<<","<<deut_count<<" ";
	for(int i=0; i<trig_05_track_count; i++)  // trigger loop
	{
	    
	    int H               = trig_05_track_num[i];
	    AliAODTrack* trackH = static_cast<AliAODTrack*>(fAOD->GetTrack(H));

	    Float_t phi_H       = trackH->Phi();
	    Float_t pt_H        = trackH->Pt();
	    Short_t charge_H    = trackH->Charge();
	    
	    if(phi_H <  -pio2){    phi_H = phi_H + twopi;   }	    if(phi_H <  -pio2){    phi_H = phi_H + twopi;   }
	    if(phi_H > 3*pio2){    phi_H = phi_H - twopi;   }	    if(phi_H > 3*pio2){    phi_H = phi_H - twopi;   }
	    trig_05_phi_pt->Fill(pt_H, phi_H);

	    if     (charge_H > 0)   trig_05_phi_pt_pos->Fill(pt_H, phi_H);
	    else if(charge_H < 0)   trig_05_phi_pt_neg->Fill(pt_H, phi_H);
	    
	    for(int j=0; j<deut_count; j++)
	    {
		int A               = deut_track_num[j];
		if(A != H)
		{
		    AliAODTrack* trackA = static_cast<AliAODTrack*>(fAOD->GetTrack(A));
		
		    Float_t phi_A       = trackA->Phi();
		    Short_t charge_A    = trackA->Charge();
		    Float_t pt_A        = trackA->Pt();


		    if(phi_A <  -pio2){    phi_A = phi_A + twopi;   }    if(phi_A <  -pio2){    phi_A = phi_A + twopi;   }
		    if(phi_A > 3*pio2){    phi_A = phi_A - twopi;   }    if(phi_A > 3*pio2){    phi_A = phi_A - twopi;   }
		    deut_phi_pt->Fill(pt_A, phi_A);

		    Float_t Sdphi = phi_A - phi_H;
		    if(Sdphi <  -pio2){    Sdphi = Sdphi + twopi;   }    if(Sdphi <  -pio2){    Sdphi = Sdphi + twopi;   }
		    if(Sdphi > 3*pio2){    Sdphi = Sdphi - twopi;   }    if(Sdphi > 3*pio2){    Sdphi = Sdphi - twopi;   }

		    if(charge_H > 0)
		    {
			if     (charge_A > 0){    deut_dphi_pt_pos_pos_05->Fill(pt_A, Sdphi);	}
			else if(charge_A < 0){    deut_dphi_pt_pos_neg_05->Fill(pt_A, Sdphi);	}
		    }
		    else if(charge_H < 0)
		    {
			if     (charge_A > 0){    deut_dphi_pt_pos_neg_05->Fill(pt_A, Sdphi);	}
			else if(charge_A < 0){    deut_dphi_pt_neg_neg_05->Fill(pt_A, Sdphi);	}
		    }
		}
	    }	    
	}
    }


    if(deut_count > 0  &&  trig_08_track_count > 0)
    {
	for(int i=0; i<trig_08_track_count; i++)  // trigger loop
	{
	    
	    int H               = trig_08_track_num[i];
	    AliAODTrack* trackH = static_cast<AliAODTrack*>(fAOD->GetTrack(H));

	    Float_t phi_H       = trackH->Phi();
	    Float_t pt_H        = trackH->Pt();
	    Short_t charge_H    = trackH->Charge();
	    
	    if(phi_H <  -pio2){    phi_H = phi_H + twopi;   }	    if(phi_H <  -pio2){    phi_H = phi_H + twopi;   }
	    if(phi_H > 3*pio2){    phi_H = phi_H - twopi;   }	    if(phi_H > 3*pio2){    phi_H = phi_H - twopi;   }
	    trig_08_phi_pt->Fill(pt_H, phi_H);

	    if     (charge_H > 0)   trig_08_phi_pt_pos->Fill(pt_H, phi_H);
	    else if(charge_H < 0)   trig_08_phi_pt_neg->Fill(pt_H, phi_H);
	    
	    for(int j=0; j<deut_count; j++)
	    {
		int A               = deut_track_num[j];

		if(A != H)
		{
		    AliAODTrack* trackA = static_cast<AliAODTrack*>(fAOD->GetTrack(A));
		    
		    Float_t phi_A       = trackA->Phi();
		    Short_t charge_A    = trackA->Charge();
		    Float_t pt_A        = trackA->Pt();


		    Float_t Sdphi = phi_A - phi_H;
		    if(Sdphi <  -pio2){    Sdphi = Sdphi + twopi;   }    if(Sdphi <  -pio2){    Sdphi = Sdphi + twopi;   }
		    if(Sdphi > 3*pio2){    Sdphi = Sdphi - twopi;   }    if(Sdphi > 3*pio2){    Sdphi = Sdphi - twopi;   }

		    if(charge_H > 0)
		    {
			if     (charge_A > 0){    deut_dphi_pt_pos_pos_08->Fill(pt_A, Sdphi);	}
			else if(charge_A < 0){    deut_dphi_pt_pos_neg_08->Fill(pt_A, Sdphi);	}
		    }
		    else if(charge_H < 0)
		    {
			if     (charge_A > 0){    deut_dphi_pt_pos_neg_08->Fill(pt_A, Sdphi);	}
			else if(charge_A < 0){    deut_dphi_pt_neg_neg_08->Fill(pt_A, Sdphi);	}
		    }
		}
	    }	    
	}
    }

                                                        // continue until all the tracks are processed
    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                        // the output manager which will take care of writing
                                                        // it to a file


}




//_____________________________________________________________________________
void AliAnalysisTaskCorPIDTOFdeut::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________



//_____________________________________________________________________________
Double_t AliAnalysisTaskCorPIDTOFdeut::Beta(AliAODTrack *track)
{
    Double_t startTime     = fPIDResponse->GetTOFResponse().GetStartTime(((AliVTrack*)track)->P());         //in ps
    Double_t stoptime      = track->GetTOFsignal();
    Double_t c             = TMath::C()*1.E-9;                                                              // m/ns
    Double_t length        = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kElectron)*1E-3*c;
    stoptime -= startTime;      
    Double_t scaleStopTime = stoptime*1E-3;          
    scaleStopTime          = scaleStopTime*c;
    return length/scaleStopTime;
}



//_____________________________________________________________________________
Double_t AliAnalysisTaskCorPIDTOFdeut::tof_minus_tpion(AliAODTrack *track)
{
    Double_t start_time     = fPIDResponse->GetTOFResponse().GetStartTime(track->P());                      // in ps
    Double_t stop_time      = track->GetTOFsignal();
    Double_t c              = TMath::C()*1.E-9;                                                             // in m/ns
    Double_t time_of_flight = stop_time - start_time;
    time_of_flight          = time_of_flight * 0.001;                                                       // convert from ps to ns
    Double_t length         = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kPion)*1E-3*c; // in meters
    length = length * 100;                                                                                  // convert to cm
    Double_t pion_time      = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kPion);        // // take a close look at this function -BRENNAN SCHAEFER-
    pion_time               = pion_time * 0.001;
    return time_of_flight - pion_time;
}



//_____________________________________________________________________________
Double_t AliAnalysisTaskCorPIDTOFdeut::get_mass_squared(AliAODTrack *track)
{
    Double_t start_time     = fPIDResponse->GetTOFResponse().GetStartTime(track->P());                      // in ps
    Double_t stop_time      = track->GetTOFsignal();
    Double_t c              = TMath::C()*1.E-9;                                                             // in m/ns
    Double_t tof            = stop_time - start_time;
    tof                     = tof * 0.001;                                                                  // convert from ps to ns
    Double_t length         = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kPion)*1E-3*c; // in meters
    length                  = length * 100;                                                                 // convert to cm
    Double_t mom            = track->P();
    Double_t m2             = 0.0;
    Double_t c2             = 29.9792458;
    
    m2 = pow(mom,2) * (tof*tof * c2 * c2 / (length * length) - 1);
    return m2;

}
