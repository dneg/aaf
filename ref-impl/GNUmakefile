.PHONY : all
all :
	cd ../dodo && $(MAKE)
	cd src && $(MAKE)
	cd aafutils && $(MAKE)
	cd aafiid && $(MAKE)
	cd aaflib && $(MAKE)
	cd libjpeg && $(MAKE)
	cd plugins && $(MAKE)

.PHONY : check
check :
	cd ../test/com/ComModTestAAF && $(MAKE) check

.PHONY : clean
clean :
	cd plugins && $(MAKE) clean
	cd libjpeg && $(MAKE) clean
	cd aaflib && $(MAKE) clean
	cd aafiid && $(MAKE) clean
	cd aafutils && $(MAKE) clean
	cd src && $(MAKE) clean
	cd ../dodo && $(MAKE) clean





