include makeinclude

OBJ	= main.o

all: validate

validate: validate.exe
	grace mkapp validate

validate.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o validate.exe $(OBJ) $(LIBS)

clean:
	rm -f *.o *.exe
	rm -rf validate.app
	rm -f validate
	
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
