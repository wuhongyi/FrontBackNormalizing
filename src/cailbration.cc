// cailbration.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 1月 19 12:53:26 2018 (+0800)
// Last-Updated: 一 1月 22 21:43:18 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 157
// URL: http://wuhongyi.cn 

#include "cailbration.hh"
#include "FitPixel.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TFile.h"
#include "TFitResultPtr.h"
#include "TGraph.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"

#include "Math/MinimizerOptions.h"
#include "TLinearFitter.h"


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

cailbration::cailbration(int maxf,int maxb)
{
  file = NULL;
  
  max_ch_f = maxf;
  max_ch_b = maxb;
  
}

cailbration::~cailbration()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void cailbration::SimpleCail(const char *inputrootfilename,const char *outputname,int ref,bool fb, int verbose)
{
  file = new TFile(inputrootfilename,"READ");//"RECREATE" "READ"
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file  "<<inputrootfilename<<std::endl;
      exit(1);
    }
  
  TGraph *gxy = NULL;
  double *x,*y;
  // TF1 *fxy = NULL;
  // TF1 *fhxy = NULL;
  // TH1D *hhxy = NULL;
  // TFitResultPtr rxy,rh;

  // TGraph *gg = NULL;

  // gStyle->SetOptFit(1111);

  TFile *filewrite = NULL;
  int max_ch;
  
  if(fb)
    {
      filewrite = new TFile(TString::Format("b2f_SampleCail_%s.root",outputname).Data(),"RECREATE");
      max_ch = max_ch_b;
    }
  else
    {
      filewrite = new TFile(TString::Format("f2b_SampleCail_%s.root",outputname).Data(),"RECREATE");
      max_ch = max_ch_f;
    }
  if(!filewrite->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
      exit(1);
    }
      
  for (int i = 0; i < max_ch; ++i)
    {
      file->cd();
      if(fb)
	{
	  gxy = (TGraph *)file->Get(TString::Format("fb_%02d_%02d",ref,i).Data());
	  y = gxy->GetX();
	  x = gxy->GetY();
	}
      else
	{
	  gxy = (TGraph *)file->Get(TString::Format("fb_%02d_%02d",i,ref).Data());
	  x = gxy->GetX();
	  y = gxy->GetY();
	}

      // int n = gxy->GetN();
      fitpixel = new FitPixel(gxy->GetN(),x,y);
      if(fb)
	{
	  fitpixel->SetName(TString::Format("b2f_%02d_%02d",ref,i).Data());
	}
      else
	{
	  fitpixel->SetName(TString::Format("f2b_%02d_%02d",i,ref).Data());
	}
      
      // fitpixel->SetRobust(50);
      fitpixel->FitRobust();
      fitpixel->CalculateRobustFitEffectHistogram();

      par0[i] = fitpixel->GetParameter0();
      par1[i] = fitpixel->GetParameter1();
      err0[i] = fitpixel->GetParError0();
      err1[i] = fitpixel->GetParError1();
      hmean[i] = fitpixel->GetRobustFitHistogramMean();
      hsigma[i] = fitpixel->GetRobustFitHistogramSigma();

      filewrite->cd();
      fitpixel->GetRobustFitGraph()->Write();
      fitpixel->GetRobustFitEffectTH2()->Write();
      fitpixel->GetRobustFitEffectTH1()->Write();
      filewrite->Write();
      delete fitpixel;
    }

  filewrite->cd();
  filewrite->Write();
  filewrite->Close();

  std::ofstream writetxt;
  if(fb)
    {
      writetxt.open(TString::Format("%s_SampleCail_2f%02d.txt",outputname,ref).Data());
    }
  else
    {
      writetxt.open(TString::Format("%s_SampleCail_2b%02d.txt",outputname,ref).Data());
    }
  if(!writetxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
      exit(1);
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      writetxt<<i<<"  "<<par0[i]<<"  "<<par1[i]<<"  "<<hmean[i]<<"  "<<hsigma[i]<<std::endl;
    }
  writetxt<<std::endl<<"ch  par/b  par/k  errorM  errorW"<<std::endl;

  if(fb)
    {
      writetxt<<"Back Normalized to Front No. "<<ref<<std::endl;
    }
  else
    {
      writetxt<<"Front Normalized to Back No. "<<ref<<std::endl;
    }

  writetxt.close();


  file->cd();
  file->Close();
  file = NULL;
  
  // if(fb)
  //   {
  //     // 将背面归一到正面参考条
      
  //     // for (int i = 0; i < max_ch_b; ++i)
  //     // 	{
  //     // 	  her[i] = new TH2I(TString::Format("her_%02d_%02d",ref,i).Data(),"",200,-100,100,1000,0,8000);
  //     // 	  gxy = (TGraph *)file->Get(TString::Format("fb_%02d_%02d",ref,i).Data());
  //     // 	  if(!gxy)
  //     // 	    {
  //     // 	      std::cout<<"Can't find TGraph with name of "<<TString::Format("fb_%02d_%02d",i,ref).Data()<<std::endl;
  //     // 	      std::exit(1);
  //     // 	    }

  //     // 	  double *x = gxy->GetX();
  //     // 	  double *y = gxy->GetY();
  //     // 	  int n = gxy->GetN();
	  
  //     // 	  std::cout<<i<<"  "<<gxy->GetN()<<std::endl;
	  
  //     // 	  gg = new TGraph(n,y,x);
	  
  //     // 	  rxy = gg->Fit("pol1","QSrob");
  //     // 	  fxy = gg->GetFunction("pol1");
  //     // 	  par0[i] = fxy->GetParameter(0);
  //     // 	  par1[i] = fxy->GetParameter(1);
  //     // 	  err0[i] = fxy->GetParError(0);
  //     // 	  err1[i] = fxy->GetParError(1);
  //     // 	  prob = fxy->GetProb();

  //     // 	  std::cout<<par0[i]<<"  "<<par1[i]<<"  "<<err0[i]<<"  "<<err1[i]<<"  "<<prob<<"  "<<fxy->GetNumberFitPoints()<<"  "<<fxy->GetChisquare()<<"  "<<gg->GetCorrelationFactor()<<std::endl;

  //     // 	  delete gg;
  //     // 	  gg = NULL;


  //     // 	  for(int j = 0;j < n; j++)
  //     // 	    {
  //     // 	      her[i]->Fill(x[j]-(par1[i]*y[j]+par0[i]),x[j]);
  //     // 	    }

  //     // 	  hhxy = (TH1D*)her[i]->ProjectionX();
  //     // 	  rh = hhxy->Fit("gaus","SQ","",-hhxy->GetStdDev(1),hhxy->GetStdDev(1));//TODO 这里应该改成拟合区间为峰高度的10%边界
  //     // 	  fhxy = (TF1*)hhxy->GetFunction("gaus");

  //     // 	  hmean[i] = fhxy->GetParameter(1);
  //     // 	  hsigma[i] = fhxy->GetParameter(2);
	  
  //     // 	  std::cout<<"gaus   "<<fhxy->GetParameter(1)<<"  "<<fhxy->GetParameter(2)<<std::endl;

  //     // 	  delete her[i];
  //     // 	}// max_ch_b


  //     // std::ofstream writetxt;
  //     // writetxt.open(TString::Format("%s_SampleCail_2f%02d.txt",outputname,ref).Data());//ios::bin ios::app
  //     // if(!writetxt.is_open())
  //     // 	{
  //     // 	  std::cout<<"can't open file."<<std::endl;
  //     // 	}
  //     // for (int i = 0; i < max_ch_b; ++i)
  //     // 	{
  //     // 	  writetxt<<i<<"  "<<par0[i]<<"  "<<par1[i]<<"  "<<hmean[i]<<"  "<<hsigma[i]<<std::endl;
  //     // 	}
  //     // writetxt<<std::endl<<"ch  par/b  par/k  errorM  errorW"<<std::endl;
  //     // writetxt<<"Back Normalized to Front No. "<<ref<<std::endl;
  //     // writetxt.close();

  //   }// 背面归一到正面
  // else
  //   {
  //     // 将正面归一到背面参考条

  //     TFile *filewrite = new TFile(TString::Format("f2b_SampleCail_%s.root",outputname).Data(),"RECREATE");//"RECREATE" "READ"
  //     if(!filewrite->IsOpen())
  // 	{
  // 	  std::cout<<"Can't open root file"<<std::endl;
  // 	}

      
  //     for (int i = 0; i < max_ch_f; ++i)
  // 	{
  // 	  file->cd();
  // 	  TGraph *gxy = (TGraph *)file->Get(TString::Format("fb_%02d_%02d",i,ref).Data());
  // 	  double *x = gxy->GetX();
  // 	  double *y = gxy->GetY();
  // 	  int n = gxy->GetN();
  // 	  fitpixel = new FitPixel(n,x,y);
  // 	  fitpixel->SetName(TString::Format("f2b_%02d_%02d",i,ref).Data());
  // 	  // fitpixel->SetRobust(50);
  // 	  fitpixel->FitRobust();
  // 	  fitpixel->CalculateRobustFitEffectHistogram();

  // 	  par0[i] = fitpixel->GetParameter0();
  // 	  par1[i] = fitpixel->GetParameter1();
  // 	  err0[i] = fitpixel->GetParError0();
  // 	  err1[i] = fitpixel->GetParError1();
  // 	  hmean[i] = fitpixel->GetRobustFitHistogramMean();
  // 	  hsigma[i] = fitpixel->GetRobustFitHistogramSigma();

  // 	  filewrite->cd();
  // 	  fitpixel->GetRobustFitGraph()->Write();
  // 	  fitpixel->GetRobustFitEffectTH2()->Write();
  // 	  fitpixel->GetRobustFitEffectTH1()->Write();
  // 	  filewrite->Write();
  // 	  delete fitpixel;
  // 	}

  //     filewrite->cd();
  //     filewrite->Write();
  //     filewrite->Close();

  //     std::ofstream writetxt;
  //     writetxt.open(TString::Format("%s_SampleCail_2b%02d.txt",outputname,ref).Data());//ios::bin ios::app
  //     if(!writetxt.is_open())
  //     	{
  //     	  std::cout<<"can't open file."<<std::endl;
  //     	}
  //     for (int i = 0; i < max_ch_f; ++i)
  //     	{
  //     	  writetxt<<i<<"  "<<par0[i]<<"  "<<par1[i]<<"  "<<hmean[i]<<"  "<<hsigma[i]<<std::endl;
  //     	}
  //     writetxt<<std::endl<<"ch  par/b  par/k  errorM  errorW"<<std::endl;
  //     writetxt<<"Front Normalized to Back No. "<<ref<<std::endl;
  //     writetxt.close();
      
  //   }// 正面归一到背面

}

void cailbration::GetSimpleCailPar(const char *inoutfilename,int b,int f)
{
  double par2b[CH_MAX][2];
  double par2f[CH_MAX][2];

  double tempdouble;
  double tempint;
  
  std::ifstream readtxt;
  
  readtxt.open(TString::Format("%s_SampleCail_2b%02d.txt",inoutfilename,b).Data());
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      readtxt>>tempint>>par2b[i][0]>>par2b[i][1]>>tempdouble>>tempdouble;
    }
  readtxt.close();

  readtxt.open(TString::Format("%s_SampleCail_2f%02d.txt",inoutfilename,f).Data());
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }
  for (int i = 0; i < max_ch_b; ++i)
    {
      readtxt>>tempint>>par2f[i][0]>>par2f[i][1]>>tempdouble>>tempdouble;
    }
  readtxt.close();
  

  std::ofstream writetxt;
  
  writetxt.open(TString::Format("%s_SampleCail_2f.txt",inoutfilename).Data());
  if(!writetxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
      exit(1);
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      writetxt<<i<<"  "<<par2f[f][1]*par2b[i][0]+par2f[f][0]<<"  "<<par2f[f][1]*par2b[i][1]<<std::endl;
    }
  for (int i = 0; i < max_ch_b; ++i)
    {
      writetxt<<i<<"  "<<par2f[i][0]<<"  "<<par2f[i][1]<<std::endl;
    }
  writetxt.close();


  writetxt.open(TString::Format("%s_SampleCail_2b.txt",inoutfilename).Data());
  if(!writetxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
      exit(1);
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      writetxt<<i<<"  "<<par2b[i][0]<<"  "<<par2b[i][1]<<std::endl;
    }
  for (int i = 0; i < max_ch_b; ++i)
    {
      writetxt<<i<<"  "<<par2b[b][1]*par2f[i][0]+par2b[b][0]<<"  "<<par2b[b][1]*par2f[i][1]<<std::endl;
    }
  writetxt.close();
}

void cailbration::TestSimpleCailEffect(const char *inputrootfilename,const char *parfilename,const char *outputrootfilename)
{
  TGraph *gxy = NULL;
  double *x,*y;
  double fx,fy;
  double bx,by;

  double par2b[2*CH_MAX][2];
  double par2f[2*CH_MAX][2];
  double tempint;
  
  std::ifstream readtxt;
  
  readtxt.open(TString::Format("%s_SampleCail_2b.txt",parfilename).Data());
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      readtxt>>tempint>>par2b[i][0]>>par2b[i][1];
    }
  for (int i = 0; i < max_ch_b; ++i)
    {
      readtxt>>tempint>>par2b[i+max_ch_f][0]>>par2b[i+max_ch_f][1];
    }
  readtxt.close();

  readtxt.open(TString::Format("%s_SampleCail_2f.txt",parfilename).Data());
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      readtxt>>tempint>>par2f[i][0]>>par2f[i][1];
    }
  for (int i = 0; i < max_ch_b; ++i)
    {
      readtxt>>tempint>>par2f[i+max_ch_f][0]>>par2f[i+max_ch_f][1];
    }
  readtxt.close();

  
  file = new TFile(inputrootfilename,"READ");
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file  "<<inputrootfilename<<std::endl;
      exit(1);
    }

  TFile *filewrite = NULL;
  filewrite = new TFile(outputrootfilename,"RECREATE");
  if(!filewrite->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
      exit(1);
    }
  TH2I *hf = new TH2I("hf","front vs #DeltaE",1000,-50,50,2000,0,8000);
  TH2I *hb = new TH2I("hb","back vs #DeltaE",1000,-50,50,2000,0,8000);

  TH2I *hxyf = new TH2I("hxyf","",2000,0,8000,2000,0,8000);
  TH2I *hxyb = new TH2I("hxyb","",2000,0,8000,2000,0,8000);
  
  for (int i = 0; i < max_ch_f; ++i)
    for (int j = 0; j < max_ch_b; ++j)
      {
	file->cd();
	gxy = (TGraph *)file->Get(TString::Format("fb_%02d_%02d",i,j).Data());
	x = gxy->GetX();
	y = gxy->GetY();

	for (int k = 0; k < gxy->GetN(); ++k)
	  {
	    fx = par2f[i][0]+par2f[i][1]*x[k];
	    fy = par2f[max_ch_f+j][0]+par2f[max_ch_f+j][1]*y[k];
	    bx = par2b[i][0]+par2b[i][1]*x[k];
	    by = par2b[max_ch_f+j][0]+par2b[max_ch_f+j][1]*y[k];

	    hxyf->Fill(fx,fy);
	    hxyb->Fill(bx,by);
	    hf->Fill(fx-fy,fx);
	    hb->Fill(bx-by,by);
	  }

      }

  file->cd();
  file->Close();
  file = NULL;
  
  
  filewrite->cd();
  hxyf->Write();
  hxyb->Write();
  hf->Write();
  hb->Write();
  filewrite->Write();
  filewrite->Close();
}



// 
// cailbration.cc ends here
