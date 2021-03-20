
# This is a makefile document where the user will be able to select the version he wants to
# run. Depending on the version he chose would be need or not an input argument. The version
# can be choose by the instruction:
#
#							make versionX
#
# where the X is the version chosen. The autors are below this:

#	Raúl Calatayud Ferre
# 	Ignacio Pérez Vilaplana


COMP   = g++
CFLAGS = -Wall -O3 -std=c++11
OPENCV = `pkg-config opencv --cflags --libs`
#OBJECTS:
IMG = getImages
CAL = getCalibrated
PRO = getProjection

TAR = makefile myFrames projections					\
      $(PRO)_v1.cpp $(CAL)_v1.cpp $(IMG)_v1.cpp     \
	  $(IMG)_v2.cpp

#---------- ALL ----------#

all: version1 version2

#-------------------------#

#Versions of the calibration
getCalibrated_v1: $(CAL)_v1.cpp
	${COMP} ${CFLAGS} -o $(CAL)_v1 $(CAL)_v1.cpp ${OPENCV}
	@echo '[7m[1m'Calibration version1 - checked...'[0m'

getCalibrated_v2: $(CAL)_v2.cpp
	$(COMP) $(CFLAGS) -o $(CAL)_v2 $(CAL)_v2.cpp $(OPENCV)
	@echo '[7m[1m'Calibration version2 - checked...'[0m'

#Versions of the saving pics
getImages_v1: $(IMG)_v1.cpp
	${COMP} ${CFLAGS} -o $(IMG)_v1 $(IMG)_v1.cpp ${OPENCV}
	@echo '[7m[1m'Get Images program version 1 - checked...'[0m'

getImages_v2: $(IMG)_v2.cpp
	${COMP} ${CFLAGS} -o $(IMG)_v2 $(IMG)_v2.cpp ${OPENCV}
	@echo '[7m[1m'Get Images program version 2 - checked...'[0m'

#Version of the projections
getProjection_v1: $(PRO)_v1.cpp
	${COMP} ${CFLAGS} -o $(PRO)_v1 $(PRO)_v1.cpp ${OPENCV}
	@echo '[7m[1m'Projection program - checked...'[0m'

getProjection_v2: $(PRO)_v2.cpp
	${COMP} ${CFLAGS} -o $(PRO)_v2 $(PRO)_v2.cpp ${OPENCV}
	@echo '[7m[1m'Projection program - checked...'[0m'

#---------- VERSIONS ----------#

version1: $(CAL)_v1 $(IMG)_v1 $(PRO)_v1
	@echo '[7m[1m'Compiling version1 ...'[0m'

version2: $(IMG)_v2
	@echo '[7m[1m'Compiling version2 ...'[0m'


#---------- PHONY ----------#

run_v1:
	@echo '[7m[1m'Save images program version1 running ...'[0m'
	./$(IMG)_v1
	@echo '[7m[1m'Calibration version1 running ...'[0m'
	./$(CAL)_v1
	@echo '[7m[1m'Projecting version1 running ...'[0m'
	./$(PRO)_v1

project_v1:
	@echo '[7m[1m'Projecting version1 running ...'[0m'
	./$(PRO)_v1

###############

run_v2:
	@echo '[7m[1m'Save images program version1 running ...'[0m'
	./$(IMG)_v2
	@echo '[7m[1m'Calibration version1 running ...'[0m'
	./$(CAL)_v2
	@echo '[7m[1m'Projecting version1 running ...'[0m'
	./$(PRO)_v2

project_v2:
	@echo '[7m[1m'Projecting version1 running ...'[0m'
	./$(PRO)_v2


###############

clean: clean_v1 clean_v2
	@echo '[7m[1m'Clean done...'[0m'

clean_v1:
	@echo '[7m[1m'Cleaning version1...'[0m'
	rm $(PRO)_v1 $(CAL)_v1 $(IMG)_v1

clean_v2:
	@echo '[7m[1m'Cleaning version2...'[0m'
	rm $(IMG)_v2 $(CAL)_v2 $(IMG)_v2

tar:
	@echo '[7m[1mCompressing: $(TARF).tgz ...[0m'
	 tar cvfz augmented_reality_and_projection.tar.gz $(TAR); 							
