INCLUDE_DIRECTORIES(.)
FIND_PACKAGE(CUDA REQUIRED)

CUDA_INCLUDE_DIRECTORIES(/usr/include/thrust/)

set(ROOT_DICT_INPUT_HEADERS RooMyPdf.h LinkDef.h)
GEN_ROOT_DICT_SOURCE(RooMyPdfDict.cxx)
cuda_add_executable(main main.cpp RooMyPdf.cxx cuda_calculate.cu ${ROOT_DICT_OUTPUT_SOURCES})
target_link_libraries(main RooFit RooFitCore)

