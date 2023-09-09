
default:
	@+make -C build

clean:
	@rm -f build/*.o libparsemid.so
	@echo "Clean!"

