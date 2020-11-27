# name of the program (Minix service)
PROG=proj

# folders
.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/headers

# source code files to be compiled
SRCS = timer.c utils.c
IPATHS = -I./src -I./headers

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic ${IPATHS}

# list of library dependencies (LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
