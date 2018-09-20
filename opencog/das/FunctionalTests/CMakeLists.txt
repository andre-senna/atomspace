ADD_EXECUTABLE (queryExample queryExample.cc SCMLoader.cc PatternIndexBuilder.cc)
TARGET_LINK_LIBRARIES(queryExample
	das
	patternindex
	smob
	atomspace
)

ADD_EXECUTABLE (checkKBBs checkKBBs.cc SCMLoader.cc PatternIndexBuilder.cc)
TARGET_LINK_LIBRARIES(checkKBBs
	das
	patternindex
	smob
	atomspace
)
