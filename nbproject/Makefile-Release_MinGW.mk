#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW_Latest-Windows
CND_DLIB_EXT=dll
CND_CONF=Release_MinGW
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AsyncConnectionlessReceiver.o \
	${OBJECTDIR}/AsyncConnectionlessSender.o \
	${OBJECTDIR}/AsyncMulticastComms.o \
	${OBJECTDIR}/AsyncMulticastCommsImpl.o \
	${OBJECTDIR}/AsyncMulticastReceiver.o \
	${OBJECTDIR}/AsyncMulticastSender.o \
	${OBJECTDIR}/AsyncTCPComms.o \
	${OBJECTDIR}/AsyncTCPCommsSendRecv.o \
	${OBJECTDIR}/AsyncUDPComms.o \
	${OBJECTDIR}/AsyncUDPCommsImpl.o \
	${OBJECTDIR}/AsyncUDPReceiver.o \
	${OBJECTDIR}/AsyncUDPSender.o \
	${OBJECTDIR}/CommsUtilities.o \
	${OBJECTDIR}/EthernetCommsModule.o \
	${OBJECTDIR}/NetworkUtilities.o \
	${OBJECTDIR}/SyncConnectionlessReceiver.o \
	${OBJECTDIR}/SyncConnectionlessSender.o \
	${OBJECTDIR}/SyncMulticastComms.o \
	${OBJECTDIR}/SyncMulticastCommsImpl.o \
	${OBJECTDIR}/SyncMulticastReceiver.o \
	${OBJECTDIR}/SyncMulticastSender.o \
	${OBJECTDIR}/SyncTCPComms.o \
	${OBJECTDIR}/SyncTCPCommsSendRecv.o \
	${OBJECTDIR}/SyncUDPComms.o \
	${OBJECTDIR}/SyncUDPCommsImpl.o \
	${OBJECTDIR}/SyncUDPReceiver.o \
	${OBJECTDIR}/SyncUDPSender.o \
	${OBJECTDIR}/TCPComms.o \
	${OBJECTDIR}/TCPCommsImpl.o \
	${OBJECTDIR}/stdafx.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSENTEL_EthernetCommsLibrary.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSENTEL_EthernetCommsLibrary.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSENTEL_EthernetCommsLibrary.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared

${OBJECTDIR}/AsyncConnectionlessReceiver.o: AsyncConnectionlessReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncConnectionlessReceiver.o AsyncConnectionlessReceiver.cpp

${OBJECTDIR}/AsyncConnectionlessSender.o: AsyncConnectionlessSender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncConnectionlessSender.o AsyncConnectionlessSender.cpp

${OBJECTDIR}/AsyncMulticastComms.o: AsyncMulticastComms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncMulticastComms.o AsyncMulticastComms.cpp

${OBJECTDIR}/AsyncMulticastCommsImpl.o: AsyncMulticastCommsImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncMulticastCommsImpl.o AsyncMulticastCommsImpl.cpp

${OBJECTDIR}/AsyncMulticastReceiver.o: AsyncMulticastReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncMulticastReceiver.o AsyncMulticastReceiver.cpp

${OBJECTDIR}/AsyncMulticastSender.o: AsyncMulticastSender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncMulticastSender.o AsyncMulticastSender.cpp

${OBJECTDIR}/AsyncTCPComms.o: AsyncTCPComms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncTCPComms.o AsyncTCPComms.cpp

${OBJECTDIR}/AsyncTCPCommsSendRecv.o: AsyncTCPCommsSendRecv.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncTCPCommsSendRecv.o AsyncTCPCommsSendRecv.cpp

${OBJECTDIR}/AsyncUDPComms.o: AsyncUDPComms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncUDPComms.o AsyncUDPComms.cpp

${OBJECTDIR}/AsyncUDPCommsImpl.o: AsyncUDPCommsImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncUDPCommsImpl.o AsyncUDPCommsImpl.cpp

${OBJECTDIR}/AsyncUDPReceiver.o: AsyncUDPReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncUDPReceiver.o AsyncUDPReceiver.cpp

${OBJECTDIR}/AsyncUDPSender.o: AsyncUDPSender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AsyncUDPSender.o AsyncUDPSender.cpp

${OBJECTDIR}/CommsUtilities.o: CommsUtilities.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/CommsUtilities.o CommsUtilities.cpp

${OBJECTDIR}/EthernetCommsModule.o: EthernetCommsModule.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EthernetCommsModule.o EthernetCommsModule.cpp

${OBJECTDIR}/NetworkUtilities.o: NetworkUtilities.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/NetworkUtilities.o NetworkUtilities.cpp

${OBJECTDIR}/SyncConnectionlessReceiver.o: SyncConnectionlessReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncConnectionlessReceiver.o SyncConnectionlessReceiver.cpp

${OBJECTDIR}/SyncConnectionlessSender.o: SyncConnectionlessSender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncConnectionlessSender.o SyncConnectionlessSender.cpp

${OBJECTDIR}/SyncMulticastComms.o: SyncMulticastComms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncMulticastComms.o SyncMulticastComms.cpp

${OBJECTDIR}/SyncMulticastCommsImpl.o: SyncMulticastCommsImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncMulticastCommsImpl.o SyncMulticastCommsImpl.cpp

${OBJECTDIR}/SyncMulticastReceiver.o: SyncMulticastReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncMulticastReceiver.o SyncMulticastReceiver.cpp

${OBJECTDIR}/SyncMulticastSender.o: SyncMulticastSender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncMulticastSender.o SyncMulticastSender.cpp

${OBJECTDIR}/SyncTCPComms.o: SyncTCPComms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncTCPComms.o SyncTCPComms.cpp

${OBJECTDIR}/SyncTCPCommsSendRecv.o: SyncTCPCommsSendRecv.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncTCPCommsSendRecv.o SyncTCPCommsSendRecv.cpp

${OBJECTDIR}/SyncUDPComms.o: SyncUDPComms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncUDPComms.o SyncUDPComms.cpp

${OBJECTDIR}/SyncUDPCommsImpl.o: SyncUDPCommsImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncUDPCommsImpl.o SyncUDPCommsImpl.cpp

${OBJECTDIR}/SyncUDPReceiver.o: SyncUDPReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncUDPReceiver.o SyncUDPReceiver.cpp

${OBJECTDIR}/SyncUDPSender.o: SyncUDPSender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncUDPSender.o SyncUDPSender.cpp

${OBJECTDIR}/TCPComms.o: TCPComms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/TCPComms.o TCPComms.cpp

${OBJECTDIR}/TCPCommsImpl.o: TCPCommsImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/TCPCommsImpl.o TCPCommsImpl.cpp

${OBJECTDIR}/stdafx.o: stdafx.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2  -MMD -MP -MF $@.d -o ${OBJECTDIR}/stdafx.o stdafx.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSENTEL_EthernetCommsLibrary.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
