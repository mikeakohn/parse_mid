
default:
	@+make -C build
	@+make -C build lib

clean:
	@rm -f build/*.o libparse_mid.so parse_mid
	@echo "Clean!"

