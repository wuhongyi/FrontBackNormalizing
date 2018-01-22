// SelectData.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 1月 21 16:47:42 2018 (+0800)
// Last-Updated: 一 1月 22 11:29:57 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 29
// URL: http://wuhongyi.cn 

#include "SelectData.hh"

#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"

#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SelectData::SelectData(int nx,int ny)
{
  hx = NULL;
  gxy = NULL;
  filein = NULL;
  
  ChX = nx;
  ChY = ny;
}

SelectData::~SelectData()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SelectData::ReservedUniformityData(const char *inputrootfilename,const char *outputrootfilename)
{
  filein = new TFile(inputrootfilename,"READ");//"RECREATE" "READ"
  if(!filein->IsOpen())
    {
      std::cout<<"Can't open root file  "<<inputrootfilename<<std::endl;
      exit(1);
    }
  
  std::cout<<"Running SelectData::ReservedUniformityData"<<std::endl;
  TFile *filewrite = new TFile(outputrootfilename,"RECREATE");//"RECREATE" "READ"
  if(!filewrite->IsOpen())
    {
      std::cout<<"Can't open root file  "<<outputrootfilename<<std::endl;
    }
  
  for (int i = 0; i < ChX; ++i)
    for (int j = 0; j < ChY; ++j)
      {
	filein->cd();
	gxy = (TGraph *)filein->Get(TString::Format("fb_%02d_%02d",i,j).Data());

	hx = new TH1I(TString::Format("hx_%02d_%02d",i,j).Data(),"",400,0,8000);
	
	double *x = gxy->GetX();
	double *y = gxy->GetY();
	int n = gxy->GetN();

	for (int k = 0; k < n; ++k)
	  {
	    hx->Fill(x[k]);
	  }
	
	// gxy->Sort();

	int countbin = 0;
	for (int k = 0; k <= hx->GetNbinsX(); ++k)
	  {
	    if(hx->GetBinContent(k) > 0.5) countbin++;
	  }

	int Thre = int(hx->GetEntries()/countbin)+1;
	hx->SetTitle(TString::Format("Cut Thre: %d   Count Bin: %d",Thre,countbin).Data());
	
	filewrite->cd();
	hx->Write();
	filewrite->Write();

	hx->Reset();

	gsxy = new TGraph;
	gsxy->SetName(TString::Format("fb_%02d_%02d",i,j).Data());
	for (int k = 0; k < n; ++k)
	  {
	    if(hx->GetBinContent(hx->FindBin(x[k])) < Thre)
	      {
		hx->Fill(x[k]);
		gsxy->SetPoint(gsxy->GetN(),x[k],y[k]);
	      }
	  }
	
	
	filewrite->cd();
	gsxy->Write();
	filewrite->Write();

	delete gsxy;
	delete hx;
      }

  filewrite->cd();
  filewrite->Write();
  filewrite->Close();

  std::cout<<"SelectData::ReservedUniformityData  ok"<<std::endl;

  filein->cd();
  filein->Close();
}



// 
// SelectData.cc ends here
