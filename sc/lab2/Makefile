CC=gcc

CFLAGS=-Wall

TARGET=app

BUILD_DIR=build
SOURCE_DIR=source
INCLUDE_DIR=include
RESOURCES_DIR=resources

$(TARGET): build_dir
	@$(CC) $(CFLAGS) $(SOURCE_DIR)/main.c -o $(BUILD_DIR)/$(TARGET)

build_dir:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGET)

run:
	@./$(TARGET)
