#include "mex.h"
#include "matrix.h"
#include <libguile.h>
#include <termios.h>
#include <unistd.h>

void debug_flags(const char* fn) {
    struct termios t0, t1, t;
    tcgetattr(0, &t0);
    tcgetattr(1, &t1);

    FILE * f; int i;
    f = fopen(fn,"w");
    fprintf(f,"input  %x %x %x %x\n  ccs: ", t0.c_iflag, t0.c_oflag, t0.c_cflag, t0.c_lflag, t0.c_line);
    for (i = 0; i < NCCS; i++) fprintf(f,"%x ",t0.c_cc[i]);
    fprintf(f,"\n");
    fprintf(f,"output %x %x %x %x\n  ccs: ", t1.c_iflag, t1.c_oflag, t1.c_cflag, t1.c_lflag, t1.c_line);
    for (i = 0; i < NCCS; i++) fprintf(f,"%x ",t1.c_cc[i]);
    fprintf(f,"\n");
    fprintf(f, "ECHO = %x, INLCR = %x\n", ECHO, INLCR);
    fclose(f);
    return;
}

void fix_flags() {
    struct termios t0, t1, t;
    tcgetattr(0,&t);
    /*t.c_iflag = 0x500; t.c_oflag = 0x5; t.c_cflag = 0xbf; t.c_lflag = 0x8a3b;*/
    t.c_iflag |= ICRNL; t.c_lflag |= (ICANON | ECHO);
    tcsetattr(0,TCSANOW,&t);
    tcgetattr(1,&t);
    /*t.c_iflag = 0x500; t.c_oflag = 0x5; t.c_cflag = 0xbf; t.c_lflag = 0x8a3b;*/
    t.c_iflag |= ICRNL; t.c_lflag |= (ICANON | ECHO);
    tcsetattr(1,TCSANOW,&t);
}

static SCM keyboard() {
    int res;
    res = mexCallMATLAB(0,NULL,0,NULL,"keyboard");
    fix_flags();
    return (res ? SCM_BOOL_F : SCM_BOOL_T);
}

static SCM matlab_eval(SCM scm_cmd) {
    char* cmd = scm_to_locale_string(scm_cmd);
    int res;
    res = mexEvalString(cmd);
    free(cmd);
    fix_flags();
    return (res ? SCM_BOOL_F : SCM_BOOL_T);
}

static void * register_functions(void * data) {
    scm_c_define_gsubr("matlab-eval", 1, 0, 0, &matlab_eval);
    scm_c_define_gsubr(":", 1, 0, 0, &matlab_eval);
    scm_c_define_gsubr("keyboard", 0, 0, 0, &keyboard);
    return NULL;
}

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
    fix_flags();
    scm_with_guile(&register_functions, NULL);
    scm_shell(0, NULL);
    return;
}
