CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image -lGL -lGLU -lvorbisfile
SOURCES=main2.cpp GetawayView.cpp GetawayDoc.cpp BSimulation.cpp OpenGLHelpers.cpp BCar.cpp BGround.cpp BCamera.cpp BObject.cpp BaseClasses.cpp NewDialogs/DlgMainMenu.cpp NewDialogs/DlgVisuals.cpp NewDialogs/DlgSounds.cpp NewDialogs/DlgControls.cpp NewDialogs/DlgDropDownList.cpp NewDialogs/Dialog.cpp NewDialogs/Item.cpp NewDialogs/Button.cpp NewDialogs/DropDownList.cpp NewDialogs/Scrollbar.cpp NewDialogs/Listbox.cpp NewDialogs/Frame.cpp NewDialogs/ButtonGroup.cpp SoundModule.cpp ControllerModule.cpp Settings.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Pakoon

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm `find . -name "*.o"` $(EXECUTABLE)
