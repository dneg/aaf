.PHONY : all
all :
	cd src && $(MAKE)
	cd aafiid && $(MAKE)
	cd aaflib && $(MAKE)
	cd libjpeg && $(MAKE)
	cd plugins && $(MAKE)

.PHONY : check
check :
	cd ../test/com/ComModTestAAF && $(MAKE) check

.PHONY : clean
clean :
	cd plugins && $(MAKE) GENDEPS=0 $@
	cd libjpeg && $(MAKE) GENDEPS=0 $@
	cd aaflib && $(MAKE) GENDEPS=0 $@
	cd aafiid && $(MAKE) GENDEPS=0 $@
	cd src && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd plugins && $(MAKE) GENDEPS=0 $@
	cd libjpeg && $(MAKE) GENDEPS=0 $@
	cd aaflib && $(MAKE) GENDEPS=0 $@
	cd aafiid && $(MAKE) GENDEPS=0 $@
	cd src && $(MAKE) GENDEPS=0 $@

