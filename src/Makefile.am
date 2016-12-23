AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = lszip
lszip_SOURCES = \
	lszip.c \
	LinkedList/LinkedList.c \
	MemoryAllocator/MemoryAllocator.c \
	minizip/ioapi.c \
	minizip/unzip.c
lszip_LDFLAGS = -lz
lszip_CFLAGS = -std=c99 -ILinkedList -IMemoryAllocator -Iminizip
