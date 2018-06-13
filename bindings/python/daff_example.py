# Add daff module if it was not installed
import sys
sys.path.append( 'dist/Lib/site-packages' )

import daff

d1 = daff.open( 'ExampleUnityOmni.ms.daff' )
d2 = daff.open( 'ExampleUnityOmni.ms.daff' )


index = daff.find_nearest_neighbour_index(d1,1,2,3)
print( daff.content_type( d1 ) )




daff.close( d2 )
daff.close( d1 )
