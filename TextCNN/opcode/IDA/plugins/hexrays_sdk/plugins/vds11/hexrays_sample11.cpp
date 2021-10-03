/*
 *      Hex-Rays Decompiler project
 *      Copyright (c) 2007-2018 by Hex-Rays, support@hex-rays.com
 *      ALL RIGHTS RESERVED.
 *
 *      Sample plugin for Hex-Rays Decompiler.
 *      It installs a custom block optimization rule:
 *
 *        goto L1     =>        goto L2
 *        ...
 *      L1:
 *        goto L2
 *
 *      In other words we fix a goto target if it points to a chain of gotos.
 *      This improves the decompiler output in some cases.
 */

#include <hexrays.hpp>

// Hex-Rays API pointer
hexdsp_t *hexdsp = NULL;

//--------------------------------------------------------------------------
struct goto_optimizer_t : public optblock_t
{
  virtual int idaapi func(mblock_t *blk)
  {
    if ( handle_goto_chain(blk) )
      return 1;
    return 0;
  }
  //lint -e{818} ins could be made const
  bool handle_goto_chain(mblock_t *blk) const
  {
    minsn_t *mgoto = blk->tail;
    if ( mgoto == NULL || mgoto->opcode != m_goto )
      return false;

    intvec_t visited;
    int t0 = mgoto->l.b;
    int i = t0;
    mbl_array_t *mba = blk->mba;

    // follow the goto chain
    while ( true )
    {
      if ( !visited.add_unique(i) )
        return false; // an endless loop, prefer to keep things as is
      mblock_t *b = mba->get_mblock(i);
      // skip assertion instructions and find first regular instruction
      minsn_t *m2 = getf_reginsn(b->head);
      if ( m2 == NULL || m2->opcode != m_goto )
        break; // not a goto
      i = m2->l.b;
    }
    if ( i == t0 )
      return false; // not a chain

    // all ok, found a goto chain
    mgoto->l.b = i; // jump directly to the end of the chain

    // fix the successor/predecessor lists
    blk->succset[0] = i;
    mba->get_mblock(i)->predset.add(blk->serial);
    mba->get_mblock(t0)->predset.del(blk->serial);

    // since we changed the control flow graph, invalidate the use/def chains.
    // stricly speaking it is not really necessary in our plugin because
    // we did not move around any microcode operands.
    mba->mark_chains_dirty();

    // it is a good idea to verify microcode after each change
    // however, it may be time consuming, so comment it out eventually
    mba->verify(true);
    return true;
  }
};
static goto_optimizer_t goto_optimizer;

//--------------------------------------------------------------------------
int idaapi init(void)
{
  if ( !init_hexrays_plugin() )
    return PLUGIN_SKIP; // no decompiler
  const char *hxver = get_hexrays_version();
  msg("Hex-rays version %s has been detected, %s ready to use\n", hxver, PLUGIN.wanted_name);
  install_optblock_handler(&goto_optimizer);
  return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
  if ( hexdsp != NULL )
  {
    remove_optblock_handler(&goto_optimizer);
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
static const char comment[] = "Sample11 plugin for Hex-Rays decompiler";

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
  "Optimize goto chains", // the preferred short name of the plugin
  ""                    // the preferred hotkey to run the plugin
};
