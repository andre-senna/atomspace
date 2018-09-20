ADD_LIBRARY (patternindex SHARED
	PatternIndexImplementationRAM
	PatternIndex
	KnowledgeBuildingBlock
)

ADD_DEPENDENCIES (patternindex
	nlp_atom_types			
)

TARGET_LINK_LIBRARIES(patternindex
	${ATOMSPACE_LIBRARIES}
)

INSTALL (FILES
	PatternIndex.h
	PatternIndexImplementationRAM.h
	KnowledgeBuildingBlock.h
	DESTINATION "include/opencog/das/patternIndex"
)

INSTALL (TARGETS patternindex DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
