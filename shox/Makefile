include makeinclude

OBJ	= main.o

all: shox

shox: $(OBJ)
	$(LD) $(LDFLAGS) -o shox $(OBJ) $(LIBS)

clean:
	rm -f *.o
	rm -f shox

allclean: clean
	rm -f makeinclude configure.paths platform.h
	
install: all
	./makeinstall

makeinclude:
	@echo please run ./configure
	@false

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<
