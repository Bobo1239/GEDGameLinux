SOURCE_FILES = $(shell find . -type f -iregex '.*\.\(cpp\|hpp\|fx\)' -not -path "./build/*" -not -path "./vendor/*")

.PHONY=format
format:
	clang-format -i $(SOURCE_FILES)

apply-linux-patches:
	-cd vendor/DXUT && git apply ../DXUT.patch
	-cd vendor/FX11 && git apply ../FX11.patch
