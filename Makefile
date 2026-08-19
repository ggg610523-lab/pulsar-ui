
CC = gcc

TARGET = liquid_glass

CFLAGS = -Wall -Wextra -O2 \
         $(shell sdl2-config --cflags)

LIBS = $(shell sdl2-config --libs) \
       -lSDL2_ttf \
       -lSDL2_image \
       -lm

SOURCES = \
    main.c \
    ui.c \
    pulsarUI.c \
    background.c \
    panel.c \
    button.c \
    primaryButton.c \
    slider.c \
    radio.c \
    checkbox.c \
    searchBar.c \
    hamburger.c \
    sidebar.c \
    dialogBox.c \
    dropdownmenu.c \
    progressBar.c \
    tabs.c \
    menuBar.c \
    tooltips.c \
    notifications.c \
    image.c \
    imageWidget.c

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all run clean
