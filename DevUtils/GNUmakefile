
.PHONY : all
all : dump

.PHONY : dump
dump :
	cd Dump && $(MAKE)

.PHONY : clean
clean :
	cd Dump && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd Dump && $(MAKE) GENDEPS=0 $@

