# a file loader for U-Boot "uImage" flash images
# Copyright (c) 2011-2015 Hex-Rays
# ALL RIGHTS RESERVED.

import idaapi
import idc
import zlib
import ida_idp

IH_TYPE_INVALID        = 0        # /* Invalid Image               */
IH_TYPE_STANDALONE     = 1        # /* Standalone Program          */
IH_TYPE_KERNEL         = 2        # /* OS Kernel Image             */
IH_TYPE_RAMDISK        = 3        # /* RAMDisk Image               */
IH_TYPE_MULTI          = 4        # /* Multi-File Image            */
IH_TYPE_FIRMWARE       = 5        # /* Firmware Image              */
IH_TYPE_SCRIPT         = 6        # /* Script file                 */
IH_TYPE_FILESYSTEM     = 7        # /* Filesystem Image (any type) */

ImageTypeNames = [ "Invalid", "Standalone Program", "OS Kernel", "RAMDisk",
                   "Multi-File", "Firmware", "Script file",  "Filesystem" ]

IH_CPU_INVALID          = 0       # /* Invalid CPU        */
IH_CPU_ALPHA            = 1       # /* Alpha        */
IH_CPU_ARM              = 2       # /* ARM                */
IH_CPU_I386             = 3       # /* Intel x86        */
IH_CPU_IA64             = 4       # /* IA64                */
IH_CPU_MIPS             = 5       # /* MIPS                */
IH_CPU_MIPS64           = 6       # /* MIPS         64 Bit */
IH_CPU_PPC              = 7       # /* PowerPC        */
IH_CPU_S390             = 8       # /* IBM S390        */
IH_CPU_SH               = 9       # /* SuperH        */
IH_CPU_SPARC            = 10      # /* Sparc        */
IH_CPU_SPARC64          = 11      # /* Sparc 64 Bit */
IH_CPU_M68K             = 12      # /* M68K                */
IH_CPU_NIOS             = 13      # /* Nios-32        */
IH_CPU_MICROBLAZE       = 14      # /* MicroBlaze   */
IH_CPU_NIOS2            = 15      # /* Nios-II        */

CPUNames = [ "Invalid", "Alpha", "ARM", "x86", "IA64", "MIPS", "MIPS64", "PowerPC",
             "IBM S390", "SuperH", "Sparc", "Sparc64", "M68K", "Nios-32", "MicroBlaze", "Nios-II"]

IDACPUNames = [ "", "alphab", "ARM", "metapc", "ia64b", "mipsl", "mipsl", "ppc",
               "", "SH4", "sparcb", "sparcb", "68K", "", "", ""]

IH_COMP_NONE            =   0     #  /*  No         Compression Used        */
IH_COMP_GZIP            =   1     #  /* gzip         Compression Used        */
IH_COMP_BZIP2           =   2     #  /* bzip2 Compression Used        */
IH_COMP_LZMA            =   3     #  /* lzma Compression Used        */

CompTypeNames = [ "", "gzip", "bzip2", "lzma" ]

IH_MAGIC = 0x27051956        # Image Magic Number
IH_NMLEN = 32                # Image Name Length

import ctypes

uint8_t  = ctypes.c_byte
uint32_t = ctypes.c_uint

class image_header(ctypes.BigEndianStructure):
    _fields_ = [
        ("ih_magic", uint32_t), #   Image Header Magic Number
        ("ih_hcrc",  uint32_t), #   Image Header CRC Checksum
        ("ih_time",  uint32_t), #   Image Creation Timestamp
        ("ih_size",  uint32_t), #   Image Data Size
        ("ih_load",  uint32_t), #   Data Load  Address
        ("ih_ep",    uint32_t), #   Entry Point Address
        ("ih_dcrc",  uint32_t), #   Image Data CRC Checksum
        ("ih_os",    uint8_t),  #   Operating System
        ("ih_arch",  uint8_t),  #   CPU architecture
        ("ih_type",  uint8_t),  #   Image Type
        ("ih_comp",  uint8_t),  #   Compression Type
        ("ih_name",  uint8_t * IH_NMLEN),  # Image Name
    ]
RomFormatName        = "U-Boot image"

# -----------------------------------------------------------------------
def dwordAt(li, off):
    li.seek(off)
    s = li.read(4)
    if len(s) < 4:
        return 0
    return struct.unpack('<I', s)[0]

def read_struct(li, struct):
    s = struct()
    slen = ctypes.sizeof(s)
    bytes = li.read(slen)
    fit = min(len(bytes), slen)
    ctypes.memmove(ctypes.addressof(s), bytes, fit)
    return s

# -----------------------------------------------------------------------
def accept_file(li, filename):
    """
    Check if the file is of supported format

    @param li: a file-like object which can be used to access the input data
    @param filename: name of the file, if it is an archive member name then the actual file doesn't exist
    @return: 0 - no more supported formats
             string "name" - format name to display in the chooser dialog
             dictionary { 'format': "name", 'options': integer }
               options: should be 1, possibly ORed with ACCEPT_FIRST (0x8000)
               to indicate preferred format
    """

    header = read_struct(li, image_header)
    # check the signature
    if header.ih_magic == IH_MAGIC:
        # accept the file
        t = header.ih_type
        c = header.ih_arch
        if t >= len(ImageTypeNames):
          t = "unknown type(%d)" % t
        else:
          t = ImageTypeNames[t]

        if c >= len(CPUNames):
          cname = "unknown CPU(%d)" % c
        else:
          cname = CPUNames[c]

        fmt = "%s (%s for %s)" % (RomFormatName, t, cname)
        comp = header.ih_comp
        if comp != IH_COMP_NONE:
          if comp >= len (CompTypeNames):
            cmpname = "unknown compression(%d)"
          else:
            cmpname = "%s compressed" % CompTypeNames[comp]
          fmt += " [%s]" % cmpname

        proc = ''
        if c < len(IDACPUNames):
          proc = IDACPUNames[c]

        return {'format': fmt, 'processor': proc}

    # unrecognized format
    return 0

# -----------------------------------------------------------------------
def load_file(li, neflags, format):

    """
    Load the file into database

    @param li: a file-like object which can be used to access the input data
    @param neflags: options selected by the user, see loader.hpp
    @return: 0-failure, 1-ok
    """

    if format.startswith(RomFormatName):
        li.seek(0)
        header = read_struct(li, image_header)
        c = header.ih_arch
        cname = IDACPUNames[c]
        if cname == "":
          idc.warning("Unsupported CPU")
          return

        if not header.ih_comp in (IH_COMP_NONE, IH_COMP_GZIP):
          idc.warning("Can only handle uncompressed or gzip-compressed images")
          return

        idaapi.set_processor_type(cname, ida_idp.SETPROC_LOADER)

        idc.AddSeg(header.ih_load, header.ih_load + header.ih_size, 0, 1, idaapi.saRelPara, idaapi.scPub)

        # copy bytes to the database

        if header.ih_comp  == IH_COMP_NONE:
          li.file2base(ctypes.sizeof(header), header.ih_load, header.ih_load + header.ih_size, 0)
        else:
          cdata = li.read(header.ih_size)
          d = zlib.decompressobj(zlib.MAX_WBITS|32)
          udata = d.decompress(cdata)
          udata += d.flush()
          # expand segment to fit uncompressed data
          idc.set_segment_bounds(header.ih_load, header.ih_load, header.ih_load+len(udata), idc.SEGMOD_KEEP)
          idaapi.put_bytes(header.ih_load, udata)

        if cname == "ARM" and (header.ih_ep & 1) != 0:
          # Thumb entry point
          header.ih_ep -= 1
          split_sreg_range(header.ih_ep, "T", 1)
        idaapi.add_entry(header.ih_ep, header.ih_ep, "start", 1)
        print "Load OK"
        return 1
