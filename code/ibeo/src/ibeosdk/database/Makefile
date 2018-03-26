.SUFFIXES: .o .cpp

SOURCES=$(wildcard *.cpp)
export OBJFOLDER:=$(OBJFOLDER)/database
OBJECTS:=$(SOURCES:%.cpp=$(OBJFOLDER)/%.o)

FOLDER_DATAMODEL=datamodel
FOLDER_BASEDATAMODEL=basedatamodel




MY_TARGETS = $(FOLDER_DATAMODEL) $(FOLDER_BASEDATAMODEL)


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
	@if [ -d $(OBJFOLDER)/$(FOLDER_DATAMODEL) ];       then $(MAKE) -C $(FOLDER_DATAMODEL) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_BASEDATAMODEL) ];       then $(MAKE) -C $(FOLDER_BASEDATAMODEL) clean; fi
	@echo "   Remove .o and .d files from directory $(OBJFOLDER)"
	@rm -f $(OBJFOLDER)/*.o $(OBJFOLDER)/*.d

-include $(OBJECTS:.o=.d)

$(OBJFOLDER)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
