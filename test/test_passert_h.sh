#! /bin/sh

cmd="${0%.sh}.c -O3 -march=native -flto"
clang="-fuse-ld=lld -rtlib=compiler-rt"
gcc=""

CASE_COMPILER="clang gcc"
CASE_DBUG="NDEBUG DEBUG"
CASE_PRED="NPRED PRED"
CASE_ARGS="VALID EMPTY NOMSG XNARG XTARG NCASE"

tests_failed=""

for CMPL in ${CASE_COMPILER}; do
  for DBUG in ${CASE_DBUG}; do
    for PRED in ${CASE_PRED}; do
      for ARGS in ${CASE_ARGS}; do
  
         TSTRESULT="(FAIL)" 
         testnm="${CMPL}_${DBUG}_${PRED}_${ARGS}"
         declare "${testnm}"=
         CMD="${CMPL} -D${DBUG} -D${PRED} ${!CMPL} ${cmd} -D${ARGS}"
         CMDLINE="  ${CMD}:"

         eval "${CMD}" && {

           case "${!testnm:=$(./a.out 2>&1)}" in
             $'(nil)')
              [[ -z "${DBUG##N*}" || -z "${PRED##N*}" ]]                      &&
                                                          TSTRESULT="(PASS)" ;;
             $'\n'[^\"]*': Assertion `'*' failed.')
              [[ -n "${ARGS##[VX][NA]*}" && -z "${DBUG##D*}${PRED##P*}" ]]    &&
                                                          TSTRESULT="(PASS)" ;;
             $'\n"message string yay!"\n\n'*)
              [[ -n "${DBUG##N*}" && -n "${PRED##N*}" && -z "${ARGS##VA*}" ]] &&
                                                          TSTRESULT="(PASS)" ;;
             $'\n"first message string!"\n\n'*)
              [[ -n "${DBUG##N*}" && -n "${PRED##N*}" && -z "${ARGS##XN*}" ]] &&
                                                          TSTRESULT="(PASS)" ;;
           esac

           [ -z "${TSTRESULT#(PASS)}" ] || tests_failed="${tests_failed:+${tests_failed};}${testnm}"

           echo 1>&2 -e "${TSTRESULT}  ${CMDLINE}"
           echo -e "${!testnm}" |
              sed 1>&2 -r '{:B;$!N;$!bB; s/^[ \t\n]+//; s/(^|\n)/\1      >    /g;}'
           true                                                                  # ${CMD} &&
         } || {

           echo 1>&2 compilation failure
           echo 1>&2 command line: ${CMD@Q}
           exit -1                                                         # ${CMD} ||
         }        

      done                             #for ARGS
      echo 1>&2 '  -------'
    done                             #for PRED
  done                             #for DBUG
  echo 1>&2 -e '  :::::::\n  -------'
done                             #for COMPILER



if [ -z ${tests_failed} ]; then
    echo 1>&2 all tests pass.
    exit 0
else

    echo 1>&2 -e problems detected: the following tests fail \\n
    echo ${tests_failed@Q} | sed -r "s/;/\n/g; s/'//g"
    exit 1
fi

