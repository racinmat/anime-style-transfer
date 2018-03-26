.SUFFIXES: .o .cpp

SOURCES=$(wildcard *.cpp)
export OBJFOLDER:=$(OBJFOLDER)/tools
OBJS:=$(SOURCES:%.cpp=%.o)
OBJECTS:=$(addprefix $(OBJFOLDER)/, $(OBJS))
TOOLBINS:=$(addprefix $(TOOLBINFOLDER)/, $(OBJS:%.o=%_$(LIBTYPE)))


################################################################################
#  Tools Targets
################################################################################

.PHONY : sharedtools statictools

sharedtools : $(TOOLBINS)
statictools : $(TOOLBINS)



$(TOOLBINFOLDER)/%_shared : $(OBJFOLDER)/%.o $(BASEDIR)/libs/$(LIBNAME)
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -L$(BASEDIR)/libs -l$(basename $(LIBLINKNAME:lib%=%)) -Wl,-rpath,$(IBEOSDK_LIBDIR) $(LIBS) -o $@



$(TOOLBINFOLDER)/%_static : %.o $(BASEDIR)/libs/$(LIBNAME)
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -Wl,--whole-archive $(BASEDIR)/libs/$(LIBLINKNAME) -Wl,--no-whole-archive $(LIBS) -o $@

################################################################################
#  Object Targets
################################################################################

$(OBJFOLDER)/%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                      | sed '\''s|\($*\)\.o[ :]*|\1.o $@ : |g'\'' \
                      | sed '\''s|$*\.o|$(OBJFOLDER)/$*.o|g'\'' > $@; \
                      [ -s $@ ] || rm -f $@'

-include $(OBJECTS:.o=.d)

$(OBJFOLDER)/%.o: %.cpp $(OBJFOLDER)/%.d
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

################################################################################
#  Cleanup Targets
################################################################################

clean:
	@if [ -d $(OBJFOLDER) ]; then \
#		echo $(wildcard $(OBJFOLDER)/*.o); \
		echo $(wildcard $(OBJFOLDER)/*.d);
		rm -f $(wildcard $(OBJFOLDER)/*.o);
		rm -f $(wildcard $(OBJFOLDER)/*.d); \
	fi
	@if [ -d $(TOOLBINFOLDER) ]; then \
#		echo $(filter %_$(LIBTYPE), $(wildcard $(TOOLBINFOLDER)/*)); 
		rm -f $(filter %_$(LIBTYPE), $(wildcard $(TOOLBINFOLDER)/*)); \
	fi



