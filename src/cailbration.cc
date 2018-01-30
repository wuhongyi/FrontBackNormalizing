// cailbration.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 1月 19 12:53:26 2018 (+0800)
// Last-Updated: 二 1月 30 23:21:10 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 175
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

void cailbration::SimpleCail(const char *inputrootfilename,const char *outputname,int ref,bool fb, int fittype)
{
  file = new TFile(inputrootfilename,"READ");
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file  "<<inputrootfilename<<std::endl;
      exit(1);
    }
  
  TGraph *gxy = NULL;
  double *x,*y;

  // gStyle->SetOptFit(1111);

  TFile *filewrite = NULL;
  int max_ch;
  
  if(fb)
    {
      filewrite = new TFile(TString::Format("b2f_SimpleCail_%s.root",outputname).Data(),"RECREATE");
      max_ch = max_ch_b;
    }
  else
    {
      filewrite = new TFile(TString::Format("f2b_SimpleCail_%s.root",outputname).Data(),"RECREATE");
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
      if(fittype == 1)
	{
	  fitpixel->SecondFit();
	}
      fitpixel->CalculateFitEffectHistogram();

      par0[i] = fitpixel->GetParameter0();
      par1[i] = fitpixel->GetParameter1();
      err0[i] = fitpixel->GetParError0();
      err1[i] = fitpixel->GetParError1();
      hmean[i] = fitpixel->GetFitHistogramMean();
      hsigma[i] = fitpixel->GetFitHistogramSigma();

      filewrite->cd();
      fitpixel->GetRobustFitGraph()->Write();
      if(fittype == 1)
	{
	  fitpixel->GetSecondFitGraph()->Write();
	}
      fitpixel->GetFitEffectTH2()->Write();
      fitpixel->GetFitEffectTH1()->Write();
      filewrite->Write();
      delete fitpixel;
    }

  filewrite->cd();
  filewrite->Write();
  filewrite->Close();

  std::ofstream writetxt;
  if(fb)
    {
      writetxt.open(TString::Format("%s_SimpleCail_2f%02d.txt",outputname,ref).Data());
    }
  else
    {
      writetxt.open(TString::Format("%s_SimpleCail_2b%02d.txt",outputname,ref).Data());
    }
  if(!writetxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
      exit(1);
    }
  for (int i = 0; i < max_ch; ++i)
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
}

void cailbration::GetSimpleCailPar(const char *inoutfilename,int b,int f)
{
  double par2b[CH_MAX][2];
  double par2f[CH_MAX][2];

  double tempdouble;
  double tempint;
  
  std::ifstream readtxt;
  
  readtxt.open(TString::Format("%s_SimpleCail_2b%02d.txt",inoutfilename,b).Data());
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      readtxt>>tempint>>par2b[i][0]>>par2b[i][1]>>tempdouble>>tempdouble;
    }
  readtxt.close();

  readtxt.open(TString::Format("%s_SimpleCail_2f%02d.txt",inoutfilename,f).Data());
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
  
  writetxt.open(TString::Format("%s_SimpleCail_2f.txt",inoutfilename).Data());
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


  writetxt.open(TString::Format("%s_SimpleCail_2b.txt",inoutfilename).Data());
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
  
  readtxt.open(TString::Format("%s_SimpleCail_2b.txt",parfilename).Data());
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

  readtxt.open(TString::Format("%s_SimpleCail_2f.txt",parfilename).Data());
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


void cailbration::OverAllCail(const char *inputrootfilename,const char *outputname,bool fb)
{
  file = new TFile(inputrootfilename,"READ");
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file  "<<inputrootfilename<<std::endl;
      exit(1);
    }
  
  TGraph *gxy = NULL;
  double *x,*y;


  for (int i = 0; i < max_ch_f; ++i)
    for (int j = 0; j < max_ch_b; ++j)
    {
      file->cd();
      if(fb)//to front
	{
	  gxy = (TGraph *)file->Get(TString::Format("fb_%02d_%02d",i,j).Data());
	  y = gxy->GetX();
	  x = gxy->GetY();
	}
      else// to back
	{
	  gxy = (TGraph *)file->Get(TString::Format("fb_%02d_%02d",i,j).Data());
	  x = gxy->GetX();
	  y = gxy->GetY();
	}

      fitpixel = new FitPixel(gxy->GetN(),x,y);
      
      fitpixel->FitRobust();
      fitpixel->SecondFit();

      p0[i][j] = fitpixel->GetParameter0();
      p1[i][j] = fitpixel->GetParameter1();
      e0[i][j] = fitpixel->GetParError0();
      e1[i][j] = fitpixel->GetParError1();
      w0[i][j] = fitpixel->GetParWeight0();
      w1[i][j] = fitpixel->GetParWeight1();

      delete fitpixel;
    }

  file->cd();
  file->Close();
  file = NULL;


  int ref;
  int max_ch;
  if(fb)//to front
    {
      ref = max_ch_f/2;
      max_ch = max_ch_b;
    }
  else
    {
      ref = max_ch_b/2;
      max_ch = max_ch_f;
    }


  double K1, B1, ek1, eb1;
  double K2, B2, ek2, eb2;
  double K3, B3, ek3, eb3;
  double Ktmp, Wktp, Btmp, Wbtp;
  
  
  if(fb)
    {//背面归一到正面
      for (int q = 0; q < max_ch_b; ++q)
	{
      
	  W_f[q][0] = w0[q][ref];
	  W_f[q][1] = w1[q][ref];
	  P_f[q][0] = W_f[q][0] * p0[q][ref];
	  P_f[q][1] = W_f[q][1] * p1[q][ref];
	  e_f[q][0] = 0;
	  e_f[q][1] = 0;
	  fNode[q] = 0.1;

	  for (int i = 0; i < max_ch_b; ++i)
	    {
	      if(i == q) continue;
	      for (int j = 0; j < max_ch_f; ++j)
		{
		  if(j == ref) continue;

		  // if() continue;//这里应该添加判断拟合是否有效，无效的pixel不参与计算权重

		  fNode[q] += 1;

		  K1 = p1[j][q];
		  B1 = p0[j][q];
		  ek1 = e1[j][q];
		  eb1 = e0[j][q];
        
		  K2 = p1[j][i];
		  B2 = p0[j][i];
		  ek2 = e1[j][i];
		  eb2 = e0[j][i];

		  K3 = p1[ref][i];
		  B3 = p0[ref][i];
		  ek3 = e1[ref][i];
		  eb3 = e0[ref][i];

		  Ktmp = K1*K3/K2;
		  Wktp = 1./((K3*ek1/K2)*(K3*ek1/K2)+(K1*ek3/K2)*(K1*ek3/K2)+(K1*K3*ek2/K2/K2)*(K1*K3*ek2/K2/K2));
		  Btmp = B3+(B1-B2)*K3/K2;
		  Wbtp = 1./(eb3*eb3+((B1-B2)*ek3/K2)*((B1-B2)*ek3/K2)+(K3*eb1/K2)*(K3*eb1/K2)+(K3*eb2/K2)*(K3*eb2/K2)+((B1-B2)*K3*ek2/K2/K2)*((B1-B2)*K3*ek2/K2/K2));
		  P_f[q][1] += Wktp*Ktmp;
		  P_f[q][0] += Wbtp*Btmp;
		  W_f[q][1] += Wktp;
		  W_f[q][0] += Wbtp;		  
		
		}// for j
	    }// for i

	  if(W_f[q][1]>0)
	    {
	      P_f[q][1] /= W_f[q][1];
	      e_f[q][1] = 1./TMath::Sqrt( W_f[q][1]);
	    }
	  if(W_f[q][0]>0)
	    {
	      P_f[q][0] /= W_f[q][0];
	      e_f[q][0] = 1./TMath::Sqrt( W_f[q][0]);
	    }
	  printf("front ch %02d;k: %8.6f +/- %8.6f, b: %8.4f +/- %8.4f, n: %10.1f\n",q,P_f[q][1],e_f[q][1],P_f[q][0],e_f[q][0],fNode[q]);

	}// for q

    }//fb


  
  if(!fb)
    {//正面归一到背面
      for (int q = 0; q < max_ch_f; ++q)
	{
      
	  W_f[q][0] = w0[q][ref];
	  W_f[q][1] = w1[q][ref];
	  P_f[q][0] = W_f[q][0] * p0[q][ref];
	  P_f[q][1] = W_f[q][1] * p1[q][ref];
	  e_f[q][0] = 0;
	  e_f[q][1] = 0;
	  fNode[q] = 0.1;

	  for (int i = 0; i < max_ch_f; ++i)
	    {
	      if(i == q) continue;
	      for (int j = 0; j < max_ch_b; ++j)
		{
		  if(j == ref) continue;

		  // if() continue;//这里应该添加判断拟合是否有效，无效的pixel不参与计算权重

		  fNode[q] += 1;

		  K1 = p1[q][j];
		  B1 = p0[q][j];
		  ek1 = e1[q][j];
		  eb1 = e0[q][j];
        
		  K2 = p1[i][j];
		  B2 = p0[i][j];
		  ek2 = e1[i][j];
		  eb2 = e0[i][j];

		  K3 = p1[i][ref];
		  B3 = p0[i][ref];
		  ek3 = e1[i][ref];
		  eb3 = e0[i][ref];

		  Ktmp = K1*K3/K2;
		  Wktp = 1./((K3*ek1/K2)*(K3*ek1/K2)+(K1*ek3/K2)*(K1*ek3/K2)+(K1*K3*ek2/K2/K2)*(K1*K3*ek2/K2/K2));
		  Btmp = B3+(B1-B2)*K3/K2;
		  Wbtp = 1./(eb3*eb3+((B1-B2)*ek3/K2)*((B1-B2)*ek3/K2)+(K3*eb1/K2)*(K3*eb1/K2)+(K3*eb2/K2)*(K3*eb2/K2)+((B1-B2)*K3*ek2/K2/K2)*((B1-B2)*K3*ek2/K2/K2));
		  P_f[q][1] += Wktp*Ktmp;
		  P_f[q][0] += Wbtp*Btmp;
		  W_f[q][1] += Wktp;
		  W_f[q][0] += Wbtp;		  
		
		}// for j
	    }// for i

	  if(W_f[q][1]>0)
	    {
	      P_f[q][1] /= W_f[q][1];
	      e_f[q][1] = 1./TMath::Sqrt( W_f[q][1]);
	    }
	  if(W_f[q][0]>0)
	    {
	      P_f[q][0] /= W_f[q][0];
	      e_f[q][0] = 1./TMath::Sqrt( W_f[q][0]);
	    }
	  printf("front ch %02d;k: %8.6f +/- %8.6f, b: %8.4f +/- %8.4f, n: %10.1f\n",q,P_f[q][1],e_f[q][1],P_f[q][0],e_f[q][0],fNode[q]);

	}// for q

    }//!fb


  std::ofstream writetxt;
  if(fb)
    {
      writetxt.open(TString::Format("%s_OverAllCail_2f%02d.txt",outputname,ref).Data());
    }
  else
    {
      writetxt.open(TString::Format("%s_OverAllCail_2b%02d.txt",outputname,ref).Data());
    }
  if(!writetxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
      exit(1);
    }
  for (int i = 0; i < max_ch; ++i)
    {
      writetxt<<i<<"  "<<P_f[i][0]<<"  "<<P_f[i][1]<<"  "<<e_f[i][0]<<"  "<<e_f[i][1]<<std::endl;
    }
  writetxt<<std::endl<<"ch  par/b  par/k  errorb  errork"<<std::endl;

  if(fb)
    {
      writetxt<<"Back Normalized to Front No. "<<ref<<std::endl;
    }
  else
    {
      writetxt<<"Front Normalized to Back No. "<<ref<<std::endl;
    }

  writetxt.close();
}


void cailbration::GetOverAllCailPar(const char *inoutfilename)
{
  double par2b[CH_MAX][2];
  double par2f[CH_MAX][2];

  double tempdouble;
  double tempint;
  
  std::ifstream readtxt;
  
  readtxt.open(TString::Format("%s_OverAllCail_2b%02d.txt",inoutfilename,max_ch_b/2).Data());
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      readtxt>>tempint>>par2b[i][0]>>par2b[i][1]>>tempdouble>>tempdouble;
    }
  readtxt.close();

  readtxt.open(TString::Format("%s_OverAllCail_2f%02d.txt",inoutfilename,max_ch_f/2).Data());
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
  
  writetxt.open(TString::Format("%s_OverAllCail_2f.txt",inoutfilename).Data());
  if(!writetxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
      exit(1);
    }
  for (int i = 0; i < max_ch_f; ++i)
    {
      writetxt<<i<<"  "<<par2f[max_ch_f/2][1]*par2b[i][0]+par2f[max_ch_f/2][0]<<"  "<<par2f[max_ch_f/2][1]*par2b[i][1]<<std::endl;
    }
  for (int i = 0; i < max_ch_b; ++i)
    {
      writetxt<<i<<"  "<<par2f[i][0]<<"  "<<par2f[i][1]<<std::endl;
    }
  writetxt.close();


  writetxt.open(TString::Format("%s_OverAllCail_2b.txt",inoutfilename).Data());
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
      writetxt<<i<<"  "<<par2b[max_ch_b/2][1]*par2f[i][0]+par2b[max_ch_b/2][0]<<"  "<<par2b[max_ch_b/2][1]*par2f[i][1]<<std::endl;
    }
  writetxt.close();

}


void cailbration::TestOverAllCailEffect(const char *inputrootfilename,const char *parfilename,const char *outputrootfilename)
{
  TGraph *gxy = NULL;
  double *x,*y;
  double fx,fy;
  double bx,by;

  double par2b[2*CH_MAX][2];
  double par2f[2*CH_MAX][2];
  double tempint;
  
  std::ifstream readtxt;
  
  readtxt.open(TString::Format("%s_OverAllCail_2b.txt",parfilename).Data());
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

  readtxt.open(TString::Format("%s_OverAllCail_2f.txt",parfilename).Data());
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
