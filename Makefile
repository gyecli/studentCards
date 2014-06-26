CXX = u++					# compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD			# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = config.o printer.o bank.o parent.o truck.o bottlingPlant.o nameServer.o vendingMachine.o student.o  \
watcardOffice.o watcard.o driver.o	# object files forming executable
DEPENDS = ${OBJECTS:.o=.d}		# substitute ".o" with ".d"
EXEC = soda				# executable name

.PHONY : clean					# not file names

${EXEC} : ${OBJECTS}				# link step
	${CXX} ${CXXFLAGS} $^ -o $@		# additional object files before $^

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

# make implicitly generates rules to compile C++ files that generate .o files

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}
    
