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
	cd OM && $(MAKE) clean
	cd impl && $(MAKE) clean
	cd com-api && $(MAKE) clean

