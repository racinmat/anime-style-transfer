SOURCES:=$(wildcard *.cpp)
export OBJFOLDER:=$(OBJFOLDER)/src
OBJECTS:=$(SOURCES:%.cpp=$(OBJFOLDER)/%.o)
export DEMOBINFOLDER
export TOOLBINFOLDER
export UNITTESTBINFOLDER

FOLDER_IBEOSDK=ibeosdk
FOLDER_DEMO=demo
FOLDER_TOOLS=tools
FOLDER_UNITTEST=unittests

MY_TARGETS = $(FOLDER_IBEOSDK)
MY_DEMOTARGETS = $(FOLDER_DEMO)
MY_TOOLTARGETS = $(FOLDER_TOOLS)
MY_UNITTESTTARGETS = $(FOLDER_UNITTEST)


################################################################################
#  Library Targets
################################################################################

.PHONY : all $(MY_TARGETS)
all: $(MY_TARGETS) $(OBJECTS)

#.NOTPARALLEL :
$(MY_TARGETS):
	@if [ ! -d "$(OBJFOLDER)/$@" ]; then mkdir -p $(OBJFOLDER)/$@; fi;
	$(MAKE) -C $@


$(OBJFOLDER)/%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                      | sed '\''s|\($*\)\.o[ :]*|\1.o $@ : |g'\'' \
                      | sed '\''s|$*\.o|$(OBJFOLDER)/$*.o|g'\'' > $@; \
                      [ -s $@ ] || rm -f $@'

################################################################################
#  Demo Targets
################################################################################

.PHONY : shareddemos staticdemos

shareddemos :  all
	@if [ ! -d "$(OBJFOLDER)/demo" ]; then mkdir -p "$(OBJFOLDER)/demo"; fi;
	$(MAKE) -C $(FOLDER_DEMO) shareddemos LIBTYPE=shared

staticdemos : all
	@if [ ! -d "$(OBJFOLDER)/demo" ]; then mkdir -p "$(OBJFOLDER)/demo"; fi;
	$(MAKE) -C $(FOLDER_DEMO) staticdemos LIBTYPE=static


################################################################################
#  Tool Targets
################################################################################

.PHONY : sharedtools statictools

sharedtools :  all
	@if [ ! -d "$(OBJFOLDER)/tools" ]; then mkdir -p "$(OBJFOLDER)/tools"; fi;
	$(MAKE) -C $(FOLDER_TOOLS) sharedtools LIBTYPE=shared

statictools : all
	@if [ ! -d "$(OBJFOLDER)/tools" ]; then mkdir -p "$(OBJFOLDER)/tools"; fi;
	$(MAKE) -C $(FOLDER_TOOLS) statictools LIBTYPE=static


################################################################################
#  Test Targets
################################################################################

.PHONY : sharedtests statictests

sharedtests :  all
	@if [ ! -d "$(OBJFOLDER)/unittests" ]; then mkdir -p "$(OBJFOLDER)/unittests"; fi;
	$(MAKE) -C $(FOLDER_UNITTEST) sharedtests LIBTYPE=shared

statictests : all
	@if [ ! -d "$(OBJFOLDER)/unittests" ]; then mkdir -p "$(OBJFOLDER)/unittests"; fi;
	$(MAKE) -C $(FOLDER_UNITTEST) statictests LIBTYPE=static


################################################################################
#  Cleanup Targets
################################################################################


.PHONY : clean cleanlibobjects docs docsclean

clean:
	@if [ -d $(OBJFOLDER)/$(FOLDER_IBEOSDK) ]; then $(MAKE) -C $(FOLDER_IBEOSDK) clean; fi
	@echo "Remove .o and .d files from directory $(OBJFOLDER)"
	@rm -f $(OBJFOLDER)/*.o $(OBJFOLDER)/*.d

democlean:
	$(MAKE) -C $(FOLDER_DEMO) clean

toolclean:
	$(MAKE) -C $(FOLDER_DEMO) clean
	
unittestclean:
	$(MAKE) -C $(FOLDER_DEMO) clean

################################################################################
#  Documentation Targets
################################################################################
docs:
	doxygen ./Doxyfile

docsclean:
	rm -rf docs/latex
	rm -rf docs/html



################################################################################
#  Dependency include
################################################################################

-include $(OBJECTS:.o=.d)

