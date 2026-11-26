#!/usr/bin/env python3
"""Convert LUT Deluxe Ultimate's four-voice ZIP to Arduino UserBank.h.

Pure Python standard library. Compacting large audio banks is intentional;
Teensy 4.0 has finite flash and no PSRAM add-on in the Teensy 4.1 sense.

Usage: python tools/convert_ultimate_bank.py "FourVoiceExport.zip"
       python tools/convert_ultimate_bank.py "Export.zip" --frames 8 --samples 256
"""
import argparse
from array import array
from bisect import bisect_left
import json
import math
from pathlib import Path
import struct
import sys
import zipfile

TARGET_TOPS = [0,40,80,160,320,640,1280,2560,10240,20000]
MAX_USER_BYTES=380000

def clamp(v,lo,hi): return max(lo,min(hi,v))

def read_manifest(z):
    for name in z.namelist():
        if name.lower().endswith('.json'):
            try: v=json.loads(z.read(name))
            except (ValueError,UnicodeError): continue
            if isinstance(v,dict) and len(v.get('voices',[]))==4 and v.get('layout')=='[band][frame][sample]':
                return v
    raise ValueError('No LUT Deluxe Ultimate four-voice manifest found in ZIP')

def decode_u16(content):
    if len(content)%2: raise ValueError('Odd binary sample length')
    buf=array('H');buf.frombytes(content)
    if sys.byteorder!='little':buf.byteswap()
    return buf

def periodic_resample(arr, idx, original_len, new_len):
    # Periodic linear resampling, supported when original <= 2048 samples.
    p=(idx*original_len)/new_len
    j=int(p);f=p-j
    return arr[j % original_len]*(1-f)+arr[(j+1)%original_len]*f

def interpolate_frame(data, source_bands, source_frames, source_samples, band, frame_pos, out_sample, out_samples):
    f0=min(int(frame_pos),source_frames-1)
    f1=min(f0+1,source_frames-1)
    blend=frame_pos-f0
    off0=(band*source_frames+f0)*source_samples
    off1=(band*source_frames+f1)*source_samples
    a=periodic_resample(data[off0:off0+source_samples],out_sample,source_samples,out_samples)
    b=periodic_resample(data[off1:off1+source_samples],out_sample,source_samples,out_samples)
    return a+(b-a)*blend

def choose_band(src_tops,target):
    if target==0:return 0
    for i in range(1,len(src_tops)):
        top=src_tops[i]
        if top is not None and top>=target:return i
    return max(0,len(src_tops)-1)

def only_fundamental(codes):
    """Retain DC and the first Fourier partial at the top 20 kHz pitch band.

    At 48 kHz playback, the second harmonic of 20 kHz would alias. Apply
    this irrespective of the source bank's published highest pitch band.
    """
    n = len(codes)
    mean = sum(codes) / n
    cos_term = 2.0 / n * sum((v - mean) * math.cos(2.0 * math.pi * i / n)
                               for i, v in enumerate(codes))
    sin_term = 2.0 / n * sum((v - mean) * math.sin(2.0 * math.pi * i / n)
                               for i, v in enumerate(codes))
    return [int(round(clamp(mean + cos_term * math.cos(2.0 * math.pi * i / n)
                            + sin_term * math.sin(2.0 * math.pi * i / n), 0, 4095)))
            for i in range(n)]


def write_header(path, bank, ratios, phases, tops, frames, samples, code_max):
    nbytes=len(bank)*2
    if nbytes>MAX_USER_BYTES: raise ValueError(
        f'User bank is {nbytes} bytes. Reduce --frames, --samples or --bands (limit {MAX_USER_BYTES}).')
    with path.open('w',encoding='utf-8',newline='\n') as f:
        f.write('// Auto-generated from LUT Deluxe Ultimate four-voice ZIP.\n'
                '#pragma once\n#include <Arduino.h>\n'
                '#define USER_BANK_AVAILABLE 1\n'
                f'static const uint16_t USER_BANK_FRAMES = {frames};\n'
                f'static const uint16_t USER_BANK_SAMPLES = {samples};\n'
                f'static const uint16_t USER_BANK_BANDS = {len(tops)};\n'
                f'static const uint16_t USER_BANK_MAXCODE = {code_max};\n'
                'static const uint32_t USER_BANK_LIMITS[] = {'+', '.join(map(str,tops))+'};\n'
                'static const float USER_BANK_RATIOS[4] = {'+', '.join(f'{v:.9f}f' for v in ratios)+'};\n'
                'static const float USER_BANK_PHASES[4] = {'+', '.join(f'{v:.9f}f' for v in phases)+'};\n'
                'static const uint16_t USER_BANK_DATA[] PROGMEM = {\n')
        for i in range(0,len(bank),24):
            f.write('  '+', '.join(map(str,bank[i:i+24]))+',\n')
        f.write('};\n')
    return nbytes

def convert(archive,output,frames=8,samples=256,bands=10):
    if samples not in (128,256,512,1024):raise ValueError('Samples must be 128, 256, 512 or 1024')
    if not 2<=frames<=32:raise ValueError('Frames must be between 2 and 32')
    if not 1<=bands<=len(TARGET_TOPS):raise ValueError(f'Bands must be 1..{len(TARGET_TOPS)}')
    with zipfile.ZipFile(archive) as z:
        manifest=read_manifest(z)
        sf=int(manifest['frames']);ss=int(manifest['samples'])
        src_tops=manifest['bandUpperHz'];src_bands=len(src_tops)
        if not 2<=sf<=256 or not 32<=ss<=4096 or ss&(ss-1):
            raise ValueError('Invalid source dimensions')
        if src_bands<1 or src_bands>16:raise ValueError('Unexpected number of bands')
        if src_bands<2 and bands>1:raise ValueError('Export pitch-aware/mipmap bands from LUT Deluxe, or use --bands 1 (unfiltered, alias-prone)')
        src_max=int(manifest.get('maxCode',4095))
        if src_max not in (3090,4095,65535):raise ValueError('Unrecognized source code range')
        requested=TARGET_TOPS[:bands]
        # Coarse conversion never labels a bank as narrower-band than the source.
        indices=[choose_band(src_tops,h) for h in requested]
        tops=[0]+[20000 if target >= 20000 else int(src_tops[i])
                  for target, i in zip(requested[1:], indices[1:])]
        bank=[];ratios=[];phases=[]
        for voice in manifest['voices']:
            path=voice['bin']; data=decode_u16(z.read(path))
            expected=src_bands*sf*ss
            if len(data)!=expected:raise ValueError(f'{path}: expected {expected} codes, found {len(data)}')
            ratio=float(voice.get('ratio',1)) * 2**(float(voice.get('octave',0)) +
                          float(voice.get('semitones',0))/12 + float(voice.get('detune',0))/1200)
            if not math.isfinite(ratio) or ratio<=0:ratio=1
            ratios.append(clamp(ratio,.01,32))
            phase=float(voice.get('phase',0))
            phases.append(phase%1 if math.isfinite(phase) else 0)
            for target_band, source_band in zip(requested, indices):
                for frame in range(frames):
                    pos=frame*(sf-1)/(frames-1)
                    codes=[]
                    for i in range(samples):
                        raw=interpolate_frame(data,src_bands,sf,ss,source_band,pos,i,samples)
                        # Convert future 16-bit exports down to original 12-bit DAC.
                        code=round(clamp(raw,0,src_max)*4095/src_max)
                        codes.append(int(clamp(code,0,4095)))
                    if target_band >= 20000:
                        codes=only_fundamental(codes)
                    bank.extend(codes)
    nbytes=write_header(output,bank,ratios,phases,tops,frames,samples,4095)
    return {'name':manifest.get('name'),'bytes':nbytes,'frames':frames,'samples':samples,
            'bands':bands,'source_bands':src_bands,'ratios':ratios}

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('zip',type=Path,help='LUT Deluxe Ultimate Four-Voice Teensy ZIP')
    p.add_argument('-o','--output',type=Path,default=Path(__file__).resolve().parents[1]/'UserBank.h')
    p.add_argument('--frames',type=int,default=8)
    p.add_argument('--samples',type=int,default=256)
    p.add_argument('--bands',type=int,default=10)
    args=p.parse_args()
    try:
        result=convert(args.zip,args.output,args.frames,args.samples,args.bands)
    except (ValueError,KeyError,OSError,zipfile.BadZipFile) as e:
        p.error(str(e))
    print(f"Wrote {args.output} ({result['bytes']:,} raw table bytes, {result['bands']} bands, "
          f"{result['frames']} frames, {result['samples']} samples, 4 voices).")
    print('Ratios:',', '.join(f'{r:.4f}' for r in result['ratios']))
    print('Compile V2.01 again in Arduino IDE to embed the new bank in Teensy flash.')

if __name__=='__main__':main()
