TARGET1 = spll
TARGET2 = dhash

COMP = gcc
CFLAGS = -pedantic -Wall -Wextra -Wfloat-equal -ansi -O2
CLIBS = -lm

$(TARGET1) : $(TARGET1).c $(TARGET2).c $(TARGET2).h

				$(COMP) $(TARGET1).c $(TARGET2).c -o $(TARGET1) $(CFLAGS) $(CLIBS)

clean :
				rm -f $(TARGET1)

run :
				./$(TARGET1) $(DICTIONARY) $(WORDS_TO_CHECK)
