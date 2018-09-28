ADD_CXXTEST(DistributedAtomSpaceUTest)
TARGET_LINK_LIBRARIES(DistributedAtomSpaceUTest
	das
	patternindex
	smob
	atomspace
)

ADD_CXXTEST(PatternIndexImplementationRAMUTest)
TARGET_LINK_LIBRARIES(PatternIndexImplementationRAMUTest
	das
	patternindex
	smob
	atomspace
)

ADD_CXXTEST(CompoundHashValueUTest)
TARGET_LINK_LIBRARIES(CompoundHashValueUTest
	das
	patternindex
	smob
	atomspace
)

ADD_CXXTEST(KnowledgeBuildingBlockUTest)
TARGET_LINK_LIBRARIES(KnowledgeBuildingBlockUTest
	das
	patternindex
	smob
	atomspace
)
