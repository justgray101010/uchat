SQLITE_OBJ = sqlite3.o
SQLITE_LIBNAME = sqlite3_lib

CC = gcc -std=c89

#library file extensions
STATIC = a

SQLITE_STATIC = ../$(SQLITE_LIBNAME).$(STATIC)

.PHONY: all static clean uninstall reinstall

all: static

static: $(SQLITE_STATIC)

$(SQLITE_STATIC): $(SQLITE_OBJ)
	@$(AR) rcs $@ $<

$(SQLITE_OBJ): sqlite3.c sqlite3.h
	@$(CC) -DSQLITE_ENABLE_UPDATE_DELETE_LIMIT=1 -c $<

uninstall: clean
	@rm -rf $(SQLITE_STATIC)

clean:
	@rm -rf $(SQLITE_OBJ)

reinstall: uninstall all
