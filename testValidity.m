%test the case vectors from reachability output

NNfile = './BGnetwork_files/PSO3-001-0001_high_APNN';
G_I_input_vector_low=load('lowerVector3.txt');
NNout_l=NN_output(G_I_input_vector_low,0,1,NNfile);

G_I_input_vector_high=load('upperVector3.txt');
NNout_u=NN_output(G_I_input_vector_high,0,1,NNfile);
display(['lower: ',num2str(NNout_l),'       upper: ',num2str(NNout_u)]);