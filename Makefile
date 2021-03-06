EXECUTABLE=compress
EDITOR=gedit
CC=g++
CFLAGS=-c -O3 -Wall -pedantic
LDFLAGS=
SOURCES=source/main.cpp source/encoder.cpp source/decoder.cpp source/struct/shan_table.class.cpp source/struct/shan_row.class.cpp
OBJECTS=$(SOURCES:.cpp=.o)
SOURLIBS=$(SOURCES:.cpp=.h)

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm source/*.o source/struct/*.o

open: 
	$(EDITOR) $(SOURCES) $(SOURLIBS) &


test:
	cp ./$(EXECUTABLE) testes/$(EXECUTABLE)
	echo "   200m.rar compress/decompress"	
	time testes/$(EXECUTABLE) <  testes/c1
	time testes/$(EXECUTABLE) <  testes/d1
	md15sum testes/200m.rar testes/200m2.rar
	echo "   300k.jpg compress/decompress"	
	time testes/$(EXECUTABLE) <  testes/c2
	time testes/$(EXECUTABLE) <  testes/d2
	md5sum testes/300k.jpg testes/300k2.jpg
	echo "   500m.zip compress/decompress"	
	time testes/$(EXECUTABLE) < testes/c3
	time testes/$(EXECUTABLE) < testes/d3
	md5sum testes/500m.zip testes/500m2.zip
	echo "    2500k.txt compress/decompress"	
	time testes/$(EXECUTABLE) < testes/c4
	time testes/$(EXECUTABLE) < testes/d4
	md5sum testes/2500k.txt testes/2500k2.txt
	

pdf:
	pdflatex relatorio.tex
	pdflatex relatorio.tex 
