#!/bin/sh
pdflatex main.tex >> /dev/null
bibtex main
pdflatex main.tex