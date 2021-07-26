/***
 * API
 *
 * ARITH_INCR(n)
 * ARITH_DECR(n)
 *
 * ARITH_ADD(n,m)
 * ARITH_SUB(n,m)
 *
 * ARITH_MUL(n,m)
 *
 * ARITH_DIV(n,m)
 * ARITH_MOD(n,m)
 *
 * ARITH_MIN(n,m)
 * ARITH_MAX(n,m)
 * 
 * ARITH_REP(n,x)           // expands `x` `n` times; each non-final result is emitted
 *                          //  with a trailing comma.
 *
 * ARITH_SEQ(n,m)           // emits numbers `n` through `m`, comma separated.
 *                          // need not be ascending.
 *
 * ARITH_SEQ(n,m,adapter)   // expands `adapter(nth)` for each `n` generated; each non-
 *                          //  final result is emitted with a trailing comma.
 *                          // `adapter` may be a macro or simple text.
 *
 * #ifdef CPPARITH_TWICE, redundant defines are defined (ARITH2_INCR et al).
 * used for nested application.
 * 
 */

/***
 * BUGS
 *
 * invalid (numeric) input catastrophic.  unresolveable?
 *
 * seq(0,n) emits nothing, due to sub(n,decr(0))
 *
 * divide/mod by zero emits garbage
 * 0/0 -> 0
 *
 * rep(n[,]) emits n comma-separated nothings (n-1 commas)
 *    remove, or add n'th?
 *
 */


#ifndef CPPARITH_H
 #define CPPARITH_H 1

 #define ARITH_BLOCK
 #define ARITH_EXPAND(...)        __VA_ARGS__
 #define ARITH_DEFER(Fn,...)      ARITH_EXPAND( Fn ARITH_BLOCK ( ARITH_EXPAND(__VA_ARGS__) ))

 #define ARITH_HEAD(list...)      ARITH_HEAD_i(list)
 #define ARITH_HEAD_i(car,cdr...) car
 #define ARITH_TAIL(list...)      ARITH_TAIL_i(list)
 #define ARITH_TAIL_i(car,cdr...) cdr

 #define ARITH_CATN(OP,N)         ARITH_DEFER( ARITH_CATN_i, OP, N )
 #define ARITH_CATN_i(OP,N)       ARITH_ ## OP ## _ ## N




 #define ARITH_INCR(N)            ARITH_CATN( INCR, N )
 #define ARITH_DECR(N)            ARITH_CATN( DECR, N )

 #define ARITH_INCR_256           ARITH_ERR_MAX
 #define ARITH_INCR_ARITH_ERR_MAX ARITH_ERR_MAX
 #define ARITH_DECR_0             ARITH_ERR_MIN
 #define ARITH_DECR_ARITH_ERR_MIN ARITH_ERR_MIN



 #define ARITH_ADD(N,M)           ARITH_HEAD(ARITH_CATN( ADD, M ) (ARITH_ADDfn,,N))
 #define ARITH_ADDfn(...)
 #define ARITH_ADDfn_0(Fn,N)      N

 #define ARITH_ADD_0(Fn1,Fn2,N)   Fn1##_0(Fn2,N)


 #define ARITH_SUB(N,M)           ARITH_HEAD(ARITH_CATN( SUB, M ) (ARITH_SUBfn,,N,))
 #define ARITH_SUBfn(...)
 #define ARITH_SUBfn_0(Fn,N,...)     N

 #define ARITH_SUB_0(Fn1,Fn2,N,REP...) Fn1##_0 (Fn2,N,REP)
 #define ARITH_SUB_ARITH_ERR_MIN(...) ARITH_ERR_MIN


 #define ARITH_REP(N,REP...)      ARITH_TAIL(ARITH_CATN( SUB, N ) (ARITH_REPfn,,N,REP))
 #define ARITH_REPfn(N,_,REP...)    , REP
 #define ARITH_REPfn_0(...)



 #define ARITH_INDEX_GE(N,M,NG,MG) ARITH_CATN( GE, ARITH_SUB(ARITH_SUB(N,M),ARITH_SUB(N,M)) ) (NG,MG)
 #define ARITH_INDEX_LE(N,M,NL,ML) ARITH_CATN( GE, ARITH_SUB(ARITH_SUB(M,N),ARITH_SUB(M,N)) ) (NL,ML)
 #define ARITH_GE_0(N,M)          N
 #define ARITH_GE_ARITH_ERR_MIN(N,M) M


 #define ARITH_MAX(N,M)           ARITH_INDEX_GE(N,M,N,M)
 #define ARITH_MIN(N,M)           ARITH_INDEX_LE(N,M,N,M)


 #define ARITH_DEFAULT(nop...)    nop
 #define ARITH_SEQ(re,expand...)  ARITH_SEQi(re,expand,ARITH_DEFAULT,)
 #define ARITH_SEQi(reexpand...)  ARITH_SEQ_i(reexpand)
 #define ARITH_SEQ_i(N,M,Fn,...)  ARITH_SEQ_ii(ARITH_INDEX_GE(N,M,n,p), Fn, N, M)
 #define ARITH_SEQ_ii(C,Fn,N,M)   ARITH_SEQ_iii(C,Fn,N,M)
 #define ARITH_SEQ_iii(C,Fn,N,M)  ARITH_CATN( SEQ, C ) (ARITH_SEQ##C##fn,Fn,N,M)

 #define ARITH_SEQ_p(C,Fn,N,M)    ARITH_TAIL(ARITH_CATN (SUB, ARITH_SUB(ARITH_INCR(M),N)) (C,Fn,M))
 #define ARITH_SEQ_n(C,Fn,N,M)    ARITH_CATN(ADD, ARITH_SUB(N,M)) (C,Fn,M)

 #define ARITH_SEQpfn(fn,N,...)   , fn(N)
 #define ARITH_SEQpfn_0(...)

 #define ARITH_SEQnfn(fn,N,...)   , fn(N)
 #define ARITH_SEQnfn_0(fn,N)     fn(N)


 #define ARITH_MUL(N,M)           ARITH_CATN( MUL, M ) (N,0)
 #define ARITH_DIV(N,M)           ARITH_CATN( DIV, N ) (ARITH_DIVfn,M,0,0)
 #define ARITH_MOD(N,M)           ARITH_CATN( DIV, N ) (ARITH_MODfn,M,0,0)

 #define ARITH_MUL_0(n,i)         0
 #define ARITH_MUL_1(n,i)         ARITH_ADD(i,n)

 #define ARITH_DIV_0(fn,m,i,p)    fn##_0 (i)
 #define ARITH_DIVfn_0(i)         i
 #define ARITH_MODfn_0(i)         0

 #define ARITH_DIVfn_ERR(i,p)     i
 #define ARITH_MODfn_ERR(i,p)     p
 #define ARITH_DIV_ARITH_ERR_MIN(fn,m,i,p) fn##_ERR (ARITH_DECR(i),p)




 #define ARITH_INCR_0    1
 #define ARITH_INCR_1    2
 #define ARITH_INCR_2    3
 #define ARITH_INCR_3    4
 #define ARITH_INCR_4    5
 #define ARITH_INCR_5    6
 #define ARITH_INCR_6    7
 #define ARITH_INCR_7    8
 #define ARITH_INCR_8    9
 #define ARITH_INCR_9   10
 #define ARITH_INCR_10  11
 #define ARITH_INCR_11  12
 #define ARITH_INCR_12  13
 #define ARITH_INCR_13  14
 #define ARITH_INCR_14  15
 #define ARITH_INCR_15  16
 #define ARITH_INCR_16  17
 #define ARITH_INCR_17  18
 #define ARITH_INCR_18  19
 #define ARITH_INCR_19  20
 #define ARITH_INCR_20  21
 #define ARITH_INCR_21  22
 #define ARITH_INCR_22  23
 #define ARITH_INCR_23  24
 #define ARITH_INCR_24  25
 #define ARITH_INCR_25  26
 #define ARITH_INCR_26  27
 #define ARITH_INCR_27  28
 #define ARITH_INCR_28  29
 #define ARITH_INCR_29  30
 #define ARITH_INCR_30  31
 #define ARITH_INCR_31  32
 #define ARITH_INCR_32  33
 #define ARITH_INCR_33  34
 #define ARITH_INCR_34  35
 #define ARITH_INCR_35  36
 #define ARITH_INCR_36  37
 #define ARITH_INCR_37  38
 #define ARITH_INCR_38  39
 #define ARITH_INCR_39  40
 #define ARITH_INCR_40  41
 #define ARITH_INCR_41  42
 #define ARITH_INCR_42  43
 #define ARITH_INCR_43  44
 #define ARITH_INCR_44  45
 #define ARITH_INCR_45  46
 #define ARITH_INCR_46  47
 #define ARITH_INCR_47  48
 #define ARITH_INCR_48  49
 #define ARITH_INCR_49  50
 #define ARITH_INCR_50  51
 #define ARITH_INCR_51  52
 #define ARITH_INCR_52  53
 #define ARITH_INCR_53  54
 #define ARITH_INCR_54  55
 #define ARITH_INCR_55  56
 #define ARITH_INCR_56  57
 #define ARITH_INCR_57  58
 #define ARITH_INCR_58  59
 #define ARITH_INCR_59  60
 #define ARITH_INCR_60  61
 #define ARITH_INCR_61  62
 #define ARITH_INCR_62  63
 #define ARITH_INCR_63  64
 #define ARITH_INCR_64  65
 #define ARITH_INCR_65  66
 #define ARITH_INCR_66  67
 #define ARITH_INCR_67  68
 #define ARITH_INCR_68  69
 #define ARITH_INCR_69  70
 #define ARITH_INCR_70  71
 #define ARITH_INCR_71  72
 #define ARITH_INCR_72  73
 #define ARITH_INCR_73  74
 #define ARITH_INCR_74  75
 #define ARITH_INCR_75  76
 #define ARITH_INCR_76  77
 #define ARITH_INCR_77  78
 #define ARITH_INCR_78  79
 #define ARITH_INCR_79  80
 #define ARITH_INCR_80  81
 #define ARITH_INCR_81  82
 #define ARITH_INCR_82  83
 #define ARITH_INCR_83  84
 #define ARITH_INCR_84  85
 #define ARITH_INCR_85  86
 #define ARITH_INCR_86  87
 #define ARITH_INCR_87  88
 #define ARITH_INCR_88  89
 #define ARITH_INCR_89  90
 #define ARITH_INCR_90  91
 #define ARITH_INCR_91  92
 #define ARITH_INCR_92  93
 #define ARITH_INCR_93  94
 #define ARITH_INCR_94  95
 #define ARITH_INCR_95  96
 #define ARITH_INCR_96  97
 #define ARITH_INCR_97  98
 #define ARITH_INCR_98  99
 #define ARITH_INCR_99  100
 #define ARITH_INCR_100  101
 #define ARITH_INCR_101  102
 #define ARITH_INCR_102  103
 #define ARITH_INCR_103  104
 #define ARITH_INCR_104  105
 #define ARITH_INCR_105  106
 #define ARITH_INCR_106  107
 #define ARITH_INCR_107  108
 #define ARITH_INCR_108  109
 #define ARITH_INCR_109  110
 #define ARITH_INCR_110  111
 #define ARITH_INCR_111  112
 #define ARITH_INCR_112  113
 #define ARITH_INCR_113  114
 #define ARITH_INCR_114  115
 #define ARITH_INCR_115  116
 #define ARITH_INCR_116  117
 #define ARITH_INCR_117  118
 #define ARITH_INCR_118  119
 #define ARITH_INCR_119  120
 #define ARITH_INCR_120  121
 #define ARITH_INCR_121  122
 #define ARITH_INCR_122  123
 #define ARITH_INCR_123  124
 #define ARITH_INCR_124  125
 #define ARITH_INCR_125  126
 #define ARITH_INCR_126  127
 #define ARITH_INCR_127  128
 #define ARITH_INCR_128  129
 #define ARITH_INCR_129  130
 #define ARITH_INCR_130  131
 #define ARITH_INCR_131  132
 #define ARITH_INCR_132  133
 #define ARITH_INCR_133  134
 #define ARITH_INCR_134  135
 #define ARITH_INCR_135  136
 #define ARITH_INCR_136  137
 #define ARITH_INCR_137  138
 #define ARITH_INCR_138  139
 #define ARITH_INCR_139  140
 #define ARITH_INCR_140  141
 #define ARITH_INCR_141  142
 #define ARITH_INCR_142  143
 #define ARITH_INCR_143  144
 #define ARITH_INCR_144  145
 #define ARITH_INCR_145  146
 #define ARITH_INCR_146  147
 #define ARITH_INCR_147  148
 #define ARITH_INCR_148  149
 #define ARITH_INCR_149  150
 #define ARITH_INCR_150  151
 #define ARITH_INCR_151  152
 #define ARITH_INCR_152  153
 #define ARITH_INCR_153  154
 #define ARITH_INCR_154  155
 #define ARITH_INCR_155  156
 #define ARITH_INCR_156  157
 #define ARITH_INCR_157  158
 #define ARITH_INCR_158  159
 #define ARITH_INCR_159  160
 #define ARITH_INCR_160  161
 #define ARITH_INCR_161  162
 #define ARITH_INCR_162  163
 #define ARITH_INCR_163  164
 #define ARITH_INCR_164  165
 #define ARITH_INCR_165  166
 #define ARITH_INCR_166  167
 #define ARITH_INCR_167  168
 #define ARITH_INCR_168  169
 #define ARITH_INCR_169  170
 #define ARITH_INCR_170  171
 #define ARITH_INCR_171  172
 #define ARITH_INCR_172  173
 #define ARITH_INCR_173  174
 #define ARITH_INCR_174  175
 #define ARITH_INCR_175  176
 #define ARITH_INCR_176  177
 #define ARITH_INCR_177  178
 #define ARITH_INCR_178  179
 #define ARITH_INCR_179  180
 #define ARITH_INCR_180  181
 #define ARITH_INCR_181  182
 #define ARITH_INCR_182  183
 #define ARITH_INCR_183  184
 #define ARITH_INCR_184  185
 #define ARITH_INCR_185  186
 #define ARITH_INCR_186  187
 #define ARITH_INCR_187  188
 #define ARITH_INCR_188  189
 #define ARITH_INCR_189  190
 #define ARITH_INCR_190  191
 #define ARITH_INCR_191  192
 #define ARITH_INCR_192  193
 #define ARITH_INCR_193  194
 #define ARITH_INCR_194  195
 #define ARITH_INCR_195  196
 #define ARITH_INCR_196  197
 #define ARITH_INCR_197  198
 #define ARITH_INCR_198  199
 #define ARITH_INCR_199  200
 #define ARITH_INCR_200  201
 #define ARITH_INCR_201  202
 #define ARITH_INCR_202  203
 #define ARITH_INCR_203  204
 #define ARITH_INCR_204  205
 #define ARITH_INCR_205  206
 #define ARITH_INCR_206  207
 #define ARITH_INCR_207  208
 #define ARITH_INCR_208  209
 #define ARITH_INCR_209  210
 #define ARITH_INCR_210  211
 #define ARITH_INCR_211  212
 #define ARITH_INCR_212  213
 #define ARITH_INCR_213  214
 #define ARITH_INCR_214  215
 #define ARITH_INCR_215  216
 #define ARITH_INCR_216  217
 #define ARITH_INCR_217  218
 #define ARITH_INCR_218  219
 #define ARITH_INCR_219  220
 #define ARITH_INCR_220  221
 #define ARITH_INCR_221  222
 #define ARITH_INCR_222  223
 #define ARITH_INCR_223  224
 #define ARITH_INCR_224  225
 #define ARITH_INCR_225  226
 #define ARITH_INCR_226  227
 #define ARITH_INCR_227  228
 #define ARITH_INCR_228  229
 #define ARITH_INCR_229  230
 #define ARITH_INCR_230  231
 #define ARITH_INCR_231  232
 #define ARITH_INCR_232  233
 #define ARITH_INCR_233  234
 #define ARITH_INCR_234  235
 #define ARITH_INCR_235  236
 #define ARITH_INCR_236  237
 #define ARITH_INCR_237  238
 #define ARITH_INCR_238  239
 #define ARITH_INCR_239  240
 #define ARITH_INCR_240  241
 #define ARITH_INCR_241  242
 #define ARITH_INCR_242  243
 #define ARITH_INCR_243  244
 #define ARITH_INCR_244  245
 #define ARITH_INCR_245  246
 #define ARITH_INCR_246  247
 #define ARITH_INCR_247  248
 #define ARITH_INCR_248  249
 #define ARITH_INCR_249  250
 #define ARITH_INCR_250  251
 #define ARITH_INCR_251  252
 #define ARITH_INCR_252  253
 #define ARITH_INCR_253  254
 #define ARITH_INCR_254  255
 #define ARITH_INCR_255  256

 #define ARITH_DECR_1    0
 #define ARITH_DECR_2    1
 #define ARITH_DECR_3    2
 #define ARITH_DECR_4    3
 #define ARITH_DECR_5    4
 #define ARITH_DECR_6    5
 #define ARITH_DECR_7    6
 #define ARITH_DECR_8    7
 #define ARITH_DECR_9    8
 #define ARITH_DECR_10   9
 #define ARITH_DECR_11  10
 #define ARITH_DECR_12  11
 #define ARITH_DECR_13  12
 #define ARITH_DECR_14  13
 #define ARITH_DECR_15  14
 #define ARITH_DECR_16  15
 #define ARITH_DECR_17  16
 #define ARITH_DECR_18  17
 #define ARITH_DECR_19  18
 #define ARITH_DECR_20  19
 #define ARITH_DECR_21  20
 #define ARITH_DECR_22  21
 #define ARITH_DECR_23  22
 #define ARITH_DECR_24  23
 #define ARITH_DECR_25  24
 #define ARITH_DECR_26  25
 #define ARITH_DECR_27  26
 #define ARITH_DECR_28  27
 #define ARITH_DECR_29  28
 #define ARITH_DECR_30  29
 #define ARITH_DECR_31  30
 #define ARITH_DECR_32  31
 #define ARITH_DECR_33  32
 #define ARITH_DECR_34  33
 #define ARITH_DECR_35  34
 #define ARITH_DECR_36  35
 #define ARITH_DECR_37  36
 #define ARITH_DECR_38  37
 #define ARITH_DECR_39  38
 #define ARITH_DECR_40  39
 #define ARITH_DECR_41  40
 #define ARITH_DECR_42  41
 #define ARITH_DECR_43  42
 #define ARITH_DECR_44  43
 #define ARITH_DECR_45  44
 #define ARITH_DECR_46  45
 #define ARITH_DECR_47  46
 #define ARITH_DECR_48  47
 #define ARITH_DECR_49  48
 #define ARITH_DECR_50  49
 #define ARITH_DECR_51  50
 #define ARITH_DECR_52  51
 #define ARITH_DECR_53  52
 #define ARITH_DECR_54  53
 #define ARITH_DECR_55  54
 #define ARITH_DECR_56  55
 #define ARITH_DECR_57  56
 #define ARITH_DECR_58  57
 #define ARITH_DECR_59  58
 #define ARITH_DECR_60  59
 #define ARITH_DECR_61  60
 #define ARITH_DECR_62  61
 #define ARITH_DECR_63  62
 #define ARITH_DECR_64  63
 #define ARITH_DECR_65  64
 #define ARITH_DECR_66  65
 #define ARITH_DECR_67  66
 #define ARITH_DECR_68  67
 #define ARITH_DECR_69  68
 #define ARITH_DECR_70  69
 #define ARITH_DECR_71  70
 #define ARITH_DECR_72  71
 #define ARITH_DECR_73  72
 #define ARITH_DECR_74  73
 #define ARITH_DECR_75  74
 #define ARITH_DECR_76  75
 #define ARITH_DECR_77  76
 #define ARITH_DECR_78  77
 #define ARITH_DECR_79  78
 #define ARITH_DECR_80  79
 #define ARITH_DECR_81  80
 #define ARITH_DECR_82  81
 #define ARITH_DECR_83  82
 #define ARITH_DECR_84  83
 #define ARITH_DECR_85  84
 #define ARITH_DECR_86  85
 #define ARITH_DECR_87  86
 #define ARITH_DECR_88  87
 #define ARITH_DECR_89  88
 #define ARITH_DECR_90  89
 #define ARITH_DECR_91  90
 #define ARITH_DECR_92  91
 #define ARITH_DECR_93  92
 #define ARITH_DECR_94  93
 #define ARITH_DECR_95  94
 #define ARITH_DECR_96  95
 #define ARITH_DECR_97  96
 #define ARITH_DECR_98  97
 #define ARITH_DECR_99  98
 #define ARITH_DECR_100  99
 #define ARITH_DECR_101  100
 #define ARITH_DECR_102  101
 #define ARITH_DECR_103  102
 #define ARITH_DECR_104  103
 #define ARITH_DECR_105  104
 #define ARITH_DECR_106  105
 #define ARITH_DECR_107  106
 #define ARITH_DECR_108  107
 #define ARITH_DECR_109  108
 #define ARITH_DECR_110  109
 #define ARITH_DECR_111  110
 #define ARITH_DECR_112  111
 #define ARITH_DECR_113  112
 #define ARITH_DECR_114  113
 #define ARITH_DECR_115  114
 #define ARITH_DECR_116  115
 #define ARITH_DECR_117  116
 #define ARITH_DECR_118  117
 #define ARITH_DECR_119  118
 #define ARITH_DECR_120  119
 #define ARITH_DECR_121  120
 #define ARITH_DECR_122  121
 #define ARITH_DECR_123  122
 #define ARITH_DECR_124  123
 #define ARITH_DECR_125  124
 #define ARITH_DECR_126  125
 #define ARITH_DECR_127  126
 #define ARITH_DECR_128  127
 #define ARITH_DECR_129  128
 #define ARITH_DECR_130  129
 #define ARITH_DECR_131  130
 #define ARITH_DECR_132  131
 #define ARITH_DECR_133  132
 #define ARITH_DECR_134  133
 #define ARITH_DECR_135  134
 #define ARITH_DECR_136  135
 #define ARITH_DECR_137  136
 #define ARITH_DECR_138  137
 #define ARITH_DECR_139  138
 #define ARITH_DECR_140  139
 #define ARITH_DECR_141  140
 #define ARITH_DECR_142  141
 #define ARITH_DECR_143  142
 #define ARITH_DECR_144  143
 #define ARITH_DECR_145  144
 #define ARITH_DECR_146  145
 #define ARITH_DECR_147  146
 #define ARITH_DECR_148  147
 #define ARITH_DECR_149  148
 #define ARITH_DECR_150  149
 #define ARITH_DECR_151  150
 #define ARITH_DECR_152  151
 #define ARITH_DECR_153  152
 #define ARITH_DECR_154  153
 #define ARITH_DECR_155  154
 #define ARITH_DECR_156  155
 #define ARITH_DECR_157  156
 #define ARITH_DECR_158  157
 #define ARITH_DECR_159  158
 #define ARITH_DECR_160  159
 #define ARITH_DECR_161  160
 #define ARITH_DECR_162  161
 #define ARITH_DECR_163  162
 #define ARITH_DECR_164  163
 #define ARITH_DECR_165  164
 #define ARITH_DECR_166  165
 #define ARITH_DECR_167  166
 #define ARITH_DECR_168  167
 #define ARITH_DECR_169  168
 #define ARITH_DECR_170  169
 #define ARITH_DECR_171  170
 #define ARITH_DECR_172  171
 #define ARITH_DECR_173  172
 #define ARITH_DECR_174  173
 #define ARITH_DECR_175  174
 #define ARITH_DECR_176  175
 #define ARITH_DECR_177  176
 #define ARITH_DECR_178  177
 #define ARITH_DECR_179  178
 #define ARITH_DECR_180  179
 #define ARITH_DECR_181  180
 #define ARITH_DECR_182  181
 #define ARITH_DECR_183  182
 #define ARITH_DECR_184  183
 #define ARITH_DECR_185  184
 #define ARITH_DECR_186  185
 #define ARITH_DECR_187  186
 #define ARITH_DECR_188  187
 #define ARITH_DECR_189  188
 #define ARITH_DECR_190  189
 #define ARITH_DECR_191  190
 #define ARITH_DECR_192  191
 #define ARITH_DECR_193  192
 #define ARITH_DECR_194  193
 #define ARITH_DECR_195  194
 #define ARITH_DECR_196  195
 #define ARITH_DECR_197  196
 #define ARITH_DECR_198  197
 #define ARITH_DECR_199  198
 #define ARITH_DECR_200  199
 #define ARITH_DECR_201  200
 #define ARITH_DECR_202  201
 #define ARITH_DECR_203  202
 #define ARITH_DECR_204  203
 #define ARITH_DECR_205  204
 #define ARITH_DECR_206  205
 #define ARITH_DECR_207  206
 #define ARITH_DECR_208  207
 #define ARITH_DECR_209  208
 #define ARITH_DECR_210  209
 #define ARITH_DECR_211  210
 #define ARITH_DECR_212  211
 #define ARITH_DECR_213  212
 #define ARITH_DECR_214  213
 #define ARITH_DECR_215  214
 #define ARITH_DECR_216  215
 #define ARITH_DECR_217  216
 #define ARITH_DECR_218  217
 #define ARITH_DECR_219  218
 #define ARITH_DECR_220  219
 #define ARITH_DECR_221  220
 #define ARITH_DECR_222  221
 #define ARITH_DECR_223  222
 #define ARITH_DECR_224  223
 #define ARITH_DECR_225  224
 #define ARITH_DECR_226  225
 #define ARITH_DECR_227  226
 #define ARITH_DECR_228  227
 #define ARITH_DECR_229  228
 #define ARITH_DECR_230  229
 #define ARITH_DECR_231  230
 #define ARITH_DECR_232  231
 #define ARITH_DECR_233  232
 #define ARITH_DECR_234  233
 #define ARITH_DECR_235  234
 #define ARITH_DECR_236  235
 #define ARITH_DECR_237  236
 #define ARITH_DECR_238  237
 #define ARITH_DECR_239  238
 #define ARITH_DECR_240  239
 #define ARITH_DECR_241  240
 #define ARITH_DECR_242  241
 #define ARITH_DECR_243  242
 #define ARITH_DECR_244  243
 #define ARITH_DECR_245  244
 #define ARITH_DECR_246  245
 #define ARITH_DECR_247  246
 #define ARITH_DECR_248  247
 #define ARITH_DECR_249  248
 #define ARITH_DECR_250  249
 #define ARITH_DECR_251  250
 #define ARITH_DECR_252  251
 #define ARITH_DECR_253  252
 #define ARITH_DECR_254  253
 #define ARITH_DECR_255  254
 #define ARITH_DECR_256  255


 #define ARITH_ADD_1(fn1,fn2,n) ARITH_ADD_0(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_2(fn1,fn2,n) ARITH_ADD_1(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_3(fn1,fn2,n) ARITH_ADD_2(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_4(fn1,fn2,n) ARITH_ADD_3(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_5(fn1,fn2,n) ARITH_ADD_4(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_6(fn1,fn2,n) ARITH_ADD_5(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_7(fn1,fn2,n) ARITH_ADD_6(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_8(fn1,fn2,n) ARITH_ADD_7(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_9(fn1,fn2,n) ARITH_ADD_8(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_10(fn1,fn2,n) ARITH_ADD_9(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_11(fn1,fn2,n) ARITH_ADD_10(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_12(fn1,fn2,n) ARITH_ADD_11(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_13(fn1,fn2,n) ARITH_ADD_12(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_14(fn1,fn2,n) ARITH_ADD_13(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_15(fn1,fn2,n) ARITH_ADD_14(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_16(fn1,fn2,n) ARITH_ADD_15(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_17(fn1,fn2,n) ARITH_ADD_16(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_18(fn1,fn2,n) ARITH_ADD_17(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_19(fn1,fn2,n) ARITH_ADD_18(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_20(fn1,fn2,n) ARITH_ADD_19(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_21(fn1,fn2,n) ARITH_ADD_20(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_22(fn1,fn2,n) ARITH_ADD_21(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_23(fn1,fn2,n) ARITH_ADD_22(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_24(fn1,fn2,n) ARITH_ADD_23(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_25(fn1,fn2,n) ARITH_ADD_24(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_26(fn1,fn2,n) ARITH_ADD_25(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_27(fn1,fn2,n) ARITH_ADD_26(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_28(fn1,fn2,n) ARITH_ADD_27(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_29(fn1,fn2,n) ARITH_ADD_28(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_30(fn1,fn2,n) ARITH_ADD_29(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_31(fn1,fn2,n) ARITH_ADD_30(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_32(fn1,fn2,n) ARITH_ADD_31(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_33(fn1,fn2,n) ARITH_ADD_32(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_34(fn1,fn2,n) ARITH_ADD_33(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_35(fn1,fn2,n) ARITH_ADD_34(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_36(fn1,fn2,n) ARITH_ADD_35(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_37(fn1,fn2,n) ARITH_ADD_36(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_38(fn1,fn2,n) ARITH_ADD_37(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_39(fn1,fn2,n) ARITH_ADD_38(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_40(fn1,fn2,n) ARITH_ADD_39(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_41(fn1,fn2,n) ARITH_ADD_40(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_42(fn1,fn2,n) ARITH_ADD_41(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_43(fn1,fn2,n) ARITH_ADD_42(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_44(fn1,fn2,n) ARITH_ADD_43(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_45(fn1,fn2,n) ARITH_ADD_44(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_46(fn1,fn2,n) ARITH_ADD_45(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_47(fn1,fn2,n) ARITH_ADD_46(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_48(fn1,fn2,n) ARITH_ADD_47(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_49(fn1,fn2,n) ARITH_ADD_48(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_50(fn1,fn2,n) ARITH_ADD_49(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_51(fn1,fn2,n) ARITH_ADD_50(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_52(fn1,fn2,n) ARITH_ADD_51(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_53(fn1,fn2,n) ARITH_ADD_52(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_54(fn1,fn2,n) ARITH_ADD_53(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_55(fn1,fn2,n) ARITH_ADD_54(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_56(fn1,fn2,n) ARITH_ADD_55(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_57(fn1,fn2,n) ARITH_ADD_56(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_58(fn1,fn2,n) ARITH_ADD_57(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_59(fn1,fn2,n) ARITH_ADD_58(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_60(fn1,fn2,n) ARITH_ADD_59(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_61(fn1,fn2,n) ARITH_ADD_60(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_62(fn1,fn2,n) ARITH_ADD_61(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_63(fn1,fn2,n) ARITH_ADD_62(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_64(fn1,fn2,n) ARITH_ADD_63(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_65(fn1,fn2,n) ARITH_ADD_64(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_66(fn1,fn2,n) ARITH_ADD_65(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_67(fn1,fn2,n) ARITH_ADD_66(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_68(fn1,fn2,n) ARITH_ADD_67(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_69(fn1,fn2,n) ARITH_ADD_68(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_70(fn1,fn2,n) ARITH_ADD_69(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_71(fn1,fn2,n) ARITH_ADD_70(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_72(fn1,fn2,n) ARITH_ADD_71(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_73(fn1,fn2,n) ARITH_ADD_72(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_74(fn1,fn2,n) ARITH_ADD_73(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_75(fn1,fn2,n) ARITH_ADD_74(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_76(fn1,fn2,n) ARITH_ADD_75(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_77(fn1,fn2,n) ARITH_ADD_76(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_78(fn1,fn2,n) ARITH_ADD_77(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_79(fn1,fn2,n) ARITH_ADD_78(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_80(fn1,fn2,n) ARITH_ADD_79(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_81(fn1,fn2,n) ARITH_ADD_80(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_82(fn1,fn2,n) ARITH_ADD_81(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_83(fn1,fn2,n) ARITH_ADD_82(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_84(fn1,fn2,n) ARITH_ADD_83(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_85(fn1,fn2,n) ARITH_ADD_84(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_86(fn1,fn2,n) ARITH_ADD_85(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_87(fn1,fn2,n) ARITH_ADD_86(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_88(fn1,fn2,n) ARITH_ADD_87(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_89(fn1,fn2,n) ARITH_ADD_88(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_90(fn1,fn2,n) ARITH_ADD_89(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_91(fn1,fn2,n) ARITH_ADD_90(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_92(fn1,fn2,n) ARITH_ADD_91(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_93(fn1,fn2,n) ARITH_ADD_92(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_94(fn1,fn2,n) ARITH_ADD_93(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_95(fn1,fn2,n) ARITH_ADD_94(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_96(fn1,fn2,n) ARITH_ADD_95(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_97(fn1,fn2,n) ARITH_ADD_96(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_98(fn1,fn2,n) ARITH_ADD_97(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_99(fn1,fn2,n) ARITH_ADD_98(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_100(fn1,fn2,n) ARITH_ADD_99(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_101(fn1,fn2,n) ARITH_ADD_100(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_102(fn1,fn2,n) ARITH_ADD_101(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_103(fn1,fn2,n) ARITH_ADD_102(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_104(fn1,fn2,n) ARITH_ADD_103(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_105(fn1,fn2,n) ARITH_ADD_104(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_106(fn1,fn2,n) ARITH_ADD_105(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_107(fn1,fn2,n) ARITH_ADD_106(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_108(fn1,fn2,n) ARITH_ADD_107(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_109(fn1,fn2,n) ARITH_ADD_108(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_110(fn1,fn2,n) ARITH_ADD_109(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_111(fn1,fn2,n) ARITH_ADD_110(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_112(fn1,fn2,n) ARITH_ADD_111(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_113(fn1,fn2,n) ARITH_ADD_112(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_114(fn1,fn2,n) ARITH_ADD_113(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_115(fn1,fn2,n) ARITH_ADD_114(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_116(fn1,fn2,n) ARITH_ADD_115(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_117(fn1,fn2,n) ARITH_ADD_116(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_118(fn1,fn2,n) ARITH_ADD_117(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_119(fn1,fn2,n) ARITH_ADD_118(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_120(fn1,fn2,n) ARITH_ADD_119(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_121(fn1,fn2,n) ARITH_ADD_120(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_122(fn1,fn2,n) ARITH_ADD_121(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_123(fn1,fn2,n) ARITH_ADD_122(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_124(fn1,fn2,n) ARITH_ADD_123(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_125(fn1,fn2,n) ARITH_ADD_124(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_126(fn1,fn2,n) ARITH_ADD_125(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_127(fn1,fn2,n) ARITH_ADD_126(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_128(fn1,fn2,n) ARITH_ADD_127(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_129(fn1,fn2,n) ARITH_ADD_128(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_130(fn1,fn2,n) ARITH_ADD_129(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_131(fn1,fn2,n) ARITH_ADD_130(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_132(fn1,fn2,n) ARITH_ADD_131(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_133(fn1,fn2,n) ARITH_ADD_132(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_134(fn1,fn2,n) ARITH_ADD_133(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_135(fn1,fn2,n) ARITH_ADD_134(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_136(fn1,fn2,n) ARITH_ADD_135(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_137(fn1,fn2,n) ARITH_ADD_136(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_138(fn1,fn2,n) ARITH_ADD_137(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_139(fn1,fn2,n) ARITH_ADD_138(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_140(fn1,fn2,n) ARITH_ADD_139(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_141(fn1,fn2,n) ARITH_ADD_140(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_142(fn1,fn2,n) ARITH_ADD_141(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_143(fn1,fn2,n) ARITH_ADD_142(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_144(fn1,fn2,n) ARITH_ADD_143(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_145(fn1,fn2,n) ARITH_ADD_144(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_146(fn1,fn2,n) ARITH_ADD_145(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_147(fn1,fn2,n) ARITH_ADD_146(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_148(fn1,fn2,n) ARITH_ADD_147(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_149(fn1,fn2,n) ARITH_ADD_148(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_150(fn1,fn2,n) ARITH_ADD_149(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_151(fn1,fn2,n) ARITH_ADD_150(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_152(fn1,fn2,n) ARITH_ADD_151(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_153(fn1,fn2,n) ARITH_ADD_152(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_154(fn1,fn2,n) ARITH_ADD_153(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_155(fn1,fn2,n) ARITH_ADD_154(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_156(fn1,fn2,n) ARITH_ADD_155(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_157(fn1,fn2,n) ARITH_ADD_156(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_158(fn1,fn2,n) ARITH_ADD_157(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_159(fn1,fn2,n) ARITH_ADD_158(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_160(fn1,fn2,n) ARITH_ADD_159(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_161(fn1,fn2,n) ARITH_ADD_160(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_162(fn1,fn2,n) ARITH_ADD_161(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_163(fn1,fn2,n) ARITH_ADD_162(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_164(fn1,fn2,n) ARITH_ADD_163(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_165(fn1,fn2,n) ARITH_ADD_164(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_166(fn1,fn2,n) ARITH_ADD_165(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_167(fn1,fn2,n) ARITH_ADD_166(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_168(fn1,fn2,n) ARITH_ADD_167(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_169(fn1,fn2,n) ARITH_ADD_168(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_170(fn1,fn2,n) ARITH_ADD_169(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_171(fn1,fn2,n) ARITH_ADD_170(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_172(fn1,fn2,n) ARITH_ADD_171(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_173(fn1,fn2,n) ARITH_ADD_172(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_174(fn1,fn2,n) ARITH_ADD_173(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_175(fn1,fn2,n) ARITH_ADD_174(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_176(fn1,fn2,n) ARITH_ADD_175(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_177(fn1,fn2,n) ARITH_ADD_176(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_178(fn1,fn2,n) ARITH_ADD_177(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_179(fn1,fn2,n) ARITH_ADD_178(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_180(fn1,fn2,n) ARITH_ADD_179(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_181(fn1,fn2,n) ARITH_ADD_180(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_182(fn1,fn2,n) ARITH_ADD_181(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_183(fn1,fn2,n) ARITH_ADD_182(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_184(fn1,fn2,n) ARITH_ADD_183(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_185(fn1,fn2,n) ARITH_ADD_184(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_186(fn1,fn2,n) ARITH_ADD_185(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_187(fn1,fn2,n) ARITH_ADD_186(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_188(fn1,fn2,n) ARITH_ADD_187(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_189(fn1,fn2,n) ARITH_ADD_188(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_190(fn1,fn2,n) ARITH_ADD_189(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_191(fn1,fn2,n) ARITH_ADD_190(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_192(fn1,fn2,n) ARITH_ADD_191(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_193(fn1,fn2,n) ARITH_ADD_192(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_194(fn1,fn2,n) ARITH_ADD_193(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_195(fn1,fn2,n) ARITH_ADD_194(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_196(fn1,fn2,n) ARITH_ADD_195(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_197(fn1,fn2,n) ARITH_ADD_196(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_198(fn1,fn2,n) ARITH_ADD_197(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_199(fn1,fn2,n) ARITH_ADD_198(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_200(fn1,fn2,n) ARITH_ADD_199(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_201(fn1,fn2,n) ARITH_ADD_200(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_202(fn1,fn2,n) ARITH_ADD_201(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_203(fn1,fn2,n) ARITH_ADD_202(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_204(fn1,fn2,n) ARITH_ADD_203(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_205(fn1,fn2,n) ARITH_ADD_204(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_206(fn1,fn2,n) ARITH_ADD_205(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_207(fn1,fn2,n) ARITH_ADD_206(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_208(fn1,fn2,n) ARITH_ADD_207(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_209(fn1,fn2,n) ARITH_ADD_208(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_210(fn1,fn2,n) ARITH_ADD_209(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_211(fn1,fn2,n) ARITH_ADD_210(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_212(fn1,fn2,n) ARITH_ADD_211(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_213(fn1,fn2,n) ARITH_ADD_212(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_214(fn1,fn2,n) ARITH_ADD_213(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_215(fn1,fn2,n) ARITH_ADD_214(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_216(fn1,fn2,n) ARITH_ADD_215(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_217(fn1,fn2,n) ARITH_ADD_216(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_218(fn1,fn2,n) ARITH_ADD_217(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_219(fn1,fn2,n) ARITH_ADD_218(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_220(fn1,fn2,n) ARITH_ADD_219(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_221(fn1,fn2,n) ARITH_ADD_220(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_222(fn1,fn2,n) ARITH_ADD_221(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_223(fn1,fn2,n) ARITH_ADD_222(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_224(fn1,fn2,n) ARITH_ADD_223(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_225(fn1,fn2,n) ARITH_ADD_224(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_226(fn1,fn2,n) ARITH_ADD_225(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_227(fn1,fn2,n) ARITH_ADD_226(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_228(fn1,fn2,n) ARITH_ADD_227(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_229(fn1,fn2,n) ARITH_ADD_228(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_230(fn1,fn2,n) ARITH_ADD_229(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_231(fn1,fn2,n) ARITH_ADD_230(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_232(fn1,fn2,n) ARITH_ADD_231(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_233(fn1,fn2,n) ARITH_ADD_232(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_234(fn1,fn2,n) ARITH_ADD_233(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_235(fn1,fn2,n) ARITH_ADD_234(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_236(fn1,fn2,n) ARITH_ADD_235(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_237(fn1,fn2,n) ARITH_ADD_236(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_238(fn1,fn2,n) ARITH_ADD_237(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_239(fn1,fn2,n) ARITH_ADD_238(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_240(fn1,fn2,n) ARITH_ADD_239(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_241(fn1,fn2,n) ARITH_ADD_240(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_242(fn1,fn2,n) ARITH_ADD_241(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_243(fn1,fn2,n) ARITH_ADD_242(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_244(fn1,fn2,n) ARITH_ADD_243(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_245(fn1,fn2,n) ARITH_ADD_244(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_246(fn1,fn2,n) ARITH_ADD_245(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_247(fn1,fn2,n) ARITH_ADD_246(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_248(fn1,fn2,n) ARITH_ADD_247(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_249(fn1,fn2,n) ARITH_ADD_248(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_250(fn1,fn2,n) ARITH_ADD_249(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_251(fn1,fn2,n) ARITH_ADD_250(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_252(fn1,fn2,n) ARITH_ADD_251(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_253(fn1,fn2,n) ARITH_ADD_252(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_254(fn1,fn2,n) ARITH_ADD_253(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_255(fn1,fn2,n) ARITH_ADD_254(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)
 #define ARITH_ADD_256(fn1,fn2,n) ARITH_ADD_255(fn1,fn2,ARITH_INCR(n)) fn1(fn2,n,)

 #define ARITH_SUB_1(fn1,fn2,n,rep...) ARITH_SUB_0(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_2(fn1,fn2,n,rep...) ARITH_SUB_1(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_3(fn1,fn2,n,rep...) ARITH_SUB_2(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_4(fn1,fn2,n,rep...) ARITH_SUB_3(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_5(fn1,fn2,n,rep...) ARITH_SUB_4(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_6(fn1,fn2,n,rep...) ARITH_SUB_5(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_7(fn1,fn2,n,rep...) ARITH_SUB_6(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_8(fn1,fn2,n,rep...) ARITH_SUB_7(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_9(fn1,fn2,n,rep...) ARITH_SUB_8(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_10(fn1,fn2,n,rep...) ARITH_SUB_9(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_11(fn1,fn2,n,rep...) ARITH_SUB_10(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_12(fn1,fn2,n,rep...) ARITH_SUB_11(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_13(fn1,fn2,n,rep...) ARITH_SUB_12(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_14(fn1,fn2,n,rep...) ARITH_SUB_13(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_15(fn1,fn2,n,rep...) ARITH_SUB_14(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_16(fn1,fn2,n,rep...) ARITH_SUB_15(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_17(fn1,fn2,n,rep...) ARITH_SUB_16(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_18(fn1,fn2,n,rep...) ARITH_SUB_17(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_19(fn1,fn2,n,rep...) ARITH_SUB_18(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_20(fn1,fn2,n,rep...) ARITH_SUB_19(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_21(fn1,fn2,n,rep...) ARITH_SUB_20(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_22(fn1,fn2,n,rep...) ARITH_SUB_21(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_23(fn1,fn2,n,rep...) ARITH_SUB_22(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_24(fn1,fn2,n,rep...) ARITH_SUB_23(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_25(fn1,fn2,n,rep...) ARITH_SUB_24(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_26(fn1,fn2,n,rep...) ARITH_SUB_25(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_27(fn1,fn2,n,rep...) ARITH_SUB_26(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_28(fn1,fn2,n,rep...) ARITH_SUB_27(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_29(fn1,fn2,n,rep...) ARITH_SUB_28(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_30(fn1,fn2,n,rep...) ARITH_SUB_29(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_31(fn1,fn2,n,rep...) ARITH_SUB_30(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_32(fn1,fn2,n,rep...) ARITH_SUB_31(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_33(fn1,fn2,n,rep...) ARITH_SUB_32(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_34(fn1,fn2,n,rep...) ARITH_SUB_33(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_35(fn1,fn2,n,rep...) ARITH_SUB_34(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_36(fn1,fn2,n,rep...) ARITH_SUB_35(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_37(fn1,fn2,n,rep...) ARITH_SUB_36(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_38(fn1,fn2,n,rep...) ARITH_SUB_37(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_39(fn1,fn2,n,rep...) ARITH_SUB_38(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_40(fn1,fn2,n,rep...) ARITH_SUB_39(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_41(fn1,fn2,n,rep...) ARITH_SUB_40(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_42(fn1,fn2,n,rep...) ARITH_SUB_41(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_43(fn1,fn2,n,rep...) ARITH_SUB_42(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_44(fn1,fn2,n,rep...) ARITH_SUB_43(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_45(fn1,fn2,n,rep...) ARITH_SUB_44(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_46(fn1,fn2,n,rep...) ARITH_SUB_45(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_47(fn1,fn2,n,rep...) ARITH_SUB_46(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_48(fn1,fn2,n,rep...) ARITH_SUB_47(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_49(fn1,fn2,n,rep...) ARITH_SUB_48(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_50(fn1,fn2,n,rep...) ARITH_SUB_49(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_51(fn1,fn2,n,rep...) ARITH_SUB_50(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_52(fn1,fn2,n,rep...) ARITH_SUB_51(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_53(fn1,fn2,n,rep...) ARITH_SUB_52(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_54(fn1,fn2,n,rep...) ARITH_SUB_53(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_55(fn1,fn2,n,rep...) ARITH_SUB_54(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_56(fn1,fn2,n,rep...) ARITH_SUB_55(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_57(fn1,fn2,n,rep...) ARITH_SUB_56(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_58(fn1,fn2,n,rep...) ARITH_SUB_57(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_59(fn1,fn2,n,rep...) ARITH_SUB_58(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_60(fn1,fn2,n,rep...) ARITH_SUB_59(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_61(fn1,fn2,n,rep...) ARITH_SUB_60(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_62(fn1,fn2,n,rep...) ARITH_SUB_61(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_63(fn1,fn2,n,rep...) ARITH_SUB_62(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_64(fn1,fn2,n,rep...) ARITH_SUB_63(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_65(fn1,fn2,n,rep...) ARITH_SUB_64(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_66(fn1,fn2,n,rep...) ARITH_SUB_65(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_67(fn1,fn2,n,rep...) ARITH_SUB_66(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_68(fn1,fn2,n,rep...) ARITH_SUB_67(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_69(fn1,fn2,n,rep...) ARITH_SUB_68(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_70(fn1,fn2,n,rep...) ARITH_SUB_69(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_71(fn1,fn2,n,rep...) ARITH_SUB_70(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_72(fn1,fn2,n,rep...) ARITH_SUB_71(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_73(fn1,fn2,n,rep...) ARITH_SUB_72(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_74(fn1,fn2,n,rep...) ARITH_SUB_73(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_75(fn1,fn2,n,rep...) ARITH_SUB_74(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_76(fn1,fn2,n,rep...) ARITH_SUB_75(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_77(fn1,fn2,n,rep...) ARITH_SUB_76(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_78(fn1,fn2,n,rep...) ARITH_SUB_77(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_79(fn1,fn2,n,rep...) ARITH_SUB_78(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_80(fn1,fn2,n,rep...) ARITH_SUB_79(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_81(fn1,fn2,n,rep...) ARITH_SUB_80(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_82(fn1,fn2,n,rep...) ARITH_SUB_81(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_83(fn1,fn2,n,rep...) ARITH_SUB_82(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_84(fn1,fn2,n,rep...) ARITH_SUB_83(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_85(fn1,fn2,n,rep...) ARITH_SUB_84(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_86(fn1,fn2,n,rep...) ARITH_SUB_85(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_87(fn1,fn2,n,rep...) ARITH_SUB_86(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_88(fn1,fn2,n,rep...) ARITH_SUB_87(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_89(fn1,fn2,n,rep...) ARITH_SUB_88(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_90(fn1,fn2,n,rep...) ARITH_SUB_89(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_91(fn1,fn2,n,rep...) ARITH_SUB_90(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_92(fn1,fn2,n,rep...) ARITH_SUB_91(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_93(fn1,fn2,n,rep...) ARITH_SUB_92(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_94(fn1,fn2,n,rep...) ARITH_SUB_93(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_95(fn1,fn2,n,rep...) ARITH_SUB_94(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_96(fn1,fn2,n,rep...) ARITH_SUB_95(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_97(fn1,fn2,n,rep...) ARITH_SUB_96(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_98(fn1,fn2,n,rep...) ARITH_SUB_97(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_99(fn1,fn2,n,rep...) ARITH_SUB_98(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_100(fn1,fn2,n,rep...) ARITH_SUB_99(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_101(fn1,fn2,n,rep...) ARITH_SUB_100(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_102(fn1,fn2,n,rep...) ARITH_SUB_101(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_103(fn1,fn2,n,rep...) ARITH_SUB_102(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_104(fn1,fn2,n,rep...) ARITH_SUB_103(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_105(fn1,fn2,n,rep...) ARITH_SUB_104(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_106(fn1,fn2,n,rep...) ARITH_SUB_105(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_107(fn1,fn2,n,rep...) ARITH_SUB_106(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_108(fn1,fn2,n,rep...) ARITH_SUB_107(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_109(fn1,fn2,n,rep...) ARITH_SUB_108(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_110(fn1,fn2,n,rep...) ARITH_SUB_109(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_111(fn1,fn2,n,rep...) ARITH_SUB_110(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_112(fn1,fn2,n,rep...) ARITH_SUB_111(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_113(fn1,fn2,n,rep...) ARITH_SUB_112(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_114(fn1,fn2,n,rep...) ARITH_SUB_113(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_115(fn1,fn2,n,rep...) ARITH_SUB_114(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_116(fn1,fn2,n,rep...) ARITH_SUB_115(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_117(fn1,fn2,n,rep...) ARITH_SUB_116(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_118(fn1,fn2,n,rep...) ARITH_SUB_117(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_119(fn1,fn2,n,rep...) ARITH_SUB_118(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_120(fn1,fn2,n,rep...) ARITH_SUB_119(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_121(fn1,fn2,n,rep...) ARITH_SUB_120(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_122(fn1,fn2,n,rep...) ARITH_SUB_121(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_123(fn1,fn2,n,rep...) ARITH_SUB_122(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_124(fn1,fn2,n,rep...) ARITH_SUB_123(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_125(fn1,fn2,n,rep...) ARITH_SUB_124(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_126(fn1,fn2,n,rep...) ARITH_SUB_125(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_127(fn1,fn2,n,rep...) ARITH_SUB_126(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_128(fn1,fn2,n,rep...) ARITH_SUB_127(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_129(fn1,fn2,n,rep...) ARITH_SUB_128(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_130(fn1,fn2,n,rep...) ARITH_SUB_129(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_131(fn1,fn2,n,rep...) ARITH_SUB_130(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_132(fn1,fn2,n,rep...) ARITH_SUB_131(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_133(fn1,fn2,n,rep...) ARITH_SUB_132(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_134(fn1,fn2,n,rep...) ARITH_SUB_133(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_135(fn1,fn2,n,rep...) ARITH_SUB_134(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_136(fn1,fn2,n,rep...) ARITH_SUB_135(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_137(fn1,fn2,n,rep...) ARITH_SUB_136(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_138(fn1,fn2,n,rep...) ARITH_SUB_137(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_139(fn1,fn2,n,rep...) ARITH_SUB_138(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_140(fn1,fn2,n,rep...) ARITH_SUB_139(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_141(fn1,fn2,n,rep...) ARITH_SUB_140(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_142(fn1,fn2,n,rep...) ARITH_SUB_141(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_143(fn1,fn2,n,rep...) ARITH_SUB_142(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_144(fn1,fn2,n,rep...) ARITH_SUB_143(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_145(fn1,fn2,n,rep...) ARITH_SUB_144(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_146(fn1,fn2,n,rep...) ARITH_SUB_145(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_147(fn1,fn2,n,rep...) ARITH_SUB_146(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_148(fn1,fn2,n,rep...) ARITH_SUB_147(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_149(fn1,fn2,n,rep...) ARITH_SUB_148(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_150(fn1,fn2,n,rep...) ARITH_SUB_149(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_151(fn1,fn2,n,rep...) ARITH_SUB_150(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_152(fn1,fn2,n,rep...) ARITH_SUB_151(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_153(fn1,fn2,n,rep...) ARITH_SUB_152(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_154(fn1,fn2,n,rep...) ARITH_SUB_153(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_155(fn1,fn2,n,rep...) ARITH_SUB_154(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_156(fn1,fn2,n,rep...) ARITH_SUB_155(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_157(fn1,fn2,n,rep...) ARITH_SUB_156(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_158(fn1,fn2,n,rep...) ARITH_SUB_157(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_159(fn1,fn2,n,rep...) ARITH_SUB_158(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_160(fn1,fn2,n,rep...) ARITH_SUB_159(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_161(fn1,fn2,n,rep...) ARITH_SUB_160(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_162(fn1,fn2,n,rep...) ARITH_SUB_161(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_163(fn1,fn2,n,rep...) ARITH_SUB_162(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_164(fn1,fn2,n,rep...) ARITH_SUB_163(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_165(fn1,fn2,n,rep...) ARITH_SUB_164(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_166(fn1,fn2,n,rep...) ARITH_SUB_165(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_167(fn1,fn2,n,rep...) ARITH_SUB_166(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_168(fn1,fn2,n,rep...) ARITH_SUB_167(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_169(fn1,fn2,n,rep...) ARITH_SUB_168(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_170(fn1,fn2,n,rep...) ARITH_SUB_169(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_171(fn1,fn2,n,rep...) ARITH_SUB_170(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_172(fn1,fn2,n,rep...) ARITH_SUB_171(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_173(fn1,fn2,n,rep...) ARITH_SUB_172(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_174(fn1,fn2,n,rep...) ARITH_SUB_173(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_175(fn1,fn2,n,rep...) ARITH_SUB_174(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_176(fn1,fn2,n,rep...) ARITH_SUB_175(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_177(fn1,fn2,n,rep...) ARITH_SUB_176(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_178(fn1,fn2,n,rep...) ARITH_SUB_177(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_179(fn1,fn2,n,rep...) ARITH_SUB_178(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_180(fn1,fn2,n,rep...) ARITH_SUB_179(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_181(fn1,fn2,n,rep...) ARITH_SUB_180(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_182(fn1,fn2,n,rep...) ARITH_SUB_181(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_183(fn1,fn2,n,rep...) ARITH_SUB_182(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_184(fn1,fn2,n,rep...) ARITH_SUB_183(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_185(fn1,fn2,n,rep...) ARITH_SUB_184(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_186(fn1,fn2,n,rep...) ARITH_SUB_185(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_187(fn1,fn2,n,rep...) ARITH_SUB_186(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_188(fn1,fn2,n,rep...) ARITH_SUB_187(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_189(fn1,fn2,n,rep...) ARITH_SUB_188(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_190(fn1,fn2,n,rep...) ARITH_SUB_189(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_191(fn1,fn2,n,rep...) ARITH_SUB_190(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_192(fn1,fn2,n,rep...) ARITH_SUB_191(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_193(fn1,fn2,n,rep...) ARITH_SUB_192(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_194(fn1,fn2,n,rep...) ARITH_SUB_193(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_195(fn1,fn2,n,rep...) ARITH_SUB_194(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_196(fn1,fn2,n,rep...) ARITH_SUB_195(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_197(fn1,fn2,n,rep...) ARITH_SUB_196(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_198(fn1,fn2,n,rep...) ARITH_SUB_197(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_199(fn1,fn2,n,rep...) ARITH_SUB_198(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_200(fn1,fn2,n,rep...) ARITH_SUB_199(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_201(fn1,fn2,n,rep...) ARITH_SUB_200(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_202(fn1,fn2,n,rep...) ARITH_SUB_201(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_203(fn1,fn2,n,rep...) ARITH_SUB_202(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_204(fn1,fn2,n,rep...) ARITH_SUB_203(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_205(fn1,fn2,n,rep...) ARITH_SUB_204(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_206(fn1,fn2,n,rep...) ARITH_SUB_205(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_207(fn1,fn2,n,rep...) ARITH_SUB_206(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_208(fn1,fn2,n,rep...) ARITH_SUB_207(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_209(fn1,fn2,n,rep...) ARITH_SUB_208(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_210(fn1,fn2,n,rep...) ARITH_SUB_209(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_211(fn1,fn2,n,rep...) ARITH_SUB_210(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_212(fn1,fn2,n,rep...) ARITH_SUB_211(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_213(fn1,fn2,n,rep...) ARITH_SUB_212(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_214(fn1,fn2,n,rep...) ARITH_SUB_213(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_215(fn1,fn2,n,rep...) ARITH_SUB_214(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_216(fn1,fn2,n,rep...) ARITH_SUB_215(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_217(fn1,fn2,n,rep...) ARITH_SUB_216(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_218(fn1,fn2,n,rep...) ARITH_SUB_217(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_219(fn1,fn2,n,rep...) ARITH_SUB_218(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_220(fn1,fn2,n,rep...) ARITH_SUB_219(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_221(fn1,fn2,n,rep...) ARITH_SUB_220(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_222(fn1,fn2,n,rep...) ARITH_SUB_221(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_223(fn1,fn2,n,rep...) ARITH_SUB_222(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_224(fn1,fn2,n,rep...) ARITH_SUB_223(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_225(fn1,fn2,n,rep...) ARITH_SUB_224(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_226(fn1,fn2,n,rep...) ARITH_SUB_225(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_227(fn1,fn2,n,rep...) ARITH_SUB_226(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_228(fn1,fn2,n,rep...) ARITH_SUB_227(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_229(fn1,fn2,n,rep...) ARITH_SUB_228(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_230(fn1,fn2,n,rep...) ARITH_SUB_229(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_231(fn1,fn2,n,rep...) ARITH_SUB_230(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_232(fn1,fn2,n,rep...) ARITH_SUB_231(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_233(fn1,fn2,n,rep...) ARITH_SUB_232(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_234(fn1,fn2,n,rep...) ARITH_SUB_233(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_235(fn1,fn2,n,rep...) ARITH_SUB_234(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_236(fn1,fn2,n,rep...) ARITH_SUB_235(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_237(fn1,fn2,n,rep...) ARITH_SUB_236(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_238(fn1,fn2,n,rep...) ARITH_SUB_237(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_239(fn1,fn2,n,rep...) ARITH_SUB_238(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_240(fn1,fn2,n,rep...) ARITH_SUB_239(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_241(fn1,fn2,n,rep...) ARITH_SUB_240(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_242(fn1,fn2,n,rep...) ARITH_SUB_241(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_243(fn1,fn2,n,rep...) ARITH_SUB_242(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_244(fn1,fn2,n,rep...) ARITH_SUB_243(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_245(fn1,fn2,n,rep...) ARITH_SUB_244(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_246(fn1,fn2,n,rep...) ARITH_SUB_245(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_247(fn1,fn2,n,rep...) ARITH_SUB_246(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_248(fn1,fn2,n,rep...) ARITH_SUB_247(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_249(fn1,fn2,n,rep...) ARITH_SUB_248(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_250(fn1,fn2,n,rep...) ARITH_SUB_249(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_251(fn1,fn2,n,rep...) ARITH_SUB_250(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_252(fn1,fn2,n,rep...) ARITH_SUB_251(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_253(fn1,fn2,n,rep...) ARITH_SUB_252(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_254(fn1,fn2,n,rep...) ARITH_SUB_253(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_255(fn1,fn2,n,rep...) ARITH_SUB_254(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)
 #define ARITH_SUB_256(fn1,fn2,n,rep...) ARITH_SUB_255(fn1,fn2,ARITH_DECR(n),rep) fn1(fn2,n,rep)

 #define ARITH_MUL_2(n,i) ARITH_MUL_1( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_3(n,i) ARITH_MUL_2( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_4(n,i) ARITH_MUL_3( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_5(n,i) ARITH_MUL_4( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_6(n,i) ARITH_MUL_5( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_7(n,i) ARITH_MUL_6( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_8(n,i) ARITH_MUL_7( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_9(n,i) ARITH_MUL_8( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_10(n,i) ARITH_MUL_9( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_11(n,i) ARITH_MUL_10( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_12(n,i) ARITH_MUL_11( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_13(n,i) ARITH_MUL_12( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_14(n,i) ARITH_MUL_13( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_15(n,i) ARITH_MUL_14( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_16(n,i) ARITH_MUL_15( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_17(n,i) ARITH_MUL_16( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_18(n,i) ARITH_MUL_17( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_19(n,i) ARITH_MUL_18( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_20(n,i) ARITH_MUL_19( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_21(n,i) ARITH_MUL_20( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_22(n,i) ARITH_MUL_21( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_23(n,i) ARITH_MUL_22( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_24(n,i) ARITH_MUL_23( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_25(n,i) ARITH_MUL_24( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_26(n,i) ARITH_MUL_25( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_27(n,i) ARITH_MUL_26( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_28(n,i) ARITH_MUL_27( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_29(n,i) ARITH_MUL_28( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_30(n,i) ARITH_MUL_29( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_31(n,i) ARITH_MUL_30( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_32(n,i) ARITH_MUL_31( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_33(n,i) ARITH_MUL_32( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_34(n,i) ARITH_MUL_33( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_35(n,i) ARITH_MUL_34( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_36(n,i) ARITH_MUL_35( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_37(n,i) ARITH_MUL_36( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_38(n,i) ARITH_MUL_37( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_39(n,i) ARITH_MUL_38( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_40(n,i) ARITH_MUL_39( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_41(n,i) ARITH_MUL_40( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_42(n,i) ARITH_MUL_41( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_43(n,i) ARITH_MUL_42( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_44(n,i) ARITH_MUL_43( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_45(n,i) ARITH_MUL_44( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_46(n,i) ARITH_MUL_45( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_47(n,i) ARITH_MUL_46( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_48(n,i) ARITH_MUL_47( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_49(n,i) ARITH_MUL_48( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_50(n,i) ARITH_MUL_49( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_51(n,i) ARITH_MUL_50( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_52(n,i) ARITH_MUL_51( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_53(n,i) ARITH_MUL_52( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_54(n,i) ARITH_MUL_53( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_55(n,i) ARITH_MUL_54( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_56(n,i) ARITH_MUL_55( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_57(n,i) ARITH_MUL_56( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_58(n,i) ARITH_MUL_57( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_59(n,i) ARITH_MUL_58( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_60(n,i) ARITH_MUL_59( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_61(n,i) ARITH_MUL_60( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_62(n,i) ARITH_MUL_61( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_63(n,i) ARITH_MUL_62( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_64(n,i) ARITH_MUL_63( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_65(n,i) ARITH_MUL_64( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_66(n,i) ARITH_MUL_65( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_67(n,i) ARITH_MUL_66( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_68(n,i) ARITH_MUL_67( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_69(n,i) ARITH_MUL_68( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_70(n,i) ARITH_MUL_69( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_71(n,i) ARITH_MUL_70( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_72(n,i) ARITH_MUL_71( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_73(n,i) ARITH_MUL_72( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_74(n,i) ARITH_MUL_73( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_75(n,i) ARITH_MUL_74( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_76(n,i) ARITH_MUL_75( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_77(n,i) ARITH_MUL_76( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_78(n,i) ARITH_MUL_77( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_79(n,i) ARITH_MUL_78( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_80(n,i) ARITH_MUL_79( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_81(n,i) ARITH_MUL_80( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_82(n,i) ARITH_MUL_81( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_83(n,i) ARITH_MUL_82( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_84(n,i) ARITH_MUL_83( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_85(n,i) ARITH_MUL_84( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_86(n,i) ARITH_MUL_85( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_87(n,i) ARITH_MUL_86( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_88(n,i) ARITH_MUL_87( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_89(n,i) ARITH_MUL_88( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_90(n,i) ARITH_MUL_89( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_91(n,i) ARITH_MUL_90( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_92(n,i) ARITH_MUL_91( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_93(n,i) ARITH_MUL_92( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_94(n,i) ARITH_MUL_93( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_95(n,i) ARITH_MUL_94( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_96(n,i) ARITH_MUL_95( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_97(n,i) ARITH_MUL_96( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_98(n,i) ARITH_MUL_97( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_99(n,i) ARITH_MUL_98( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_100(n,i) ARITH_MUL_99( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_101(n,i) ARITH_MUL_100( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_102(n,i) ARITH_MUL_101( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_103(n,i) ARITH_MUL_102( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_104(n,i) ARITH_MUL_103( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_105(n,i) ARITH_MUL_104( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_106(n,i) ARITH_MUL_105( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_107(n,i) ARITH_MUL_106( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_108(n,i) ARITH_MUL_107( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_109(n,i) ARITH_MUL_108( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_110(n,i) ARITH_MUL_109( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_111(n,i) ARITH_MUL_110( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_112(n,i) ARITH_MUL_111( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_113(n,i) ARITH_MUL_112( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_114(n,i) ARITH_MUL_113( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_115(n,i) ARITH_MUL_114( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_116(n,i) ARITH_MUL_115( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_117(n,i) ARITH_MUL_116( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_118(n,i) ARITH_MUL_117( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_119(n,i) ARITH_MUL_118( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_120(n,i) ARITH_MUL_119( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_121(n,i) ARITH_MUL_120( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_122(n,i) ARITH_MUL_121( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_123(n,i) ARITH_MUL_122( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_124(n,i) ARITH_MUL_123( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_125(n,i) ARITH_MUL_124( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_126(n,i) ARITH_MUL_125( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_127(n,i) ARITH_MUL_126( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_128(n,i) ARITH_MUL_127( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_129(n,i) ARITH_MUL_128( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_130(n,i) ARITH_MUL_129( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_131(n,i) ARITH_MUL_130( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_132(n,i) ARITH_MUL_131( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_133(n,i) ARITH_MUL_132( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_134(n,i) ARITH_MUL_133( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_135(n,i) ARITH_MUL_134( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_136(n,i) ARITH_MUL_135( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_137(n,i) ARITH_MUL_136( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_138(n,i) ARITH_MUL_137( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_139(n,i) ARITH_MUL_138( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_140(n,i) ARITH_MUL_139( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_141(n,i) ARITH_MUL_140( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_142(n,i) ARITH_MUL_141( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_143(n,i) ARITH_MUL_142( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_144(n,i) ARITH_MUL_143( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_145(n,i) ARITH_MUL_144( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_146(n,i) ARITH_MUL_145( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_147(n,i) ARITH_MUL_146( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_148(n,i) ARITH_MUL_147( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_149(n,i) ARITH_MUL_148( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_150(n,i) ARITH_MUL_149( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_151(n,i) ARITH_MUL_150( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_152(n,i) ARITH_MUL_151( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_153(n,i) ARITH_MUL_152( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_154(n,i) ARITH_MUL_153( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_155(n,i) ARITH_MUL_154( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_156(n,i) ARITH_MUL_155( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_157(n,i) ARITH_MUL_156( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_158(n,i) ARITH_MUL_157( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_159(n,i) ARITH_MUL_158( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_160(n,i) ARITH_MUL_159( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_161(n,i) ARITH_MUL_160( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_162(n,i) ARITH_MUL_161( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_163(n,i) ARITH_MUL_162( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_164(n,i) ARITH_MUL_163( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_165(n,i) ARITH_MUL_164( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_166(n,i) ARITH_MUL_165( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_167(n,i) ARITH_MUL_166( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_168(n,i) ARITH_MUL_167( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_169(n,i) ARITH_MUL_168( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_170(n,i) ARITH_MUL_169( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_171(n,i) ARITH_MUL_170( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_172(n,i) ARITH_MUL_171( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_173(n,i) ARITH_MUL_172( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_174(n,i) ARITH_MUL_173( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_175(n,i) ARITH_MUL_174( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_176(n,i) ARITH_MUL_175( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_177(n,i) ARITH_MUL_176( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_178(n,i) ARITH_MUL_177( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_179(n,i) ARITH_MUL_178( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_180(n,i) ARITH_MUL_179( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_181(n,i) ARITH_MUL_180( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_182(n,i) ARITH_MUL_181( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_183(n,i) ARITH_MUL_182( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_184(n,i) ARITH_MUL_183( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_185(n,i) ARITH_MUL_184( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_186(n,i) ARITH_MUL_185( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_187(n,i) ARITH_MUL_186( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_188(n,i) ARITH_MUL_187( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_189(n,i) ARITH_MUL_188( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_190(n,i) ARITH_MUL_189( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_191(n,i) ARITH_MUL_190( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_192(n,i) ARITH_MUL_191( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_193(n,i) ARITH_MUL_192( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_194(n,i) ARITH_MUL_193( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_195(n,i) ARITH_MUL_194( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_196(n,i) ARITH_MUL_195( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_197(n,i) ARITH_MUL_196( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_198(n,i) ARITH_MUL_197( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_199(n,i) ARITH_MUL_198( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_200(n,i) ARITH_MUL_199( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_201(n,i) ARITH_MUL_200( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_202(n,i) ARITH_MUL_201( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_203(n,i) ARITH_MUL_202( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_204(n,i) ARITH_MUL_203( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_205(n,i) ARITH_MUL_204( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_206(n,i) ARITH_MUL_205( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_207(n,i) ARITH_MUL_206( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_208(n,i) ARITH_MUL_207( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_209(n,i) ARITH_MUL_208( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_210(n,i) ARITH_MUL_209( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_211(n,i) ARITH_MUL_210( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_212(n,i) ARITH_MUL_211( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_213(n,i) ARITH_MUL_212( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_214(n,i) ARITH_MUL_213( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_215(n,i) ARITH_MUL_214( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_216(n,i) ARITH_MUL_215( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_217(n,i) ARITH_MUL_216( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_218(n,i) ARITH_MUL_217( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_219(n,i) ARITH_MUL_218( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_220(n,i) ARITH_MUL_219( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_221(n,i) ARITH_MUL_220( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_222(n,i) ARITH_MUL_221( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_223(n,i) ARITH_MUL_222( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_224(n,i) ARITH_MUL_223( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_225(n,i) ARITH_MUL_224( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_226(n,i) ARITH_MUL_225( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_227(n,i) ARITH_MUL_226( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_228(n,i) ARITH_MUL_227( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_229(n,i) ARITH_MUL_228( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_230(n,i) ARITH_MUL_229( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_231(n,i) ARITH_MUL_230( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_232(n,i) ARITH_MUL_231( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_233(n,i) ARITH_MUL_232( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_234(n,i) ARITH_MUL_233( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_235(n,i) ARITH_MUL_234( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_236(n,i) ARITH_MUL_235( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_237(n,i) ARITH_MUL_236( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_238(n,i) ARITH_MUL_237( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_239(n,i) ARITH_MUL_238( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_240(n,i) ARITH_MUL_239( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_241(n,i) ARITH_MUL_240( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_242(n,i) ARITH_MUL_241( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_243(n,i) ARITH_MUL_242( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_244(n,i) ARITH_MUL_243( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_245(n,i) ARITH_MUL_244( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_246(n,i) ARITH_MUL_245( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_247(n,i) ARITH_MUL_246( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_248(n,i) ARITH_MUL_247( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_249(n,i) ARITH_MUL_248( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_250(n,i) ARITH_MUL_249( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_251(n,i) ARITH_MUL_250( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_252(n,i) ARITH_MUL_251( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_253(n,i) ARITH_MUL_252( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_254(n,i) ARITH_MUL_253( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_255(n,i) ARITH_MUL_254( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_256(n,i) ARITH_MUL_255( n,ARITH_ADD(i,n) )

 #define ARITH_DIV_1(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(1,m)) (fn,m,ARITH_INCR(i),1)
 #define ARITH_DIV_2(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(2,m)) (fn,m,ARITH_INCR(i),2)
 #define ARITH_DIV_3(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(3,m)) (fn,m,ARITH_INCR(i),3)
 #define ARITH_DIV_4(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(4,m)) (fn,m,ARITH_INCR(i),4)
 #define ARITH_DIV_5(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(5,m)) (fn,m,ARITH_INCR(i),5)
 #define ARITH_DIV_6(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(6,m)) (fn,m,ARITH_INCR(i),6)
 #define ARITH_DIV_7(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(7,m)) (fn,m,ARITH_INCR(i),7)
 #define ARITH_DIV_8(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(8,m)) (fn,m,ARITH_INCR(i),8)
 #define ARITH_DIV_9(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(9,m)) (fn,m,ARITH_INCR(i),9)
 #define ARITH_DIV_10(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(10,m)) (fn,m,ARITH_INCR(i),10)
 #define ARITH_DIV_11(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(11,m)) (fn,m,ARITH_INCR(i),11)
 #define ARITH_DIV_12(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(12,m)) (fn,m,ARITH_INCR(i),12)
 #define ARITH_DIV_13(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(13,m)) (fn,m,ARITH_INCR(i),13)
 #define ARITH_DIV_14(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(14,m)) (fn,m,ARITH_INCR(i),14)
 #define ARITH_DIV_15(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(15,m)) (fn,m,ARITH_INCR(i),15)
 #define ARITH_DIV_16(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(16,m)) (fn,m,ARITH_INCR(i),16)
 #define ARITH_DIV_17(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(17,m)) (fn,m,ARITH_INCR(i),17)
 #define ARITH_DIV_18(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(18,m)) (fn,m,ARITH_INCR(i),18)
 #define ARITH_DIV_19(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(19,m)) (fn,m,ARITH_INCR(i),19)
 #define ARITH_DIV_20(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(20,m)) (fn,m,ARITH_INCR(i),20)
 #define ARITH_DIV_21(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(21,m)) (fn,m,ARITH_INCR(i),21)
 #define ARITH_DIV_22(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(22,m)) (fn,m,ARITH_INCR(i),22)
 #define ARITH_DIV_23(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(23,m)) (fn,m,ARITH_INCR(i),23)
 #define ARITH_DIV_24(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(24,m)) (fn,m,ARITH_INCR(i),24)
 #define ARITH_DIV_25(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(25,m)) (fn,m,ARITH_INCR(i),25)
 #define ARITH_DIV_26(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(26,m)) (fn,m,ARITH_INCR(i),26)
 #define ARITH_DIV_27(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(27,m)) (fn,m,ARITH_INCR(i),27)
 #define ARITH_DIV_28(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(28,m)) (fn,m,ARITH_INCR(i),28)
 #define ARITH_DIV_29(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(29,m)) (fn,m,ARITH_INCR(i),29)
 #define ARITH_DIV_30(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(30,m)) (fn,m,ARITH_INCR(i),30)
 #define ARITH_DIV_31(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(31,m)) (fn,m,ARITH_INCR(i),31)
 #define ARITH_DIV_32(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(32,m)) (fn,m,ARITH_INCR(i),32)
 #define ARITH_DIV_33(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(33,m)) (fn,m,ARITH_INCR(i),33)
 #define ARITH_DIV_34(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(34,m)) (fn,m,ARITH_INCR(i),34)
 #define ARITH_DIV_35(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(35,m)) (fn,m,ARITH_INCR(i),35)
 #define ARITH_DIV_36(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(36,m)) (fn,m,ARITH_INCR(i),36)
 #define ARITH_DIV_37(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(37,m)) (fn,m,ARITH_INCR(i),37)
 #define ARITH_DIV_38(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(38,m)) (fn,m,ARITH_INCR(i),38)
 #define ARITH_DIV_39(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(39,m)) (fn,m,ARITH_INCR(i),39)
 #define ARITH_DIV_40(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(40,m)) (fn,m,ARITH_INCR(i),40)
 #define ARITH_DIV_41(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(41,m)) (fn,m,ARITH_INCR(i),41)
 #define ARITH_DIV_42(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(42,m)) (fn,m,ARITH_INCR(i),42)
 #define ARITH_DIV_43(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(43,m)) (fn,m,ARITH_INCR(i),43)
 #define ARITH_DIV_44(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(44,m)) (fn,m,ARITH_INCR(i),44)
 #define ARITH_DIV_45(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(45,m)) (fn,m,ARITH_INCR(i),45)
 #define ARITH_DIV_46(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(46,m)) (fn,m,ARITH_INCR(i),46)
 #define ARITH_DIV_47(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(47,m)) (fn,m,ARITH_INCR(i),47)
 #define ARITH_DIV_48(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(48,m)) (fn,m,ARITH_INCR(i),48)
 #define ARITH_DIV_49(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(49,m)) (fn,m,ARITH_INCR(i),49)
 #define ARITH_DIV_50(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(50,m)) (fn,m,ARITH_INCR(i),50)
 #define ARITH_DIV_51(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(51,m)) (fn,m,ARITH_INCR(i),51)
 #define ARITH_DIV_52(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(52,m)) (fn,m,ARITH_INCR(i),52)
 #define ARITH_DIV_53(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(53,m)) (fn,m,ARITH_INCR(i),53)
 #define ARITH_DIV_54(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(54,m)) (fn,m,ARITH_INCR(i),54)
 #define ARITH_DIV_55(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(55,m)) (fn,m,ARITH_INCR(i),55)
 #define ARITH_DIV_56(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(56,m)) (fn,m,ARITH_INCR(i),56)
 #define ARITH_DIV_57(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(57,m)) (fn,m,ARITH_INCR(i),57)
 #define ARITH_DIV_58(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(58,m)) (fn,m,ARITH_INCR(i),58)
 #define ARITH_DIV_59(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(59,m)) (fn,m,ARITH_INCR(i),59)
 #define ARITH_DIV_60(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(60,m)) (fn,m,ARITH_INCR(i),60)
 #define ARITH_DIV_61(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(61,m)) (fn,m,ARITH_INCR(i),61)
 #define ARITH_DIV_62(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(62,m)) (fn,m,ARITH_INCR(i),62)
 #define ARITH_DIV_63(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(63,m)) (fn,m,ARITH_INCR(i),63)
 #define ARITH_DIV_64(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(64,m)) (fn,m,ARITH_INCR(i),64)
 #define ARITH_DIV_65(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(64,m)) (fn,m,ARITH_INCR(i),65)
 #define ARITH_DIV_66(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(65,m)) (fn,m,ARITH_INCR(i),66)
 #define ARITH_DIV_67(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(66,m)) (fn,m,ARITH_INCR(i),67)
 #define ARITH_DIV_68(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(67,m)) (fn,m,ARITH_INCR(i),68)
 #define ARITH_DIV_69(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(68,m)) (fn,m,ARITH_INCR(i),69)
 #define ARITH_DIV_70(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(69,m)) (fn,m,ARITH_INCR(i),70)
 #define ARITH_DIV_71(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(70,m)) (fn,m,ARITH_INCR(i),71)
 #define ARITH_DIV_72(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(71,m)) (fn,m,ARITH_INCR(i),72)
 #define ARITH_DIV_73(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(72,m)) (fn,m,ARITH_INCR(i),73)
 #define ARITH_DIV_74(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(73,m)) (fn,m,ARITH_INCR(i),74)
 #define ARITH_DIV_75(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(74,m)) (fn,m,ARITH_INCR(i),75)
 #define ARITH_DIV_76(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(75,m)) (fn,m,ARITH_INCR(i),76)
 #define ARITH_DIV_77(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(76,m)) (fn,m,ARITH_INCR(i),77)
 #define ARITH_DIV_78(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(77,m)) (fn,m,ARITH_INCR(i),78)
 #define ARITH_DIV_79(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(78,m)) (fn,m,ARITH_INCR(i),79)
 #define ARITH_DIV_80(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(79,m)) (fn,m,ARITH_INCR(i),80)
 #define ARITH_DIV_81(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(80,m)) (fn,m,ARITH_INCR(i),81)
 #define ARITH_DIV_82(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(81,m)) (fn,m,ARITH_INCR(i),82)
 #define ARITH_DIV_83(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(82,m)) (fn,m,ARITH_INCR(i),83)
 #define ARITH_DIV_84(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(83,m)) (fn,m,ARITH_INCR(i),84)
 #define ARITH_DIV_85(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(84,m)) (fn,m,ARITH_INCR(i),85)
 #define ARITH_DIV_86(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(85,m)) (fn,m,ARITH_INCR(i),86)
 #define ARITH_DIV_87(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(86,m)) (fn,m,ARITH_INCR(i),87)
 #define ARITH_DIV_88(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(87,m)) (fn,m,ARITH_INCR(i),88)
 #define ARITH_DIV_89(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(88,m)) (fn,m,ARITH_INCR(i),89)
 #define ARITH_DIV_90(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(89,m)) (fn,m,ARITH_INCR(i),90)
 #define ARITH_DIV_91(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(90,m)) (fn,m,ARITH_INCR(i),91)
 #define ARITH_DIV_92(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(91,m)) (fn,m,ARITH_INCR(i),92)
 #define ARITH_DIV_93(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(92,m)) (fn,m,ARITH_INCR(i),93)
 #define ARITH_DIV_94(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(93,m)) (fn,m,ARITH_INCR(i),94)
 #define ARITH_DIV_95(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(94,m)) (fn,m,ARITH_INCR(i),95)
 #define ARITH_DIV_96(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(95,m)) (fn,m,ARITH_INCR(i),96)
 #define ARITH_DIV_97(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(96,m)) (fn,m,ARITH_INCR(i),97)
 #define ARITH_DIV_98(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(97,m)) (fn,m,ARITH_INCR(i),98)
 #define ARITH_DIV_99(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(98,m)) (fn,m,ARITH_INCR(i),99)
 #define ARITH_DIV_100(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(99,m)) (fn,m,ARITH_INCR(i),100)
 #define ARITH_DIV_101(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(100,m)) (fn,m,ARITH_INCR(i),101)
 #define ARITH_DIV_102(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(101,m)) (fn,m,ARITH_INCR(i),102)
 #define ARITH_DIV_103(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(102,m)) (fn,m,ARITH_INCR(i),103)
 #define ARITH_DIV_104(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(103,m)) (fn,m,ARITH_INCR(i),104)
 #define ARITH_DIV_105(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(104,m)) (fn,m,ARITH_INCR(i),105)
 #define ARITH_DIV_106(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(105,m)) (fn,m,ARITH_INCR(i),106)
 #define ARITH_DIV_107(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(106,m)) (fn,m,ARITH_INCR(i),107)
 #define ARITH_DIV_108(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(107,m)) (fn,m,ARITH_INCR(i),108)
 #define ARITH_DIV_109(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(108,m)) (fn,m,ARITH_INCR(i),109)
 #define ARITH_DIV_110(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(109,m)) (fn,m,ARITH_INCR(i),110)
 #define ARITH_DIV_111(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(110,m)) (fn,m,ARITH_INCR(i),111)
 #define ARITH_DIV_112(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(111,m)) (fn,m,ARITH_INCR(i),112)
 #define ARITH_DIV_113(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(112,m)) (fn,m,ARITH_INCR(i),113)
 #define ARITH_DIV_114(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(113,m)) (fn,m,ARITH_INCR(i),114)
 #define ARITH_DIV_115(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(114,m)) (fn,m,ARITH_INCR(i),115)
 #define ARITH_DIV_116(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(115,m)) (fn,m,ARITH_INCR(i),116)
 #define ARITH_DIV_117(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(116,m)) (fn,m,ARITH_INCR(i),117)
 #define ARITH_DIV_118(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(117,m)) (fn,m,ARITH_INCR(i),118)
 #define ARITH_DIV_119(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(118,m)) (fn,m,ARITH_INCR(i),119)
 #define ARITH_DIV_120(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(119,m)) (fn,m,ARITH_INCR(i),120)
 #define ARITH_DIV_121(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(120,m)) (fn,m,ARITH_INCR(i),121)
 #define ARITH_DIV_122(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(121,m)) (fn,m,ARITH_INCR(i),122)
 #define ARITH_DIV_123(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(122,m)) (fn,m,ARITH_INCR(i),123)
 #define ARITH_DIV_124(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(123,m)) (fn,m,ARITH_INCR(i),124)
 #define ARITH_DIV_125(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(124,m)) (fn,m,ARITH_INCR(i),125)
 #define ARITH_DIV_126(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(125,m)) (fn,m,ARITH_INCR(i),126)
 #define ARITH_DIV_127(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(126,m)) (fn,m,ARITH_INCR(i),127)
 #define ARITH_DIV_128(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(127,m)) (fn,m,ARITH_INCR(i),128)
 #define ARITH_DIV_129(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(128,m)) (fn,m,ARITH_INCR(i),129)
 #define ARITH_DIV_130(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(129,m)) (fn,m,ARITH_INCR(i),130)
 #define ARITH_DIV_131(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(130,m)) (fn,m,ARITH_INCR(i),131)
 #define ARITH_DIV_132(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(131,m)) (fn,m,ARITH_INCR(i),132)
 #define ARITH_DIV_133(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(132,m)) (fn,m,ARITH_INCR(i),133)
 #define ARITH_DIV_134(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(133,m)) (fn,m,ARITH_INCR(i),134)
 #define ARITH_DIV_135(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(134,m)) (fn,m,ARITH_INCR(i),135)
 #define ARITH_DIV_136(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(135,m)) (fn,m,ARITH_INCR(i),136)
 #define ARITH_DIV_137(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(136,m)) (fn,m,ARITH_INCR(i),137)
 #define ARITH_DIV_138(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(137,m)) (fn,m,ARITH_INCR(i),138)
 #define ARITH_DIV_139(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(138,m)) (fn,m,ARITH_INCR(i),139)
 #define ARITH_DIV_140(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(139,m)) (fn,m,ARITH_INCR(i),140)
 #define ARITH_DIV_141(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(140,m)) (fn,m,ARITH_INCR(i),141)
 #define ARITH_DIV_142(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(141,m)) (fn,m,ARITH_INCR(i),142)
 #define ARITH_DIV_143(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(142,m)) (fn,m,ARITH_INCR(i),143)
 #define ARITH_DIV_144(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(143,m)) (fn,m,ARITH_INCR(i),144)
 #define ARITH_DIV_145(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(144,m)) (fn,m,ARITH_INCR(i),145)
 #define ARITH_DIV_146(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(145,m)) (fn,m,ARITH_INCR(i),146)
 #define ARITH_DIV_147(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(146,m)) (fn,m,ARITH_INCR(i),147)
 #define ARITH_DIV_148(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(147,m)) (fn,m,ARITH_INCR(i),148)
 #define ARITH_DIV_149(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(148,m)) (fn,m,ARITH_INCR(i),149)
 #define ARITH_DIV_150(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(149,m)) (fn,m,ARITH_INCR(i),150)
 #define ARITH_DIV_151(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(150,m)) (fn,m,ARITH_INCR(i),151)
 #define ARITH_DIV_152(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(151,m)) (fn,m,ARITH_INCR(i),152)
 #define ARITH_DIV_153(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(152,m)) (fn,m,ARITH_INCR(i),153)
 #define ARITH_DIV_154(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(153,m)) (fn,m,ARITH_INCR(i),154)
 #define ARITH_DIV_155(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(154,m)) (fn,m,ARITH_INCR(i),155)
 #define ARITH_DIV_156(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(155,m)) (fn,m,ARITH_INCR(i),156)
 #define ARITH_DIV_157(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(156,m)) (fn,m,ARITH_INCR(i),157)
 #define ARITH_DIV_158(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(157,m)) (fn,m,ARITH_INCR(i),158)
 #define ARITH_DIV_159(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(158,m)) (fn,m,ARITH_INCR(i),159)
 #define ARITH_DIV_160(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(159,m)) (fn,m,ARITH_INCR(i),160)
 #define ARITH_DIV_161(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(160,m)) (fn,m,ARITH_INCR(i),161)
 #define ARITH_DIV_162(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(161,m)) (fn,m,ARITH_INCR(i),162)
 #define ARITH_DIV_163(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(162,m)) (fn,m,ARITH_INCR(i),163)
 #define ARITH_DIV_164(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(163,m)) (fn,m,ARITH_INCR(i),164)
 #define ARITH_DIV_165(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(164,m)) (fn,m,ARITH_INCR(i),165)
 #define ARITH_DIV_166(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(165,m)) (fn,m,ARITH_INCR(i),166)
 #define ARITH_DIV_167(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(166,m)) (fn,m,ARITH_INCR(i),167)
 #define ARITH_DIV_168(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(167,m)) (fn,m,ARITH_INCR(i),168)
 #define ARITH_DIV_169(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(168,m)) (fn,m,ARITH_INCR(i),169)
 #define ARITH_DIV_170(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(169,m)) (fn,m,ARITH_INCR(i),170)
 #define ARITH_DIV_171(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(170,m)) (fn,m,ARITH_INCR(i),171)
 #define ARITH_DIV_172(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(171,m)) (fn,m,ARITH_INCR(i),172)
 #define ARITH_DIV_173(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(172,m)) (fn,m,ARITH_INCR(i),173)
 #define ARITH_DIV_174(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(173,m)) (fn,m,ARITH_INCR(i),174)
 #define ARITH_DIV_175(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(174,m)) (fn,m,ARITH_INCR(i),175)
 #define ARITH_DIV_176(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(175,m)) (fn,m,ARITH_INCR(i),176)
 #define ARITH_DIV_177(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(176,m)) (fn,m,ARITH_INCR(i),177)
 #define ARITH_DIV_178(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(177,m)) (fn,m,ARITH_INCR(i),178)
 #define ARITH_DIV_179(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(178,m)) (fn,m,ARITH_INCR(i),179)
 #define ARITH_DIV_180(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(179,m)) (fn,m,ARITH_INCR(i),180)
 #define ARITH_DIV_181(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(180,m)) (fn,m,ARITH_INCR(i),181)
 #define ARITH_DIV_182(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(181,m)) (fn,m,ARITH_INCR(i),182)
 #define ARITH_DIV_183(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(182,m)) (fn,m,ARITH_INCR(i),183)
 #define ARITH_DIV_184(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(183,m)) (fn,m,ARITH_INCR(i),184)
 #define ARITH_DIV_185(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(184,m)) (fn,m,ARITH_INCR(i),185)
 #define ARITH_DIV_186(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(185,m)) (fn,m,ARITH_INCR(i),186)
 #define ARITH_DIV_187(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(186,m)) (fn,m,ARITH_INCR(i),187)
 #define ARITH_DIV_188(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(187,m)) (fn,m,ARITH_INCR(i),188)
 #define ARITH_DIV_189(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(188,m)) (fn,m,ARITH_INCR(i),189)
 #define ARITH_DIV_190(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(189,m)) (fn,m,ARITH_INCR(i),190)
 #define ARITH_DIV_191(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(190,m)) (fn,m,ARITH_INCR(i),191)
 #define ARITH_DIV_192(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(191,m)) (fn,m,ARITH_INCR(i),192)
 #define ARITH_DIV_193(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(192,m)) (fn,m,ARITH_INCR(i),193)
 #define ARITH_DIV_194(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(193,m)) (fn,m,ARITH_INCR(i),194)
 #define ARITH_DIV_195(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(194,m)) (fn,m,ARITH_INCR(i),195)
 #define ARITH_DIV_196(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(195,m)) (fn,m,ARITH_INCR(i),196)
 #define ARITH_DIV_197(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(196,m)) (fn,m,ARITH_INCR(i),197)
 #define ARITH_DIV_198(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(197,m)) (fn,m,ARITH_INCR(i),198)
 #define ARITH_DIV_199(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(198,m)) (fn,m,ARITH_INCR(i),199)
 #define ARITH_DIV_200(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(199,m)) (fn,m,ARITH_INCR(i),200)
 #define ARITH_DIV_201(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(200,m)) (fn,m,ARITH_INCR(i),201)
 #define ARITH_DIV_202(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(201,m)) (fn,m,ARITH_INCR(i),202)
 #define ARITH_DIV_203(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(202,m)) (fn,m,ARITH_INCR(i),203)
 #define ARITH_DIV_204(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(203,m)) (fn,m,ARITH_INCR(i),204)
 #define ARITH_DIV_205(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(204,m)) (fn,m,ARITH_INCR(i),205)
 #define ARITH_DIV_206(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(205,m)) (fn,m,ARITH_INCR(i),206)
 #define ARITH_DIV_207(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(206,m)) (fn,m,ARITH_INCR(i),207)
 #define ARITH_DIV_208(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(207,m)) (fn,m,ARITH_INCR(i),208)
 #define ARITH_DIV_209(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(208,m)) (fn,m,ARITH_INCR(i),209)
 #define ARITH_DIV_210(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(209,m)) (fn,m,ARITH_INCR(i),210)
 #define ARITH_DIV_211(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(210,m)) (fn,m,ARITH_INCR(i),211)
 #define ARITH_DIV_212(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(211,m)) (fn,m,ARITH_INCR(i),212)
 #define ARITH_DIV_213(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(212,m)) (fn,m,ARITH_INCR(i),213)
 #define ARITH_DIV_214(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(213,m)) (fn,m,ARITH_INCR(i),214)
 #define ARITH_DIV_215(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(214,m)) (fn,m,ARITH_INCR(i),215)
 #define ARITH_DIV_216(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(215,m)) (fn,m,ARITH_INCR(i),216)
 #define ARITH_DIV_217(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(216,m)) (fn,m,ARITH_INCR(i),217)
 #define ARITH_DIV_218(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(217,m)) (fn,m,ARITH_INCR(i),218)
 #define ARITH_DIV_219(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(218,m)) (fn,m,ARITH_INCR(i),219)
 #define ARITH_DIV_220(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(219,m)) (fn,m,ARITH_INCR(i),220)
 #define ARITH_DIV_221(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(220,m)) (fn,m,ARITH_INCR(i),221)
 #define ARITH_DIV_222(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(221,m)) (fn,m,ARITH_INCR(i),222)
 #define ARITH_DIV_223(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(222,m)) (fn,m,ARITH_INCR(i),223)
 #define ARITH_DIV_224(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(223,m)) (fn,m,ARITH_INCR(i),224)
 #define ARITH_DIV_225(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(224,m)) (fn,m,ARITH_INCR(i),225)
 #define ARITH_DIV_226(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(225,m)) (fn,m,ARITH_INCR(i),226)
 #define ARITH_DIV_227(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(226,m)) (fn,m,ARITH_INCR(i),227)
 #define ARITH_DIV_228(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(227,m)) (fn,m,ARITH_INCR(i),228)
 #define ARITH_DIV_229(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(228,m)) (fn,m,ARITH_INCR(i),229)
 #define ARITH_DIV_230(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(229,m)) (fn,m,ARITH_INCR(i),230)
 #define ARITH_DIV_231(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(230,m)) (fn,m,ARITH_INCR(i),231)
 #define ARITH_DIV_232(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(231,m)) (fn,m,ARITH_INCR(i),232)
 #define ARITH_DIV_233(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(232,m)) (fn,m,ARITH_INCR(i),233)
 #define ARITH_DIV_234(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(233,m)) (fn,m,ARITH_INCR(i),234)
 #define ARITH_DIV_235(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(234,m)) (fn,m,ARITH_INCR(i),235)
 #define ARITH_DIV_236(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(235,m)) (fn,m,ARITH_INCR(i),236)
 #define ARITH_DIV_237(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(236,m)) (fn,m,ARITH_INCR(i),237)
 #define ARITH_DIV_238(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(237,m)) (fn,m,ARITH_INCR(i),238)
 #define ARITH_DIV_239(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(238,m)) (fn,m,ARITH_INCR(i),239)
 #define ARITH_DIV_240(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(239,m)) (fn,m,ARITH_INCR(i),240)
 #define ARITH_DIV_241(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(240,m)) (fn,m,ARITH_INCR(i),241)
 #define ARITH_DIV_242(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(241,m)) (fn,m,ARITH_INCR(i),242)
 #define ARITH_DIV_243(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(242,m)) (fn,m,ARITH_INCR(i),243)
 #define ARITH_DIV_244(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(243,m)) (fn,m,ARITH_INCR(i),244)
 #define ARITH_DIV_245(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(244,m)) (fn,m,ARITH_INCR(i),245)
 #define ARITH_DIV_246(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(245,m)) (fn,m,ARITH_INCR(i),246)
 #define ARITH_DIV_247(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(246,m)) (fn,m,ARITH_INCR(i),247)
 #define ARITH_DIV_248(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(247,m)) (fn,m,ARITH_INCR(i),248)
 #define ARITH_DIV_249(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(248,m)) (fn,m,ARITH_INCR(i),249)
 #define ARITH_DIV_250(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(249,m)) (fn,m,ARITH_INCR(i),250)
 #define ARITH_DIV_251(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(250,m)) (fn,m,ARITH_INCR(i),251)
 #define ARITH_DIV_252(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(251,m)) (fn,m,ARITH_INCR(i),252)
 #define ARITH_DIV_253(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(252,m)) (fn,m,ARITH_INCR(i),253)
 #define ARITH_DIV_254(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(253,m)) (fn,m,ARITH_INCR(i),254)
 #define ARITH_DIV_255(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(254,m)) (fn,m,ARITH_INCR(i),255)
 #define ARITH_DIV_256(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(255,m)) (fn,m,ARITH_INCR(i),256)

#endif /* CPPARITH_H */


#ifdef CPPARITH_TWICE
 #ifndef CPPARITH_HAVE_TWICE
  #define CPPARITH_HAVE_TWICE 1
 
  #define ARITH2_BLOCK
  #define ARITH2_EXPAND(...)        __VA_ARGS__
  #define ARITH2_DEFER(Fn,...)      ARITH2_EXPAND( Fn ARITH2_BLOCK ( ARITH2_EXPAND(__VA_ARGS__) ))
 
  #define ARITH2_HEAD(list...)      ARITH2_HEAD_i(list)
  #define ARITH2_HEAD_i(car,cdr...) car
  #define ARITH2_TAIL(list...)      ARITH2_TAIL_i(list)
  #define ARITH2_TAIL_i(car,cdr...) cdr
 
  #define ARITH2_CATN(OP,N)         ARITH2_DEFER( ARITH2_CATN_i, OP, N )
  #define ARITH2_CATN_i(OP,N)       ARITH2_ ## OP ## _ ## N
 
 
 
 
  #define ARITH2_INCR(N)            ARITH2_CATN( INCR, N )
  #define ARITH2_DECR(N)            ARITH2_CATN( DECR, N )
 
  #define ARITH2_INCR_256           ARITH2_ERR_MAX
  #define ARITH2_INCR_ARITH2_ERR_MAX ARITH2_ERR_MAX
  #define ARITH2_DECR_0             ARITH2_ERR_MIN
  #define ARITH2_DECR_ARITH2_ERR_MIN ARITH2_ERR_MIN
 
 
 
  #define ARITH2_ADD(N,M)           ARITH2_HEAD(ARITH2_CATN( ADD, M ) (ARITH2_ADDfn,,N))
  #define ARITH2_ADDfn(...)
  #define ARITH2_ADDfn_0(Fn,N)      N
 
  #define ARITH2_ADD_0(Fn1,Fn2,N)   Fn1##_0(Fn2,N)
 
 
  #define ARITH2_SUB(N,M)           ARITH2_HEAD(ARITH2_CATN( SUB, M ) (ARITH2_SUBfn,,N,))
  #define ARITH2_SUBfn(...)
  #define ARITH2_SUBfn_0(Fn,N,...)     N
 
  #define ARITH2_SUB_0(Fn1,Fn2,N,REP...) Fn1##_0 (Fn2,N,REP)
  #define ARITH2_SUB_ARITH2_ERR_MIN(...) ARITH2_ERR_MIN
 
 
  #define ARITH2_REP(N,REP...)      ARITH2_TAIL(ARITH2_CATN( SUB, N ) (ARITH2_REPfn,,N,REP))
  #define ARITH2_REPfn(N,_,REP...)    , REP
  #define ARITH2_REPfn_0(...)
 
 
 
  #define ARITH2_INDEX_GE(N,M,NG,MG) ARITH2_CATN( GE, ARITH2_SUB(ARITH2_SUB(N,M),ARITH2_SUB(N,M)) ) (NG,MG)
  #define ARITH2_INDEX_LE(N,M,NL,ML) ARITH2_CATN( GE, ARITH2_SUB(ARITH2_SUB(M,N),ARITH2_SUB(M,N)) ) (NL,ML)
  #define ARITH2_GE_0(N,M)          N
  #define ARITH2_GE_ARITH2_ERR_MIN(N,M) M
 
 
  #define ARITH2_MAX(N,M)           ARITH2_INDEX_GE(N,M,N,M)
  #define ARITH2_MIN(N,M)           ARITH2_INDEX_LE(N,M,N,M)
 
 
  #define ARITH2_DEFAULT(nop...)    nop
  #define ARITH2_SEQ(re,expand...)  ARITH2_SEQi(re,expand,ARITH2_DEFAULT,)
  #define ARITH2_SEQi(reexpand...)  ARITH2_SEQ_i(reexpand)
  #define ARITH2_SEQ_i(N,M,Fn,...)  ARITH2_SEQ_ii(ARITH2_INDEX_GE(N,M,n,p), Fn, N, M)
  #define ARITH2_SEQ_ii(C,Fn,N,M)   ARITH2_SEQ_iii(C,Fn,N,M)
  #define ARITH2_SEQ_iii(C,Fn,N,M)  ARITH2_CATN( SEQ, C ) (ARITH2_SEQ##C##fn,Fn,N,M)
 
  #define ARITH2_SEQ_p(C,Fn,N,M)    ARITH2_TAIL(ARITH2_CATN (SUB, ARITH2_SUB(ARITH2_INCR(M),N)) (C,Fn,M))
  #define ARITH2_SEQ_n(C,Fn,N,M)    ARITH2_CATN(ADD, ARITH2_SUB(N,M)) (C,Fn,M)
 
  #define ARITH2_SEQpfn(fn,N,...)   , fn(N)
  #define ARITH2_SEQpfn_0(...)
 
  #define ARITH2_SEQnfn(fn,N,...)   , fn(N)
  #define ARITH2_SEQnfn_0(fn,N)     fn(N)
 
 
  #define ARITH2_MUL(N,M)           ARITH2_CATN( MUL, M ) (N,0)
  #define ARITH2_DIV(N,M)           ARITH2_CATN( DIV, N ) (ARITH2_DIVfn,M,0,0)
  #define ARITH2_MOD(N,M)           ARITH2_CATN( DIV, N ) (ARITH2_MODfn,M,0,0)
 
  #define ARITH2_MUL_0(n,i)         0
  #define ARITH2_MUL_1(n,i)         ARITH2_ADD(i,n)
 
  #define ARITH2_DIV_0(fn,m,i,p)    fn##_0 (i)
  #define ARITH2_DIVfn_0(i)         i
  #define ARITH2_MODfn_0(i)         0
 
  #define ARITH2_DIVfn_ERR(i,p)     i
  #define ARITH2_MODfn_ERR(i,p)     p
  #define ARITH2_DIV_ARITH2_ERR_MIN(fn,m,i,p) fn##_ERR (ARITH2_DECR(i),p)
 
 
 
 
  #define ARITH2_INCR_0    1
  #define ARITH2_INCR_1    2
  #define ARITH2_INCR_2    3
  #define ARITH2_INCR_3    4
  #define ARITH2_INCR_4    5
  #define ARITH2_INCR_5    6
  #define ARITH2_INCR_6    7
  #define ARITH2_INCR_7    8
  #define ARITH2_INCR_8    9
  #define ARITH2_INCR_9   10
  #define ARITH2_INCR_10  11
  #define ARITH2_INCR_11  12
  #define ARITH2_INCR_12  13
  #define ARITH2_INCR_13  14
  #define ARITH2_INCR_14  15
  #define ARITH2_INCR_15  16
  #define ARITH2_INCR_16  17
  #define ARITH2_INCR_17  18
  #define ARITH2_INCR_18  19
  #define ARITH2_INCR_19  20
  #define ARITH2_INCR_20  21
  #define ARITH2_INCR_21  22
  #define ARITH2_INCR_22  23
  #define ARITH2_INCR_23  24
  #define ARITH2_INCR_24  25
  #define ARITH2_INCR_25  26
  #define ARITH2_INCR_26  27
  #define ARITH2_INCR_27  28
  #define ARITH2_INCR_28  29
  #define ARITH2_INCR_29  30
  #define ARITH2_INCR_30  31
  #define ARITH2_INCR_31  32
  #define ARITH2_INCR_32  33
  #define ARITH2_INCR_33  34
  #define ARITH2_INCR_34  35
  #define ARITH2_INCR_35  36
  #define ARITH2_INCR_36  37
  #define ARITH2_INCR_37  38
  #define ARITH2_INCR_38  39
  #define ARITH2_INCR_39  40
  #define ARITH2_INCR_40  41
  #define ARITH2_INCR_41  42
  #define ARITH2_INCR_42  43
  #define ARITH2_INCR_43  44
  #define ARITH2_INCR_44  45
  #define ARITH2_INCR_45  46
  #define ARITH2_INCR_46  47
  #define ARITH2_INCR_47  48
  #define ARITH2_INCR_48  49
  #define ARITH2_INCR_49  50
  #define ARITH2_INCR_50  51
  #define ARITH2_INCR_51  52
  #define ARITH2_INCR_52  53
  #define ARITH2_INCR_53  54
  #define ARITH2_INCR_54  55
  #define ARITH2_INCR_55  56
  #define ARITH2_INCR_56  57
  #define ARITH2_INCR_57  58
  #define ARITH2_INCR_58  59
  #define ARITH2_INCR_59  60
  #define ARITH2_INCR_60  61
  #define ARITH2_INCR_61  62
  #define ARITH2_INCR_62  63
  #define ARITH2_INCR_63  64
  #define ARITH2_INCR_64  65
  #define ARITH2_INCR_65  66
  #define ARITH2_INCR_66  67
  #define ARITH2_INCR_67  68
  #define ARITH2_INCR_68  69
  #define ARITH2_INCR_69  70
  #define ARITH2_INCR_70  71
  #define ARITH2_INCR_71  72
  #define ARITH2_INCR_72  73
  #define ARITH2_INCR_73  74
  #define ARITH2_INCR_74  75
  #define ARITH2_INCR_75  76
  #define ARITH2_INCR_76  77
  #define ARITH2_INCR_77  78
  #define ARITH2_INCR_78  79
  #define ARITH2_INCR_79  80
  #define ARITH2_INCR_80  81
  #define ARITH2_INCR_81  82
  #define ARITH2_INCR_82  83
  #define ARITH2_INCR_83  84
  #define ARITH2_INCR_84  85
  #define ARITH2_INCR_85  86
  #define ARITH2_INCR_86  87
  #define ARITH2_INCR_87  88
  #define ARITH2_INCR_88  89
  #define ARITH2_INCR_89  90
  #define ARITH2_INCR_90  91
  #define ARITH2_INCR_91  92
  #define ARITH2_INCR_92  93
  #define ARITH2_INCR_93  94
  #define ARITH2_INCR_94  95
  #define ARITH2_INCR_95  96
  #define ARITH2_INCR_96  97
  #define ARITH2_INCR_97  98
  #define ARITH2_INCR_98  99
  #define ARITH2_INCR_99  100
  #define ARITH2_INCR_100  101
  #define ARITH2_INCR_101  102
  #define ARITH2_INCR_102  103
  #define ARITH2_INCR_103  104
  #define ARITH2_INCR_104  105
  #define ARITH2_INCR_105  106
  #define ARITH2_INCR_106  107
  #define ARITH2_INCR_107  108
  #define ARITH2_INCR_108  109
  #define ARITH2_INCR_109  110
  #define ARITH2_INCR_110  111
  #define ARITH2_INCR_111  112
  #define ARITH2_INCR_112  113
  #define ARITH2_INCR_113  114
  #define ARITH2_INCR_114  115
  #define ARITH2_INCR_115  116
  #define ARITH2_INCR_116  117
  #define ARITH2_INCR_117  118
  #define ARITH2_INCR_118  119
  #define ARITH2_INCR_119  120
  #define ARITH2_INCR_120  121
  #define ARITH2_INCR_121  122
  #define ARITH2_INCR_122  123
  #define ARITH2_INCR_123  124
  #define ARITH2_INCR_124  125
  #define ARITH2_INCR_125  126
  #define ARITH2_INCR_126  127
  #define ARITH2_INCR_127  128
  #define ARITH2_INCR_128  129
  #define ARITH2_INCR_129  130
  #define ARITH2_INCR_130  131
  #define ARITH2_INCR_131  132
  #define ARITH2_INCR_132  133
  #define ARITH2_INCR_133  134
  #define ARITH2_INCR_134  135
  #define ARITH2_INCR_135  136
  #define ARITH2_INCR_136  137
  #define ARITH2_INCR_137  138
  #define ARITH2_INCR_138  139
  #define ARITH2_INCR_139  140
  #define ARITH2_INCR_140  141
  #define ARITH2_INCR_141  142
  #define ARITH2_INCR_142  143
  #define ARITH2_INCR_143  144
  #define ARITH2_INCR_144  145
  #define ARITH2_INCR_145  146
  #define ARITH2_INCR_146  147
  #define ARITH2_INCR_147  148
  #define ARITH2_INCR_148  149
  #define ARITH2_INCR_149  150
  #define ARITH2_INCR_150  151
  #define ARITH2_INCR_151  152
  #define ARITH2_INCR_152  153
  #define ARITH2_INCR_153  154
  #define ARITH2_INCR_154  155
  #define ARITH2_INCR_155  156
  #define ARITH2_INCR_156  157
  #define ARITH2_INCR_157  158
  #define ARITH2_INCR_158  159
  #define ARITH2_INCR_159  160
  #define ARITH2_INCR_160  161
  #define ARITH2_INCR_161  162
  #define ARITH2_INCR_162  163
  #define ARITH2_INCR_163  164
  #define ARITH2_INCR_164  165
  #define ARITH2_INCR_165  166
  #define ARITH2_INCR_166  167
  #define ARITH2_INCR_167  168
  #define ARITH2_INCR_168  169
  #define ARITH2_INCR_169  170
  #define ARITH2_INCR_170  171
  #define ARITH2_INCR_171  172
  #define ARITH2_INCR_172  173
  #define ARITH2_INCR_173  174
  #define ARITH2_INCR_174  175
  #define ARITH2_INCR_175  176
  #define ARITH2_INCR_176  177
  #define ARITH2_INCR_177  178
  #define ARITH2_INCR_178  179
  #define ARITH2_INCR_179  180
  #define ARITH2_INCR_180  181
  #define ARITH2_INCR_181  182
  #define ARITH2_INCR_182  183
  #define ARITH2_INCR_183  184
  #define ARITH2_INCR_184  185
  #define ARITH2_INCR_185  186
  #define ARITH2_INCR_186  187
  #define ARITH2_INCR_187  188
  #define ARITH2_INCR_188  189
  #define ARITH2_INCR_189  190
  #define ARITH2_INCR_190  191
  #define ARITH2_INCR_191  192
  #define ARITH2_INCR_192  193
  #define ARITH2_INCR_193  194
  #define ARITH2_INCR_194  195
  #define ARITH2_INCR_195  196
  #define ARITH2_INCR_196  197
  #define ARITH2_INCR_197  198
  #define ARITH2_INCR_198  199
  #define ARITH2_INCR_199  200
  #define ARITH2_INCR_200  201
  #define ARITH2_INCR_201  202
  #define ARITH2_INCR_202  203
  #define ARITH2_INCR_203  204
  #define ARITH2_INCR_204  205
  #define ARITH2_INCR_205  206
  #define ARITH2_INCR_206  207
  #define ARITH2_INCR_207  208
  #define ARITH2_INCR_208  209
  #define ARITH2_INCR_209  210
  #define ARITH2_INCR_210  211
  #define ARITH2_INCR_211  212
  #define ARITH2_INCR_212  213
  #define ARITH2_INCR_213  214
  #define ARITH2_INCR_214  215
  #define ARITH2_INCR_215  216
  #define ARITH2_INCR_216  217
  #define ARITH2_INCR_217  218
  #define ARITH2_INCR_218  219
  #define ARITH2_INCR_219  220
  #define ARITH2_INCR_220  221
  #define ARITH2_INCR_221  222
  #define ARITH2_INCR_222  223
  #define ARITH2_INCR_223  224
  #define ARITH2_INCR_224  225
  #define ARITH2_INCR_225  226
  #define ARITH2_INCR_226  227
  #define ARITH2_INCR_227  228
  #define ARITH2_INCR_228  229
  #define ARITH2_INCR_229  230
  #define ARITH2_INCR_230  231
  #define ARITH2_INCR_231  232
  #define ARITH2_INCR_232  233
  #define ARITH2_INCR_233  234
  #define ARITH2_INCR_234  235
  #define ARITH2_INCR_235  236
  #define ARITH2_INCR_236  237
  #define ARITH2_INCR_237  238
  #define ARITH2_INCR_238  239
  #define ARITH2_INCR_239  240
  #define ARITH2_INCR_240  241
  #define ARITH2_INCR_241  242
  #define ARITH2_INCR_242  243
  #define ARITH2_INCR_243  244
  #define ARITH2_INCR_244  245
  #define ARITH2_INCR_245  246
  #define ARITH2_INCR_246  247
  #define ARITH2_INCR_247  248
  #define ARITH2_INCR_248  249
  #define ARITH2_INCR_249  250
  #define ARITH2_INCR_250  251
  #define ARITH2_INCR_251  252
  #define ARITH2_INCR_252  253
  #define ARITH2_INCR_253  254
  #define ARITH2_INCR_254  255
  #define ARITH2_INCR_255  256
 
  #define ARITH2_DECR_1    0
  #define ARITH2_DECR_2    1
  #define ARITH2_DECR_3    2
  #define ARITH2_DECR_4    3
  #define ARITH2_DECR_5    4
  #define ARITH2_DECR_6    5
  #define ARITH2_DECR_7    6
  #define ARITH2_DECR_8    7
  #define ARITH2_DECR_9    8
  #define ARITH2_DECR_10   9
  #define ARITH2_DECR_11  10
  #define ARITH2_DECR_12  11
  #define ARITH2_DECR_13  12
  #define ARITH2_DECR_14  13
  #define ARITH2_DECR_15  14
  #define ARITH2_DECR_16  15
  #define ARITH2_DECR_17  16
  #define ARITH2_DECR_18  17
  #define ARITH2_DECR_19  18
  #define ARITH2_DECR_20  19
  #define ARITH2_DECR_21  20
  #define ARITH2_DECR_22  21
  #define ARITH2_DECR_23  22
  #define ARITH2_DECR_24  23
  #define ARITH2_DECR_25  24
  #define ARITH2_DECR_26  25
  #define ARITH2_DECR_27  26
  #define ARITH2_DECR_28  27
  #define ARITH2_DECR_29  28
  #define ARITH2_DECR_30  29
  #define ARITH2_DECR_31  30
  #define ARITH2_DECR_32  31
  #define ARITH2_DECR_33  32
  #define ARITH2_DECR_34  33
  #define ARITH2_DECR_35  34
  #define ARITH2_DECR_36  35
  #define ARITH2_DECR_37  36
  #define ARITH2_DECR_38  37
  #define ARITH2_DECR_39  38
  #define ARITH2_DECR_40  39
  #define ARITH2_DECR_41  40
  #define ARITH2_DECR_42  41
  #define ARITH2_DECR_43  42
  #define ARITH2_DECR_44  43
  #define ARITH2_DECR_45  44
  #define ARITH2_DECR_46  45
  #define ARITH2_DECR_47  46
  #define ARITH2_DECR_48  47
  #define ARITH2_DECR_49  48
  #define ARITH2_DECR_50  49
  #define ARITH2_DECR_51  50
  #define ARITH2_DECR_52  51
  #define ARITH2_DECR_53  52
  #define ARITH2_DECR_54  53
  #define ARITH2_DECR_55  54
  #define ARITH2_DECR_56  55
  #define ARITH2_DECR_57  56
  #define ARITH2_DECR_58  57
  #define ARITH2_DECR_59  58
  #define ARITH2_DECR_60  59
  #define ARITH2_DECR_61  60
  #define ARITH2_DECR_62  61
  #define ARITH2_DECR_63  62
  #define ARITH2_DECR_64  63
  #define ARITH2_DECR_65  64
  #define ARITH2_DECR_66  65
  #define ARITH2_DECR_67  66
  #define ARITH2_DECR_68  67
  #define ARITH2_DECR_69  68
  #define ARITH2_DECR_70  69
  #define ARITH2_DECR_71  70
  #define ARITH2_DECR_72  71
  #define ARITH2_DECR_73  72
  #define ARITH2_DECR_74  73
  #define ARITH2_DECR_75  74
  #define ARITH2_DECR_76  75
  #define ARITH2_DECR_77  76
  #define ARITH2_DECR_78  77
  #define ARITH2_DECR_79  78
  #define ARITH2_DECR_80  79
  #define ARITH2_DECR_81  80
  #define ARITH2_DECR_82  81
  #define ARITH2_DECR_83  82
  #define ARITH2_DECR_84  83
  #define ARITH2_DECR_85  84
  #define ARITH2_DECR_86  85
  #define ARITH2_DECR_87  86
  #define ARITH2_DECR_88  87
  #define ARITH2_DECR_89  88
  #define ARITH2_DECR_90  89
  #define ARITH2_DECR_91  90
  #define ARITH2_DECR_92  91
  #define ARITH2_DECR_93  92
  #define ARITH2_DECR_94  93
  #define ARITH2_DECR_95  94
  #define ARITH2_DECR_96  95
  #define ARITH2_DECR_97  96
  #define ARITH2_DECR_98  97
  #define ARITH2_DECR_99  98
  #define ARITH2_DECR_100  99
  #define ARITH2_DECR_101  100
  #define ARITH2_DECR_102  101
  #define ARITH2_DECR_103  102
  #define ARITH2_DECR_104  103
  #define ARITH2_DECR_105  104
  #define ARITH2_DECR_106  105
  #define ARITH2_DECR_107  106
  #define ARITH2_DECR_108  107
  #define ARITH2_DECR_109  108
  #define ARITH2_DECR_110  109
  #define ARITH2_DECR_111  110
  #define ARITH2_DECR_112  111
  #define ARITH2_DECR_113  112
  #define ARITH2_DECR_114  113
  #define ARITH2_DECR_115  114
  #define ARITH2_DECR_116  115
  #define ARITH2_DECR_117  116
  #define ARITH2_DECR_118  117
  #define ARITH2_DECR_119  118
  #define ARITH2_DECR_120  119
  #define ARITH2_DECR_121  120
  #define ARITH2_DECR_122  121
  #define ARITH2_DECR_123  122
  #define ARITH2_DECR_124  123
  #define ARITH2_DECR_125  124
  #define ARITH2_DECR_126  125
  #define ARITH2_DECR_127  126
  #define ARITH2_DECR_128  127
  #define ARITH2_DECR_129  128
  #define ARITH2_DECR_130  129
  #define ARITH2_DECR_131  130
  #define ARITH2_DECR_132  131
  #define ARITH2_DECR_133  132
  #define ARITH2_DECR_134  133
  #define ARITH2_DECR_135  134
  #define ARITH2_DECR_136  135
  #define ARITH2_DECR_137  136
  #define ARITH2_DECR_138  137
  #define ARITH2_DECR_139  138
  #define ARITH2_DECR_140  139
  #define ARITH2_DECR_141  140
  #define ARITH2_DECR_142  141
  #define ARITH2_DECR_143  142
  #define ARITH2_DECR_144  143
  #define ARITH2_DECR_145  144
  #define ARITH2_DECR_146  145
  #define ARITH2_DECR_147  146
  #define ARITH2_DECR_148  147
  #define ARITH2_DECR_149  148
  #define ARITH2_DECR_150  149
  #define ARITH2_DECR_151  150
  #define ARITH2_DECR_152  151
  #define ARITH2_DECR_153  152
  #define ARITH2_DECR_154  153
  #define ARITH2_DECR_155  154
  #define ARITH2_DECR_156  155
  #define ARITH2_DECR_157  156
  #define ARITH2_DECR_158  157
  #define ARITH2_DECR_159  158
  #define ARITH2_DECR_160  159
  #define ARITH2_DECR_161  160
  #define ARITH2_DECR_162  161
  #define ARITH2_DECR_163  162
  #define ARITH2_DECR_164  163
  #define ARITH2_DECR_165  164
  #define ARITH2_DECR_166  165
  #define ARITH2_DECR_167  166
  #define ARITH2_DECR_168  167
  #define ARITH2_DECR_169  168
  #define ARITH2_DECR_170  169
  #define ARITH2_DECR_171  170
  #define ARITH2_DECR_172  171
  #define ARITH2_DECR_173  172
  #define ARITH2_DECR_174  173
  #define ARITH2_DECR_175  174
  #define ARITH2_DECR_176  175
  #define ARITH2_DECR_177  176
  #define ARITH2_DECR_178  177
  #define ARITH2_DECR_179  178
  #define ARITH2_DECR_180  179
  #define ARITH2_DECR_181  180
  #define ARITH2_DECR_182  181
  #define ARITH2_DECR_183  182
  #define ARITH2_DECR_184  183
  #define ARITH2_DECR_185  184
  #define ARITH2_DECR_186  185
  #define ARITH2_DECR_187  186
  #define ARITH2_DECR_188  187
  #define ARITH2_DECR_189  188
  #define ARITH2_DECR_190  189
  #define ARITH2_DECR_191  190
  #define ARITH2_DECR_192  191
  #define ARITH2_DECR_193  192
  #define ARITH2_DECR_194  193
  #define ARITH2_DECR_195  194
  #define ARITH2_DECR_196  195
  #define ARITH2_DECR_197  196
  #define ARITH2_DECR_198  197
  #define ARITH2_DECR_199  198
  #define ARITH2_DECR_200  199
  #define ARITH2_DECR_201  200
  #define ARITH2_DECR_202  201
  #define ARITH2_DECR_203  202
  #define ARITH2_DECR_204  203
  #define ARITH2_DECR_205  204
  #define ARITH2_DECR_206  205
  #define ARITH2_DECR_207  206
  #define ARITH2_DECR_208  207
  #define ARITH2_DECR_209  208
  #define ARITH2_DECR_210  209
  #define ARITH2_DECR_211  210
  #define ARITH2_DECR_212  211
  #define ARITH2_DECR_213  212
  #define ARITH2_DECR_214  213
  #define ARITH2_DECR_215  214
  #define ARITH2_DECR_216  215
  #define ARITH2_DECR_217  216
  #define ARITH2_DECR_218  217
  #define ARITH2_DECR_219  218
  #define ARITH2_DECR_220  219
  #define ARITH2_DECR_221  220
  #define ARITH2_DECR_222  221
  #define ARITH2_DECR_223  222
  #define ARITH2_DECR_224  223
  #define ARITH2_DECR_225  224
  #define ARITH2_DECR_226  225
  #define ARITH2_DECR_227  226
  #define ARITH2_DECR_228  227
  #define ARITH2_DECR_229  228
  #define ARITH2_DECR_230  229
  #define ARITH2_DECR_231  230
  #define ARITH2_DECR_232  231
  #define ARITH2_DECR_233  232
  #define ARITH2_DECR_234  233
  #define ARITH2_DECR_235  234
  #define ARITH2_DECR_236  235
  #define ARITH2_DECR_237  236
  #define ARITH2_DECR_238  237
  #define ARITH2_DECR_239  238
  #define ARITH2_DECR_240  239
  #define ARITH2_DECR_241  240
  #define ARITH2_DECR_242  241
  #define ARITH2_DECR_243  242
  #define ARITH2_DECR_244  243
  #define ARITH2_DECR_245  244
  #define ARITH2_DECR_246  245
  #define ARITH2_DECR_247  246
  #define ARITH2_DECR_248  247
  #define ARITH2_DECR_249  248
  #define ARITH2_DECR_250  249
  #define ARITH2_DECR_251  250
  #define ARITH2_DECR_252  251
  #define ARITH2_DECR_253  252
  #define ARITH2_DECR_254  253
  #define ARITH2_DECR_255  254
  #define ARITH2_DECR_256  255
 
 
  #define ARITH2_ADD_1(fn1,fn2,n) ARITH2_ADD_0(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_2(fn1,fn2,n) ARITH2_ADD_1(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_3(fn1,fn2,n) ARITH2_ADD_2(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_4(fn1,fn2,n) ARITH2_ADD_3(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_5(fn1,fn2,n) ARITH2_ADD_4(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_6(fn1,fn2,n) ARITH2_ADD_5(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_7(fn1,fn2,n) ARITH2_ADD_6(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_8(fn1,fn2,n) ARITH2_ADD_7(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_9(fn1,fn2,n) ARITH2_ADD_8(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_10(fn1,fn2,n) ARITH2_ADD_9(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_11(fn1,fn2,n) ARITH2_ADD_10(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_12(fn1,fn2,n) ARITH2_ADD_11(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_13(fn1,fn2,n) ARITH2_ADD_12(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_14(fn1,fn2,n) ARITH2_ADD_13(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_15(fn1,fn2,n) ARITH2_ADD_14(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_16(fn1,fn2,n) ARITH2_ADD_15(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_17(fn1,fn2,n) ARITH2_ADD_16(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_18(fn1,fn2,n) ARITH2_ADD_17(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_19(fn1,fn2,n) ARITH2_ADD_18(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_20(fn1,fn2,n) ARITH2_ADD_19(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_21(fn1,fn2,n) ARITH2_ADD_20(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_22(fn1,fn2,n) ARITH2_ADD_21(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_23(fn1,fn2,n) ARITH2_ADD_22(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_24(fn1,fn2,n) ARITH2_ADD_23(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_25(fn1,fn2,n) ARITH2_ADD_24(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_26(fn1,fn2,n) ARITH2_ADD_25(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_27(fn1,fn2,n) ARITH2_ADD_26(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_28(fn1,fn2,n) ARITH2_ADD_27(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_29(fn1,fn2,n) ARITH2_ADD_28(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_30(fn1,fn2,n) ARITH2_ADD_29(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_31(fn1,fn2,n) ARITH2_ADD_30(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_32(fn1,fn2,n) ARITH2_ADD_31(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_33(fn1,fn2,n) ARITH2_ADD_32(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_34(fn1,fn2,n) ARITH2_ADD_33(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_35(fn1,fn2,n) ARITH2_ADD_34(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_36(fn1,fn2,n) ARITH2_ADD_35(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_37(fn1,fn2,n) ARITH2_ADD_36(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_38(fn1,fn2,n) ARITH2_ADD_37(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_39(fn1,fn2,n) ARITH2_ADD_38(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_40(fn1,fn2,n) ARITH2_ADD_39(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_41(fn1,fn2,n) ARITH2_ADD_40(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_42(fn1,fn2,n) ARITH2_ADD_41(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_43(fn1,fn2,n) ARITH2_ADD_42(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_44(fn1,fn2,n) ARITH2_ADD_43(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_45(fn1,fn2,n) ARITH2_ADD_44(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_46(fn1,fn2,n) ARITH2_ADD_45(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_47(fn1,fn2,n) ARITH2_ADD_46(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_48(fn1,fn2,n) ARITH2_ADD_47(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_49(fn1,fn2,n) ARITH2_ADD_48(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_50(fn1,fn2,n) ARITH2_ADD_49(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_51(fn1,fn2,n) ARITH2_ADD_50(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_52(fn1,fn2,n) ARITH2_ADD_51(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_53(fn1,fn2,n) ARITH2_ADD_52(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_54(fn1,fn2,n) ARITH2_ADD_53(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_55(fn1,fn2,n) ARITH2_ADD_54(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_56(fn1,fn2,n) ARITH2_ADD_55(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_57(fn1,fn2,n) ARITH2_ADD_56(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_58(fn1,fn2,n) ARITH2_ADD_57(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_59(fn1,fn2,n) ARITH2_ADD_58(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_60(fn1,fn2,n) ARITH2_ADD_59(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_61(fn1,fn2,n) ARITH2_ADD_60(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_62(fn1,fn2,n) ARITH2_ADD_61(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_63(fn1,fn2,n) ARITH2_ADD_62(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_64(fn1,fn2,n) ARITH2_ADD_63(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_65(fn1,fn2,n) ARITH2_ADD_64(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_66(fn1,fn2,n) ARITH2_ADD_65(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_67(fn1,fn2,n) ARITH2_ADD_66(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_68(fn1,fn2,n) ARITH2_ADD_67(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_69(fn1,fn2,n) ARITH2_ADD_68(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_70(fn1,fn2,n) ARITH2_ADD_69(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_71(fn1,fn2,n) ARITH2_ADD_70(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_72(fn1,fn2,n) ARITH2_ADD_71(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_73(fn1,fn2,n) ARITH2_ADD_72(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_74(fn1,fn2,n) ARITH2_ADD_73(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_75(fn1,fn2,n) ARITH2_ADD_74(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_76(fn1,fn2,n) ARITH2_ADD_75(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_77(fn1,fn2,n) ARITH2_ADD_76(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_78(fn1,fn2,n) ARITH2_ADD_77(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_79(fn1,fn2,n) ARITH2_ADD_78(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_80(fn1,fn2,n) ARITH2_ADD_79(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_81(fn1,fn2,n) ARITH2_ADD_80(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_82(fn1,fn2,n) ARITH2_ADD_81(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_83(fn1,fn2,n) ARITH2_ADD_82(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_84(fn1,fn2,n) ARITH2_ADD_83(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_85(fn1,fn2,n) ARITH2_ADD_84(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_86(fn1,fn2,n) ARITH2_ADD_85(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_87(fn1,fn2,n) ARITH2_ADD_86(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_88(fn1,fn2,n) ARITH2_ADD_87(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_89(fn1,fn2,n) ARITH2_ADD_88(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_90(fn1,fn2,n) ARITH2_ADD_89(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_91(fn1,fn2,n) ARITH2_ADD_90(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_92(fn1,fn2,n) ARITH2_ADD_91(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_93(fn1,fn2,n) ARITH2_ADD_92(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_94(fn1,fn2,n) ARITH2_ADD_93(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_95(fn1,fn2,n) ARITH2_ADD_94(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_96(fn1,fn2,n) ARITH2_ADD_95(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_97(fn1,fn2,n) ARITH2_ADD_96(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_98(fn1,fn2,n) ARITH2_ADD_97(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_99(fn1,fn2,n) ARITH2_ADD_98(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_100(fn1,fn2,n) ARITH2_ADD_99(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_101(fn1,fn2,n) ARITH2_ADD_100(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_102(fn1,fn2,n) ARITH2_ADD_101(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_103(fn1,fn2,n) ARITH2_ADD_102(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_104(fn1,fn2,n) ARITH2_ADD_103(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_105(fn1,fn2,n) ARITH2_ADD_104(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_106(fn1,fn2,n) ARITH2_ADD_105(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_107(fn1,fn2,n) ARITH2_ADD_106(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_108(fn1,fn2,n) ARITH2_ADD_107(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_109(fn1,fn2,n) ARITH2_ADD_108(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_110(fn1,fn2,n) ARITH2_ADD_109(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_111(fn1,fn2,n) ARITH2_ADD_110(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_112(fn1,fn2,n) ARITH2_ADD_111(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_113(fn1,fn2,n) ARITH2_ADD_112(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_114(fn1,fn2,n) ARITH2_ADD_113(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_115(fn1,fn2,n) ARITH2_ADD_114(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_116(fn1,fn2,n) ARITH2_ADD_115(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_117(fn1,fn2,n) ARITH2_ADD_116(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_118(fn1,fn2,n) ARITH2_ADD_117(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_119(fn1,fn2,n) ARITH2_ADD_118(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_120(fn1,fn2,n) ARITH2_ADD_119(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_121(fn1,fn2,n) ARITH2_ADD_120(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_122(fn1,fn2,n) ARITH2_ADD_121(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_123(fn1,fn2,n) ARITH2_ADD_122(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_124(fn1,fn2,n) ARITH2_ADD_123(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_125(fn1,fn2,n) ARITH2_ADD_124(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_126(fn1,fn2,n) ARITH2_ADD_125(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_127(fn1,fn2,n) ARITH2_ADD_126(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_128(fn1,fn2,n) ARITH2_ADD_127(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_129(fn1,fn2,n) ARITH2_ADD_128(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_130(fn1,fn2,n) ARITH2_ADD_129(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_131(fn1,fn2,n) ARITH2_ADD_130(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_132(fn1,fn2,n) ARITH2_ADD_131(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_133(fn1,fn2,n) ARITH2_ADD_132(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_134(fn1,fn2,n) ARITH2_ADD_133(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_135(fn1,fn2,n) ARITH2_ADD_134(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_136(fn1,fn2,n) ARITH2_ADD_135(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_137(fn1,fn2,n) ARITH2_ADD_136(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_138(fn1,fn2,n) ARITH2_ADD_137(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_139(fn1,fn2,n) ARITH2_ADD_138(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_140(fn1,fn2,n) ARITH2_ADD_139(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_141(fn1,fn2,n) ARITH2_ADD_140(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_142(fn1,fn2,n) ARITH2_ADD_141(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_143(fn1,fn2,n) ARITH2_ADD_142(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_144(fn1,fn2,n) ARITH2_ADD_143(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_145(fn1,fn2,n) ARITH2_ADD_144(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_146(fn1,fn2,n) ARITH2_ADD_145(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_147(fn1,fn2,n) ARITH2_ADD_146(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_148(fn1,fn2,n) ARITH2_ADD_147(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_149(fn1,fn2,n) ARITH2_ADD_148(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_150(fn1,fn2,n) ARITH2_ADD_149(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_151(fn1,fn2,n) ARITH2_ADD_150(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_152(fn1,fn2,n) ARITH2_ADD_151(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_153(fn1,fn2,n) ARITH2_ADD_152(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_154(fn1,fn2,n) ARITH2_ADD_153(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_155(fn1,fn2,n) ARITH2_ADD_154(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_156(fn1,fn2,n) ARITH2_ADD_155(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_157(fn1,fn2,n) ARITH2_ADD_156(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_158(fn1,fn2,n) ARITH2_ADD_157(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_159(fn1,fn2,n) ARITH2_ADD_158(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_160(fn1,fn2,n) ARITH2_ADD_159(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_161(fn1,fn2,n) ARITH2_ADD_160(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_162(fn1,fn2,n) ARITH2_ADD_161(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_163(fn1,fn2,n) ARITH2_ADD_162(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_164(fn1,fn2,n) ARITH2_ADD_163(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_165(fn1,fn2,n) ARITH2_ADD_164(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_166(fn1,fn2,n) ARITH2_ADD_165(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_167(fn1,fn2,n) ARITH2_ADD_166(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_168(fn1,fn2,n) ARITH2_ADD_167(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_169(fn1,fn2,n) ARITH2_ADD_168(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_170(fn1,fn2,n) ARITH2_ADD_169(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_171(fn1,fn2,n) ARITH2_ADD_170(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_172(fn1,fn2,n) ARITH2_ADD_171(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_173(fn1,fn2,n) ARITH2_ADD_172(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_174(fn1,fn2,n) ARITH2_ADD_173(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_175(fn1,fn2,n) ARITH2_ADD_174(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_176(fn1,fn2,n) ARITH2_ADD_175(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_177(fn1,fn2,n) ARITH2_ADD_176(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_178(fn1,fn2,n) ARITH2_ADD_177(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_179(fn1,fn2,n) ARITH2_ADD_178(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_180(fn1,fn2,n) ARITH2_ADD_179(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_181(fn1,fn2,n) ARITH2_ADD_180(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_182(fn1,fn2,n) ARITH2_ADD_181(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_183(fn1,fn2,n) ARITH2_ADD_182(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_184(fn1,fn2,n) ARITH2_ADD_183(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_185(fn1,fn2,n) ARITH2_ADD_184(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_186(fn1,fn2,n) ARITH2_ADD_185(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_187(fn1,fn2,n) ARITH2_ADD_186(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_188(fn1,fn2,n) ARITH2_ADD_187(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_189(fn1,fn2,n) ARITH2_ADD_188(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_190(fn1,fn2,n) ARITH2_ADD_189(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_191(fn1,fn2,n) ARITH2_ADD_190(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_192(fn1,fn2,n) ARITH2_ADD_191(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_193(fn1,fn2,n) ARITH2_ADD_192(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_194(fn1,fn2,n) ARITH2_ADD_193(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_195(fn1,fn2,n) ARITH2_ADD_194(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_196(fn1,fn2,n) ARITH2_ADD_195(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_197(fn1,fn2,n) ARITH2_ADD_196(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_198(fn1,fn2,n) ARITH2_ADD_197(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_199(fn1,fn2,n) ARITH2_ADD_198(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_200(fn1,fn2,n) ARITH2_ADD_199(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_201(fn1,fn2,n) ARITH2_ADD_200(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_202(fn1,fn2,n) ARITH2_ADD_201(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_203(fn1,fn2,n) ARITH2_ADD_202(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_204(fn1,fn2,n) ARITH2_ADD_203(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_205(fn1,fn2,n) ARITH2_ADD_204(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_206(fn1,fn2,n) ARITH2_ADD_205(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_207(fn1,fn2,n) ARITH2_ADD_206(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_208(fn1,fn2,n) ARITH2_ADD_207(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_209(fn1,fn2,n) ARITH2_ADD_208(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_210(fn1,fn2,n) ARITH2_ADD_209(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_211(fn1,fn2,n) ARITH2_ADD_210(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_212(fn1,fn2,n) ARITH2_ADD_211(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_213(fn1,fn2,n) ARITH2_ADD_212(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_214(fn1,fn2,n) ARITH2_ADD_213(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_215(fn1,fn2,n) ARITH2_ADD_214(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_216(fn1,fn2,n) ARITH2_ADD_215(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_217(fn1,fn2,n) ARITH2_ADD_216(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_218(fn1,fn2,n) ARITH2_ADD_217(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_219(fn1,fn2,n) ARITH2_ADD_218(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_220(fn1,fn2,n) ARITH2_ADD_219(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_221(fn1,fn2,n) ARITH2_ADD_220(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_222(fn1,fn2,n) ARITH2_ADD_221(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_223(fn1,fn2,n) ARITH2_ADD_222(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_224(fn1,fn2,n) ARITH2_ADD_223(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_225(fn1,fn2,n) ARITH2_ADD_224(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_226(fn1,fn2,n) ARITH2_ADD_225(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_227(fn1,fn2,n) ARITH2_ADD_226(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_228(fn1,fn2,n) ARITH2_ADD_227(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_229(fn1,fn2,n) ARITH2_ADD_228(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_230(fn1,fn2,n) ARITH2_ADD_229(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_231(fn1,fn2,n) ARITH2_ADD_230(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_232(fn1,fn2,n) ARITH2_ADD_231(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_233(fn1,fn2,n) ARITH2_ADD_232(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_234(fn1,fn2,n) ARITH2_ADD_233(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_235(fn1,fn2,n) ARITH2_ADD_234(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_236(fn1,fn2,n) ARITH2_ADD_235(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_237(fn1,fn2,n) ARITH2_ADD_236(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_238(fn1,fn2,n) ARITH2_ADD_237(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_239(fn1,fn2,n) ARITH2_ADD_238(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_240(fn1,fn2,n) ARITH2_ADD_239(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_241(fn1,fn2,n) ARITH2_ADD_240(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_242(fn1,fn2,n) ARITH2_ADD_241(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_243(fn1,fn2,n) ARITH2_ADD_242(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_244(fn1,fn2,n) ARITH2_ADD_243(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_245(fn1,fn2,n) ARITH2_ADD_244(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_246(fn1,fn2,n) ARITH2_ADD_245(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_247(fn1,fn2,n) ARITH2_ADD_246(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_248(fn1,fn2,n) ARITH2_ADD_247(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_249(fn1,fn2,n) ARITH2_ADD_248(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_250(fn1,fn2,n) ARITH2_ADD_249(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_251(fn1,fn2,n) ARITH2_ADD_250(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_252(fn1,fn2,n) ARITH2_ADD_251(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_253(fn1,fn2,n) ARITH2_ADD_252(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_254(fn1,fn2,n) ARITH2_ADD_253(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_255(fn1,fn2,n) ARITH2_ADD_254(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
  #define ARITH2_ADD_256(fn1,fn2,n) ARITH2_ADD_255(fn1,fn2,ARITH2_INCR(n)) fn1(fn2,n,)
 
  #define ARITH2_SUB_1(fn1,fn2,n,rep...) ARITH2_SUB_0(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_2(fn1,fn2,n,rep...) ARITH2_SUB_1(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_3(fn1,fn2,n,rep...) ARITH2_SUB_2(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_4(fn1,fn2,n,rep...) ARITH2_SUB_3(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_5(fn1,fn2,n,rep...) ARITH2_SUB_4(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_6(fn1,fn2,n,rep...) ARITH2_SUB_5(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_7(fn1,fn2,n,rep...) ARITH2_SUB_6(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_8(fn1,fn2,n,rep...) ARITH2_SUB_7(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_9(fn1,fn2,n,rep...) ARITH2_SUB_8(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_10(fn1,fn2,n,rep...) ARITH2_SUB_9(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_11(fn1,fn2,n,rep...) ARITH2_SUB_10(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_12(fn1,fn2,n,rep...) ARITH2_SUB_11(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_13(fn1,fn2,n,rep...) ARITH2_SUB_12(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_14(fn1,fn2,n,rep...) ARITH2_SUB_13(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_15(fn1,fn2,n,rep...) ARITH2_SUB_14(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_16(fn1,fn2,n,rep...) ARITH2_SUB_15(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_17(fn1,fn2,n,rep...) ARITH2_SUB_16(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_18(fn1,fn2,n,rep...) ARITH2_SUB_17(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_19(fn1,fn2,n,rep...) ARITH2_SUB_18(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_20(fn1,fn2,n,rep...) ARITH2_SUB_19(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_21(fn1,fn2,n,rep...) ARITH2_SUB_20(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_22(fn1,fn2,n,rep...) ARITH2_SUB_21(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_23(fn1,fn2,n,rep...) ARITH2_SUB_22(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_24(fn1,fn2,n,rep...) ARITH2_SUB_23(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_25(fn1,fn2,n,rep...) ARITH2_SUB_24(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_26(fn1,fn2,n,rep...) ARITH2_SUB_25(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_27(fn1,fn2,n,rep...) ARITH2_SUB_26(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_28(fn1,fn2,n,rep...) ARITH2_SUB_27(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_29(fn1,fn2,n,rep...) ARITH2_SUB_28(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_30(fn1,fn2,n,rep...) ARITH2_SUB_29(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_31(fn1,fn2,n,rep...) ARITH2_SUB_30(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_32(fn1,fn2,n,rep...) ARITH2_SUB_31(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_33(fn1,fn2,n,rep...) ARITH2_SUB_32(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_34(fn1,fn2,n,rep...) ARITH2_SUB_33(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_35(fn1,fn2,n,rep...) ARITH2_SUB_34(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_36(fn1,fn2,n,rep...) ARITH2_SUB_35(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_37(fn1,fn2,n,rep...) ARITH2_SUB_36(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_38(fn1,fn2,n,rep...) ARITH2_SUB_37(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_39(fn1,fn2,n,rep...) ARITH2_SUB_38(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_40(fn1,fn2,n,rep...) ARITH2_SUB_39(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_41(fn1,fn2,n,rep...) ARITH2_SUB_40(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_42(fn1,fn2,n,rep...) ARITH2_SUB_41(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_43(fn1,fn2,n,rep...) ARITH2_SUB_42(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_44(fn1,fn2,n,rep...) ARITH2_SUB_43(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_45(fn1,fn2,n,rep...) ARITH2_SUB_44(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_46(fn1,fn2,n,rep...) ARITH2_SUB_45(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_47(fn1,fn2,n,rep...) ARITH2_SUB_46(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_48(fn1,fn2,n,rep...) ARITH2_SUB_47(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_49(fn1,fn2,n,rep...) ARITH2_SUB_48(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_50(fn1,fn2,n,rep...) ARITH2_SUB_49(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_51(fn1,fn2,n,rep...) ARITH2_SUB_50(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_52(fn1,fn2,n,rep...) ARITH2_SUB_51(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_53(fn1,fn2,n,rep...) ARITH2_SUB_52(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_54(fn1,fn2,n,rep...) ARITH2_SUB_53(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_55(fn1,fn2,n,rep...) ARITH2_SUB_54(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_56(fn1,fn2,n,rep...) ARITH2_SUB_55(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_57(fn1,fn2,n,rep...) ARITH2_SUB_56(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_58(fn1,fn2,n,rep...) ARITH2_SUB_57(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_59(fn1,fn2,n,rep...) ARITH2_SUB_58(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_60(fn1,fn2,n,rep...) ARITH2_SUB_59(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_61(fn1,fn2,n,rep...) ARITH2_SUB_60(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_62(fn1,fn2,n,rep...) ARITH2_SUB_61(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_63(fn1,fn2,n,rep...) ARITH2_SUB_62(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_64(fn1,fn2,n,rep...) ARITH2_SUB_63(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_65(fn1,fn2,n,rep...) ARITH2_SUB_64(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_66(fn1,fn2,n,rep...) ARITH2_SUB_65(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_67(fn1,fn2,n,rep...) ARITH2_SUB_66(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_68(fn1,fn2,n,rep...) ARITH2_SUB_67(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_69(fn1,fn2,n,rep...) ARITH2_SUB_68(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_70(fn1,fn2,n,rep...) ARITH2_SUB_69(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_71(fn1,fn2,n,rep...) ARITH2_SUB_70(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_72(fn1,fn2,n,rep...) ARITH2_SUB_71(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_73(fn1,fn2,n,rep...) ARITH2_SUB_72(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_74(fn1,fn2,n,rep...) ARITH2_SUB_73(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_75(fn1,fn2,n,rep...) ARITH2_SUB_74(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_76(fn1,fn2,n,rep...) ARITH2_SUB_75(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_77(fn1,fn2,n,rep...) ARITH2_SUB_76(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_78(fn1,fn2,n,rep...) ARITH2_SUB_77(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_79(fn1,fn2,n,rep...) ARITH2_SUB_78(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_80(fn1,fn2,n,rep...) ARITH2_SUB_79(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_81(fn1,fn2,n,rep...) ARITH2_SUB_80(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_82(fn1,fn2,n,rep...) ARITH2_SUB_81(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_83(fn1,fn2,n,rep...) ARITH2_SUB_82(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_84(fn1,fn2,n,rep...) ARITH2_SUB_83(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_85(fn1,fn2,n,rep...) ARITH2_SUB_84(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_86(fn1,fn2,n,rep...) ARITH2_SUB_85(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_87(fn1,fn2,n,rep...) ARITH2_SUB_86(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_88(fn1,fn2,n,rep...) ARITH2_SUB_87(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_89(fn1,fn2,n,rep...) ARITH2_SUB_88(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_90(fn1,fn2,n,rep...) ARITH2_SUB_89(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_91(fn1,fn2,n,rep...) ARITH2_SUB_90(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_92(fn1,fn2,n,rep...) ARITH2_SUB_91(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_93(fn1,fn2,n,rep...) ARITH2_SUB_92(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_94(fn1,fn2,n,rep...) ARITH2_SUB_93(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_95(fn1,fn2,n,rep...) ARITH2_SUB_94(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_96(fn1,fn2,n,rep...) ARITH2_SUB_95(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_97(fn1,fn2,n,rep...) ARITH2_SUB_96(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_98(fn1,fn2,n,rep...) ARITH2_SUB_97(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_99(fn1,fn2,n,rep...) ARITH2_SUB_98(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_100(fn1,fn2,n,rep...) ARITH2_SUB_99(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_101(fn1,fn2,n,rep...) ARITH2_SUB_100(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_102(fn1,fn2,n,rep...) ARITH2_SUB_101(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_103(fn1,fn2,n,rep...) ARITH2_SUB_102(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_104(fn1,fn2,n,rep...) ARITH2_SUB_103(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_105(fn1,fn2,n,rep...) ARITH2_SUB_104(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_106(fn1,fn2,n,rep...) ARITH2_SUB_105(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_107(fn1,fn2,n,rep...) ARITH2_SUB_106(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_108(fn1,fn2,n,rep...) ARITH2_SUB_107(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_109(fn1,fn2,n,rep...) ARITH2_SUB_108(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_110(fn1,fn2,n,rep...) ARITH2_SUB_109(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_111(fn1,fn2,n,rep...) ARITH2_SUB_110(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_112(fn1,fn2,n,rep...) ARITH2_SUB_111(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_113(fn1,fn2,n,rep...) ARITH2_SUB_112(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_114(fn1,fn2,n,rep...) ARITH2_SUB_113(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_115(fn1,fn2,n,rep...) ARITH2_SUB_114(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_116(fn1,fn2,n,rep...) ARITH2_SUB_115(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_117(fn1,fn2,n,rep...) ARITH2_SUB_116(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_118(fn1,fn2,n,rep...) ARITH2_SUB_117(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_119(fn1,fn2,n,rep...) ARITH2_SUB_118(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_120(fn1,fn2,n,rep...) ARITH2_SUB_119(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_121(fn1,fn2,n,rep...) ARITH2_SUB_120(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_122(fn1,fn2,n,rep...) ARITH2_SUB_121(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_123(fn1,fn2,n,rep...) ARITH2_SUB_122(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_124(fn1,fn2,n,rep...) ARITH2_SUB_123(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_125(fn1,fn2,n,rep...) ARITH2_SUB_124(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_126(fn1,fn2,n,rep...) ARITH2_SUB_125(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_127(fn1,fn2,n,rep...) ARITH2_SUB_126(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_128(fn1,fn2,n,rep...) ARITH2_SUB_127(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_129(fn1,fn2,n,rep...) ARITH2_SUB_128(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_130(fn1,fn2,n,rep...) ARITH2_SUB_129(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_131(fn1,fn2,n,rep...) ARITH2_SUB_130(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_132(fn1,fn2,n,rep...) ARITH2_SUB_131(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_133(fn1,fn2,n,rep...) ARITH2_SUB_132(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_134(fn1,fn2,n,rep...) ARITH2_SUB_133(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_135(fn1,fn2,n,rep...) ARITH2_SUB_134(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_136(fn1,fn2,n,rep...) ARITH2_SUB_135(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_137(fn1,fn2,n,rep...) ARITH2_SUB_136(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_138(fn1,fn2,n,rep...) ARITH2_SUB_137(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_139(fn1,fn2,n,rep...) ARITH2_SUB_138(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_140(fn1,fn2,n,rep...) ARITH2_SUB_139(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_141(fn1,fn2,n,rep...) ARITH2_SUB_140(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_142(fn1,fn2,n,rep...) ARITH2_SUB_141(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_143(fn1,fn2,n,rep...) ARITH2_SUB_142(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_144(fn1,fn2,n,rep...) ARITH2_SUB_143(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_145(fn1,fn2,n,rep...) ARITH2_SUB_144(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_146(fn1,fn2,n,rep...) ARITH2_SUB_145(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_147(fn1,fn2,n,rep...) ARITH2_SUB_146(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_148(fn1,fn2,n,rep...) ARITH2_SUB_147(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_149(fn1,fn2,n,rep...) ARITH2_SUB_148(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_150(fn1,fn2,n,rep...) ARITH2_SUB_149(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_151(fn1,fn2,n,rep...) ARITH2_SUB_150(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_152(fn1,fn2,n,rep...) ARITH2_SUB_151(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_153(fn1,fn2,n,rep...) ARITH2_SUB_152(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_154(fn1,fn2,n,rep...) ARITH2_SUB_153(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_155(fn1,fn2,n,rep...) ARITH2_SUB_154(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_156(fn1,fn2,n,rep...) ARITH2_SUB_155(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_157(fn1,fn2,n,rep...) ARITH2_SUB_156(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_158(fn1,fn2,n,rep...) ARITH2_SUB_157(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_159(fn1,fn2,n,rep...) ARITH2_SUB_158(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_160(fn1,fn2,n,rep...) ARITH2_SUB_159(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_161(fn1,fn2,n,rep...) ARITH2_SUB_160(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_162(fn1,fn2,n,rep...) ARITH2_SUB_161(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_163(fn1,fn2,n,rep...) ARITH2_SUB_162(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_164(fn1,fn2,n,rep...) ARITH2_SUB_163(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_165(fn1,fn2,n,rep...) ARITH2_SUB_164(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_166(fn1,fn2,n,rep...) ARITH2_SUB_165(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_167(fn1,fn2,n,rep...) ARITH2_SUB_166(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_168(fn1,fn2,n,rep...) ARITH2_SUB_167(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_169(fn1,fn2,n,rep...) ARITH2_SUB_168(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_170(fn1,fn2,n,rep...) ARITH2_SUB_169(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_171(fn1,fn2,n,rep...) ARITH2_SUB_170(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_172(fn1,fn2,n,rep...) ARITH2_SUB_171(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_173(fn1,fn2,n,rep...) ARITH2_SUB_172(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_174(fn1,fn2,n,rep...) ARITH2_SUB_173(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_175(fn1,fn2,n,rep...) ARITH2_SUB_174(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_176(fn1,fn2,n,rep...) ARITH2_SUB_175(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_177(fn1,fn2,n,rep...) ARITH2_SUB_176(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_178(fn1,fn2,n,rep...) ARITH2_SUB_177(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_179(fn1,fn2,n,rep...) ARITH2_SUB_178(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_180(fn1,fn2,n,rep...) ARITH2_SUB_179(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_181(fn1,fn2,n,rep...) ARITH2_SUB_180(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_182(fn1,fn2,n,rep...) ARITH2_SUB_181(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_183(fn1,fn2,n,rep...) ARITH2_SUB_182(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_184(fn1,fn2,n,rep...) ARITH2_SUB_183(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_185(fn1,fn2,n,rep...) ARITH2_SUB_184(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_186(fn1,fn2,n,rep...) ARITH2_SUB_185(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_187(fn1,fn2,n,rep...) ARITH2_SUB_186(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_188(fn1,fn2,n,rep...) ARITH2_SUB_187(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_189(fn1,fn2,n,rep...) ARITH2_SUB_188(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_190(fn1,fn2,n,rep...) ARITH2_SUB_189(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_191(fn1,fn2,n,rep...) ARITH2_SUB_190(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_192(fn1,fn2,n,rep...) ARITH2_SUB_191(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_193(fn1,fn2,n,rep...) ARITH2_SUB_192(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_194(fn1,fn2,n,rep...) ARITH2_SUB_193(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_195(fn1,fn2,n,rep...) ARITH2_SUB_194(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_196(fn1,fn2,n,rep...) ARITH2_SUB_195(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_197(fn1,fn2,n,rep...) ARITH2_SUB_196(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_198(fn1,fn2,n,rep...) ARITH2_SUB_197(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_199(fn1,fn2,n,rep...) ARITH2_SUB_198(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_200(fn1,fn2,n,rep...) ARITH2_SUB_199(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_201(fn1,fn2,n,rep...) ARITH2_SUB_200(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_202(fn1,fn2,n,rep...) ARITH2_SUB_201(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_203(fn1,fn2,n,rep...) ARITH2_SUB_202(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_204(fn1,fn2,n,rep...) ARITH2_SUB_203(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_205(fn1,fn2,n,rep...) ARITH2_SUB_204(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_206(fn1,fn2,n,rep...) ARITH2_SUB_205(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_207(fn1,fn2,n,rep...) ARITH2_SUB_206(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_208(fn1,fn2,n,rep...) ARITH2_SUB_207(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_209(fn1,fn2,n,rep...) ARITH2_SUB_208(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_210(fn1,fn2,n,rep...) ARITH2_SUB_209(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_211(fn1,fn2,n,rep...) ARITH2_SUB_210(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_212(fn1,fn2,n,rep...) ARITH2_SUB_211(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_213(fn1,fn2,n,rep...) ARITH2_SUB_212(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_214(fn1,fn2,n,rep...) ARITH2_SUB_213(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_215(fn1,fn2,n,rep...) ARITH2_SUB_214(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_216(fn1,fn2,n,rep...) ARITH2_SUB_215(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_217(fn1,fn2,n,rep...) ARITH2_SUB_216(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_218(fn1,fn2,n,rep...) ARITH2_SUB_217(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_219(fn1,fn2,n,rep...) ARITH2_SUB_218(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_220(fn1,fn2,n,rep...) ARITH2_SUB_219(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_221(fn1,fn2,n,rep...) ARITH2_SUB_220(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_222(fn1,fn2,n,rep...) ARITH2_SUB_221(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_223(fn1,fn2,n,rep...) ARITH2_SUB_222(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_224(fn1,fn2,n,rep...) ARITH2_SUB_223(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_225(fn1,fn2,n,rep...) ARITH2_SUB_224(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_226(fn1,fn2,n,rep...) ARITH2_SUB_225(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_227(fn1,fn2,n,rep...) ARITH2_SUB_226(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_228(fn1,fn2,n,rep...) ARITH2_SUB_227(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_229(fn1,fn2,n,rep...) ARITH2_SUB_228(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_230(fn1,fn2,n,rep...) ARITH2_SUB_229(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_231(fn1,fn2,n,rep...) ARITH2_SUB_230(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_232(fn1,fn2,n,rep...) ARITH2_SUB_231(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_233(fn1,fn2,n,rep...) ARITH2_SUB_232(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_234(fn1,fn2,n,rep...) ARITH2_SUB_233(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_235(fn1,fn2,n,rep...) ARITH2_SUB_234(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_236(fn1,fn2,n,rep...) ARITH2_SUB_235(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_237(fn1,fn2,n,rep...) ARITH2_SUB_236(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_238(fn1,fn2,n,rep...) ARITH2_SUB_237(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_239(fn1,fn2,n,rep...) ARITH2_SUB_238(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_240(fn1,fn2,n,rep...) ARITH2_SUB_239(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_241(fn1,fn2,n,rep...) ARITH2_SUB_240(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_242(fn1,fn2,n,rep...) ARITH2_SUB_241(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_243(fn1,fn2,n,rep...) ARITH2_SUB_242(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_244(fn1,fn2,n,rep...) ARITH2_SUB_243(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_245(fn1,fn2,n,rep...) ARITH2_SUB_244(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_246(fn1,fn2,n,rep...) ARITH2_SUB_245(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_247(fn1,fn2,n,rep...) ARITH2_SUB_246(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_248(fn1,fn2,n,rep...) ARITH2_SUB_247(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_249(fn1,fn2,n,rep...) ARITH2_SUB_248(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_250(fn1,fn2,n,rep...) ARITH2_SUB_249(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_251(fn1,fn2,n,rep...) ARITH2_SUB_250(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_252(fn1,fn2,n,rep...) ARITH2_SUB_251(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_253(fn1,fn2,n,rep...) ARITH2_SUB_252(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_254(fn1,fn2,n,rep...) ARITH2_SUB_253(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_255(fn1,fn2,n,rep...) ARITH2_SUB_254(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
  #define ARITH2_SUB_256(fn1,fn2,n,rep...) ARITH2_SUB_255(fn1,fn2,ARITH2_DECR(n),rep) fn1(fn2,n,rep)
 
  #define ARITH2_MUL_2(n,i) ARITH2_MUL_1( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_3(n,i) ARITH2_MUL_2( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_4(n,i) ARITH2_MUL_3( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_5(n,i) ARITH2_MUL_4( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_6(n,i) ARITH2_MUL_5( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_7(n,i) ARITH2_MUL_6( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_8(n,i) ARITH2_MUL_7( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_9(n,i) ARITH2_MUL_8( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_10(n,i) ARITH2_MUL_9( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_11(n,i) ARITH2_MUL_10( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_12(n,i) ARITH2_MUL_11( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_13(n,i) ARITH2_MUL_12( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_14(n,i) ARITH2_MUL_13( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_15(n,i) ARITH2_MUL_14( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_16(n,i) ARITH2_MUL_15( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_17(n,i) ARITH2_MUL_16( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_18(n,i) ARITH2_MUL_17( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_19(n,i) ARITH2_MUL_18( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_20(n,i) ARITH2_MUL_19( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_21(n,i) ARITH2_MUL_20( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_22(n,i) ARITH2_MUL_21( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_23(n,i) ARITH2_MUL_22( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_24(n,i) ARITH2_MUL_23( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_25(n,i) ARITH2_MUL_24( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_26(n,i) ARITH2_MUL_25( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_27(n,i) ARITH2_MUL_26( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_28(n,i) ARITH2_MUL_27( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_29(n,i) ARITH2_MUL_28( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_30(n,i) ARITH2_MUL_29( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_31(n,i) ARITH2_MUL_30( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_32(n,i) ARITH2_MUL_31( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_33(n,i) ARITH2_MUL_32( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_34(n,i) ARITH2_MUL_33( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_35(n,i) ARITH2_MUL_34( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_36(n,i) ARITH2_MUL_35( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_37(n,i) ARITH2_MUL_36( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_38(n,i) ARITH2_MUL_37( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_39(n,i) ARITH2_MUL_38( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_40(n,i) ARITH2_MUL_39( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_41(n,i) ARITH2_MUL_40( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_42(n,i) ARITH2_MUL_41( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_43(n,i) ARITH2_MUL_42( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_44(n,i) ARITH2_MUL_43( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_45(n,i) ARITH2_MUL_44( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_46(n,i) ARITH2_MUL_45( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_47(n,i) ARITH2_MUL_46( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_48(n,i) ARITH2_MUL_47( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_49(n,i) ARITH2_MUL_48( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_50(n,i) ARITH2_MUL_49( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_51(n,i) ARITH2_MUL_50( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_52(n,i) ARITH2_MUL_51( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_53(n,i) ARITH2_MUL_52( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_54(n,i) ARITH2_MUL_53( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_55(n,i) ARITH2_MUL_54( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_56(n,i) ARITH2_MUL_55( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_57(n,i) ARITH2_MUL_56( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_58(n,i) ARITH2_MUL_57( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_59(n,i) ARITH2_MUL_58( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_60(n,i) ARITH2_MUL_59( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_61(n,i) ARITH2_MUL_60( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_62(n,i) ARITH2_MUL_61( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_63(n,i) ARITH2_MUL_62( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_64(n,i) ARITH2_MUL_63( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_65(n,i) ARITH2_MUL_64( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_66(n,i) ARITH2_MUL_65( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_67(n,i) ARITH2_MUL_66( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_68(n,i) ARITH2_MUL_67( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_69(n,i) ARITH2_MUL_68( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_70(n,i) ARITH2_MUL_69( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_71(n,i) ARITH2_MUL_70( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_72(n,i) ARITH2_MUL_71( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_73(n,i) ARITH2_MUL_72( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_74(n,i) ARITH2_MUL_73( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_75(n,i) ARITH2_MUL_74( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_76(n,i) ARITH2_MUL_75( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_77(n,i) ARITH2_MUL_76( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_78(n,i) ARITH2_MUL_77( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_79(n,i) ARITH2_MUL_78( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_80(n,i) ARITH2_MUL_79( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_81(n,i) ARITH2_MUL_80( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_82(n,i) ARITH2_MUL_81( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_83(n,i) ARITH2_MUL_82( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_84(n,i) ARITH2_MUL_83( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_85(n,i) ARITH2_MUL_84( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_86(n,i) ARITH2_MUL_85( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_87(n,i) ARITH2_MUL_86( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_88(n,i) ARITH2_MUL_87( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_89(n,i) ARITH2_MUL_88( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_90(n,i) ARITH2_MUL_89( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_91(n,i) ARITH2_MUL_90( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_92(n,i) ARITH2_MUL_91( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_93(n,i) ARITH2_MUL_92( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_94(n,i) ARITH2_MUL_93( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_95(n,i) ARITH2_MUL_94( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_96(n,i) ARITH2_MUL_95( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_97(n,i) ARITH2_MUL_96( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_98(n,i) ARITH2_MUL_97( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_99(n,i) ARITH2_MUL_98( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_100(n,i) ARITH2_MUL_99( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_101(n,i) ARITH2_MUL_100( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_102(n,i) ARITH2_MUL_101( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_103(n,i) ARITH2_MUL_102( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_104(n,i) ARITH2_MUL_103( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_105(n,i) ARITH2_MUL_104( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_106(n,i) ARITH2_MUL_105( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_107(n,i) ARITH2_MUL_106( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_108(n,i) ARITH2_MUL_107( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_109(n,i) ARITH2_MUL_108( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_110(n,i) ARITH2_MUL_109( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_111(n,i) ARITH2_MUL_110( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_112(n,i) ARITH2_MUL_111( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_113(n,i) ARITH2_MUL_112( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_114(n,i) ARITH2_MUL_113( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_115(n,i) ARITH2_MUL_114( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_116(n,i) ARITH2_MUL_115( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_117(n,i) ARITH2_MUL_116( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_118(n,i) ARITH2_MUL_117( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_119(n,i) ARITH2_MUL_118( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_120(n,i) ARITH2_MUL_119( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_121(n,i) ARITH2_MUL_120( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_122(n,i) ARITH2_MUL_121( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_123(n,i) ARITH2_MUL_122( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_124(n,i) ARITH2_MUL_123( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_125(n,i) ARITH2_MUL_124( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_126(n,i) ARITH2_MUL_125( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_127(n,i) ARITH2_MUL_126( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_128(n,i) ARITH2_MUL_127( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_129(n,i) ARITH2_MUL_128( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_130(n,i) ARITH2_MUL_129( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_131(n,i) ARITH2_MUL_130( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_132(n,i) ARITH2_MUL_131( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_133(n,i) ARITH2_MUL_132( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_134(n,i) ARITH2_MUL_133( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_135(n,i) ARITH2_MUL_134( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_136(n,i) ARITH2_MUL_135( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_137(n,i) ARITH2_MUL_136( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_138(n,i) ARITH2_MUL_137( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_139(n,i) ARITH2_MUL_138( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_140(n,i) ARITH2_MUL_139( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_141(n,i) ARITH2_MUL_140( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_142(n,i) ARITH2_MUL_141( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_143(n,i) ARITH2_MUL_142( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_144(n,i) ARITH2_MUL_143( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_145(n,i) ARITH2_MUL_144( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_146(n,i) ARITH2_MUL_145( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_147(n,i) ARITH2_MUL_146( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_148(n,i) ARITH2_MUL_147( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_149(n,i) ARITH2_MUL_148( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_150(n,i) ARITH2_MUL_149( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_151(n,i) ARITH2_MUL_150( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_152(n,i) ARITH2_MUL_151( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_153(n,i) ARITH2_MUL_152( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_154(n,i) ARITH2_MUL_153( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_155(n,i) ARITH2_MUL_154( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_156(n,i) ARITH2_MUL_155( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_157(n,i) ARITH2_MUL_156( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_158(n,i) ARITH2_MUL_157( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_159(n,i) ARITH2_MUL_158( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_160(n,i) ARITH2_MUL_159( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_161(n,i) ARITH2_MUL_160( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_162(n,i) ARITH2_MUL_161( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_163(n,i) ARITH2_MUL_162( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_164(n,i) ARITH2_MUL_163( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_165(n,i) ARITH2_MUL_164( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_166(n,i) ARITH2_MUL_165( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_167(n,i) ARITH2_MUL_166( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_168(n,i) ARITH2_MUL_167( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_169(n,i) ARITH2_MUL_168( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_170(n,i) ARITH2_MUL_169( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_171(n,i) ARITH2_MUL_170( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_172(n,i) ARITH2_MUL_171( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_173(n,i) ARITH2_MUL_172( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_174(n,i) ARITH2_MUL_173( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_175(n,i) ARITH2_MUL_174( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_176(n,i) ARITH2_MUL_175( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_177(n,i) ARITH2_MUL_176( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_178(n,i) ARITH2_MUL_177( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_179(n,i) ARITH2_MUL_178( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_180(n,i) ARITH2_MUL_179( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_181(n,i) ARITH2_MUL_180( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_182(n,i) ARITH2_MUL_181( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_183(n,i) ARITH2_MUL_182( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_184(n,i) ARITH2_MUL_183( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_185(n,i) ARITH2_MUL_184( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_186(n,i) ARITH2_MUL_185( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_187(n,i) ARITH2_MUL_186( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_188(n,i) ARITH2_MUL_187( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_189(n,i) ARITH2_MUL_188( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_190(n,i) ARITH2_MUL_189( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_191(n,i) ARITH2_MUL_190( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_192(n,i) ARITH2_MUL_191( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_193(n,i) ARITH2_MUL_192( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_194(n,i) ARITH2_MUL_193( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_195(n,i) ARITH2_MUL_194( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_196(n,i) ARITH2_MUL_195( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_197(n,i) ARITH2_MUL_196( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_198(n,i) ARITH2_MUL_197( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_199(n,i) ARITH2_MUL_198( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_200(n,i) ARITH2_MUL_199( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_201(n,i) ARITH2_MUL_200( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_202(n,i) ARITH2_MUL_201( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_203(n,i) ARITH2_MUL_202( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_204(n,i) ARITH2_MUL_203( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_205(n,i) ARITH2_MUL_204( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_206(n,i) ARITH2_MUL_205( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_207(n,i) ARITH2_MUL_206( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_208(n,i) ARITH2_MUL_207( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_209(n,i) ARITH2_MUL_208( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_210(n,i) ARITH2_MUL_209( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_211(n,i) ARITH2_MUL_210( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_212(n,i) ARITH2_MUL_211( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_213(n,i) ARITH2_MUL_212( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_214(n,i) ARITH2_MUL_213( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_215(n,i) ARITH2_MUL_214( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_216(n,i) ARITH2_MUL_215( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_217(n,i) ARITH2_MUL_216( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_218(n,i) ARITH2_MUL_217( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_219(n,i) ARITH2_MUL_218( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_220(n,i) ARITH2_MUL_219( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_221(n,i) ARITH2_MUL_220( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_222(n,i) ARITH2_MUL_221( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_223(n,i) ARITH2_MUL_222( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_224(n,i) ARITH2_MUL_223( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_225(n,i) ARITH2_MUL_224( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_226(n,i) ARITH2_MUL_225( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_227(n,i) ARITH2_MUL_226( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_228(n,i) ARITH2_MUL_227( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_229(n,i) ARITH2_MUL_228( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_230(n,i) ARITH2_MUL_229( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_231(n,i) ARITH2_MUL_230( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_232(n,i) ARITH2_MUL_231( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_233(n,i) ARITH2_MUL_232( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_234(n,i) ARITH2_MUL_233( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_235(n,i) ARITH2_MUL_234( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_236(n,i) ARITH2_MUL_235( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_237(n,i) ARITH2_MUL_236( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_238(n,i) ARITH2_MUL_237( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_239(n,i) ARITH2_MUL_238( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_240(n,i) ARITH2_MUL_239( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_241(n,i) ARITH2_MUL_240( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_242(n,i) ARITH2_MUL_241( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_243(n,i) ARITH2_MUL_242( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_244(n,i) ARITH2_MUL_243( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_245(n,i) ARITH2_MUL_244( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_246(n,i) ARITH2_MUL_245( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_247(n,i) ARITH2_MUL_246( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_248(n,i) ARITH2_MUL_247( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_249(n,i) ARITH2_MUL_248( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_250(n,i) ARITH2_MUL_249( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_251(n,i) ARITH2_MUL_250( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_252(n,i) ARITH2_MUL_251( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_253(n,i) ARITH2_MUL_252( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_254(n,i) ARITH2_MUL_253( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_255(n,i) ARITH2_MUL_254( n,ARITH2_ADD(i,n) )
  #define ARITH2_MUL_256(n,i) ARITH2_MUL_255( n,ARITH2_ADD(i,n) )
 
  #define ARITH2_DIV_1(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(1,m)) (fn,m,ARITH2_INCR(i),1)
  #define ARITH2_DIV_2(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(2,m)) (fn,m,ARITH2_INCR(i),2)
  #define ARITH2_DIV_3(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(3,m)) (fn,m,ARITH2_INCR(i),3)
  #define ARITH2_DIV_4(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(4,m)) (fn,m,ARITH2_INCR(i),4)
  #define ARITH2_DIV_5(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(5,m)) (fn,m,ARITH2_INCR(i),5)
  #define ARITH2_DIV_6(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(6,m)) (fn,m,ARITH2_INCR(i),6)
  #define ARITH2_DIV_7(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(7,m)) (fn,m,ARITH2_INCR(i),7)
  #define ARITH2_DIV_8(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(8,m)) (fn,m,ARITH2_INCR(i),8)
  #define ARITH2_DIV_9(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(9,m)) (fn,m,ARITH2_INCR(i),9)
  #define ARITH2_DIV_10(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(10,m)) (fn,m,ARITH2_INCR(i),10)
  #define ARITH2_DIV_11(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(11,m)) (fn,m,ARITH2_INCR(i),11)
  #define ARITH2_DIV_12(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(12,m)) (fn,m,ARITH2_INCR(i),12)
  #define ARITH2_DIV_13(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(13,m)) (fn,m,ARITH2_INCR(i),13)
  #define ARITH2_DIV_14(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(14,m)) (fn,m,ARITH2_INCR(i),14)
  #define ARITH2_DIV_15(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(15,m)) (fn,m,ARITH2_INCR(i),15)
  #define ARITH2_DIV_16(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(16,m)) (fn,m,ARITH2_INCR(i),16)
  #define ARITH2_DIV_17(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(17,m)) (fn,m,ARITH2_INCR(i),17)
  #define ARITH2_DIV_18(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(18,m)) (fn,m,ARITH2_INCR(i),18)
  #define ARITH2_DIV_19(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(19,m)) (fn,m,ARITH2_INCR(i),19)
  #define ARITH2_DIV_20(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(20,m)) (fn,m,ARITH2_INCR(i),20)
  #define ARITH2_DIV_21(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(21,m)) (fn,m,ARITH2_INCR(i),21)
  #define ARITH2_DIV_22(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(22,m)) (fn,m,ARITH2_INCR(i),22)
  #define ARITH2_DIV_23(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(23,m)) (fn,m,ARITH2_INCR(i),23)
  #define ARITH2_DIV_24(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(24,m)) (fn,m,ARITH2_INCR(i),24)
  #define ARITH2_DIV_25(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(25,m)) (fn,m,ARITH2_INCR(i),25)
  #define ARITH2_DIV_26(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(26,m)) (fn,m,ARITH2_INCR(i),26)
  #define ARITH2_DIV_27(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(27,m)) (fn,m,ARITH2_INCR(i),27)
  #define ARITH2_DIV_28(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(28,m)) (fn,m,ARITH2_INCR(i),28)
  #define ARITH2_DIV_29(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(29,m)) (fn,m,ARITH2_INCR(i),29)
  #define ARITH2_DIV_30(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(30,m)) (fn,m,ARITH2_INCR(i),30)
  #define ARITH2_DIV_31(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(31,m)) (fn,m,ARITH2_INCR(i),31)
  #define ARITH2_DIV_32(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(32,m)) (fn,m,ARITH2_INCR(i),32)
  #define ARITH2_DIV_33(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(33,m)) (fn,m,ARITH2_INCR(i),33)
  #define ARITH2_DIV_34(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(34,m)) (fn,m,ARITH2_INCR(i),34)
  #define ARITH2_DIV_35(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(35,m)) (fn,m,ARITH2_INCR(i),35)
  #define ARITH2_DIV_36(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(36,m)) (fn,m,ARITH2_INCR(i),36)
  #define ARITH2_DIV_37(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(37,m)) (fn,m,ARITH2_INCR(i),37)
  #define ARITH2_DIV_38(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(38,m)) (fn,m,ARITH2_INCR(i),38)
  #define ARITH2_DIV_39(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(39,m)) (fn,m,ARITH2_INCR(i),39)
  #define ARITH2_DIV_40(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(40,m)) (fn,m,ARITH2_INCR(i),40)
  #define ARITH2_DIV_41(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(41,m)) (fn,m,ARITH2_INCR(i),41)
  #define ARITH2_DIV_42(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(42,m)) (fn,m,ARITH2_INCR(i),42)
  #define ARITH2_DIV_43(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(43,m)) (fn,m,ARITH2_INCR(i),43)
  #define ARITH2_DIV_44(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(44,m)) (fn,m,ARITH2_INCR(i),44)
  #define ARITH2_DIV_45(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(45,m)) (fn,m,ARITH2_INCR(i),45)
  #define ARITH2_DIV_46(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(46,m)) (fn,m,ARITH2_INCR(i),46)
  #define ARITH2_DIV_47(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(47,m)) (fn,m,ARITH2_INCR(i),47)
  #define ARITH2_DIV_48(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(48,m)) (fn,m,ARITH2_INCR(i),48)
  #define ARITH2_DIV_49(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(49,m)) (fn,m,ARITH2_INCR(i),49)
  #define ARITH2_DIV_50(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(50,m)) (fn,m,ARITH2_INCR(i),50)
  #define ARITH2_DIV_51(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(51,m)) (fn,m,ARITH2_INCR(i),51)
  #define ARITH2_DIV_52(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(52,m)) (fn,m,ARITH2_INCR(i),52)
  #define ARITH2_DIV_53(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(53,m)) (fn,m,ARITH2_INCR(i),53)
  #define ARITH2_DIV_54(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(54,m)) (fn,m,ARITH2_INCR(i),54)
  #define ARITH2_DIV_55(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(55,m)) (fn,m,ARITH2_INCR(i),55)
  #define ARITH2_DIV_56(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(56,m)) (fn,m,ARITH2_INCR(i),56)
  #define ARITH2_DIV_57(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(57,m)) (fn,m,ARITH2_INCR(i),57)
  #define ARITH2_DIV_58(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(58,m)) (fn,m,ARITH2_INCR(i),58)
  #define ARITH2_DIV_59(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(59,m)) (fn,m,ARITH2_INCR(i),59)
  #define ARITH2_DIV_60(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(60,m)) (fn,m,ARITH2_INCR(i),60)
  #define ARITH2_DIV_61(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(61,m)) (fn,m,ARITH2_INCR(i),61)
  #define ARITH2_DIV_62(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(62,m)) (fn,m,ARITH2_INCR(i),62)
  #define ARITH2_DIV_63(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(63,m)) (fn,m,ARITH2_INCR(i),63)
  #define ARITH2_DIV_64(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(64,m)) (fn,m,ARITH2_INCR(i),64)
  #define ARITH2_DIV_65(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(64,m)) (fn,m,ARITH2_INCR(i),65)
  #define ARITH2_DIV_66(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(65,m)) (fn,m,ARITH2_INCR(i),66)
  #define ARITH2_DIV_67(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(66,m)) (fn,m,ARITH2_INCR(i),67)
  #define ARITH2_DIV_68(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(67,m)) (fn,m,ARITH2_INCR(i),68)
  #define ARITH2_DIV_69(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(68,m)) (fn,m,ARITH2_INCR(i),69)
  #define ARITH2_DIV_70(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(69,m)) (fn,m,ARITH2_INCR(i),70)
  #define ARITH2_DIV_71(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(70,m)) (fn,m,ARITH2_INCR(i),71)
  #define ARITH2_DIV_72(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(71,m)) (fn,m,ARITH2_INCR(i),72)
  #define ARITH2_DIV_73(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(72,m)) (fn,m,ARITH2_INCR(i),73)
  #define ARITH2_DIV_74(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(73,m)) (fn,m,ARITH2_INCR(i),74)
  #define ARITH2_DIV_75(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(74,m)) (fn,m,ARITH2_INCR(i),75)
  #define ARITH2_DIV_76(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(75,m)) (fn,m,ARITH2_INCR(i),76)
  #define ARITH2_DIV_77(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(76,m)) (fn,m,ARITH2_INCR(i),77)
  #define ARITH2_DIV_78(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(77,m)) (fn,m,ARITH2_INCR(i),78)
  #define ARITH2_DIV_79(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(78,m)) (fn,m,ARITH2_INCR(i),79)
  #define ARITH2_DIV_80(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(79,m)) (fn,m,ARITH2_INCR(i),80)
  #define ARITH2_DIV_81(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(80,m)) (fn,m,ARITH2_INCR(i),81)
  #define ARITH2_DIV_82(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(81,m)) (fn,m,ARITH2_INCR(i),82)
  #define ARITH2_DIV_83(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(82,m)) (fn,m,ARITH2_INCR(i),83)
  #define ARITH2_DIV_84(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(83,m)) (fn,m,ARITH2_INCR(i),84)
  #define ARITH2_DIV_85(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(84,m)) (fn,m,ARITH2_INCR(i),85)
  #define ARITH2_DIV_86(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(85,m)) (fn,m,ARITH2_INCR(i),86)
  #define ARITH2_DIV_87(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(86,m)) (fn,m,ARITH2_INCR(i),87)
  #define ARITH2_DIV_88(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(87,m)) (fn,m,ARITH2_INCR(i),88)
  #define ARITH2_DIV_89(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(88,m)) (fn,m,ARITH2_INCR(i),89)
  #define ARITH2_DIV_90(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(89,m)) (fn,m,ARITH2_INCR(i),90)
  #define ARITH2_DIV_91(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(90,m)) (fn,m,ARITH2_INCR(i),91)
  #define ARITH2_DIV_92(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(91,m)) (fn,m,ARITH2_INCR(i),92)
  #define ARITH2_DIV_93(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(92,m)) (fn,m,ARITH2_INCR(i),93)
  #define ARITH2_DIV_94(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(93,m)) (fn,m,ARITH2_INCR(i),94)
  #define ARITH2_DIV_95(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(94,m)) (fn,m,ARITH2_INCR(i),95)
  #define ARITH2_DIV_96(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(95,m)) (fn,m,ARITH2_INCR(i),96)
  #define ARITH2_DIV_97(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(96,m)) (fn,m,ARITH2_INCR(i),97)
  #define ARITH2_DIV_98(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(97,m)) (fn,m,ARITH2_INCR(i),98)
  #define ARITH2_DIV_99(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(98,m)) (fn,m,ARITH2_INCR(i),99)
  #define ARITH2_DIV_100(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(99,m)) (fn,m,ARITH2_INCR(i),100)
  #define ARITH2_DIV_101(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(100,m)) (fn,m,ARITH2_INCR(i),101)
  #define ARITH2_DIV_102(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(101,m)) (fn,m,ARITH2_INCR(i),102)
  #define ARITH2_DIV_103(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(102,m)) (fn,m,ARITH2_INCR(i),103)
  #define ARITH2_DIV_104(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(103,m)) (fn,m,ARITH2_INCR(i),104)
  #define ARITH2_DIV_105(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(104,m)) (fn,m,ARITH2_INCR(i),105)
  #define ARITH2_DIV_106(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(105,m)) (fn,m,ARITH2_INCR(i),106)
  #define ARITH2_DIV_107(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(106,m)) (fn,m,ARITH2_INCR(i),107)
  #define ARITH2_DIV_108(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(107,m)) (fn,m,ARITH2_INCR(i),108)
  #define ARITH2_DIV_109(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(108,m)) (fn,m,ARITH2_INCR(i),109)
  #define ARITH2_DIV_110(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(109,m)) (fn,m,ARITH2_INCR(i),110)
  #define ARITH2_DIV_111(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(110,m)) (fn,m,ARITH2_INCR(i),111)
  #define ARITH2_DIV_112(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(111,m)) (fn,m,ARITH2_INCR(i),112)
  #define ARITH2_DIV_113(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(112,m)) (fn,m,ARITH2_INCR(i),113)
  #define ARITH2_DIV_114(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(113,m)) (fn,m,ARITH2_INCR(i),114)
  #define ARITH2_DIV_115(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(114,m)) (fn,m,ARITH2_INCR(i),115)
  #define ARITH2_DIV_116(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(115,m)) (fn,m,ARITH2_INCR(i),116)
  #define ARITH2_DIV_117(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(116,m)) (fn,m,ARITH2_INCR(i),117)
  #define ARITH2_DIV_118(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(117,m)) (fn,m,ARITH2_INCR(i),118)
  #define ARITH2_DIV_119(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(118,m)) (fn,m,ARITH2_INCR(i),119)
  #define ARITH2_DIV_120(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(119,m)) (fn,m,ARITH2_INCR(i),120)
  #define ARITH2_DIV_121(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(120,m)) (fn,m,ARITH2_INCR(i),121)
  #define ARITH2_DIV_122(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(121,m)) (fn,m,ARITH2_INCR(i),122)
  #define ARITH2_DIV_123(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(122,m)) (fn,m,ARITH2_INCR(i),123)
  #define ARITH2_DIV_124(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(123,m)) (fn,m,ARITH2_INCR(i),124)
  #define ARITH2_DIV_125(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(124,m)) (fn,m,ARITH2_INCR(i),125)
  #define ARITH2_DIV_126(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(125,m)) (fn,m,ARITH2_INCR(i),126)
  #define ARITH2_DIV_127(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(126,m)) (fn,m,ARITH2_INCR(i),127)
  #define ARITH2_DIV_128(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(127,m)) (fn,m,ARITH2_INCR(i),128)
  #define ARITH2_DIV_129(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(128,m)) (fn,m,ARITH2_INCR(i),129)
  #define ARITH2_DIV_130(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(129,m)) (fn,m,ARITH2_INCR(i),130)
  #define ARITH2_DIV_131(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(130,m)) (fn,m,ARITH2_INCR(i),131)
  #define ARITH2_DIV_132(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(131,m)) (fn,m,ARITH2_INCR(i),132)
  #define ARITH2_DIV_133(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(132,m)) (fn,m,ARITH2_INCR(i),133)
  #define ARITH2_DIV_134(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(133,m)) (fn,m,ARITH2_INCR(i),134)
  #define ARITH2_DIV_135(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(134,m)) (fn,m,ARITH2_INCR(i),135)
  #define ARITH2_DIV_136(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(135,m)) (fn,m,ARITH2_INCR(i),136)
  #define ARITH2_DIV_137(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(136,m)) (fn,m,ARITH2_INCR(i),137)
  #define ARITH2_DIV_138(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(137,m)) (fn,m,ARITH2_INCR(i),138)
  #define ARITH2_DIV_139(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(138,m)) (fn,m,ARITH2_INCR(i),139)
  #define ARITH2_DIV_140(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(139,m)) (fn,m,ARITH2_INCR(i),140)
  #define ARITH2_DIV_141(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(140,m)) (fn,m,ARITH2_INCR(i),141)
  #define ARITH2_DIV_142(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(141,m)) (fn,m,ARITH2_INCR(i),142)
  #define ARITH2_DIV_143(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(142,m)) (fn,m,ARITH2_INCR(i),143)
  #define ARITH2_DIV_144(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(143,m)) (fn,m,ARITH2_INCR(i),144)
  #define ARITH2_DIV_145(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(144,m)) (fn,m,ARITH2_INCR(i),145)
  #define ARITH2_DIV_146(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(145,m)) (fn,m,ARITH2_INCR(i),146)
  #define ARITH2_DIV_147(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(146,m)) (fn,m,ARITH2_INCR(i),147)
  #define ARITH2_DIV_148(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(147,m)) (fn,m,ARITH2_INCR(i),148)
  #define ARITH2_DIV_149(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(148,m)) (fn,m,ARITH2_INCR(i),149)
  #define ARITH2_DIV_150(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(149,m)) (fn,m,ARITH2_INCR(i),150)
  #define ARITH2_DIV_151(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(150,m)) (fn,m,ARITH2_INCR(i),151)
  #define ARITH2_DIV_152(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(151,m)) (fn,m,ARITH2_INCR(i),152)
  #define ARITH2_DIV_153(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(152,m)) (fn,m,ARITH2_INCR(i),153)
  #define ARITH2_DIV_154(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(153,m)) (fn,m,ARITH2_INCR(i),154)
  #define ARITH2_DIV_155(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(154,m)) (fn,m,ARITH2_INCR(i),155)
  #define ARITH2_DIV_156(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(155,m)) (fn,m,ARITH2_INCR(i),156)
  #define ARITH2_DIV_157(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(156,m)) (fn,m,ARITH2_INCR(i),157)
  #define ARITH2_DIV_158(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(157,m)) (fn,m,ARITH2_INCR(i),158)
  #define ARITH2_DIV_159(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(158,m)) (fn,m,ARITH2_INCR(i),159)
  #define ARITH2_DIV_160(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(159,m)) (fn,m,ARITH2_INCR(i),160)
  #define ARITH2_DIV_161(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(160,m)) (fn,m,ARITH2_INCR(i),161)
  #define ARITH2_DIV_162(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(161,m)) (fn,m,ARITH2_INCR(i),162)
  #define ARITH2_DIV_163(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(162,m)) (fn,m,ARITH2_INCR(i),163)
  #define ARITH2_DIV_164(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(163,m)) (fn,m,ARITH2_INCR(i),164)
  #define ARITH2_DIV_165(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(164,m)) (fn,m,ARITH2_INCR(i),165)
  #define ARITH2_DIV_166(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(165,m)) (fn,m,ARITH2_INCR(i),166)
  #define ARITH2_DIV_167(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(166,m)) (fn,m,ARITH2_INCR(i),167)
  #define ARITH2_DIV_168(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(167,m)) (fn,m,ARITH2_INCR(i),168)
  #define ARITH2_DIV_169(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(168,m)) (fn,m,ARITH2_INCR(i),169)
  #define ARITH2_DIV_170(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(169,m)) (fn,m,ARITH2_INCR(i),170)
  #define ARITH2_DIV_171(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(170,m)) (fn,m,ARITH2_INCR(i),171)
  #define ARITH2_DIV_172(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(171,m)) (fn,m,ARITH2_INCR(i),172)
  #define ARITH2_DIV_173(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(172,m)) (fn,m,ARITH2_INCR(i),173)
  #define ARITH2_DIV_174(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(173,m)) (fn,m,ARITH2_INCR(i),174)
  #define ARITH2_DIV_175(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(174,m)) (fn,m,ARITH2_INCR(i),175)
  #define ARITH2_DIV_176(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(175,m)) (fn,m,ARITH2_INCR(i),176)
  #define ARITH2_DIV_177(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(176,m)) (fn,m,ARITH2_INCR(i),177)
  #define ARITH2_DIV_178(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(177,m)) (fn,m,ARITH2_INCR(i),178)
  #define ARITH2_DIV_179(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(178,m)) (fn,m,ARITH2_INCR(i),179)
  #define ARITH2_DIV_180(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(179,m)) (fn,m,ARITH2_INCR(i),180)
  #define ARITH2_DIV_181(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(180,m)) (fn,m,ARITH2_INCR(i),181)
  #define ARITH2_DIV_182(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(181,m)) (fn,m,ARITH2_INCR(i),182)
  #define ARITH2_DIV_183(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(182,m)) (fn,m,ARITH2_INCR(i),183)
  #define ARITH2_DIV_184(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(183,m)) (fn,m,ARITH2_INCR(i),184)
  #define ARITH2_DIV_185(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(184,m)) (fn,m,ARITH2_INCR(i),185)
  #define ARITH2_DIV_186(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(185,m)) (fn,m,ARITH2_INCR(i),186)
  #define ARITH2_DIV_187(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(186,m)) (fn,m,ARITH2_INCR(i),187)
  #define ARITH2_DIV_188(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(187,m)) (fn,m,ARITH2_INCR(i),188)
  #define ARITH2_DIV_189(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(188,m)) (fn,m,ARITH2_INCR(i),189)
  #define ARITH2_DIV_190(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(189,m)) (fn,m,ARITH2_INCR(i),190)
  #define ARITH2_DIV_191(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(190,m)) (fn,m,ARITH2_INCR(i),191)
  #define ARITH2_DIV_192(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(191,m)) (fn,m,ARITH2_INCR(i),192)
  #define ARITH2_DIV_193(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(192,m)) (fn,m,ARITH2_INCR(i),193)
  #define ARITH2_DIV_194(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(193,m)) (fn,m,ARITH2_INCR(i),194)
  #define ARITH2_DIV_195(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(194,m)) (fn,m,ARITH2_INCR(i),195)
  #define ARITH2_DIV_196(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(195,m)) (fn,m,ARITH2_INCR(i),196)
  #define ARITH2_DIV_197(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(196,m)) (fn,m,ARITH2_INCR(i),197)
  #define ARITH2_DIV_198(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(197,m)) (fn,m,ARITH2_INCR(i),198)
  #define ARITH2_DIV_199(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(198,m)) (fn,m,ARITH2_INCR(i),199)
  #define ARITH2_DIV_200(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(199,m)) (fn,m,ARITH2_INCR(i),200)
  #define ARITH2_DIV_201(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(200,m)) (fn,m,ARITH2_INCR(i),201)
  #define ARITH2_DIV_202(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(201,m)) (fn,m,ARITH2_INCR(i),202)
  #define ARITH2_DIV_203(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(202,m)) (fn,m,ARITH2_INCR(i),203)
  #define ARITH2_DIV_204(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(203,m)) (fn,m,ARITH2_INCR(i),204)
  #define ARITH2_DIV_205(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(204,m)) (fn,m,ARITH2_INCR(i),205)
  #define ARITH2_DIV_206(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(205,m)) (fn,m,ARITH2_INCR(i),206)
  #define ARITH2_DIV_207(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(206,m)) (fn,m,ARITH2_INCR(i),207)
  #define ARITH2_DIV_208(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(207,m)) (fn,m,ARITH2_INCR(i),208)
  #define ARITH2_DIV_209(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(208,m)) (fn,m,ARITH2_INCR(i),209)
  #define ARITH2_DIV_210(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(209,m)) (fn,m,ARITH2_INCR(i),210)
  #define ARITH2_DIV_211(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(210,m)) (fn,m,ARITH2_INCR(i),211)
  #define ARITH2_DIV_212(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(211,m)) (fn,m,ARITH2_INCR(i),212)
  #define ARITH2_DIV_213(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(212,m)) (fn,m,ARITH2_INCR(i),213)
  #define ARITH2_DIV_214(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(213,m)) (fn,m,ARITH2_INCR(i),214)
  #define ARITH2_DIV_215(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(214,m)) (fn,m,ARITH2_INCR(i),215)
  #define ARITH2_DIV_216(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(215,m)) (fn,m,ARITH2_INCR(i),216)
  #define ARITH2_DIV_217(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(216,m)) (fn,m,ARITH2_INCR(i),217)
  #define ARITH2_DIV_218(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(217,m)) (fn,m,ARITH2_INCR(i),218)
  #define ARITH2_DIV_219(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(218,m)) (fn,m,ARITH2_INCR(i),219)
  #define ARITH2_DIV_220(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(219,m)) (fn,m,ARITH2_INCR(i),220)
  #define ARITH2_DIV_221(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(220,m)) (fn,m,ARITH2_INCR(i),221)
  #define ARITH2_DIV_222(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(221,m)) (fn,m,ARITH2_INCR(i),222)
  #define ARITH2_DIV_223(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(222,m)) (fn,m,ARITH2_INCR(i),223)
  #define ARITH2_DIV_224(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(223,m)) (fn,m,ARITH2_INCR(i),224)
  #define ARITH2_DIV_225(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(224,m)) (fn,m,ARITH2_INCR(i),225)
  #define ARITH2_DIV_226(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(225,m)) (fn,m,ARITH2_INCR(i),226)
  #define ARITH2_DIV_227(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(226,m)) (fn,m,ARITH2_INCR(i),227)
  #define ARITH2_DIV_228(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(227,m)) (fn,m,ARITH2_INCR(i),228)
  #define ARITH2_DIV_229(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(228,m)) (fn,m,ARITH2_INCR(i),229)
  #define ARITH2_DIV_230(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(229,m)) (fn,m,ARITH2_INCR(i),230)
  #define ARITH2_DIV_231(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(230,m)) (fn,m,ARITH2_INCR(i),231)
  #define ARITH2_DIV_232(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(231,m)) (fn,m,ARITH2_INCR(i),232)
  #define ARITH2_DIV_233(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(232,m)) (fn,m,ARITH2_INCR(i),233)
  #define ARITH2_DIV_234(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(233,m)) (fn,m,ARITH2_INCR(i),234)
  #define ARITH2_DIV_235(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(234,m)) (fn,m,ARITH2_INCR(i),235)
  #define ARITH2_DIV_236(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(235,m)) (fn,m,ARITH2_INCR(i),236)
  #define ARITH2_DIV_237(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(236,m)) (fn,m,ARITH2_INCR(i),237)
  #define ARITH2_DIV_238(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(237,m)) (fn,m,ARITH2_INCR(i),238)
  #define ARITH2_DIV_239(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(238,m)) (fn,m,ARITH2_INCR(i),239)
  #define ARITH2_DIV_240(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(239,m)) (fn,m,ARITH2_INCR(i),240)
  #define ARITH2_DIV_241(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(240,m)) (fn,m,ARITH2_INCR(i),241)
  #define ARITH2_DIV_242(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(241,m)) (fn,m,ARITH2_INCR(i),242)
  #define ARITH2_DIV_243(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(242,m)) (fn,m,ARITH2_INCR(i),243)
  #define ARITH2_DIV_244(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(243,m)) (fn,m,ARITH2_INCR(i),244)
  #define ARITH2_DIV_245(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(244,m)) (fn,m,ARITH2_INCR(i),245)
  #define ARITH2_DIV_246(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(245,m)) (fn,m,ARITH2_INCR(i),246)
  #define ARITH2_DIV_247(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(246,m)) (fn,m,ARITH2_INCR(i),247)
  #define ARITH2_DIV_248(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(247,m)) (fn,m,ARITH2_INCR(i),248)
  #define ARITH2_DIV_249(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(248,m)) (fn,m,ARITH2_INCR(i),249)
  #define ARITH2_DIV_250(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(249,m)) (fn,m,ARITH2_INCR(i),250)
  #define ARITH2_DIV_251(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(250,m)) (fn,m,ARITH2_INCR(i),251)
  #define ARITH2_DIV_252(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(251,m)) (fn,m,ARITH2_INCR(i),252)
  #define ARITH2_DIV_253(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(252,m)) (fn,m,ARITH2_INCR(i),253)
  #define ARITH2_DIV_254(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(253,m)) (fn,m,ARITH2_INCR(i),254)
  #define ARITH2_DIV_255(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(254,m)) (fn,m,ARITH2_INCR(i),255)
  #define ARITH2_DIV_256(fn,m,i,p) ARITH2_CATN( DIV,ARITH2_SUB(255,m)) (fn,m,ARITH2_INCR(i),256)
 
 
 #endif /* CPPARITH_HAVE_TWICE */
#endif /* CPPARITH_TWICE */
