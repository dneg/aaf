EXCEL="c:/program files/Microsoft Office/Office/EXCEL.EXE"

all : AAFMetaDictionary.h

DictP18-277-1847BMaster.csv : DictP18-277-1847BMaster.xls
	@ echo "1) Save the new .xls file as a .csv file"
	@ echo "   Choose File/Save As ..."
	@ echo "   File name: DictP18-277-1847BMaster.csv"
	@ echo "   Save as type :CSV(OS/2 or MSDOS)"
	@ echo "2) get out of Excel"
	@ echo "   Choose File/Exit"
	@ echo "   Choose No in the anoying dialog"
	$(EXCEL) /e /r DictP18-277-1847BMaster.xls /p .

AAFMetaDictionary.h : DictP18-277-1847BMaster.csv
	awk -f mkdict.awk DictP18-277-1847BMaster.csv > AAFMetaDictionary.h

clean:
	rm -f DictP18-277-1847BMaster.csv
	rm -f AAFMetaDictionary.h
