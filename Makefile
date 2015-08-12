override CXXFLAGS += -Wall -std=c++14 -fPIC -fvisibility=hidden $(shell pkg-config glib-2.0 --cflags)
override NAME := pam_restrict
override TARGET := pam_restrict.so
override OBJECTS := $(NAME).o configfile.o restrict.o logger.o exception.o
override LDLIBS := $(shell pkg-config glib-2.0 --libs)
PREFIX := /usr
LIBDIR := $(PREFIX)/lib
LIBSECURITYDIR := $(LIBDIR)/security
CONFDIR := /etc
CONFSECURITYDIR := $(CONFDIR)/security
CONFIGFILE := $(NAME).conf
override DEFINES := -DCONFIGFILE_NAME=\"$(CONFSECURITYDIR)/$(CONFIGFILE)\"

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $^ $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEFINES) -c $<

configfile.o: configfile.h exception.h
$(NAME).o: restrict.h configfile.h logger.h
restrict.o: restrict.h configfile.h exception.h logger.h
logger.o: logger.h
exception.o: exception.h

install: $(TARGET) $(CONFIGFILE)
	install -D -m755 $(TARGET) $(DESTDIR)$(LIBSECURITYDIR)/$(TARGET)
	install -D -m644 $(CONFIGFILE) $(DESTDIR)$(CONFSECURITYDIR)/$(CONFIGFILE)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean install
