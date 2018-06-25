# Add daffCppInterface module if it was not installed
import sys
sys.path.append( 'dist/Lib/site-packages' )

import daffCppInterface

d1 = daffCppInterface.open( '../../matlab/ExampleShortDiracOmni.ir.daff' )
d2 = daffCppInterface.open( '../../matlab/ExampleUnityOmni.ms.daff' )

index = daffCppInterface.nearest_neighbour_index(d1,1,2,3)
print( daffCppInterface.content_type( d1 ) )

daffCppInterface.close( d2 )
daffCppInterface.close( d1 )
