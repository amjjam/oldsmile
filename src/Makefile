FLAGS=-g -Wall -I ../include -I /usr/lib/openmpi/include -I /usr/include/openmpi-x86_64  -pg
CPPFLAGS=$(FLAGS)
CPP=g++
MPICPP=mpic++

WRITEMODEL_SRC=writeModel.C model.C pModel.C model1.C model2.C model3.C model4.C modelMHD.C boundary.C boundaryE.C boundaryTF.C modelShue.C
WRITEMODEL_OBJ=$(WRITEMODEL_SRC:.C=.o)

MAKEIMAGES_SRC=makeImages.C orbit.C orbit1.C orbit2.C orbit3.C orbitSMILE.C model.C pModel.C model1.C model2.C model3.C model4.C modelB.C modelMHD.C boundary.C boundaryE.C boundaryTF.C boundaryShue2.C modelShue.C camera.C camera1.C image.C stringutils.C
MAKEIMAGES_OBJ=$(MAKEIMAGES_SRC:.C=.o)

WRITEORBIT_SRC=writeOrbit.C orbit.C orbit3.C
WRITEORBIT_OBJ=$(WRITEORBIT_SRC:.C=.o)

MAKESHUEIMAGES_SRC=makeShueImages.C orbit.C orbitSMILE.C model.C pModel.C modelMHD.C modelShue.C camera.C camera1.C image.C stringutils.C
MAKESHUEIMAGES_OBJ=$(MAKESHUEIMAGES_SRC:.C=.o)

TESTFITMODELTOIMAGE_SRC=testFitModelToImage.C fitModelToImage.C orbit.C orbitSMILE.C model.C pModel.C modelMHD.C modelShue.C camera.C camera1.C image.C stringutils.C
TESTFITMODELTOIMAGE_OBJ=$(TESTFITMODELTOIMAGE_SRC:.C=.o)

SAMPLEXEMS_SRC=sampleXems.C model.C modelMHD.C boundary.C boundaryTF.C
SAMPLEXEMS_OBJ=$(SAMPLEXEMS_SRC:.C=.o)

TESTFITMODELATOMHDIMAGE_SRC=testFitModelAToMHDImage.C fitModelToImage.C orbit.C orbitSMILE.C model.C pModel.C modelMHD.C modelA.C modelAX.C camera.C camera1.C image.C stringutils.C boundary.C boundaryShue2.C poissonImage.C
TESTFITMODELATOMHDIMAGE_OBJ=$(TESTFITMODELATOMHDIMAGE_SRC:.C=.o)

TESTFITMODELBTOMHDIMAGE_SRC=testFitModelBToMHDImage.C fitModelToImage.C orbit.C orbitSMILE.C model.C pModel.C modelMHD.C modelB.C modelBX.C camera.C camera1.C image.C stringutils.C boundary.C boundaryShue2.C poissonImage.C
TESTFITMODELBTOMHDIMAGE_OBJ=$(TESTFITMODELBTOMHDIMAGE_SRC:.C=.o)

WRITESMILEORBIT_SRC=writeSMILEOrbit.C orbit.C orbitSMILE.C stringutils.C
WRITESMILEORBIT_OBJ=$(WRITESMILEORBIT_SRC:.C=.o)

TESTMODELS_SRC=testModels.C model.C modelMHD.C boundary.C boundaryTF.C
TESTMODELS_OBJ=$(TESTMODELS_SRC:.C=.o)

LOADDATA_SRC=loadData.C model.C modelMHD.C boundary.C boundaryTF.C orbit.C orbitSMILE.C stringutils.C
LOADDATA_OBJ=$(LOADDATA_SRC:.C=.o)

BINS=writeModel makeImages writeOrbit makeShueImages testFitModelToImage \
	sampleXems testFitModelAToMHDImage writeSMILEOrbit testModels \
	loadData

all: $(BINS)

writeModel: $(WRITEMODEL_OBJ)
	$(CPP) -static -o $@ $^ -L ../lib/ -laVec -lz -lm -lgsl
makeImages: $(MAKEIMAGES_OBJ)
	$(MPICPP) -o $@ $^ -L ../lib/ -laVec -laTime -lm -lz -lrt \
	-L /usr/lib/openmpi/lib/ -L /usr/lib64/openmpi/lib -L /usr/local/lib -lmpi -lgsl \
	-lgslcblas
writeOrbit: $(WRITEORBIT_OBJ)
	$(CPP) -static -o $@ $^ -L ../lib/ -laVec -laTime -lm -lz
makeShueImages: $(MAKESHUEIMAGES_OBJ)
	$(MPICPP) -o $@ $^ -L ../lib/ -laVec -laTime -lm -lz -lrt \
	-L /usr/lib/openmpi/lib/ -L /usr/lib64/openmpi/lib -lmpi -lgsl \
	-lgslcblas
testFitModelToImage: $(TESTFITMODELTOIMAGE_OBJ)
	$(MPICPP) -o $@ $^ -L ../lib/ -laVec -laTime -lm -lz -lrt \
	-L /usr/lib/openmpi/lib/ -L /usr/lib64/openmpi/lib -lmpi -lgsl \
	-lgslcblas
sampleXems: $(SAMPLEXEMS_OBJ)
	$(CPP) -o $@ $^ -L ../lib/ -laVec -lm -lgsl -lgslcblas -lrt
testFitModelAToMHDImage: $(TESTFITMODELATOMHDIMAGE_OBJ)
	$(MPICPP) -o $@ $^ -L ../lib/ -laVec -laTime -lm -lz -lrt \
	-L /usr/lib/openmpi/lib/ -L /usr/lib64/openmpi/lib/ -lmpi -lgsl \
	-lgslcblas -pg -O3

testFitModelBToMHDImage: $(TESTFITMODELBTOMHDIMAGE_OBJ)
	$(MPICPP) -o $@ $^ -L ../lib/ -laVec -laTime -lm -lz -lrt \
	-L /usr/lib/openmpi/lib/ -L /usr/lib64/openmpi/lib/ -lmpi -lgsl \
	-lgslcblas -pg -O3

writeSMILEOrbit: $(WRITESMILEORBIT_OBJ)
	$(CPP) -o $@ $^ -L ../lib/ -laVec -lm -laTime

testModels: $(TESTMODELS_OBJ)
	$(CPP) -o $@ $^ -L ../lib/ -laVec -lm -laTime -lgsl -lgslcblas -lrt

loadData: $(LOADDATA_OBJ)
	$(CPP) -o $@ $^ -L ../lib/ -laVec -lm -laTime -lgsl -lgslcblas -lrt

clean:
	-rm -f $(BINS)
	-rm -f *.o
