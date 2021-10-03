/*
 *      Hex-Rays Decompiler project
 *      Copyright (c) 2007-2018 by Hex-Rays, support@hex-rays.com
 *      ALL RIGHTS RESERVED.
 *
 *      Sample plugin for Hex-Rays Decompiler.
 *      It generates microcode for selection and dumps it to the output window.
 */

#include <hexrays.hpp>
#include <frame.hpp>

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
  ea_t ea1, ea2;
  if ( !read_range_selection(NULL, &ea1, &ea2) )
  {
    warning("Please select a range of addresses to analyze");
    return true;
  }

  flags_t F = get_flags(ea1);
  if ( !is_code(F) )
  {
    warning("The selected range must start with an instruction");
    return true;
  }

  // generate microcode
  hexrays_failure_t hf;
  mba_ranges_t mbr;
  mbr.ranges.push_back(range_t(ea1, ea2));
  mbl_array_t *mba = gen_microcode(mbr, &hf, NULL, DECOMP_WARNINGS);
  if ( mba == NULL )
  {
    warning("%a: %s", hf.errea, hf.desc().c_str());
    return true;
  }

  msg("Successfully generated microcode for %a..%a\n", ea1, ea2);
  vd_printer_t vp;
  mba->print(vp);

  // We must explicitly delete the microcode array
  delete mba;
  return true;
}

//--------------------------------------------------------------------------
static const char comment[] = "Sample13 plugin for Hex-Rays decompiler";

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
  "Dump microcode for selected range", // the preferred short name of the plugin
  ""                    // the preferred hotkey to run the plugin
};
