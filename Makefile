### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: 五 1月 19 12:14:48 2018 (+0800)
## Last-Updated: 五 1月 19 12:58:00 2018 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 6
## URL: http://wuhongyi.cn 


OBJ = wu

MainFile = main.cc

# ------------------

SourceFile := $(wildcard $(shell pwd)/src/*.c $(shell pwd)/src/*.cc $(shell pwd)/src/*.C $(shell pwd)/src/*.cpp $(shell pwd)/src/*.cxx)
#IncludeFile := $(wildcard $(shell pwd)/include/*.h $(shell pwd)/include/*.hh $(shell pwd)/include/*.hpp)

# ------------------

GXX = g++ 

ROOTCFLAGS  = $(shell root-config --cflags) # -pthread -std=c++11 -m64   dir include
ROOTLIBS    = $(shell root-config --libs)  # dir lib   -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic
ROOTGLIBS   = $(shell root-config --glibs) # dir lib   -lGui -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic
ROOTADDLIBS = -lGeom -lSpectrum -lGQt -lQtGSI -lQtRoot -lXMLParser 

QT4_INC = -I/usr/include/Qt
QT4_LIB = -L/usr/lib64/lib -lQtCore -lQtGui

# ------------------

## 用户补充需要加载的库
DIR_INC = -I$(shell pwd)/include $(ROOTCFLAGS)
DIR_LIB = $(ROOTLIBS) $(ROOTGLIBS) $(ROOTADDLIBS)
CFLAGS  = -g -Wall -O2  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

# ------------------

all:$(OBJ)

$(OBJ): $(MainFile) $(SourceFile)
	$(GXX) $(DIR_INC) $(CFLAGS) $(DIR_LIB) -o $@ $(MainFile) $(SourceFile)
	@echo "=============================================================="
	@echo "$@ done !"
	@echo "=============================================================="
clean:
	@echo "Removing linked and compiled files......" 
	rm -f *.o *~ $(OBJ)



######################################################################
### Makefile ends here
