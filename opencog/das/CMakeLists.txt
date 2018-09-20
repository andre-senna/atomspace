ADD_SUBDIRECTORY (PatternIndex)
ADD_SUBDIRECTORY (FunctionalTests)

ADD_LIBRARY (das SHARED
	DistributedAtomSpace
	CacheManager
	CompoundHashValue
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
