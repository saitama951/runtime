/**
 * \file
 * Copyright 2003 Ximian, Inc
 * Copyright 2003-2011 Novell Inc
 * Copyright 2011 Xamarin Inc
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */
MINI_OP(OP_LOAD,	"load", NONE, NONE, NONE)
MINI_OP(OP_LDADDR,	"ldaddr", IREG, NONE, NONE)
MINI_OP(OP_STORE,	"store", NONE, NONE, NONE)
MINI_OP(OP_NOP,     "nop", NONE, NONE, NONE)
MINI_OP(OP_HARD_NOP,    "hard_nop", NONE, NONE, NONE)
MINI_OP(OP_RELAXED_NOP,     "relaxed_nop", NONE, NONE, NONE)
MINI_OP(OP_PHI,		"phi", IREG, NONE, NONE)
MINI_OP(OP_FPHI,	"fphi", FREG, NONE, NONE)
MINI_OP(OP_VPHI,	"vphi", VREG, NONE, NONE)
MINI_OP(OP_COMPARE,	"compare", NONE, IREG, IREG)
MINI_OP(OP_COMPARE_IMM,	"compare_imm", NONE, IREG, NONE)
MINI_OP(OP_FCOMPARE,	"fcompare", NONE, FREG, FREG)
MINI_OP(OP_RCOMPARE,	"rcompare", NONE, FREG, FREG)
MINI_OP(OP_LCOMPARE,	"lcompare", NONE, LREG, LREG)
MINI_OP(OP_ICOMPARE,	"icompare", NONE, IREG, IREG)
MINI_OP(OP_ICOMPARE_IMM,	"icompare_imm", NONE, IREG, NONE)
MINI_OP(OP_LCOMPARE_IMM,	"lcompare_imm", NONE, LREG, NONE)
MINI_OP(OP_LOCAL,	"local", NONE, NONE, NONE)
MINI_OP(OP_ARG,		"arg", NONE, NONE, NONE)
/* inst_imm contains the local index */
MINI_OP(OP_GSHAREDVT_LOCAL, "gsharedvt_local", NONE, NONE, NONE)
MINI_OP(OP_GSHAREDVT_ARG_REGOFFSET, "gsharedvt_arg_regoffset", NONE, NONE, NONE)
/*
 * Represents passing a valuetype argument which has not been decomposed yet.
 * inst_p0 points to the call.
 */
MINI_OP(OP_OUTARG_VT,	"outarg_vt", NONE, VREG, NONE)
MINI_OP(OP_OUTARG_VTRETADDR, "outarg_vtretaddr", IREG, NONE, NONE)
MINI_OP(OP_SETRET,	"setret", NONE, IREG, NONE)
MINI_OP(OP_SETFRET,	"setfret", FREG, FREG, NONE)
MINI_OP(OP_SETLRET,	"setlret", NONE, IREG, IREG)
MINI_OP(OP_LOCALLOC, "localloc", IREG, IREG, NONE)
MINI_OP(OP_LOCALLOC_IMM, "localloc_imm", IREG, NONE, NONE)
MINI_OP(OP_CHECK_THIS,	"check_this", NONE, IREG, NONE)
MINI_OP(OP_SEQ_POINT, "seq_point", NONE, NONE, NONE)
MINI_OP(OP_IL_SEQ_POINT, "il_seq_point", NONE, NONE, NONE)
MINI_OP(OP_IMPLICIT_EXCEPTION, "implicit_exception", NONE, NONE, NONE)
MINI_OP(OP_BOX, "box", IREG, IREG, NONE)
/* A box of the int value in inst_c0 */
MINI_OP(OP_BOX_ICONST, "box_iconst", IREG, NONE, NONE)
/* Same as OP_MOVE, ins->dreg is an objref of type ins->klass */
MINI_OP(OP_TYPED_OBJREF, "typed_objref", IREG, IREG, NONE)

/* CALL opcodes need to stay together, see MONO_IS_CALL macro */
MINI_OP(OP_VOIDCALL,	"voidcall", NONE, NONE, NONE)
MINI_OP(OP_VOIDCALL_REG,	"voidcall_reg", NONE, IREG, NONE)
MINI_OP(OP_VOIDCALL_MEMBASE,	"voidcall_membase", NONE, IREG, NONE)
MINI_OP(OP_CALL,        "call", IREG, NONE, NONE)
MINI_OP(OP_CALL_REG,	"call_reg", IREG, IREG, NONE)
MINI_OP(OP_CALL_MEMBASE,	"call_membase", IREG, IREG, NONE)
MINI_OP(OP_FCALL,	"fcall", FREG, NONE, NONE)
MINI_OP(OP_FCALL_REG,	"fcall_reg", FREG, IREG, NONE)
MINI_OP(OP_FCALL_MEMBASE,	"fcall_membase", FREG, IREG, NONE)
MINI_OP(OP_RCALL,	"rcall", FREG, NONE, NONE)
MINI_OP(OP_RCALL_REG,	"rcall_reg", FREG, IREG, NONE)
MINI_OP(OP_RCALL_MEMBASE,	"rcall_membase", FREG, IREG, NONE)
MINI_OP(OP_LCALL,	"lcall", LREG, NONE, NONE)
MINI_OP(OP_LCALL_REG,	"lcall_reg", LREG, IREG, NONE)
MINI_OP(OP_LCALL_MEMBASE,	"lcall_membase", LREG, IREG, NONE)
MINI_OP(OP_VCALL, 	"vcall", VREG, NONE, NONE)
MINI_OP(OP_VCALL_REG,	"vcall_reg", VREG, IREG, NONE)
MINI_OP(OP_VCALL_MEMBASE,	"vcall_membase", VREG, IREG, NONE)
MINI_OP(OP_XCALL, 	"xcall", XREG, NONE, NONE)
MINI_OP(OP_XCALL_REG,	"xcall_reg", XREG, IREG, NONE)
MINI_OP(OP_XCALL_MEMBASE,	"xcall_membase", XREG, IREG, NONE)
/* Represents the decomposed vcall which doesn't return a vtype no more */
MINI_OP(OP_VCALL2, 	"vcall2", NONE, NONE, NONE)
MINI_OP(OP_VCALL2_REG,	"vcall2_reg", NONE, IREG, NONE)
MINI_OP(OP_VCALL2_MEMBASE,	"vcall2_membase", NONE, IREG, NONE)
MINI_OP(OP_DYN_CALL, "dyn_call", NONE, IREG, IREG)

MINI_OP(OP_ICONST,	"iconst", IREG, NONE, NONE)
MINI_OP(OP_I8CONST,	"i8const", LREG, NONE, NONE)
MINI_OP(OP_R4CONST,	"r4const", FREG, NONE, NONE)
MINI_OP(OP_R8CONST,	"r8const", FREG, NONE, NONE)
MINI_OP(OP_DUMMY_ICONST, "dummy_iconst", IREG, NONE, NONE)
MINI_OP(OP_DUMMY_I8CONST, "dummy_i8const", LREG, NONE, NONE)
MINI_OP(OP_DUMMY_R8CONST, "dummy_r8const", FREG, NONE, NONE)
MINI_OP(OP_DUMMY_R4CONST, "dummy_r4const", FREG, NONE, NONE)
MINI_OP(OP_DUMMY_VZERO, "dummy_vzero", VREG, NONE, NONE)
MINI_OP(OP_REGVAR,	"regvar", NONE, NONE, NONE)
MINI_OP(OP_REGOFFSET,	"regoffset", NONE, NONE, NONE)
MINI_OP(OP_VTARG_ADDR,	"vtarg_addr", NONE, NONE, NONE)
MINI_OP(OP_LABEL,	"label", NONE, NONE, NONE)
MINI_OP(OP_SWITCH,  "switch", NONE, IREG, NONE)
MINI_OP(OP_THROW, "throw", NONE, IREG, NONE)
MINI_OP(OP_RETHROW,	"rethrow", NONE, IREG, NONE)

/*
 * Vararg calls are implemented as follows:
 * - the caller emits a hidden argument just before the varargs argument. this
 *   'signature cookie' argument contains the signature describing the call.
 * - all implicit arguments are passed in memory right after the signature cookie, i.e.
 *   the stack will look like this:
 *   <argn>
 *   ..
 *   <arg1>
 *   <sig cookie>
 * - the OP_ARGLIST opcode in the callee computes the address of the sig cookie argument
 *   on the stack and saves it into its sreg1.
 * - mono_ArgIterator_Setup receives this value and uses it to find the signature and
 *   the arguments.
 */
MINI_OP(OP_ARGLIST,	"arglist", NONE, IREG, NONE)

/* MONO_IS_STORE_MEMBASE depends on the order here */
MINI_OP(OP_STORE_MEMBASE_REG,"store_membase_reg", IREG, IREG, NONE)
MINI_OP(OP_STOREI1_MEMBASE_REG, "storei1_membase_reg", IREG, IREG, NONE)
MINI_OP(OP_STOREI2_MEMBASE_REG, "storei2_membase_reg", IREG, IREG, NONE)
MINI_OP(OP_STOREI4_MEMBASE_REG, "storei4_membase_reg", IREG, IREG, NONE)
MINI_OP(OP_STOREI8_MEMBASE_REG, "storei8_membase_reg", IREG, LREG, NONE)
MINI_OP(OP_STORER4_MEMBASE_REG, "storer4_membase_reg", IREG, FREG, NONE)
MINI_OP(OP_STORER8_MEMBASE_REG, "storer8_membase_reg", IREG, FREG, NONE)

#if defined(TARGET_X86) || defined(TARGET_AMD64)
MINI_OP(OP_STOREX_MEMBASE_REG, "storex_membase_reg", IREG, XREG, NONE)
MINI_OP(OP_STOREX_NTA_MEMBASE_REG,     "storex_nta_membase_reg", IREG, XREG, NONE)
#endif

MINI_OP(OP_STORE_MEMBASE_IMM,"store_membase_imm", IREG, NONE, NONE)
MINI_OP(OP_STOREI1_MEMBASE_IMM, "storei1_membase_imm", IREG, NONE, NONE)
MINI_OP(OP_STOREI2_MEMBASE_IMM, "storei2_membase_imm", IREG, NONE, NONE)
MINI_OP(OP_STOREI4_MEMBASE_IMM, "storei4_membase_imm", IREG, NONE, NONE)
MINI_OP(OP_STOREI8_MEMBASE_IMM, "storei8_membase_imm", IREG, NONE, NONE)
/* klass must be set to a simd class */
MINI_OP(OP_STOREX_MEMBASE,      	"storex_membase", IREG, XREG, NONE)
MINI_OP(OP_STOREV_MEMBASE,      "storev_membase", IREG, VREG, NONE)

/* MONO_IS_LOAD_MEMBASE depends on the order here */
MINI_OP(OP_LOAD_MEMBASE,	"load_membase", IREG, IREG, NONE)
MINI_OP(OP_LOADI1_MEMBASE,"loadi1_membase", IREG, IREG, NONE)
MINI_OP(OP_LOADU1_MEMBASE,"loadu1_membase", IREG, IREG, NONE)
MINI_OP(OP_LOADI2_MEMBASE,"loadi2_membase", IREG, IREG, NONE)
MINI_OP(OP_LOADU2_MEMBASE,"loadu2_membase", IREG, IREG, NONE)
MINI_OP(OP_LOADI4_MEMBASE,"loadi4_membase", IREG, IREG, NONE)
MINI_OP(OP_LOADU4_MEMBASE,"loadu4_membase", IREG, IREG, NONE)
MINI_OP(OP_LOADI8_MEMBASE,"loadi8_membase", LREG, IREG, NONE)
MINI_OP(OP_LOADR4_MEMBASE,"loadr4_membase", FREG, IREG, NONE)
MINI_OP(OP_LOADR8_MEMBASE,"loadr8_membase", FREG, IREG, NONE)

/* klass must be set to a simd class */
MINI_OP(OP_LOADX_MEMBASE, 			"loadx_membase", XREG, IREG, NONE)

#if defined(TARGET_X86) || defined(TARGET_AMD64) || defined(TARGET_S390X)
MINI_OP(OP_LOADX_ALIGNED_MEMBASE,  "loadx_aligned_membase", XREG, IREG, NONE)
MINI_OP(OP_STOREX_ALIGNED_MEMBASE_REG, "storex_aligned_membase_reg", IREG, XREG, NONE)
#endif

MINI_OP(OP_LOADV_MEMBASE,   "loadv_membase", VREG, IREG, NONE)

/* indexed loads: dreg = load at (sreg1 + sreg2)*/
MINI_OP(OP_LOAD_MEMINDEX,  "load_memindex", IREG, IREG, IREG)
MINI_OP(OP_LOADI1_MEMINDEX,"loadi1_memindex", IREG, IREG, IREG)
MINI_OP(OP_LOADU1_MEMINDEX,"loadu1_memindex", IREG, IREG, IREG)
MINI_OP(OP_LOADI2_MEMINDEX,"loadi2_memindex", IREG, IREG, IREG)
MINI_OP(OP_LOADU2_MEMINDEX,"loadu2_memindex", IREG, IREG, IREG)
MINI_OP(OP_LOADI4_MEMINDEX,"loadi4_memindex", IREG, IREG, IREG)
MINI_OP(OP_LOADU4_MEMINDEX,"loadu4_memindex", IREG, IREG, IREG)
MINI_OP(OP_LOADI8_MEMINDEX,"loadi8_memindex", IREG, IREG, IREG)
MINI_OP(OP_LOADR4_MEMINDEX,"loadr4_memindex", FREG, IREG, IREG)
MINI_OP(OP_LOADR8_MEMINDEX,"loadr8_memindex", FREG, IREG, IREG)
/* indexed stores: store sreg1 at (destbasereg + sreg2) */
/* MONO_IS_STORE_MEMINDEX depends on the order here */
MINI_OP(OP_STORE_MEMINDEX,"store_memindex", IREG, IREG, IREG)
MINI_OP(OP_STOREI1_MEMINDEX,"storei1_memindex", IREG, IREG, IREG)
MINI_OP(OP_STOREI2_MEMINDEX,"storei2_memindex", IREG, IREG, IREG)
MINI_OP(OP_STOREI4_MEMINDEX,"storei4_memindex", IREG, IREG, IREG)
MINI_OP(OP_STOREI8_MEMINDEX,"storei8_memindex", IREG, IREG, IREG)
MINI_OP(OP_STORER4_MEMINDEX,"storer4_memindex", IREG, FREG, IREG)
MINI_OP(OP_STORER8_MEMINDEX,"storer8_memindex", IREG, FREG, IREG)

MINI_OP(OP_LOAD_MEM,"load_mem", IREG, NONE, NONE)
MINI_OP(OP_LOADU1_MEM,"loadu1_mem", IREG, NONE, NONE)
MINI_OP(OP_LOADU2_MEM,"loadu2_mem", IREG, NONE, NONE)
MINI_OP(OP_LOADI4_MEM,"loadi4_mem", IREG, NONE, NONE)
MINI_OP(OP_LOADU4_MEM,"loadu4_mem", IREG, NONE, NONE)
MINI_OP(OP_LOADI8_MEM,"loadi8_mem", IREG, NONE, NONE)

MINI_OP(OP_MOVE,	"move", IREG, IREG, NONE)
MINI_OP(OP_LMOVE,	"lmove", LREG, LREG, NONE)
MINI_OP(OP_FMOVE,	"fmove", FREG, FREG, NONE)
MINI_OP(OP_VMOVE,   "vmove", VREG, VREG, NONE)
MINI_OP(OP_RMOVE,	"rmove", FREG, FREG, NONE)

/*
 * All 4 of these are only available when soft float isn't active. They
 * perform no conversions; they simply move values back and forth.
 */
MINI_OP(OP_MOVE_F_TO_I4, "move_f_to_i4", IREG, FREG, NONE)
MINI_OP(OP_MOVE_I4_TO_F, "move_i4_to_f", FREG, IREG, NONE)
/* These 2 are only available on 64-bit targets. */
MINI_OP(OP_MOVE_F_TO_I8, "move_f_to_i8", IREG, FREG, NONE)
MINI_OP(OP_MOVE_I8_TO_F, "move_i8_to_f", FREG, IREG, NONE)

MINI_OP(OP_VZERO,   "vzero", VREG, NONE, NONE)

MINI_OP(OP_ADD_IMM,    "add_imm", IREG, IREG, NONE)
MINI_OP(OP_SUB_IMM,    "sub_imm", IREG, IREG, NONE)
MINI_OP(OP_MUL_IMM,    "mul_imm", IREG, IREG, NONE)
MINI_OP(OP_DIV_IMM,    "div_imm", IREG, IREG, NONE)
MINI_OP(OP_DIV_UN_IMM, "div_un_imm", IREG, IREG, NONE)
MINI_OP(OP_REM_IMM,    "rem_imm", IREG, IREG, NONE)
MINI_OP(OP_REM_UN_IMM, "rem_un_imm", IREG, IREG, NONE)
MINI_OP(OP_AND_IMM,    "and_imm", IREG, IREG, NONE)
MINI_OP(OP_OR_IMM,     "or_imm", IREG, IREG, NONE)
MINI_OP(OP_XOR_IMM,    "xor_imm", IREG, IREG, NONE)
MINI_OP(OP_SHL_IMM,    "shl_imm", IREG, IREG, NONE)
MINI_OP(OP_SHR_IMM,    "shr_imm", IREG, IREG, NONE)
MINI_OP(OP_SHR_UN_IMM, "shr_un_imm", IREG, IREG, NONE)

MINI_OP(OP_BR,         "br", NONE, NONE, NONE)
/* Similar to old OP_JMP, but the passing of arguments is done similarly to calls */
MINI_OP(OP_TAILCALL,   "tailcall", NONE, NONE, NONE)
MINI_OP(OP_TAILCALL_PARAMETER, "tailcall_parameter", NONE, NONE, NONE) // no code, just size
MINI_OP(OP_TAILCALL_REG, "tailcall_reg", NONE, IREG, NONE)
MINI_OP(OP_TAILCALL_MEMBASE, "tailcall_membase", NONE, IREG, NONE)
MINI_OP(OP_BREAK,      "break", NONE, NONE, NONE)

MINI_OP(OP_CEQ,   "ceq", IREG, NONE, NONE)
MINI_OP(OP_CGT,   "cgt", IREG, NONE, NONE)
MINI_OP(OP_CGT_UN,"cgt_un", IREG, NONE, NONE)
MINI_OP(OP_CLT,   "clt", IREG, NONE, NONE)
MINI_OP(OP_CLT_UN,"clt_un", IREG, NONE, NONE)

/* exceptions: must be in the same order as the matching CEE_ branch opcodes */
MINI_OP(OP_COND_EXC_EQ, "cond_exc_eq", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_GE, "cond_exc_ge", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_GT, "cond_exc_gt", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_LE, "cond_exc_le", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_LT, "cond_exc_lt", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_NE_UN, "cond_exc_ne_un", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_GE_UN, "cond_exc_ge_un", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_GT_UN, "cond_exc_gt_un", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_LE_UN, "cond_exc_le_un", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_LT_UN, "cond_exc_lt_un", NONE, NONE, NONE)

MINI_OP(OP_COND_EXC_OV, "cond_exc_ov", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_NO, "cond_exc_no", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_C, "cond_exc_c", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_NC, "cond_exc_nc", NONE, NONE, NONE)

MINI_OP(OP_COND_EXC_IEQ, "cond_exc_ieq", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_IGE, "cond_exc_ige", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_IGT, "cond_exc_igt", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_ILE, "cond_exc_ile", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_ILT, "cond_exc_ilt", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_INE_UN, "cond_exc_ine_un", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_IGE_UN, "cond_exc_ige_un", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_IGT_UN, "cond_exc_igt_un", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_ILE_UN, "cond_exc_ile_un", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_ILT_UN, "cond_exc_ilt_un", NONE, NONE, NONE)

MINI_OP(OP_COND_EXC_IOV, "cond_exc_iov", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_INO, "cond_exc_ino", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_IC, "cond_exc_ic", NONE, NONE, NONE)
MINI_OP(OP_COND_EXC_INC, "cond_exc_inc", NONE, NONE, NONE)

/* 64 bit opcodes: must be in the same order as the matching CEE_ opcodes: binops_op_map */
MINI_OP(OP_LADD,    "long_add", LREG, LREG, LREG)
MINI_OP(OP_LSUB,    "long_sub", LREG, LREG, LREG)
MINI_OP(OP_LMUL,    "long_mul", LREG, LREG, LREG)
MINI_OP(OP_LDIV,    "long_div", LREG, LREG, LREG)
MINI_OP(OP_LDIV_UN, "long_div_un", LREG, LREG, LREG)
MINI_OP(OP_LREM,    "long_rem", LREG, LREG, LREG)
MINI_OP(OP_LREM_UN, "long_rem_un", LREG, LREG, LREG)
MINI_OP(OP_LAND,    "long_and", LREG, LREG, LREG)
MINI_OP(OP_LOR,     "long_or", LREG, LREG, LREG)
MINI_OP(OP_LXOR,    "long_xor", LREG, LREG, LREG)
MINI_OP(OP_LSHL,    "long_shl", LREG, LREG, IREG)
MINI_OP(OP_LSHR,    "long_shr", LREG, LREG, IREG)
MINI_OP(OP_LSHR_UN, "long_shr_un", LREG, LREG, IREG)

/* 64 bit opcodes: must be in the same order as the matching CEE_ opcodes: unops_op_map */
MINI_OP(OP_LNEG,       "long_neg", LREG, LREG, NONE)
MINI_OP(OP_LNOT,       "long_not", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_I1,"long_conv_to_i1", IREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_I2,"long_conv_to_i2", IREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_I4,"long_conv_to_i4", IREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_I8,"long_conv_to_i8", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_R4,"long_conv_to_r4", FREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_R8,"long_conv_to_r8", FREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_U4,"long_conv_to_u4", IREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_U8,"long_conv_to_u8", LREG, LREG, NONE)

MINI_OP(OP_LCONV_TO_U2,   "long_conv_to_u2", IREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_U1,   "long_conv_to_u1", IREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_I,    "long_conv_to_i", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_I,"long_conv_to_ovf_i", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U,"long_conv_to_ovf_u", LREG, LREG, NONE)

/* inst_exc_name contains the exception name to throw */
MINI_OP(OP_LADD_OVF,      "long_add_ovf", LREG, LREG, LREG)
MINI_OP(OP_LADD_OVF_UN,   "long_add_ovf_un", LREG, LREG, LREG)
MINI_OP(OP_LMUL_OVF,      "long_mul_ovf", LREG, LREG, LREG)
MINI_OP(OP_LMUL_OVF_UN,   "long_mul_ovf_un", LREG, LREG, LREG)
MINI_OP(OP_LSUB_OVF,      "long_sub_ovf", LREG, LREG, LREG)
MINI_OP(OP_LSUB_OVF_UN,   "long_sub_ovf_un", LREG, LREG, LREG)

MINI_OP(OP_LCONV_TO_OVF_I1_UN,"long_conv_to_ovf_i1_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_I2_UN,"long_conv_to_ovf_i2_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_I4_UN,"long_conv_to_ovf_i4_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_I8_UN,"long_conv_to_ovf_i8_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U1_UN,"long_conv_to_ovf_u1_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U2_UN,"long_conv_to_ovf_u2_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U4_UN,"long_conv_to_ovf_u4_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U8_UN,"long_conv_to_ovf_u8_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_I_UN, "long_conv_to_ovf_i_un", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U_UN, "long_conv_to_ovf_u_un", LREG, LREG, NONE)

MINI_OP(OP_LCONV_TO_OVF_I1,"long_conv_to_ovf_i1", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U1,"long_conv_to_ovf_u1", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_I2,"long_conv_to_ovf_i2", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U2,"long_conv_to_ovf_u2", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_I4,"long_conv_to_ovf_i4", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U4,"long_conv_to_ovf_u4", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_I8,"long_conv_to_ovf_i8", LREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_OVF_U8,"long_conv_to_ovf_u8", LREG, LREG, NONE)

/* mono_decompose_long_opts () depends on the order here */
MINI_OP(OP_LCEQ,   "long_ceq", IREG, NONE, NONE)
MINI_OP(OP_LCGT,   "long_cgt", IREG, NONE, NONE)
MINI_OP(OP_LCGT_UN,"long_cgt_un", IREG, NONE, NONE)
MINI_OP(OP_LCLT,   "long_clt", IREG, NONE, NONE)
MINI_OP(OP_LCLT_UN,"long_clt_un", IREG, NONE, NONE)

MINI_OP(OP_LCONV_TO_R_UN,"long_conv_to_r_un", FREG, LREG, NONE)
MINI_OP(OP_LCONV_TO_U,   "long_conv_to_u", IREG, LREG, NONE)

MINI_OP(OP_LADD_IMM,    "long_add_imm", LREG, LREG, NONE)
MINI_OP(OP_LSUB_IMM,    "long_sub_imm", LREG, LREG, NONE)
MINI_OP(OP_LMUL_IMM,    "long_mul_imm", LREG, LREG, NONE)
MINI_OP(OP_LAND_IMM,    "long_and_imm", LREG, LREG, NONE)
MINI_OP(OP_LOR_IMM,     "long_or_imm", LREG, LREG, NONE)
MINI_OP(OP_LXOR_IMM,    "long_xor_imm", LREG, LREG, NONE)
MINI_OP(OP_LSHL_IMM,    "long_shl_imm", LREG, LREG, NONE)
MINI_OP(OP_LSHR_IMM,    "long_shr_imm", LREG, LREG, NONE)
MINI_OP(OP_LSHR_UN_IMM, "long_shr_un_imm", LREG, LREG, NONE)
MINI_OP(OP_LDIV_IMM,    "long_div_imm", LREG, LREG, NONE)
MINI_OP(OP_LDIV_UN_IMM, "long_div_un_imm", LREG, LREG, NONE)
MINI_OP(OP_LREM_IMM,    "long_rem_imm", LREG, LREG, NONE)
MINI_OP(OP_LREM_UN_IMM, "long_rem_un_imm", LREG, LREG, NONE)

/* mono_decompose_long_opts () depends on the order here */
MINI_OP(OP_LBEQ,    "long_beq", NONE, NONE, NONE)
MINI_OP(OP_LBGE,    "long_bge", NONE, NONE, NONE)
MINI_OP(OP_LBGT,    "long_bgt", NONE, NONE, NONE)
MINI_OP(OP_LBLE,    "long_ble", NONE, NONE, NONE)
MINI_OP(OP_LBLT,    "long_blt", NONE, NONE, NONE)
MINI_OP(OP_LBNE_UN, "long_bne_un", NONE, NONE, NONE)
MINI_OP(OP_LBGE_UN, "long_bge_un", NONE, NONE, NONE)
MINI_OP(OP_LBGT_UN, "long_bgt_un", NONE, NONE, NONE)
MINI_OP(OP_LBLE_UN, "long_ble_un", NONE, NONE, NONE)
MINI_OP(OP_LBLT_UN, "long_blt_un", NONE, NONE, NONE)

/* Variants of the original opcodes which take the two parts of the long as two arguments */
MINI_OP(OP_LCONV_TO_R8_2,"long_conv_to_r8_2", FREG, IREG, IREG)
MINI_OP(OP_LCONV_TO_R4_2,"long_conv_to_r4_2", FREG, IREG, IREG)
MINI_OP(OP_LCONV_TO_R_UN_2,"long_conv_to_r_un_2", FREG, IREG, IREG)
MINI_OP(OP_LCONV_TO_OVF_I4_2,"long_conv_to_ovf_i4_2", IREG, IREG, IREG)

/* 32 bit opcodes: must be in the same order as the matching CEE_ opcodes: binops_op_map */
MINI_OP(OP_IADD,    "int_add", IREG, IREG, IREG)
MINI_OP(OP_ISUB,    "int_sub", IREG, IREG, IREG)
MINI_OP(OP_IMUL,    "int_mul", IREG, IREG, IREG)
MINI_OP(OP_IDIV,    "int_div", IREG, IREG, IREG)
MINI_OP(OP_IDIV_UN, "int_div_un", IREG, IREG, IREG)
MINI_OP(OP_IREM,    "int_rem", IREG, IREG, IREG)
MINI_OP(OP_IREM_UN, "int_rem_un", IREG, IREG, IREG)
MINI_OP(OP_IAND,    "int_and", IREG, IREG, IREG)
MINI_OP(OP_IOR,     "int_or", IREG, IREG, IREG)
MINI_OP(OP_IXOR,    "int_xor", IREG, IREG, IREG)
MINI_OP(OP_ISHL,    "int_shl", IREG, IREG, IREG)
MINI_OP(OP_ISHR,    "int_shr", IREG, IREG, IREG)
MINI_OP(OP_ISHR_UN, "int_shr_un", IREG, IREG, IREG)

/* 32 bit opcodes: must be in the same order as the matching CEE_ opcodes: unops_op_map */
MINI_OP(OP_INEG,       "int_neg", IREG, IREG, NONE)
MINI_OP(OP_INOT,       "int_not", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_I1,"int_conv_to_i1", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_I2,"int_conv_to_i2", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_I4,"int_conv_to_i4", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_I8,"int_conv_to_i8", LREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_R4,"int_conv_to_r4", FREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_R8,"int_conv_to_r8", FREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_U4,"int_conv_to_u4", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_U8,"int_conv_to_u8", LREG, IREG, NONE)

MINI_OP(OP_ICONV_TO_R_UN, "int_conv_to_r_un", FREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_U,   "int_conv_to_u", IREG, IREG, NONE)

/* 32 bit opcodes: must be in the same order as the matching CEE_ opcodes: ovfops_op_map */
MINI_OP(OP_ICONV_TO_U2,   "int_conv_to_u2", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_U1,   "int_conv_to_u1", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_I,    "int_conv_to_i", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_I,"int_conv_to_ovf_i", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U,"int_conv_to_ovf_u", IREG, IREG, NONE)

/* inst_exc_name contains the exception name to throw */
MINI_OP(OP_IADD_OVF,      "int_add_ovf", IREG, IREG, IREG)
MINI_OP(OP_IADD_OVF_UN,   "int_add_ovf_un", IREG, IREG, IREG)
MINI_OP(OP_IMUL_OVF,      "int_mul_ovf", IREG, IREG, IREG)
MINI_OP(OP_IMUL_OVF_UN,   "int_mul_ovf_un", IREG, IREG, IREG)
MINI_OP(OP_ISUB_OVF,      "int_sub_ovf", IREG, IREG, IREG)
MINI_OP(OP_ISUB_OVF_UN,   "int_sub_ovf_un", IREG, IREG, IREG)

/* 32 bit opcodes: must be in the same order as the matching CEE_ opcodes: ovf2ops_op_map */
MINI_OP(OP_ICONV_TO_OVF_I1_UN,"int_conv_to_ovf_i1_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_I2_UN,"int_conv_to_ovf_i2_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_I4_UN,"int_conv_to_ovf_i4_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_I8_UN,"int_conv_to_ovf_i8_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U1_UN,"int_conv_to_ovf_u1_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U2_UN,"int_conv_to_ovf_u2_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U4_UN,"int_conv_to_ovf_u4_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U8_UN,"int_conv_to_ovf_u8_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_I_UN, "int_conv_to_ovf_i_un", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U_UN, "int_conv_to_ovf_u_un", IREG, IREG, NONE)

/* 32 bit opcodes: must be in the same order as the matching CEE_ opcodes: ovf3ops_op_map */
MINI_OP(OP_ICONV_TO_OVF_I1,"int_conv_to_ovf_i1", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U1,"int_conv_to_ovf_u1", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_I2,"int_conv_to_ovf_i2", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U2,"int_conv_to_ovf_u2", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_I4,"int_conv_to_ovf_i4", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U4,"int_conv_to_ovf_u4", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_I8,"int_conv_to_ovf_i8", IREG, IREG, NONE)
MINI_OP(OP_ICONV_TO_OVF_U8,"int_conv_to_ovf_u8", IREG, IREG, NONE)

MINI_OP(OP_IADC,     "int_adc", IREG, IREG, IREG)
MINI_OP(OP_IADC_IMM, "int_adc_imm", IREG, IREG, NONE)
MINI_OP(OP_ISBB,     "int_sbb", IREG, IREG, IREG)
MINI_OP(OP_ISBB_IMM, "int_sbb_imm", IREG, IREG, NONE)
MINI_OP(OP_IADDCC,   "int_addcc", IREG, IREG, IREG)
MINI_OP(OP_ISUBCC,   "int_subcc", IREG, IREG, IREG)

MINI_OP(OP_IADD_IMM,    "int_add_imm", IREG, IREG, NONE)
MINI_OP(OP_ISUB_IMM,    "int_sub_imm", IREG, IREG, NONE)
MINI_OP(OP_IMUL_IMM,    "int_mul_imm", IREG, IREG, NONE)
MINI_OP(OP_IDIV_IMM,    "int_div_imm", IREG, IREG, NONE)
MINI_OP(OP_IDIV_UN_IMM, "int_div_un_imm", IREG, IREG, NONE)
MINI_OP(OP_IREM_IMM,    "int_rem_imm", IREG, IREG, NONE)
MINI_OP(OP_IREM_UN_IMM, "int_rem_un_imm", IREG, IREG, NONE)
MINI_OP(OP_IAND_IMM,    "int_and_imm", IREG, IREG, NONE)
MINI_OP(OP_IOR_IMM,     "int_or_imm", IREG, IREG, NONE)
MINI_OP(OP_IXOR_IMM,    "int_xor_imm", IREG, IREG, NONE)
MINI_OP(OP_ISHL_IMM,    "int_shl_imm", IREG, IREG, NONE)
MINI_OP(OP_ISHR_IMM,    "int_shr_imm", IREG, IREG, NONE)
MINI_OP(OP_ISHR_UN_IMM, "int_shr_un_imm", IREG, IREG, NONE)

MINI_OP(OP_ICEQ,   "int_ceq", IREG, NONE, NONE)
MINI_OP(OP_ICGT,   "int_cgt", IREG, NONE, NONE)
MINI_OP(OP_ICGT_UN,"int_cgt_un", IREG, NONE, NONE)
MINI_OP(OP_ICLT,   "int_clt", IREG, NONE, NONE)
MINI_OP(OP_ICLT_UN,"int_clt_un", IREG, NONE, NONE)

MINI_OP(OP_ICNEQ,  "int_cneq", IREG, NONE, NONE)
MINI_OP(OP_ICGE,   "int_cge", IREG, NONE, NONE)
MINI_OP(OP_ICLE,   "int_cle", IREG, NONE, NONE)
MINI_OP(OP_ICGE_UN,"int_cge_un", IREG, NONE, NONE)
MINI_OP(OP_ICLE_UN,"int_cle_un", IREG, NONE, NONE)

MINI_OP(OP_IBEQ,    "int_beq", NONE, NONE, NONE)
MINI_OP(OP_IBGE,    "int_bge", NONE, NONE, NONE)
MINI_OP(OP_IBGT,    "int_bgt", NONE, NONE, NONE)
MINI_OP(OP_IBLE,    "int_ble", NONE, NONE, NONE)
MINI_OP(OP_IBLT,    "int_blt", NONE, NONE, NONE)
MINI_OP(OP_IBNE_UN, "int_bne_un", NONE, NONE, NONE)
MINI_OP(OP_IBGE_UN, "int_bge_un", NONE, NONE, NONE)
MINI_OP(OP_IBGT_UN, "int_bgt_un", NONE, NONE, NONE)
MINI_OP(OP_IBLE_UN, "int_ble_un", NONE, NONE, NONE)
MINI_OP(OP_IBLT_UN, "int_blt_un", NONE, NONE, NONE)

MINI_OP(OP_FBEQ,    "float_beq", NONE, NONE, NONE)
MINI_OP(OP_FBGE,    "float_bge", NONE, NONE, NONE)
MINI_OP(OP_FBGT,    "float_bgt", NONE, NONE, NONE)
MINI_OP(OP_FBLE,    "float_ble", NONE, NONE, NONE)
MINI_OP(OP_FBLT,    "float_blt", NONE, NONE, NONE)
MINI_OP(OP_FBNE_UN, "float_bne_un", NONE, NONE, NONE)
MINI_OP(OP_FBGE_UN, "float_bge_un", NONE, NONE, NONE)
MINI_OP(OP_FBGT_UN, "float_bgt_un", NONE, NONE, NONE)
MINI_OP(OP_FBLE_UN, "float_ble_un", NONE, NONE, NONE)
MINI_OP(OP_FBLT_UN, "float_blt_un", NONE, NONE, NONE)

MINI_OP(OP_RBEQ,    "r4_beq", NONE, NONE, NONE)
MINI_OP(OP_RBGE,    "r4_bge", NONE, NONE, NONE)
MINI_OP(OP_RBGT,    "r4_bgt", NONE, NONE, NONE)
MINI_OP(OP_RBLE,    "r4_ble", NONE, NONE, NONE)
MINI_OP(OP_RBLT,    "r4_blt", NONE, NONE, NONE)
MINI_OP(OP_RBNE_UN, "r4_bne_un", NONE, NONE, NONE)
MINI_OP(OP_RBGE_UN, "r4_bge_un", NONE, NONE, NONE)
MINI_OP(OP_RBGT_UN, "r4_bgt_un", NONE, NONE, NONE)
MINI_OP(OP_RBLE_UN, "r4_ble_un", NONE, NONE, NONE)
MINI_OP(OP_RBLT_UN, "r4_blt_un", NONE, NONE, NONE)

/* float opcodes: must be in the same order as the matching CEE_ opcodes: binops_op_map */
MINI_OP(OP_FADD,   "float_add", FREG, FREG, FREG)
MINI_OP(OP_FSUB,   "float_sub", FREG, FREG, FREG)
MINI_OP(OP_FMUL,   "float_mul", FREG, FREG, FREG)
MINI_OP(OP_FDIV,   "float_div", FREG, FREG, FREG)
MINI_OP(OP_FDIV_UN,"float_div_un", FREG, FREG, FREG)
MINI_OP(OP_FREM,   "float_rem", FREG, FREG, FREG)
MINI_OP(OP_FREM_UN,"float_rem_un", FREG, FREG, FREG)

/* r4 opcodes: must be in the same order as the matching CEE_ opcodes: binops_op_map */
MINI_OP(OP_RADD,   "r4_add", FREG, FREG, FREG)
MINI_OP(OP_RSUB,   "r4_sub", FREG, FREG, FREG)
MINI_OP(OP_RMUL,   "r4_mul", FREG, FREG, FREG)
MINI_OP(OP_RDIV,   "r4_div", FREG, FREG, FREG)
MINI_OP(OP_RDIV_UN,"r4_div_un", FREG, FREG, FREG)
MINI_OP(OP_RREM,   "r4_rem", FREG, FREG, FREG)
MINI_OP(OP_RREM_UN,"r4_rem_un", FREG, FREG, FREG)

/* float opcodes: must be in the same order as the matching CEE_ opcodes: unops_op_map */
MINI_OP(OP_FNEG,       "float_neg", FREG, FREG, NONE)
MINI_OP(OP_FNOT,       "float_not", FREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_I1,"float_conv_to_i1", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_I2,"float_conv_to_i2", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_I4,"float_conv_to_i4", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_I8,"float_conv_to_i8", LREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_R4,"float_conv_to_r4", FREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_R8,"float_conv_to_r8", FREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_U4,"float_conv_to_u4", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_U8,"float_conv_to_u8", LREG, FREG, NONE)

MINI_OP(OP_RNEG,       "r4_neg", FREG, FREG, NONE)
MINI_OP(OP_RNOT,       "r4_not", FREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_I1,"r4_conv_to_i1", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_I2,"r4_conv_to_i2", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_I4,"r4_conv_to_i4", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_I8,"r4_conv_to_i8", LREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_R4,"r4_conv_to_r4", FREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_R8,"r4_conv_to_r8", FREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_U4,"r4_conv_to_u4", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_U8,"r4_conv_to_u8", LREG, FREG, NONE)

/* float opcodes: must be in the same order as the matching CEE_ opcodes: ovfops_op_map */
MINI_OP(OP_FCONV_TO_U2,   "float_conv_to_u2", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_U1,   "float_conv_to_u1", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_I,    "float_conv_to_i", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_I,"float_conv_to_ovf_i", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U,"float_conv_to_ovd_u", IREG, FREG, NONE)

/* float opcodes: must be in the same order as the matching CEE_ opcodes: ovfops_op_map */
MINI_OP(OP_FADD_OVF,      "float_add_ovf", FREG, FREG, FREG)
MINI_OP(OP_FADD_OVF_UN,   "float_add_ovf_un", FREG, FREG, FREG)
MINI_OP(OP_FMUL_OVF,      "float_mul_ovf", FREG, FREG, FREG)
MINI_OP(OP_FMUL_OVF_UN,   "float_mul_ovf_un", FREG, FREG, FREG)
MINI_OP(OP_FSUB_OVF,      "float_sub_ovf", FREG, FREG, FREG)
MINI_OP(OP_FSUB_OVF_UN,   "float_sub_ovf_un", FREG, FREG, FREG)

MINI_OP(OP_FCONV_TO_OVF_I1_UN,"float_conv_to_ovf_i1_un", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_I2_UN,"float_conv_to_ovf_i2_un", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_I4_UN,"float_conv_to_ovf_i4_un", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_I8_UN,"float_conv_to_ovf_i8_un", LREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U1_UN,"float_conv_to_ovf_u1_un", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U2_UN,"float_conv_to_ovf_u2_un", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U4_UN,"float_conv_to_ovf_u4_un", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U8_UN,"float_conv_to_ovf_u8_un", LREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_I_UN, "float_conv_to_ovf_i_un", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U_UN, "float_conv_to_ovf_u_un", IREG, FREG, NONE)

MINI_OP(OP_FCONV_TO_OVF_I1,"float_conv_to_ovf_i1", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U1,"float_conv_to_ovf_u1", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_I2,"float_conv_to_ovf_i2", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U2,"float_conv_to_ovf_u2", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_I4,"float_conv_to_ovf_i4", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U4,"float_conv_to_ovf_u4", IREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_I8,"float_conv_to_ovf_i8", LREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_OVF_U8,"float_conv_to_ovf_u8", LREG, FREG, NONE)

/* These do the comparison too */
MINI_OP(OP_FCEQ,   "float_ceq", IREG, FREG, FREG)
MINI_OP(OP_FCGT,   "float_cgt", IREG, FREG, FREG)
MINI_OP(OP_FCGT_UN,"float_cgt_un", IREG, FREG, FREG)
MINI_OP(OP_FCLT,   "float_clt", IREG, FREG, FREG)
MINI_OP(OP_FCLT_UN,"float_clt_un", IREG, FREG, FREG)

MINI_OP(OP_FCNEQ,  "float_cneq", IREG, FREG, FREG)
MINI_OP(OP_FCGE,   "float_cge", IREG, FREG, FREG)
MINI_OP(OP_FCLE,   "float_cle", IREG, FREG, FREG)

MINI_OP(OP_FCEQ_MEMBASE,   "float_ceq_membase", IREG, FREG, IREG)
MINI_OP(OP_FCGT_MEMBASE,   "float_cgt_membase", IREG, FREG, IREG)
MINI_OP(OP_FCGT_UN_MEMBASE,"float_cgt_un_membase", IREG, FREG, IREG)
MINI_OP(OP_FCLT_MEMBASE,   "float_clt_membase", IREG, FREG, IREG)
MINI_OP(OP_FCLT_UN_MEMBASE,"float_clt_un_membase", IREG, FREG, IREG)

MINI_OP(OP_CKFINITE, "ckfinite", FREG, FREG, NONE)

/* r4 opcodes: must be in the same order as the matching CEE_ opcodes: ovfops_op_map */
MINI_OP(OP_RCONV_TO_U2,   "r4_conv_to_u2", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_U1,   "r4_conv_to_u1", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_I,    "r4_conv_to_i", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_I,"r4_conv_to_ovf_i", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U,"r4_conv_to_ovd_u", IREG, FREG, NONE)

/* r4 opcodes: must be in the same order as the matching CEE_ opcodes: ovfops_op_map */
MINI_OP(OP_RADD_OVF,      "r4_add_ovf", FREG, FREG, FREG)
MINI_OP(OP_RADD_OVF_UN,   "r4_add_ovf_un", FREG, FREG, FREG)
MINI_OP(OP_RMUL_OVF,      "r4_mul_ovf", FREG, FREG, FREG)
MINI_OP(OP_RMUL_OVF_UN,   "r4_mul_ovf_un", FREG, FREG, FREG)
MINI_OP(OP_RSUB_OVF,      "r4_sub_ovf", FREG, FREG, FREG)
MINI_OP(OP_RSUB_OVF_UN,   "r4_sub_ovf_un", FREG, FREG, FREG)

MINI_OP(OP_RCONV_TO_OVF_I1_UN,"r4_conv_to_ovf_i1_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_I2_UN,"r4_conv_to_ovf_i2_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_I4_UN,"r4_conv_to_ovf_i4_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_I8_UN,"r4_conv_to_ovf_i8_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U1_UN,"r4_conv_to_ovf_u1_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U2_UN,"r4_conv_to_ovf_u2_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U4_UN,"r4_conv_to_ovf_u4_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U8_UN,"r4_conv_to_ovf_u8_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_I_UN, "r4_conv_to_ovf_i_un", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U_UN, "r4_conv_to_ovf_u_un", IREG, FREG, NONE)

MINI_OP(OP_RCONV_TO_OVF_I1,"r4_conv_to_ovf_i1", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U1,"r4_conv_to_ovf_u1", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_I2,"r4_conv_to_ovf_i2", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U2,"r4_conv_to_ovf_u2", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_I4,"r4_conv_to_ovf_i4", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U4,"r4_conv_to_ovf_u4", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_I8,"r4_conv_to_ovf_i8", IREG, FREG, NONE)
MINI_OP(OP_RCONV_TO_OVF_U8,"r4_conv_to_ovf_u8", IREG, FREG, NONE)

/* r4 opcodes: must be in the same order as the matching CEE_ opcodes: ceqops_op_map */
MINI_OP(OP_RCEQ,   "r4_ceq", IREG, FREG, FREG)
MINI_OP(OP_RCGT,   "r4_cgt", IREG, FREG, FREG)
MINI_OP(OP_RCGT_UN,"r4_cgt_un", IREG, FREG, FREG)
MINI_OP(OP_RCLT,   "r4_clt", IREG, FREG, FREG)
MINI_OP(OP_RCLT_UN,"r4_clt_un", IREG, FREG, FREG)

MINI_OP(OP_RCNEQ,  "r4_cneq", IREG, FREG, FREG)
MINI_OP(OP_RCGE,   "r4_cge", IREG, FREG, FREG)
MINI_OP(OP_RCLE,   "r4_cle", IREG, FREG, FREG)

/* Return the low 32 bits of a double vreg */
MINI_OP(OP_FGETLOW32, "float_getlow32", IREG, FREG, NONE)
/* Return the high 32 bits of a double vreg */
MINI_OP(OP_FGETHIGH32, "float_gethigh32", IREG, FREG, NONE)

MINI_OP(OP_JUMP_TABLE, "jump_table", IREG, NONE, NONE)

/* Same as OP_IMUL_OVF_UN, but throws an OutOfMemoryException */
/* Emulated */
MINI_OP(OP_IMUL_OVF_UN_OOM,   "int_mul_ovf_un_oom", IREG, IREG, IREG)
MINI_OP(OP_LMUL_OVF_UN_OOM,   "long_mul_ovf_un_oom", LREG, LREG, LREG)

/* aot compiler */
MINI_OP(OP_AOTCONST, "aotconst", IREG, NONE, NONE)
MINI_OP(OP_PATCH_INFO, "patch_info", NONE, NONE, NONE)
MINI_OP(OP_GOT_ENTRY, "got_entry", IREG, IREG, NONE)

/* exception related opcodes */
MINI_OP(OP_CALL_HANDLER  , "call_handler", NONE, NONE, NONE)
MINI_OP(OP_START_HANDLER  , "start_handler", NONE, NONE, NONE)
MINI_OP(OP_ENDFILTER,  "endfilter", NONE, IREG, NONE)
MINI_OP(OP_ENDFINALLY,  "endfinally", NONE, NONE, NONE)
/*
 * Returns the exception object passed to catch clauses in
 * by the EH code in a register.
 */
MINI_OP(OP_GET_EX_OBJ, "get_ex_obj", IREG, NONE, NONE)

/* inline (long)int * (long)int */
MINI_OP(OP_BIGMUL, "bigmul", LREG, IREG, IREG)
MINI_OP(OP_BIGMUL_UN, "bigmul_un", LREG, IREG, IREG)
MINI_OP(OP_IMIN_UN, "int_min_un", IREG, IREG, IREG)
MINI_OP(OP_IMAX_UN, "int_max_un", IREG, IREG, IREG)
MINI_OP(OP_LMIN_UN, "long_min_un", LREG, LREG, LREG)
MINI_OP(OP_LMAX_UN, "long_max_un", LREG, LREG, LREG)

MINI_OP(OP_MIN, "min", IREG, IREG, IREG)
MINI_OP(OP_MAX, "max", IREG, IREG, IREG)

MINI_OP(OP_IMIN, "int_min", IREG, IREG, IREG)
MINI_OP(OP_IMAX, "int_max", IREG, IREG, IREG)
MINI_OP(OP_LMIN, "long_min", LREG, LREG, LREG)
MINI_OP(OP_LMAX, "long_max", LREG, LREG, LREG)
MINI_OP(OP_RMAX,     "rmax", FREG, FREG, FREG)
MINI_OP(OP_RMIN,     "rmin", FREG, FREG, FREG)
MINI_OP(OP_RPOW,     "rpow", FREG, FREG, FREG)
MINI_OP(OP_FMAX,     "fmax", FREG, FREG, FREG)
MINI_OP(OP_FMIN,     "fmin", FREG, FREG, FREG)
MINI_OP(OP_FPOW,     "fpow", FREG, FREG, FREG)
MINI_OP(OP_RCOPYSIGN,"rcopysign", FREG, FREG, FREG)
MINI_OP(OP_FCOPYSIGN,"fcopysign", FREG, FREG, FREG)

/* opcodes most architecture have */
MINI_OP(OP_ADC,     "adc", IREG, IREG, IREG)
MINI_OP(OP_ADC_IMM, "adc_imm", IREG, IREG, NONE)
MINI_OP(OP_SBB,     "sbb", IREG, IREG, IREG)
MINI_OP(OP_SBB_IMM, "sbb_imm", IREG, IREG, NONE)
MINI_OP(OP_ADDCC,   "addcc", IREG, IREG, IREG)
MINI_OP(OP_ADDCC_IMM,   "addcc_imm", IREG, IREG, NONE)
MINI_OP(OP_SUBCC,   "subcc", IREG, IREG, IREG)
MINI_OP(OP_SUBCC_IMM,   "subcc_imm", IREG, IREG, NONE)
MINI_OP(OP_BR_REG,  "br_reg", NONE, IREG, NONE)
MINI_OP(OP_SEXT_I1,  "sext_i1", IREG, IREG, NONE)
MINI_OP(OP_SEXT_I2,  "sext_i2", IREG, IREG, NONE)
MINI_OP(OP_SEXT_I4,  "sext_i4", LREG, IREG, NONE)
MINI_OP(OP_ZEXT_I1,  "zext_i1", IREG, IREG, NONE)
MINI_OP(OP_ZEXT_I2,  "zext_i2", IREG, IREG, NONE)
MINI_OP(OP_ZEXT_I4,  "zext_i4", LREG, IREG, NONE)
MINI_OP(OP_CNE,      "cne", NONE, NONE, NONE)
MINI_OP(OP_TRUNC_I4, "trunc_i4", IREG, LREG, NONE)
/* to implement the upper half of long32 add and sub */
MINI_OP(OP_ADD_OVF_CARRY,   "add_ovf_carry", IREG, IREG, IREG)
MINI_OP(OP_SUB_OVF_CARRY,   "sub_ovf_carry", IREG, IREG, IREG)
MINI_OP(OP_ADD_OVF_UN_CARRY,   "add_ovf_un_carry", IREG, IREG, IREG)
MINI_OP(OP_SUB_OVF_UN_CARRY,   "sub_ovf_un_carry", IREG, IREG, IREG)

/* instructions with explicit long arguments to deal with 64-bit ilp32 machines */
MINI_OP(OP_LADDCC,   "laddcc", LREG, LREG, LREG)
MINI_OP(OP_LSUBCC,   "lsubcc", LREG, LREG, LREG)


/* FP functions usually done by the CPU */
MINI_OP(OP_SIN,     "sin", FREG, FREG, NONE)
MINI_OP(OP_COS,     "cos", FREG, FREG, NONE)
MINI_OP(OP_ABS,     "abs", FREG, FREG, NONE)
MINI_OP(OP_TAN,     "tan", FREG, FREG, NONE)
MINI_OP(OP_ATAN,    "atan", FREG, FREG, NONE)
MINI_OP(OP_SQRT,    "sqrt", FREG, FREG, NONE)
MINI_OP(OP_ROUND,   "round", FREG, FREG, NONE)
MINI_OP(OP_CEIL,    "ceil", FREG, FREG, NONE)
MINI_OP(OP_FLOOR,   "floor", FREG, FREG, NONE)
MINI_OP3(OP_FMA,     "fma", FREG, FREG, FREG, FREG)
MINI_OP(OP_SINF,     "sinf", FREG, FREG, NONE)
MINI_OP(OP_COSF,     "cosf", FREG, FREG, NONE)
MINI_OP(OP_EXPF,     "expf", FREG, FREG, NONE)
MINI_OP(OP_EXP,      "exp", FREG, FREG, NONE)
MINI_OP(OP_LOG,      "log", FREG, FREG, NONE)
MINI_OP(OP_LOG2,     "log2", FREG, FREG, NONE)
MINI_OP(OP_LOG2F,    "log2f", FREG, FREG, NONE)
MINI_OP(OP_LOG10,    "log10", FREG, FREG, NONE)
MINI_OP(OP_LOG10F,   "log10f", FREG, FREG, NONE)
MINI_OP(OP_TRUNC,    "trunc", FREG, FREG, NONE)
MINI_OP(OP_TRUNCF,   "truncf", FREG, FREG, NONE)
MINI_OP(OP_ABSF,     "absf", FREG, FREG, NONE)
MINI_OP(OP_SQRTF,    "sqrtf", FREG, FREG, NONE)
MINI_OP(OP_CEILF,    "ceilf", FREG, FREG, NONE)
MINI_OP(OP_FLOORF,   "floorf", FREG, FREG, NONE)
MINI_OP3(OP_FMAF,     "fmaf", FREG, FREG, FREG, FREG)

/* Operations that can be computed at constants at JIT time  */
MINI_OP(OP_ACOS,     "acos", FREG, FREG, NONE)
MINI_OP(OP_SINH,     "sinh", FREG, FREG, NONE)
MINI_OP(OP_ACOSH,    "acosh", FREG, FREG, NONE)
MINI_OP(OP_ASIN,     "asin", FREG, FREG, NONE)
MINI_OP(OP_ASINH,    "asinh", FREG, FREG, NONE)
MINI_OP(OP_ATANH,    "atanh2", FREG, FREG, NONE)
MINI_OP(OP_CBRT,     "cbrt", FREG, FREG, NONE)
MINI_OP(OP_COSH,     "cosh", FREG, FREG, NONE)
MINI_OP(OP_TANH,     "tanh", FREG, FREG, NONE)

/* to optimize strings */
MINI_OP(OP_STRLEN, "strlen", IREG, IREG, NONE)
MINI_OP(OP_NEWARR, "newarr", IREG, IREG, NONE)
/* Load a readonly length field from [sreg1+inst_imm] */
MINI_OP(OP_LDLEN, "ldlen", IREG, IREG, NONE)
/* inst_p0 is the exception name to throw or NULL */
MINI_OP(OP_BOUNDS_CHECK, "bounds_check", NONE, IREG, IREG)
/* type checks */
MINI_OP(OP_ISINST, "isinst", IREG, IREG, NONE)
MINI_OP(OP_CASTCLASS, "castclass", IREG, IREG, NONE)
/* get address of element in a 2D array */
MINI_OP(OP_LDELEMA2D, "ldelema2d", NONE, NONE, NONE)
/* inlined small memcpy with constant length */
MINI_OP(OP_MEMCPY, "memcpy", NONE, NONE, NONE)
/* inlined small memset with constant length */
MINI_OP(OP_MEMSET, "memset", NONE, NONE, NONE)
MINI_OP(OP_MEMSET_ZERO, "memset_zero", NONE, IREG, IREG)
/*
 * A RuntimeType object, the result ldtoken+GetTypeFromHandle.
 * inst_p0 is a MonoClass.
 */
MINI_OP(OP_RTTYPE, "rttype", IREG, NONE, NONE)

MINI_OP(OP_SAVE_LMF, "save_lmf", NONE, NONE, NONE)
MINI_OP(OP_RESTORE_LMF, "restore_lmf", NONE, NONE, NONE)

MINI_OP3(OP_MEMMOVE, "memmove", NONE, IREG, IREG, IREG)

/* write barrier */
MINI_OP(OP_CARD_TABLE_WBARRIER, "card_table_wbarrier", NONE, IREG, IREG)

/* arch-dep tls access */
MINI_OP(OP_TLS_GET,            "tls_get", IREG, NONE, NONE)
MINI_OP(OP_TLS_GET_REG,        "tls_get_reg", IREG, IREG, NONE)
/* inst_offset contains the TLS offset */
MINI_OP(OP_TLS_SET,            "tls_set", NONE, IREG, NONE)
MINI_OP(OP_TLS_SET_REG,        "tls_set_reg", NONE, IREG, IREG)

MINI_OP(OP_LOAD_GOTADDR, "load_gotaddr", IREG, NONE, NONE)
MINI_OP(OP_DUMMY_USE, "dummy_use", NONE, IREG, NONE)
MINI_OP(OP_NOT_REACHED, "not_reached", NONE, NONE, NONE)
MINI_OP(OP_NOT_NULL, "not_null", NONE, IREG, NONE)
MINI_OP(OP_LDTOKEN_FIELD, "ldtoken_field", VREG, VREG, NONE)

/* SIMD opcodes. */

MINI_OP(OP_ICONV_TO_R4_RAW, "iconv_to_r4_raw", FREG, IREG, NONE)

/* Extract an element from a vector with a constant lane index.
 * inst_c0 is the lane index.
 * inst_c1 is a MonoTypeEnum representing the element type.
 */
MINI_OP(OP_EXTRACT_I1, "extract_i1", IREG, XREG, NONE)
MINI_OP(OP_EXTRACT_I2, "extract_i2", IREG, XREG, NONE)
MINI_OP(OP_EXTRACT_I4, "extract_i4", IREG, XREG, NONE)
MINI_OP(OP_EXTRACT_I8, "extract_i8", LREG, XREG, NONE)
MINI_OP(OP_EXTRACT_R4, "extract_r4", FREG, XREG, NONE)
MINI_OP(OP_EXTRACT_R8, "extract_r8", FREG, XREG, NONE)
MINI_OP(OP_EXTRACTX_U2, "extractx_u2", IREG, XREG, NONE)

/*
 * Insert an element into a vector with a constant lane index.
 * inst_c0 is the lane index.
 */
MINI_OP(OP_INSERT_I1, "insert_i1", XREG, XREG, IREG)
MINI_OP(OP_INSERT_I2, "insert_i2", XREG, XREG, IREG)
MINI_OP(OP_INSERT_I4, "insert_i4", XREG, XREG, IREG)
MINI_OP(OP_INSERT_I8, "insert_i8", XREG, XREG, LREG)
MINI_OP(OP_INSERT_R4, "insert_r4", XREG, XREG, FREG)
MINI_OP(OP_INSERT_R8, "insert_r8", XREG, XREG, FREG)

/*these slow ops are modeled around the availability of a fast 2 bytes insert op*/
/*insertx_u1_slow takes old value and new value as source regs */
MINI_OP(OP_INSERTX_U1_SLOW, "insertx_u1_slow", XREG, IREG, IREG)
/*insertx_i4_slow takes target xreg and new value as source regs */
MINI_OP(OP_INSERTX_I4_SLOW, "insertx_i4_slow", XREG, XREG, IREG)

MINI_OP(OP_INSERTX_R4_SLOW, "insertx_r4_slow", XREG, XREG, FREG)
MINI_OP(OP_INSERTX_R8_SLOW, "insertx_r8_slow", XREG, XREG, FREG)
MINI_OP(OP_INSERTX_I8_SLOW, "insertx_i8_slow", XREG, XREG, LREG)

MINI_OP(OP_FCONV_TO_R4_X, "fconv_to_r4_x", XREG, FREG, NONE)
MINI_OP(OP_FCONV_TO_R8_X, "fconv_to_r8_x", XREG, FREG, NONE)
MINI_OP(OP_XCONV_R8_TO_I4, "xconv_r8_to_i4", IREG, XREG, NONE)
MINI_OP(OP_ICONV_TO_X, "iconv_to_x", XREG, IREG, NONE)

MINI_OP(OP_EXPAND_I1, "expand_i1", XREG, IREG, NONE)
MINI_OP(OP_EXPAND_I2, "expand_i2", XREG, IREG, NONE)
MINI_OP(OP_EXPAND_I4, "expand_i4", XREG, IREG, NONE)
MINI_OP(OP_EXPAND_R4, "expand_r4", XREG, FREG, NONE)
MINI_OP(OP_EXPAND_I8, "expand_i8", XREG, IREG, NONE)
MINI_OP(OP_EXPAND_R8, "expand_r8", XREG, FREG, NONE)

// wasm specific SIMD v128

#if defined(TARGET_WASM)
MINI_OP(OP_WASM_SIMD_BITMASK, "wasm_bitmask", IREG, XREG, NONE)
MINI_OP3(OP_WASM_BITSELECT, "wasm_bitselect", XREG, XREG, XREG, XREG)
MINI_OP3(OP_WASM_SIMD_SHUFFLE, "wasm_shuffle", XREG, XREG, XREG, XREG)
MINI_OP(OP_WASM_SIMD_SUM, "wasm_sum", XREG, XREG, NONE)
MINI_OP(OP_WASM_SIMD_SWIZZLE, "wasm_swizzle", XREG, XREG, XREG)
MINI_OP(OP_WASM_EXTRACT_NARROW, "wasm_extract_narrow", XREG, XREG, XREG)
MINI_OP(OP_WASM_EXTMUL_LOWER, "wasm_extmul_lower", XREG, XREG, XREG)
MINI_OP(OP_WASM_EXTMUL_UPPER, "wasm_extmul_upper", XREG, XREG, XREG)
MINI_OP(OP_WASM_EXTMUL_LOWER_U, "wasm_extmul_lower_u", XREG, XREG, XREG)
MINI_OP(OP_WASM_EXTMUL_UPPER_U, "wasm_extmul_upper_u", XREG, XREG, XREG)
MINI_OP(OP_WASM_SIMD_CONV_R8_TO_R4, "wasm_simd_conv_r8_to_r4", XREG, XREG, NONE)
MINI_OP(OP_WASM_SIMD_CONV_R8_TO_I4_ZERO, "wasm_simd_conv_r8_to_i4_zero", XREG, XREG, NONE)
MINI_OP(OP_WASM_SIMD_CONV_U4_TO_R8_LOW, "wasm_simd_conv_u4_to_r8_low", XREG, XREG, NONE)
MINI_OP3(OP_WASM_SIMD_LOAD_SCALAR_INSERT, "wasm_simd_load_scalar_insert", XREG, IREG, XREG, IREG)
MINI_OP(OP_WASM_SIMD_LOAD_SCALAR_SPLAT, "wasm_simd_load_scalar_splat", XREG, IREG, NONE)
MINI_OP(OP_WASM_SIMD_LOAD_WIDENING, "wasm_simd_load_widening", XREG, IREG, NONE)
MINI_OP(OP_WASM_SIMD_SEXT_LOWER, "wasm_simd_ext_lower_s", XREG, XREG, NONE)
MINI_OP(OP_WASM_SIMD_SEXT_UPPER, "wasm_simd_ext_upper_s", XREG, XREG, NONE)
MINI_OP3(OP_WASM_SIMD_STORE_LANE, "wasm_simd_store_lane", NONE, IREG, XREG, IREG)
MINI_OP(OP_WASM_SIMD_ZEXT_LOWER, "wasm_simd_ext_lower_u", XREG, XREG, NONE)
MINI_OP(OP_WASM_SIMD_ZEXT_UPPER, "wasm_simd_ext_upper_u", XREG, XREG, NONE)
#endif

#if defined(TARGET_X86) || defined(TARGET_AMD64) || defined(TARGET_WASM)

MINI_OP(OP_ADDPS, "addps", XREG, XREG, XREG)
MINI_OP(OP_DIVPS, "divps", XREG, XREG, XREG)
MINI_OP(OP_MULPS, "mulps", XREG, XREG, XREG)
MINI_OP(OP_SUBPS, "subps", XREG, XREG, XREG)
MINI_OP(OP_MAXPS, "maxps", XREG, XREG, XREG)
MINI_OP(OP_MINPS, "minps", XREG, XREG, XREG)
MINI_OP(OP_COMPPS, "compps", XREG, XREG, XREG)
MINI_OP(OP_ANDPS, "andps", XREG, XREG, XREG)
MINI_OP(OP_ANDNPS, "andnps", XREG, XREG, XREG)
MINI_OP(OP_ORPS, "orps", XREG, XREG, XREG)
MINI_OP(OP_XORPS, "xorps", XREG, XREG, XREG)
MINI_OP(OP_HADDPS, "haddps", XREG, XREG, XREG)
MINI_OP(OP_HSUBPS, "hsubps", XREG, XREG, XREG)
MINI_OP(OP_ADDSUBPS, "addsubps", XREG, XREG, XREG)
MINI_OP(OP_DUPPS_LOW, "dupps_low", XREG, XREG, NONE)
MINI_OP(OP_DUPPS_HIGH, "dupps_high", XREG, XREG, NONE)

MINI_OP(OP_RSQRTPS, "rsqrtps", XREG, XREG, NONE)
MINI_OP(OP_SQRTPS, "sqrtps", XREG, XREG, NONE)
MINI_OP(OP_RCPPS, "rcpps", XREG, XREG, NONE)

MINI_OP(OP_PSHUFLEW_HIGH, "pshuflew_high", XREG, XREG, NONE)
MINI_OP(OP_PSHUFLEW_LOW, "pshuflew_low", XREG, XREG, NONE)
MINI_OP(OP_PSHUFLED, "pshufled", XREG, XREG, NONE)
MINI_OP(OP_SHUFPS, "shufps", XREG, XREG, XREG)
MINI_OP(OP_SHUFPD, "shufpd", XREG, XREG, XREG)

MINI_OP(OP_ADDPD, "addpd", XREG, XREG, XREG)
MINI_OP(OP_DIVPD, "divpd", XREG, XREG, XREG)
MINI_OP(OP_MULPD, "mulpd", XREG, XREG, XREG)
MINI_OP(OP_SUBPD, "subpd", XREG, XREG, XREG)
MINI_OP(OP_MAXPD, "maxpd", XREG, XREG, XREG)
MINI_OP(OP_MINPD, "minpd", XREG, XREG, XREG)
MINI_OP(OP_COMPPD, "comppd", XREG, XREG, XREG)
MINI_OP(OP_ANDPD, "andpd", XREG, XREG, XREG)
MINI_OP(OP_ANDNPD, "andnpd", XREG, XREG, XREG)
MINI_OP(OP_ORPD, "orpd", XREG, XREG, XREG)
MINI_OP(OP_XORPD, "xorpd", XREG, XREG, XREG)
MINI_OP(OP_HADDPD, "haddpd", XREG, XREG, XREG)
MINI_OP(OP_HSUBPD, "hsubpd", XREG, XREG, XREG)
MINI_OP(OP_ADDSUBPD, "addsubpd", XREG, XREG, XREG)
MINI_OP(OP_DUPPD, "duppd", XREG, XREG, NONE)

MINI_OP(OP_SQRTPD, "sqrtpd", XREG, XREG, NONE)

MINI_OP(OP_EXTRACT_MASK, "extract_mask", IREG, XREG, NONE)

MINI_OP(OP_PAND, "pand", XREG, XREG, XREG)
MINI_OP(OP_PANDN, "pandn", XREG, XREG, XREG)
MINI_OP(OP_POR, "por", XREG, XREG, XREG)
MINI_OP(OP_PXOR, "pxor", XREG, XREG, XREG)

MINI_OP(OP_PADDB, "paddb", XREG, XREG, XREG)
MINI_OP(OP_PADDW, "paddw", XREG, XREG, XREG)
MINI_OP(OP_PADDD, "paddd", XREG, XREG, XREG)
MINI_OP(OP_PADDQ, "paddq", XREG, XREG, XREG)

MINI_OP(OP_PSUBB, "psubb", XREG, XREG, XREG)
MINI_OP(OP_PSUBW, "psubw", XREG, XREG, XREG)
MINI_OP(OP_PSUBD, "psubd", XREG, XREG, XREG)
MINI_OP(OP_PSUBQ, "psubq", XREG, XREG, XREG)

MINI_OP(OP_PMAXB_UN, "pmaxb_un", XREG, XREG, XREG)
MINI_OP(OP_PMAXW_UN, "pmaxw_un", XREG, XREG, XREG)
MINI_OP(OP_PMAXD_UN, "pmaxd_un", XREG, XREG, XREG)

MINI_OP(OP_PMAXB, "pmaxb", XREG, XREG, XREG)
MINI_OP(OP_PMAXW, "pmaxw", XREG, XREG, XREG)
MINI_OP(OP_PMAXD, "pmaxd", XREG, XREG, XREG)

MINI_OP(OP_PAVGB_UN, "pavgb_un", XREG, XREG, XREG)
MINI_OP(OP_PAVGW_UN, "pavgw_un", XREG, XREG, XREG)

MINI_OP(OP_PMINB_UN, "pminb_un", XREG, XREG, XREG)
MINI_OP(OP_PMINW_UN, "pminw_un", XREG, XREG, XREG)
MINI_OP(OP_PMIND_UN, "pmind_un", XREG, XREG, XREG)

MINI_OP(OP_PMINB, "pminb", XREG, XREG, XREG)
MINI_OP(OP_PMINW, "pminw", XREG, XREG, XREG)
MINI_OP(OP_PMIND, "pmind", XREG, XREG, XREG)

MINI_OP(OP_PCMPEQB, "pcmpeqb", XREG, XREG, XREG)
MINI_OP(OP_PCMPEQW, "pcmpeqw", XREG, XREG, XREG)
MINI_OP(OP_PCMPEQD, "pcmpeqd", XREG, XREG, XREG)
MINI_OP(OP_PCMPEQQ, "pcmpeqq", XREG, XREG, XREG)

MINI_OP(OP_PCMPGTB, "pcmpgtb", XREG, XREG, XREG)
MINI_OP(OP_PCMPGTW, "pcmpgtw", XREG, XREG, XREG)
MINI_OP(OP_PCMPGTD, "pcmpgtd", XREG, XREG, XREG)
MINI_OP(OP_PCMPGTQ, "pcmpgtq", XREG, XREG, XREG)

MINI_OP(OP_PSUM_ABS_DIFF, "psum_abs_diff", XREG, XREG, XREG)

MINI_OP(OP_UNPACK_LOWB, "unpack_lowb", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_LOWW, "unpack_loww", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_LOWD, "unpack_lowd", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_LOWQ, "unpack_lowq", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_LOWPS, "unpack_lowps", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_LOWPD, "unpack_lowpd", XREG, XREG, XREG)

MINI_OP(OP_UNPACK_HIGHB, "unpack_highb", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_HIGHW, "unpack_highw", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_HIGHD, "unpack_highd", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_HIGHQ, "unpack_highq", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_HIGHPS, "unpack_highps", XREG, XREG, XREG)
MINI_OP(OP_UNPACK_HIGHPD, "unpack_highpd", XREG, XREG, XREG)

MINI_OP(OP_PACKW, "packw", XREG, XREG, XREG)
MINI_OP(OP_PACKD, "packd", XREG, XREG, XREG)

MINI_OP(OP_PACKW_UN, "packw_un", XREG, XREG, XREG)
MINI_OP(OP_PACKD_UN, "packd_un", XREG, XREG, XREG)

MINI_OP(OP_PADDB_SAT, "paddb_sat", XREG, XREG, XREG)
MINI_OP(OP_PADDB_SAT_UN, "paddb_sat_un", XREG, XREG, XREG)

MINI_OP(OP_PADDW_SAT, "paddw_sat", XREG, XREG, XREG)
MINI_OP(OP_PADDW_SAT_UN, "paddw_sat_un", XREG, XREG, XREG)

MINI_OP(OP_PSUBB_SAT, "psubb_sat", XREG, XREG, XREG)
MINI_OP(OP_PSUBB_SAT_UN, "psubb_sat_un", XREG, XREG, XREG)

MINI_OP(OP_PSUBW_SAT, "psubw_sat", XREG, XREG, XREG)
MINI_OP(OP_PSUBW_SAT_UN, "psubw_sat_un", XREG, XREG, XREG)

MINI_OP(OP_PMULW, "pmulw", XREG, XREG, XREG)
MINI_OP(OP_PMULD, "pmuld", XREG, XREG, XREG)
/* Multiplies two 32 bit numbers into a 64 bit one */
MINI_OP(OP_PMULQ, "pmulq", XREG, XREG, XREG)

MINI_OP(OP_PMULW_HIGH_UN, "pmulw_high_un", XREG, XREG, XREG)
MINI_OP(OP_PMULW_HIGH, "pmulw_high", XREG, XREG, XREG)

/*SSE2 Shift ops must have the _reg version right after as code depends on this ordering.*/
MINI_OP(OP_PSHRW, "pshrw", XREG, XREG, NONE)
MINI_OP(OP_PSHRW_REG, "pshrw_reg", XREG, XREG, XREG)

MINI_OP(OP_PSARW, "psarw", XREG, XREG, NONE)
MINI_OP(OP_PSARW_REG, "psarw_reg", XREG, XREG, XREG)

MINI_OP(OP_PSHLW, "pshlw", XREG, XREG, NONE)
MINI_OP(OP_PSHLW_REG, "pshlw_reg", XREG, XREG, XREG)

MINI_OP(OP_PSHRD, "pshrd", XREG, XREG, NONE)
MINI_OP(OP_PSHRD_REG, "pshrd_reg", XREG, XREG, XREG)

MINI_OP(OP_PSHRQ, "pshrq", XREG, XREG, NONE)
MINI_OP(OP_PSHRQ_REG, "pshrq_reg", XREG, XREG, XREG)

MINI_OP(OP_PSARD, "psard", XREG, XREG, NONE)
MINI_OP(OP_PSARD_REG, "psard_reg", XREG, XREG, XREG)

MINI_OP(OP_PSHLD, "pshld", XREG, XREG, NONE)
MINI_OP(OP_PSHLD_REG, "pshld_reg", XREG, XREG, XREG)

MINI_OP(OP_PSHLQ, "pshlq", XREG, XREG, NONE)
MINI_OP(OP_PSHLQ_REG, "pshlq_reg", XREG, XREG, XREG)

MINI_OP(OP_PREFETCH_MEMBASE, "prefetch_membase", NONE, IREG, NONE)

MINI_OP(OP_CVTDQ2PD, "cvtdq2pd", XREG, XREG, NONE)
MINI_OP(OP_CVTDQ2PS, "cvtdq2ps", XREG, XREG, NONE)
MINI_OP(OP_CVTPD2DQ, "cvtpd2dq", XREG, XREG, NONE)
MINI_OP(OP_CVTPD2PS, "cvtpd2ps", XREG, XREG, NONE)
MINI_OP(OP_CVTPS2DQ, "cvtps2dq", XREG, XREG, NONE)
MINI_OP(OP_CVTPS2PD, "cvtps2pd", XREG, XREG, NONE)
MINI_OP(OP_CVTTPD2DQ, "cvttpd2dq", XREG, XREG, NONE)
MINI_OP(OP_CVTTPS2DQ, "cvttps2dq", XREG, XREG, NONE)


/* sse 1 */
/* inst_c1 is target type */
MINI_OP(OP_SSE_LOADU, "sse_loadu", XREG, XREG, NONE)
MINI_OP(OP_SSE_MOVMSK, "sse_movmsk", IREG, XREG, NONE)
MINI_OP(OP_SSE_STORES, "sse_stores", NONE, XREG, XREG)
MINI_OP(OP_SSE_MOVS, "sse_movs", XREG, XREG, NONE)
MINI_OP(OP_SSE_MOVS2, "sse_movs2", XREG, XREG, XREG)
MINI_OP(OP_SSE_MOVEHL, "sse_movehl", XREG, XREG, XREG)
MINI_OP(OP_SSE_MOVELH, "sse_movelh", XREG, XREG, XREG)
MINI_OP(OP_SSE_UNPACKLO, "sse_unpacklo", XREG, XREG, XREG)
MINI_OP(OP_SSE_UNPACKHI, "sse_unpackhi", XREG, XREG, XREG)
MINI_OP3(OP_SSE_SHUFPS, "sse_shufps", XREG, XREG, XREG, IREG)
MINI_OP(OP_SSE_AND, "sse_and", XREG, XREG, XREG)
MINI_OP(OP_SSE_OR, "sse_or", XREG, XREG, XREG)
MINI_OP(OP_SSE_XOR, "sse_xor", XREG, XREG, XREG)
MINI_OP(OP_SSE_ADDSS, "sse_addss", XREG, XREG, XREG)
MINI_OP(OP_SSE_SUBSS, "sse_subss", XREG, XREG, XREG)
MINI_OP(OP_SSE_DIVSS, "sse_divss", XREG, XREG, XREG)
MINI_OP(OP_SSE_MULSS, "sse_mulss", XREG, XREG, XREG)
MINI_OP(OP_SSE_CMPSS, "sse_cmpss", XREG, XREG, XREG)
MINI_OP(OP_SSE_COMISS, "sse_comiss", IREG, XREG, XREG)
MINI_OP(OP_SSE_UCOMISS, "sse_ucomiss", IREG, XREG, XREG)
MINI_OP(OP_SSE_MOVSS, "sse_movss", XREG, IREG, NONE)
MINI_OP(OP_SSE_MOVSS_STORE, "sse_movss_store", NONE, IREG, XREG)
MINI_OP(OP_SSE_MOVHPS_LOAD, "sse_movhps_load", XREG, XREG, IREG)
MINI_OP(OP_SSE_MOVLPS_LOAD, "sse_movlps_load", XREG, XREG, IREG)
MINI_OP(OP_SSE_MOVHPS_STORE, "sse_movhps_store", NONE, IREG, XREG)
MINI_OP(OP_SSE_MOVLPS_STORE, "sse_movlps_store", NONE, IREG, XREG)
MINI_OP(OP_SSE_MOVNTPS, "sse_movntps", NONE, IREG, XREG)
MINI_OP(OP_SSE_PREFETCHT0, "sse_prefetcht0", NONE, IREG, NONE)
MINI_OP(OP_SSE_PREFETCHT1, "sse_prefetcht1", NONE, IREG, NONE)
MINI_OP(OP_SSE_PREFETCHT2, "sse_prefetcht2", NONE, IREG, NONE)
MINI_OP(OP_SSE_PREFETCHNTA, "sse_prefetchnta", NONE, IREG, NONE)
MINI_OP(OP_SSE_SQRTSS, "sse_sqrtss", XREG, XREG, XREG)
MINI_OP(OP_SSE_RSQRTSS, "sse_rsqrtss", XREG, XREG, XREG)
MINI_OP(OP_SSE_RCPSS, "sse_rcpss", XREG, XREG, XREG)
MINI_OP(OP_SSE_CVTSI2SS, "sse_cvtsi2ss", XREG, XREG, IREG)
MINI_OP(OP_SSE_CVTSI2SS64, "sse_cvtsi2ss64", XREG, XREG, LREG)

/* sse 2 */
MINI_OP(OP_SSE2_PACKUS, "sse2_packus", XREG, XREG, XREG)
MINI_OP(OP_SSE2_SRLI, "sse2_srli", XREG, XREG, XREG)
MINI_OP(OP_SSE2_ADDS, "sse2_adds", XREG, XREG, XREG)
MINI_OP(OP_SSE2_SUBS, "sse2_subs", XREG, XREG, XREG)
MINI_OP(OP_SSE2_CMPSD, "sse2_cmpsd", XREG, XREG, XREG)
MINI_OP(OP_SSE2_COMIEQ_SD, "sse2_comieq_sd", XREG, XREG, XREG)
MINI_OP(OP_SSE2_COMISD, "sse2_comisd", IREG, XREG, XREG)
MINI_OP(OP_SSE2_UCOMISD, "sse2_ucomisd", IREG, XREG, XREG)
MINI_OP(OP_SSE2_ADDSD, "sse2_addsd", XREG, XREG, XREG)
MINI_OP(OP_SSE2_SUBSD, "sse2_subsd", XREG, XREG, XREG)
MINI_OP(OP_SSE2_DIVSD, "sse2_divsd", XREG, XREG, XREG)
MINI_OP(OP_SSE2_MULSD, "sse2_mulsd", XREG, XREG, XREG)
MINI_OP(OP_SSE2_PSLLDQ, "sse2_pslldq", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSRLDQ, "sse2_psrldq", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSRAW_IMM, "sse2_psraw_imm", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSRAD_IMM, "sse2_psrad_imm", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSRLW_IMM, "sse2_psrlw_imm", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSRLD_IMM, "sse2_psrld_imm", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSRLQ_IMM, "sse2_psrlq_imm", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSHUFD, "sse2_pshufd", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSHUFHW, "sse2_pshufhw", XREG, XREG, IREG)
MINI_OP(OP_SSE2_PSHUFLW, "sse2_pshuflw", XREG, XREG, IREG)
MINI_OP3(OP_SSE2_SHUFPD, "sse2_shufpd", XREG, XREG, XREG, IREG)
MINI_OP(OP_SSE2_MOVHPD_LOAD, "sse2_movhpd_load", XREG, XREG, IREG)
MINI_OP(OP_SSE2_MOVLPD_LOAD, "sse2_movlpd_load", XREG, XREG, IREG)
MINI_OP(OP_SSE2_MOVHPD_STORE, "sse2_movhpd_store", NONE, IREG, XREG)
MINI_OP(OP_SSE2_MOVLPD_STORE, "sse2_movlpd_store", NONE, IREG, XREG)
MINI_OP(OP_SSE2_SQRTSD, "sse2_sqrtsd", XREG, XREG, XREG)
MINI_OP(OP_SSE2_CVTSI2SD, "sse2_cvtsi2sd", XREG, XREG, IREG)
MINI_OP(OP_SSE2_CVTSI2SD64, "sse2_cvtsi2sd64", XREG, XREG, LREG)
MINI_OP(OP_SSE2_CVTSS2SD, "sse2_cvtsd2sd", XREG, XREG, XREG)
MINI_OP(OP_SSE2_PMULUDQ, "sse2_pmuludq", XREG, XREG, XREG)
MINI_OP3(OP_SSE2_MASKMOVDQU, "sse2_maskmovdqu", NONE, XREG, XREG, IREG)

/* sse 3 */
MINI_OP(OP_SSE3_MOVDDUP, "sse3_movddup", XREG, XREG, NONE)
MINI_OP(OP_SSE3_MOVSHDUP, "sse3_movshdup", XREG, XREG, NONE)
MINI_OP(OP_SSE3_MOVSLDUP, "sse3_movsldup", XREG, XREG, NONE)
MINI_OP(OP_SSE3_MOVDDUP_MEM, "sse3_movddup_mem", XREG, IREG, NONE)

/* ssse 3 */
MINI_OP(OP_SSSE3_SHUFFLE, "ssse3_shuffle", XREG, XREG, XREG)
MINI_OP3(OP_SSSE3_ALIGNR, "ssse3_alignr", XREG, XREG, XREG, IREG)

/* sse 4.1 */
MINI_OP(OP_SSE41_ROUNDP, "roundp", XREG, XREG, NONE) // packed, inst_c0 - mode, inst_c1 - r4 or r8
MINI_OP(OP_SSE41_ROUNDS, "sse41_rounds", XREG, XREG, XREG) // scalar, inst_c0 - mode, inst_c1 - r4 or r8
MINI_OP3(OP_SSE41_INSERTPS, "sse41_insertps", XREG, XREG, XREG, IREG)
MINI_OP3(OP_SSE41_BLENDV, "sse41_blendv", XREG, XREG, XREG, XREG)
MINI_OP3(OP_SSE41_BLEND, "sse41_blend", XREG, XREG, XREG, IREG)
MINI_OP(OP_SSE41_LOADANT, "sse41_loadant", XREG, XREG, NONE)
MINI_OP(OP_SSE41_MUL, "sse41_mul", XREG, XREG, XREG)
MINI_OP(OP_SSE41_MULLO, "sse41_mullo", XREG, XREG, XREG)
MINI_OP(OP_SSE_CVTII, "sse_cvtii", XREG, XREG, NONE)
MINI_OP3(OP_SSE41_DPPS, "sse41_dpps", XREG, XREG, XREG, IREG)
MINI_OP3(OP_SSE41_DPPD, "sse41_dppd", XREG, XREG, XREG, IREG)
MINI_OP3(OP_SSE41_MPSADBW, "sse41_mpsadbw", XREG, XREG, XREG, IREG)
/* inst_c0 contains the mask value */
MINI_OP(OP_SSE41_DPPS_IMM, "sse41_dpps_imm", XREG, XREG, XREG)
MINI_OP(OP_SSE41_DPPD_IMM, "sse41_dppd_imm", XREG, XREG, XREG)

/* pclmulqdq */
MINI_OP3(OP_PCLMULQDQ, "pclmulqdq", XREG, XREG, XREG, IREG)

/* aes */
MINI_OP(OP_AES_KEYGENASSIST, "aes_keygen", XREG, XREG, IREG)

/* sse 4.2 */
MINI_OP(OP_SSE42_CRC32, "sse42_crc32", IREG, IREG, IREG)
MINI_OP(OP_SSE42_CRC64, "sse42_crc64", LREG, LREG, LREG)
MINI_OP(OP_SSE42_PTESTZ, "sse42_ptestc", IREG, XREG, XREG)

/* Intel BMI1 */
/* Count trailing zeroes, return 32/64 if the input is 0 */
MINI_OP(OP_CTTZ32, "cttz32", IREG, IREG, NONE)
MINI_OP(OP_CTTZ64, "cttz64", LREG, LREG, NONE)
MINI_OP(OP_BMI1_BEXTR32, "bmi1_bextr32", IREG, IREG, IREG)
MINI_OP(OP_BMI1_BEXTR64, "bmi1_bextr64", LREG, LREG, LREG)

/* Intel BMI2 */
MINI_OP(OP_BZHI32, "bzhi32", IREG, IREG, IREG)
MINI_OP(OP_BZHI64, "bzhi64", LREG, LREG, LREG)
MINI_OP(OP_PEXT32, "pext32", IREG, IREG, IREG)
MINI_OP(OP_PEXT64, "pext64", LREG, LREG, LREG)
MINI_OP(OP_PDEP32, "pdep32", IREG, IREG, IREG)
MINI_OP(OP_PDEP64, "pdep64", LREG, LREG, LREG)
MINI_OP(OP_MULX_H32, "mulxh32", IREG, IREG, IREG)
MINI_OP(OP_MULX_H64, "mulxh64", LREG, LREG, LREG)
MINI_OP3(OP_MULX_HL32, "mulxhl32", IREG, IREG, IREG, IREG)
MINI_OP3(OP_MULX_HL64, "mulxhl64", LREG, LREG, LREG, LREG)

#endif

#if defined(TARGET_X86) || defined(TARGET_AMD64)
/*
 * These operations exist to facilitate simultaneous int/uint division
 * and remainder on x86/x86-64. On that platform the DIV/IDIV instructions
 * operate as follows edx:eax/reg32 -> (eax=quotient,edx=remainder). Mono
 * ops only support one destination register, so two operations are needed
 * to obtain two result values. One would use {long,int}_divrem[_un] first,
 * and the corresponding {long_int}_divrem2 immediately afterwards. The
 * first instruction returns the quotient and leaves the remainder in the
 * edx(rdx) register. The second instruction puts a virtual register over
 * edx, so that its value can be used. Note that if the first instruction
 * is emitted, the second must be also (there is an assert). This works
 * both in LLVM and mini.
 */
MINI_OP3(OP_X86_LDIVREM, "long_divrem", LREG, LREG, LREG, LREG)
MINI_OP3(OP_X86_LDIVREMU, "long_divrem_un", LREG, LREG, LREG, LREG)
MINI_OP3(OP_X86_LDIVREM2, "long_divrem2", LREG, NONE, NONE, NONE)
MINI_OP3(OP_X86_IDIVREM, "int_divrem", IREG, IREG, IREG, IREG)
MINI_OP3(OP_X86_IDIVREMU, "int_divrem_un", IREG, IREG, IREG, IREG)
MINI_OP3(OP_X86_IDIVREM2, "int_divrem2", IREG, NONE, NONE, NONE)
#endif

MINI_OP(OP_CREATE_SCALAR_UNSAFE, "create_scalar_unsafe", XREG, XREG, NONE)
MINI_OP(OP_CREATE_SCALAR, "create_scalar", XREG, XREG, NONE)

MINI_OP(OP_CREATE_SCALAR_UNSAFE_INT, "create_scalar_unsafe_int", XREG, IREG, NONE)
MINI_OP(OP_CREATE_SCALAR_UNSAFE_FLOAT, "create_scalar_unsafe_float", XREG, FREG, NONE)
MINI_OP(OP_CREATE_SCALAR_INT, "create_scalar_int", XREG, IREG, NONE)
MINI_OP(OP_CREATE_SCALAR_FLOAT, "create_scalar_float", XREG, FREG, NONE)

MINI_OP(OP_XMOVE,   "xmove", XREG, XREG, NONE)
MINI_OP(OP_XZERO,   "xzero", XREG, NONE, NONE)
MINI_OP(OP_XONES,   "xones", XREG, NONE, NONE)
MINI_OP(OP_XCONST,  "xconst", XREG, NONE, NONE)
MINI_OP(OP_XPHI,	"xphi", XREG, NONE, NONE)

/*
 * These are used for efficient implementation of the
 * atomic methods on Interlocked, Volatile, and Thread.
 * This is done only on architectures that support it,
 * as per mono_arch_opcode_supported ().
 *
 * Note that while the 32-bit variants are used on
 * both 32-bit and 64-bit systems, the 64-bit variants
 * are only used if the system is 64-bit. If that is
 * not the case, the fallback code in the runtime is
 * used instead. This is done because decomposing the
 * 64-bit variants to instructions operating on 32-bit
 * registers is very complicated on some architectures.
 *
 * For memory_barrier and load/store instructions, the
 * inst.backend.memory_barrier_kind field indicates
 * which semantics to use.
 *
 * Where relevant, all of these return the new value at
 * the given memory location after performing the
 * operation.
 */

MINI_OP(OP_MEMORY_BARRIER, "memory_barrier", NONE, NONE, NONE)

MINI_OP(OP_ATOMIC_LOAD_I1, "atomic_load_i1", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_I2, "atomic_load_i2", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_I4, "atomic_load_i4", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_I8, "atomic_load_i8", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_U1, "atomic_load_u1", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_U2, "atomic_load_u2", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_U4, "atomic_load_u4", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_U8, "atomic_load_u8", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_R4, "atomic_load_r4", FREG, IREG, NONE)
MINI_OP(OP_ATOMIC_LOAD_R8, "atomic_load_r8", FREG, IREG, NONE)

MINI_OP(OP_ATOMIC_STORE_I1, "atomic_store_i1", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_STORE_I2, "atomic_store_i2", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_STORE_I4, "atomic_store_i4", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_STORE_I8, "atomic_store_i8", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_STORE_U1, "atomic_store_u1", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_STORE_U2, "atomic_store_u2", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_STORE_U4, "atomic_store_u4", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_STORE_U8, "atomic_store_u8", IREG, IREG, NONE)
MINI_OP(OP_ATOMIC_STORE_R4, "atomic_store_r4", IREG, FREG, NONE)
MINI_OP(OP_ATOMIC_STORE_R8, "atomic_store_r8", IREG, FREG, NONE)

MINI_OP(OP_ATOMIC_ADD_I4, "atomic_add_i4", IREG, IREG, IREG)
MINI_OP(OP_ATOMIC_ADD_I8, "atomic_add_i8", IREG, IREG, IREG)
MINI_OP(OP_ATOMIC_AND_I4, "atomic_and_i4", IREG, IREG, IREG)
MINI_OP(OP_ATOMIC_AND_I8, "atomic_and_i8", IREG, IREG, IREG)
MINI_OP(OP_ATOMIC_OR_I4, "atomic_or_i4", IREG, IREG, IREG)
MINI_OP(OP_ATOMIC_OR_I8, "atomic_or_i8", IREG, IREG, IREG)

MINI_OP(OP_ATOMIC_EXCHANGE_U1, "atomic_exchange_u1", IREG, IREG, IREG)
MINI_OP(OP_ATOMIC_EXCHANGE_U2, "atomic_exchange_u2", IREG, IREG, IREG)
MINI_OP(OP_ATOMIC_EXCHANGE_I4, "atomic_exchange_i4", IREG, IREG, IREG)
MINI_OP(OP_ATOMIC_EXCHANGE_I8, "atomic_exchange_i8", IREG, IREG, IREG)

MINI_OP3(OP_ATOMIC_CAS_U1, "atomic_cas_u1", IREG, IREG, IREG, IREG)
MINI_OP3(OP_ATOMIC_CAS_U2, "atomic_cas_u2", IREG, IREG, IREG, IREG)
MINI_OP3(OP_ATOMIC_CAS_I4, "atomic_cas_i4", IREG, IREG, IREG, IREG)
MINI_OP3(OP_ATOMIC_CAS_I8, "atomic_cas_i8", IREG, IREG, IREG, IREG)

/* Conditional move opcodes.
 * Must be in the same order as the matching CEE_B... opcodes
 * sreg2 will be assigned to dreg if the condition is true.
 * sreg1 should be equal to dreg and models the fact the instruction doesn't necessary
 * modify dreg. The sreg1==dreg condition could be violated by SSA, so the local
 * register allocator or the code generator should generate a mov dreg, sreg1 before
 * the cmov in those cases.
 * These opcodes operate on pointer sized values.
 */
MINI_OP(OP_CMOV_IEQ,    "cmov_ieq", IREG, IREG, IREG)
MINI_OP(OP_CMOV_IGE,    "cmov_ige", IREG, IREG, IREG)
MINI_OP(OP_CMOV_IGT,    "cmov_igt", IREG, IREG, IREG)
MINI_OP(OP_CMOV_ILE,    "cmov_ile", IREG, IREG, IREG)
MINI_OP(OP_CMOV_ILT,    "cmov_ilt", IREG, IREG, IREG)
MINI_OP(OP_CMOV_INE_UN, "cmov_ine_un", IREG, IREG, IREG)
MINI_OP(OP_CMOV_IGE_UN, "cmov_ige_un", IREG, IREG, IREG)
MINI_OP(OP_CMOV_IGT_UN, "cmov_igt_un", IREG, IREG, IREG)
MINI_OP(OP_CMOV_ILE_UN, "cmov_ile_un", IREG, IREG, IREG)
MINI_OP(OP_CMOV_ILT_UN, "cmov_ilt_un", IREG, IREG, IREG)

MINI_OP(OP_CMOV_LEQ,    "cmov_leq", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LGE,    "cmov_lge", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LGT,    "cmov_lgt", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LLE,    "cmov_lle", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LLT,    "cmov_llt", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LNE_UN, "cmov_lne_un", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LGE_UN, "cmov_lge_un", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LGT_UN, "cmov_lgt_un", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LLE_UN, "cmov_lle_un", IREG, IREG, IREG)
MINI_OP(OP_CMOV_LLT_UN, "cmov_llt_un", IREG, IREG, IREG)

/* Debugging support */
/*
 * Marks the start of the live range of the variable in inst_c0, that is the
 * first instruction where the variable has a value.
 */
MINI_OP(OP_LIVERANGE_START, "liverange_start", NONE, NONE, NONE)
/*
 * Marks the end of the live range of the variable in inst_c0, that is the
 * first instruction where the variable no longer has a value.
 */
MINI_OP(OP_LIVERANGE_END, "liverange_end", NONE, NONE, NONE)

/* GC support */
/*
 * mono_arch_output_basic_block () will set the backend.pc_offset field to the current pc
 * offset.
 */
MINI_OP(OP_GC_LIVENESS_DEF, "gc_liveness_def", NONE, NONE, NONE)
MINI_OP(OP_GC_LIVENESS_USE, "gc_liveness_use", NONE, NONE, NONE)

/*
 * This marks the location inside a basic block where a GC tracked spill slot has been
 * defined. The spill slot is assumed to be alive until the end of the bblock.
 */
MINI_OP(OP_GC_SPILL_SLOT_LIVENESS_DEF, "gc_spill_slot_liveness_def", NONE, NONE, NONE)

/*
 * This marks the location inside a basic block where a GC tracked param area slot has
 * been defined. The slot is assumed to be alive until the next call.
 */
MINI_OP(OP_GC_PARAM_SLOT_LIVENESS_DEF, "gc_param_slot_liveness_def", NONE, NONE, NONE)

MINI_OP(OP_GC_SAFE_POINT, "gc_safe_point", NONE, IREG, NONE)

/*
 * Check if the class given by sreg1 was inited, if not, call
 * mono_generic_class_init_trampoline () though a trampoline.
 * Since the trampoline saves all registers, this doesn't clobber
 * any registers.
 */
MINI_OP(OP_GENERIC_CLASS_INIT, "generic_class_init", NONE, IREG, NONE)

/*
 * Call mini_init_method_rgctx () if needed.
 * sreg1 is a MonoMethodRuntimeGenericContext.
 * sreg2 is a MonoGSharedMethodInfo.
 */
MINI_OP(OP_INIT_MRGCTX, "init_mrgctx", NONE, IREG, IREG)

/* Arch specific opcodes */
#if defined(TARGET_X86) || defined(TARGET_AMD64)
MINI_OP(OP_X86_TEST_NULL,          "x86_test_null", NONE, IREG, NONE)
MINI_OP(OP_X86_COMPARE_MEMBASE_REG,"x86_compare_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_X86_COMPARE_MEMBASE_IMM,"x86_compare_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_X86_COMPARE_MEM_IMM,    "x86_compare_mem_imm", NONE, NONE, NONE)
MINI_OP(OP_X86_COMPARE_MEMBASE8_IMM,"x86_compare_membase8_imm", NONE, IREG, NONE)
MINI_OP(OP_X86_COMPARE_REG_MEMBASE,"x86_compare_reg_membase", NONE, IREG, IREG)
MINI_OP(OP_X86_INC_REG,            "x86_inc_reg", IREG, IREG, NONE)
MINI_OP(OP_X86_INC_MEMBASE,        "x86_inc_membase", NONE, IREG, NONE)
MINI_OP(OP_X86_DEC_REG,            "x86_dec_reg", IREG, IREG, NONE)
MINI_OP(OP_X86_DEC_MEMBASE,        "x86_dec_membase", NONE, IREG, NONE)
MINI_OP(OP_X86_ADD_MEMBASE_IMM,    "x86_add_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_X86_SUB_MEMBASE_IMM,    "x86_sub_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_X86_AND_MEMBASE_IMM,    "x86_and_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_X86_OR_MEMBASE_IMM,     "x86_or_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_X86_XOR_MEMBASE_IMM,    "x86_xor_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_X86_ADD_MEMBASE_REG,    "x86_add_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_X86_SUB_MEMBASE_REG,    "x86_sub_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_X86_AND_MEMBASE_REG,    "x86_and_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_X86_OR_MEMBASE_REG,     "x86_or_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_X86_XOR_MEMBASE_REG,    "x86_xor_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_X86_MUL_MEMBASE_REG,    "x86_mul_membase_reg", NONE, IREG, IREG)

MINI_OP(OP_X86_ADD_REG_MEMBASE,    "x86_add_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_X86_SUB_REG_MEMBASE,    "x86_sub_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_X86_MUL_REG_MEMBASE,    "x86_mul_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_X86_AND_REG_MEMBASE,    "x86_and_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_X86_OR_REG_MEMBASE,     "x86_or_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_X86_XOR_REG_MEMBASE,    "x86_xor_reg_membase", IREG, IREG, IREG)

MINI_OP(OP_X86_PUSH_MEMBASE,       "x86_push_membase", NONE, IREG, NONE)
MINI_OP(OP_X86_PUSH_IMM,           "x86_push_imm", NONE, NONE, NONE)
MINI_OP(OP_X86_PUSH,               "x86_push", NONE, IREG, NONE)
MINI_OP(OP_X86_PUSH_OBJ,           "x86_push_obj", NONE, IREG, NONE)
MINI_OP(OP_X86_PUSH_GOT_ENTRY,     "x86_push_got_entry", NONE, IREG, NONE)
MINI_OP(OP_X86_LEA,                "x86_lea", IREG, IREG, IREG)
MINI_OP(OP_X86_LEA_MEMBASE,        "x86_lea_membase", IREG, IREG, NONE)
MINI_OP(OP_X86_XCHG,               "x86_xchg", NONE, IREG, IREG)
MINI_OP(OP_X86_FPOP,               "x86_fpop", NONE, FREG, NONE)
MINI_OP(OP_X86_FP_LOAD_I8,         "x86_fp_load_i8", FREG, IREG, NONE)
MINI_OP(OP_X86_FP_LOAD_I4,         "x86_fp_load_i4", FREG, IREG, NONE)
MINI_OP(OP_X86_SETEQ_MEMBASE,      "x86_seteq_membase", NONE, IREG, NONE)
MINI_OP(OP_X86_SETNE_MEMBASE,      "x86_setne_membase", NONE, IREG, NONE)
MINI_OP(OP_X86_FXCH,               "x86_fxch", NONE, NONE, NONE)
MINI_OP(OP_X86_BSF32,              "x86_bsf32", IREG, IREG, NONE)
MINI_OP(OP_X86_BSR32,              "x86_bsr32", IREG, IREG, NONE)
MINI_OP(OP_X86_BSF64,              "x86_bsf64", LREG, LREG, NONE)
MINI_OP(OP_X86_BSR64,              "x86_bsr64", LREG, LREG, NONE)
MINI_OP(OP_X86_MOVE_R8_TO_FPSTACK,    "x86_move_r8_to_fpstack", NONE, FREG, NONE)
MINI_OP(OP_X86_MOVE_R4_TO_FPSTACK,    "x86_move_r4_to_fpstack", NONE, FREG, NONE)
#endif

#if defined(TARGET_AMD64)
MINI_OP(OP_AMD64_TEST_NULL,              "amd64_test_null", NONE, IREG, NONE)
MINI_OP(OP_AMD64_SET_XMMREG_R4,          "amd64_set_xmmreg_r4", FREG, FREG, NONE)
MINI_OP(OP_AMD64_SET_XMMREG_R8,          "amd64_set_xmmreg_r8", FREG, FREG, NONE)
MINI_OP(OP_AMD64_ICOMPARE_MEMBASE_REG,   "amd64_icompare_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_AMD64_ICOMPARE_MEMBASE_IMM,   "amd64_icompare_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_AMD64_ICOMPARE_REG_MEMBASE,   "amd64_icompare_reg_membase", NONE, IREG, IREG)
MINI_OP(OP_AMD64_COMPARE_MEMBASE_REG,    "amd64_compare_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_AMD64_COMPARE_MEMBASE_IMM,    "amd64_compare_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_AMD64_COMPARE_REG_MEMBASE,    "amd64_compare_reg_membase", NONE, IREG, IREG)

MINI_OP(OP_AMD64_ADD_MEMBASE_REG,        "amd64_add_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_AMD64_SUB_MEMBASE_REG,        "amd64_sub_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_AMD64_AND_MEMBASE_REG,        "amd64_and_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_AMD64_OR_MEMBASE_REG,         "amd64_or_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_AMD64_XOR_MEMBASE_REG,        "amd64_xor_membase_reg", NONE, IREG, IREG)
MINI_OP(OP_AMD64_MUL_MEMBASE_REG,        "amd64_mul_membase_reg", NONE, IREG, IREG)

MINI_OP(OP_AMD64_ADD_MEMBASE_IMM,        "amd64_add_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_AMD64_SUB_MEMBASE_IMM,        "amd64_sub_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_AMD64_AND_MEMBASE_IMM,        "amd64_and_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_AMD64_OR_MEMBASE_IMM,         "amd64_or_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_AMD64_XOR_MEMBASE_IMM,        "amd64_xor_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_AMD64_MUL_MEMBASE_IMM,        "amd64_mul_membase_imm", NONE, IREG, NONE)
MINI_OP(OP_AMD64_LEA_MEMBASE,            "amd64_lea_membase", IREG, IREG, NONE)

MINI_OP(OP_AMD64_ADD_REG_MEMBASE,        "amd64_add_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_AMD64_SUB_REG_MEMBASE,        "amd64_sub_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_AMD64_AND_REG_MEMBASE,        "amd64_and_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_AMD64_OR_REG_MEMBASE,         "amd64_or_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_AMD64_XOR_REG_MEMBASE,        "amd64_xor_reg_membase", IREG, IREG, IREG)
MINI_OP(OP_AMD64_MUL_REG_MEMBASE,        "amd64_mul_reg_membase", IREG, IREG, IREG)

MINI_OP(OP_AMD64_LOADI8_MEMINDEX,        "amd64_loadi8_memindex", IREG, IREG, IREG)
MINI_OP(OP_AMD64_SAVE_SP_TO_LMF,         "amd64_save_sp_to_lmf", NONE, NONE, NONE)
#endif

#if  defined(TARGET_POWERPC) || defined(TARGET_POWERPC64)
MINI_OP(OP_PPC_SUBFIC,             "ppc_subfic", IREG, IREG, NONE)
MINI_OP(OP_PPC_SUBFZE,             "ppc_subfze", IREG, IREG, NONE)
MINI_OP(OP_PPC_CHECK_FINITE,       "ppc_check_finite", NONE, IREG, NONE)
MINI_OP(OP_PPC_CEIL,               "ppc_ceil", FREG, FREG, NONE)
MINI_OP(OP_PPC_FLOOR,              "ppc_floor", FREG, FREG, NONE)
MINI_OP(OP_PPC_TRUNC,              "ppc_trunc", FREG, FREG, NONE)
#endif

#if defined(TARGET_ARM) || defined(TARGET_ARM64)
MINI_OP(OP_ARM_RSBS_IMM,            "arm_rsbs_imm", IREG, IREG, NONE)
MINI_OP(OP_ARM_RSC_IMM,             "arm_rsc_imm", IREG, IREG, NONE)
/* Set dreg to an r4 value */
MINI_OP(OP_ARM_SETFREG_R4,             "arm_setfreg_r4", FREG, FREG, NONE)
#endif

#if defined(TARGET_S390X)
MINI_OP(OP_S390_LOADARG,	   "s390_loadarg", NONE, NONE, NONE)
MINI_OP(OP_S390_ARGREG, 	   "s390_argreg", NONE, NONE, NONE)
MINI_OP(OP_S390_ARGPTR, 	   "s390_argptr", NONE, NONE, NONE)
MINI_OP(OP_S390_STKARG, 	   "s390_stkarg", NONE, NONE, NONE)
MINI_OP(OP_S390_MOVE,	 	   "s390_move", NONE, IREG, IREG)
MINI_OP(OP_S390_SETF4RET,	   "s390_setf4ret", FREG, FREG, NONE)
MINI_OP(OP_S390_BKCHAIN, 	   "s390_bkchain", IREG, IREG, NONE)
MINI_OP(OP_S390_LADD,          "s390_long_add", LREG, IREG, IREG)
MINI_OP(OP_S390_LADD_OVF,      "s390_long_add_ovf", LREG, IREG, IREG)
MINI_OP(OP_S390_LADD_OVF_UN,   "s390_long_add_ovf_un", LREG, IREG, IREG)
MINI_OP(OP_S390_LSUB,          "s390_long_sub", LREG, IREG, IREG)
MINI_OP(OP_S390_LSUB_OVF,      "s390_long_sub_ovf", LREG, IREG, IREG)
MINI_OP(OP_S390_LSUB_OVF_UN,   "s390_long_sub_ovf_un", LREG, IREG, IREG)
MINI_OP(OP_S390_LNEG,          "s390_long_neg", LREG, IREG, IREG)
MINI_OP(OP_S390_IADD_OVF,       "s390_int_add_ovf", IREG, IREG, IREG)
MINI_OP(OP_S390_IADD_OVF_UN,    "s390_int_add_ovf_un", IREG, IREG, IREG)
MINI_OP(OP_S390_ISUB_OVF,       "s390_int_sub_ovf", IREG, IREG, IREG)
MINI_OP(OP_S390_ISUB_OVF_UN,    "s390_int_sub_ovf_un", IREG, IREG, IREG)
MINI_OP(OP_S390_CRJ,            "s390_crj", IREG, IREG, IREG)
MINI_OP(OP_S390_CLRJ,           "s390_crj_un", IREG, IREG, IREG)
MINI_OP(OP_S390_CGRJ,           "s390_cgrj", LREG, LREG, IREG)
MINI_OP(OP_S390_CLGRJ,          "s390_cgrj_un", LREG, LREG, IREG)
MINI_OP(OP_S390_CIJ,            "s390_cij", IREG, NONE, NONE)
MINI_OP(OP_S390_CLIJ,           "s390_cij_un", IREG, IREG, NONE)
MINI_OP(OP_S390_CGIJ,           "s390_cgij", LREG, NONE, NONE)
MINI_OP(OP_S390_CLGIJ,          "s390_cgij_un", LREG, NONE, NONE)
MINI_OP(OP_S390_VAB, "s390_vab", XREG, XREG, XREG)
MINI_OP(OP_S390_VAH, "s390_vah", XREG, XREG, XREG)
MINI_OP(OP_S390_VAF, "s390_vaf", XREG, XREG, XREG)
MINI_OP(OP_S390_VAG, "s390_vag", XREG, XREG, XREG)
MINI_OP(OP_S390_VFASB, "s390_vfasb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFADB, "s390_vfadb", XREG, XREG, XREG)
MINI_OP(OP_S390_VSB, "s390_vsb", XREG, XREG, XREG)
MINI_OP(OP_S390_VSH, "s390_vsh", XREG, XREG, XREG)
MINI_OP(OP_S390_VSF, "s390_vsf", XREG, XREG, XREG)
MINI_OP(OP_S390_VSG, "s390_vsg", XREG, XREG, XREG)
MINI_OP(OP_S390_VFSSB, "s390_vfssb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFSDB, "s390_vfsdb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMLB, "s390_vmlb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMLHW, "s390_vmlhw", XREG, XREG, XREG)
MINI_OP(OP_S390_VMLF, "s390_vmlf", XREG, XREG, XREG)
MINI_OP(OP_S390_VFMSB, "s390_vfmsb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFMDB, "s390_vfmdb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFDSB, "s390_vfdsb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFDDB, "s390_vfddb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMXB, "s390_vmxb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMXH, "s390_vmxh", XREG, XREG, XREG)
MINI_OP(OP_S390_VMXF, "s390_vmxf", XREG, XREG, XREG)
MINI_OP(OP_S390_VMXG, "s390_vmxg", XREG, XREG, XREG)
MINI_OP(OP_S390_VFMAXSB, "s390_vfmaxsb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFMAXDB, "s390_vfmaxdb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMNB, "s390_vmnb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMNH, "s390_vmnh", XREG, XREG, XREG)
MINI_OP(OP_S390_VMNF, "s390_vmnf", XREG, XREG, XREG)
MINI_OP(OP_S390_VMNG, "s390_vmng", XREG, XREG, XREG)
MINI_OP(OP_S390_VFMINSB, "s390_vfminsb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFMINDB, "s390_vfmindb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMXLB, "s390_vmxlb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMXLH, "s390_vmxlh", XREG, XREG, XREG)
MINI_OP(OP_S390_VMXLF, "s390_vmxlf", XREG, XREG, XREG)
MINI_OP(OP_S390_VMXLG, "s390_vmxlg", XREG, XREG, XREG)
MINI_OP(OP_S390_VMNLB, "s390_vmnlb", XREG, XREG, XREG)
MINI_OP(OP_S390_VMNLH, "s390_vmnlh", XREG, XREG, XREG)
MINI_OP(OP_S390_VMNLF, "s390_vmnlf", XREG, XREG, XREG)
MINI_OP(OP_S390_VMNLG, "s390_vmnlg", XREG, XREG, XREG)
MINI_OP(OP_S390_VO, "s390_vo", XREG, XREG, XREG)
MINI_OP(OP_S390_VNO, "s390_vno", XREG, XREG, XREG)
MINI_OP(OP_S390_VX, "s390_vx", XREG, XREG, XREG)
MINI_OP(OP_S390_VN, "s390_vn", XREG, XREG, XREG)
MINI_OP(OP_S390_VNN, "s390_vnn", XREG, XREG, XREG)
MINI_OP(OP_S390_VSUMB, "s390_vsumb", XREG, XREG, XREG)
MINI_OP(OP_S390_VSUMH, "s390_vsumh", XREG, XREG, XREG)
MINI_OP(OP_S390_VSUMQF, "s390_vsumqf", XREG, XREG, XREG)
MINI_OP(OP_S390_VSUMQG, "s390_vsumqg", XREG, XREG, XREG)
MINI_OP(OP_S390_VPERM, "s390_vperm", XREG, XREG, XREG)
MINI_OP(OP_S390_VREPIB, "s390_vrepib", XREG, NONE, NONE)
MINI_OP(OP_S390_VREPIH, "s390_vrepih", XREG, NONE, NONE)
MINI_OP(OP_S390_VREPIF, "s390_vrepif", XREG, NONE, NONE)
MINI_OP(OP_S390_VREPIG, "s390_vrepig", XREG, NONE, NONE)
MINI_OP(OP_S390_VFSQSB, "s390_vfsqsb", XREG, XREG, NONE)
MINI_OP(OP_S390_VFSQDB, "s390_vfsqdb", XREG, XREG, NONE)
MINI_OP(OP_S390_VFCESBS, "s390_vfcesbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCEDBS, "s390_vfcedbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCHSBS, "s390_vfchsbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCHDBS, "s390_vfchdbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCESB, "s390_vfcesb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCEDB, "s390_vfcedb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCHSB, "s390_vfchsb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCHDB, "s390_vfchdb", XREG, XREG, XREG)
MINI_OP(OP_S390_VCEQBS, "s390_vceqbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCEQHS, "s390_vceqhs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCEQFS, "s390_vceqfs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCEQGS, "s390_vceqgs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCEQB, "s390_vceqb", XREG, XREG, XREG)
MINI_OP(OP_S390_VCEQH, "s390_vceqh", XREG, XREG, XREG)
MINI_OP(OP_S390_VCEQF, "s390_vceqf", XREG, XREG, XREG)
MINI_OP(OP_S390_VCEQG, "s390_vceqg", XREG, XREG, XREG)
MINI_OP(OP_S390_VGMB, "s390_vgmb", XREG, IREG, IREG)
MINI_OP(OP_S390_VGMH, "s390_vgmh", XREG, IREG, IREG)
MINI_OP(OP_S390_VGMF, "s390_vgmf", XREG, IREG, IREG)
MINI_OP(OP_S390_VGMG, "s390_vgmg", XREG, IREG, IREG)
MINI_OP(OP_S390_VECB, "s390_vecb", XREG, XREG, NONE)
MINI_OP(OP_S390_VECF, "s390_vecf", XREG, XREG, NONE)
MINI_OP(OP_S390_VECH, "s390_vech", XREG, XREG, NONE)
MINI_OP(OP_S390_VECG, "s390_vecg", XREG, XREG, NONE)
MINI_OP(OP_S390_VECLB, "s390_veclb", XREG, XREG, NONE)
MINI_OP(OP_S390_VECLF, "s390_veclf", XREG, XREG, NONE)
MINI_OP(OP_S390_VECLH, "s390_veclh", XREG, XREG, NONE)
MINI_OP(OP_S390_VECLG, "s390_veclg", XREG, XREG, NONE)
MINI_OP(OP_S390_VCHBS, "s390_vchbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHHS, "s390_vchhs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHFS, "s390_vchfs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHGS, "s390_vchgs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHB, "s390_vchb", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHH, "s390_vchh", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHF, "s390_vchf", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHG, "s390_vchg", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHLBS, "s390_vchlbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHLHS, "s390_vchlhs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHLFS, "s390_vchlfs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHLGS, "s390_vchlgs", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHLB, "s390_vchlb", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHLH, "s390_vchlh", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHLF, "s390_vchlf", XREG, XREG, XREG)
MINI_OP(OP_S390_VCHLG, "s390_vchlg", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCHESBS, "s390_vfchesbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCHEDBS, "s390_vfchedbs", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCHESB, "s390_vfchesb", XREG, XREG, XREG)
MINI_OP(OP_S390_VFCHEDB, "s390_vfchedb", XREG, XREG, XREG)
MINI_OP(OP_S390_VLPB, "s390_vlpb", XREG, XREG, NONE)
MINI_OP(OP_S390_VLPH, "s390_vlph", XREG, XREG, NONE)
MINI_OP(OP_S390_VLPF, "s390_vlpf", XREG, XREG, NONE)
MINI_OP(OP_S390_VLPG, "s390_vlpg", XREG, XREG, NONE)
MINI_OP(OP_S390_VFLPDB, "s390_vflpdb", XREG, XREG, NONE)
MINI_OP(OP_S390_VFLPSB, "s390_vflpsb", XREG, XREG, NONE)
MINI_OP(OP_S390_VFLCDB, "s390_vflcdb", XREG, XREG, NONE)
MINI_OP(OP_S390_VFLCSB, "s390_vflcsb", XREG, XREG, NONE)
MINI_OP(OP_S390_VPKH, "s390_vpkh", XREG, XREG, XREG)
MINI_OP(OP_S390_VPKF, "s390_vpkf", XREG, XREG, XREG)
MINI_OP(OP_S390_VPKG, "s390_vpkg", XREG, XREG, XREG)
MINI_OP(OP_S390_VLCB, "s390_vlcb", XREG, XREG, NONE)
MINI_OP(OP_S390_VLCH, "s390_vlch", XREG, XREG, NONE)
MINI_OP(OP_S390_VLCF, "s390_vlcf", XREG, XREG, NONE)
MINI_OP(OP_S390_VLCG, "s390_vlcg", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPHB, "s390_vuphb", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPHH, "s390_vuphh", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPHF, "s390_vuphf", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLB, "s390_vuplb", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLHW, "s390_vuplhw", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLF, "s390_vuplf", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLHB, "s390_vuplhb", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLHH, "s390_vuplhh", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLHF, "s390_vuplhf", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLLB, "s390_vupllb", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLLH, "s390_vupllh", XREG, XREG, NONE)
MINI_OP(OP_S390_VUPLLF, "s390_vupllf", XREG, XREG, NONE)
MINI_OP(OP_S390_VFISB, "s390_vfidb", XREG, XREG, NONE)
MINI_OP(OP_S390_VFIDB, "s390_vfisb", XREG, XREG, NONE)
MINI_OP(OP_S390_XCOMPARE_XEXTRACT, "s390_xcompare_xextract", IREG, XREG, XREG)
#endif

#if defined(TARGET_ARM64)
/* Branch if sreg1 == 0 */
MINI_OP(OP_ARM64_CBZW, "arm64_cbzw", NONE, IREG, NONE)
MINI_OP(OP_ARM64_CBZX, "arm64_cbzx", NONE, IREG, NONE)
/* Branch if sreg1 != 0 */
MINI_OP(OP_ARM64_CBNZW, "arm64_cbnzw", NONE, IREG, NONE)
MINI_OP(OP_ARM64_CBNZX, "arm64_cbnzx", NONE, IREG, NONE)
MINI_OP(OP_ARM64_HINT, "arm64_hint", NONE, NONE, NONE)
#endif

/* Same as OUTARG_VT, but has a dreg */
MINI_OP(OP_LLVM_OUTARG_VT,	"llvm_outarg_vt", IREG, VREG, NONE)

MINI_OP(OP_OBJC_GET_SELECTOR, "objc_get_selector", IREG, NONE, NONE)

MINI_OP(OP_GET_SP, "get_sp", IREG, NONE, NONE)
MINI_OP(OP_SET_SP, "set_sp", NONE, IREG, NONE)

MINI_OP(OP_GET_LAST_ERROR, "get_last_error", IREG, NONE, NONE)

/*
 * Fill out a MonoContext contained in a MonoProfilerCallContext. This only
 * stores the stack pointer, frame pointer, and callee-saved registers. This
 * should be enough to locate arguments and variables.
 */
MINI_OP(OP_FILL_PROF_CALL_CTX, "fill_prof_call_ctx", NONE, IREG, NONE)

/* LLVM only, compare 2 vectors for equality, set dreg to 1/0 */
MINI_OP(OP_XEQUAL, "xequal", IREG, XREG, XREG)
#if defined(TARGET_ARM64)
MINI_OP(OP_XEQUAL_ARM64_V128_FAST, "arm64_xequal_v128", IREG, XREG, XREG)
#endif
/* Per element compate, inst_c0 contains a CompRelation */
MINI_OP(OP_XCOMPARE, "xcompare", XREG, XREG, XREG)
MINI_OP(OP_XCOMPARE_SCALAR, "xcompare_scalar", XREG, XREG, XREG)
MINI_OP(OP_XCOMPARE_FP, "xcompare_fp", XREG, XREG, XREG)
MINI_OP(OP_XCOMPARE_FP_SCALAR, "xcompare_fp_scalar", XREG, XREG, XREG)

/*
 * The input reg is the result ofg OP_XCOMPARE, i.e.
 * every element is either 0 or 0xff.
 * Compute an integer result based on whenever all or any
 * bits are non-zero.
 *   inst_c0 - specific instruction, one of SIMD_EXTR_...
 *   inst_c1 - vector size in bytes
 */
MINI_OP(OP_XEXTRACT, "xextract", IREG, XREG, NONE)

/*
 * Generic SIMD operations, the rest of the JIT doesn't care about the exact operation.
 */
MINI_OP(OP_XUNOP, "xunop", XREG, XREG, NONE)
/* inst_c0 is a OP_ constant, inst_c1 is a MONO_TYPE_ constant */
MINI_OP(OP_XBINOP, "xbinop", XREG, XREG, XREG)
/* The arguments are treated as vectors of integer types. inst_c0 is a XBINOP_FORCEINT_ constant */
MINI_OP(OP_XBINOP_FORCEINT, "xbinop_forceint", XREG, XREG, XREG)
MINI_OP(OP_XBINOP_SCALAR, "xbinop_scalar", XREG, XREG, XREG)
MINI_OP(OP_XBINOP_BYSCALAR, "xbinop_byscalar", XREG, XREG, XREG)

/* inst_c0 contains an INTRINS_ enum, inst_c1 might contain additional data */
MINI_OP(OP_XOP, "xop", NONE, NONE, NONE)
MINI_OP(OP_XOP_X_I, "xop_x_i", XREG, IREG, NONE)
MINI_OP(OP_XOP_X_X, "xop_x_x", XREG, XREG, NONE)
MINI_OP(OP_XOP_I4_X, "xop_i4_x", IREG, XREG, NONE)
MINI_OP(OP_XOP_I4_X_X, "xop_i4_x_x", IREG, XREG, XREG)
MINI_OP(OP_XOP_I8_X, "xop_i8_x", LREG, XREG, NONE)
MINI_OP(OP_XOP_X_X_X, "xop_x_x_x", XREG, XREG, XREG)
MINI_OP(OP_XOP_X_X_I4, "xop_x_x_i4", XREG, XREG, IREG)
MINI_OP(OP_XOP_X_X_I8, "xop_x_x_i8", XREG, XREG, LREG)
MINI_OP(OP_XOP_I4_I8, "xop_i4_i8", IREG, LREG, NONE)
MINI_OP(OP_XOP_I8_I8, "xop_i8_i8", LREG, LREG, NONE)
MINI_OP(OP_XOP_I4_I4, "xop_i4_i4", IREG, IREG, NONE)
MINI_OP(OP_XOP_I4_I4_I4, "xop_i4_i4_i4", IREG, IREG, IREG)
MINI_OP(OP_XOP_I4_I4_I8, "xop_i4_i4_i8", IREG, IREG, LREG)
MINI_OP3(OP_XOP_X_X_X_X, "xop_x_x_x_x", XREG, XREG, XREG, XREG)

MINI_OP(OP_XOP_OVR_X_X, "xop_ovr_x_x", XREG, XREG, NONE)
MINI_OP(OP_XOP_OVR_X_X_X, "xop_ovr_x_x_x", XREG, XREG, XREG)
MINI_OP3(OP_XOP_OVR_X_X_X_X, "xop_ovr_x_x_x_x", XREG, XREG, XREG, XREG)
MINI_OP(OP_XOP_OVR_SCALAR_X_X, "xop_ovr_scalar_x_x", XREG, XREG, NONE)
MINI_OP(OP_XOP_OVR_SCALAR_X_X_X, "xop_ovr_scalar_x_x_x", XREG, XREG, XREG)
MINI_OP3(OP_XOP_OVR_SCALAR_X_X_X_X, "xop_ovr_scalar_x_x_x_x", XREG, XREG, XREG, XREG)
MINI_OP(OP_XOP_OVR_BYSCALAR_X_X_X, "xop_ovr_byscalar_x_x_x", XREG, XREG, XREG)

MINI_OP(OP_XCONCAT, "xconcat", XREG, XREG, XREG)
MINI_OP(OP_XCAST, "xcast", XREG, XREG, NONE)
/* Return a new vector containing the lower half of the source */
MINI_OP(OP_XLOWER, "xlower", XREG, XREG, NONE)
/* Return a new vector containing the upper half of the source */
MINI_OP(OP_XUPPER, "xupper", XREG, XREG, NONE)
MINI_OP(OP_XWIDEN, "xwiden", XREG, XREG, NONE)
MINI_OP(OP_XWIDEN_UNSAFE, "xwiden_unsafe", XREG, XREG, NONE)
MINI_OP(OP_XINSERT_LOWER, "xinsert_lower", XREG, XREG, XREG)
MINI_OP(OP_XINSERT_UPPER, "xinsert_upper", XREG, XREG, XREG)

/* Extract an element from a vector with a variable lane index.
 * The index is assumed to be in range.
 * inst_c1 is a MonoTypeEnum representing the element type.
 */
MINI_OP(OP_XEXTRACT_I1, "xextract_i1", IREG, XREG, IREG)
MINI_OP(OP_XEXTRACT_I2, "xextract_i2", IREG, XREG, IREG)
MINI_OP(OP_XEXTRACT_I4, "xextract_i4", IREG, XREG, IREG)
MINI_OP(OP_XEXTRACT_I8, "xextract_i8", LREG, XREG, IREG)
MINI_OP(OP_XEXTRACT_R4, "xextract_r4", FREG, XREG, IREG)
MINI_OP(OP_XEXTRACT_R8, "xextract_r8", FREG, XREG, IREG)

/* Insert an element into a vector with a variable lane index.
 * sreg1 is the vector, sreg2 is the value, sreg3 is the index.
 * inst_c1 is a MonoTypeEnum representing the element type, used to control
 * sign/zero extension if conversion of sreg2 happens to be necessary.
 * klass must be set to the MonoClass representing the return type of the op.
 */
MINI_OP3(OP_XINSERT_I1, "xinsert_i1", XREG, XREG, IREG, IREG)
MINI_OP3(OP_XINSERT_I2, "xinsert_i2", XREG, XREG, IREG, IREG)
MINI_OP3(OP_XINSERT_I4, "xinsert_i4", XREG, XREG, IREG, IREG)
MINI_OP3(OP_XINSERT_I8, "xinsert_i8", XREG, XREG, LREG, IREG)
MINI_OP3(OP_XINSERT_R4, "xinsert_r4", XREG, XREG, FREG, IREG)
MINI_OP3(OP_XINSERT_R8, "xinsert_r8", XREG, XREG, FREG, IREG)


MINI_OP(OP_LZCNT32, "lzcnt32", IREG, IREG, NONE)
MINI_OP(OP_LZCNT64, "lzcnt64", LREG, LREG, NONE)
MINI_OP(OP_POPCNT32, "popcnt32", IREG, IREG, NONE)
MINI_OP(OP_POPCNT64, "popcnt64", LREG, LREG, NONE)

#if defined(TARGET_ARM64)
MINI_OP(OP_LSCNT32, "lscnt32", IREG, IREG, NONE)
MINI_OP(OP_LSCNT64, "lscnt64", LREG, LREG, NONE)

MINI_OP(OP_ARM64_CLZ, "arm64_clz", XREG, XREG, NONE)

MINI_OP3(OP_ARM64_LD1_INSERT, "arm64_ld1_insert", XREG, XREG, IREG, IREG)
MINI_OP3(OP_ARM64_LDM_INSERT, "arm64_ldm_insert", VREG, VREG, IREG, IREG)

MINI_OP(OP_ARM64_LD1, "arm64_ld1", XREG, IREG, NONE)
MINI_OP(OP_ARM64_LD1R, "arm64_ld1r", XREG, IREG, NONE)

/* Loads two consecutive vector (or scalar) values into a pair of
 * vector registers. Returns a ValueTuple in a vreg. klass must be set to the
 * MonoClass representing the ValueTuple return type of the op.
 */
MINI_OP(OP_ARM64_LDNP, "arm64_ldnp", VREG, IREG, NONE)
MINI_OP(OP_ARM64_LDNP_SCALAR, "arm64_ldnp_scalar", VREG, IREG, NONE)
MINI_OP(OP_ARM64_LDP, "arm64_ldp", VREG, IREG, NONE)
MINI_OP(OP_ARM64_LDP_SCALAR, "arm64_ldp_scalar", VREG, IREG, NONE)

MINI_OP(OP_ARM64_LDM, "arm64_ldm", VREG, IREG, NONE)

MINI_OP(OP_ARM64_ST1, "arm64_st1", NONE, IREG, XREG)
MINI_OP(OP_ARM64_SXTL, "arm64_sxtl", XREG, XREG, NONE)
MINI_OP(OP_ARM64_SXTL2, "arm64_sxtl2", XREG, XREG, NONE)

MINI_OP(OP_ARM64_STM, "arm64_stm", NONE, IREG, VREG)
MINI_OP(OP_ARM64_STM_ZIP, "arm64_stm_zip", NONE, IREG, VREG)

MINI_OP(OP_ARM64_SMULH, "arm64_smulh", LREG, LREG, LREG)
MINI_OP(OP_ARM64_SQRT_SCALAR, "arm64_sqrt_scalar", XREG, XREG, NONE)
MINI_OP(OP_ARM64_TRN1, "arm64_trn1", XREG, XREG, XREG)
MINI_OP(OP_ARM64_TRN2, "arm64_trn2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UMULH, "arm64_umulh", LREG, LREG, LREG)
MINI_OP(OP_ARM64_UXTL, "arm64_uxtl", XREG, XREG, NONE)
MINI_OP(OP_ARM64_UXTL2, "arm64_uxtl2", XREG, XREG, NONE)
MINI_OP(OP_ARM64_UZP1, "arm64_uzp1", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UZP2, "arm64_uzp2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_ZIP1, "arm64_zip1", XREG, XREG, XREG)
MINI_OP(OP_ARM64_ZIP2, "arm64_zip2", XREG, XREG, XREG)
MINI_OP3(OP_ARM64_ST1_SCALAR, "arm64_st1_scalar", NONE, IREG, XREG, IREG)
MINI_OP3(OP_ARM64_STM_SCALAR, "arm64_stm_scalar", NONE, IREG, VREG, IREG)
MINI_OP3(OP_ARM64_STNP, "arm64_stnp", NONE, IREG, XREG, XREG)
MINI_OP3(OP_ARM64_STNP_SCALAR, "arm64_stnp_scalar", NONE, IREG, XREG, XREG)
MINI_OP3(OP_ARM64_STP, "arm64_stp", NONE, IREG, XREG, XREG)
MINI_OP3(OP_ARM64_STP_SCALAR, "arm64_stp_scalar", NONE, IREG, XREG, XREG)

MINI_OP(OP_ARM64_ADDHN, "arm64_addhn", XREG, XREG, XREG)
MINI_OP3(OP_ARM64_ADDHN2, "arm64_addhn2", XREG, XREG, XREG, XREG)
MINI_OP(OP_ARM64_SUBHN, "arm64_subhn", XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SUBHN2, "arm64_subhn2", XREG, XREG, XREG, XREG)

MINI_OP(OP_ARM64_RADDHN, "arm64_raddhn", XREG, XREG, XREG)
MINI_OP3(OP_ARM64_RADDHN2, "arm64_raddhn2", XREG, XREG, XREG, XREG)
MINI_OP(OP_ARM64_RSUBHN, "arm64_rsubhn", XREG, XREG, XREG)
MINI_OP3(OP_ARM64_RSUBHN2, "arm64_rsubhn2", XREG, XREG, XREG, XREG)

MINI_OP(OP_ARM64_SHRN, "arm64_shrn", XREG, XREG, IREG)
MINI_OP3(OP_ARM64_SHRN2, "arm64_shrn2", XREG, XREG, XREG, IREG)

MINI_OP3(OP_ARM64_SLI, "arm64_sli", XREG, XREG, XREG, IREG)
MINI_OP3(OP_ARM64_SRI, "arm64_sri", XREG, XREG, XREG, IREG)

MINI_OP(OP_ARM64_SRSHR, "arm64_srshr", XREG, XREG, IREG)
MINI_OP(OP_ARM64_URSHR, "arm64_urshr", XREG, XREG, IREG)
MINI_OP3(OP_ARM64_SRSRA, "arm64_srsra", XREG, XREG, XREG, IREG)
MINI_OP3(OP_ARM64_URSRA, "arm64_ursra", XREG, XREG, XREG, IREG)

/* Narrowing arm64 shifts that aren't decomposed into urshl or srshl. */
MINI_OP(OP_ARM64_XNSHIFT_SCALAR, "arm64_xrshift_scalar", XREG, XREG, IREG)
MINI_OP(OP_ARM64_XNSHIFT, "arm64_xnshift", XREG, XREG, IREG)
MINI_OP3(OP_ARM64_XNSHIFT2, "arm64_xnshift2", XREG, XREG, XREG, IREG)

MINI_OP(OP_ARM64_SQSHLU, "arm64_sqshlu", XREG, XREG, IREG)
MINI_OP(OP_ARM64_SQSHLU_SCALAR, "arm64_sqshlu_scalar", XREG, XREG, IREG)

MINI_OP(OP_ARM64_REVN, "arm64_revn", XREG, XREG, NONE)

MINI_OP(OP_ARM64_PMULL, "arm64_pmull", XREG, XREG, XREG)
MINI_OP(OP_ARM64_PMULL2, "arm64_pmull2", XREG, XREG, XREG)

MINI_OP(OP_ARM64_SMULL, "arm64_smull", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SMULL_SCALAR, "arm64_smull_scalar", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SMULL2, "arm64_smull2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SMULL2_SCALAR, "arm64_smull2_scalar", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UMULL, "arm64_umull", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UMULL_SCALAR, "arm64_umull_scalar", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UMULL2, "arm64_umull2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UMULL2_SCALAR, "arm64_umull2_scalar", XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SMLAL, "arm64_smlal", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SMLAL_SCALAR, "arm64_smlal_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SMLAL2, "arm64_smlal2", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SMLAL2_SCALAR, "arm64_smlal2_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UMLAL, "arm64_umlal", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UMLAL_SCALAR, "arm64_umlal_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UMLAL2, "arm64_umlal2", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UMLAL2_SCALAR, "arm64_umlal2_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SMLSL, "arm64_smlsl", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SMLSL_SCALAR, "arm64_smlsl_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SMLSL2, "arm64_smlsl2", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SMLSL2_SCALAR, "arm64_smlsl2_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UMLSL, "arm64_umlsl", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UMLSL_SCALAR, "arm64_umlsl_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UMLSL2, "arm64_umlsl2", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UMLSL2_SCALAR, "arm64_umlsl2_scalar", XREG, XREG, XREG, XREG)

MINI_OP3(OP_ARM64_MLS, "arm64_mls", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_MLS_SCALAR, "arm64_mls_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_MLA, "arm64_mla", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_MLA_SCALAR, "arm64_mla_scalar", XREG, XREG, XREG, XREG)

MINI_OP3(OP_ARM64_FMUL_SEL, "arm64_fmul_sel", XREG, XREG, XREG, IREG)

MINI_OP(OP_ARM64_SQDMULL_SCALAR, "arm64_sqdmull_scalar", XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLSL_SCALAR, "arm64_sqdmlsl_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLAL_SCALAR, "arm64_sqdmlal_scalar", XREG, XREG, XREG, XREG)

MINI_OP(OP_ARM64_SQDMULL, "arm64_sqdmull", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SQDMULL_BYSCALAR, "arm64_sqdmull_byscalar", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SQDMULL2, "arm64_sqdmull2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SQDMULL2_BYSCALAR, "arm64_sqdmull2_byscalar", XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLSL, "arm64_sqdmlsl", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLSL_BYSCALAR, "arm64_sqdmlsl_byscalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLAL, "arm64_sqdmlal", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLAL_BYSCALAR, "arm64_sqdmlal_byscalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLSL2, "arm64_sqdmlsl2", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLSL2_BYSCALAR, "arm64_sqdmlsl2_byscalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLAL2, "arm64_sqdmlal2", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQDMLAL2_BYSCALAR, "arm64_sqdmlal2_byscalar", XREG, XREG, XREG, XREG)

MINI_OP3(OP_ARM64_FMSUB, "arm64_fmsub", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_FMSUB_BYSCALAR, "arm64_fmsub_byscalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_FMSUB_SCALAR, "arm64_fmsub_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_FNMSUB_SCALAR, "arm64_fnmsub_scalar", XREG, XREG, XREG, XREG)

MINI_OP3(OP_ARM64_FMADD, "arm64_fmadd", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_FMADD_BYSCALAR, "arm64_fmadd_byscalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_FMADD_SCALAR, "arm64_fmadd_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_FNMADD_SCALAR, "arm64_fnmadd_scalar", XREG, XREG, XREG, XREG)

MINI_OP(OP_ARM64_XTN, "arm64_xtn", XREG, XREG, NONE)
MINI_OP(OP_ARM64_XTN2, "arm64_xtn2", XREG, XREG, XREG)

MINI_OP(OP_ARM64_SQXTN2, "arm64_sqxtn2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UQXTN2, "arm64_uqxtn2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SQXTUN2, "arm64_sqxtun2", XREG, XREG, XREG)

MINI_OP(OP_ARM64_SELECT_SCALAR, "arm64_select_scalar", XREG, XREG, IREG)
MINI_OP(OP_ARM64_SELECT_QUAD, "arm64_select_quad", XREG, XREG, IREG)
/* Take a word elem of sreg1 identified by inst_c0 and broadcast it to all elements of dreg */
MINI_OP(OP_ARM64_BROADCAST_ELEM, "arm64_broadcast_elem", XREG, XREG, NONE)

MINI_OP(OP_ARM64_FCVTN, "arm64_fcvtn", XREG, XREG, NONE)
MINI_OP(OP_ARM64_FCVTN2, "arm64_fcvtn2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_FCVTXN, "arm64_fcvtxn", XREG, XREG, NONE)
MINI_OP(OP_ARM64_FCVTXN2, "arm64_fcvtxn2", XREG, XREG, XREG)

MINI_OP(OP_ARM64_CMTST, "arm64_cmtst", XREG, XREG, XREG)

MINI_OP(OP_ARM64_BIC, "arm64_bic", XREG, XREG, XREG)

MINI_OP(OP_ARM64_SADD, "arm64_sadd", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SADD2, "arm64_sadd2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UADD, "arm64_uadd", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UADD2, "arm64_uadd2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SSUB, "arm64_ssub", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SSUB2, "arm64_ssub2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_USUB, "arm64_usub", XREG, XREG, XREG)
MINI_OP(OP_ARM64_USUB2, "arm64_usub2", XREG, XREG, XREG)

MINI_OP(OP_ARM64_ADDP_SCALAR, "arm64_addp_scalar", XREG, XREG, NONE)
MINI_OP(OP_ARM64_FADDP_SCALAR, "arm64_faddp_scalar", XREG, XREG, NONE)

MINI_OP(OP_ARM64_SADALP, "arm64_sadalp_scalar", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UADALP, "arm64_uadalp_scalar", XREG, XREG, XREG)

MINI_OP(OP_ARM64_SADDLV, "arm64_saddlv", XREG, XREG, NONE)
MINI_OP(OP_ARM64_UADDLV, "arm64_uaddlv", XREG, XREG, NONE)
MINI_OP(OP_ARM64_XADDV, "arm64_xaddv", XREG, XREG, NONE)
MINI_OP(OP_ARM64_XHORIZ, "arm64_xhoriz", XREG, XREG, NONE)

MINI_OP3(OP_ARM64_SABAL, "arm64_sabal", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SABAL2, "arm64_sabal2", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UABAL, "arm64_uabal", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UABAL2, "arm64_uabal2", XREG, XREG, XREG, XREG)

MINI_OP(OP_ARM64_SABDL, "arm64_sabdl", XREG, XREG, XREG)
MINI_OP(OP_ARM64_SABDL2, "arm64_sabdl2", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UABDL, "arm64_uabdl", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UABDL2, "arm64_uabdl2", XREG, XREG, XREG)

MINI_OP3(OP_ARM64_SABA, "arm64_saba", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_UABA, "arm64_uaba", XREG, XREG, XREG, XREG)
MINI_OP(OP_ARM64_SABD, "arm64_sabd", XREG, XREG, XREG)
MINI_OP(OP_ARM64_UABD, "arm64_uabd", XREG, XREG, XREG)

MINI_OP(OP_ARM64_ABSCOMPARE, "arm64_abscompare", XREG, XREG, XREG)

MINI_OP(OP_ARM64_XNARROW_SCALAR, "arm64_xnarrow_scalar", XREG, XREG, NONE)

MINI_OP3(OP_ARM64_EXT, "arm64_ext", XREG, XREG, XREG, IREG)

MINI_OP3(OP_ARM64_SQRDMLAH, "arm64_sqrdmlah", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQRDMLAH_BYSCALAR, "arm64_sqrdmlah_byscalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQRDMLAH_SCALAR, "arm64_sqrdmlah_scalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQRDMLSH, "arm64_sqrdmlsh", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQRDMLSH_BYSCALAR, "arm64_sqrdmlsh_byscalar", XREG, XREG, XREG, XREG)
MINI_OP3(OP_ARM64_SQRDMLSH_SCALAR, "arm64_sqrdmlsh_scalar", XREG, XREG, XREG, XREG)

/*
 * sreg1 points to a memory area with the input vectors.
 * inst_c0 is the number of vectors.
 * inst_p1 points to an int array with the offsets inside the memory area.
 */
MINI_OP(OP_ARM64_TBL_INDIRECT, "arm64_tbl_indirect", XREG, IREG, XREG)
MINI_OP3(OP_ARM64_TBX_INDIRECT, "arm64_tbx_indirect", XREG, IREG, XREG, XREG)

#endif // TARGET_ARM64

MINI_OP(OP_SIMD_FCVTL, "simd_convert_to_higher_precision", XREG, XREG, NONE)
MINI_OP(OP_SIMD_FCVTL2, "simd_convert_to_higher_precision_2", XREG, XREG, NONE)
MINI_OP(OP_SIMD_USHLL, "simd_unsigned_shift_left_long", XREG, XREG, IREG)
MINI_OP(OP_SIMD_USHLL2, "simd_unsigned_shift_left_long_2", XREG, XREG, IREG)
MINI_OP(OP_SIMD_SSHLL, "simd_signed_shift_left_long", XREG, XREG, IREG)
MINI_OP(OP_SIMD_SSHLL2, "simd_signed_shift_left_long_2", XREG, XREG, IREG)
MINI_OP(OP_SIMD_SHL, "simd_shl", XREG, XREG, IREG)
MINI_OP(OP_SIMD_SSHR, "simd_sshr", XREG, XREG, IREG)
MINI_OP(OP_SIMD_USHR, "simd_ushr", XREG, XREG, IREG)
MINI_OP3(OP_SIMD_USRA, "simd_usra", XREG, XREG, XREG, IREG)
MINI_OP3(OP_SIMD_SSRA, "simd_ssra", XREG, XREG, XREG, IREG)
MINI_OP(OP_SIMD_LOAD_SCALAR_I4, "simd_load_scalar_i4", XREG, IREG, NONE)
MINI_OP(OP_SIMD_LOAD_SCALAR_I8, "simd_load_scalar_i8", XREG, IREG, NONE)
MINI_OP(OP_SIMD_LOAD_SCALAR_R8, "simd_load_scalar_r8", XREG, IREG, NONE)
MINI_OP(OP_SIMD_STORE, "simd_store", NONE, XREG, XREG)

#if defined(TARGET_ARM64) || defined(TARGET_AMD64) || defined(TARGET_WASM)
MINI_OP(OP_CVT_FP_UI,        "convert_fp_to_ui", XREG, XREG, NONE)
MINI_OP(OP_CVT_FP_SI,        "convert_fp_to_si", XREG, XREG, NONE)
MINI_OP(OP_CVT_FP_UI_SCALAR, "convert_fp_to_ui_scalar", XREG, XREG, NONE)
MINI_OP(OP_CVT_FP_SI_SCALAR, "convert_fp_to_si_scalar", XREG, XREG, NONE)
MINI_OP(OP_CVT_UI_FP,        "convert_ui_to_fp", XREG, XREG, NONE)
MINI_OP(OP_CVT_SI_FP,        "convert_si_to_fp", XREG, XREG, NONE)
MINI_OP(OP_CVT_UI_FP_SCALAR, "convert_ui_to_fp_scalar", XREG, XREG, NONE)
MINI_OP(OP_CVT_SI_FP_SCALAR, "convert_si_to_fp_scalar", XREG, XREG, NONE)
/* inst_c1 is one of the MONO_TYPE_ constants */
MINI_OP(OP_NEGATION_SCALAR, "negate_scalar", XREG, XREG, NONE)
#endif // TARGET_ARM64 || TARGET_AMD64 || TARGET_WASM

#if defined(TARGET_ARM64) || defined(TARGET_AMD64) || defined(TARGET_WASM) || defined(TARGET_S390X)
MINI_OP(OP_NEGATION,        "negate", XREG, XREG, NONE)
MINI_OP(OP_ONES_COMPLEMENT,  "ones_complement", XREG, XREG, NONE)
/* Select bits from src2/src3 using src1 */
MINI_OP3(OP_BSL,            "bitwise_select", XREG, XREG, XREG, XREG)
MINI_OP(OP_VECTOR_ANDN, "vector_andnot", XREG, XREG, XREG)
MINI_OP(OP_VECTOR_IABS, "vector_integer_abs", XREG, XREG, NONE)
#endif

#if defined(TARGET_RISCV64) || defined(TARGET_RISCV32)
MINI_OP(OP_RISCV_EXC_BEQ, "riscv_exc_beq", NONE, IREG, IREG)
MINI_OP(OP_RISCV_EXC_BNE, "riscv_exc_bne", NONE, IREG, IREG)
MINI_OP(OP_RISCV_EXC_BGEU, "riscv_exc_bgeu", NONE, IREG, IREG)
MINI_OP(OP_RISCV_EXC_BLT, "riscv_exc_blt", NONE, IREG, IREG)
MINI_OP(OP_RISCV_EXC_BLTU, "riscv_exc_bltu", NONE, IREG, IREG)

MINI_OP(OP_RISCV_BEQ, "riscv_beq", NONE, IREG, IREG)
MINI_OP(OP_RISCV_BNE, "riscv_bne", NONE, IREG, IREG)
MINI_OP(OP_RISCV_BGE, "riscv_bge", NONE, IREG, IREG)
MINI_OP(OP_RISCV_BGEU, "riscv_bgeu", NONE, IREG, IREG)
MINI_OP(OP_RISCV_BLT, "riscv_blt", NONE, IREG, IREG)
MINI_OP(OP_RISCV_BLTU, "riscv_bltu", NONE, IREG, IREG)
MINI_OP(OP_RISCV_RBNAN, "riscv_r4_bnan", NONE, FREG, NONE)
MINI_OP(OP_RISCV_FBNAN, "riscv_float_bnan", NONE, FREG, NONE)

MINI_OP(OP_RISCV_ADDIW, "riscv_addiw", IREG, IREG, NONE)

MINI_OP(OP_RISCV_SLT, "riscv_slt", IREG, IREG, IREG)
MINI_OP(OP_RISCV_SLTU, "riscv_sltu", IREG, IREG, IREG)
MINI_OP(OP_RISCV_SLTI, "riscv_slti", IREG, IREG, NONE)
MINI_OP(OP_RISCV_SLTIU, "riscv_sltiu", IREG, IREG, NONE)

// used for cfg->r4fp == FALSE
MINI_OP(OP_RISCV_SETFREG_R4,"riscv_setfreg_r4", FREG, FREG, NONE)
#endif

#if defined(TARGET_RISCV64)
MINI_OP(OP_RISCV_ADDUW, "riscv_adduw", IREG, IREG, IREG)
#endif
