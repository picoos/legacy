#  Copyright (c) 2004-2005, Dennis Kuschel / Swen Moczarski
#  All rights reserved. 
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#   3. The name of the author may not be used to endorse or promote
#      products derived from this software without specific prior written
#      permission. 
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
#  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
#  INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
#  OF THE POSSIBILITY OF SUCH DAMAGE.


#  This file is originally from the pico]OS realtime operating system
#  (http://picoos.sourceforge.net).
#
#  $Id: port.mak,v 1.1 2005/01/10 22:39:26 dkuschel Exp $


# Set default compiler.
# Possible compilers are GCC (GNU C) and VC6 (MS Visual C++ V6.0).
#
#COMPILER = GCC
#COMPILER = VC6

# Try to determine the installed compiler. Seach for MS VC6.
ifeq '$(strip $(COMPILER))' ''
ifneq '$(findstring Microsoft,$(shell lib))' ''
COMPILER = VC6
$(warning COMPILER not specified, trying MS VC++ 6.0)
else
COMPILER = GCC
$(warning COMPILER not specified, trying GCC (MinGW))
endif
endif
export COMPILER


# Set to 1 to include generic pico]OS "findbit" function
GENERIC_FINDBIT = 1

# Define extensions
EXT_C   = .c
EXT_ASM = .asm
EXT_OBJ = .obj
EXT_LIB = .lib
EXT_OUT = .exe


#----------------------------------------------------------------------------
#  MS Visual C++ Compiler
#
ifeq '$(COMPILER)' 'VC6'

# Define tools: compiler, assembler, archiver, linker
CC = cl
AS = masm
AR = lib
LD = link

# Define to 1 if CC outputs an assembly file
CC2ASM = 0

# Define to 1 if assembler code must be preprocessed by the compiler
A2C2A  = 0

# Define general options
OPT_CC_INC   = /I
OPT_CC_DEF   = /D
OPT_AS_INC   = /I
OPT_AS_DEF   = /D
OPT_AR_ADD   =
OPT_LD_SEP   = 
OPT_LD_PFOBJ = 
OPT_LD_PFLIB = 
OPT_LD_FIRST =
OPT_LD_LAST  = kernel32.lib user32.lib winmm.lib

# Set global defines for compiler / assembler
CDEFINES =
ADEFINES =

# Set global includes
#CINCLUDES =
#AINCLUDES =

# Distinguish between build modes
ifeq '$(BUILD)' 'DEBUG'
  CFLAGS   += /nologo /Od /W3 /ZI /c /MT
  CDEFINES += _DBG
  ADEFINES += _DBG
else
  CFLAGS   += /nologo /O2 /W3 /c /MT
  CDEFINES += _REL
  ADEFINES += _REL
endif

# Define Compiler Flags
CFLAGS += /Fo

# Define Assembler Flags
ASFLAGS +=

# Define Linker Flags
ifeq '$(BUILD)' 'DEBUG'
LDFLAGS = /debug
endif
LDFLAGS += /nologo /subsystem:console /incremental:yes /machine:I386 /out:

# Define archiver flags
ARFLAGS = /NOLOGO /OUT:



#----------------------------------------------------------------------------
#  GNU C  (MinGW for MS Windows)
#
else

# Set path to the compiler
#GCC_DIR = E:/Programme/mingw
GCC_DIR =

ifeq '$(strip $(GCC_DIR))' ''
ifeq '$(strip $(GCC_DIRWARNED))' ''
$(warning ******> GCC_DIR not set! Please set this variable to your\
 MinGW directory (see file x86w32/port.mak), otherwise you will not have\
 an accurate timer interrupt! <******)
export GCC_DIRWARNED=true
endif
GCC_BINDIR =
GCC_LIBDIR =
else
GCC_BINDIR = $(GCC_DIR)/bin/
GCC_LIBDIR = $(GCC_DIR)/lib/
endif

# Define tools: compiler, assembler, archiver, linker
CC = $(GCC_BINDIR)gcc
AS = $(GCC_BINDIR)gcc
AR = $(GCC_BINDIR)ar
LD = $(GCC_BINDIR)gcc

# Define to 1 if CC outputs an assembly file
CC2ASM = 0

# Define to 1 if assembler code must be preprocessed by the compiler
A2C2A  = 0

# Set global defines for compiler / assembler
CDEFINES = GCC
ADEFINES = GCC

# Setup windows mmsystem library
ifeq '$(strip $(GCC_LIBDIR))' ''
LINKLIBS =
else
LINKLIBS = $(GCC_LIBDIR)libwinmm.a
CDEFINES += HAVEMMLIB
endif

# Define general options
OPT_CC_INC   = -I
OPT_CC_DEF   = -D
OPT_AS_INC   = -I
OPT_AS_DEF   = -D
OPT_AR_ADD   =
OPT_LD_SEP   =
OPT_LD_PFOBJ =
OPT_LD_PFLIB =
OPT_LD_FIRST =
OPT_LD_LAST  = $(LINKLIBS)

# Set global includes
CINCLUDES = .
AINCLUDES = .

# Distinguish between build modes
ifeq '$(BUILD)' 'DEBUG'
  CFLAGS   += -O0 -g
  AFLAGS   += -g
  CDEFINES += _DBG
  ADEFINES += _DBG
else
  CFLAGS   += -O3
  CDEFINES += _REL
  ADEFINES += _REL
endif

# Define Compiler Flags
CFLAGS +=  -pedantic -Wall -c -o

# Define Assembler Flags
ASFLAGS += -c -Wa,-mregnames -x assembler-with-cpp -o

# Define Linker Flags
LDFLAGS = -mconsole -Wl,-Map,$(DIR_OUT)/$(TARGET).map \
          -Wl,--cref  -o

# Define archiver flags
ARFLAGS = r 

endif
