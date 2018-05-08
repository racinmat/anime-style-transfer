FILENAME = jasek
TARGET ?= pdf
TO_REMOVE = aux bbl blg fdb_latexmk idx ilg ind lof log lot out toc fls
DEPENDS = $(wildcard text/*.tex) $(wildcard img/*.tex) $(FILENAME).tex

.PHONY: clean fullclean

all: $(FILENAME).$(TARGET)

$(FILENAME).$(TARGET): $(DEPENDS)
	latexmk -$(TARGET) $(FILENAME)

clean:
	rm -rf $(foreach ext,$(TO_REMOVE),$(wildcard *.$(ext)))

fullclean: clean
	rm -rf $(FILENAME).$(TARGET)
