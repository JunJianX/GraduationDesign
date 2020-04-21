#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)


COMPONENT_ADD_INCLUDEDIRS += include
COMPONENT_ADD_INCLUDEDIRS += eng/certs
COMPONENT_ADD_INCLUDEDIRS += eng/dev_model
COMPONENT_ADD_INCLUDEDIRS += eng/dev_sign
COMPONENT_ADD_INCLUDEDIRS += eng/infra
COMPONENT_ADD_INCLUDEDIRS += eng/mqtt
COMPONENT_ADD_INCLUDEDIRS += eng/wrappers
COMPONENT_ADD_INCLUDEDIRS += eng/eng
COMPONENT_ADD_INCLUDEDIRS += eng/wrappers/ubuntu


COMPONENT_SRCDIRS += src
COMPONENT_SRCDIRS += eng/dev_sign
COMPONENT_SRCDIRS += eng/infra
COMPONENT_SRCDIRS += eng/wrappers
COMPONENT_SRCDIRS += eng/mqtt
COMPONENT_SRCDIRS += eng/dev_model
COMPONENT_SRCDIRS += eng/certs
# COMPONENT_SRCDIRS += eng/wrappers/ubuntu

