
AAFBASE ?= ../../..


.PHONY : all
all : ComModTestAAF CreateSequence EssenceAccess

.PHONY : ComModTestAAF
ComModTestAAF :
	cd ComModTestAAF && $(MAKE)

.PHONY : CreateSequence
CreateSequence :
	cd CreateSequence && $(MAKE)

.PHONY : EssenceAccess
EssenceAccess :
	cd EssenceAccess && $(MAKE)

.PHONY : clean
clean :
	cd ComModTestAAF && $(MAKE) GENDEPS=0 $@
	cd CreateSequence && $(MAKE) GENDEPS=0 $@
	cd EssenceAccess && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd ComModTestAAF && $(MAKE) GENDEPS=0 $@
	cd CreateSequence && $(MAKE) GENDEPS=0 $@
	cd EssenceAccess && $(MAKE) GENDEPS=0 $@

