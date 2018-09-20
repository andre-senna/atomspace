ADD_EXECUTABLE (queryExample queryExample.cc)
TARGET_LINK_LIBRARIES(queryExample
	das
	patternindex
	smob
	atomspace
)

ADD_EXECUTABLE (checkKBBs checkKBBs.cc)
TARGET_LINK_LIBRARIES(checkKBBs
	das
	patternindex
	smob
	atomspace
)

ADD_EXECUTABLE (dasCreation dasCreation.cc)
TARGET_LINK_LIBRARIES(dasCreation
	das
	patternindex
	smob
	atomspace
)
