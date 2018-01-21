// SelectData.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 1月 21 16:47:17 2018 (+0800)
// Last-Updated: 日 1月 21 21:30:14 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#ifndef _SELECTDATA_H_
#define _SELECTDATA_H_

// 应当考虑X Y两个方向上都是均匀的

#include "TString.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TGraph;
class TFile;
class TH1I;

class SelectData
{
public:
  SelectData(const char *rootfilename,int nx,int ny);
  virtual ~SelectData();

  void ReservedUniformityData(const char *outputrootfilename);
  

protected:

  int ChX;
  int ChY;

  TFile *filein;
  TFile *fileout;

  TH1I *hx;
  TGraph *gxy;
  TGraph *gsxy;
};

#endif /* _SELECTDATA_H_ */
// 
// SelectData.hh ends here
