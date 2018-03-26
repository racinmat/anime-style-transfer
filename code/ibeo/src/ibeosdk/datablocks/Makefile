.SUFFIXES: .o .cpp

SOURCES=$(wildcard *.cpp)
export OBJFOLDER:=$(OBJFOLDER)/datablocks
OBJECTS:=$(SOURCES:%.cpp=$(OBJFOLDER)/%.o)

FOLDER_COMMANDS=commands
FOLDER_SNIPPETS=snippets
MY_TARGETS = $(FOLDER_COMMANDS) $(FOLDER_SNIPPETS)


.PHONY : $(MY_TARGETS) all clean

all: $(MY_TARGETS) $(OBJECTS)

#$(MY_TARGETS): CXXFLAGS+=$(LIBOBJCXXFLAGS)
$(MY_TARGETS): FORCE
	@if [ ! -d "$(OBJFOLDER)/$@" ]; then mkdir -p $(OBJFOLDER)/$@; fi;
	$(MAKE) -C $@ all


FORCE:

$(OBJFOLDER)/%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                      | sed '\''s|\($*\)\.o[ :]*|\1.o $@ : |g'\'' \
                      | sed '\''s|$*\.o|$(OBJFOLDER)/$*.o|g'\'' > $@; \
                      [ -s $@ ] || rm -f $@'

clean:
	@if [ -d $(OBJFOLDER)/$(FOLDER_COMMANDS) ];       then $(MAKE) -C $(FOLDER_COMMANDS) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_SNIPPETS) ];       then $(MAKE) -C $(FOLDER_SNIPPETS) clean; fi
	@echo "   Remove .o and .d files from directory $(OBJFOLDER)"
	@rm -f $(OBJFOLDER)/*.o $(OBJFOLDER)/*.d

-include $(OBJECTS:.o=.d)

$(OBJFOLDER)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
