ADD_CXXTEST(DistributedAtomSpaceUTest)
TARGET_LINK_LIBRARIES(DistributedAtomSpaceUTest
	das
	patternindex
	atomspace
)

ADD_CXXTEST(PatternIndexImplementationRAMUTest)
TARGET_LINK_LIBRARIES(PatternIndexImplementationRAMUTest
	patternindex
	atomspace
)

ADD_CXXTEST(CompoundHashValueUTest)
TARGET_LINK_LIBRARIES(CompoundHashValueUTest
	das
	patternindex
	atomspace
)
