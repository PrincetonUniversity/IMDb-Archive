export TIMESTAMP=$(date +%Y%m%d-%H%M%S)
n_bits=(1 2 4 8 16)
clk_freqs=(205 203 214 233 270)
# n_bits=(32)
# clk_freqs=(410)

len=${#n_bits[@]}


for ((i=0; i<$len; i++));
do
  export P_NBITS=${n_bits[i]}
  export CLK_FREQ=${clk_freqs[i]}
  ./build-param.sh
done

