bench: build
	./build/merle_benchmarks

test: build
	./build/merle_tests

build: gen
	cmake --build --preset default

gen:
	cmake --preset default

clean:
	rm -rf build
	rm -rf site

sync:
	git submodule update --init --recursive --jobs 8

docs:
	mkdocs serve

docker:
	docker build --platform linux/amd64 . --tag merle
