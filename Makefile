
default:
	@+make -C build
	@+make -C build lib
	@+make -C build player

clean:
	@rm -f build/*.o libparse_mid.so parse_mid
	@echo "Clean!"

