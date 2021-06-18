from __future__ import print_function
import idaapi
import idc


def imp_cb(ea, name, orda):
    
    if not name:
        return True
    else:
        wristr="%08x: %s (ord#%d)" % (ea, name, orda)
        with open('C:\\Users\\admin\\Desktop\\res.txt','a+') as f:
            f.write(wristr+'\n')
    return True


idaapi.autoWait()
nimps = idaapi.get_import_module_qty()

for i in xrange(0, nimps):
    name = idaapi.get_import_module_name(i)
    if not name:
        continue
    with open('C:\\Users\\admin\\Desktop\\res.txt','a+') as f:
        f.write('---module_name:'+name+'----'+'\n')
    idaapi.enum_import_names(i, imp_cb)
    with open('C:\\Users\\admin\\Desktop\\res.txt','a+') as f:
        f.write("----\n")
with open('C:\\Users\\admin\\Desktop\\res.txt','a+') as f:
    f.write("\n")    

idc.Exit(0)
