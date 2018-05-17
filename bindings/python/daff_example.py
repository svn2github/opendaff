# Add daff module if it was not installed
import sys
sys.path.append( 'dist/Lib/site-packages' )

import daff

d1 = daff.open( '../../matlab/ExampleUnityOmni.ms.v17.daff' )
d2 = daff.open( '../../matlab/ExampleUnityOmni.ms.v17.daff' )

print( daff.content_type( d1 ) )

daff.close( d2 )
daff.close( d1 )
