// cailbration.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 1月 19 12:52:39 2018 (+0800)
// Last-Updated: 日 1月 21 00:00:27 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 15
// URL: http://wuhongyi.cn 

#ifndef _CAILBRATION_H_
#define _CAILBRATION_H_

// 本类用于数据的正背面归一
// 要求读取的ROOT文件...

class FitPixel;

class TH2I;
class TFile;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define CH_MAX (128)

class cailbration
{
public:
  // par rootfilename 为要读取的ROOT文件名，里面存放fb_xx_xx命名的TGraph数据
  // par maxf 为正面channel总数
  // par maxb 为背面channel总数
  cailbration(const char *rootfilename,int maxf,int maxb);
  virtual ~cailbration();

  // par outputname 指定输出文件的名字
  // par ref 指定用来归一的参考条
  // par fb 指定用来归一参考条来自哪一面，false指背面，true指正面
  // par verbose 用来展示冗余输出，数值越大输出越多
  void SimpleCail(const char *outputname,int ref,bool fb, int verbose = 0);
  
private:
  FitPixel *fitpixel;
  

protected:
  TFile *file;
  
  int max_ch_f;
  int max_ch_b;


  double par0[CH_MAX];
  double par1[CH_MAX];
  double err0[CH_MAX];
  double err1[CH_MAX];
  double prob;
  
  TH2I *her[CH_MAX];
  double hmean[CH_MAX];
  double hsigma[CH_MAX];
  
};

#endif /* _CAILBRATION_H_ */
// 
// cailbration.hh ends here















