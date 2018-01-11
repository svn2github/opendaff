
# OpenDAFF - A free, open source software package for directional audio data

## License 

Copyright 2016 - 2018 Institute of Technical Acoustics, RWTH Aachen University

Licensed under the Apache License, Version 2.0 (the "License");
you may not use the OpenDAFF software package except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

You should also have received a [copy of the License](LICENSE.md) with the OpenDAFF software package.


## File format description

### File header

Struct: DAFFFileHeader
Static: yes
Size: 2+4+4 = 10 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
2 | character | Signature | 'FW'
4 | integer | FileFormatVersion | == 105
4 | integer | NumFileBlocks | Number of subsequent file blocks

### File block table

Table of descriptors for the file blocks (like main header, content header, etc)

Struct: DAFFFileBlockEntry
Static: no
Size = (4+4+4) x NumFileBlocks

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 | integer | ID |
4 | integer | Offset | Relative to beginning of file
4 | integer | Size | 

#### Main header

Struct: DAFFMainHeader
Static: yes
Size: 15 x 4 = 60 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | integer | ContentType | IR or MS or ...
4 bytes | integer | Quantization | int16, float, etc
4 bytes | integer | NumChannels |
4 bytes | integer | NumRecords | Number of individual data sets
4 bytes | integer | ElementsPerRecord |
4 bytes | integer | MetadataIndex | Index of metadata set for general information
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

Struct: DAFFContentHeaderIR
Static: yes
Size: 4 + 4 + 4 = 12 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | float | SampleRate | Sampling rate
4 bytes | integer | MinFilterOffset | Guaranteed leading zeros in all records
4 bytes | integer | MaxEffectiveFilterLength | Maximum number of non-zero samples within all records

##### MS

Struct: DAFFContentHeaderMS
Static: yes
Size: 4 + 4 = 8 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | float | Max | Maximum value of all records
4 bytes | integer | NumFreqs | Number of frequencies

##### PS

Struct: DAFFContentHeaderMS
Static: yes
Size: 4 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | integer | NumFreqs | Number of frequencies

##### MPS

Struct: DAFFContentHeaderMPS
Static: yes
Size: 4 + 4 = 8 bytes

Bytes | Type | Name | Notes
--- | --- | --- | ---
4 bytes | float | Max | Maximum value of all records
4 bytes | integer | NumFreqs | Number of frequencies

##### DFT

Struct: DAFFContentHeaderDFT
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

Binary data depending on number and individual size of record data sets. Accessed via DataOffset.

#### Metadata

Binary data depending on number and size of metadata. Accessed via MetadataIndex.

