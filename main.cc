// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 1月 19 12:51:11 2018 (+0800)
// Last-Updated: 三 1月 31 00:53:52 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 47
// URL: http://wuhongyi.cn 

#include "cailbration.hh"
#include "SelectData.hh"

#include "RVersion.h"//版本判断
#include "TApplication.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TBenchmark.h"
#include "TBranch.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TColor.h"
#include "TCutG.h"
#include "TDatime.h"
#include "TError.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TFormula.h"
#include "TGaxis.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TList.h"
#include "TLorentzVector.h"
#include "TMarker.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TMultiGraph.h"
#include "TNtuple.h"
#include "TObject.h"
#include "TPad.h"
#include "TPaveLabel.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TRandom.h"
#include "TRandom1.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TRint.h"
#include "TROOT.h"
#include "TSlider.h"
#include "TSpectrum.h"
#include "TSpectrum2.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTimer.h"
#include "TTimeStamp.h"
#include "TTree.h"
#include "TVector3.h"
#include "TVectorD.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);

  // Select data
  
  // SelectData *sd = new SelectData(16,16);
  // sd->ReservedUniformityData("data/le2_all.root","data/s_le2_all.root");
  // sd->ReservedUniformityData("data/le2_sel_all.root","data/ss_le2_all.root");
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // cailbration
  
  cailbration *cail = new cailbration(16,16);

  // cail->SimpleCail("data/s_le2_all.root","test_le2",8,false,1);
  // cail->SimpleCail("data/s_le2_all.root","test_le2",8,true,1);
  // cail->GetSimpleCailPar("test_le2",8,8);
  // cail->TestSimpleCailEffect("data/s_le2_all.root","test_le2","resultsimple_le2_all.root");
  // cail->TestSimpleCailEffect("data/le2_all.root","test_le2","resultsimple_orig_le2_all.root");

  // cail->SimpleCail("data/s_le2_all.root","testr_le2",8,false,0);
  // cail->SimpleCail("data/s_le2_all.root","testr_le2",8,true,0);
  // cail->GetSimpleCailPar("testr_le2",8,8);
  // cail->TestSimpleCailEffect("data/s_le2_all.root","testr_le2","resultsimpler_le2_all.root");
  // cail->TestSimpleCailEffect("data/le2_all.root","testr_le2","resultsimpler_orig_le2_all.root");

  // cail->OverAllCail("data/s_le2_all.root","test_le2",false);
  // cail->OverAllCail("data/s_le2_all.root","test_le2",true);
  // cail->GetOverAllCailPar("test_le2");
  // cail->TestOverAllCailEffect("data/s_le2_all.root","test_le2","resultoverall_le2_all.root");
  // cail->TestOverAllCailEffect("data/le2_all.root","test_le2","resultoverall_orig_le2_all.root");



  cail->SimpleCail("data/ss_le2_all.root","tests_le2",8,false,1);
  cail->SimpleCail("data/ss_le2_all.root","tests_le2",8,true,1);
  cail->GetSimpleCailPar("tests_le2",8,8);
  cail->TestSimpleCailEffect("data/ss_le2_all.root","tests_le2","resultsimples_le2_all.root");
  cail->TestSimpleCailEffect("data/le2_sel_all.root","tests_le2","resultsimples_orig_le2_all.root");

  cail->SimpleCail("data/ss_le2_all.root","testsr_le2",8,false,0);
  cail->SimpleCail("data/ss_le2_all.root","testsr_le2",8,true,0);
  cail->GetSimpleCailPar("testsr_le2",8,8);
  cail->TestSimpleCailEffect("data/ss_le2_all.root","testsr_le2","resultsimplesr_le2_all.root");
  cail->TestSimpleCailEffect("data/le2_sel_all.root","testsr_le2","resultsimplesr_orig_le2_all.root");
  
  cail->OverAllCail("data/ss_le2_all.root","tests_le2",false);
  cail->OverAllCail("data/ss_le2_all.root","tests_le2",true);
  cail->GetOverAllCailPar("tests_le2");
  cail->TestOverAllCailEffect("data/ss_le2_all.root","tests_le2","resultoveralls_le2_all.root");
  cail->TestOverAllCailEffect("data/le2_sel_all.root","tests_le2","resultoveralls_orig_le2_all.root");



  
  // and enter the event loop...
  theApp->Run();
  delete theApp;

  return 0;
}

// 
// main.cc ends here
