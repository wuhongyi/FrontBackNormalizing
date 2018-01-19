<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 五 1月 19 23:00:40 2018 (+0800)
;; Last-Updated: 五 1月 19 23:01:50 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 1
;; URL: http://wuhongyi.cn -->

# README

class **cailbration** 用于数据的正背面归一

- 拟合采用 rob 拟合，迭代保留最接近的75%的数据点，因此要求要拟合的数据在该直线上分布尽可能的均匀
- 在制作ROOT数据文件中，应对数据进行一定筛选，保证落在该直线上尽可能均匀
- 例如将坐标分成n份，Fill填充时候对他们进行筛选，保证各区间数据相当


<!-- README.md ends here -->
