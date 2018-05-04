FILENAME = jasek
TARGET ?= pdf
TO_REMOVE = aux bbl blg fdb_latexmk idx ilg ind lof log lot out toc fls

.PHONY: $(FILENAME).$(TARGET) clean

all: $(FILENAME).$(TARGET)

$(FILENAME).$(TARGET):
	latexmk -$(TARGET) $(FILENAME)

clean:
	rm -rf $(foreach ext,$(TO_REMOVE),$(wildcard *.$(ext)))
