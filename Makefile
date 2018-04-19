# generation de programme PVM
#  TP algorithmique parallele
#  maitrise
#  LL
#  07/10/97
# 
BDIR = $(PWD)
CC = gcc
CFLAGS=-g -Wall -DEPATH=\"$(BDIR)\"
LDFLAGS=-lpvm3

# executable directory
#BDIR  =  $(HOME)/bin/$(PVM_ARCH)


# Programme sequentiel
PROG = upper slave

#PVM_ROOT = /usr/share/pvm3
#PVM_ARCH = LINUXI386

#PVM_ROOT = /home/commun_depinfo/enseignants/lemarchand/TPPVMM1/PvmLinux/pvm3
#PVM_ARCH = LINUX64


#### do not edit #####

EXEC = $(PROG:%=$(BDIR)/%)

#### generation rules #####

all: $(EXEC)
	
$(BDIR)/%: point.o pb.o upper.o slave.o
	$(CC) -o $@ upper.o point.o pb.o $(LDFLAGS)

.c.o: point.h pb.h slave.h
	$(CC) $(CFLAGS) -c $< -o $@

slave: point.o pb.o upper.c
	$(CC) $(CFLAGS) point.o pb.o slave.c -o slave $(LDFLAGS)

clean:
	rm -f *.o $(EXEC) points upper_hull.pdf slave
