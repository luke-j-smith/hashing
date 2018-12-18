TARGET1 = compare_main
TARGET2 = compare_dhash
TARGET3 = heat_map_display
TARGET4 = neillsdl2_ls

COMP = gcc
CFLAGS = `pkg-config sdl2 --cflags` -pedantic -Wall -Wextra -Wfloat-equal -ansi -O2
CLIBS = `pkg-config sdl2 --libs` -lm

$(TARGET1) : $(TARGET1).c $(TARGET2).c $(TARGET2).h $(TARGET3).c $(TARGET3).h $(TARGET4).c $(TARGET4).h

				$(COMP) $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c -o $(TARGET1) $(CFLAGS) $(CLIBS)

clean :
				rm -f $(TARGET1)

run :
				./$(TARGET1) $(DICTIONARY) $(WORDS_TO_CHECK)
