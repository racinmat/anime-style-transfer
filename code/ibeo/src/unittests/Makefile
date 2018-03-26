#.SUFFIXES: .o .cpp
SOURCES=$(wildcard *.cpp)
export OBJFOLDER:=$(OBJFOLDER)/unittests
OBJS:=$(SOURCES:%.cpp=%.o)
OBJECTS:=$(addprefix $(OBJFOLDER)/, $(OBJS))
UNITTESTBINS:=$(addprefix $(UNITTESTBINFOLDER)/, $(OBJS:%.o=%_$(LIBTYPE)))

FOLDER_COMMON=common

MY_TARGETS = $(FOLDER_COMMON)

COMMONSOURCES=$(wildcard $(FOLDER_COMMON)/*.cpp)
COMMONOBJECTS:=$(COMMONSOURCES:%.cpp=%.o)


################################################################################
#  UnitTests Targets
################################################################################

.PHONY : $(MY_TARGETS) sharedtests statictests

sharedtests : $(MY_TARGETS) $(UNITTESTBINS)
statictests : $(MY_TARGETS) $(UNITTESTBINS)

$(MY_TARGETS):
	@if [ ! -d "$(OBJFOLDER)/$@" ]; then mkdir -p $(OBJFOLDER)/$@; fi;
	$(MAKE) -C $@ all


$(UNITTESTBINFOLDER)/%_shared : $(OBJFOLDER)/%.o $(COMMONOBJECTS) $(BASEDIR)/libs/$(LIBNAME)
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -L$(BASEDIR)/libs -l$(basename $(LIBLINKNAME:lib%=%)) -Wl,-rpath,$(IBEOSDK_LIBDIR) $(LIBS) $(ADDITIONALUNITTESTLIBS) -o $@



$(UNITTESTBINFOLDER)/%_static : $(OBJFOLDER)/%.o  $(COMMONOBJECTS) $(BASEDIR)/libs/$(LIBNAME)
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -Wl,--whole-archive $(BASEDIR)/libs/$(LIBLINKNAME) -Wl,--no-whole-archive $(LIBS) $(ADDITIONALUNITTESTLIBS) -o $@

################################################################################
#  Object Targets
################################################################################

-include $(OBJECTS:.o=.d)

$(OBJFOLDER)/%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                      | sed '\''s|\($*\)\.o[ :]*|\1.o $@ : |g'\'' \
                      | sed '\''s|$*\.o|$(OBJFOLDER)/$*.o|g'\'' > $@; \
                      [ -s $@ ] || rm -f $@'

$(OBJFOLDER)/%.o: %.cpp $(OBJFOLDER)/%.d
	@$(CXX) $(CXXFLAGS) -DLINKTYPE_$(LIBTYPE) -o $@ -c $<

################################################################################
#  Cleanup Targets
################################################################################

clean:
	@if [ -d $(OBJFOLDER)/$(FOLDER_COMMON) ]; then $(MAKE) -C $(FOLDER_COMMON) clean; fi
	@if [ -d $(OBJFOLDER) ]; then \
#		echo $(wildcard $(OBJFOLDER)/*.o); \
#		echo $(wildcard $(OBJFOLDER)/*.d); 
		rm -f $(wildcard $(OBJFOLDER)/*.o); \
		rm -f $(wildcard $(OBJFOLDER)/*.d); \
	fi
	@if [ -d $(UNITTESTBINFOLDER) ]; then \
#		echo $(filter %_$(LIBTYPE), $(wildcard $(UNITTESTBINFOLDER)/*)); 
		rm -f $(filter %_$(LIBTYPE), $(wildcard $(UNITTESTBINFOLDER)/*)); \
	fi
