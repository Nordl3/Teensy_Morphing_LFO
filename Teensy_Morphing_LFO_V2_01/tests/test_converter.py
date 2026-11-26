#!/usr/bin/env python3
"""Standard-library test for LUT Deluxe Ultimate export conversion."""
import json
import math
from pathlib import Path
import struct
import tempfile
import zipfile
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from convert_ultimate_bank import convert, only_fundamental

def test():
    with tempfile.TemporaryDirectory() as temp:
        root=Path(temp);zip_file=root/'ultimate.zip'
        frames,samples,bands=16,512,4
        tops=[None,80,320,10240]
        manifest={'name':'Test Bank','version':2,'voices':[],'frames':frames,'samples':samples,
                  'layout':'[band][frame][sample]','bandUpperHz':tops,'maxCode':3090}
        with zipfile.ZipFile(zip_file,'w') as out:
            for ch in range(4):
                data=[]
                for b in range(bands):
                    for f in range(frames):
                        for i in range(samples):
                            value=(math.sin(2*math.pi*(i/samples)+(ch*math.pi/2)) + 1)*1545
                            data.append(int(round(value)))
                filename=f'test_{ch}.bin'
                out.writestr(filename,struct.pack('<'+'H'*len(data),*data))
                manifest['voices'].append({'name':f'Voice {ch+1}','bin':filename,'ratio':1+ch*.5,
                                           'phase':ch*.25,'octave':0,'semitones':0,'detune':0})
            out.writestr('TestBank.json',json.dumps(manifest))
        output=root/'UserBank.h'
        m=convert(zip_file,output,frames=8,samples=256,bands=9)
        assert m['bytes']==4*8*9*256*2
        code=output.read_text()
        assert '#define USER_BANK_AVAILABLE 1' in code
        assert 'USER_BANK_RATIOS[4]' in code
        assert 'static const uint16_t USER_BANK_BANDS = 9;' in code
        assert '{0, 80, 80, 320, 320, 10240, 10240, 10240, 10240}' in code
        assert 'USER_BANK_DATA[] PROGMEM' in code
        assert not any(int(x)>4095 for x in __import__('re').findall(r'\b\d+\b',code.split('USER_BANK_DATA[] PROGMEM')[1]))
        # Default conversion now includes a guaranteed clean 20 kHz one-harmonic band.
        r=convert(zip_file,output,frames=8,samples=256)
        assert r['bands']==10
        assert r['bytes']==4*10*8*256*2
        code=output.read_text()
        assert 'USER_BANK_BANDS = 10' in code
        assert code.split('USER_BANK_LIMITS[] = {')[1].split('}')[0].strip().endswith('20000')
        # Confirm the reconstructed high-pitch band does not retain significant 2nd harmonics.
        w=[2047 + 900*math.sin(2*math.pi*i/256) + 700*math.sin(4*math.pi*i/256) for i in range(256)]
        a=only_fundamental(w)
        assert max(a)<4096 and min(a)>=0
        second=sum((v-2047)*math.sin(4*math.pi*i/256) for i,v in enumerate(a))
        assert abs(second)<10.0
        print('PASS: Ultimate ZIP -> 20 kHz safe four-channel wavetable, pitch bands, metadata')
if __name__=='__main__':test()
