onehots = [\
  ('m1.acc_0.state_rbm_0_cmos32soi_rbm_train_rbm_next', 95), \
  ('m1.acc_0.state_rbm_0_cmos32soi_rbm_train_rbm', 95), \
  ('m1.acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm_next', 39), \
  ('m1.acc_0.state_rbm_0_cmos32soi_rbm_predict_rbm', 39), \
  ('m1.acc_0.state_rbm_0_cmos32soi_rbm_load_next', 16), \
  ('m1.acc_0.state_rbm_0_cmos32soi_rbm_load', 16), \
  ('m1.acc_0.state_rbm_0_cmos32soi_rbm_store_next', 7), \
  ('m1.acc_0.state_rbm_0_cmos32soi_rbm_store', 7) \
]

with open('inv.txt','w') as fout:
  for n, w in onehots:
    # must have one
    print ('"'+ ' | '.join( [n + '[%d]' % i for i in range(w)] ) + '",', file = fout)
    # must not have two
    for i in range(w):
      for j in range(i+1,w):
        print ('"!(%s[%d] & %s[%d])",' % (n,i,n,j), file = fout)
