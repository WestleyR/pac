.PHONY:
install: $(TARGET)
	chmod +x $(TARGET)
	cp -f $(TARGET) $(PREFIX)

