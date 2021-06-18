/*
 *      Hex-Rays Decompiler project
 *      Copyright (c) 2007-2018 by Hex-Rays, support@hex-rays.com
 *      ALL RIGHTS RESERVED.
 *
 *      Sample plugin for Hex-Rays Decompiler.
 *      It generates microcode for the current function and prints it
 *      in the output window.
 *
 */

#include <hexrays.hpp>

// Hex-Rays API pointer
hexdsp_t *hexdsp = NULL;

//--------------------------------------------------------------------------
int idaapi init(void)
{
  if ( !init_hexrays_plugin() )
    return PLUGIN_SKIP; // no decompiler
  const char *hxver = get_hexrays_version();
  msg("Hex-rays version %s has been detected, %s ready to use\n", hxver, PLUGIN.wanted_name);
  return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
  if ( hexdsp != NULL )
    term_hexrays_plugin();
}

//--------------------------------------------------------------------------
bool idaapi run(size_t)
{
  func_t *pfn = get_func(get_screen_ea());
  if ( pfn == NULL )
  {
    warning("Please position the cursor within a function");
    return true;
  }

  // Generate microcode. This call returns fully optimized microcode.
  // If desired, we could hook to decompiler events and return MERR_STOP
  // to return microcode from previous analysis stages. Another and easier
  // way of obtaining microcode of earlier stages is to explicitly specify
  // the required maturity level in the gen_mircocode() call.
  hexrays_failure_t hf;
  mbl_array_t *mba = gen_microcode(pfn, &hf, NULL, DECOMP_WARNINGS);
  if ( mba == NULL )
  {
    warning("#error \"%a: %s", hf.errea, hf.desc().c_str());
    return true;
  }
  msg("%a: successfully generated microcode\n", pfn->start_ea);

  // Dump the microcode to the output window
  vd_printer_t vp;
  mba->print(vp);

  // Notes:
  // 1. You may derive your own class based on vd_printer_t and redirect the
  //    output anywhere you want.
  // 2. There is also mblock_t::print() that prints one basic block.
  // 3. There are also mbl_array_t::dump() and mblock_t::dump() functions
  //    that create a file in the directory pointed by IDA_DUMPDIR environment
  //    variable. These function work only under debugger (they are convenient
  //    to use under debugger: dump the current microcode and study it).
  //    The decompiler itself will dump its internal state if run under
  //    debugger, so that all microcode transformations can be tracked.
  // 4. Printing individual instructions with minsn_t::print() and omitting
  //    SHINS_SHORT is supported only while decompiling the function or immediately
  //    after it because minsn_t::print() uses a global variable that points
  //    to the current mbl_array_t. However, printing mblock_t and mbl_array_t
  //    is ok any time from the main thread. Decompiler is not thread-safe
  //    and must be used only from the main thread.

  // We must explicitly delete the microcode array
  delete mba;
  return true;
}

//--------------------------------------------------------------------------
static char comment[] = "Sample9 plugin for Hex-Rays decompiler";

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION,
  0,                    // plugin flags
  init,                 // initialize
  term,                 // terminate. this pointer may be NULL.
  run,                  // invoke plugin
  comment,              // long comment about the plugin
                        // it could appear in the status line
                        // or as a hint
  "",                   // multiline help about the plugin
  "Generate microcode & Print",  // the preferred short name of the plugin
  ""                    // the preferred hotkey to run the plugin
};
