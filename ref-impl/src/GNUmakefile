.PHONY : all
all : OM impl com-api


.PHONY : OM
OM :
	@echo "\nBuilding OM\n"
	cd OM && $(MAKE)


.PHONY : impl
impl : OM
	@echo "\nBuilding impl\n"
	cd impl && $(MAKE)


.PHONY : com-api
com-api : OM impl
	@echo "\nBuilding com-api\n"
	cd com-api && $(MAKE)


.PHONY : clean
clean : 
	cd OM && $(MAKE) GENDEPS=0 $@
	cd impl && $(MAKE) GENDEPS=0 $@
	cd com-api && $(MAKE) GENDEPS=0 $@


.PHONY : realclean
realclean :
	cd OM && $(MAKE) GENDEPS=0 $@
	cd impl && $(MAKE) GENDEPS=0 $@
	cd com-api && $(MAKE) GENDEPS=0 $@


