# File IO helper commands
# 
# To better abstract away the host platform from the build system, common (but
# platform dependent) file IO commands are defined as Make variables. This is
# mostly done to allow developers who use windows to seamlessly use the projects
# build without modifying every use of a file operation.
#
# The expectations are defined as follows:
#
#   - The RM variable contains the command string necessary to recursively
#     remove a given path (file or object). If the path does not exists, the
#     command should exit WITHOUT error.
#
#   - The MKDIR contains the command string necessary to recuresively create
#     a directory tree given a path. The script should exist successfully even
#     if a directory from the given path is already present.
#
# Inputs:
#   None
#
# Outputs:
#   RM
#   MKDIR

RM      := rm -rf
MKDIR   := mkdir -p