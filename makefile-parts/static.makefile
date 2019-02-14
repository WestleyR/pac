.PHONY:
static: $(SRC)
	$(CC) $(STATIC_BUILD_FLAG) $(FLAG) -o $(TARGET) $(SRC)

