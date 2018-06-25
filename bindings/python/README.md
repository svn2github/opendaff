
# OpenDAFF - A free, open source software package for directional audio data


## License 

Copyright 2016 - 2018 Institute of Technical Acoustics, RWTH Aachen University

Licensed under the Apache License, Version 2.0 (the "License");
you may not use the OpenDAFF software package except in compliance with the License.
You may obtain a copy of the License at

<http://www.apache.org/licenses/LICENSE-2.0>

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

You should also have received a [copy of the License](LICENSE.md) with the OpenDAFF software package.


## Python bindings

CMake generates python setup scripts for the `distutils` package manager.
To create the Python bindings and let the package manager compile the C++ code, run `python setup.py build`. In case you want to link against the DAFF library that has already been compiled, run `python setup_with_lib.py build`. In case you want to install the scripts into your system, set the target to `install`.
