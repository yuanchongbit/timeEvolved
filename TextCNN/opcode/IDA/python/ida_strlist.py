# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.




"""
IDA Plugin SDK API wrapper: strlist
"""


from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ida_strlist', [dirname(__file__)])
        except ImportError:
            import _ida_strlist
            return _ida_strlist
        if fp is not None:
            try:
                _mod = imp.load_module('_ida_strlist', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ida_strlist = swig_import_helper()
    del swig_import_helper
else:
    import _ida_strlist
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

class strwinsetup_t(object):
    """
    Proxy of C++ strwinsetup_t class
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args):
        """
        __init__(self) -> strwinsetup_t
        """
        this = _ida_strlist.new_strwinsetup_t(*args)
        try: self.this.append(this)
        except: self.this = this
    minlen = _swig_property(_ida_strlist.strwinsetup_t_minlen_get, _ida_strlist.strwinsetup_t_minlen_set)
    display_only_existing_strings = _swig_property(_ida_strlist.strwinsetup_t_display_only_existing_strings_get, _ida_strlist.strwinsetup_t_display_only_existing_strings_set)
    only_7bit = _swig_property(_ida_strlist.strwinsetup_t_only_7bit_get, _ida_strlist.strwinsetup_t_only_7bit_set)
    ignore_heads = _swig_property(_ida_strlist.strwinsetup_t_ignore_heads_get, _ida_strlist.strwinsetup_t_ignore_heads_set)
    def is_initialized(self, *args):
        """
        is_initialized(self) -> bool
        """
        return _ida_strlist.strwinsetup_t_is_initialized(self, *args)

    def _get_strtypes(self, *args):
        """
        _get_strtypes(self) -> PyObject *
        """
        return _ida_strlist.strwinsetup_t__get_strtypes(self, *args)

    def _set_strtypes(self, *args):
        """
        _set_strtypes(self, py_t) -> PyObject *
        """
        return _ida_strlist.strwinsetup_t__set_strtypes(self, *args)

    strtypes = property(_get_strtypes, _set_strtypes)

    __swig_destroy__ = _ida_strlist.delete_strwinsetup_t
    __del__ = lambda self : None;
strwinsetup_t_swigregister = _ida_strlist.strwinsetup_t_swigregister
strwinsetup_t_swigregister(strwinsetup_t)

class string_info_t(object):
    """
    Proxy of C++ string_info_t class
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    ea = _swig_property(_ida_strlist.string_info_t_ea_get, _ida_strlist.string_info_t_ea_set)
    length = _swig_property(_ida_strlist.string_info_t_length_get, _ida_strlist.string_info_t_length_set)
    type = _swig_property(_ida_strlist.string_info_t_type_get, _ida_strlist.string_info_t_type_set)
    def __init__(self, *args):
        """
        __init__(self) -> string_info_t
        __init__(self, _ea) -> string_info_t
        """
        this = _ida_strlist.new_string_info_t(*args)
        try: self.this.append(this)
        except: self.this = this
    def __lt__(self, *args):
        """
        __lt__(self, r) -> bool
        """
        return _ida_strlist.string_info_t___lt__(self, *args)

    __swig_destroy__ = _ida_strlist.delete_string_info_t
    __del__ = lambda self : None;
string_info_t_swigregister = _ida_strlist.string_info_t_swigregister
string_info_t_swigregister(string_info_t)


def get_strlist_options(*args):
  """
  get_strlist_options() -> strwinsetup_t


  Get access to the static string list options.
  """
  return _ida_strlist.get_strlist_options(*args)

def build_strlist(*args):
  """
  build_strlist()


  Build the string list. You should initialize options before this call
  using the restore_config() or setup_strings_window() methods.
  """
  return _ida_strlist.build_strlist(*args)

def clear_strlist(*args):
  """
  clear_strlist()


  Clear the string list.
  """
  return _ida_strlist.clear_strlist(*args)

def get_strlist_qty(*args):
  """
  get_strlist_qty() -> size_t


  Get number of elements in the string list.
  """
  return _ida_strlist.get_strlist_qty(*args)

def get_strlist_item(*args):
  """
  get_strlist_item(si, n) -> bool


  Get nth element of the string list (n=0.. 'get_strlist_qty()' -1)
  
  
  @param si (C++: string_info_t  *)
  @param n (C++: size_t)
  """
  return _ida_strlist.get_strlist_item(*args)
if _BC695:
    def refresh_strlist(*args):
        build_strlist()



