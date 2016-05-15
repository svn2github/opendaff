
# OpenDAFF - A free, open source software package for directional audio data

OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL) with an exception for static linking permission.

Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
Visit the OpenDAFF homepage: http://www.opendaff.org


## File format description

### File header

Class: DAFFFileHeader
Static: yes
Size: 2+4+4 = 10 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
2 | character | Signature | 'FW'
4 | integer | FileFormatVersion | == 105
4 | integer | NumFileBlocks | Number of subsequent file blocks

### File block table

Table of descriptors for the file blocks (like main header, content header, etc)

Class: DAFFFileBlockEntry
Static: no
Size = (4+4+4) x NumFileBlocks

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 | integer | ID |
4 | integer | Offset | Relative to beginning of file
4 | integer | Size | 

#### Main header

Class: DAFFMainHeader
Static: yes
Size: 15 x 4 = 60 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | integer | ContentType | IR or MS or ...
4 bytes | integer | Quantization | int16, float, etc
4 bytes | integer | NumChannels |
4 bytes | integer | NumRecords | Number of individual data sets
4 bytes | integer | ElementsPerRecord |
4 bytes | integer | MetadataIndex | Index of metadata set in metadata table for record data sets
4 bytes | integer | AlphaPoints |
4 bytes | float | AlphaStart |
4 bytes | float | fAlphaEnd |
4 bytes | integer |BetaPoints |
4 bytes | float | BetaStart |
4 bytes | float | fBetaEnd |
4 bytes | float | OrientYaw |
4 bytes | float | OrientPitch |
4 bytes | float | OrientRoll |


#### Content header

##### IR

Class: DAFFContentHeaderIR
Static: yes
Size: 4 + 4 + 4 = 12 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | float | SampleRate | Sampling rate
4 bytes | integer | MinFilterOffset | Guaranteed leading zeros in all records
4 bytes | integer | MaxEffectiveFilterLength | Maximum number of non-zero samples within all records

##### MS

Class: DAFFContentHeaderMS
Static: yes
Size: 4 + 4 = 8 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | float | Max | Maximum value of all records
4 bytes | integer | NumFreqs | Number of frequencies

##### PS

Class: DAFFContentHeaderMS
Static: yes
Size: 4 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | integer | NumFreqs | Number of frequencies

##### MPS

Class: DAFFContentHeaderMPS
Static: yes
Size: 4 + 4 = 8 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | float | Max | Maximum value of all records
4 bytes | integer | NumFreqs | Number of frequencies

##### DFT

Class: DAFFContentHeaderDFT
Static: yes
Size: 4 + 4 + 4 + 4 = 16 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | integer | NumDFTCoeffs | Number of (stored) DFT coefficients (= complex elements per record)
4 bytes | integer | TransformSize | DFT transform size
4 bytes | float | SampleRate | Sampling rate
4 bytes | float | Max | Maximum value of all records


#### Record descriptor block

##### IR

Struct: DAFFRecordChannelDescIR
Static: yes
Size: 4 + 4 + 4 + 8 = 24 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | integer | Offset | ?
4 bytes | integer | Length | ?
4 bytes | float | Scaling | Scaling factor (only used for integer quantization)
8 bytes | unsigned integer | DataOffset | osition inside the file where samples/coefficients reside

##### MS, PS, MPS, DFT

Struct: DAFFRecordChannelDescDefault
Static: yes
Size: 8 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
8 bytes | unsigned integer | DataOffset | 

#### Data block

