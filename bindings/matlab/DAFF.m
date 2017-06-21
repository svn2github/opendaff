classdef DAFF < handle
   properties
      daffhandle
   end
   methods
        function obj = DAFF( filepath )
            if( nargin > 0 )
                open( filepath )
            end
        end
        function idx = get_nearest_neighbour_index( obj, azi, ele )
            idx = DAFFv17( obj.daffhandle, 'getNearestNeighbourIndex', 'object', azi, ele );
        end
        function open( obj, filepath )
             obj.daffhandle = DAFFv17( 'open', filepath );
        end
   end
end
