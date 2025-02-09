# Need TeX Live 2023

TEX=latexmk -f -xelatex
BIB=bibtex8

%.pdf: %.tex 
	$(TEX) $<

syllabus.pdf: syllabus.tex

.PHONY: clean pdfclean
clean:
	rm -f *.aux *.dvi *.fls *.fdb_latexmk *.xdv *.out *.log

pdfclean:
	rm -f *.pdf 

