!if $(AXP64)
GUID_CPL_OPTIONS=/cpl_opt:" /qap64"
!else
GUID_CPL_OPTIONS=
!endif

$(O)\guids.obj: guids.cpp
    @type <<$(ECHO_RSP)
$(ECHO_MSG_P)
<<NOKEEP
    @$(C_COMPILER_NAME) @<<$(CL_RSP) -Fo"$(MAKEDIR)\$@" $(USECXX_FLAG) "$(MAKEDIR)\$**"
$(NP_COMPILER_FLAGS: =
)
<<NOKEEP

