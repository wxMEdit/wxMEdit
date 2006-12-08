CXX = g++
CXXFLAGS = \
	-c -I./tables \
	-O2 -Wall \
	-fno-rtti -fno-exceptions -nostdinc++ \
	-D_REENTRANT
LDFLAGS =

SRCS = \
	CharDistribution.cpp \
	JpCntx.cpp \
	LangBulgarianModel.cpp \
	LangCyrillicModel.cpp \
	LangGreekModel.cpp \
	LangHebrewModel.cpp \
	LangHungarianModel.cpp \
	LangThaiModel.cpp \
	nsBig5Prober.cpp \
	nsCharSetProber.cpp \
	nsEUCJPProber.cpp \
	nsEUCKRProber.cpp \
	nsEUCTWProber.cpp \
	nsEscCharsetProber.cpp \
	nsEscSM.cpp \
	nsGB2312Prober.cpp \
	nsHebrewProber.cpp \
	nsLatin1Prober.cpp \
	nsMBCSGroupProber.cpp \
	nsMBCSSM.cpp \
	nsSBCSGroupProber.cpp \
	nsSBCharSetProber.cpp \
	nsSJISProber.cpp \
	nsUTF8Prober.cpp \
	nsUniversalDetector.cpp

OBJS=$(SRCS:%.cpp=%.o)

all: $(OBJS)

$(OBJS): %.o: %.cpp
	libtool --mode=compile $(CXX) $(CXXFLAGS) $<

clean:
	rm -f *.o
	rm -f *.lo
	rm -rf .libs
