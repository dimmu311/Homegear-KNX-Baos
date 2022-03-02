AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -Wall -std=c++14 -DFORTIFY_SOURCE=2 -DGCRYPT_NO_DEPRECATED -IPhysicalInterfaces/baos/kdrive/include -IPhysicalInterfaces/baos/build_unix/sdk/include -lkdriveBAOS
AM_LDFLAGS = -Wl,-rpath=/lib/homegear -Wl,-rpath=/usr/lib/homegear -Wl,-rpath=/usr/local/lib/homegear

libdir = $(localstatedir)/lib/homegear/modules
lib_LTLIBRARIES = mod_knx_baos.la
mod_knx_baos_la_SOURCES = Baos.cpp Factory.cpp BaosPacket.cpp GD.cpp BaosPeer.cpp PhysicalInterfaces/Serial.cpp BaosCentral.cpp Interfaces.cpp MakeDeviceDescriptionFile.cpp
mod_knx_baos_la_LDFLAGS =-module -avoid-version -shared
install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/mod_knx_baos.la
