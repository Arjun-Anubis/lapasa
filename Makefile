CC= gcc
FXTENSION=c
LIBS= 
HEADERDIR=header
CFLAGS=-Wall -g -I$(HEADERDIR)

SRCDIR=src
# SRCS:=main.c pasa.c log.c
SRCS:=$(addprefix $(SRCDIR)/,$(SRCS) )

BUILDDIR=build
OBJS:= log.o lapasa.o
OBJS:=$(addprefix $(BUILDDIR)/,$(OBJS) )

EXE=main
EXECSRC=main.c

all: $(EXE)


$(EXE): $(OBJS) 
	$(CC) $(CFLAGS) $(SRCDIR)/$(EXECSRC) -o $(EXE) $(LIBS) $(OBJS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(FXTENSION)
	$(CC) -c $(CFLAGS) $< -o $(BUILDDIR)/$(notdir $@) 

clean:
	$(RM) $(EXE) $(OBJS)
