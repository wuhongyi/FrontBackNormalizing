// FitPixel.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 1月 20 22:19:14 2018 (+0800)
// Last-Updated: 日 1月 21 00:41:47 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
// URL: http://wuhongyi.cn 

#include "FitPixel.hh"

#include "TGraph.h"
#include "TF1.h"

#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FitPixel::FitPixel(int n,double *x,double *y)
{
  robust = -1;
  par0 = 0;
  par1 = 0;
  err0 = 0;
  err1 = 0;
  orig = NULL;

  
  pointn = n;
  xx = x;
  yy = y;


  orig = new TGraph(n,x,y);
  // for (int i = 0; i < pointn; ++i)
  //   {
  //     std::cout<<i<<"  "<<x[i]<<"  "<<y[i]<<std::endl;
  //   }
}

FitPixel::~FitPixel()
{
  delete orig;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
  if(robust == -1)
    orig->Fit("pol1","Qrob");
  else
    orig->Fit("pol1",TString::Format("Qrob=%0.2f",robust/100.0));

  par0 = orig->GetFunction("pol1")->GetParameter(0);
  par1 = orig->GetFunction("pol1")->GetParameter(1);
}



// 
// FitPixel.cc ends here
