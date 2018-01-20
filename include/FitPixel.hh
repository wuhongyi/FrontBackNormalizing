// FitPixel.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 1月 20 22:18:31 2018 (+0800)
// Last-Updated: 日 1月 21 00:36:48 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#ifndef _FITPIXEL_H_
#define _FITPIXEL_H_

class TGraph;
class TH1I;
class TH1D;
class TH2I;

class FitPixel
{
public:
  FitPixel(int n,double *x,double *y);
  virtual ~FitPixel();

  void SetRobust(int rob = -1);// 0-100


  
public:
  inline double GetParameter0() { return par0; }
  inline double GetParameter1() { return par1; }
  inline double GetParError0() { return err0; }
  inline double GetParError1() { return err1; }
  inline double GetParWeight0() { return 1.0/err0/err0; };
  inline double GetParWeight1() { return 1.0/err1/err1; };

  inline int GetPointN() { return pointn; }

protected:
  void FitRobust();


  
private:
  TGraph *orig;
  
  double par0;
  double par1;
  double err0;
  double err1;

  TH2I *h2_y_er;
  TH1D *h1_er;
  
private:
  double *xx;
  double *yy;
  int pointn;

  int robust;
};

#endif /* _FITPIXEL_H_ */
// 
// FitPixel.hh ends here









