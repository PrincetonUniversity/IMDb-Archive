# $SYSTEM_DIR, $ILANG_PATH, $IMDB_PATH are needed

DEMO_MODEL_PATH=$IMDB_PATH/accls/AES/AES-ILASIM/
mkdir $ILANG_PATH/ila_obj/
cd $ILANG_PATH
mkdir build 
cd build
cmake .. -DILANG_INSTALL_DEV=on -DCMAKE_INSTALL_PREFIX=$ILANG_PATH/ila_obj/
make -j2
make install

cd $DEMO_MODEL_PATH
mkdir build 
cd build
cmake .. -DCMAKE_PREFIX_PATH=$ILANG_PATH/ila_obj/
make
mkdir co_sim_model/
./AES_ILASIM ./co_sim_model/ $SYSTEMC_DIR
cd co_sim_model/
chmod +x ./mk.sh
./mk.sh
