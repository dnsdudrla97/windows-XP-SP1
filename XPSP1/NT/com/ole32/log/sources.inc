!IF 0

Copyright (c) 1999  Microsoft Corporation

Module Name:

    sources.

Abstract:

    This file specifies the target component being built and the list of
    sources files needed to build that component.  Also specifies optional
    compiler switches and libraries that are unique for the component being
    built.

!ENDIF

#
# This is the name of the target built from the source files specified
# below.  The name should include neither the path nor the file extension.
#
TARGETNAME=   dbglog

INCLUDES  =  $(INCLUDES);..\..\ih

#
# This specifies where the target is to be built.  A private target of
# type LIBRARY or DYNLINK should go to obj, whereas a public target of
# type LIBRARY or DYNLINK should go to $(BASEDIR)\public\sdk\lib.
#

TARGETPATH=   obj

#
# This specifies the type of the target, such as PROGRAM, DYNLINK, LIBRARY,
# etc.
#
TARGETTYPE=   LIBRARY

SOURCES=      ..\log.cxx

UMTYPE=       windows
UMAPPL=
UMTEST=