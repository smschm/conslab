# conslab

### MATLAB with more parentheses

#### Building, installing, using

    $ mex -lguile guile.c
    $ matlab
    >> addpath /path/to/conslab
    >> guile
    guile> (+ 4 5)
    9   
    guile> (: "z = 9;")
    #t
    guile> (keyboard)
    K>> whos
    whos
      Name      Size            Bytes  Class     Attributes
    
      z         1x1                 8  double              
    
    K>> return
    return
    #t
    guile> (quit)