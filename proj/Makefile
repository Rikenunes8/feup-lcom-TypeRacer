# name of the program (Minix service)
PROG=proj

# folders
.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/headers
.PATH: ${.CURDIR}/xpm

# source code files to be compiled
SRCS = proj.c utils.c timer.c keyboard.c mouse.c graphic.c Chars.c race.c menus.c sprite.c rtc.c best_results.c
IPATHS = -I./src -I./headers

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
# CPPFLAGS += -pedantic ${IPATHS}
CPPFLAGS += -pedantic ${IPATHS} #-D __LCOM_OPTIMIZED__

# list of library dependencies (LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
