/*
 *      Hex-Rays Decompiler project
 *      Copyright (c) 2007-2018 by Hex-Rays, support@hex-rays.com
 *      ALL RIGHTS RESERVED.
 *
 *      Sample plugin for Hex-Rays Decompiler.
 *      It installs a custom instruction optimization rule:
 *
 *        mov #N, var.4                  mov #N, var.4
 *        xor var@1.1, #M, var@1.1    => mov #NM, var@1.1
 *                                         where NM == (N>>8)^M
 *
 *      We need this rule because the decompiler can not propagate the second
 *      byte of VAR into the xor instruction.
 *
 *      The XOR opcode can be replaced by any other, we do not rely on it.
 *      Also operand sizes can vary.
 *
 *      This improves the decompiler output for some obfuscated code.
 */

#include <hexrays.hpp>

// Hex-Rays API pointer
hexdsp_t *hexdsp = NULL;

//--------------------------------------------------------------------------
// compare operands but ignore the sizes
static bool equal_mops_ignore_size(const mop_t &lo, const mop_t &ro)
{
  if ( lo.t != ro.t )
    return false;

  switch ( lo.t )
  {
    case mop_z:         // none
      return true;
    case mop_fn:        // floating point
      return *ro.fpc == *lo.fpc;
    case mop_n:         // immediate
      {
        int minsize = qmin(lo.size, ro.size);
        uint64 v1 = extend_sign(ro.nnn->value, minsize, false);
        uint64 v2 = extend_sign(lo.nnn->value, minsize, false);
        return v1 == v2;
      }
    case mop_S:         // stack variable
      return *ro.s == *lo.s;
    case mop_v:         // global variable
      return ro.g == lo.g;
    case mop_d:         // result of another instruction
      break;            // not implemented
    case mop_b:         // micro basic block (mblock_t)
      return ro.b == lo.b;
    case mop_r:         // register
      return ro.r == lo.r;
    case mop_f:
      break;            // not implemented
    case mop_l:
      return *ro.l == *lo.l;
    case mop_a:
      return lo.a->insize == ro.a->insize
          && lo.a->outsize == ro.a->outsize
          && equal_mops_ignore_size(*lo.a, *ro.a);
    case mop_h:
      return streq(ro.helper, lo.helper);
    case mop_str:
      return streq(ro.cstr, lo.cstr);
    case mop_c:
      return *ro.c == *lo.c;
    case mop_p:
      return equal_mops_ignore_size(lo.pair->lop, ro.pair->lop)
          && equal_mops_ignore_size(lo.pair->hop, ro.pair->hop);
    case mop_sc: // not implemented
      break;
  }
  return false;
}

//--------------------------------------------------------------------------
// find backwards the instruction that defines anything from LST
static const minsn_t *find_prev_def(
        const mblock_t *blk,
        const mlist_t &lst,
        const minsn_t *ins)
{
  const minsn_t *p = ins;
  while ( (p=p->prev) != NULL )
  {
    mlist_t def = blk->build_def_list(*p, MAY_ACCESS|FULL_XDSU);
    if ( def.has_common(lst) )
      break;
  }
  return p;
}

//--------------------------------------------------------------------------
struct glbprop_t : public optinsn_t
{
  virtual int idaapi func(mblock_t *blk, minsn_t *ins)
  {
    if ( ins->r.t != mop_n )
      return 0; // we want a constant as the second operand
    if ( ins->r.size > 2 )
      return 0; // bigger sizes are handled by the decompiler without problems

    // build list of data used by INS
    mlist_t use = blk->build_use_list(*ins, MAY_ACCESS);

    // find the instruction that defines anything from USE
    const minsn_t *di = find_prev_def(blk, use, ins);
    if ( di == NULL )
      return 0; // not found

    if ( di->opcode != m_mov || di->l.t != mop_n )
      return 0; // must be 'mov #N, ...'

    // compare the destination of DI and the left operand of INS
    mop_t v1 = ins->l;
    const mop_t &v2 = di->d;
    if ( v1.t != v2.t )
      return 0; // operand types are different

    // if operand sizes are the same, hexrays can handle it without our help
    // if the size of INS->L is bigger than the size of DI->D, may not propagate
    // we handle only the case where the size of INS->L is less than the size
    // of DI->D because the hexrays sometimes has problems with it.
    if ( v1.size >= v2.size )
      return 0;

    // this is not very efficient... but acceptable
    int off = 0;
    while ( !equal_mops_ignore_size(v1, v2) )
    {
      if ( ++off >= v2.size )
        return 0;
      if ( !v1.shift_mop(-1) )
        return 0;
    }

    // found a match! shift N in order to propagate the correct part of it
    // we don't truncate the high bits, it will happen in make_number()
    uint64 N = di->l.value(false);
    N >>= (off * 8);

    // store the new value in INS
    ins->l.make_number(N, ins->l.size, di->l.nnn->ea, di->l.nnn->opnum);

    // optimize the instruction, it is highly likely that we will get
    // a much simpler instruction like 'mov'
    ins->optimize_solo();

    return 1; // success, we made one change
  }
};
static glbprop_t glbprop;

//--------------------------------------------------------------------------
int idaapi init(void)
{
  if ( !init_hexrays_plugin() )
    return PLUGIN_SKIP; // no decompiler
  const char *hxver = get_hexrays_version();
  msg("Hex-rays version %s has been detected, %s ready to use\n", hxver, PLUGIN.wanted_name);
  install_optinsn_handler(&glbprop);
  return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
  if ( hexdsp != NULL )
  {
    remove_optinsn_handler(&glbprop);
    term_hexrays_plugin();
  }
}

//--------------------------------------------------------------------------
bool idaapi run(size_t)
{
  warning("The '%s' plugin is fully automatic", PLUGIN.wanted_name);
  return false;
}

//--------------------------------------------------------------------------
static const char comment[] = "Sample16 plugin for Hex-Rays decompiler";

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION,
  PLUGIN_HIDE,          // plugin flags
  init,                 // initialize
  term,                 // terminate. this pointer may be NULL.
  run,                  // invoke plugin
  comment,              // long comment about the plugin
                        // it could appear in the status line
                        // or as a hint
  "",                   // multiline help about the plugin
  "Propagation helper", // the preferred short name of the plugin
  ""                    // the preferred hotkey to run the plugin
};
