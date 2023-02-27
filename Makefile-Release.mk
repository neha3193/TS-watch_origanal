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
CCC=c++
CXX=c++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/88919362/auto_subfile.o \
	${OBJECTDIR}/_ext/88919362/cmnd_line.o \
	${OBJECTDIR}/_ext/88919362/mygetip.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=-lpthread

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/daemon

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/daemon: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/daemon ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/88919362/auto_subfile.o: /home/feanisd/ESSEL/WorkSpace/OTHER/Daemon/auto_subfile.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/88919362
	${RM} $@.d
	$(COMPILE.c) -O2 -lpthread -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/88919362/auto_subfile.o /home/feanisd/ESSEL/WorkSpace/OTHER/Daemon/auto_subfile.c

${OBJECTDIR}/_ext/88919362/cmnd_line.o: /home/feanisd/ESSEL/WorkSpace/OTHER/Daemon/cmnd_line.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/88919362
	${RM} $@.d
	$(COMPILE.c) -O2 -lpthread -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/88919362/cmnd_line.o /home/feanisd/ESSEL/WorkSpace/OTHER/Daemon/cmnd_line.c

${OBJECTDIR}/_ext/88919362/mygetip.o: /home/feanisd/ESSEL/WorkSpace/OTHER/Daemon/mygetip.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/88919362
	${RM} $@.d
	$(COMPILE.c) -O2 -lpthread -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/88919362/mygetip.o /home/feanisd/ESSEL/WorkSpace/OTHER/Daemon/mygetip.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -lpthread -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/daemon

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
