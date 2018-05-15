# Add daff module if it was not installed
import sys
sys.path.append( 'dist/Lib/site-packages' )

import daff

x = daff.open( 'example.daff' )
print( x )
daff.close( x )

