#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SDCard_Writing_New.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SDCard_Writing_New.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../../../../../../../../microchip/mla/v2018_11_26/apps/fileio/sd_card_demo/firmware/src/main.c ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/src/fileio.c ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/drivers/sd_spi/sd_spi.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/148754417/main.o ${OBJECTDIR}/_ext/485325724/fileio.o ${OBJECTDIR}/_ext/1648915070/sd_spi.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/148754417/main.o.d ${OBJECTDIR}/_ext/485325724/fileio.o.d ${OBJECTDIR}/_ext/1648915070/sd_spi.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/148754417/main.o ${OBJECTDIR}/_ext/485325724/fileio.o ${OBJECTDIR}/_ext/1648915070/sd_spi.o

# Source Files
SOURCEFILES=../../../../../../../../../../microchip/mla/v2018_11_26/apps/fileio/sd_card_demo/firmware/src/main.c ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/src/fileio.c ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/drivers/sd_spi/sd_spi.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SDCard_Writing_New.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MM0064GPL036
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/148754417/main.o: ../../../../../../../../../../microchip/mla/v2018_11_26/apps/fileio/sd_card_demo/firmware/src/main.c  .generated_files/46aed96e86acbf36637f328cc71fee760176dcec.flag .generated_files/5476a8ba234974ffa0ea0daeb639b475345adc30.flag
	@${MKDIR} "${OBJECTDIR}/_ext/148754417" 
	@${RM} ${OBJECTDIR}/_ext/148754417/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/148754417/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/148754417/main.o.d" -o ${OBJECTDIR}/_ext/148754417/main.o ../../../../../../../../../../microchip/mla/v2018_11_26/apps/fileio/sd_card_demo/firmware/src/main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/485325724/fileio.o: ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/src/fileio.c  .generated_files/68789e2efb83b5a1e5be4d7755a01ee3eea0ac3c.flag .generated_files/5476a8ba234974ffa0ea0daeb639b475345adc30.flag
	@${MKDIR} "${OBJECTDIR}/_ext/485325724" 
	@${RM} ${OBJECTDIR}/_ext/485325724/fileio.o.d 
	@${RM} ${OBJECTDIR}/_ext/485325724/fileio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/485325724/fileio.o.d" -o ${OBJECTDIR}/_ext/485325724/fileio.o ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/src/fileio.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1648915070/sd_spi.o: ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/drivers/sd_spi/sd_spi.c  .generated_files/be16636e9092e8219e6dca86c849029a82eda9a0.flag .generated_files/5476a8ba234974ffa0ea0daeb639b475345adc30.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1648915070" 
	@${RM} ${OBJECTDIR}/_ext/1648915070/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1648915070/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1648915070/sd_spi.o.d" -o ${OBJECTDIR}/_ext/1648915070/sd_spi.o ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/drivers/sd_spi/sd_spi.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/148754417/main.o: ../../../../../../../../../../microchip/mla/v2018_11_26/apps/fileio/sd_card_demo/firmware/src/main.c  .generated_files/98d42755e2e564902696c792e0e128399873b666.flag .generated_files/5476a8ba234974ffa0ea0daeb639b475345adc30.flag
	@${MKDIR} "${OBJECTDIR}/_ext/148754417" 
	@${RM} ${OBJECTDIR}/_ext/148754417/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/148754417/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/148754417/main.o.d" -o ${OBJECTDIR}/_ext/148754417/main.o ../../../../../../../../../../microchip/mla/v2018_11_26/apps/fileio/sd_card_demo/firmware/src/main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/485325724/fileio.o: ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/src/fileio.c  .generated_files/eca5f7fc64a59ef1c78c9fc02d2a8b551a96a608.flag .generated_files/5476a8ba234974ffa0ea0daeb639b475345adc30.flag
	@${MKDIR} "${OBJECTDIR}/_ext/485325724" 
	@${RM} ${OBJECTDIR}/_ext/485325724/fileio.o.d 
	@${RM} ${OBJECTDIR}/_ext/485325724/fileio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/485325724/fileio.o.d" -o ${OBJECTDIR}/_ext/485325724/fileio.o ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/src/fileio.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1648915070/sd_spi.o: ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/drivers/sd_spi/sd_spi.c  .generated_files/746548fe3ef0eaf604b0b2ca76bd5ee124fecbb3.flag .generated_files/5476a8ba234974ffa0ea0daeb639b475345adc30.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1648915070" 
	@${RM} ${OBJECTDIR}/_ext/1648915070/sd_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1648915070/sd_spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1648915070/sd_spi.o.d" -o ${OBJECTDIR}/_ext/1648915070/sd_spi.o ../../../../../../../../../../microchip/mla/v2018_11_26/framework/fileio/drivers/sd_spi/sd_spi.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SDCard_Writing_New.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/SDCard_Writing_New.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/SDCard_Writing_New.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/SDCard_Writing_New.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/SDCard_Writing_New.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
