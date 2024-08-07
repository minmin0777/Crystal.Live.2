#######################################################################
# Makro for precompiled header
#######################################################################
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "$(IntDir)/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    # generate the precompiled header
    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Zm500 /Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                            OBJECT_OUTPUTS "${PrecompiledBinary}")

    # set the usage of this header only to the other files than rc
    FOREACH(fname ${Sources})
        IF ( NOT ${fname} MATCHES ".*rc$" )
            SET_SOURCE_FILES_PROPERTIES(${fname}
                                        PROPERTIES COMPILE_FLAGS "/Zm500 /Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                                    OBJECT_DEPENDS "${PrecompiledBinary}")
        ENDIF( NOT ${fname} MATCHES ".*rc$" )
    ENDFOREACH(fname)

    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)