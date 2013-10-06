echo $(tput bold)$(tput setaf 5)"--------------------------------------------"
echo "--------------------------------------------"
echo $(tput bold)$(tput setaf 1)"----------------------"
echo "-- GENERATING CMAKE --"
echo "----------------------"$(tput sgr0)
cmake -G"Unix Makefiles" pro/make_$1/
cd pro/make_$1/
time make all
cd ../../bin
echo $(tput bold)$(tput setaf 1)"----------------------"
echo "-- RUNNING PROJECT  --"
echo "----------------------"$(tput sgr0)
time ./ELF
cd ../
echo $(tput bold)$(tput setaf 1)"----------------------"
echo $(tput bold)$(tput setaf 5)"--------------------------------------------"
echo "--------------------------------------------"$(tput sgr0)
