// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 1月 19 12:51:11 2018 (+0800)
// Last-Updated: 一 1月 22 18:11:50 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 24
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

  SelectData *sd = new SelectData(16,16);
  sd->ReservedUniformityData("data/le2.root","data/sel_le2.root");
  sd->ReservedUniformityData("data/se2.root","data/sel_se2.root");

  cailbration *cail = new cailbration(16,16);
  cail->SimpleCail("data/le2.root","testle2",8,false);
  cail->SimpleCail("data/le2.root","testle2",8,true);
  cail->GetSimpleCailPar("testle2",8,8);
  cail->TestSimpleCailEffect("data/le2.root","testle2","result_le2.root");

  cail->SimpleCail("data/sel_le2.root","test_sel_le2",8,false);
  cail->SimpleCail("data/sel_le2.root","test_sel_le2",8,true);
  cail->GetSimpleCailPar("test_sel_le2",8,8);
  cail->TestSimpleCailEffect("data/sel_le2.root","test_sel_le2","result_sel_le2.root");

  
  // and enter the event loop...
  theApp->Run();
  delete theApp;

  return 0;
}

// 
// main.cc ends here
