
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
	cd ComModTestAAF && $(MAKE) clean
	cd CreateSequence && $(MAKE) clean
	cd EssenceAccess && $(MAKE) clean
	$(RMDIR) $(OBJDIR)/*

