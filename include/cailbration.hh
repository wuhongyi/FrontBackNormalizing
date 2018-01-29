// cailbration.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 1月 19 12:52:39 2018 (+0800)
// Last-Updated: 二 1月 30 02:15:29 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 26
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
  // 初始化，设置正背面条数
  // par maxf 为正面channel总数
  // par maxb 为背面channel总数
  cailbration(int maxf,int maxb);
  virtual ~cailbration();

  // 对某一面进行简单自刻度
  // par inputrootfilename 为要读取的ROOT文件名，里面存放fb_xx_xx命名的TGraph数据
  // par outputname 指定输出文件的名字(不含文件后缀)
  // par ref 指定用来归一的参考条
  // par fb 指定用来归一参考条来自哪一面，false指背面，true指正面
  // par fittype 用来选择拟合方式，0只进行rob拟合，1为rob之后筛选正负范围内的数据进行pol拟合
  void SimpleCail(const char *inputrootfilename,const char *outputname,int ref,bool fb, int fittype = 0);

  // 将简单自刻度两面的刻度系数统一到一面
  // par inoutfilename 指定输入输出文件的名字(不含文件后缀)
  // par b 正面归一到背面时背面的参考条
  // par f 背面归一到正面时正面的参考条
  void GetSimpleCailPar(const char *inoutfilename,int b,int f);

  // 利用已有的数据及归一系数计算误差情况
  void TestSimpleCailEffect(const char *inputrootfilename,const char *parfilename,const char *outputrootfilename);
  
  // 对某一面进行整体自刻度
  // par inputrootfilename 为要读取的ROOT文件名，里面存放fb_xx_xx命名的TGraph数据
  // par outputname 指定输出文件的名字(不含文件后缀)
  // par fb 指定用来归一参考条来自哪一面，false指背面，true指正面
  void OverAllCail(const char *inputrootfilename,const char *outputname,bool fb);

  // 将整体自刻度两面的刻度系数统一到一面
  // par inoutfilename 指定输入输出文件的名字(不含文件后缀)
  void GetOverAllCailPar(const char *inoutfilename);

  // 利用已有的数据及归一系数计算误差情况
  void TestOverAllCailEffect(const char *inputrootfilename,const char *parfilename,const char *outputrootfilename);

  
  
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


  double p0[CH_MAX][CH_MAX];
  double p1[CH_MAX][CH_MAX];
  double e0[CH_MAX][CH_MAX];
  double e1[CH_MAX][CH_MAX];
  double w0[CH_MAX][CH_MAX];
  double w1[CH_MAX][CH_MAX];

  double P_f[CH_MAX][2],W_f[CH_MAX][2],e_f[CH_MAX][2];
  // double P_b[CH_MAX][2],W_b[CH_MAX][2],e_b[CH_MAX][2];
  float fNode[CH_MAX];
  // float bNode[CH_MAX];

  
};

#endif /* _CAILBRATION_H_ */
// 
// cailbration.hh ends here
