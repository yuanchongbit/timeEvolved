/*
 *      Decompiler project
 *      Copyright (c) 2005-2018 Hex-Rays SA <support@hex-rays.com>
 *      ALL RIGHTS RESERVED.
 *
 *      Verify microcode consistency
 *
 */

#include "allmicro.h"

// PC-lint errs on union members:
//lint -esym(413,nnn) Likely use of null
//lint -esym(413,d)
//lint -esym(413,f)
//lint -esym(413,a)
//lint -esym(413,c)
//lint -esym(413,fpc)
//lint -esym(413,pair)

//-------------------------------------------------------------------------
bool should_verify()
{
#ifndef NDEBUG
  return true;
#else
  return under_debugger || (vdrun_flags & VDRUN_TEST) != 0;
#endif
}

//-------------------------------------------------------------------------
void mcallinfo_t::verify(const micro_verifier_t &mv, int size) const
{
  if ( get_cc(cc) == CM_CC_INVALID )
    INTERR(50733);
  // check args
  for ( int i=0; i < args.size(); i++ )
  {
    const mcallarg_t &a = args[i];
    if ( !a.type.is_correct() )
      INTERR(50734);
    if ( a.type.get_size() != a.size )
      INTERR(50735);
    if ( a.size == 0 )
      INTERR(50736);
    if ( a.ea != BADADDR && !is_mapped(a.ea) )
      INTERR(51066);
    if ( verify_argloc(a.argloc, a.size, NULL) != 0 )
      INTERR(50732);
    // we already checked that the operand size is equal to type size
    a.verify(mv, VMOP_ANYSIZE);
    if ( a.t == mop_z )
      INTERR(50737);
  }
  if ( solid_args > args.size() )
    INTERR(50738);
  if ( !return_type.is_correct() )
    INTERR(50739);
  if ( !pass_regs.empty() )
  {
    if ( !mv.mba->has_passregs() )
      INTERR(51087);
    if ( !pass_regs.is_subset_of(spoiled) )
      INTERR(50991);
  }
  if ( !return_regs.is_subset_of(spoiled) )
    INTERR(50740);
  if ( !dead_regs.is_subset_of(return_regs) )
    INTERR(50741);

  if ( size == NOSIZE )
    INTERR(50742);
  if ( (flags & FCI_PROP) == 0 )
  {
    mlist_t tmp = used_retvals();
    if ( tmp.reg.count() != size )
      INTERR(50743);
    int s2 = 0;
    int vmop = 0;
    // special handling for long double (10 bytes)
    if ( retregs.size() == 1 && return_type.get_size() == retregs[0].size )
      vmop = VMOP_ANYSIZE;
    for ( int i=0; i < retregs.size(); i++ )
    {
      const mop_t &m = retregs[i];
      m.verify(mv, vmop);
      s2 += m.size;
    }
    if ( flags & FCI_DEAD )  // some return registers are dead
    {
      if ( s2 < size )
        INTERR(50744);
    }
    else
    {
      if ( s2 != size )
        INTERR(50745);
    }
  }
}

//-------------------------------------------------------------------------
void mcases_t::verify(const micro_verifier_t &mv) const
{
  int n = targets.size();
  if ( n != values.size() )
    INTERR(50746);
  if ( n == 0 )
    INTERR(50747);
  if ( n == 1 )
  {
    int nvals = values[0].size();
    if ( nvals == 0 )
      INTERR(50748);
  }
  bool seen_default = false;
  std::set<uint64> seen;
  easet_t targset;
  for ( int i=0; i < n; i++ )
  {
    const svalvec_t &v = values[i];
    if ( v.empty() )
    {
      if ( seen_default )
        INTERR(50750);
      seen_default = true;
    }
    for ( int j=0; j < v.size(); j++ )
      if ( !seen.insert(v[j]).second )
        INTERR(50751); // duplicate case value
    int b = targets[i];
    if ( b <= 0 || b >= mv.mba->qty )
      INTERR(50752);
    if ( !targset.insert(b).second )
      INTERR(50753); // duplicate target
  }

}

//-------------------------------------------------------------------------
inline bool valid_pair_part(mopt_t t)
{
  switch ( t )
  {
    case mop_r:
    case mop_n:
    case mop_d:
    case mop_S:
    case mop_v:
    case mop_l:
    case mop_a:
    case mop_fn:
    case mop_p:
    case mop_sc:
      return true;
  }
  return false;
}

//-------------------------------------------------------------------------
void mop_t::verify(const micro_verifier_t &mv, int flags) const
{
  // check the operand size
  switch ( t )
  {
    case mop_z:
      // propagated insn destination must have a valid size
      if ( (flags & VMOP_PROPDST) != 0 )
        break;
       // no break
    case mop_b:           // basic blocks have no size
    case mop_c:           // cases have no size
      if ( size != NOSIZE )
        INTERR(50754);
    case mop_h:           // helper functions have no size
      break;
    case mop_str:         // constant strings must be SPWIDTH
      if ( size != SPWIDTH )
        INTERR(50755);
      // fallthrough
    case mop_n:
      if ( size <= 0 || size > 8 )
        INTERR(51586);
      if ( (flags & VMOP_ANYSIZE) == 0 && (size&(size-1)) != 0 )
        INTERR(51587);
      break;
    case mop_r:
    default:
      // mop_a operand size is unknown
      if ( (flags & VMOP_ADRUSED) != 0 )
      {
        // &reg must be used with the size info.
        if ( t != mop_r && t != mop_l && size != NOSIZE )
          INTERR(50756);
      }
      else if ( (flags & VMOP_ANYSIZE) == 0 && !is_udt() )
      {
        // function calls might end up having zero size if the returned
        // result is not used or the function does not return anything
        if ( !is_valid_size(size)
          && size != sizeof_ldbl()
          && size != ph.tbyte_size
          && (t != mop_f || size != 0 && size != DOUBLE_OPSIZE) // calls may be void or xmm
          && (size != DOUBLE_OPSIZE || !accepts_double_size_ops(mv.curins->opcode)) )
        {
          // allow any size for external instruction addresses
          if ( mv.mba == NULL || !mv.mba->is_extins_ea(mv.curins->ea) )
            INTERR(50757);
        }
        if ( (flags & VMOP_FPVAL) != 0 )
          if ( size != 4 && size != 8 && size != sizeof_ldbl() && size != ph.tbyte_size )
            INTERR(51275);
      }
      break;
  }

  switch ( t )
  {
    case mop_z: // none
      break;
    case mop_n: // immediate
      if ( nnn == NULL )
        INTERR(50758);
      if ( nnn->ea != BADADDR && !is_mapped(nnn->ea) )
        INTERR(50759);
      if ( nnn->opnum > UA_MAXOP )
        INTERR(50760);
      if ( size < sizeof(uint64) && (nnn->value & ~(left_shift(uint64(1), size*8)-1)) != 0 )
        INTERR(50761);
      break;
    case mop_S: // local stack variable                     LOW
      if ( mv.mba != NULL && s->mba != mv.mba )
        INTERR(50762);
      if ( s->off < 0 )
        INTERR(50763);
      // this check is too strong, therefore it is turned off.
      // example: stkvar.8 gets propagated into another instruction as stkvar.4.
      // the initial stkvar.8 was good because its end was above minstkref
      // but stkvar.4 is not because its end is below minstkref.
      // ideally we should convert it into virtreg (or get rid of virtregs
      // completely, as was planned ages ago)
      /*
      if ( s->off < mv.mba->minstkref
        && mv.curins->opcode != m_cfadd
        && mv.curins->opcode != m_ofadd )
      {
        sval_t end = s->off + (size == NOSIZE ? 0 : size);
        if ( end <= mv.mba->minstkref )
          INTERR(51228);
      }*/
      break;
    case mop_r: // register                                 LOW
      if ( r < 0 )
        INTERR(50764);
      if ( is_bit_reg() && size != 1 && mv.curins->opcode != m_ext )
        INTERR(50765);
      if ( size <= 0 )
        INTERR(50766);
      break;
    case mop_d: // result of another instruction
      {
        if ( d == NULL )
          INTERR(50767);
        if ( size != d->d.size )
          INTERR(50768);
        micro_verifier_t mv2 = mv;
        mv2.curins = d;
        d->verify(mv2, false);
      }
      break;
    case mop_v: // global variable
//      if ( !is_mapped(g) )
//        INTERR(50769);
      break;
    case mop_b: // micro basic block (mblock_t)
      if ( !mv.mba->is_pattern() && (b < 0 || b >= mv.mba->qty) )
        INTERR(50770);
      if ( (flags & VMOP_MOPB) == 0 )
        INTERR(51650);
      break;
    case mop_f: // list of arguments
      if ( f == NULL )
        INTERR(50771);
      f->verify(mv, size);
      // valid only as the 'd' operand of a call
      if ( &mv.curins->d != this )
        INTERR(50772);
      if ( !is_mcode_call(mv.curins->opcode) )
        INTERR(50773);
      break;
    case mop_l: // local variable
      if ( l->mba == NULL )
        INTERR(50774);
      if ( mv.mba != NULL )
      {
        if ( l->mba != mv.mba )
          INTERR(50775);
        const lvars_t &lvs = mv.mba->vars;
        if ( l->idx >= lvs.size() )
          INTERR(50776);
        if ( !mv.mba->lvar_alloc_failed() )
        {
          const lvar_t &v = lvs[l->idx];
          if ( !mv.mba->is_pattern()
            && !mv.mba->is_stkarg(v)
            && !v.has_user_type()
            && !v.is_mapdst_var() ) // relax the check for mapdsts because map destination may eventually shrink
          {
            if ( size == NOSIZE )
            { // allow address references past end of item: &buf[sizeof(buf)]
              if ( v.width < l->off )
                INTERR(50777);
            }
            else
            { // allow references only in the middle of the item
              if ( v.width <= l->off )
                INTERR(50778);
            }
          }
          if ( l->off < 0 )
            INTERR(50779);
        }
      }
      break;
    case mop_a: // address of variable (mop_l, mop_v, mop_S, mop_r)
      if ( a == NULL )
        INTERR(51067);
      if ( a->t != mop_l && a->t != mop_v && a->t != mop_S )
      {
        // addresses of registers are allowed in helper functions
        if ( a->t != mop_r || mv.curins->l.t != mop_h )
          INTERR(50780);
      }
      if ( size > SPWIDTH )
        INTERR(50781);
      a->verify(mv, VMOP_ADRUSED);
      break;
    case mop_h: // helper function
      if ( helper == NULL || helper[0] == 0 )
        INTERR(50782);
      if ( mv.curins->opcode != m_call )
        INTERR(50784);
      break;
    case mop_str:
      if ( cstr == NULL )
        INTERR(50785);
      break;
    case mop_c: // cases
      if ( (flags & VMOP_MOPC) == 0 )
        INTERR(51651);
      if ( c == NULL )
        INTERR(50786);
      c->verify(mv);
      break;
    case mop_fn:
      if ( fpc == NULL )
        INTERR(50787);
      if ( uint(fpc->nbytes) > 16 )
        INTERR(50788);
      break;
    case mop_p:
      if ( pair == NULL )
        INTERR(50789);
      if ( pair->lop.size != pair->hop.size )
        INTERR(50790);
      if ( size != pair->lop.size+pair->hop.size )
        INTERR(50791);
      if ( !valid_pair_part(pair->lop.t) )
        INTERR(50792);
      if ( !valid_pair_part(pair->hop.t) )
        INTERR(50793);
      pair->lop.verify(mv, 0);
      pair->hop.verify(mv, 0);
      break;
    case mop_sc:
      {
        // only scattered vdlocs are allowed, other vdlocs must be
        // represented by other operand types
        if ( !scif->is_scattered() )
          INTERR(51135);
        ushort last = 0;
        const scattered_aloc_t &scvl = scif->scattered();
        scattered_aloc_t::const_iterator p = scvl.begin();
        scattered_aloc_t::const_iterator pend = scvl.end();
        while ( p != pend )
        {
          if ( p->off < last )
            INTERR(51136);
          if ( ushort(p->off+p->size) < p->off )
            INTERR(51137);
          last = p->off + p->size;
          // only reg/stack locations are currently allowed
          if ( !p->is_stkoff() && !p->is_reg1() )
            INTERR(51138);
          ++p;
        }
      }
      break;
    default:
      INTERR(50794);
  }
}

//-------------------------------------------------------------------------
static bool is_valid_m_ext_op(const mop_t &m)
{
  switch ( m.t )
  {
    case mop_b:
    case mop_f:
      return false;
    case mop_d:
      return !m.d->has_side_effects();
  }
  return true;
}

//-------------------------------------------------------------------------
void minsn_t::verify(micro_verifier_t &mv, bool with_target) const
{
  if ( !mv.mba->is_pattern() )
  {
    if ( ea == BADADDR )
      INTERR(50795);
    if ( !mv.mba->range_contains(ea) )
      INTERR(50863);
  }
  // check insn list
  if ( next != NULL && next->prev != this )
    INTERR(50797);
  if ( prev != NULL && prev->next != this )
    INTERR(50798);
  if ( !with_target && (prev != NULL || next != NULL) )
    INTERR(50799);

  bool hasd = d.t != mop_z;
  int lf = 0;
  int rf = 0;
  int df = with_target ? 0 : VMOP_PROPDST;

  if ( !with_target && !is_mcode_propagatable(opcode) )
    INTERR(50800);

  // check fpinsn flag
  switch ( opcode )
  {
    case m_ext:
    case m_ldx:
    case m_stx:
    case m_mov:
    case m_setnz:
    case m_setz:
    case m_setae:
    case m_setb:
    case m_seta:
    case m_setbe:
    case m_setp:
    case m_jnz:
    case m_jz:
    case m_jae:
    case m_jbe:
    case m_jb:
    case m_ja:          // may or may not be fpinsn
      break;
    default:
      if ( is_mcode_fpu(opcode) != is_fpinsn() )
        INTERR(50801);
      break;
  }

  switch ( opcode )
  {
    // these insn can not be propagated
    case m_goto:
    case m_nop:
    case m_ext:
    case m_push:
    case m_ijmp:
    case m_stx:
    case m_und:
    case m_pop:
    case m_jcnd:
    case m_jnz:
    case m_jz:
    case m_jae:
    case m_jb:
    case m_ja:
    case m_jbe:
    case m_jg:
    case m_jge:
    case m_jl:
    case m_jle:
    case m_jtbl:
    case m_ret:
      if ( !with_target )
        INTERR(50802);
      break;
    // these insns may be propagated
    case m_add:
    case m_sub:
    case m_mul:
    case m_udiv:
    case m_sdiv:
    case m_umod:
    case m_smod:
    case m_or:
    case m_and:
    case m_xor:
    case m_shl:
    case m_shr:
    case m_sar:
    case m_cfadd:
    case m_ofadd:
    case m_cfshl:
    case m_cfshr:
    case m_ldc:
    case m_neg:
    case m_xds:
    case m_xdu:
    case m_low:
    case m_high:
    case m_setz:
    case m_setp:
    case m_setnz:
    case m_sets:
    case m_lnot:
    case m_bnot:
    case m_setae:
    case m_setb:
    case m_seta:
    case m_setbe:
    case m_setg:
    case m_setge:
    case m_setl:
    case m_setle:
    case m_seto:
    case m_f2i:
    case m_f2u:
    case m_i2f:
    case m_u2f:
    case m_f2f:
    case m_fneg:
    case m_fadd:
    case m_fsub:
    case m_fmul:
    case m_fdiv:
      if ( hasd != with_target )
        INTERR(50803);
      break;
      // ldx and mov instructions without the target are allowed even at the top level
      // such mov instructions are deleted by eliminate_dead_regs()
      // ldx instructions may survided up to m2c and generate useless memory reads.
      // well, they are useful because they show that there was a memory access
      // in the input binary code.
    case m_ldx:
    case m_mov:
      // even if the target register is not present, the size must be present
      if ( d.t == mop_z )
        df |= VMOP_PROPDST;
      break;
    case m_call:
    case m_icall:
      break;
    default:
      INTERR(50804);
  }

  // propagated insns must not have prev or next fields
  if ( !with_target && (next != NULL || prev != NULL) )
    INTERR(50805);

  // check operand presence
  switch ( opcode )
  {
    case m_nop:
    case m_ret:
      if ( l.t != mop_z
        || r.t != mop_z
        || d.t != mop_z )
      {
        INTERR(50806);
      }
      break;
    case m_ext:
      lf |= VMOP_ANYSIZE;
      rf |= VMOP_ANYSIZE;
      df |= VMOP_ANYSIZE;
      if ( !is_valid_m_ext_op(l) )
        INTERR(50807);
      if ( !is_valid_m_ext_op(r) )
        INTERR(50808);
      if ( !is_valid_m_ext_op(d) )
        INTERR(50809);
      if ( !all_subinsns_are_at(ea) )
        INTERR(50810); // all subinsns must have the same address
      break;
    case m_push:
      if ( l.t == mop_z
        || r.t != mop_z
        || d.t != mop_z )
      {
        INTERR(50811);
      }
      break;
    case m_goto:
      if ( l.t != mop_b && l.t != mop_v
        || r.t != mop_z
        || d.t != mop_z )
      {
        INTERR(50812);
      }
      lf = VMOP_ADRUSED | VMOP_MOPB;
      break;
    case m_ijmp:
      if ( l.t != mop_z
        || r.t == mop_z
        || d.t == mop_z )
      {
        INTERR(50813);
      }
      break;
    case m_stx:
    case m_ldx:
    case m_add:
    case m_sub:
    case m_mul:
    case m_udiv:
    case m_sdiv:
    case m_umod:
    case m_smod:
    case m_or:
    case m_and:
    case m_xor:
    case m_shl:
    case m_shr:
    case m_sar:
    case m_cfadd:
    case m_ofadd:
    case m_cfshl:
    case m_cfshr:
    case m_setp:
    case m_setz:
    case m_setnz:
    case m_setae:
    case m_setb:
    case m_seta:
    case m_setbe:
    case m_setg:
    case m_setge:
    case m_setl:
    case m_setle:
    case m_seto:
    case m_fadd:
    case m_fsub:
    case m_fmul:
    case m_fdiv:
      if ( l.t == mop_z || r.t == mop_z )
        INTERR(50815);
      break;
    case m_low:
    case m_high:
    case m_ldc:
    case m_mov:
    case m_neg:
    case m_xds:
    case m_xdu:
    case m_sets:
    case m_lnot:
    case m_bnot:
    case m_f2i:
    case m_f2u:
    case m_i2f:
    case m_u2f:
    case m_f2f:
    case m_fneg:
      if ( l.t == mop_z || r.t != mop_z )
        INTERR(50817);
      break;
    case m_und:
    case m_pop:
      if ( l.t != mop_z
        || r.t != mop_z
        || d.t == mop_z )
      {
        INTERR(50818);
      }
      break;
    case m_jcnd:
      if ( l.t == mop_z
        || r.t != mop_z
        || d.t != mop_v && d.t != mop_b )
      {
        INTERR(50819);
      }
      df = VMOP_ADRUSED | VMOP_MOPB;
      break;
    case m_jnz:
    case m_jz:
    case m_jae:
    case m_jb:
    case m_ja:
    case m_jbe:
    case m_jg:
    case m_jge:
    case m_jl:
    case m_jle:
      if ( l.t == mop_z
        || r.t == mop_z
        || d.t != mop_v && d.t != mop_b )
      {
        INTERR(50820);
      }
      df = VMOP_ADRUSED | VMOP_MOPB;
      break;
    case m_jtbl:
      if ( l.t == mop_z
        || r.t != mop_c
        || d.t != mop_z )
      {
        INTERR(50821);
      }
      rf |= VMOP_MOPC;
      break;
    case m_call:
      if ( r.t != mop_z )
        INTERR(50822);
      lf = VMOP_ADRUSED;
      // no break
    case m_icall:
      switch ( l.t )
      {
        case mop_z:
        case mop_b:
        case mop_f:
        case mop_a:
          INTERR(50823);
      }
      if ( mv.blk != NULL && (mv.blk->flags & MBL_CALL) != 0 && d.t != mop_f )
        INTERR(50824);
      // each call must have a unique address. we need this to avoid confusion
      // and interrs during type derivation
      if ( mv.seen_calls != NULL
        && l.t != mop_h // ignore helpers
        && !mv.mba->is_pattern() )
      {
        if ( !mv.seen_calls->add_unique(ea) )
          INTERR(51264);
      }
      break;
  }

  // ok, now check the operand sizes
  switch ( opcode )
  {
    case m_stx:
    case m_ijmp:
      if ( d.size != 2 && d.size != SPWIDTH )
        INTERR(50826);
      if ( r.size != 2 )
        INTERR(50827);
      break;
    case m_ldx:
      if ( r.size != 2 && r.size != SPWIDTH )
        INTERR(50828);
      if ( l.size != 2 )
        INTERR(50829);
      break;
    case m_add:
    case m_sub:
    case m_mul:
    case m_udiv:
    case m_sdiv:
    case m_umod:
    case m_smod:
    case m_or:
    case m_and:
    case m_xor:
    case m_fadd:
    case m_fsub:
    case m_fmul:
    case m_fdiv:
      if ( r.size != d.size )
        INTERR(50830);
      // no break
    case m_jnz:
    case m_jz:
    case m_jae:
    case m_jb:
    case m_ja:
    case m_jbe:
    case m_jg:
    case m_jge:
    case m_jl:
    case m_jle:
      if ( l.size != r.size )
        INTERR(50831);
      break;
    case m_cfadd:
    case m_ofadd:
    case m_setp:
    case m_setz:
    case m_setnz:
    case m_setae:
    case m_setb:
    case m_seta:
    case m_setbe:
    case m_setg:
    case m_setge:
    case m_setl:
    case m_setle:
    case m_seto:
      if ( l.size != r.size )
        INTERR(50832);
      // no break
    case m_sets:
      if ( d.size != 1 )
        INTERR(50833);
      break;
    case m_cfshl:
    case m_cfshr:
      if ( r.size != 1 || d.size != 1 )
        INTERR(50834);
      break;
    case m_shl:
    case m_shr:
    case m_sar:
      if ( r.size != 1 )
        INTERR(50835);
      // no break
    case m_ldc:
    case m_mov:
    case m_neg:
    case m_lnot:
    case m_bnot:
    case m_fneg:
      if ( l.size != d.size )
        INTERR(50836);
      break;
    case m_xds:
    case m_xdu:
      if ( l.size >= d.size )
        INTERR(50837);
      break;
    case m_low:
    case m_high:
      if ( l.size <= d.size )
        INTERR(50838);
      break;
  }

  if ( opcode != m_ijmp && opcode != m_stx && opcode != m_ext )
  {
    switch ( d.t )
    {
      case mop_d: // d can not be another insn
        INTERR(50839);
      case mop_a:
      case mop_n:
      case mop_fn:
      case mop_str:
      case mop_h:
        INTERR(51652);
    }
  }

  // check fpinsn operand sizes
  if ( is_fpinsn() )
  {
    if ( is_l_fpval() )
      lf |= VMOP_FPVAL;
    if ( is_r_fpval() )
      rf |= VMOP_FPVAL;
    if ( is_d_fpval() )
      df |= VMOP_FPVAL;
  }

  // check each operand
  mv.curins = CONST_CAST(minsn_t*)(this);
  l.verify(mv, lf);
  r.verify(mv, rf);
  d.verify(mv, df);
}

//-------------------------------------------------------------------------
void mblock_t::verify(eavec_t *seen_calls) const
{
  if ( nextb != NULL && nextb->prevb != this )
    INTERR(50840);
  if ( prevb != NULL && prevb->nextb != this )
    INTERR(50841);

  if ( (nextb == NULL) != (mba->qty-1 == serial) )
    INTERR(50842);

  if ( (prevb == NULL) != (serial == 0) )
    INTERR(50843);

  int all = MBL_PRIV|MBL_FAKE|MBL_GOTO|MBL_TCAL|MBL_PUSH|MBL_DMT64|MBL_COMB
          | MBL_PROP|MBL_DEAD|MBL_LIST|MBL_INCONST|MBL_CALL|MBL_BACKPROP
          | MBL_NORET|MBL_DSLOT|MBL_VALRANGES;
  if ( flags & ~all )
    INTERR(50844);
  if ( !needs_propagation() && lists_dirty() && (flags & MBL_INCONST) == 0 )
    INTERR(50845);

  if ( !mustbuse.is_subset_of(maybuse) )
    INTERR(50846);
  if ( !mustbdef.is_subset_of(maybdef) )
    INTERR(50847);

  if ( serial == 0 || type == BLT_STOP || type == BLT_XTRN )
  {
    if ( head != NULL && !mba->is_pattern() )
      INTERR(51814); // must be empty
    if ( !mustbuse.empty() || !mustbdef.empty() )
      INTERR(50848);
    if ( serial != 0 && !maybdef.empty() )
      INTERR(50850);
  }
  if ( serial == 0 && !maybuse.empty() )
    INTERR(50849);

  if ( serial >= mba->qty )
    INTERR(50851);
  if ( mba->natural[serial] != this )
    INTERR(50852);

  if ( minbstkref != 0 && mba->minstkref > minbstkref )
    INTERR(50853);

  if ( type != BLT_NONE )
  {
    int ns;
    switch ( type )
    {
      //case BLT_NONE: // unknown block type
      //  break;
      case BLT_STOP: // stops execution
        ns = 0;
        // stop block may not have dirty lists after building calls
        // its use-list is calculated by refine_return_type and must not
        // be destroyed
        if ( lists_dirty() && mba->callinfo_built() )
          INTERR(51328);
        break;
      case BLT_XTRN: // external block
      case BLT_0WAY: // does not have successors
        ns = 0;
        break;
      case BLT_1WAY: // passes execution to one block
        ns = 1;
        // passes execution to another function?
        if ( is_call_block() )
        {
          if ( tail->is_noret_call() ) // -V595 tail is used before verifying against NULL
            INTERR(51774);    // should be BLT_0WAY
          if ( nsucc() == 0 || succ(0) != serial+1 )
            INTERR(50854);
        }
        break;
      case BLT_2WAY: // passes execution to two blocks
        ns = 2;
        break;
      case BLT_NWAY: // passes execution to many blocks
        ns = nsucc();
        break;
      default:
        INTERR(51815);
    }
    // nway blocks can be used only with jtbl instructions
    // jtbl instructions always imply BLT_NWAY
    if ( (type == BLT_NWAY) != (tail != NULL && tail->opcode == m_jtbl) )
      INTERR(50855);

    if ( nsucc() != ns )
      INTERR(50856);
    for ( int i=0; i < ns; i++ )
    {
      int n = succ(i);
      if ( n < 0 || n >= mba->qty )
        INTERR(50857);
      if ( !mba->natural[n]->predset.has(serial) )
        INTERR(50858);
    }

    // check that the successor list is correct
    intvec_t outs;
    switch ( tail == NULL ? m_nop : tail->opcode )
    {
      case m_jtbl:
        if ( tail->r.t != mop_c )
          INTERR(50859);
        outs = tail->r.c->targets;
        break;
      case m_goto:
        if ( tail->l.t == mop_b )
          outs.add(tail->l.b);
        break;
      case m_jcnd:
      case m_jnz:
      case m_jz:
      case m_jae:
      case m_jb:
      case m_ja:
      case m_jbe:
      case m_jg:
      case m_jge:
      case m_jl:
      case m_jle:
        outs.add(serial+1);
        if ( tail->d.t == mop_b )
          outs.add_unique(tail->d.b);
        break;
      default:
        if ( ns != 0 )
          outs.add(serial+1);
        break;
      case m_ijmp:
      case m_ret:
        break;
      case m_ext:
        // we can not verify m_ext insns because of ignored insns
        outs = succset;
        break;
    }
    if ( outs != succset )
      INTERR(50860);
  }

  // check that predecessors have us in their succset's
  for ( int i=0; i < npred(); i++ )
  {
    int p = pred(i);
    if ( !mba->natural[p]->succset.has(serial) )
      INTERR(50861);
  }

  // check that predecessors are unique
  {
    intset_t pr;
    for ( int i=0; i < npred(); i++ )
    {
      int p = pred(i);
      if ( !pr.insert(p).second )
        INTERR(50862);
    }
  }

  bool found_tail = false;
  micro_verifier_t mv;
  mv.mba = mba;
  mv.blk = CONST_CAST(mblock_t *)(this);
  mv.seen_calls = seen_calls;
  for ( minsn_t *i=head; i != NULL; i=i->next )
  {
    mv.topins = i;
    mv.curins = i;
    i->verify(mv, true);
    if ( i == tail )
      found_tail = true;
    else if ( must_mcode_close_block(i->opcode, false) )
      INTERR(50864);
    if ( (flags & MBL_PUSH) == 0
      && (i->opcode == m_push || i->opcode == m_pop) )
    {
      INTERR(50865);
    }
  }

  if ( !empty() )
  {
    if ( !found_tail )
      INTERR(50866);
    if ( head->prev != NULL )
      INTERR(50867);
    if ( tail->next != NULL )
      INTERR(50868);
    if ( (mba->get_mba_flags() & MBA_NOFUNC) == 0 )
    {
      if ( start >= end && (flags & MBL_FAKE) == 0 )
        INTERR(50869);
      if ( end != BADADDR
        && (mba->get_mba_flags() & MBA_CMBBLK) != 0
        && !test_bit(mba->occurred_warns, WARN_FIXED_MACRO) )
      {
        if ( !mba->range_contains(end-1) )
          INTERR(50870);
      }
    }
  }
  else
  {
    if ( head != NULL )
      INTERR(50871);
    if ( tail != NULL )
      INTERR(50872);
  }

  if ( lists_ready()
    && !mba->lvars_allocated()
    && !mba->deleted_pairs()     // delete_dest_pairs() may introduce kernel regs that are not taken into account in use/def lists
    && serial != 0
    && type != BLT_STOP
    && type != BLT_XTRN )
  {
    mlist_t test_maybuse;
    mlist_t test_maybdef;
    mlist_t test_mustbuse;
    mlist_t test_mustbdef;
    mlist_t test_dnu;
    for ( minsn_t *m=head; m != NULL; m=m->next )
    {
      if ( !m->is_assert() )
      {
        mlist_t ui1 = build_use_list(*m, MAY_ACCESS);
        if ( !ui1.empty() )
        {
          test_dnu.sub(ui1);
          ui1.sub(extract_restricted_list(mba, test_maybdef));
          test_maybuse.add(ui1);
          mlist_t ui2 = build_use_list(*m, MUST_ACCESS);
          ui2.sub(test_maybdef);
          test_mustbuse.add(ui2);
        }
        mlist_t di1 = build_def_list(*m, MAY_ACCESS);
        if ( !di1.empty() )
        {
          // fixme: spoiled registers are not really defined by the block
          //        introduce 'spoiled' list
          test_maybdef.add(di1);
          mlist_t di2 = build_def_list(*m, MUST_ACCESS);
          test_mustbdef.add(di2);
          test_dnu.add(extract_restricted_list(mba, di2));
        }
      }
    }
    const mlist_t &temp = mvm.get_temp_regs();
    test_mustbdef.sub(temp);
    test_maybdef.sub(temp);
    test_dnu.sub(extract_restricted_list(mba, temp));
    if ( test_maybuse != maybuse )
      INTERR(50873);
    if ( test_maybdef != maybdef )
      INTERR(50874);
    if ( test_mustbuse != mustbuse )
      INTERR(50875);
    if ( test_mustbdef != mustbdef )
      INTERR(50876);
    if ( test_dnu != dnu )
      INTERR(50877);
  }

  // temporary registers can not cross block boundaries
  const mlist_t &tmp = mvm.get_temp_regs();
  if ( maybuse.has_common(tmp) )
    INTERR(50920);
}

//-------------------------------------------------------------------------
// verify input arguments
void mbl_array_t::verify_args(void) const
{
  mlist_t used;
  stkloc_verifier_t stk_verifier;
  for ( int i=0; i < argidx.size(); i++ )
  {
    const lvar_t &v = vars[argidx[i]];
    if ( !v.is_arg_var() )
      INTERR(50906);

    if ( lvar_alloc_failed() )
      continue;

    mlist_t vlst;
    v.append_list(&vlst);
    if ( vlst.has_common(used) )
      INTERR(50904);
    used.add(vlst);

    if ( is_user_cc(cc) && nargs() < MAX_FUNC_ARGS )
    {
      if ( !stk_verifier.validate_next_argloc(v.location, v.type(), v.width) )
        INTERR(51053);
    }
  }
  if ( is_cdtr() )
  { // the first argument must be 'this'
    if ( nargs() == 0 )
      INTERR(51871);
    const lvar_t &thisarg = arg(0);
    if ( thisarg.name != "this" )
      INTERR(51872);
    if ( !thisarg.type().is_ptr() )
      INTERR(51873);
    if ( !thisarg.is_thisarg() )
      INTERR(51887);
  }
}

//-------------------------------------------------------------------------
static void verify_lvar_names(const strings_t &s1, const strings_t &s2)
{
//  for ( auto &p : s1 )
//    msg("s1 %s\n", p.c_str());
//  for ( auto &p : s2 )
//    msg("s2 %s\n", p.c_str());

  strings_t::iterator p = s1.begin();
  strings_t::iterator q = s2.begin();
  while ( true )
  {
    if ( p == s1.end() )
    {
      if ( q != s2.end() )
        INTERR(51502);
      break;
    }
    if ( q == s2.end() )
      INTERR(51503);
    if ( *p != *q )
      INTERR(51504);
    ++p;
    ++q;
  }
}

//-------------------------------------------------------------------------
void mbl_array_t::verify_lvars(void) const
{
  strings_t names;
  std::set<lvar_locator_t> seen;
  for ( int i=0; i < vars.size(); i++ )
  {
    const lvar_t &v = vars[i];
    if ( v.name.empty() )
      QASSERT(50891, lvars_renamed()); // empty variable names are permitted only at the final stage
    else
      names.insert(v.name);
    if ( v.type().empty() )
      INTERR(50892);
    if ( !v.type().is_correct() )
      INTERR(50893);                             // incorrect type
    if ( v.defblk != 0 && v.defea == BADADDR && !v.is_fake_var() && !v.is_overlapped_var() )
      INTERR(50894);
    if ( v.defblk < 0 || v.defblk >= qty && qty > 0 )
      INTERR(50895);
    if ( v.is_arg_var() )
    {
      if ( v.defblk != 0 )
        INTERR(50896);
      if ( lvars_allocated() )
      { // check argidx only when lvars are allocated
        if ( !argidx.has(i) )
          INTERR(50897);
      }
    }
    else
    {
      if ( v.is_thisarg() )
        INTERR(51888);
    }
    if ( v.width <= 0 )
      INTERR(51297);
    if ( v.type().get_size() != v.width )
    {
      if ( !v.is_unpadded() )
        INTERR(50898);
      if ( v.width != v.type().get_unpadded_size() )
        INTERR(51926);
    }
    if ( v.location.is_badloc() )
      INTERR(51219);
    if ( retvaridx == i )
    {
      if ( v.location.calc_stack_range(NULL, NULL, v.width) )
        INTERR(50899); // can not have any stack part
      if ( !v.is_result_var() )
        INTERR(50900);
    }
    else
    {
      if ( v.is_result_var() )
        INTERR(50901);
    }
    if ( precise_defeas() && !lvar_alloc_failed() && !seen.insert(v).second )
      INTERR(50902); // two indistinguishable variables (the same location and defea)
  }
  if ( lvar_names_ok() )
    verify_lvar_names(lvar_names, names);
}

//-------------------------------------------------------------------------
void mbl_array_t::verify(bool always) const
{
  if ( !always && !should_verify() )
    return;
  DECLARE_HIT_COUNTER("verify");

  bool real_code = !is_pattern() && (flags & MBA_LOADED) == 0;
  int cnt = 0;
  ivlset_t fbody;
  eavec_t seen_calls;
  for ( mblock_t *b=blocks; b != NULL; b=b->nextb )
  {
    b->verify(&seen_calls);
    if ( natural[cnt] != b )
      INTERR(50878);
//msg("%d: %a..%a\n", cnt, b->start, b->end);
    if ( real_code && (b->flags & MBL_FAKE) == 0 )
    {
      ivl_t bbody(b->start, b->end-b->start);
      if ( fbody.has_common(bbody) )
        INTERR(50879);
      fbody.add(bbody);
    }
    cnt++;
  }
  if ( cnt != qty )
    INTERR(50880);

  if ( flags & 0x80000000 )
    INTERR(51685);
  if ( flags2 & ~MBA2_ALL_FLAGS )
    INTERR(50881);

  //mbl_graph_t *bg;              // graph of basic blocks
  //prolog_info_t *pi;            // prolog information

  if ( fullsize < 0 || stacksize < 0 || minstkref < 0 )
    INTERR(50882);
  if ( fullsize < minargref )
    INTERR(50883);
  if ( real_code )
  {
    if ( fullsize < stacksize )
      INTERR(50884);
    if ( minstkref > stacksize )
      INTERR(50885);
    if ( frsize+frregs > stacksize )
      INTERR(50886);
    func_t *pfn = get_curfunc();
    if ( pfn != NULL )
    {
      if ( pfn->frsize != frsize )
        INTERR(50887);
      if ( pfn->frregs != frregs )
        INTERR(50888);
      if ( pfn->fpd != fpd )
        INTERR(51704);
      if ( get_frame_retsize(pfn) != retsize )
        INTERR(50889);
    }
    else
    {
      if ( frsize != 0 || frregs != 0 || fpd != 0 || retsize != 0 )
        INTERR(51709);
    }
  }

  if ( !is_pattern() )
    verify_lvars();

  verify_args();

  if ( retvaridx != -1 && (retvaridx < 0 || retvaridx >= vars.size()) )
    INTERR(50911);

  // int npurged;         // -1 - unknown
  if ( cc == 0 )
    INTERR(50912);

  if ( final_type )
  {
    if ( !idb_type.is_correct() )
      INTERR(50913);
  }

  if ( real_code && use_frame() )
  {
    flags_t f = get_flags(entry_ea);
    if ( !is_func(f) )
      INTERR(50914);
  }
}

//-------------------------------------------------------------------------
void pattern_t::verify(bool always) const
{
  mbl_array_t::verify(always);

  for ( labels_t::const_iterator p=labels.begin(); p != labels.end(); ++p )
  {
    const insref_t &ir = p->second;
    if ( ir.nblk < 0 || ir.nblk >= qty )
      INTERR(50958);
    const mblock_t *b = get_mblock(ir.nblk);
    const minsn_t *m = b->get_insn(ir.nins);
    if ( m == NULL )
      INTERR(50959);
  }

  micro_verifier_t mv;
  mv.mba = CONST_CAST(pattern_t*)(this);
  mv.blk = NULL;
  for ( int i=0; i < postactions.size(); i++ )
  {
    mv.topins = mv.curins = CONST_CAST(minsn_t*)(&postactions[i].insn);
    mv.topins->verify(mv, true);
  }

  mv.topins = mv.curins = NULL;
  for ( int i=0; i < conditions.size(); i++ )
    conditions[i].verify(mv, 0);

  for ( int i=0; i < typereqs.size(); i++ )
    typereqs[i].mop.verify(mv, 0);
}

//-------------------------------------------------------------------------
// in order to lvar allocation to create distinguishable lvars,
// all instruction destinations must be different for any given ea.
// unfortunately this requirement is too strong for the moment.
// there are some instructions (adc, for example), that
// update their destinations multiple times. we will have to
// this this in the future. meanwhile, this check is commented out.
void mbl_array_t::verify_dest_eas(void) const
{
/*
  struct ida_local dest_ea_verifier_t : public minsn_visitor_t
  {
    const mlist_t &tempregs;
    std::set<lvar_locator_t> seen;
    int idaapi visit_minsn(void)
    {
      if ( curins->modifes_d() )
      {
        lvar_locator_t ll;
        if ( curins->d.is_reg() )
        {
          ll.location = ARGLOC_REG | curins->d.r;
          if ( tempregs.has(curins->d.r) )
            return 0;
        }
        else if ( curins->d.t == mop_S )
        {
          ll.location = curins->d.s->off;
        }
        else
        {
          return 0;
        }
        ll.defea = curins->ea;
        if ( !seen.insert(ll).second )
        {
          interr_ea = ll.defea;
          INTERR(50919);
        }
      }
      return 0;
    }
    dest_ea_verifier_t(void) : tempregs(mvm.get_temp_regs()) {}
  };
  dest_ea_verifier_t dv;
  CONST_CAST(mbl_array_t*)(this)->for_all_topinsns(dv);
*/
}

//-------------------------------------------------------------------------
void mbl_array_t::verify_stkpnts(bool cumulative_spds, bool good_stacksize) const
{
  size_t n = stkpnts.size();
  for ( size_t i=0; i < n; i++ )
  {
    const stkpnt_t &sp = stkpnts[i];
    if ( cumulative_spds && good_stacksize && sp.spd + stacksize < 0 )
      INTERR(51712);
    if ( i > 0 && sp.ea <= stkpnts[i-1].ea )
      INTERR(51713);
  }
  if ( good_stacksize )
  {
    sval_t sp_entry = getspd(entry_ea);
    QASSERT(51714, is_snippet() ? sp_entry <= stacksize : sp_entry == stacksize);
  }
}
