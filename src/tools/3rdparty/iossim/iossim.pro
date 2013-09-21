CONFIG   += console

CONFIG -= app_bundle

include(../../../../qtcreator.pri)

# Prevent from popping up in the dock when launched.
# We embed the Info.plist file, so the application doesn't need to
# be a bundle.
QMAKE_LFLAGS += -sectcreate __TEXT __info_plist $$shell_quote($$PWD/Info.plist) \
  -fobjc-link-runtime

LIBS += \
  -framework Foundation \
  -framework CoreServices \
  -framework ApplicationServices \
  -framework CoreFoundation \
  -F/System/Library/PrivateFrameworks \
  -framework IOKit

iPhoneSimulatorRemoteClientDirectLinking {
  LIBS += \
    -F/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/Library/PrivateFrameworks \
    -F/Applications/Xcode.app/Contents/OtherFrameworks

    QMAKE_LFLAGS += -Wl,-rpath,/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/Library/PrivateFrameworks \
    -Wl,-rpath,/Applications/Xcode.app/Contents/OtherFrameworks \
  LIBS += \
    -framework iPhoneSimulatorRemoteClient
  QMAKE_RPATHDIR += /Applications/Xcode5-DP5.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/Library/PrivateFrameworks \
    /Applications/Xcode.app/Contents/OtherFrameworks
    /System/Library/PrivateFrameworks \
}

TEMPLATE = app

DESTDIR = $$IDE_BIN_PATH
include(../../../rpath.pri)

OBJECTIVE_SOURCES += \
  main.mm \
  nsprintf.m \
  nsstringexpandPath.m \
  iphonesimulator.m

HEADERS += \
  iphonesimulator.h \
  nsprintf.h \
  nsstringexpandpath.h \
  version.h \
  iphonesimulatorremoteclient/iphonesimulatorremoteclient.h

OTHER_FILES = LICENSE