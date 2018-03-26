.SUFFIXES: .o .cpp

SOURCES=$(wildcard *.cpp)
export OBJFOLDER:=$(OBJFOLDER)/basedatamodel
OBJECTS:=$(SOURCES:%.cpp=$(OBJFOLDER)/%.o)

################################################################################
#  Object Targets
################################################################################

.PHONY : all

all: $(OBJECTS)


$(OBJFOLDER)/%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                      | sed '\''s|\($*\)\.o[ :]*|\1.o $@ : |g'\'' \
                      | sed '\''s|$*\.o|$(OBJFOLDER)/$*.o|g'\'' > $@; \
                      [ -s $@ ] || rm -f $@'

-include $(OBJECTS:.o=.d)

$(OBJFOLDER)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<


################################################################################
#  Cleanup Targets
################################################################################

clean:
	@echo "   Remove .o and .d files from directory $(OBJFOLDER)"
	@rm -f $(OBJFOLDER)/*.o $(OBJFOLDER)/*.d


