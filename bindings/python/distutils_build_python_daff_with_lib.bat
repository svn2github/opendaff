python.exe setup_with_lib.py clean
python.exe setup_with_lib.py build --force
python.exe setup_with_lib.py install --prefix dist
python.exe setup_with_lib.py sdist --formats=zip
rem later:
rem python.exe setup_with_lib.py bdist_wininst
python.exe setup_with_lib.py check
