import cplex
import sys


lp_file = "Int_prog.lp" 
re_file = "result.txt" 
c = cplex.Cplex(lp_file) 
c.set_log_stream(None) 
c.set_error_stream(None) 
c.set_warning_stream(None) 
c.set_results_stream(None) 
c.solve() 
vals = c.solution.get_values() 
fout = open(re_file,"w") 
fout.write(" \n".join([str(int(p+0.1)) for p in vals])) 
fout.close()