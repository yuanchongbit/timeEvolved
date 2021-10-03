import idaapi
import idc
import idautils
from idautils import *
from idc import *
from idaapi import *
import csv


idaapi.autoWait()

name = GetInputFile()
path = GetInputFilePath()
res = []

for func in idautils.Functions():

    flags = idc.GetFunctionFlags(func)
    if flags & FUNC_LIB or flags & FUNC_THUNK:
		continue
    dism_addr = list(idautils.FuncItems(func))
    for line in dism_addr:
        m = idc.GetMnem(line)
        res.append(m)

opcode_str = ''
for i in res:
	opcode_str = opcode_str + i + ' '
opcode_str = opcode_str[ :-1]

file_path = 'C:\\Users\\bitcjx\\Desktop\\res.csv'
with open(file_path, 'ab') as f:
	writer = csv.writer(f)
	writer.writerow([name, path, opcode_str])
idc.Exit(0)