// FitPixel.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 1月 20 22:19:14 2018 (+0800)
// Last-Updated: 日 1月 28 15:47:22 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 44
// URL: http://wuhongyi.cn 

#include "FitPixel.hh"

#include "TGraph.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"

#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FitPixel::FitPixel(int n,double *x,double *y)
{
  robust = -1;
  fitfunction = 0;
  par0 = 0;
  par1 = 0;
  err0 = 0;
  err1 = 0;
  orig = NULL;
  h2_y_er = NULL;
  h1_er = NULL;
  datarange = NULL;
  
  pointn = n;
  xx = x;
  yy = y;

  orig = new TGraph(n,x,y);
}

FitPixel::~FitPixel()
{
  if(orig != NULL) delete orig;
  if(datarange != NULL) delete datarange;
  if(h2_y_er != NULL) delete h2_y_er;
  if(h1_er != NULL) delete h1_er;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FitPixel::SetName(const char *name)
{
  nameofsave = TString(name);
}

void FitPixel::SetName(const TString name)  
{
  nameofsave = name;
}

void FitPixel::SetRobust(int rob)
{
  if(rob < -1 || rob >= 100)
    {
      std::cout<<"@@ Function FitPixel::SetRobust(int rob) paramrter 'rob' invalid !!!"<<std::endl;
      return;
    }
  robust = rob;
}

void FitPixel::FitRobust()
{
  fitfunction = 1;
  
  if(robust == -1)
    orig->Fit("pol1","Qrob");
  else
    orig->Fit("pol1",TString::Format("Qrob=%0.2f",robust/100.0));

  par0 = orig->GetFunction("pol1")->GetParameter(0);
  par1 = orig->GetFunction("pol1")->GetParameter(1);

  orig->SetName(TString::Format("orig_%s",nameofsave.Data()).Data());
  orig->SetTitle(TString::Format("b_%f_k_%f",par0,par1).Data());

  std::cout<<"FitRobust: "<<par0<<"  "<<par1<<std::endl;
}

void FitPixel::CalculateFitEffectHistogram()
{
  if(fitfunction == 0) return;
  
  h2_y_er = new TH2I(TString::Format("h2er_%s_f%d",nameofsave.Data(),fitfunction).Data(),"",200,-100,100,1000,0,8000);
  h1_er = new TH1I(TString::Format("h1er_%s_f%d",nameofsave.Data(),fitfunction).Data(),"",100,-50,50);
  
  for (int i = 0; i < pointn; ++i)
    {
      h2_y_er->Fill(yy[i]-(par1*xx[i]+par0),yy[i]);
      h1_er->Fill(yy[i]-(par1*xx[i]+par0));
    }
  
  // double left,right;
  // GetRangeAbove(h1_er,0.3,left,right);
  // h1_er->Fit("gaus","SQ","",left,right);

  // h1_er->Fit("gaus","SQ","",-h1_er->GetStdDev(1),h1_er->GetStdDev(1));
  h1_er->Fit("gaus","SQ","");
  
  h1_er_mean = h1_er->GetFunction("gaus")->GetParameter(1);
  h1_er_sigma = h1_er->GetFunction("gaus")->GetParameter(2);
  h1_er->SetTitle(TString::Format("mean_%f_sigma_%f",h1_er_mean,h1_er_sigma).Data());
  
  std::cout<<"h1 er: "<<h1_er_mean<<"  "<<h1_er_sigma<<std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FitPixel::SecondFit(int range)
{
  fitfunction = 2;
  
  if(datarange != NULL)
    {
      delete datarange;
      datarange = NULL;
    }

  datarange = new TGraph;

  for (int i = 0; i < pointn; ++i)
    {
      if(TMath::Abs(par0+par1*xx[i]-yy[i])/(1+par1*par1) <= range)
	{
	  datarange->SetPoint(datarange->GetN(),xx[i],yy[i]);
	}
    }

  // std::cout<<"Point:  "<<pointn<<"  "<<datarange->GetN()<<std::endl;
  
  datarange->Fit("pol1","Q");

  datarange->SetName(TString::Format("datarange_%s",nameofsave.Data()).Data());
  datarange->SetTitle(TString::Format("b_%f_k_%f_be_%f_bk_%f",par0,par1,err0,err1).Data());
  
  par0 = datarange->GetFunction("pol1")->GetParameter(0);
  par1 = datarange->GetFunction("pol1")->GetParameter(1);

  err0 = datarange->GetFunction("pol1")->GetParError(0);
  err1 = datarange->GetFunction("pol1")->GetParError(1);

  std::cout<<"Second Fit: "<<par0<<"  "<<par1<<"  "<<err0<<"  "<<err1<<std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FitPixel::GetRangeAbove(TH1 *h,double thre,double &left,double &right)
{
  double maxbincontent = h->GetBinContent(h->GetMaximumBin());
  left = h->GetBinCenter(h->FindFirstBinAbove(thre*maxbincontent));
  right = h->GetBinCenter(h->FindLastBinAbove(thre*maxbincontent));
  // std::cout<<"Fit Range: "<<left<<"  "<<right<<std::endl;
}



// 
// FitPixel.cc ends here
