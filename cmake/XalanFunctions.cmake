# CMake build for xerces-c
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

# function checks

include(CheckSymbolExists)

include(CheckCXXSourceCompiles)

check_symbol_exists(signal "signal.h" XALAN_HAVE_SIGNAL)
check_symbol_exists(SetConsoleCtrlHandler "windows.h" XALAN_HAVE_WIN32_SET_CONSOLE_CTRL_HANDLER)

check_symbol_exists(usleep "unistd.h" XALAN_HAVE_USLEEP)
check_symbol_exists(Sleep "windows.h" XALAN_HAVE_WIN32_SLEEP)

check_symbol_exists(gmtime_r "time.h" XALAN_HAVE_GMTIME_R)
check_symbol_exists(localtime_r "time.h" XALAN_HAVE_LOCALTIME_R)

check_symbol_exists(_create_locale "windows.h" XALAN_USE_WINDOWS_COLLATION)

check_symbol_exists(isnan "math.h" XALAN_HAVE_ISNAN)
check_symbol_exists(_isnan "math.h" XALAN_HAVE__ISNAN)

check_cxx_source_compiles("
#include <cmath>
#include <cfloat>

int main() {
  std::isnan(0.0);
  std::isnan(NAN);
}"
XALAN_HAVE_STD_ISNAN)
