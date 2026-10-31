"""Minimal PNG reader for emulator screenshots: rows of 5-bit (r, g, b) tuples, as on the GBA."""
import struct,zlib


def read_png(path):
 data=path.read_bytes();i=8;idat=b''
 while i<len(data):
  n=struct.unpack('>I',data[i:i+4])[0];kind=data[i+4:i+8];chunk=data[i+8:i+8+n];i+=12+n
  if kind==b'IHDR':width,height,_,color=struct.unpack('>IIBB',chunk[:10])
  elif kind==b'IDAT':idat+=chunk
 bpp={2:3,6:4}[color];raw=zlib.decompress(idat);stride=width*bpp;rows=[];prev=bytearray(stride);o=0
 for _ in range(height):
  kind=raw[o];line=bytearray(raw[o+1:o+1+stride]);o+=1+stride
  for x in range(stride):
   a=line[x-bpp] if x>=bpp else 0;b=prev[x];c=prev[x-bpp] if x>=bpp else 0
   if kind==1:line[x]=(line[x]+a)&255
   elif kind==2:line[x]=(line[x]+b)&255
   elif kind==3:line[x]=(line[x]+(a+b)//2)&255
   elif kind==4:pa,pb,pc=abs(b-c),abs(a-c),abs(a+b-2*c);line[x]=(line[x]+(a if pa<=pb and pa<=pc else b if pb<=pc else c))&255
  rows.append([tuple(v>>3 for v in line[x*bpp:x*bpp+3]) for x in range(width)]);prev=line
 return rows
