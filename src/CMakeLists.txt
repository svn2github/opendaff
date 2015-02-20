set(daff_SRCS   
	DAFFReader.cpp       
	DAFFSCTransform.cpp  
	DAFFMetadataImpl.cpp  
	DAFFReaderImpl.cpp  
	Utils.cpp
	DAFFUtils.cpp     
) 
include_directories(
	${DAFF_SOURCE_DIR}/include
	${DAFF_SOURCE_DIR}/src
)

# Es wird festgelegt, dass es sich um eine gemeinsam genutzt Bibliothek
# handelt. Um eine statische Bibliothek zu bekommen, wird
# SHARED durch STATIC ersetzt.
add_library (daff SHARED ${daff_SRCS})

install(TARGETS daff
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
