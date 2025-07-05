# Need TeX Live 2023

TEX=latexmk -f -xelatex
BIB=bibtex8

all: mobdesign.pdf

mobdesign.pdf: mobdesign.tex mobdesign.bib

%.pdf: %.tex 
	$(TEX) $<

.PHONY: clean

clean:
	rm *.aux *.bbl *.blg *.bcf *.dvi *.fls *.fdb_latexmk *.xdv *.pdf *.log *.out *.run.xml
