#  Copyright (c) 2004, Dennis Kuschel / Swen Moczarski
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
#  $Id: compile.mak,v 1.3 2004/06/05 11:42:25 dkuschel Exp $


# Compile files

ifeq '$(strip $(MAKE_CPL))' ''
($error common.mak not included)
endif

# Ensure the port variable is set.
ifneq '$(PORT)' ''

# Set alternative binary output directory
ifneq '$(strip $(DIR_OUTPUT))' ''
DIR_OUTB := $(DIR_OUTPUT)
DIR_OUTP := $(DIR_OUTB)/$(PORT)
DIR_OUT  := $(DIR_OUTP)/$(MODEEXT)
DIR_LIBB := $(DIR_OUTPUT)/$(PORT)
DIR_LIBP := $(DIR_LIBB)/temp
DIR_LIB  := $(DIR_LIBP)/$(MODEEXT)
DIR_OBJB := $(DIR_OUTPUT)/$(PORT)
DIR_OBJP := $(DIR_OBJB)/temp
DIR_OBJ  := $(DIR_OBJP)/$(MODEEXT)
endif

# Set default configuration directory
ifeq '$(strip $(DIR_CONFIG))' ''
DCFGDEF = $(DIR_PORT)/default
endif

# Define all object files.
OBJC = $(filter-out $(SRC),$(SRC:$(EXT_C)=$(EXT_OBJ)))
OBJA = $(filter-out $(SRC),$(SRC:$(EXT_ASM)=$(EXT_OBJ)))
OBJ += $(addprefix $(DIR_OBJ)/,$(notdir $(OBJC) $(OBJA)))

# Define source directories. Strip them from the SRC variable.
SRCDIRS  = $(sort $(dir $(SRC)))
DIR_SRC1 = $(strip $(word 1,$(SRCDIRS)))
DIR_SRC2 = $(strip $(word 2,$(SRCDIRS)))
DIR_SRC3 = $(strip $(word 3,$(SRCDIRS)))
DIR_SRC4 = $(strip $(word 4,$(SRCDIRS)))

# Place include/define options here
CINCLUDES += $(DIR_INC) $(DCFGDEF) $(DIR_CONFIG) $(DIR_PORT) $(DIR_USRINC)
AINCLUDES += $(DIR_INC) $(DCFGDEF) $(DIR_CONFIG) $(DIR_PORT) $(DIR_USRINC)
CINCS = $(addprefix $(OPT_CC_INC),$(call adjpath,$(CINCLUDES)))
CDEFS = $(addprefix $(OPT_CC_DEF),$(CDEFINES))
AINCS = $(addprefix $(OPT_AS_INC),$(call adjpath,$(AINCLUDES)))
ADEFS = $(addprefix $(OPT_AS_DEF),$(ADEFINES))

# ---------------------------------------------------------------------------

# Make subdirectories
ifneq '$(strip $(DIR_OUTPUT))' ''
$(DIR_OUTPUT):
	$(MKDIR) $@
endif
ifneq '$(DIR_OBJB)' '$(DIR_OUTPUT)'
$(DIR_OBJB): $(DIR_OUTPUT)
	$(MKDIR) $@
endif
$(DIR_OBJP): $(DIR_OBJB)
	$(MKDIR) $@
$(DIR_OBJ): $(DIR_OBJP)
	$(MKDIR) $@
ifneq '$(DIR_LIBB)' '$(DIR_OBJB)'
ifneq '$(DIR_LIBB)' '$(DIR_OUTPUT)'
$(DIR_LIBB): $(DIR_OUTPUT)
	$(MKDIR) $@
endif
endif
ifneq '$(DIR_LIBP)' '$(DIR_OBJP)'
$(DIR_LIBP): $(DIR_LIBB)
	$(MKDIR) $@
endif
ifneq '$(DIR_LIB)' '$(DIR_OBJ)'
$(DIR_LIB): $(DIR_LIBP)
	$(MKDIR) $@
endif
ifneq '$(DIR_OUTB)' '$(DIR_OBJB)'
ifneq '$(DIR_OUTB)' '$(DIR_OUTPUT)'
$(DIR_OUTB): $(DIR_OUTPUT)
	$(MKDIR) $@
endif
endif
ifneq '$(DIR_OUTP)' '$(DIR_OBJB)'
$(DIR_OUTP): $(DIR_OUTB)
	$(MKDIR) $@
endif
ifneq '$(DIR_OUT)' '$(DIR_OBJ)'
$(DIR_OUT): $(DIR_OUTP)
	$(MKDIR) $@
endif

# ---------------------------------------------------------------------------

ifeq '$(strip $(CC2ASM))' '1'

# Compile: create intermediate files from C source files.
$(DIR_OBJ)/%$(EXT_INT) : $(DIR_SRC1)%$(EXT_C) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CFLAGS)$(call adjpath,$@) $(CINCS) $(CDEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_INT) : $(DIR_SRC2)%$(EXT_C) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CFLAGS)$(call adjpath,$@) $(CINCS) $(CDEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_INT) : $(DIR_SRC3)%$(EXT_C) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CFLAGS)$(call adjpath,$@) $(CINCS) $(CDEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_INT) : $(DIR_SRC4)%$(EXT_C) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CFLAGS)$(call adjpath,$@) $(CINCS) $(CDEFS) $(call adjpath,$<)

# Assemble: create object files from intermediate files.
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_OBJ)/%$(EXT_INT) $(DIR_OBJ)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$<)
	-$(REMOVE) $<

else

# Compile: create object files from C source files
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC1)%$(EXT_C) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CFLAGS)$(call adjpath,$@) $(CINCS) $(CDEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC2)%$(EXT_C) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CFLAGS)$(call adjpath,$@) $(CINCS) $(CDEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC3)%$(EXT_C) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CFLAGS)$(call adjpath,$@) $(CINCS) $(CDEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC4)%$(EXT_C) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CFLAGS)$(call adjpath,$@) $(CINCS) $(CDEFS) $(call adjpath,$<)

endif

ifeq '$(strip $(A2C2A))' '1'

# Assemble: create object files from assembler source files,
# but preprocess the assembler files with the C-compiler preprocessor.
# (This code matches the MetaWare HighC/C++ compiler)
CAINCS = $(call adjpath,$(addprefix $(OPT_CC_INC),$(AINCLUDES)))
CADEFS = $(addprefix $(OPT_CC_DEF),$(ADEFINES))
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC1)%$(EXT_ASM) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CAFLAGS)$(call adjpath,$@) $(CAINCS) $(CADEFS) $(call adjpath,$<)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$*$(EXT_INT))
	-$(RM) $*$(EXT_INT)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC2)%$(EXT_ASM) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CAFLAGS)$(call adjpath,$@) $(CAINCS) $(CADEFS) $(call adjpath,$<)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$*$(EXT_INT))
	-$(RM) $*$(EXT_INT)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC3)%$(EXT_ASM) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CAFLAGS)$(call adjpath,$@) $(CAINCS) $(CADEFS) $(call adjpath,$<)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$*$(EXT_INT))
	-$(RM) $*$(EXT_INT)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC4)%$(EXT_ASM) $(DIR_OBJ) $(COMMONDEP)
	$(CC) $(CAFLAGS)$(call adjpath,$@) $(CAINCS) $(CADEFS) $(call adjpath,$<)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$*$(EXT_INT))
	-$(RM) $*$(EXT_INT)

else

# Assemble: create object files from assembler source files.
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC1)%$(EXT_ASM) $(DIR_OBJ) $(COMMONDEP)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC2)%$(EXT_ASM) $(DIR_OBJ) $(COMMONDEP)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC3)%$(EXT_ASM) $(DIR_OBJ) $(COMMONDEP)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$<)
$(DIR_OBJ)/%$(EXT_OBJ) : $(DIR_SRC4)%$(EXT_ASM) $(DIR_OBJ) $(COMMONDEP)
	$(AS) $(ASFLAGS)$(call adjpath,$@) $(AINCS) $(ADEFS) $(call adjpath,$<)

endif

endif
