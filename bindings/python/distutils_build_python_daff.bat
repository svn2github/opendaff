python.exe setup.py clean
python.exe setup.py build --force
python.exe setup.py install --prefix dist
python.exe setup.py sdist --formats=zip
rem later:
rem python.exe setup.py bdist_wininst
python.exe setup.py check
