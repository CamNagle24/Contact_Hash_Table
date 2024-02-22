CFLAGS = -Wall -Werror -g
CC = gcc $(CFLAGS)
SHELL = /bin/bash
CWD = $(shell pwd | sed 's/.*\///g')
AN = proj1

.PHONY: all test-setup test clean clean-tests zip

all: contacts_main

contacts.o: contacts.c contacts.h
	$(CC) -c $<

contacts_main.o: contacts_main.c contacts.h
	$(CC) -c $<

contacts_main: contacts_main.o contacts.o
	$(CC) -o $@ $^

test-setup:
	@chmod u+x testius
	@chmod u-w University.txt Directory.bin
	@rm -rf Work.txt School.bin Island.txt Fellowship.bin \
		University.bin Directory.txt

ifdef testnum
test: contacts_main test-setup
	./testius test_cases/tests.json -v -n "$(testnum)"
else
test: contacts_main test-setup
	./testius test_cases/tests.json
endif

clean:
	rm -f *.o contacts_main

clean-tests:
	rm -rf test_results Work.txt School.bin Island.txt Fellowship.bin \
		University.bin Directory.txt

# TODO May also need to exclude provided data files from zip
zip: clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)" -x "$(CWD)/test_cases/*"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi

help:
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make clean-tests              # remove temporary testing files'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make test                     # run all tests'
	@echo '  > make test testnum=n           # run only test number n'
