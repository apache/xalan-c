# CMake build for xalan-c
#
# Written by Roger Leigh <rleigh@codelibre.net>
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

message(STATUS "Checking for doxygen errors in ${logfile}")

file(READ "${logfile}" LOG)
string(LENGTH "${LOG}" LOGLEN)

if(LOGLEN GREATER 0)
  message(WARNING "Doxygen encountered undocumented code or errors in ${logfile}")
  message("══════════════════════════════ UNDOCUMENTED CODE ══════════════════════════════")
  message("${LOG}")
  message("═════════════════════════════ END UNDOCUMENTED CODE ═══════════════════════════")
endif(LOGLEN GREATER 0)
