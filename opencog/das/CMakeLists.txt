ADD_SUBDIRECTORY (PatternIndex)
ADD_SUBDIRECTORY (IntegrationTests)
ADD_SUBDIRECTORY (AtomeseParser)

ADD_LIBRARY (das SHARED
	DistributedAtomSpace
	CacheManager
	CompoundHashValue
	PatternIndexSCMBuilder
	SCMLoader
	CartesianProductGenerator
	utils
)

INSTALL (FILES
	DistributedAtomSpace.h
	CacheManager.h
	CompoundHashValue.h
	CartesianProductGenerator.h
	utils.h
	DESTINATION "include/opencog/das"
)

INSTALL (TARGETS das DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
