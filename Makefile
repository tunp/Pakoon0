CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image -lGL -lGLU -lvorbisfile

EMXX = em++
EMXXFLAGS = -Oz -c -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_VORBIS=1 -I/home/jettis/git/gl4es/include
EMXXLINK = -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_VORBIS=1 -s FULL_ES2=1 -L/home/jettis/git/gl4es/lib -lGL4es -L/home/jettis/git/GLU/.libs -lGLU4es -s ALLOW_MEMORY_GROWTH=1

NEW_DIALOG_OBJECTS = DlgMainMenu DlgVisuals DlgSounds DlgControls DlgDropDownList Dialog Item Button DropDownList Scrollbar Listbox Frame ButtonGroup DlgOnScreenKbd KbdButton
OBJECTS=main2 GetawayView GetawayDoc BSimulation OpenGLHelpers BCar BGround BCamera BObject BaseClasses SoundModule ControllerModule Settings $(addprefix NewDialogs/,$(NEW_DIALOG_OBJECTS))
ASSETS = World/CarJetHeli.gaw World/ground.raw Textures/monval.raw Textures/skysmall.raw Textures/skydetailed.raw Textures/seabed.raw Textures/cartexture.raw Textures/wheeldetailed.raw Textures/43aa.raw res/MenuFinal.bmp res/bitmap1.bmp Sounds/43aa.ogg Sounds/MixDown3.ogg Sounds/EngineJuliet2.ogg Sounds/crash.ogg Sounds/JetMono.ogg Sounds/huey.ogg FreeSans.ttf NewDialogs/controls.png NewDialogs/menu.png NewDialogs/sounds.png NewDialogs/visuals.png
EXECUTABLE=Pakoon

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS:%=%.o)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

Pakoon.html: $(OBJECTS:%=%.em.o) $(ASSETS)
	$(EMXX) $(EMXXLINK) $(filter %.o,$^) $(ASSETS:%=--preload-file %) -o $@

%.em.o: %.cpp
	$(EMXX) $(EMXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm `find . -name "*.o"` $(EXECUTABLE)
