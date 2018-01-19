// cailbration.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 1月 19 12:52:39 2018 (+0800)
// Last-Updated: 五 1月 19 22:55:42 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
// URL: http://wuhongyi.cn 

#ifndef _CAILBRATION_H_
#define _CAILBRATION_H_

// 本类用于数据的正背面归一
// 要求读取的ROOT文件...

// 拟合采用 rob 拟合，迭代保留最接近的75%的数据点，因此要求要拟合的数据在该直线上分布尽可能的均匀
// 在制作ROOT数据文件中，应对数据进行一定筛选，保证落在该直线上尽可能均匀
// 例如将坐标分成n份，Fill填充时候对他们进行筛选，保证各区间数据相当

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


protected:
  TFile *file;
  
  int max_ch_f;
  int max_ch_b;


  double par0[CH_MAX];
  double par1[CH_MAX];
  double err0[CH_MAX];
  double err1[CH_MAX];

  TH2I *her[CH_MAX];
  double hmean[CH_MAX];
  double hsigma[CH_MAX];
  
};

#endif /* _CAILBRATION_H_ */
// 
// cailbration.hh ends here















