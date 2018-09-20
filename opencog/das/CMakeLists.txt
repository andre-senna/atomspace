ADD_SUBDIRECTORY (PatternIndex)
ADD_SUBDIRECTORY (IntegrationTests)

ADD_LIBRARY (das SHARED
	DistributedAtomSpace
	CacheManager
	CompoundHashValue
	PatternIndexSCMBuilder
	SCMLoader
	utils
)

INSTALL (FILES
	DistributedAtomSpace.h
	CacheManager.h
	CompoundHashValue.h
	utils.h
	DESTINATION "include/opencog/das"
)

INSTALL (TARGETS das DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")
