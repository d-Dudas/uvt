CC=gcc

CFLAGS=-Wall

TARGET=app

$(TARGET): main.c
	@$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	rm -f $(TARGET)

run:
	@./$(TARGET)
