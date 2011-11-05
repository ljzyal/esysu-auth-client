#  EsysuAuthClient is Copyright (c) 2010-2011 Xu Guanglin mzguanglin@gmail.com
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; version 2 dated June, 1991, or
#  (at your option) version 3 dated 29 June, 2007.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#    
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#gcc -o EsysuAuthClient debug.c EsysuAuthClient.c initDeamon.c interfaceInfoProvider.c packetFactory.c -lpcap

OBJS = EsysuAuthClient.o debug.o initDeamon.o interfaceInfoProvider.o packetFactory.o

all : $(OBJS)
	$(CC) -o EsysuAuthClient $(OBJS) -lpcap $(LDFLAGS)

$(OBJS) :
	$(CC) -c $(CFLAGS) debug.c EsysuAuthClient.c initDeamon.c interfaceInfoProvider.c packetFactory.c

clean :
	rm -f *.o EsysuAuthClient
	
