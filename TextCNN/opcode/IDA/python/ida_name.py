# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.




"""
IDA Plugin SDK API wrapper: name
"""


from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ida_name', [dirname(__file__)])
        except ImportError:
            import _ida_name
            return _ida_name
        if fp is not None:
            try:
                _mod = imp.load_module('_ida_name', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ida_name = swig_import_helper()
    del swig_import_helper
else:
    import _ida_name
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except:
    weakref_proxy = lambda x: x


import ida_idaapi

import sys
_BC695 = sys.modules["__main__"].IDAPYTHON_COMPAT_695_API

if _BC695:






    def bc695redef(func):
        ida_idaapi._BC695.replace_fun(func)
        return func

class ea_name_vec_t(object):
    """
    Proxy of C++ qvector<(ea_name_t)> class
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args):
        """
        __init__(self) -> ea_name_vec_t
        __init__(self, x) -> ea_name_vec_t
        """
        this = _ida_name.new_ea_name_vec_t(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ida_name.delete_ea_name_vec_t
    __del__ = lambda self : None;
    def push_back(self, *args):
        """
        push_back(self, x)
        push_back(self) -> ea_name_t
        """
        return _ida_name.ea_name_vec_t_push_back(self, *args)

    def pop_back(self, *args):
        """
        pop_back(self)
        """
        return _ida_name.ea_name_vec_t_pop_back(self, *args)

    def size(self, *args):
        """
        size(self) -> size_t
        """
        return _ida_name.ea_name_vec_t_size(self, *args)

    def empty(self, *args):
        """
        empty(self) -> bool
        """
        return _ida_name.ea_name_vec_t_empty(self, *args)

    def at(self, *args):
        """
        at(self, _idx) -> ea_name_t
        """
        return _ida_name.ea_name_vec_t_at(self, *args)

    def qclear(self, *args):
        """
        qclear(self)
        """
        return _ida_name.ea_name_vec_t_qclear(self, *args)

    def clear(self, *args):
        """
        clear(self)
        """
        return _ida_name.ea_name_vec_t_clear(self, *args)

    def resize(self, *args):
        """
        resize(self, _newsize, x)
        resize(self, _newsize)
        """
        return _ida_name.ea_name_vec_t_resize(self, *args)

    def grow(self, *args):
        """
        grow(self, x=ea_name_t())
        """
        return _ida_name.ea_name_vec_t_grow(self, *args)

    def capacity(self, *args):
        """
        capacity(self) -> size_t
        """
        return _ida_name.ea_name_vec_t_capacity(self, *args)

    def reserve(self, *args):
        """
        reserve(self, cnt)
        """
        return _ida_name.ea_name_vec_t_reserve(self, *args)

    def truncate(self, *args):
        """
        truncate(self)
        """
        return _ida_name.ea_name_vec_t_truncate(self, *args)

    def swap(self, *args):
        """
        swap(self, r)
        """
        return _ida_name.ea_name_vec_t_swap(self, *args)

    def extract(self, *args):
        """
        extract(self) -> ea_name_t
        """
        return _ida_name.ea_name_vec_t_extract(self, *args)

    def inject(self, *args):
        """
        inject(self, s, len)
        """
        return _ida_name.ea_name_vec_t_inject(self, *args)

    def begin(self, *args):
        """
        begin(self) -> ea_name_t
        begin(self) -> ea_name_t
        """
        return _ida_name.ea_name_vec_t_begin(self, *args)

    def end(self, *args):
        """
        end(self) -> ea_name_t
        end(self) -> ea_name_t
        """
        return _ida_name.ea_name_vec_t_end(self, *args)

    def insert(self, *args):
        """
        insert(self, it, x) -> ea_name_t
        """
        return _ida_name.ea_name_vec_t_insert(self, *args)

    def erase(self, *args):
        """
        erase(self, it) -> ea_name_t
        erase(self, first, last) -> ea_name_t
        """
        return _ida_name.ea_name_vec_t_erase(self, *args)

    def __len__(self, *args):
        """
        __len__(self) -> size_t
        """
        return _ida_name.ea_name_vec_t___len__(self, *args)

    def __getitem__(self, *args):
        """
        __getitem__(self, i) -> ea_name_t
        """
        return _ida_name.ea_name_vec_t___getitem__(self, *args)

    def __setitem__(self, *args):
        """
        __setitem__(self, i, v)
        """
        return _ida_name.ea_name_vec_t___setitem__(self, *args)

    front = ida_idaapi._qvector_front
    back = ida_idaapi._qvector_back
    __iter__ = ida_idaapi._bounded_getitem_iterator

ea_name_vec_t_swigregister = _ida_name.ea_name_vec_t_swigregister
ea_name_vec_t_swigregister(ea_name_vec_t)


def get_name(*args):
  """
  get_name(ea) -> qstring
  """
  return _ida_name.get_name(*args)

def get_colored_name(*args):
  """
  get_colored_name(ea) -> qstring
  """
  return _ida_name.get_colored_name(*args)
MAXNAMELEN = _ida_name.MAXNAMELEN
"""
Maximum length of a name in IDA (with the trailing zero)
"""
FUNC_IMPORT_PREFIX = _ida_name.FUNC_IMPORT_PREFIX
"""
Name prefix used by IDA for the imported functions.
"""

def set_name(*args):
  """
  set_name(ea, name, flags=0) -> bool


  Set or delete name of an item at the specified address. An item can be
  anything: instruction, function, data byte, word, string, structure,
  etc... Include name into the list of names.
  
  @param ea: linear address. do nothing if ea is not valid (return 0).
             tail bytes can't have names. (C++: ea_t)
  @param name: new name.   NULL: do nothing (return 0). "" : delete
               name. otherwise this is a new name. (C++: const char *)
  @param flags: Set name flags . If a bit is not specified, then the
                corresponding action is not performed and the name will
                retain the same bits as before calling this function.
                For new names, default is: non-public, non-weak, non-
                auto. (C++: int)
  """
  return _ida_name.set_name(*args)
SN_CHECK = _ida_name.SN_CHECK
SN_NOCHECK = _ida_name.SN_NOCHECK
"""
Don't fail if the name contains invalid characters. If this bit is
clear, all invalid chars (those !is_ident_cp()) will be replaced by
SUBSTCHAR List of valid characters is defined in ida.cfg
"""
SN_PUBLIC = _ida_name.SN_PUBLIC
"""
if set, make name public
"""
SN_NON_PUBLIC = _ida_name.SN_NON_PUBLIC
"""
if set, make name non-public
"""
SN_WEAK = _ida_name.SN_WEAK
"""
if set, make name weak
"""
SN_NON_WEAK = _ida_name.SN_NON_WEAK
"""
if set, make name non-weak
"""
SN_AUTO = _ida_name.SN_AUTO
"""
if set, make name autogenerated
"""
SN_NON_AUTO = _ida_name.SN_NON_AUTO
"""
if set, make name non-autogenerated
"""
SN_NOLIST = _ida_name.SN_NOLIST
"""
if set, exclude name from the list. if not set, then include the name
into the list (however, if other bits are set, the name might be
immediately excluded from the list).
"""
SN_NOWARN = _ida_name.SN_NOWARN
"""
don't display a warning if failed
"""
SN_LOCAL = _ida_name.SN_LOCAL
"""
create local name. a function should exist. local names can't be
public or weak. also they are not included into the list of names they
can't have dummy prefixes.
"""
SN_IDBENC = _ida_name.SN_IDBENC
"""
non-ASCII bytes will be decoded accordingly

the name is given in the IDB encoding;
"""
SN_FORCE = _ida_name.SN_FORCE
"""
if the specified name is already present in the database, try
variations with a numerical suffix like "_123"
"""

def force_name(*args):
  """
  force_name(ea, name, flags=0) -> bool
  """
  return _ida_name.force_name(*args)

def del_global_name(*args):
  """
  del_global_name(ea) -> bool
  """
  return _ida_name.del_global_name(*args)

def del_local_name(*args):
  """
  del_local_name(ea) -> bool
  """
  return _ida_name.del_local_name(*args)

def set_dummy_name(*args):
  """
  set_dummy_name(_from, ea) -> bool


  Give an autogenerated (dummy) name. Autogenerated names have special
  prefixes (loc_...).
  
  @param _from: linear address of the operand which references to the
                address (C++: ea_t)
  @param ea: linear address (C++: ea_t)
  """
  return _ida_name.set_dummy_name(*args)

def make_name_auto(*args):
  """
  make_name_auto(ea) -> bool
  """
  return _ida_name.make_name_auto(*args)

def make_name_user(*args):
  """
  make_name_user(ea) -> bool
  """
  return _ida_name.make_name_user(*args)
URK_NameChars = _ida_name.URK_NameChars
URK_MangleChars = _ida_name.URK_MangleChars
URK_StrlitChars = _ida_name.URK_StrlitChars
URK_TypeNameChars = _ida_name.URK_TypeNameChars
VNT_IDENT = _ida_name.VNT_IDENT
VNT_TYPE = _ida_name.VNT_TYPE
VNT_UDTMEM = _ida_name.VNT_UDTMEM
VNT_STRLIT = _ida_name.VNT_STRLIT
VNT_VISIBLE = _ida_name.VNT_VISIBLE

def is_valid_cp(*args):
  """
  is_valid_cp(cp, kind, data=None) -> bool


  Is the given codepoint acceptable in the given context?
  
  
  @param cp (C++: wchar32_t)
  @param kind (C++: nametype_t)
  @param data (C++: void *)
  """
  return _ida_name.is_valid_cp(*args)

def set_cp_validity(*args):
  """
  set_cp_validity(kind, cp, endcp=wchar32_t(-1), valid=True)


  Mark the given codepoint (or range) as acceptable or unacceptable in
  the given context If 'endcp' is not BADCP, it is considered to be the
  end of the range: [cp, endcp), and is not included in the range
  
  @param kind (C++: ucdr_kind_t)
  @param cp (C++: wchar32_t)
  @param endcp (C++: wchar32_t)
  @param valid (C++: bool)
  """
  return _ida_name.set_cp_validity(*args)

def is_ident_cp(*args):
  """
  is_ident_cp(cp) -> bool


  Can a character appear in a name? (present in ::NameChars or
  ::MangleChars)
  
  
  @param cp (C++: wchar32_t)
  """
  return _ida_name.is_ident_cp(*args)

def is_strlit_cp(*args):
  """
  is_strlit_cp(cp, specific_ranges=None) -> bool


  Can a character appear in a string literal (present in ::StrlitChars)
  If 'specific_ranges' are specified, those will be used instead of the
  ones corresponding to the current culture (only if ::StrlitChars is
  configured to use the current culture)
  
  @param cp (C++: wchar32_t)
  @param specific_ranges (C++: const  rangeset_crefvec_t  *)
  """
  return _ida_name.is_strlit_cp(*args)

def is_visible_cp(*args):
  """
  is_visible_cp(cp) -> bool


  Can a character be displayed in a name? (present in ::NameChars)
  
  
  @param cp (C++: wchar32_t)
  """
  return _ida_name.is_visible_cp(*args)

def is_ident(*args):
  """
  is_ident(name) -> bool


  Is a valid name? (including ::MangleChars)
  
  
  @param name (C++: const char *)
  """
  return _ida_name.is_ident(*args)

def is_uname(*args):
  """
  is_uname(name) -> bool


  Is valid user-specified name? (valid name & !dummy prefix).
  
  @param name: name to test. may be NULL. (C++: const char *)
  """
  return _ida_name.is_uname(*args)

def is_valid_typename(*args):
  """
  is_valid_typename(name) -> bool


  Is valid type name?
  
  @param name: name to test. may be NULL. (C++: const char *)
  """
  return _ida_name.is_valid_typename(*args)

def extract_name(*args):
  """
  extract_name(line, x) -> ssize_t


  Extract a name or address from the specified string.
  
  @param line: input string (C++: const char *)
  @param x: x coordinate of cursor (C++: int)
  @return: -1 if can not extract. otherwise length of the name
  """
  return _ida_name.extract_name(*args)

def hide_name(*args):
  """
  hide_name(ea)


  Remove name from the list of names
  
  @param ea: address of the name (C++: ea_t)
  """
  return _ida_name.hide_name(*args)

def show_name(*args):
  """
  show_name(ea)


  Insert name to the list of names.
  
  
  @param ea (C++: ea_t)
  """
  return _ida_name.show_name(*args)

def get_name_ea(*args):
  """
  get_name_ea(_from, name) -> ea_t


  Get address of the name. Dummy names (like byte_xxxx where xxxx are
  hex digits) are parsed by this function to obtain the address. The
  database is not consulted for them. This function works only with
  regular names.
  
  @param _from: linear address where the name is used. if not
                applicable, then should be  BADADDR . (C++: ea_t)
  @param name: any name in the program or NULL (C++: const char *)
  @return: address of the name or  BADADDR
  """
  return _ida_name.get_name_ea(*args)

def get_name_base_ea(*args):
  """
  get_name_base_ea(_from, to) -> ea_t


  Get address of the name used in the expression for the address
  
  @param _from: address of the operand which references to the address
                (C++: ea_t)
  @param to: the referenced address (C++: ea_t)
  @return: address of the name used to represent the operand
  """
  return _ida_name.get_name_base_ea(*args)

def get_name_value(*args):
  """
  get_name_value(_from, name) -> int


  Get value of the name. This function knows about: regular names,
  enums, special segments, etc.
  
  @param _from: linear address where the name is used if not applicable,
                then should be BADADDR (C++: ea_t)
  @param name: any name in the program or NULL (C++: const char *)
  @return: Name value result codes
  """
  return _ida_name.get_name_value(*args)
NT_NONE = _ida_name.NT_NONE
"""
name doesn't exist or has no value
"""
NT_BYTE = _ida_name.NT_BYTE
"""
name is byte name (regular name)
"""
NT_LOCAL = _ida_name.NT_LOCAL
"""
name is local label
"""
NT_STKVAR = _ida_name.NT_STKVAR
"""
name is stack variable name
"""
NT_ENUM = _ida_name.NT_ENUM
"""
name is symbolic constant
"""
NT_ABS = _ida_name.NT_ABS
"""
name is absolute symbol ( 'SEG_ABSSYM' )
"""
NT_SEG = _ida_name.NT_SEG
"""
name is segment or segment register name
"""
NT_STROFF = _ida_name.NT_STROFF
"""
name is structure member
"""
NT_BMASK = _ida_name.NT_BMASK
"""
name is a bit group mask name
"""
NT_REGVAR = _ida_name.NT_REGVAR
"""
name is a renamed register (*value is idx into pfn->regvars)
"""
GN_VISIBLE = _ida_name.GN_VISIBLE
"""
replace forbidden characters by SUBSTCHAR
"""
GN_COLORED = _ida_name.GN_COLORED
"""
return colored name
"""
GN_DEMANGLED = _ida_name.GN_DEMANGLED
"""
return demangled name
"""
GN_STRICT = _ida_name.GN_STRICT
"""
fail if can not demangle
"""
GN_SHORT = _ida_name.GN_SHORT
"""
use short form of demangled name
"""
GN_LONG = _ida_name.GN_LONG
"""
use long form of demangled name
"""
GN_LOCAL = _ida_name.GN_LOCAL
"""
try to get local name first; if failed, get global
"""
GN_ISRET = _ida_name.GN_ISRET
"""
for dummy names: use retloc
"""
GN_NOT_ISRET = _ida_name.GN_NOT_ISRET
"""
for dummy names: do not use retloc
"""
GN_NOT_DUMMY = _ida_name.GN_NOT_DUMMY
"""
do not return a dummy name
"""

def get_visible_name(*args):
  """
  get_visible_name(ea, gtn_flags=0) -> qstring
  """
  return _ida_name.get_visible_name(*args)

def get_short_name(*args):
  """
  get_short_name(ea, gtn_flags=0) -> qstring
  """
  return _ida_name.get_short_name(*args)

def get_long_name(*args):
  """
  get_long_name(ea, gtn_flags=0) -> qstring
  """
  return _ida_name.get_long_name(*args)

def get_colored_short_name(*args):
  """
  get_colored_short_name(ea, gtn_flags=0) -> qstring
  """
  return _ida_name.get_colored_short_name(*args)

def get_colored_long_name(*args):
  """
  get_colored_long_name(ea, gtn_flags=0) -> qstring
  """
  return _ida_name.get_colored_long_name(*args)

def get_demangled_name(*args):
  """
  get_demangled_name(ea, inhibitor, demform, gtn_flags=0) -> qstring
  """
  return _ida_name.get_demangled_name(*args)

def get_colored_demangled_name(*args):
  """
  get_colored_demangled_name(ea, inhibitor, demform, gtn_flags=0) -> qstring
  """
  return _ida_name.get_colored_demangled_name(*args)

def get_name_color(*args):
  """
  get_name_color(_from, ea) -> color_t


  Calculate flags for 'get_ea_name()' function.
  
  Get name color.
  
  @param _from: linear address where the name is used. if not
                applicable, then should be  BADADDR . The kernel returns
                a local name color if the reference is within a
                function, i.e. 'from' and 'ea' belong to the same
                function. (C++: ea_t)
  @param ea: linear address (C++: ea_t)
  """
  return _ida_name.get_name_color(*args)
GETN_APPZERO = _ida_name.GETN_APPZERO
"""
append a struct field name if the field offset is zero?

meaningful only if the name refers to a structure.
"""
GETN_NOFIXUP = _ida_name.GETN_NOFIXUP
"""
ignore the fixup information when producing the name
"""
GETN_NODUMMY = _ida_name.GETN_NODUMMY
"""
do not create a new dummy name but pretend it exists
"""

def get_name_expr(*args):
  """
  get_name_expr(_from, n, ea, off, flags=0x0001) -> ssize_t


  Convert address to name expression (name with a displacement). This
  function takes into account fixup information and returns a colored
  name expression (in the form <name> +/- <offset>). It also knows about
  structure members and arrays. If the specified address doesn't have a
  name, a dummy name is generated.
  
  @param _from: linear address of instruction operand or data referring
                to the name. This address will be used to get fixup
                information, so it should point to exact position of the
                operand in the instruction. (C++: ea_t)
  @param n: number of referencing operand. for data items specify 0
            (C++: int)
  @param ea: address to convert to name expression (C++: ea_t)
  @param off: the value of name expression. this parameter is used only
              to check that the name expression will have the wanted
              value. 'off' may be equal to BADADDR but this is
              discouraged because it prohibits checks. (C++: uval_t)
  @param flags: Name expression flags (C++: int)
  @return: < 0 if address is not valid, no segment or other failure.
           otherwise the length of the name expression in characters.
  """
  return _ida_name.get_name_expr(*args)

def get_nice_colored_name(*args):
  """
  get_nice_colored_name(ea, flags=0) -> ssize_t


  Get a nice colored name at the specified address. Ex:segment:sub+offse
  tsegment:sub:local_labelsegment:labelsegment:addresssegment:address+of
  fset
  
  @param ea (C++: ea_t)
  @param flags (C++: int)
  @return: the length of the generated name in bytes.
  """
  return _ida_name.get_nice_colored_name(*args)
GNCN_NOSEG = _ida_name.GNCN_NOSEG
"""
ignore the segment prefix when producing the name
"""
GNCN_NOCOLOR = _ida_name.GNCN_NOCOLOR
"""
generate an uncolored name
"""
GNCN_NOLABEL = _ida_name.GNCN_NOLABEL
"""
don't generate labels
"""
GNCN_NOFUNC = _ida_name.GNCN_NOFUNC
"""
don't generate funcname+... expressions
"""
GNCN_SEG_FUNC = _ida_name.GNCN_SEG_FUNC
"""
generate both segment and function names (default is to omit segment
name if a function name is present)
"""
GNCN_SEGNUM = _ida_name.GNCN_SEGNUM
"""
segment part is displayed as a hex number
"""
GNCN_REQFUNC = _ida_name.GNCN_REQFUNC
"""
return 0 if the address does not belong to a function
"""
GNCN_REQNAME = _ida_name.GNCN_REQNAME
"""
return 0 if the address can only be represented as a hex number
"""
GNCN_NODBGNM = _ida_name.GNCN_NODBGNM
"""
don't use debug names
"""
GNCN_PREFDBG = _ida_name.GNCN_PREFDBG
"""
if using debug names, prefer debug names over function names
"""

def append_struct_fields(*args):
  """
  append_struct_fields(disp, n, path, flags, delta, appzero) -> flags_t


  Append names of struct fields to a name if the name is a struct name.
  
  @param disp: displacement from the name (C++: adiff_t *)
  @param n: number of operand n which the name appears (C++: int)
  @param path: path in the struct. path is an array of id's. maximal
               length of array is  MAXSTRUCPATH . the first element of
               the array is the structure id. consecutive elements are
               id's of used union members (if any). (C++: const  tid_t
               *)
  @param flags: the input flags. they will be returned if the struct
                cannot be found. (C++: flags_t)
  @param delta: delta to add to displacement (C++: adiff_t)
  @param appzero: should append a struct field name if the displacement
                  is zero? (C++: bool)
  @return: flags of the innermost struct member or the input flags
  """
  return _ida_name.append_struct_fields(*args)

def is_public_name(*args):
  """
  is_public_name(ea) -> bool
  """
  return _ida_name.is_public_name(*args)

def make_name_public(*args):
  """
  make_name_public(ea)
  """
  return _ida_name.make_name_public(*args)

def make_name_non_public(*args):
  """
  make_name_non_public(ea)
  """
  return _ida_name.make_name_non_public(*args)

def is_weak_name(*args):
  """
  is_weak_name(ea) -> bool
  """
  return _ida_name.is_weak_name(*args)

def make_name_weak(*args):
  """
  make_name_weak(ea)
  """
  return _ida_name.make_name_weak(*args)

def make_name_non_weak(*args):
  """
  make_name_non_weak(ea)
  """
  return _ida_name.make_name_non_weak(*args)

def get_nlist_size(*args):
  """
  get_nlist_size() -> size_t


  Get number of names in the list.
  """
  return _ida_name.get_nlist_size(*args)

def get_nlist_idx(*args):
  """
  get_nlist_idx(ea) -> size_t


  Get index of the name in the listreturns the closest match. may return
  idx >= size.
  
  @param ea (C++: ea_t)
  """
  return _ida_name.get_nlist_idx(*args)

def is_in_nlist(*args):
  """
  is_in_nlist(ea) -> bool


  Is name included into names list?
  
  
  @param ea (C++: ea_t)
  """
  return _ida_name.is_in_nlist(*args)

def get_nlist_ea(*args):
  """
  get_nlist_ea(idx) -> ea_t


  Get address from the list at 'idx'.
  
  
  @param idx (C++: size_t)
  """
  return _ida_name.get_nlist_ea(*args)

def get_nlist_name(*args):
  """
  get_nlist_name(idx) -> char const *


  Get name using idx.
  
  
  @param idx (C++: size_t)
  """
  return _ida_name.get_nlist_name(*args)

def rebuild_nlist(*args):
  """
  rebuild_nlist()


  Rebuild names list.
  """
  return _ida_name.rebuild_nlist(*args)

def reorder_dummy_names(*args):
  """
  reorder_dummy_names()


  Renumber dummy names.
  """
  return _ida_name.reorder_dummy_names(*args)
DEBNAME_EXACT = _ida_name.DEBNAME_EXACT
DEBNAME_LOWER = _ida_name.DEBNAME_LOWER
DEBNAME_UPPER = _ida_name.DEBNAME_UPPER
DEBNAME_NICE = _ida_name.DEBNAME_NICE
class ea_name_t(object):
    """
    Proxy of C++ ea_name_t class
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    ea = _swig_property(_ida_name.ea_name_t_ea_get, _ida_name.ea_name_t_ea_set)
    name = _swig_property(_ida_name.ea_name_t_name_get, _ida_name.ea_name_t_name_set)
    def __init__(self, *args):
        """
        __init__(self) -> ea_name_t
        __init__(self, _ea, _name) -> ea_name_t
        """
        this = _ida_name.new_ea_name_t(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ida_name.delete_ea_name_t
    __del__ = lambda self : None;
ea_name_t_swigregister = _ida_name.ea_name_t_swigregister
ea_name_t_swigregister(ea_name_t)


def set_debug_name(*args):
  """
  set_debug_name(ea, name) -> bool
  """
  return _ida_name.set_debug_name(*args)

def get_debug_name(*args):
  """
  get_debug_name(ea_ptr, how) -> ssize_t
  """
  return _ida_name.get_debug_name(*args)

def del_debug_names(*args):
  """
  del_debug_names(ea1, ea2)
  """
  return _ida_name.del_debug_names(*args)

def get_debug_name_ea(*args):
  """
  get_debug_name_ea(name) -> ea_t
  """
  return _ida_name.get_debug_name_ea(*args)
DQT_NPURGED_8 = _ida_name.DQT_NPURGED_8
DQT_NPURGED_4 = _ida_name.DQT_NPURGED_4
DQT_NPURGED_2 = _ida_name.DQT_NPURGED_2
DQT_COMPILER = _ida_name.DQT_COMPILER
DQT_NAME_TYPE = _ida_name.DQT_NAME_TYPE
DQT_FULL = _ida_name.DQT_FULL

def demangle_name(*args):
  """
  demangle_name(name, disable_mask, demreq=DQT_FULL) -> int32


  Demangle a name.
  
  @param name: name to demangle (C++: const char *)
  @param disable_mask: bits to inhibit parts of demangled name (see
                       MNG_). by the M_COMPILER bits a specific compiler
                       can be selected (see MT_). (C++: uint32)
  @param demreq (C++: demreq_type_t)
  @return: ME_... or MT__ bitmasks from  demangle.hpp
  """
  return _ida_name.demangle_name(*args)

def is_name_defined_locally(*args):
  """
  is_name_defined_locally(pfn, name, ignore_name_def, ea1=BADADDR, ea2=BADADDR) -> bool


  Is the name defined locally in the specified function?
  
  @param pfn: pointer to function (C++: func_t  *)
  @param name: name to check (C++: const char *)
  @param ignore_name_def: which names to ignore when checking (C++:
                          ignore_name_def_t)
  @param ea1: the starting address of the range inside the function
              (optional) (C++: ea_t)
  @param ea2: the ending address of the range inside the function
              (optional) (C++: ea_t)
  @return: true if the name has been defined
  """
  return _ida_name.is_name_defined_locally(*args)

def cleanup_name(*args):
  """
  cleanup_name(ea, name, flags=0) -> bool
  """
  return _ida_name.cleanup_name(*args)
CN_KEEP_TRAILING__DIGITS = _ida_name.CN_KEEP_TRAILING__DIGITS

def get_debug_names(*args):
  """
    get_debug_names(names, ea1, ea2)
    get_debug_names(ea1, ea2) -> PyObject *
    """
  return _ida_name.get_debug_names(*args)

def get_ea_name(*args):
  """
  get_ea_name(ea, gtn_flags=0) -> qstring


  Get name at the specified address.
  
  @param ea: linear address (C++: ea_t)
  @param gtn_flags: how exactly the name should be retrieved.
                    combination of  bits for get_ea_name() function.
                    There is a convenience  bits (C++: int)
  @return: success
  """
  return _ida_name.get_ea_name(*args)

def validate_name(*args):
  """
  validate_name(name, type, flags=0) -> PyObject *


  Validate a name. This function replaces all invalid characters in the
  name with SUBSTCHAR. However, it will return false if name is valid
  but not allowed to be an identifier (is a register name).
  
  @param name: ptr to name. the name will be modified (C++: qstring  *)
  @param type: the type of name we want to validate (C++: nametype_t)
  @param flags: see SN_* . Only SN_IDBENC is currently considered (C++:
                int)
  @return: success
  """
  return _ida_name.validate_name(*args)
#<pycode(py_name)>
import _ida_idaapi
import _ida_funcs
import bisect


class NearestName(object):
    """
    Utility class to help find the nearest name in a given ea/name dictionary
    """
    def __init__(self, ea_names):
        self.update(ea_names)


    def update(self, ea_names):
        """
        Updates the ea/names map
        """
        self._names = ea_names
        self._addrs = ea_names.keys()
        self._addrs.sort()


    def find(self, ea):
        """
        Returns a tupple (ea, name, pos) that is the nearest to the passed ea
        If no name is matched then None is returned
        """
        pos = bisect.bisect_left(self._addrs, ea)
        # no match
        if pos >= len(self._addrs):
            return None
        # exact match?
        if self._addrs[pos] != ea:
            pos -= 1 # go to previous element
        if pos < 0:
            return None
        return self[pos]


    def _get_item(self, index):
        ea = self._addrs[index]
        return (ea, self._names[ea], index)


    def __iter__(self):
        return (self._get_item(index) for index in xrange(0, len(self._addrs)))


    def __getitem__(self, index):
        """
        Returns the tupple (ea, name, index)
        """
        if index > len(self._addrs):
            raise StopIteration
        return self._get_item(index)

def calc_gtn_flags(fromaddr, ea):
    """
    Calculate flags for get_ea_name() function

    @param fromaddr: the referring address. May be BADADDR.
    @param ea: linear address

    @return: flags
    """
    gtn_flags = 0
    if fromaddr != _ida_idaapi.BADADDR:
        pfn = _ida_funcs.get_func(fromaddr)
        if _ida_funcs.func_contains(pfn, ea):
            gtn_flags = GN_LOCAL
    return gtn_flags

#</pycode(py_name)>

if _BC695:
    GN_INSNLOC=0
    @bc695redef
    def demangle_name(name, mask, demreq=DQT_FULL): # make flag optional, so demangle_name & demangle_name2 can use it
        return _ida_name.demangle_name(name, mask, demreq)
    demangle_name2=demangle_name
    def do_name_anyway(ea, name, maxlen=0):
        return force_name(ea, name)
    extract_name2=extract_name
    get_debug_name2=get_debug_name
    def get_true_name(ea0, ea1=None):
        if ea1 is None:
            ea = ea0
        else:
            ea = ea1
        return get_name(ea)
    is_ident_char=is_ident_cp
    is_visible_char=is_visible_cp
    def make_visible_name(name, sz=0):
        if sz > 0:
            name = name[0:sz]
        return _ida_name.validate_name(name, VNT_VISIBLE)
    def validate_name2(name, sz=0):
        if sz > 0:
            name = name[0:sz]
        return _ida_name.validate_name(name, VNT_IDENT)
    def validate_name3(name):
        return _ida_name.validate_name(name, VNT_IDENT)
    isident=is_ident
    @bc695redef
    def get_name(*args):
        if len(args) == 2:
            if args[0] != _ida_idaapi.BADADDR:
                print("Compatibility get_name(from, ea) was called with non-BADADDR first argument (0x%08x). There is no equivalent in the new API, and the results might be erroneous." % args[0]);
            return _ida_name.get_name(args[1])
        else:
            return _ida_name.get_name(*args)



cvar = _ida_name.cvar
ignore_none = cvar.ignore_none
ignore_regvar = cvar.ignore_regvar
ignore_llabel = cvar.ignore_llabel
ignore_stkvar = cvar.ignore_stkvar
ignore_glabel = cvar.ignore_glabel
