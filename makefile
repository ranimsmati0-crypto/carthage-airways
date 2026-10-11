CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags gtk+-2.0` -Isrc/vols -DPACKAGE_DATA_DIR=\"data\" -DPACKAGE_SOURCE_DIR=\".\" -DPACKAGE=\"vols\"
LIBS = `pkg-config --libs gtk+-2.0`
OBJ = ui/src/main.o ui/src/interface.o ui/src/callbacks.o ui/src/support.o src/vols/vols.o src/vols/vols_ui.o

vols: $(OBJ)
	$(CC) $(OBJ) -o vols $(LIBS)

clean:
	rm -f $(OBJ) vols
