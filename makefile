CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags gtk+-2.0` -Isrc/vols -DPACKAGE_DATA_DIR=\"data\" -DPACKAGE_SOURCE_DIR=\".\" -DPACKAGE=\"vols\"
LIBS = `pkg-config --libs gtk+-2.0`
OBJ = ui/src/main.o ui/src/interface.o ui/src/callbacks.o ui/src/support.o src/vols/vols.o ui/src/vols_ui.o

vols: $(OBJ)
	$(CC) $(OBJ) -o vols $(LIBS)

clean:
	rm -f $(OBJ) vols test_vols demo_design

test_vols: src/vols/test_vols.c src/vols/vols.c
	$(CC) -Wall -g -Isrc/vols src/vols/test_vols.c src/vols/vols.c -o test_vols

demo_design: src/common/demo_design.c src/common/design.c
	$(CC) $(CFLAGS) -Isrc/common src/common/demo_design.c src/common/design.c -o demo_design $(LIBS)
