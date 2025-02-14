/*++

Copyright (c) 1992  Microsoft Corporation

Module Name:

    dntext.c

Abstract:

    Translatable text for DOS based NT installation program.

Author:

    Ted Miller (tedm) 30-March-1992

Revision History:

--*/


#include "winnt.h"


//
// Name of sections in inf file.  If these are translated, the section
// names in dosnet.inf must be kept in sync.
//

CHAR DnfDirectories[]       = "Directories";
CHAR DnfFiles[]             = "Files";
CHAR DnfFloppyFiles0[]      = "FloppyFiles.0";
CHAR DnfFloppyFiles1[]      = "FloppyFiles.1";
CHAR DnfFloppyFiles2[]      = "FloppyFiles.2";
CHAR DnfFloppyFiles3[]      = "FloppyFiles.3";
CHAR DnfFloppyFilesX[]      = "FloppyFiles.x";
CHAR DnfSpaceRequirements[] = "DiskSpaceRequirements";
CHAR DnfMiscellaneous[]     = "Miscellaneous";
CHAR DnfRootBootFiles[]     = "RootBootFiles";
CHAR DnfAssemblyDirectories[] = SXS_INF_ASSEMBLY_DIRECTORIES_SECTION_NAME_A;


//
// Names of keys in inf file.  Same caveat for translation.
//

CHAR DnkBootDrive[]     = "BootDrive";      // in [SpaceRequirements]
CHAR DnkNtDrive[]       = "NtDrive";        // in [SpaceRequirements]
CHAR DnkMinimumMemory[] = "MinimumMemory";  // in [Miscellaneous]

CHAR DntMsWindows[]   = "Microsoft Windows";
CHAR DntMsDos[]       = "MS-DOS";
CHAR DntPcDos[]       = "PC-DOS";
CHAR DntOs2[]         = "OS/2";
CHAR DntPreviousOs[]  = "¨¦¦ç£¤¦ ¢ «¦¬¨ ¡æ ©ç©«£ ©«¦ C:";

CHAR DntBootIniLine[] = "¡«á©«©/¤á£ © «à¤ Windows XP";

//
// Plain text, status msgs.
//

CHAR DntStandardHeader[]      = "\n ¡«á©«© «à¤ Windows XP\nÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ";
CHAR DntPersonalHeader[]      = "\n ¡«á©«© «à¤ Windows XP Personal\nÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ";
CHAR DntWorkstationHeader[]   = "\n ¡«á©«© «à¤ Windows XP Professional\nÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ";
CHAR DntServerHeader[]        = "\n ¡«á©«© «à¤ Windows 2002 Server\nÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ";
CHAR DntParsingArgs[]         = "¤á¢¬© §¨£â«¨à¤...";
CHAR DntEnterEqualsExit[]     = "ENTER=ë¥¦¦ª";
CHAR DntEnterEqualsRetry[]    = "ENTER=§¤á¢¯";
CHAR DntEscEqualsSkipFile[]   = "ESC=¨á¢ ¯ ¨®å¦¬";
CHAR DntEnterEqualsContinue[] = "ENTER=¬¤â® ";
CHAR DntPressEnterToExit[]    = " ¡«á©«© ¤ å¤  ¬¤«æ ¤ ©¬¤®å© .  â©« ENTER   â¥¦¦.";
CHAR DntF3EqualsExit[]        = "F3=ë¥¦¦ª";
CHAR DntReadingInf[]          = "¤á¤à© «¦¬ ¨®å¦¬ INF %s...";
CHAR DntCopying[]             = "³¤« ¨­ã: ";
CHAR DntVerifying[]           = "³§¢ã¬©: ";
CHAR DntCheckingDiskSpace[]   = "ë¢®¦ª ®é¨¦¬ ©«¦ å©¡¦...";
CHAR DntConfiguringFloppy[]   = " £æ¨­à©  ©¡â«ª...";
CHAR DntWritingData[]         = "¨­ã §¨£â«¨à¤ £¦¨­¦§¦å©ª...";
CHAR DntPreparingData[]       = "¦¨ ©£æª §¨£â«¨à¤ £¦¨­¦§¦å©ª...";
CHAR DntFlushingData[]        = "¡¡â¤à© ¦£â¤à¤ © å©¡¦...";
CHAR DntInspectingComputer[]  = "¤á¢¬© ¬§¦¢¦ ©«ã...";
CHAR DntOpeningInfFile[]      = "ê¤¦ £ ¨®å¦¬ INF...";
CHAR DntRemovingFile[]        = "«á¨© ¨®å¦¬ %s";
CHAR DntXEqualsRemoveFiles[]  = "X=«á¨© ¨®å¦¬";
CHAR DntXEqualsSkipFile[]     = "X=¨á¢ ¯ ¨®å¦¬";

//
// confirmation keystroke for DnsConfirmRemoveNt screen.
// Kepp in sync with DnsConfirmRemoveNt and DntXEqualsRemoveFiles.
//
ULONG DniAccelRemove1 = (ULONG)'x',
      DniAccelRemove2 = (ULONG)'X';

//
// confirmation keystroke for DnsSureSkipFile screen.
// Kepp in sync with DnsSureSkipFile and DntXEqualsSkipFile.
//
ULONG DniAccelSkip1 = (ULONG)'x',
      DniAccelSkip2 = (ULONG)'X';

CHAR DntEmptyString[] = "";

//
// Usage text.
//

PCHAR DntUsage[] = {

    "¡ ©«á « Windows 2002 Server ã « Windows XP Professional.",
    "",
    "",
    "WINNT [/s[: ¨¦£ã §¨¦â¢¬©ª]] [/t[:£¦¤á]]",
    "	   [/u[:¨®å¦ §æ¡¨ ©ª]] [/udf:id[,UDF_¨®å¦]]",
    "	   [/r:­á¡¢¦ª] [/r[x]:­á¡¢¦ª] [/e:¤«¦¢ã] [/a]",
    "",
    "",
    "/s[: ¨¦£ã §¨¦â¢¬©ª]",
    "   ¦¨å  « â© §¨¦â¢¤©ª «à¤ ¨®åà¤ «à¤ Windows.",
    "    â© §¨â§  ¤ å¤  £  §¢ã¨ª  ¨¦£ã £ ©ç¤«¥ x:\\[ ¨¦£ã] ã ",
    "	\\\\ ¡¦£ ©«ãª\\¡¦ ¤æ®¨©«¦[\\ ¨¦£ã].",
    "",
    "/t[:£¦¤á]",
    "   ¦å «¦ §¨æ¨££ ¡«á©«©ª   «¦§¦â«© «à¤ §¨¦©à¨ ¤é¤ ¨®åà¤ ©«¤ ¡¦¨ ©£â¤",
    "   £¦¤á ¡  ¡«á©«© «à¤ Windows XP © ¬«ã¤ « £¦¤á. á¤ ¤ ¡¦¨ ©«å £  â©",
    "   «¦ §¨æ¨££ ¡«á©«©ª  § ® ¨ã©  «¦¤ ¤«¦§ ©£æ £ ª £¦¤áª.",
    "",
    "/u[:¨®å¦ §æ¡¨ ©ª]",
    "   ¡«¢å «¦ §¨æ¨££ ¡«á©«©ª ®à¨åª §¨¡¦¢¦ç© £ ®¨ã© ¤æª ¨®å¦¬ §æ¡¨ ©ª",
    "   (§ «å«  /s). ¦ ¨®å¦ §æ¡¨ ©ª §¨â®  §¤«ã© ª © ¡á§¦ ª ã æ¢ª « ª ¨à«ã© ª",
    "   ¦  ¦§¦åª ¬§¦á¢¢¦¤« ©¬¤ãàª ©«¦¤ «¢ ¡æ ®¨ã©« ¡«á «  á¨¡  «ª ¡«á©«©ª.",
    "",
    "/udf:id[,UDF_¨®å¦]	",
    "   §¦ ¡¤ç  â¤ ¤¤à¨ ©« ¡æ (id) «¦ ¦§¦å¦ ®¨© £¦§¦ å «¦ §¨æ¨££",
    "   ¡«á©«©ª   ¤ ¡¦¨å©  §àª â¤ ¨®å¦ UDF (Uniqueness Database",
    "   File) «¨¦§¦§¦ å â¤ ¨®å¦ §æ¡¨ ©ª (å« /u).  §¨á£«¨¦ª /udf",
    "	¤« ¡ ©«á « ª « £âª ©«¦ ¨®å¦ §æ¡¨ ©ª ¡  «¦ ¤¤à¨ ©« ¡æ",
    "   ¡¦¨å  §¦ âª « £âª «¦¬ ¨®å¦¬ UDF ®¨© £¦§¦ ¦ç¤« . á¤ ¤",
    "   ¡¦¨ ©«å ¨®å¦ UDF, «¦ §¨æ¨££ ¡«á©«©ª ©áª «á ¤ ",
    "   «¦§¦«ã©« £   ©¡â«  ¦§¦å ¤ §¨ â®  «¦ ¨®å¦ $Unique$.udb.",   
    "",
    "/r[:­á¡¢¦ª]",
    "	¦¨å  â¤ §¨¦ ¨« ¡æ ­á¡¢¦ §¨¦ª ¡«á©«©. ",
    "	­á¡¢¦ª §¨£â¤  £«á «¦ «â¢¦ª «¦¬ §¨¦¨á££«¦ª ¡«á©«©ª.",
    "",
    "/rx[:­á¡¢¦ª]",
    "	¦¨å  â¤ §¨¦ ¨« ¡æ ­á¡¢¦ §¨¦ª ¡«á©«©.  ­á¡¢¦ª",
    "	¡«¨å«  £«á «¦ «â¢¦ª «¦¬ §¨¦¨á££«¦ª ¡«á©«©ª.",
    "",
    "/e ¦¨å  £  ¤«¦¢ã §¨¦ª ¡«â¢© £«á «¦ ¨­ ¡æ «£ã£ «¦¬ ",
    "   §¨¦¨á££«¦ª ¡«á©«©ª.",
    "",
    "/a	¤¨¦§¦ å « ª § ¢¦âª   á«¦£ £   ¡âª ¤á¡ª.",
     NULL
};

//
//  Inform that /D is no longer supported
//
PCHAR DntUsageNoSlashD[] = {

    "¡ ©«á « Windows XP.",
    "",
    "WINNT [/S[:] ¨¦£ã§¨¦â¢¬©ª] [/T[:]£¦¤á] [/I[:]¨®å¦INF]",
    "      [/O[X]] [/X | [/F] [/C]] [/B] [/U[:¨®å¦â©£ª¤¨ é¤]]",
    "      [/R[X]:¡«á¢¦¦ª] [/E:¤«¦¢ã] [/A]",
    "",
    "/D[:]winntroot",
    "       ¬«ã  § ¢¦ã ¤ ¬§¦©«¨å«  § .",
    NULL
};

//
// out of memory screen
//

SCREEN
DnsOutOfMemory = { 4,6,
                   { " £¤ã£ ¤ §¨¡å ¡  «¦ §¨æ¨££ ¡«á©«©ª ¤ å¤  ¬¤«æ ¤ ©¬¤®å© .",
                     NULL
                   }
                 };

//
// Let user pick the accessibility utilities to install
//

SCREEN
DnsAccessibilityOptions = { 3, 5,
{   "§ ¢¦ã «à¤ ¦« ¡é¤ §¨¦¨££á«à¤ §¨¦ª ¡«á©«©:",
    DntEmptyString,
    "[ ]  â©« F1   «¦ ¬¤« ¡æ ­¡æ «ª Microsoft",
#ifdef NARRATOR
    "[ ]  â©« F2   «¦¤ ­«ã «ª Microsoft",
#endif
#if 0
    "[ ]  â©« F3   «¦ §¢¡«¨¦¢æ ¦ ¦æ¤ª «ª Microsoft",
#endif
    NULL
}
};

//
// User did not specify source on cmd line screen
//

SCREEN
DnsNoShareGiven = { 3,5,
{ "¦ §¨æ¨££ ¡«á©«©ª ®¨ á«  « â© «à¤ ¨®åà¤ «à¤ Windows XP.",
  " ©á« «  ¨¦£ã «ª â©ª æ§¦¬ ¨å©¡¦¤«  « ¨®å «à¤ Windows XP.",
  NULL
}
};


//
// User specified a bad source path
//

SCREEN
DnsBadSource = { 3,5,
                 { " ¡¦¨ ©£â¤ §¨¦â¢¬© ¤ å¤  â¡¬¨, ¤ å¤  §¨¦©§¢á© £ ã ¤ §¨ â® ",
                   "£  â¡¬¨ ¡«á©«© «à¤ Windows XP.  ©á« £  ¤â  ¨¦£ã â©ª ©«¤ ¦§¦å",
                   "§¨â§  ¤ ¨å©¡¦¤«  « ¨®å «à¤ Windows XP. ¨© £¦§¦ ã©« «¦ §¢ã¡«¨¦ BACKSPACE",
                   "  ¤  ¨á¯« «¦¬ª ®¨¡«ã¨ª ¡  §¢¡«¨¦¢¦ã©« ©« ©¬¤â®  «  ¨¦£ã.",
                   NULL
                 }
               };


//
// Inf file can't be read, or an error occured parsing it.
//

SCREEN
DnsBadInf = { 3,5,
              { "_¤ ã«¤ ¬¤«ã  ¤á¤à© §æ «¦ §¨æ¨££ ¡«á©«©ª «¦¬ ¨®å¦¬ §¢¨¦­¦¨ é¤ (INF)",
                "ã «¦ ¨®å¦ â®  ¡«©«¨­å. § ¡¦ ¤à¤ã©« £ «¦  ® ¨ ©«ã «¦¬ ©¬©«ã£«æª ©ª.",
                NULL
              }
            };

//
// The specified local source drive is invalid.
//
// Remember that the first %u will expand to 2 or 3 characters and
// the second one will expand to 8 or 9 characters!
//
SCREEN
DnsBadLocalSrcDrive = { 3,4,
{ " ¡¦¨ ©£â¤ £¦¤á   « §¨¦©à¨ ¤á ¨®å «ª ¡«á©«©ª ¤ å¤ ",
  "£  â¡¬¨ £¦¤á ã ¤ §¨ â®  «¦¬¢á® ©«¦¤ %u  (%lu byte)",
  " â© £¦¬ ®é¨¦¬.",
  NULL
}
};

//
// No drives exist that are suitable for the local source.
//
// Remeber that the %u's will expand!
//
SCREEN
DnsNoLocalSrcDrives = { 3,4,
{  " Windows XP § «¦ç¤ â¤ «æ£¦ ©«¦ ©¡¢¨æ å©¡¦ «¦¬¢á® ©«¦¤ %u ",
   "(%lu byte)  â© £¦¬ ®é¨¦¬.  ¡«á©«©  ®¨© £¦§¦ ã© ",
   "£â¨¦ª «¦¬ ®é¨¦¬   §¨¦©à¨ ¤ã §¦ã¡¬© «à¤ ¨®åà¤ ¡«á «¤ ¡«á©«©.",
   " £¦¤á å©¡¦¬ §¨â§  ¤ ¨å©¡«  © â¤ «¦§ ¡æ ©¡¢¨æ å©¡¦ ©«¨á ",
   "¬§¦©«¨ æ£¤¦ §æ « Windows XP ¡  ¤ §¨â§  ¤ å¤  ©¬£§ ©£â¤.",
   DntEmptyString,
   "¦ §¨æ¨££ ¡«á©«©ª ¤ ã«¤ ¬¤«æ ¤ ¤«¦§å©  «â«¦ ¦ å©¡¦ ¦",
   "¦§¦å¦ª ¤  â«  ¢ç¨¦ «¦¤ § «¦ç£¤¦ ®é¨¦.",
  NULL
}
};

SCREEN
DnsNoSpaceOnSyspart = { 3,5,
{ " ¢ç¨¦ª ®é¨¦ª ©« £¦¤á ©ª ¡¡å¤©ª (©¬¤ãàª C:) ¤ §¨¡å",
  "  ¢ «¦¬¨å ®à¨åª  ©¡â«.   ¢ «¦¬¨å ®à¨åª  ©¡â« § «å",
  "«¦¬¢á® ©«¦¤ 3,5 MB (3.641.856 byte)  â© £¦¬ ®é¨¦¬ © ¡å¤ « £¦¤á.",
  NULL
}
};

//
// Missing info in inf file
//

SCREEN
DnsBadInfSection = { 3,5,
                     { " ¤æ«« [%s] «¦¬ ¨®å¦¬ §¢¨¦­¦¨ é¤ «¦¬ §¨¦¨á££«¦ª ¡«á©«©ª ¤",
                       "¬§á¨®  ã â®  ¡«©«¨­å. § ¡¦ ¤à¤ã©« £ «¦  ® ¨ ©«ã «¦¬ ©¬©«ã£«æª ©ª.",
                       NULL
                     }
                   };


//
// Couldn't create directory
//

SCREEN
DnsCantCreateDir = { 3,5,
                     { " ¡«á©«© ¤ ã«¤ ¬¤«æ ¤ £ ¦¬¨ã©  «¦¬ª §¨¡á«à ¡«¢æ¦¬ª ©« £¦¤á §¨¦¦¨ ©£¦ç.",
                       DntEmptyString,
                       "%s",
                       DntEmptyString,
                       "¢â¥« « £¦¤á §¨¦¦¨ ©£¦ç ¡   àå« æ«  ¤ ¬§á¨®¦¬¤ ¨®å £ «¦ å ¦ æ¤¦£",
                       "«¦¬ ¡«¢æ¦¬ §¨¦¦¨ ©£¦ç. ¢â¥« §å©ª ¤  ©ç¤© «à¤ ¡¢àåà¤",
                       "å¤  ©à©«ã.",
                       NULL
                     }
                   };

//
// Error copying a file
//

SCREEN
DnsCopyError = { 4,5,
{  " ¡«á©«© ¤ ã«¤ ¬¤«æ ¤ ¤« ¨á¯  «¦ §¨¡á«à ¨®å¦:",
   DntEmptyString,
   DntEmptyString,          // see DnCopyError (dnutil.c)
   DntEmptyString,
   DntEmptyString,
   "  â©« ENTER   §¤á¢¯ «ª ¢ «¦¬¨åª ¤« ¨­ãª.",
   "  â©« ESC   ¤æ© «¦¬ ©­á¢£«¦ª ¡  ©¬¤â® © «ª ¡«á©«©ª.",
   "  â©« F3   â¥¦¦ §æ «¦ §¨æ¨££ ¡«á©«©ª.",
   DntEmptyString,
   "£åà©: M ¤æ© «¦¬ ©­á¢£«¦ª ¡  ©¬¤â® © «ª ¡«á©«©ª, å©àª",
   "¤« £«à§å©« á¢¢ §¨¦¢ã£« ¡«á «  á¨¡  «ª ¡«á©«©ª.",
   NULL
}
},
DnsVerifyError = { 4,5,
{  "¦ ¤«å¨­¦ «¦¬ ¨®å¦¬ §¦¬ £ ¦ç¨© «¦ §¨æ¨££ ¡«á©«©ª ¤ å¤ ",
   "å ¦ £ «¦ ¨® ¡æ ¨®å¦. í©àª ¬«æ ¤ ¦­å¢«  © ©­á¢£« «¦¬  ¡«ç¦¬,",
   "§¨¦¢ã£« «ª  ©¡â«ª ã á¢¢ §¨¦¢ã£« ©®« ¡á £ «¦ ¬¢ ¡æ.",
   DntEmptyString,
   DntEmptyString,          // see DnCopyError (dnutil.c)
   DntEmptyString,
   DntEmptyString,
   "  â©« ENTER   §¤á¢¯ «ª ¢ «¦¬¨åª ¤« ¨­ãª.",
   "  â©« ESC   ¤æ© «¦¬ ©­á¢£«¦ª ¡  ©¬¤â® © «ª ¡«á©«©ª.",
   "  â©« F3   â¥¦¦ §æ «¦ §¨æ¨££ ¡«á©«©ª.",
   DntEmptyString,
   "£åà©: M ¤æ© «¦¬ ©­á¢£«¦ª ¡  ©¬¤â® © «ª ¡«á©«©ª, å©àª",
   "¤« £«à§å©« á¢¢ §¨¦¢ã£« ¡«á «  á¨¡  «ª ¡«á©«©ª.",
   NULL
}
};

SCREEN DnsSureSkipFile = { 4,5,
{  "á¤ ¤¦ã©« «¦ ©­á¢£ ¤  £ ¦¬¨å ¤«å¨­¦ «¦¬ ¨®å¦¬.",
   "¬«ã  § ¢¦ã §¨¦¦¨å«    §¨¦®à¨£â¤¦¬ª ®¨ã©«ª ¦  ¦§¦å¦  ¡«¤¦¦ç¤",
   "« ª ©¬¤â§ ª §æ «¤ â¢¢ ¯ ¨®åà¤ «¦¬ ©¬©«ã£«¦ª.",
   DntEmptyString,
   "  â©« ENTER   §¤á¢¯ «ª ¢ «¦¬¨åª ¤« ¨­ãª.",
   "  â©« I   ¤æ© «¦¬ ¨®å¦¬.",
   DntEmptyString,
   "£åà©: M ¤æ© «¦¬ ©­á¢£«¦ª ¡  ©¬¤â® © «ª ¡«á©«©ª, å©àª",
   "¤ £¤ å¤   ¡«á©«© ã ¤á£ © © Windows XP £ § «¬®å.",
  NULL
}
};

//
// Wait while setup cleans up previous local source trees.
//

SCREEN
DnsWaitCleanup =
    { 12,6,
        { "¨ £â¤« æ©¦  ¡«á©«© ¡«¨å « §¨¦¦ç£¤ §¨¦©à¨ ¤á ¨®å.",
           NULL
        }
    };

//
// Wait while setup copies files
//

SCREEN
DnsWaitCopying = { 13,6,
                   { "¨ £â¤« æ©¦  ¡«á©«© ¤« ¨á­  « ¨®å ©«¦ å©¡¦.",
                     NULL
                   }
                 },
DnsWaitCopyFlop= { 13,6,
                   { "¨ £â¤« æ©¦  ¡«á©«© ¤« ¨á­  « ¨®å ©«  ©¡â«.",
                     NULL
                   }
                 };

//
// Setup boot floppy errors/prompts.
//

SCREEN
DnsNeedFloppyDisk3_0 = { 4,4,
{  "¦ §¨æ¨££ ¡«á©«©ª § «å «¨ ª ¡¤âª  ©¡â«ª ¬¯¢ãª §¬¡¤æ««ª",
   "£¦¨­¦§¦ £â¤ª.  å¤«  ¤­¦¨á © ¬«âª àª\"¡«á©«© «à¤ Windows XP",
   " ©¡â« ¡¡å¤©ª,\" \"¡«á©«© «à¤ Windows XP  ©¡â« #2,\" \"¡«á©«© «à¤",
   " Windows XP  ©¡â« #3\" and \"¡«á©«© «à¤ Windows XP  ©¡â« #4.\"",
   DntEmptyString,
   "¦§¦«ã©« £å §æ ¬«âª « ª «â©©¨ ª  ©¡â«ª ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« 4\"",
   NULL
}
};

SCREEN
DnsNeedFloppyDisk3_1 = { 4,4,
{  "¦§¦«ã©« £  ¡¤ã  ©¡â« ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤ ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« 4\"",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk2_0 = { 4,4,
{  "¦§¦«ã©« £  ¡¤ã  ©¡â« ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤ ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« 3\"",
  NULL
}
};

SCREEN
DnsNeedFloppyDisk1_0 = { 4,4,
{  "¦§¦«ã©« £  ¡¤ã  ©¡â« ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤ ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« 2\"",
   NULL
}
};

SCREEN
DnsNeedFloppyDisk0_0 = { 4,4,
{  "¦§¦«ã©« £  ¡¤ã  ©¡â« ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤ ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« ¡¡å¤©ª\"",  
   NULL
}
};

SCREEN
DnsNeedSFloppyDsk3_0 = { 4,4,
{  "¦ §¨æ¨££ ¡«á©«©ª § «å «¨ ª ¡¤âª  ©¡â«ª ¬¯¢ãª §¬¡¤æ««ª",
   "£¦¨­¦§¦ £â¤ª.  å¤«  ¤­¦¨á © ¬«âª àª\"¡«á©«© «à¤ Windows XP",
   " ©¡â« ¡¡å¤©ª,\" \"¡«á©«© «à¤ Windows XP  ©¡â« #2,\" \"¡«á©«©",
   "«à¤ Windows XP  ©¡â« #3\" ¡  \"¡«á©«© «à¤ Windows XP  ©¡â« #4.\"",
   DntEmptyString,
   "¦§¦«ã©« £å §æ ¬«âª « ª «â©©¨ ª  ©¡â«ª ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« 4\"",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk3_1 = { 4,4,
{  "¦§¦«ã©« £  ¡¤ã  ©¡â« ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤ ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« 4\"",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk2_0 = { 4,4,
{  "¦§¦«ã©« £  ¡¤ã  ©¡â« ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤ ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« 3\"",
  NULL
}
};

SCREEN
DnsNeedSFloppyDsk1_0 = { 4,4,
{  "¦§¦«ã©« £  ¡¤ã  ©¡â« ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤ ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« 2\"",
   NULL
}
};

SCREEN
DnsNeedSFloppyDsk0_0 = { 4,4,
{  "¦§¦«ã©« £  ¡¤ã  ©¡â« ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤ ©« £¦¤á A:.",
   "¬«ã   ©¡â«  å¤  \"¡«á©«© «à¤ Windows XP  ©¡â« ¡¡å¤©ª\"",
  NULL
}
};

//
// The floppy is not formatted.
//
SCREEN
DnsFloppyNotFormatted = { 3,4,
{ "¬«ã   ©¡â« ¤ å¤  ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤   ®¨ã© £ MS-DOS",
  "ã å¤  ¡«©«¨££â¤. ¤ å¤  ¬¤«ã  ®¨ã© «ª §æ «¤ ¡«á©«©.",
  NULL
}
};

//
// We think the floppy is not formatted with a standard format.
//
SCREEN
DnsFloppyBadFormat = { 3,4,
{ "¬«ã   ©¡â« ¤ å¤  ¬¯¢ãª §¬¡¤æ««ª, £¦¨­¦§¦ £â¤   ®¨ã© £ MS-DOS",
  "ã å¤  ¡«©«¨££â¤. ¤ å¤  ¬¤«ã  ®¨ã© «ª §æ «¤ ¡«á©«©.",
  NULL
}
};

//
// We can't determine the free space on the floppy.
//
SCREEN
DnsFloppyCantGetSpace = { 3,4,
{ "¦ §¨æ¨££ ¡«á©«©ª ¤ å¤  ¬¤«æ ¤ §¨¦© ¦¨å©  «¦¤ ¢ç¨¦ ®é¨¦",
  "©«  ©¡â« §¦¬ «¦§¦«ã©«. ¤ å¤  ¬¤«ã  ®¨ã© «ª §æ «¤ ¡«á©«©.",
  NULL
}
};

//
// The floppy is not blank.
//
SCREEN
DnsFloppyNotBlank = { 3,4,
{ "  ©¡â« §¦¬ «¦§¦«ã©« ¤ å¤  ¬¯¢ãª §¬¡¤æ««ª ã ¤ å¤  ¡¤ã.",
  "¤ å¤  ¬¤«ã  ®¨ã© ¬«ãª «ª  ©¡â«ª §æ «¤ ¡«á©«©.",
  NULL
}
};

//
// Couldn't write the boot sector of the floppy.
//
SCREEN
DnsFloppyWriteBS = { 3,4,
{ "¤ ã«¤ ¬¤«ã  ¨­ã §æ «¤ ¡«á©«© ©«¤ §¨ ¦®ã ©¬©«ã£«¦ª «ª",
  " ©¡â«ª. ¤ å¤  ¬¤«ã  ®¨ã© ¬«ãª «ª  ©¡â«ª §æ «¤ ¡«á©«©.",
  NULL
}
};

//
// Verify of boot sector on floppy failed (ie, what we read back is not the
// same as what we wrote out).
//
SCREEN
DnsFloppyVerifyBS = { 3,4,
{ " ¤á¤à© ¦£â¤à¤ §æ «¤ ¡«á©«© «ª §¨ ¦®ãª ©¬©«ã£«¦ª «ª  ©¡â«ª",
  "¤ « ¨ á  £ «¤ ¨­ã ¦£â¤à¤ ã ¤ ã«¤ ¬¤«ã  ¤á¤à© §æ «¤",
  "¡«á©«© «ª §¨ ¦®ãª ©¬©«ã£«¦ª «ª  ©¡â«ª   «¤ §¢ã¬©ã «¦¬ª.",
  DntEmptyString,
  "¬«æ ¦­å¢«  © £å ã §¨ ©©æ«¨ª §æ « ª §¨¡á«à ©¬¤ã¡ª:",
  DntEmptyString,
  "   ¬§¦¢¦ ©«ãª â®  §¨¦©¢å §æ â¤  æ.",
  "   «¦§¦«£â¤  ©¡â« å¤  ¡«©«¨££â¤.",
  "   £¦¤á  ©¡â«ª â®  â¤ §¨æ¢£ ¬¢ ¡¦ç ã ¨ç£ ©ª §¨£â«¨à¤.",
  NULL
}
};


//
// We couldn't write to the floppy drive to create winnt.sif.
//

SCREEN
DnsCantWriteFloppy = { 3,5,
{ "¤ ã«¤ ¬¤«ã  ¨­ã ©«  ©¡â« ©« £¦¤á A:. í©àª   ©¡â«",
  "¤ å¤  ¡«©«¨££â¤. ¦¡ £á©« £   ­¦¨« ¡ã  ©¡â«.",
  NULL
}
};


//
// Exit confirmation dialog
//

SCREEN
DnsExitDialog = { 13,6,
                  { "ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»",
                    "º   Windows XP ¤ â®¦¬¤ ¡«©«å §¢ã¨àª       º",
                    "º  ©«¦ ©ç©«£á ©ª. ¤ ¡¢å©« «¦ §¨æ¨££         º",
                    "º  ¡«á©«©ª «é¨  §¨â§  ¤ ¡«¢©«å §á¢     º",
                    "º    ¤ ¡«©«ã©« « Windows XP.               º",
                    "º     * «ã©« ENTER ¤ ©¬¤®å©« «¤ ¡«á©«©. º",
                    "º     * «ã©« F3 ¤ ¡¢å©« «¤ ¡«á©«©.      º",
                    "ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶",
                    "º  F3=ë¥¦¦ª  ENTER=¬¤â®                          º",
                    "ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼",
                    NULL
                  }
                };


//
// About to reboot machine and continue setup
//

SCREEN
DnsAboutToRebootW =
{ 3,5,
{ "¦ «£ã£ «ª ¡«á©«©ª §¦¬ ©å«  © MS-DOS ¦¢¦¡¢¨é¡.",
  "¦ §¨æ¨££ ¡«á©«©ª  ¥¡ ¤ã©  §á¢  «¦¤ ¬§¦¢¦ ©«ã ©ª.",
  "   ¡©å ¡«á©«©ª «à¤ Windows XP  ©¬¤®å©  £«á «¤ §¤¡¡å¤©.",
  DntEmptyString,
  " àå« æ«    ©¡â« ©ª \"¡«á©«© «à¤ Windows XP", 
  " ©¡â« ¡¡å¤©ª\" ¨å©¡«  ©« £¦¤á A: §¨¦«¦ç ©¬¤®å©«.",
  DntEmptyString,
  " â©« ENTER   §¤¡¡å¤© «¦¬ ¬§¦¢¦ ©«ã ¡  ©¬¤â® © «ª ¡«á©«©ª",
  "«à¤ Windows XP.",
  NULL
}
},

DnsAboutToRebootS =  
{ 3,5,
{ "¦ «£ã£ «ª ¡«á©«©ª §¦¬ ©å«  © MS-DOS ¦¢¦¡¢¨é¡.",
  "¦ §¨æ¨££ ¡«á©«©ª  ¥¡ ¤ã©  §á¢  «¦¤ ¬§¦¢¦ ©«ã ©ª.",
  "   ¡©å ¡«á©«©ª «à¤ Windows XP  ©¬¤®å©  £«á «¤ §¤¡¡å¤©.",
  DntEmptyString,
  " àå« æ«    ©¡â« ©ª \"¡«á©«© «à¤ Windows XP", 
  " ©¡â« ¡¡å¤©ª\" ¨å©¡«  ©« £¦¤á A: §¨¦«¦ç ©¬¤®å©«.",
  DntEmptyString,
  " â©« ENTER   §¤¡¡å¤© «¦¬ ¬§¦¢¦ ©«ã ¡  ©¬¤â® © «ª ¡«á©«©ª",
  "«à¤ Windows XP.",
 NULL
}
},
DnsAboutToRebootX =
{ 3,5,
{ "¦ «£ã£ «ª ¡«á©«©ª §¦¬ ©å«  © MS-DOS ¦¢¦¡¢¨é¡.",
  "¦ §¨æ¨££ ¡«á©«©ª  ¥¡ ¤ã©  §á¢  «¦¤ ¬§¦¢¦ ©«ã ©ª.",
  "   ¡©å ¡«á©«©ª «à¤ Windows XP ©¬¤®å«  £«á «¤ §¤¡¡å¤©.",
  DntEmptyString,
  "á¤ ¬§á¨®  £   ©¡â« ©« £¦¤á A:, ­ ¨â©« «¤ «é¨.",
  DntEmptyString,
  " â©« ENTER   §¤¡¡å¤© «¦¬ ¬§¦¢¦ ©«ã ¡  ©¬¤â® © «ª ¡«á©«©ª",
  "«à¤ Windows XP.",
  NULL
}
};


//
// Need another set for '/w' switch since we can't reboot from within Windows.
//

SCREEN
DnsAboutToExitW =
{ 3,5,
{ "¦ «£ã£ «ª ¡«á©«©ª §¦¬ ©å«  © MS-DOS ¦¢¦¡¢¨é¡.",
  "¦ §¨æ¨££ ¡«á©«©ª  ¥¡ ¤ã©  §á¢  «¦¤ ¬§¦¢¦ ©«ã ©ª.",
  "   ¡©å ¡«á©«©ª «à¤ Windows XP ©¬¤®å«  £«á «¤ §¤¡¡å¤©.",
  DntEmptyString,
  " àå« æ«    ©¡â« ©ª \"¡«á©«© «à¤ Windows XP", 
  " ©¡â« ¡¡å¤©ª\" ¨å©¡«  ©« £¦¤á A: §¨¦«¦ç ©¬¤®å©«.",
  DntEmptyString,
  " â©« ENTER   § ©«¨¦­ã © MS-DOS, §¤¡¡ ¤ã©« «¦¤ ¬§¦¢¦ ©«ã ¡ ",
  "©¬¤®å©« «¦ §¨æ¨££ ¡«á©«©ª «à¤ Windows XP.",
  NULL
}

},
DnsAboutToExitS =
{ 3,5,
{ "¦ «£ã£ «ª ¡«á©«©ª §¦¬ ©å«  © MS-DOS ¦¢¦¡¢¨é¡.",
  "¦ §¨æ¨££ ¡«á©«©ª  ¥¡ ¤ã©  §á¢  «¦¤ ¬§¦¢¦ ©«ã ©ª.",
  "   ¡©å ¡«á©«©ª «à¤ Windows XP ©¬¤®å«  £«á «¤ §¤¡¡å¤©.",
  DntEmptyString,
  " àå« æ«    ©¡â« ©ª \"¡«á©«© «à¤ Windows XP", 
  " ©¡â« ¡¡å¤©ª\" ¨å©¡«  ©« £¦¤á A: §¨¦«¦ç ©¬¤®å©«.",
  DntEmptyString,
  " â©« ENTER   § ©«¨¦­ã © MS-DOS, §¤¡¡ ¤ã©« «¦¤ ¬§¦¢¦ ©«ã ¡ ",
  "©¬¤®å©« «¦ §¨æ¨££ ¡«á©«©ª «à¤ Windows XP.",
  NULL
}
},
DnsAboutToExitX =
{ 3,5,
{ "¦ «£ã£ «ª ¡«á©«©ª §¦¬ ©å«  © MS-DOS ¦¢¦¡¢¨é¡.",
  "¦ §¨æ¨££ ¡«á©«©ª  ¥¡ ¤ã©  §á¢  «¦¤ ¬§¦¢¦ ©«ã ©ª.",
  "   ¡©å ¡«á©«©ª «à¤ Windows XP ©¬¤®å«  £«á «¤ §¤¡¡å¤©.",
  DntEmptyString,
  "á¤ ¬§á¨®  £   ©¡â« ©« £¦¤á A:, ­ ¨â©« «¤ «é¨.",
  DntEmptyString,
  " â©« ENTER   § ©«¨¦­ã © MS-DOS, §¤¡¡ ¤ã©« «¦¤ ¬§¦¢¦ ©«ã ¡ ",
  "©¬¤®å©« «¦ §¨æ¨££ ¡«á©«©ª «à¤ Windows XP.",
  NULL
}
};


//
// Gas gauge
//

SCREEN
DnsGauge = { 7,15,
             { "ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»",
               "º ¦ §¨æ¨££ ¡«á©«©ª ¤« ¨á­  ¨®å...                 º",
               "º                                                                º",
               "º      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿      º",
               "º      ³                                                  ³      º",
               "º      ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ      º",
               "ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼",
               NULL
             }
           };


//
// Error screens for initial checks on the machine environment
//

SCREEN
DnsBadDosVersion = { 3,5,
{ "¬«æ «¦ §¨æ¨££ § «å MS-DOS â¡¦©ª 5.0 ã ¤æ«¨ª.",
  NULL
}
},

DnsRequiresFloppy = { 3,5,
#ifdef ALLOW_525
{ "¦ §¨æ¨££ ¡«á©«©ª ¤«æ§ © æ«   £¦¤á A: ¤ ¬§á¨®  ã å¤ ",
  "£¦¤á ®£¢ãª §¬¡¤æ««ª.   «¤ ¡«â¢© «¦¬ §¨¦¨á££«¦ª ¡«á©«©ª",
  "§ «å«  £  £¦¤á  ©¡â«ª 1,2  ã ¬¯¢æ«¨.",
#else
{"¦ §¨æ¨££ ¡«á©«©ª ¤«æ§ © æ«   £¦¤á A: ¤ ¬§á¨®  ã å¤ ",
 "3,5  ¤«©é¤ ¬¯¢ãª §¬¡¤æ««ª. § «å«  £  £¦¤á A:  ®à¨« ¡æ««ª 1,44 ",
 "ã ¤é«¨ª   «¤ ¡«á©«© §æ  ©¡â«ª.",
 DntEmptyString,
"  «¤ ¡«á©«© «à¤ Windows XP ®à¨åª « ®¨ã©  ©¡«é¤, ¥¡ ¤ã©« §á¢  «¦",
"§¨æ¨££ ¡  ¡¦¨å©« «¤ § ¢¦ã /b ©« ¨££ã ¤«¦¢é¤.",
#endif
  NULL
}
},

DnsRequires486 = { 3,5,
{ "¦ §¨æ¨££ ¡«á©«©ª ¤«æ§ © æ«  ¬«æª ¦ ¬§¦¢¦ ©«ãª ¤  â« ",
  "§¥¨©«ã 80486 ã ¤æ«¨¦. ¤ å¤  ¬¤«ã  ¡«â¢© «à¤ Windows XP.",
  NULL
}
},

DnsCantRunOnNt = { 3,5,
{ "¤ å¤  ¬¤«ã  ¡«â¢© «¦¬ §¨¦¨á££«¦ª © ¡£å â¡¦© 32 bit «à¤ Windows.",
  DntEmptyString,
  "¨© £¦§¦ ã©« winnt32.exe.",
  NULL
}
},

DnsNotEnoughMemory = { 3,5,
{ "¦ §¨æ¨££ ¡«á©«©ª ¤«æ§ © æ«   ¡«©«£â¤ £¤ã£ «¦¬ ¬§¦¢¦ ©«ã",
  "¤ §¨¡å   «¤ ¡«â¢© «à¤ Windows XP.",
  DntEmptyString,
  "¤ã£ §¦¬ § «å« : %lu%s Mb",
  "¤ã£ §¦¬ ¤«¦§å©«¡: %lu%s Mb",
  NULL
}
};

//
// Screens used when removing existing nt files
//
SCREEN
DnsConfirmRemoveNt = { 5,5,
{   "¦ §¨æ¨££ ¡«á©«©ª  ¡«¨ã©  « ¨®å «à¤ Windows XP §æ",
    "«¦¤ ¡¦¨ æ£¤¦ §¨¡á«à ¡«á¢¦¦.  ¡«á©«© «à¤ Windows XP ©",
    "¬«æ¤ «¦¤ ¡«á¢¦¦  ¡«©«¨­å ¦¨ ©« ¡á.",
    DntEmptyString,
    "%s",
    DntEmptyString,
    DntEmptyString,
    "  â©« F3   â¥¦¦ §æ «¦ §¨æ¨££ ¡«á©«©ª ®à¨åª ¡«á¨© ¨®åà¤.",
    "  â©«    ¡«á¨© «à¤ ¨®åà¤ «à¤ Windows §æ «¦¤ §¨§á¤à ¡«á¢¦¦.",
    NULL
}
},

DnsCantOpenLogFile = { 3,5,
{ "¤ ã«¤ ¬¤«æ ¤ ¤¦ ®«å «¦ §¨¡á«à ¨®å¦ ¡«¨­ãª §æ «¤ ¡«á©«©.",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "¦ §¨æ¨££ ¡«á©«©ª ¤ å¤  ¬¤«æ ¤ ¡«¨ã©  « ¨®å «à¤", 
  "Windows XP §æ «¦¤ ¡¦¨ ©£â¤¦ ¡«á¢¦¦.",
  NULL
}
},

DnsLogFileCorrupt = { 3,5,
{ "¤ å¤  ¬¤«ã ç¨© §æ «¦ §¨æ¨££ ¡«á©«©ª «ª ¤æ««ª %s",
  "©«¦ ¡¦¨ æ£¤¦ §¨¡á«à ¨®å¦ ¡«¨­ãª «ª ¡«á©«©ª.",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "¦ §¨æ¨££ ¡«á©«©ª ¤ å¤  ¬¤«æ ¤ ¡«¨ã©  « ¨®å «à¤", 
  "Windows XP §æ «¦¤ ¡¦¨ ©£â¤¦ ¡«á¢¦¦.",
  NULL
}
},

DnsRemovingNtFiles = { 3,5,
{ "           ¨ £â¤« æ©¦ ¡«¨¦ç¤«  « ¨®å «à¤ Windows XP.",
  NULL
}
};

SCREEN
DnsNtBootSect = { 3,5,
{ "¤ å¤  ¬¤«ã  ¡«á©«© «¦¬ §¨¦¨á££«¦ª ¡¡å¤©ª «à¤ Windows XP.",
  DntEmptyString,
  " àå« æ«   £¦¤á ©ª C: å¤   £¦¨­à£â¤ ¡  ¤ å¤  ¡«©«¨££â¤.",
 NULL
}
};

SCREEN
DnsOpenReadScript = { 3,5,
{ "¤ ã«¤ ¬¤«ã  §¨æ©© ©«¦ ¨®å¦ â©£ª ¤¨ é¤ §¦¬ ¡¦¨å©«¡ £ «¦",
  " ¡æ§« /u ©« ¨££ã ¤«¦¢é¤.",
  DntEmptyString,
  " ¢ «¦¬¨å ®à¨åª §¨¡¦¢¦ç© ¤ å¤  ¬¤«æ ¤ ©¬¤® ©«å.",
  NULL
}
};

SCREEN
DnsParseScriptFile = { 3,5,
{ "¦ ¨®å¦ â©£ª ¤¨ é¤ §¦¬ ¡¦¨å©«¡ £ «¦  ¡æ§« /u ©« ¨££ã",
  DntEmptyString,
  "%s",
  DntEmptyString,
  "¤«¦¢é¤, §¨ â®  â¤ ©­á¢£ ©ç¤«¥ª ©« © ¨á %u.",
  DntEmptyString,
  NULL
}
};

SCREEN
DnsBootMsgsTooLarge = { 3,5,
{ "¨¦¬© á©«¡ â¤ ©à«¨ ¡æ ©­á¢£ «ª ¡«á©«©ª.",
  DntEmptyString,
  "  £«­¨á© ª «à¤ £¤¬£á«à¤ ¡¡å¤©ª å¤  §¦¢ç £á¢ª.",
  NULL
}
};

SCREEN
DnsNoSwapDrive = { 3,5,
{ "¨¦¬© á©«¡ â¤ ©à«¨ ¡æ ©­á¢£ «ª ¡«á©«©ª.",
  DntEmptyString,
  "¤ ã«¤ ¬¤«ã  ç¨© â©ª   «¦ ¨®å¦ ¤«¢¢ãª.",
  NULL
}
};

SCREEN
DnsNoSmartdrv = { 3,5,
{ "¦ §¨æ¨££ ¡«á©«©ª ¤ ¤«æ§ © SmartDrive ©«¦¤ ¬§¦¢¦ ©«ã ©ª. ¦",
  "SmartDrive  ¢« é©  ©£¤« ¡á « ª § æ© ª ¬«ãª «ª ­á©ª ¡«á©«©ª",
  "«à¤ Windows.",
  DntEmptyString,
  "§ «å«  á£© â¥¦¦ª, ¡¡å¤© SmartDrive ¡  §¤¡¡å¤© «ª",
  "¡«á©«©ª. ¤«¨â¥« ©«¤ «¡£¨åà© DOS   §¨ ©©æ«¨ª §¢¨¦­¦¨åª.",
  DntEmptyString,
    "   â©« F3   â¥¦¦ §æ «¦ §¨æ¨££ ¡«á©«©ª.",
    "   â©« ENTER   ¤ ©¬¤®å©« ®à¨åª «¦ SmartDrive.",
  NULL
}
};

//
// Boot messages. These go in the fat and fat32 boot sectors.
//
CHAR BootMsgNtldrIsMissing[] = "NTLDR is missing";
CHAR BootMsgDiskError[] = "Disk error";
CHAR BootMsgPressKey[] = "Press any key to restart";
