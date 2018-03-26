.SUFFIXES: .o .cpp

ifeq ($(WITHMONGODB), on)
SOURCES=$(wildcard *.cpp)
else
SOURCES=$(filter-out DbReader.cpp DatabaseReplayer.cpp, $(wildcard *.cpp))
endif

export OBJFOLDER:=$(OBJFOLDER)/devices
OBJECTS:=$(SOURCES:%.cpp=$(OBJFOLDER)/%.o)

ifeq ($(WITHMONGODB), on)
	FOLDER_DATABASE=database
endif
FOLDER_SUTP=sutp


MY_TARGETS = $(FOLDER_DATABASE) $(FOLDER_SUTP)
.PHONY : $(MY_TARGETS) all output

all: $(MY_TARGETS) $(OBJECTS)

$(MY_TARGETS): CXXFLAGS+=$(LIBOBJCXXFLAGS)
$(MY_TARGETS):
	@if [ ! -d "$(OBJFOLDER)/$@" ]; then mkdir -p $(OBJFOLDER)/$@; fi;
	$(MAKE) -C $@ all

$(OBJFOLDER)/%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                      | sed '\''s|\($*\)\.o[ :]*|\1.o $@ : |g'\'' \
                      | sed '\''s|$*\.o|$(OBJFOLDER)/$*.o|g'\'' > $@; \
                      [ -s $@ ] || rm -f $@'


clean:
	@if [ -d $(OBJFOLDER)/$(FOLDER_DATABASE) ];   then $(MAKE) -C $(FOLDER_DATABASE) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_SUTP) ];       then $(MAKE) -C $(FOLDER_SUTP) clean; fi
	@echo "   Remove .o and .d files from directory $(OBJFOLDER)"
	@rm -f $(OBJFOLDER)/*.o $(OBJFOLDER)/*.d

-include $(OBJECTS:.o=.d)

$(OBJFOLDER)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
