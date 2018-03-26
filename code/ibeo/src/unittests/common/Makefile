SOURCES=$(wildcard *.cpp)
export OBJFOLDER:=$(OBJFOLDER)/common
OBJECTS:=$(SOURCES:%.cpp=$(OBJFOLDER)/%.o)

UNITTESTSCOMMONOBJECTS=$(OBJECTS)
export UNITTESTSCOMMONOBJECTS

.PHONY : all output

all: $(OBJECTS)

$(OBJFOLDER)/%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                      | sed '\''s|\($*\)\.o[ :]*|\1.o $@ : |g'\'' \
                      | sed '\''s|$*\.o|$(OBJFOLDER)/$*.o|g'\'' > $@; \
                      [ -s $@ ] || rm -f $@'


clean:
	@echo "   Remove .o and .d files from directory $(OBJFOLDER)"
	@rm -f $(OBJFOLDER)/*.o $(OBJFOLDER)/*.d

-include $(OBJECTS:.o=.d)

$(OBJFOLDER)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
