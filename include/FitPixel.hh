// FitPixel.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 1月 20 22:18:31 2018 (+0800)
// Last-Updated: 日 1月 21 13:33:06 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 14
// URL: http://wuhongyi.cn 

#ifndef _FITPIXEL_H_
#define _FITPIXEL_H_

#include "TString.h"

class TGraph;
class TH1;
class TH1I;
class TH1D;
class TH2I;

class FitPixel
{
public:
  FitPixel(int n,double *x,double *y);
  virtual ~FitPixel();

  // 用于区分归一类型，pixel的正背面信息等
  void SetName(const char *name);
  void SetName(const TString name);
  
  void SetRobust(int rob = -1);// 0-100
  void FitRobust();
  TGraph *GetRobustFitGraph() { return orig; }
  
  void CalculateRobustFitEffectHistogram();
  TH2I *GetRobustFitEffectTH2() { return h2_y_er; }
  TH1I *GetRobustFitEffectTH1() { return h1_er; }

  
public:
  inline double GetParameter0() { return par0; }
  inline double GetParameter1() { return par1; }
  inline double GetParError0() { return err0; }
  inline double GetParError1() { return err1; }
  inline double GetParWeight0() { return 1.0/err0/err0; };
  inline double GetParWeight1() { return 1.0/err1/err1; };

  inline int GetPointN() { return pointn; }

protected:
  void GetRangeAbove(TH1 *h,double thre,int &left,int &right);
  
protected:
  TString nameofsave;


  
private:
  TGraph *orig;
  
  double par0;
  double par1;
  double err0;
  double err1;

  TH2I *h2_y_er;
  TH1I *h1_er;
  double h1_er_mean;
  double h1_er_sigma;
  
private:
  double *xx;
  double *yy;
  int pointn;

  int robust;
};

#endif /* _FITPIXEL_H_ */
// 
// FitPixel.hh ends here










