SYSTEMC_DIR=~/obj/
ILANG_PATH=~/ILAng/
IMDB_PATH=~/IMDb/

DEMO_MODEL_PATH=$IMDB_PATH/accls/AES/AES-ILASIM/
mkdir ~/ila_obj/
cd $ILANG_PATH
mkdir build 
cd build
cmake .. -DILANG_INSTALL_DEV=on -DCMAKE_INSTALL_PREFIX=~/ila_obj/
make -j
make install

cd $DEMO_MODEL_PATH
mkdir build 
cd build
cmake .. -DCMAKE_PREFIX_PATH=~/ila_obj/
make
mkdir co_sim_model/
./AES_ILASIM ./co_sim_model/ $SYSTEMC_DIR
cd co_sim_model/
source ./mk.sh
