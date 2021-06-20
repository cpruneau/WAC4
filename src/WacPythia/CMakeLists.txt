################################################################################################
# Project WAC/Pythia
################################################################################################

ROOT_GENERATE_DICTIONARY(G__WacPythia PythiaEventGenerator.hpp PythiaEventReader.hpp PythiaConfiguration.hpp  LINKDEF WacPythiaLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(WacPythia SHARED PythiaEventGenerator.cpp PythiaEventReader.cpp PythiaConfiguration.cpp    G__WacPythia.cxx)
target_link_libraries(WacPythia  Base PYTHIA8_LIB EGPYTHIA8  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(WacPythia  PUBLIC  Base WacPythia ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libWacPythia.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libWacPythia_rdict.pcm" DESTINATION "$ENV{WAC_LIB}")
install(TARGETS WacPythia  LIBRARY DESTINATION "$ENV{WAC_LIB}") 
