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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/enc28j60-dspic33.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/enc28j60-dspic33.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1278887395/enc28j60.o ${OBJECTDIR}/_ext/1278887395/23lc1024.o ${OBJECTDIR}/_ext/1278887395/enc624j600.o ${OBJECTDIR}/_ext/1639450193/spi.o ${OBJECTDIR}/_ext/1639450193/uart.o ${OBJECTDIR}/_ext/1639450193/spiAsm.o ${OBJECTDIR}/_ext/1733914311/arp.o ${OBJECTDIR}/_ext/1733914311/icmp.o ${OBJECTDIR}/_ext/1733914311/ipv4.o ${OBJECTDIR}/_ext/1733914311/ntp.o ${OBJECTDIR}/_ext/1733914311/udp.o ${OBJECTDIR}/_ext/1733914311/tcp.o ${OBJECTDIR}/_ext/1519963337/insight.o ${OBJECTDIR}/_ext/1519963337/circularBuffer.o ${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1278887395/enc28j60.o.d ${OBJECTDIR}/_ext/1278887395/23lc1024.o.d ${OBJECTDIR}/_ext/1278887395/enc624j600.o.d ${OBJECTDIR}/_ext/1639450193/spi.o.d ${OBJECTDIR}/_ext/1639450193/uart.o.d ${OBJECTDIR}/_ext/1639450193/spiAsm.o.d ${OBJECTDIR}/_ext/1733914311/arp.o.d ${OBJECTDIR}/_ext/1733914311/icmp.o.d ${OBJECTDIR}/_ext/1733914311/ipv4.o.d ${OBJECTDIR}/_ext/1733914311/ntp.o.d ${OBJECTDIR}/_ext/1733914311/udp.o.d ${OBJECTDIR}/_ext/1733914311/tcp.o.d ${OBJECTDIR}/_ext/1519963337/insight.o.d ${OBJECTDIR}/_ext/1519963337/circularBuffer.o.d ${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1278887395/enc28j60.o ${OBJECTDIR}/_ext/1278887395/23lc1024.o ${OBJECTDIR}/_ext/1278887395/enc624j600.o ${OBJECTDIR}/_ext/1639450193/spi.o ${OBJECTDIR}/_ext/1639450193/uart.o ${OBJECTDIR}/_ext/1639450193/spiAsm.o ${OBJECTDIR}/_ext/1733914311/arp.o ${OBJECTDIR}/_ext/1733914311/icmp.o ${OBJECTDIR}/_ext/1733914311/ipv4.o ${OBJECTDIR}/_ext/1733914311/ntp.o ${OBJECTDIR}/_ext/1733914311/udp.o ${OBJECTDIR}/_ext/1733914311/tcp.o ${OBJECTDIR}/_ext/1519963337/insight.o ${OBJECTDIR}/_ext/1519963337/circularBuffer.o ${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/enc28j60-dspic33.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ128GP804
MP_LINKER_FILE_OPTION=,--script=p33FJ128GP804.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1278887395/enc28j60.o: ../src/devices/enc28j60.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1278887395 
	@${RM} ${OBJECTDIR}/_ext/1278887395/enc28j60.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/devices/enc28j60.c  -o ${OBJECTDIR}/_ext/1278887395/enc28j60.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1278887395/enc28j60.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=0
	@${FIXDEPS} "${OBJECTDIR}/_ext/1278887395/enc28j60.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1278887395/23lc1024.o: ../src/devices/23lc1024.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1278887395 
	@${RM} ${OBJECTDIR}/_ext/1278887395/23lc1024.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/devices/23lc1024.c  -o ${OBJECTDIR}/_ext/1278887395/23lc1024.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1278887395/23lc1024.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=1
	@${FIXDEPS} "${OBJECTDIR}/_ext/1278887395/23lc1024.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1278887395/enc624j600.o: ../src/devices/enc624j600.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1278887395 
	@${RM} ${OBJECTDIR}/_ext/1278887395/enc624j600.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/devices/enc624j600.c  -o ${OBJECTDIR}/_ext/1278887395/enc624j600.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1278887395/enc624j600.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=2
	@${FIXDEPS} "${OBJECTDIR}/_ext/1278887395/enc624j600.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/spi.o: ../src/drivers/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1639450193 
	@${RM} ${OBJECTDIR}/_ext/1639450193/spi.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/spi.c  -o ${OBJECTDIR}/_ext/1639450193/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/spi.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=3
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/uart.o: ../src/drivers/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1639450193 
	@${RM} ${OBJECTDIR}/_ext/1639450193/uart.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/uart.c  -o ${OBJECTDIR}/_ext/1639450193/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/uart.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=4
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/arp.o: ../src/ipstack/arp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/arp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/arp.c  -o ${OBJECTDIR}/_ext/1733914311/arp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/arp.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=5
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/arp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/icmp.o: ../src/ipstack/icmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/icmp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/icmp.c  -o ${OBJECTDIR}/_ext/1733914311/icmp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/icmp.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=6
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/icmp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/ipv4.o: ../src/ipstack/ipv4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/ipv4.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/ipv4.c  -o ${OBJECTDIR}/_ext/1733914311/ipv4.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/ipv4.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=7
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/ipv4.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/ntp.o: ../src/ipstack/ntp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/ntp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/ntp.c  -o ${OBJECTDIR}/_ext/1733914311/ntp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/ntp.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=8
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/ntp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/udp.o: ../src/ipstack/udp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/udp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/udp.c  -o ${OBJECTDIR}/_ext/1733914311/udp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/udp.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=9
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/udp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/tcp.o: ../src/ipstack/tcp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/tcp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/tcp.c  -o ${OBJECTDIR}/_ext/1733914311/tcp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/tcp.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=10
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/tcp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1519963337/insight.o: ../src/utils/insight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1519963337 
	@${RM} ${OBJECTDIR}/_ext/1519963337/insight.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/utils/insight.c  -o ${OBJECTDIR}/_ext/1519963337/insight.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1519963337/insight.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=11
	@${FIXDEPS} "${OBJECTDIR}/_ext/1519963337/insight.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1519963337/circularBuffer.o: ../src/utils/circularBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1519963337 
	@${RM} ${OBJECTDIR}/_ext/1519963337/circularBuffer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/utils/circularBuffer.c  -o ${OBJECTDIR}/_ext/1519963337/circularBuffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1519963337/circularBuffer.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=12
	@${FIXDEPS} "${OBJECTDIR}/_ext/1519963337/circularBuffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=13
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1278887395/enc28j60.o: ../src/devices/enc28j60.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1278887395 
	@${RM} ${OBJECTDIR}/_ext/1278887395/enc28j60.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/devices/enc28j60.c  -o ${OBJECTDIR}/_ext/1278887395/enc28j60.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1278887395/enc28j60.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=0
	@${FIXDEPS} "${OBJECTDIR}/_ext/1278887395/enc28j60.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1278887395/23lc1024.o: ../src/devices/23lc1024.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1278887395 
	@${RM} ${OBJECTDIR}/_ext/1278887395/23lc1024.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/devices/23lc1024.c  -o ${OBJECTDIR}/_ext/1278887395/23lc1024.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1278887395/23lc1024.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=1
	@${FIXDEPS} "${OBJECTDIR}/_ext/1278887395/23lc1024.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1278887395/enc624j600.o: ../src/devices/enc624j600.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1278887395 
	@${RM} ${OBJECTDIR}/_ext/1278887395/enc624j600.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/devices/enc624j600.c  -o ${OBJECTDIR}/_ext/1278887395/enc624j600.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1278887395/enc624j600.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=2
	@${FIXDEPS} "${OBJECTDIR}/_ext/1278887395/enc624j600.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/spi.o: ../src/drivers/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1639450193 
	@${RM} ${OBJECTDIR}/_ext/1639450193/spi.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/spi.c  -o ${OBJECTDIR}/_ext/1639450193/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/spi.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=3
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1639450193/uart.o: ../src/drivers/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1639450193 
	@${RM} ${OBJECTDIR}/_ext/1639450193/uart.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/drivers/uart.c  -o ${OBJECTDIR}/_ext/1639450193/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1639450193/uart.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=4
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/arp.o: ../src/ipstack/arp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/arp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/arp.c  -o ${OBJECTDIR}/_ext/1733914311/arp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/arp.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=5
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/arp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/icmp.o: ../src/ipstack/icmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/icmp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/icmp.c  -o ${OBJECTDIR}/_ext/1733914311/icmp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/icmp.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=6
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/icmp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/ipv4.o: ../src/ipstack/ipv4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/ipv4.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/ipv4.c  -o ${OBJECTDIR}/_ext/1733914311/ipv4.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/ipv4.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=7
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/ipv4.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/ntp.o: ../src/ipstack/ntp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/ntp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/ntp.c  -o ${OBJECTDIR}/_ext/1733914311/ntp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/ntp.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=8
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/ntp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/udp.o: ../src/ipstack/udp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/udp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/udp.c  -o ${OBJECTDIR}/_ext/1733914311/udp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/udp.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=9
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/udp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1733914311/tcp.o: ../src/ipstack/tcp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1733914311 
	@${RM} ${OBJECTDIR}/_ext/1733914311/tcp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ipstack/tcp.c  -o ${OBJECTDIR}/_ext/1733914311/tcp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1733914311/tcp.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=10
	@${FIXDEPS} "${OBJECTDIR}/_ext/1733914311/tcp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1519963337/insight.o: ../src/utils/insight.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1519963337 
	@${RM} ${OBJECTDIR}/_ext/1519963337/insight.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/utils/insight.c  -o ${OBJECTDIR}/_ext/1519963337/insight.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1519963337/insight.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=11
	@${FIXDEPS} "${OBJECTDIR}/_ext/1519963337/insight.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1519963337/circularBuffer.o: ../src/utils/circularBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1519963337 
	@${RM} ${OBJECTDIR}/_ext/1519963337/circularBuffer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/utils/circularBuffer.c  -o ${OBJECTDIR}/_ext/1519963337/circularBuffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1519963337/circularBuffer.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=12
	@${FIXDEPS} "${OBJECTDIR}/_ext/1519963337/circularBuffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"        -g -omf=elf -mlarge-data -O1 -I"../src" -I"../src/devices" -I"../src/drivers" -I"../src/ipstack" -I"C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/src/utils" -msmart-io=1 -Wall -msfr-warn=off -DF_NUM=13
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1639450193/spiAsm.o: ../src/drivers/spiAsm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1639450193 
	@${RM} ${OBJECTDIR}/_ext/1639450193/spiAsm.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../src/drivers/spiAsm.s  -o ${OBJECTDIR}/_ext/1639450193/spiAsm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wa,-MD,"${OBJECTDIR}/_ext/1639450193/spiAsm.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/spiAsm.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o: ../src/addrErrLoc.s.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../src/addrErrLoc.s.s  -o ${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wa,-MD,"${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o: ../src/drivers/spiAsmENC624.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1639450193 
	@${RM} ${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../src/drivers/spiAsmENC624.s  -o ${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wa,-MD,"${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1639450193/spiAsm.o: ../src/drivers/spiAsm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1639450193 
	@${RM} ${OBJECTDIR}/_ext/1639450193/spiAsm.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../src/drivers/spiAsm.s  -o ${OBJECTDIR}/_ext/1639450193/spiAsm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -Wa,-MD,"${OBJECTDIR}/_ext/1639450193/spiAsm.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/spiAsm.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o: ../src/addrErrLoc.s.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../src/addrErrLoc.s.s  -o ${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -Wa,-MD,"${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/addrErrLoc.s.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o: ../src/drivers/spiAsmENC624.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1639450193 
	@${RM} ${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../src/drivers/spiAsmENC624.s  -o ${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -Wa,-MD,"${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1639450193/spiAsmENC624.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/enc28j60-dspic33.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/enc28j60-dspic33.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/enc28j60-dspic33.X/enc28j60.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/enc28j60-dspic33.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/enc28j60-dspic33.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="C:/Users/Hans/Documents/GitHub/enc28j60-dspic33/enc28j60-dspic33.X/enc28j60.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/enc28j60-dspic33.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
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
