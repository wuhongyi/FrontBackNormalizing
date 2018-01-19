// cailbration.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 1月 19 12:53:26 2018 (+0800)
// Last-Updated: 五 1月 19 20:49:28 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 51
// URL: http://wuhongyi.cn 

#include "cailbration.hh"

#include "TFile.h"
#include "TFitResultPtr.h"
#include "TGraph.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

cailbration::cailbration(const char *rootfilename,int maxf,int maxb)
{
  file = NULL;
  
  max_ch_f = maxf;
  max_ch_b = maxb;


  file = new TFile(rootfilename,"READ");//"RECREATE" "READ"
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file  "<<rootfilename<<std::endl;
    }
  
  // file->ls("");
  // TObject->Write();
  // TH1D *h = (TH1D*)file->Get("name");


  
}

cailbration::~cailbration()
{
  file->cd();
  file->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void cailbration::SimpleCail(const char *outputname,int ref,bool fb, int verbose)
{
  TGraph *gxy = NULL;
  TF1 *fxy = NULL;
  TF1 *fhxy = NULL;
  TH1D *hhxy = NULL;
  TFitResultPtr rxy,rh;

  TGraph *gg = NULL;
  
  if(fb)
    {
      // 将背面归一到正面参考条


    }// 背面归一到正面
  else
    {
      // 将正面归一到背面参考条
      for (int i = 0; i < max_ch_f; ++i)
	{
	  her[i] = new TH2I(TString::Format("her_%02d_%02d",i,ref).Data(),"",200,-100,100,1000,0,8000);
	  gxy = (TGraph *)file->Get(TString::Format("fb_%02d_%02d",i,ref).Data());
	  if(!gxy)
	    {
	      std::cout<<"Can't find TGraph with name of "<<TString::Format("fb_%02d_%02d",i,ref).Data()<<std::endl;
	      std::exit(1);
	    }

	  double *x = gxy->GetX();
	  double *y = gxy->GetY();
	  int n = gxy->GetN();
	  
	  std::cout<<i<<"  "<<gxy->GetN()<<std::endl;
	  // gxy->Print();

	  gg = new TGraph(n,x,y);

	  
	  rxy = gg->Fit("pol1","QS rob");
	  
	  fxy = gg->GetFunction("pol1");
	  
	  par0[i] = fxy->GetParameter(0);
	  par1[i] = fxy->GetParameter(1);
	  err0[i] = fxy->GetParError(0);
	  err1[i] = fxy->GetParError(1);

	  std::cout<<par0[i]<<"  "<<par1[i]<<"  "<<err0[i]<<"  "<<err1[i]<<std::endl;

	  delete gg;
	  gg = NULL;

	  for(int j = 0;j < n; j++)
	    {
	      // if(TMath::Abs(y[j]-(par1[i]*x[j]+par0[i])) < 20)
	      her[i]->Fill(y[j]-(par1[i]*x[j]+par0[i]),y[j]);
	    }

	  hhxy = (TH1D*)her[i]->ProjectionX();
	  rh = hhxy->Fit("gaus","SQ","",-hhxy->GetStdDev(1),hhxy->GetStdDev(1));//TODO 这里应该改成拟合区间为峰高度的10%边界
	  fhxy = (TF1*)hhxy->GetFunction("gaus");

	  hmean[i] = fhxy->GetParameter(1);
	  hsigma[i] = fhxy->GetParameter(2);
	  
	  std::cout<<"gaus   "<<fhxy->GetParameter(1)<<"  "<<fhxy->GetParameter(2)<<std::endl;
	}// for max_ch_f

      
      std::ofstream writetxt;
      writetxt.open(TString::Format("%s_SampleCail_2b%02d.txt",outputname,ref).Data());//ios::bin ios::app
      if(!writetxt.is_open())
	{
	  std::cout<<"can't open file."<<std::endl;
	}
      for (int i = 0; i < max_ch_f; ++i)
	{
	  writetxt<<i<<"  "<<par0[i]<<"  "<<par1[i]<<"  "<<hmean[i]<<"  "<<hsigma[i]<<std::endl;
	}
      writetxt<<std::endl<<"ch  par/b  par/k  errorM  errorW"<<std::endl;
      writetxt.close();
      
      
    }// 正面归一到背面

}


// 
// cailbration.cc ends here
