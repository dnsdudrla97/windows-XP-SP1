/* Copyright 1990-1997. Microsoft Corporation. */
#include "typedefs.h"  /* for Far declaration in ttofmt.h header */
#include "ttofmt.h"
/* from TTOBASE.FMT 1.2	*/
 
char * g_BASEFmt[] = {
  "DEFINE ZERO=0",
  "DEFINE MAXCOUNT=0xFFFF",
  "BASEHeader, HEAD {",
  "fixed32, 0x00010000",
  "Offset, Axis",
  "Offset, Axis",
  "}",
  "DeviceTableFormat1, TABLE {",
  "uint16",
  "uint16",
  "uint16, 1",
  "Array, $T2 - $T1+1, packedint2",
  "}",
  "DeviceTableFormat2, TABLE {",
  "uint16",
  "uint16",
  "uint16, 2",
  "Array, $T2 - $T1+1, packedint4",
  "}",
  "DeviceTableFormat3, TABLE {",
  "uint16",
  "uint16",
  "uint16, 3",
  "Array, $T2 - $T1+1, packedint8",
  "}",
  "DeviceTable, CLASS, , $B4 {",
  "DeviceTableFormat1",
  "DeviceTableFormat2",
  "DeviceTableFormat3",
  "}",
  "Axis, TABLE {",
  "Offset, BaseTagList",
  "Offset, BaseScriptList, NOTNULL",
  "}",
  "BaseTagList, TABLE {",
  "Count, 1, MAXCOUNT",
  "Array, $T1, Tag",
  "}",
  "BaseScriptList, TABLE {",
  "Count, 1, MAXCOUNT",
  "Array, $T1, BaseScriptRecord",
  "}",
  "BaseScriptRecord, RECORD {",
  "Tag",
  "Offset, BaseScript, NOTNULL",
  "}",
  "BaseScript, TABLE {",
  "Offset, BaseValues",
  "Offset, MinMax",
  "Count",
  "Array, $T3, BaseLangSysRecord",
  "}",
  "BaseLangSysRecord, RECORD {",
  "Tag",
  "Offset, MinMax, NOTNULL",
  "}",
  "BaseValues, TABLE {",
  "uint16",
  "Count",
  "Array, $T2, Offset, BaseCoord, NOTNULL",
  "}",
  "MinMax, TABLE {",
  "Offset, BaseCoord",
  "Offset, BaseCoord",
  "Count",
  "Array, $T3, FeatMinMaxRecord",
  "}",
  "FeatMinMaxRecord, RECORD {",
  "Tag",
  "Offset, BaseCoord",
  "Offset, BaseCoord",
  "}",
  "BaseCoordFormat1, TABLE {",
  "uint16, 1",
  "int16",
  "}",
  "BaseCoordFormat2, TABLE {",
  "uint16, 2",
  "int16",
  "GlyphID, 0, MAXGLYPHID",
  "uint16",
  "}",
  "BaseCoordFormat3, TABLE {",
  "uint16, 3",
  "int16",
  "Offset, DeviceTable, NOTNULL",
  "}",
  "BaseCoord, CLASS, , $B0 {",
  "BaseCoordFormat1",
  "BaseCoordFormat2",
  "BaseCoordFormat3",
  "}",
  0
};
