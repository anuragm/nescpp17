all: lint

lint:
	git ls-files '*.cpp' '*.hpp' '*.h' -z --full-name | xargs -0 clang-format-11 -i
