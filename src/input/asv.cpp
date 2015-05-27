//
//  asv.cpp
//  aces-ansatz
//
//  Created by Jason Byrd on 3/23/15.
//  Copyright (c) 2015 Jason Byrd. All rights reserved.
//

#include "input_reader.h"
#include "molecule_data.h"
#include "core.h"
#include "abort.h"
#include "flags.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fenv.h>
#include <ctime>
#include <execinfo.h>
#include <signal.h>
#include <fstream>
#include <cstdlib>
#include <iterator>     // back_inserter
#include <regex>        // regex, sregex_token_iterator
#include <locale>
#include <iomanip>

input::ASV_nl_t ASV_nl[] = {
    /* COMMENTED NUMBERS ARE FORTRAN INDICES !!! */
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*1*/   "IPRNT",	"PRINT#",	h_ICHAR_long,	0,	""},
    {/*2*/   "CALTYPE",	"CALC#LEVEL",	h_ICHAR_handle,	0,	""},
    {/*3*/   "IDRLVL",	"DERIV#_LEV",	h_ICHAR_handle,	-1,	""},
    {/*4*/   "ICCCNV",	"CC_C#ONV",	h_ICHAR_long,	7,	"(tol)"},
    {/*5*/   "ISCFCV",	"SCF_C#ONV",	h_ICHAR_long,	7,	"(tol)"},
    {/*6*/   "IXFTOL",	"XFORM#_TOL",	h_ICHAR_long,	11,	"(tol)"},
    {/*7*/   "ICCCYC",	"CC_MAX#CYC",	h_ICHAR_long,	0,	"cycles"},
    {/*8*/   "ILINDP",	"LIN#DEP_TOL",	h_ICHAR_long,	8,	""},
    {/*9*/   "IRDOFM",	"RDO#",		h_ICHAR_handle,	-1,	""},
    {/*10*/  "IRPP",	"SCF_EXTRAP#",	h_ICHAR_handle,	1,	""},
    {/*11*/  "IREFNC",	"REF#ERENCE",	h_ICHAR_handle,	0,	""},
    {/*12*/  "ICCEOR",	"CC_EXP#ORDER",	h_ICHAR_long,	0,	""},
    {/*13*/  "IEVERY",	"TAM#P_SUM",	h_ICHAR_long,	0,	""},
    {/*14*/  "ITOPT2",	"NTO#P_TAMP",	h_ICHAR_long,	15,	""},
    {/*15*/  "ISCFDP",	"DAMPSCF#",	h_ICHAR_long,	20,	"x 0.01"},
    {/*16*/  "ISCFCY",	"SCF_M#AXCYC",	h_ICHAR_long,	150,	"cycles"},
    {/*17*/  "IOCCU",	"OCC#UPATION",	h_ICHAR_string,	0,	""},
    {/*18*/  "IPROPS",	"PROP#S",	h_ICHAR_handle,	0,	""},
    {/*19*/  "IDENS",	"DENS#ITY",	h_ICHAR_handle,	0,	""},
    {/*20*/  "IRPPOR",	"SCF_EXPOR#DE",	h_ICHAR_long,	6,	""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*21*/  "ICCEXT",	"CC_EXT#RAPOL",	h_ICHAR_handle,	1,	""},
    {/*22*/  "IBRKNR",	"BRUEC#KNER",	h_ICHAR_handle,	0,	""},
    {/*23*/  "IXEFLD",	"XFI#ELD",	h_ICHAR_long,	0,	"x 10-6"},
    {/*24*/  "IYEFLD",	"YFI#ELD",	h_ICHAR_long,	0,	"x 10-6"},
    {/*25*/  "IZEFLD",	"ZFI#ELD",	h_ICHAR_long,	0,	"x 10-6"},
    {/*26*/  "ISVINT",	"SAVE#_INTS",	h_ICHAR_handle,	0,	""},
    {/*27*/  "IDRPMO",	"DROP#MO",	h_ICHAR_string,	0,	""},
    {/*28*/  "ICHRGE",	"CHARGE#",	h_ICHAR_long,	0,	""},
    {/*29*/  "IMULTP",	"MULT#IPLICTY",	h_ICHAR_long,	1,	""},
    {/*30*/  "ICPHFT",	"CPHF_C#ONVER",	h_ICHAR_long,	12,	"(tol)"},
    {/*31*/  "ICPHFC",	"CPHF_M#AXCYC",	h_ICHAR_long,	64,	"cycles"},
    {/*32*/  "","",0,0,""},
    {/*33*/  "","",0,0,""},
    {/*34*/  "IQRHFO",	"QRHF_O#RBITA",	h_ICHAR_string,	0,	"offset"},
    {/*35*/  "INCORE",	"INC#ORE",	h_ICHAR_handle,	0,	""},
    {/*36*/  "IMEMSZ",	"MEM#ORY_SIZE",	h_ICHAR_long,	15000000,"Words"},
    {/*37*/  "IFLREC",	"FILE_REC#SIZ",	h_ICHAR_long,	-1,	"Words"},
    {/*38*/  "NON-HF",	"NONHF#",	h_ICHAR_handle,	0,	""},
    {/*39*/  "IORBTP",	"ORB#ITALS",	h_ICHAR_handle,	-1,	""},
    {/*40*/  "IRPPLS",	"SCF_EXPST#AR",	h_ICHAR_long,	8,	""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*41*/  "ILOCOC",	"LOCK_ORBO#CC",	h_ICHAR_handle,	0,	""},
    {/*42*/  "ISTRIP",	"FILE_STRI#PE",	h_ICHAR_long,	0,	""},
    {/*43*/  "HBAR",	"DOHBAR#",	h_ICHAR_handle,	0,	""},
    {/*44*/  "ICHREC",	"CACHE#_RECS",	h_ICHAR_long,	-1,	""},
    {/*45*/  "IGUESS",	"GUE#SS",	h_ICHAR_handle,	0,	""},
    {/*46*/  "IJPRNT",	"JOD#A_PRINT",	h_ICHAR_long,	0,	""},
    {/*47*/  "INR",		"OPT_METHO#D",	h_ICHAR_handle,	0,	""},
    {/*48*/  "ICONTL",	"CONV#ERGENCE",	h_ICHAR_long,	4,	"H/bohr"},
    {/*49*/  "IVEC",	"EIG#ENVECTOR",	h_ICHAR_long,	1,	""},
    {/*50*/  "IDIE",	"NEG#EVAL",	h_ICHAR_handle,	2,	""},
    {/*51*/  "ICURVY",	"CUR#VILINEAR",	h_ICHAR_handle,	0,	""},
    {/*52*/  "ISTCRT",	"STP_SIZ_CTL#",	h_ICHAR_handle,	0,	""},
    {/*53*/  "IMXSTP",	"MAX#_STEP",	h_ICHAR_long,	300,	"millibohr"},
    {/*54*/  "IVIB",	"VIB#RATION",	h_ICHAR_handle,	0,	""},
    {/*55*/  "IRECAL",	"EVA#L_HESS",	h_ICHAR_long,	-1,	"# of cyc."},
    {/*56*/  "INTPROG",	"INTEGRAL#S",	h_ICHAR_handle,	1,	""},
    {/*57*/  "IDISFD",	"FD_STEP#SIZE",	h_ICHAR_long,	0,	"10-4 bohr"},
    {/*58*/  "IGRDFD",	"POI#NTS",	h_ICHAR_handle,	0,	""},
    {/*59*/  "ICNTYP",	"CONT#RACTION",	h_ICHAR_handle,	1,	""},
    {/*60*/  "ISYM",	"SYM#METRY",	h_ICHAR_handle,	2,	""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*61*/  "IBASIS",	"BAS#IS",	h_ICHAR_string,	0,	""},
    {/*62*/  "IDFGHI",	"SPHER#ICAL",	h_ICHAR_handle,	1,	""},
    {/*63*/  "IRESET",	"RESET#_FLAGS",	h_ICHAR_handle,	0,	""},
    {/*64*/  "IPTORB",	"PER#T_ORB",	h_ICHAR_handle,	2,	""},
    {/*65*/  "IGNBS1",	"GENBAS_1#",	h_ICHAR_long,	0,	""},
    {/*66*/  "IGNBS2",	"GENBAS_2#",	h_ICHAR_long,	0,	""},
    {/*67*/  "IGNBS3",	"GENBAS_3#",	h_ICHAR_long,	0,	""},
    {/*68*/  "ICOORD",	"COORD#INATES",	h_ICHAR_handle,	3,	""},
    {/*69*/  "ICKSYM",	"CHECK_SYM#",	h_ICHAR_handle,	1,	""},
    {/*70*/  "ISCFPR",	"SCF_PR#INT",	h_ICHAR_long,	0,	""},
    {/*71*/  "IECP",	"ECP#",		h_ICHAR_handle,	0,	""},
    {/*72*/  "IRSTRT",	"RESTART#",	h_ICHAR_handle,	1,	""},
    {/*73*/  "ITRAIN",	"TRA#NS_INV",	h_ICHAR_handle,	0,	""},
    {/*74*/  "ISTABL",	"HFS#TABILITY",	h_ICHAR_handle,	0,	""},
    {/*75*/  "ROTVEC",	"ROT_E#VEC",	h_ICHAR_long,	0,	""},
    {/*76*/  "IBRTOL",	"BRUCK#_CONV",	h_ICHAR_long,	4,	"(tol)"},
    {/*77*/  "IQRHFG",	"QRHF_G#ENERA",	h_ICHAR_string,	0,	""},
    {/*78*/  "IUNITS",	"UNI#TS",	h_ICHAR_handle,	0,	""},
    {/*79*/  "IFDGRP",	"FD_U#SEGROUP",	h_ICHAR_handle,	0,	""},
    {/*80*/  "IFDPRJ",	"FD_P#ROJECT",	h_ICHAR_handle,	0,	""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    /* 81    "IFDCAL",	"FD_C#ALTYPE",	h_ICHAR_handle,	0,	""*/
    {/*81*/  "","",0,0,""},
    {/*82*/  "IFDIRR",	"FD_IRR#EPS",	h_ICHAR_string,	0,	""},
    {/*83*/  "IVTRAN",	"VTR#AN",	h_ICHAR_handle,	0,	""},
    {/*84*/  "IHF2Fl",	"HF2#_FILE",	h_ICHAR_handle,	1,	""},
    {/*85*/  "ISUBGP",	"SUBGRO#UP",	h_ICHAR_handle,	0,	""},
    {/*86*/  "ISBXYZ",	"SUBGRP#AXIS",	h_ICHAR_handle,	0,	""},
    {/*87*/  "IEXCIT",	"EXCITE#",	h_ICHAR_handle,	0,	""},
    {/*88*/  "IZTACN",	"ZETA_CON#V",	h_ICHAR_long,	12,	"(tol)"},
    {/*89*/  "IEXSYM",	"ESTATE_SYM#",	h_ICHAR_string,	0,	""},
    {/*90*/  "ITREAT",	"TREA#T_PERT",	h_ICHAR_handle,	0,	""},
    {/*91*/  "IEXPRP",	"ESTATE_PROP#",	h_ICHAR_handle,	0,	""},
    {/*92*/  "IOPTCY",	"OPT_MAX#CYC",	h_ICHAR_long,	50,	""},
    {/*93*/  "IABCDT",	"ABCDTYP#E",	h_ICHAR_handle,	0,	""},
    {/*94*/  "IQRHFS",	"QRHF_S#PIN",	h_ICHAR_string,	0,	""},
    {/*95*/  "IAOLAD",	"AO_LAD#DERS",	h_ICHAR_handle,	1,	""},
    {/*96*/  "IFOCK",	"FOCK#",	h_ICHAR_handle,	1,	""},
    {/*97*/  "IEXMXC",	"ESTATE_MAX#C",	h_ICHAR_long,	20,	""},
    {/*98*/  "IEXTOL",	"ESTATE_TOL#",	h_ICHAR_long,	-1,	"(tol)"},
    {/*99*/  "ITURBO",	"TURBOMOL#E",	h_ICHAR_handle,	0,	""},
    {/*100*/ "IGABCD",	"GAMMA#_ABCD",	h_ICHAR_handle,	0,	""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*101*/ "IZTYPE",	"ZETA_TYP#E",	h_ICHAR_handle,	1,	""},
    {/*102*/ "IZMAXC",	"ZETA_MAX#CYC",	h_ICHAR_long,	50,	""},
    {/*103*/ "IRESRM",	"RESRAMAN#",	h_ICHAR_handle,	0,	""},
    {/*104*/ "IPSI",	"PSI#",		h_ICHAR_handle,	0,	""},
    {/*105*/ "IGEOPT",	"GEOM_OPT#",	h_ICHAR_handle,	0,	""},
    {/*106*/ "IEXTRN",	"EXTERNAL#",	h_ICHAR_handle,	0,	""},
    {/*107*/ "IHESUP",	"HESS_UPD#ATE",	h_ICHAR_handle,	0,	""},
    {/*108*/ "IINHES",	"INIT_HESS#IAN",h_ICHAR_handle,	0,	""},
    {/*109*/ "IEXTRP",	"EXTRAP#OLATE",	h_ICHAR_handle,	0,	""},
    {/*110*/ "","",0,0,""},
    {/*111*/ "","",0,0,""},
    {/*112*/ "","",0,0,""},
    {/*113*/ "","",0,0,""},
    {/*114*/ "","",0,0,""},
    {/*115*/ "","",0,0,""},
    {/*116*/ "","",0,0,""},
    {/*117*/ "","",0,0,""},
    {/*118*/ "","",0,0,""},
    {/*119*/ "","",0,0,""},
    {/*120*/ "","",0,0,""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*121*/ "IFRAG",	"FRAG#",h_ICHAR_handle,	0,	""},
    {/*122*/ "","",0,0,""},
    {/*123*/ "","",0,0,""},
    {/*124*/ "","",0,0,""},
    {/*125*/ "","",0,0,""},
    {/*126*/ "","",0,0,""},
    {/*127*/ "","",0,0,""},
    {/*128*/ "","",0,0,""},
    {/*129*/ "","",0,0,""},
    {/*130*/ "","",0,0,""},
    {/*131*/ "","",0,0,""},
    {/*132*/ "","",0,0,""},
    {/*133*/ "","",0,0,""},
    {/*134*/ "","",0,0,""},
    {/*135*/ "","",0,0,""},
    {/*136*/ "","",0,0,""},
    {/*137*/ "","",0,0,""},
    {/*138*/ "","",0,0,""},
    {/*139*/ "","",0,0,""},
    {/*140*/ "","",0,0,""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*141*/ "","",0,0,""},
    {/*142*/ "","",0,0,""},
    {/*143*/ "","",0,0,""},
    {/*144*/ "","",0,0,""},
    {/*145*/ "","",0,0,""},
    {/*146*/ "","",0,0,""},
    {/*147*/ "","",0,0,""},
    {/*148*/ "","",0,0,""},
    {/*149*/ "","",0,0,""},
    {/*150*/ "","",0,0,""},
    {/*151*/ "","",0,0,""},
    {/*152*/ "","",0,0,""},
    {/*153*/ "","",0,0,""},
    {/*154*/ "","",0,0,""},
    {/*155*/ "","",0,0,""},
    {/*156*/ "","",0,0,""},
    {/*157*/ "","",0,0,""},
    {/*158*/ "","",0,0,""},
    {/*159*/ "","",0,0,""},
    {/*160*/ "","",0,0,""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*161*/ "","",0,0,""},
    {/*162*/ "","",0,0,""},
    {/*163*/ "","",0,0,""},
    {/*164*/ "","",0,0,""},
    {/*165*/ "","",0,0,""},
    {/*166*/ "","",0,0,""},
    {/*167*/ "","",0,0,""},
    {/*168*/ "","",0,0,""},
    {/*169*/ "","",0,0,""},
    {/*170*/ "","",0,0,""},
    {/*171*/ "","",0,0,""},
    {/*172*/ "","",0,0,""},
    {/*173*/ "","",0,0,""},
    {/*174*/ "","",0,0,""},
    {/*175*/ "","",0,0,""},
    {/*176*/ "","",0,0,""},
    {/*177*/ "","",0,0,""},
    {/*178*/ "","",0,0,""},
    {/*179*/ "","",0,0,""},
    {/*180*/ "","",0,0,""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*181*/ "","",0,0,""},
    {/*182*/ "","",0,0,""},
    {/*183*/ "","",0,0,""},
    {/*184*/ "","",0,0,""},
    {/*185*/ "","",0,0,""},
    {/*186*/ "","",0,0,""},
    {/*187*/ "","",0,0,""},
    {/*188*/ "","",0,0,""},
    {/*189*/ "","",0,0,""},
    {/*190*/ "","",0,0,""},
    {/*191*/ "","",0,0,""},
    {/*192*/ "","",0,0,""},
    {/*193*/ "","",0,0,""},
    {/*194*/ "","",0,0,""},
    {/*195*/ "","",0,0,""},
    {/*196*/ "","",0,0,""},
    {/*197*/ "","",0,0,""},
    {/*198*/ "","",0,0,""},
    {/*199*/ "","",0,0,""},
    {/*200*/ "","",0,0,""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*201*/ "IEACLC",	"EA_CALC#",	h_ICHAR_handle,	0,	""},
    {/*202*/ "IEASYM",	"EA_SYM#",	h_ICHAR_string,	0,	""},
    {/*203*/ "ITDHF",	"TDHF#",	h_ICHAR_handle,	0,	""},
    {/*204*/ "IFNCTL",	"FUNCT#IONAL",	h_ICHAR_handle,	4,	""},
    {/*205*/ "IEOMCY",	"EOM_MAXCY#C",	h_ICHAR_long,	50,	"cycles"},
    {/*206*/ "IEOMPR",	"EOMPROP#",	h_ICHAR_handle,	0,	""},
    {/*207*/ "IABCDF",	"ABCDFULL#",	h_ICHAR_handle,	0,	""},
    {/*208*/ "IINTOL",	"INTGRL_TOL#",	h_ICHAR_long,	14,	"(tol)"},
    {/*209*/ "IDMPTY",	"DAMP_TYP#",	h_ICHAR_handle,	0,	""},
    {/*210*/ "IDMPTL",	"DAMP_TOL#",	h_ICHAR_long,	10,	"x 0.01"},
    {/*211*/ "ILSHA1",	"LSHF_A1#",	h_ICHAR_long,	0,	"x 0.01"},
    {/*212*/ "ILSHB1",	"LSHF_B1#",	h_ICHAR_long,	0,	"x 0.01"},
    {/*213*/ "IPOLYR",	"POLYRATE#",	h_ICHAR_handle,	0,	""},
    {/*214*/ "IIPCLC",	"IP_CALC#",	h_ICHAR_handle,	0,	""},
    {/*215*/ "IIPSYM",	"IP_SYM#",	h_ICHAR_string,	0,	""},
    {/*216*/ "IPTYPE",	"IP_SEARCH#",	h_ICHAR_handle,	0,	""},
    {/*217*/ "IEOM",	"EOMREF#",	h_ICHAR_handle,	0,	""},
    {/*218*/ "ISOLV",	"SOLVEN#T",	h_ICHAR_long,	0,	""},
    {/*219*/ "EETYPE",	"EE_SEARCH#",	h_ICHAR_handle,	0,	""},
    {/*220*/ "IEOMPR",	"EOM_PRJCT#",	h_ICHAR_handle,	0,	""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*221*/ "INWVRT",	"NEWVRT#",	h_ICHAR_handle,	0,	""},
    {/*222*/ "IABCD",	"HBARABCD#",	h_ICHAR_handle,	0,	""},
    {/*223*/ "IABCI",	"HBARABCI#",	h_ICHAR_handle,	0,	""},
    {/*224*/ "INT3EE",	"NT3EOMEE#",	h_ICHAR_handle,	0,	""},
    {/*225*/ "INOREO",	"NOREORI#",	h_ICHAR_handle,	2,	""},
    {/*226*/ "IEESYM",	"EE_SYM#",	h_ICHAR_string,	0,	""},
    {/*227*/ "IKSPOT",	"KS_POT#",	h_ICHAR_handle,	0,	""},
    {/*228*/ "IDIPC",	"DIP_CAL#C",	h_ICHAR_handle,	0,	""},
    {/*229*/ "IDIPSY",	"DIP_SY#M",	h_ICHAR_string,	0,	""},
    {/*230*/ "IDEAC",	"DEA_CAL#C",	h_ICHAR_handle,	0,	""},
    {/*231*/ "IDEASY",	"DEA_SY#M",	h_ICHAR_string,	0,	""},
    {/*232*/ "IPROG",	"PROGRAM#",	h_ICHAR_handle,	0,	""},
    {/*233*/ "ICCR12",	"CCR12#",	h_ICHAR_handle,	0,	""},
    {/*234*/ "IXEOMF",	"EOMXFIELD#",	h_ICHAR_long,	0,	"x 10-6"},
    {/*235*/ "IYEOMF",	"EOMYFIELD#",	h_ICHAR_long,	0,	"x 10-6"},
    {/*236*/ "IZEOMF",	"EOMZFIELD#",	h_ICHAR_long,	0,	"x 10-6"},
    {/*237*/ "IINSF",	"INSERTF#",	h_ICHAR_handle,	0,	""},
    {/*238*/ "IGRDCL",	"GRAD_CALC#",	h_ICHAR_handle,	0,	""},
    {/*239*/ "IIMEM",	"IMEM#_SIZE",	h_ICHAR_long,	3000000,"Words"},
    {/*240*/ "IMKRHF",	"MAKERHF#",	h_ICHAR_handle,	0,	""},
    /*       *alias,	*oppar,		ichar,		ideflt,	*units */
    {/*241*/ "IGLBMM",	"GLOBAL_MEM#",	h_ICHAR_long,	0,	"Words"},
    {/*242*/ "IPRPNT",	"PRP_INT#S",	h_ICHAR_handle,	0,	""},
    {/*243*/ "IFNOKP",	"FNO_KEEP#",	h_ICHAR_long,	0,	"percent"},
    {/*244*/ "IFNOPT",	"FNO_POST#",	h_ICHAR_handle,	0,	""},
    {/*245*/ "IFNOAC",	"FNO_ACT#IVE",	h_ICHAR_long,	0,	"percent"},
    {/*246*/ "INAT",	"NATURAL#",	h_ICHAR_handle,	0,	""},
    {/*247*/ "ICCSYM",	"ACC_SYM#",	h_ICHAR_string,	0,	""},
    {/*248*/ "IUNO_R",	"UNO_REF#",	h_ICHAR_handle,	0,	""},
    {/*249*/ "IUNO_C",	"UNO_CHARG#E",	h_ICHAR_long,	0,	""},
    {/*250*/ "IUNO_M",	"UNO_MULT#",	h_ICHAR_long,	1,	""},
    {/*251*/ "IRAMAN",	"RAMAN#",	h_ICHAR_handle,	0,	""},
    {/*252*/ "IKUCH",	"KUCHARSKI#",	h_ICHAR_handle,	0,	""},
    {/*253*/ "ISCF",	"SCF_TYPE#",	h_ICHAR_handle,	0,	""},
    {/*254*/ "IDIRCT",	"DIRECT#",	h_ICHAR_handle,	0,	""},
    {/*255*/ "BSNGST",	"SINGLE_STOR#E",h_ICHAR_handle,	0,	""},
    {/*256*/ "","",0,0,""},
    {/*257*/ "","",0,0,""},
    {/*258*/ "","",0,0,""},
    {/*259*/ "","",0,0,""},
    {/*260*/ "","",0,0,""}
}; /* end ASV_nl[] definition */


namespace input {
    
    int asv_handle_proc(int index, std::string input_string_value)
    {
        
        int output_keyword_value = -999;
        switch (index) /* Fortran-style indexing (to match h_IOPPAR_* )*/
        {
                
                /* You do not HAVE to have a separate case block for your ASV
                 unless the list of handles is unique or requires special
                 processing. If you find a handles[] array that matches your
                 ASV, then PLEASE use it. Likewise, if you have to edit an
                 existing array that belongs to more than one ASV, BE CAREFUL
                 and create a new case block if necessary. */
                
                /* REMEMBER TO TERMINATE THE LISTS WITH NULL STRINGS! */
                
            case h_IOPPAR_calclevel:
            case h_IOPPAR_fno_keep:
            {
                const char *handles[] =
                {
                    /* 0- 3*/ "SCF",		"MBPT(2)",	"MBPT(3)",	"SDQ-MBPT(4)",
                    /* 4- 7*/ "MBPT(4)",	"LCCD",		"LCCSD",	"UCCSD(4)",
                    /* 8-11*/ "CCD",		"UCC(4)",	"CCSD",		"CCSD[T]",
                    /*12-15*/ "CCSD+TQ*",	"CCSDT-1",	"CCSDT-1b",	"CCSDT-2",
                    /*16-19*/ "CCSDT-3",	"CCSDT-4",	"CCSDT",	"LCCSDT",
                    /*20-23*/ "CCD+ST(CCD)",	"QCISD(T)",	"CCSD(T)",	"QCISD",
                    /*24-27*/ "CID",		"CISD",		"QCISD(TQ)",	"CCSD(TQ)",
                    /*28-31*/ "CCSD+TQ",	"CCSDT+Q*",	"CCSDT+Q",	"CC5SD(T)",
                    /*32-35*/ "CCSD-T",	"CC3",		"CCSDT-T1T2",	"CCSDTQ-1",
                    /*36-39*/ "CCSDTQF-1",	"CCSDTQ-2",	"CCSDTQ-3",	"CCSDTQ",
                    /*40-43*/ "ACCSD",	"HFDFT",	"ACCSD(T)",	"CCSD(TQf)",
                    /*44-47*/ "CCSDT(Qf)",	""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_reference:
            {
                const char *handles[] = { "RHF",  "UHF",
                    "ROHF", "TWODET", "ROHF-OS", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_props:
            {
                const char *handles[] =
                {
                    "OFF",		/* 0 */
                    "FIRST_ORDER",	/* 1 */
                    "SECOND_ORDER",	/* 2 */
                    "NMR",		/* 3 */
                    "NMR_SWITCH",	/* 4 */
                    "CONVENTIONAL",	/* 5 */
                    "GIAO",		/* 6 */
                    "TDHF",		/* 7 */
                    "J_SO",		/* 8 */
                    "J_FC",		/* 9 */
                    "J_SD",		/* 10 */
                    "EOM_NLO",	/* 11 */
                    "GEERTSEN",	/* 12 */
                    "JSC_ALL",	/* 13 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_density:
            {
                const char *handles[] = { "RELAXED", "RESPONSE", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_cc_extrapol:
            {
                const char *handles[] = { "STANDARD", "DIIS", "NOJACOBI",
                    "OFF", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_scf_extrap:
            {
                const char *handles[] = { "NONE", "RPP", "QC",""};
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_incore:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "NOABCD",	/* 1 */
                    "T",		/* 2 */
                    "=",		/* 3 (unused) */
                    "NOABCI",	/* 4 */
                    "NOABIJ",	/* 5 */
                    "ALL",		/* 6 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_orbitals:
            {
                const char *handles[] = { "STANDARD", "SEMICANONICAL", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_guess:
            {
                const char *handles[] =
                {
                    "MOREAD",	/* 0 */
                    "CORE",		/* 1 */
                    "NDDO",		/* 2 */
                    "WALT_PRJDEN",	/* 3 */
                    "READ_SO_MOS",	/* 4 */
                    "READ_AO_MOS",	/* 5 */
                    "MIN_BASIS",	/* 6 */
                    "HUCKEL",	/* 7 */
                    "OVERRIDE"      /* 8 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_geom_opt:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "PARTIAL",   	/* 1 */
                    "CART",		/* 2 */
                    "FULL",		/* 3 */
                    "RIC" ,         /* 4 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_external:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "HYPERCHEM",	/* 1 */
                    "MOLDEN",	/* 2 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_stp_siz_ctl:
            {
                const char *handles[] =
                {
                    "TRUST_RADIUS",	/* 0 */
                    "NORM",		/* 1 */
                    "MAXIMUM",	/* 2 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_hess_update:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "POWELL",	/* 1 */
                    "BFGS",		/* 2 */
                    "MS",		/* 3 */
                    "BOFILL",	/* 4 */
                    "PSB",		/* 5 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_init_hessian:
            {
                const char *handles[] =
                {
                    "SPECIAL",	/* 0 */
                    "FCMINT",	/* 1 */
                    "MOPAC",	/* 2 */
                    "EXACT",	/* 3 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_extrap:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "GRADIENT",	/* 1 */
                    "ENERGY",	/* 2 */
                    "COMBO",	/* 3 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_opt_method:
            {
                const char *handles[] =
                {
                    "AUTO",		/* 0 */
                    "NR",		/* 1 */
                    "RFA",		/* 2 */
                    "MANR",		/* 3 */
                    "EVFTS",	/* 4 */
                    "MAEVFTS",	/* 5 */
                    "IGTS",		/* 6 - used to be ENERONLY? */
                    "QSD",		/* 7 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_negeval:
            {
                const char *handles[] = { "ABORT", "ABSVAL", "RFA", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_curvilinear:
            {
                const char *handles[] = { "OFF", "ON", "NO", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_vibration:
            {
                const char *handles[] = { "NO", "EXACT", "FINDIF_OLD", "FINDIF",
                    "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_integrals:
            {
                const char *handles[] = { "ARGOS", "VMOL", "HERMIT", "CADPAC",
                    "SEWARD", "GAMESS", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_points:
            {
                const char *handles[] = { "DOUBLE", "SINGLE", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_contraction:
            {
                const char *handles[] = { "SEGMENTED", "GENERAL", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_symmetry:
            {
                const char *handles[] = { "NONE", "OFF", "ON", "FULL",
                    ""};
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_spherical:
            {
                const char *handles[] = { "ON", "OFF", ""};
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_pert_orb:
            {
                const char *handles[] = { "STANDARD", "CANONICAL", "UNKNOWN", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_coordinates:
            {
                const char *handles[] = { "INTERNAL", "CARTESIAN", "XYZ2INT",
                    "AUTO", "" };
                
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_check_sym:
            {
                const char *handles[] = { "NORMAL", "OVERRIDE", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_trans_inv:
            {
                const char *handles[] = { "USE", "IGNORE", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_hfstability:
            {
                const char *handles[] = { "OFF", "ON", "FOLLOW", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_units:
            {
                const char *handles[] = { "ANGSTROM", "BOHR", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_fd_usegroup:
            {
                const char *handles[] = { "FULL", "COMP", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_fd_project:
            {
                const char *handles[] = { "ON", "OFF", "" }; /* WHY GOD!?! */
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_vtran:
            {
                const char *handles[] = { "FULL/PARTIAL", "FULL", "PARTIAL", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_hf2_file:
            {
                const char *handles[] = { "SKIP", "USE", "SAVE", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_subgroup:
            {
                const char *handles[] =
                {
                    "DEFAULT",	/* 0 */
                    "C1",		/* 1 */
                    "C2",		/* 2 */
                    "CS",		/* 3 */
                    "CI",		/* 4 */
                    "C2V",		/* 5 */
                    "C2H",		/* 6 */
                    "D2",		/* 7 */
                    "D2H",		/* 8 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_subgrpaxis:
            {
                const char *handles[] = { "X", "Y", "Z", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_excite:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "TDA",		/* 1 */
                    "RPA",		/* 2 */
                    "EOMEE",	/* 3 */
                    "-=EOMIP=-",	/* 4 - unused? */
                    "CIS",		/* 5 */
                    "CIS(D)",	/* 6 */
                    "P-EOMEE",	/* 7 */
                    "EOM-BWPT2",	/* 8 */
                    "STEOM",	/* 9 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_treat_pert:
            {
                const char *handles[] = { "SIMULTANEOUS", "SEQUENTIAL", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_estate_prop:
            {
                const char *handles[] = { "OFF",       "EXPECTATION",
                    "UNRELAXED", "DERIVATIVE",  "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_abcdtype:
            {
                const char *handles[] = { "STANDARD", "MULTIPASS", "AOBASIS", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_ao_ladders:
            {
                const char *handles[] = { "MULTIPASS", "SINGLEPASS", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_fock:
            {
                const char *handles[] = { "PK", "AO", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_gamma_abcd:
            {
                const char *handles[] = { "DISK", "DIRECT", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_zeta_type:
            {
                const char *handles[] = { "POPLE", "DIIS", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_frag:
            {
                const char *handles[] = { "OFF", "ON"};
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_ea_calc:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "MBPT(2)",	/* 1 */
                    "SO_DYSON",	/* 2 */
                    "OVGF",		/* 3 */
                    "P_EOMEA",	/* 4 */
                    "EA_EOMCC",	/* 5 */
                    "COMBO",	/* 6 */
                    "OS_CCSD",	/* 7 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_functional:
            {
                const char *handles[] =
                {
                    "XALPHA",	/* 0 */
                    "BECKE",	/* 1 */
                    "LYP",		/* 2 */
                    "XLYP",		/* 3 */
                    "BLYP",		/* 4 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_eomprop:
            {
                const char *handles[] = { "CILIKE", "LINEAR", "QUADRATIC", "COMBO",
                    "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_abcdfull:
            {
                const char *handles[] = { "UNKNOWN", "ON", "OFF", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_damp_typ:
            {
                const char *handles[] = { "NONE", "DAVIDSON", "OTHER", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_ip_calc:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "MBPT(2)",	/* 1 */
                    "SO_DYSON",	/* 2 */
                    "OVGF",		/* 3 */
                    "P_EOMIP",	/* 4 */
                    "IP_EOMCC",	/* 5 */
                    "COMBO",	/* 6 */
                    "OS_CCSD",	/* 7 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_ip_search:
            {
                const char *handles[] = { "VALENCE",  "LOWEST", "COREIP", "SHAKEUP",
                    "KOOPMANS", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_eomref:
            {
                const char *handles[] = { "NONE", "CCSD", "MBPT(2)", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_ee_search:
            {
                const char *handles[] = { "LOWEST", "CORE", "LUMO", "HOMO", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_eom_prjct:
            {
                const char *handles[] = { "NO", "SEARCH_ONLY", "PRJCT_ALL",
                    "PRJCT_NOISE", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_hbarabcd:
            case h_IOPPAR_hbarabci:
            {
                const char *handles[] = { "UNKNOWN", "OFF", "ON", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_nt3eomee:
            {
                const char *handles[] =
                {
                    "NONE",		/* 0 */
                    "NCCSDT-1",	/* 1 */
                    "F-NCCSDT-1",	/* 2 */
                    "NCCSDT-1b",	/* 3 */
                    "F-NCCSDT-1b",	/* 4 */
                    "CCSD(TPR)",	/* 5 */
                    "F-CCSD(TPR)",	/* 6 */
                    "CCSDR(T)",	/* 7 */
                    "F-CCSDR(T)",	/* 8 */
                    "CCSDR(3)",	/* 9 */
                    "F-CCSDR(3)",	/* 10 */
                    "NCCSDT-3",	/* 11 */
                    "F-NCCSDT-3",	/* 12 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_ks_pot:
            {
                const char *handles[] =
                {
                    "HF",		/* 0 */
                    "XALPHA",	/* 1 */
                    "LYP",		/* 2 */
                    "XALYP",	/* 3 */
                    "BLYP",		/* 4 */
                    "LDA",		/* 5 */
                    "B3LYP",	/* 6 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_dip_calc:
            case h_IOPPAR_dea_calc:
            {
                const char *handles[] = { "NONE", "TDA", "EOMCC", "STEOM",
                    "OS_CCSD", "SS_STEOM", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_program:
            {
                const char *handles[] = { "DEFAULT", "NOT_USED", "ACES2", "MN_A3",
                    "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_insertf:
            {
                const char *handles[] =
                {
                    "OFF",		/* 0 */
                    "SCF",		/* 1 */
                    "TDA",		/* 2 */
                    "CC",		/* 3 */
                    "EOM",		/* 4 */
                    "STEOM",	/* 5 */
                    ""
                };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_grad_calc:
            {
                const char *handles[] = { "NONE", "ANALYTICAL", "NUMERICAL",""};
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_prp_ints:
            {
                const char *handles[] = { "PARTIAL", "FULL", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_scf_type:
            {
                const char *handles[] = { "HF", "KS", "HFDFT", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
            case h_IOPPAR_noreori:
            {
                const char *handles[] = { "OFF", "ON", "AUTO", "" };
                output_keyword_value = input::asv_update_handle(index,input_string_value,*handles);
                break;
            }
                
                
        } /* end switch (*index) */
        
        return output_keyword_value;
    }

    int asv_update_handle(int index, std::string input_string_value, const char* handles) {
        int output_keyword_value = 1;
        return output_keyword_value=1;
    }
    
}
