#!/bin/bash
mkdir -p ./log
#make clean > /dev/null 2>&1
make all > ./log/cxx_log_clean 2>&1
res=$?
chmod +x ./exe/ray

# colorize log
# https://github.com/cornet/ccze
if command -v ccze &> /dev/null
then
  ccze -A < ./log/cxx_log_clean > ./log/cxx_log
else
  echo "For color log install ccze (sudo pacman -S ccze)"
fi

# Handle failed builds
if [ $res -ne 0 ]; then
  echo -e "\e[91m\e[1mCompilation failed. Here's the log:\e[0m"
  if command -v ccze &> /dev/null
  then
    cat ./log/cxx_log
  else
    cat ./log/cxx_log_clean
  fi
else
  ./exe/ray "$@"
fi
