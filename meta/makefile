EXCEL="c:/program files/Microsoft Office/Office/EXCEL.EXE"

all : AAFMetaDictionary.h

AAFMetaDict.csv : AAFMetaDict.xls
	@ echo "1) Save the new .xls file as a .csv file"
	@ echo "   Choose File/Save As ..."
	@ echo "   File name: AAFMetaDict.csv"
	@ echo "   Save as type: CSV(OS/2 or MSDOS)"
	@ echo "   Choose Yes in the annoying dialog"
	@ echo "2) get out of Excel"
	@ echo "   Choose File/Exit"
	@ echo "   Choose No in the annoying dialog"
	$(EXCEL) /e /r AAFMetaDict.xls /p .

AAFMetaDictionary.h : AAFMetaDict.csv
	awk -f mkdict.awk AAFMetaDict.csv > AAFMetaDictionary.h

clean:
	rm -f AAFMetaDict.csv
	rm -f AAFMetaDictionary.h
