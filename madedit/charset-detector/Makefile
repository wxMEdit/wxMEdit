TARGET=chardetect.la
CXXFLAGS=-c \
	-O2 \
	-fno-rtti -fno-exceptions -nostdinc++ \
	-D_REENTRANT -I./src -I./include

SUBDIRS=src
DESTLIBDIR=/usr/lib/pcmanx/addons

all: $(TARGET)

$(TARGET):
	@for mdir in $(SUBDIRS); do \
	  if test -d $$mdir; then \
	    $(MAKE) -C $$mdir ; \
	  fi; \
	done
	libtool --mode=compile $(CXX) $(CXXFLAGS) src/entry/impl.cpp
	libtool --tag=CC --mode=link gcc -O2 -o $(TARGET) \
		impl.lo src/*.lo \
	        -rpath $(DESTLIBDIR) -avoid-version -module \
		-export-symbols src/entry/export-symbol.list \
		-lstdc++
	mv -f .libs/chardetect.so .

clean:
	@for mdir in $(SUBDIRS); do \
	  if test -d $$mdir; then \
	    $(MAKE) -C $$mdir clean ; \
	  fi; \
	done
	rm -f *.o *.lo *.la *.so
	rm -rf .libs

