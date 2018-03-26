.SUFFIXES: .o .cpp

ifeq ($(WITHMONGODB), on)
SOURCES=$(wildcard *.cpp)
else
SOURCES=$(filter-out IbeoSdkDbDemo.cpp IbeoSdkDbReplayDemo.cpp IbeoSdkDbInfoDemo.cpp IbeoSdkDb2IdcDemo.cpp IbeoSdkDbRoadMarkingDemo.cpp, $(wildcard *.cpp))
endif

export OBJFOLDER:=$(OBJFOLDER)/demo
OBJS:=$(SOURCES:%.cpp=%.o)
OBJECTS:=$(addprefix $(OBJFOLDER)/, $(OBJS))
DEMOBINS:=$(addprefix $(DEMOBINFOLDER)/, $(OBJS:%.o=%_$(LIBTYPE)))

ifeq ($(WITHJPEGSUPPORT),on)
	WITHJPEGSUPPORTDEF=-DWITHJPEGSUPPORT
endif

ifeq ($(WITHMONGODB), on)
	WITHMONGODBDEF=-DWITHMONGODB
endif

################################################################################
#  Demo Targets
################################################################################

.PHONY : shareddemos staticdemos

shareddemos : $(DEMOBINS)
staticdemos : $(DEMOBINS)


$(DEMOBINFOLDER)/%_shared : $(OBJFOLDER)/%.o $(BASEDIR)/libs/$(LIBNAME)
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -L$(BASEDIR)/libs -l$(basename $(LIBLINKNAME:lib%=%)) -Wl,-rpath,$(IBEOSDK_LIBDIR) $(LIBS) -o $@



$(DEMOBINFOLDER)/%_static : %.o $(BASEDIR)/libs/$(LIBNAME)
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
	@$(CXX) $(CXXFLAGS) $(WITHJPEGSUPPORTDEF) $(WITHMONGODBDEF) -o $@ -c $<

################################################################################
#  Cleanup Targets
################################################################################

clean:
	@if [ -d $(OBJFOLDER) ]; then \
#		echo $(wildcard $(OBJFOLDER)/*.o); \
#		echo $(wildcard $(OBJFOLDER)/*.d);
		rm -f $(wildcard $(OBJFOLDER)/*.o); \
		rm -f $(wildcard $(OBJFOLDER)/*.d); \
	fi
	@if [ -d $(DEMOBINFOLDER) ]; then \
#		echo $(filter %_$(LIBTYPE), $(wildcard $(DEMOBINFOLDER)/*));
		rm -f $(filter %_$(LIBTYPE), $(wildcard $(DEMOBINFOLDER)/*)); \
	fi



