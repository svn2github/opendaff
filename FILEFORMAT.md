
# OpenDAFF - A free, open source software package for directional audio data

OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL) with an exception for static linking permission.

Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
Visit the OpenDAFF homepage: http://www.opendaff.org


## File format description

### File header

Type: DAFFFileHeader
Static: yes
Size: 10 bytes

2 bytes | 4 bytes | 4 bytes
character | integer | integer
Signature | FileFormatVersion | NumFileBlock

### File block table

Table of descriptors for the file blocks (like main header, content header, etc)

Type: DAFFFileBlockEntry
Static: yes
Size: 20 bytes

4 bytes | 8 bytes | 8 bytes
integer | integer | integer
ID | Offset | Size

#### Main header

Type: DAFFMainHeader
Static: yes
Size: 

4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes
integer | integer | integer | integer | integer | integer | integer | float | float | integer | float | float | float | float | float
ContentType | Quantization | NumChannels | NumRecords | ElementsPerRecord | MetadataIndex | AlphaPoints | AlphaStart| fAlphaEnd | BetaPoints | BetaStart | fBetaEnd | OrientYaw | OrientPitch | OrientRoll

#### Content header

#### Record descriptor block

#### Data block

