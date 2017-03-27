A=readSMAT('../../graph/usps_3nn_list.smat');
seedids=[7575];
nseedids=1;
alpha=0.99;
eps=0.0001;
rho=0.0;
[xlength,~]=size(A);
[actual_length,xids]=ppr_path(A,seedids,nseedids, ...
                              alpha,eps,rho,xlength);
actual_length
xids'