include makeinclude

OBJ	= main.o

all: compilerflags.h interactive

compilerflags.h: makeinclude
	./mkflags

interactive: $(OBJ)
	$(LD) $(LDFLAGS) -o interactive $(OBJ) $(LIBS)

clean:
	rm -f *.o
	rm -f interactive

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
