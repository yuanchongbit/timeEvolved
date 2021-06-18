/*
 *      Hex-Rays Decompiler project
 *      Copyright (c) 2007-2018 by Hex-Rays, support@hex-rays.com
 *      ALL RIGHTS RESERVED.
 *
 *      Sample plugin for Hex-Rays Decompiler.
 *      It installs a custom microcode optimization rule:
 *        call   !DbgRaiseAssertionFailure <fast:>.0
 *      =>
 *        call   !DbgRaiseAssertionFailure <fast:"char *" "assertion text">.0
 *
 *      To see this plugin in action please use arm64_brk.i64
 */

#include <hexrays.hpp>

// Hex-Rays API pointer
hexdsp_t *hexdsp = NULL;

//--------------------------------------------------------------------------
struct nt_assert_optimizer_t : public optinsn_t
{
  virtual int idaapi func(mblock_t *, minsn_t *ins)
  {
    if ( handle_nt_assert(ins) )
      return 1;
    return 0;
  }
  //lint -e{818} ins could be made const
  bool handle_nt_assert(minsn_t *ins) const
  {
    // recognize call   !DbgRaiseAssertionFailure <fast:>.0
    if ( !ins->is_helper("DbgRaiseAssertionFailure") )
      return false;

    // did we already add an argument?
    mcallinfo_t &fi = *ins->d.f;
    if ( !fi.args.empty() )
      return false;

    // use a comment from the disassembly listing as the call argument
    qstring cmt;
    if ( !get_cmt(&cmt, ins->ea, false) )
      return false;

    // remove "NT_ASSERT(" to make the listing nicer
    if ( strneq(cmt.begin(), "NT_ASSERT(\"", 11) )
      cmt.remove(0, 11);
    if ( cmt.length() > 2 && streq(cmt.begin()+cmt.length()-2, "\")") )
      cmt.remove_last(2);

    // all ok, transform the instruction by adding one more call argument
    mcallarg_t &fa = fi.args.push_back();
    fa.t    = mop_str;
    fa.cstr = cmt.extract();
    fa.type = tinfo_t::get_stock(STI_PCCHAR); // const char *
    fa.size = fa.type.get_size();
    return true;
  }
};
static nt_assert_optimizer_t nt_assert_optimizer;

//--------------------------------------------------------------------------
int idaapi init(void)
{
  if ( !init_hexrays_plugin() )
    return PLUGIN_SKIP; // no decompiler
  const char *hxver = get_hexrays_version();
  msg("Hex-rays version %s has been detected, %s ready to use\n", hxver, PLUGIN.wanted_name);
  install_optinsn_handler(&nt_assert_optimizer);
  return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
  if ( hexdsp != NULL )
  {
    remove_optinsn_handler(&nt_assert_optimizer);
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
static const char comment[] = "Sample10 plugin for Hex-Rays decompiler";

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
  "Optimize DbgRaiseAssertionFailure", // the preferred short name of the plugin
  ""                    // the preferred hotkey to run the plugin
};
